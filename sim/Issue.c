#include "Issue.h"
#include "Queue.h"

#define IRRELEVANT (-1)

extern RS rs_add[MAX_CONFIG_SIZE];
extern RS rs_mul[MAX_CONFIG_SIZE];
extern RS rs_div[MAX_CONFIG_SIZE]; 
extern char rs_add_names[MAX_CONFIG_SIZE][NAME_LEN];
extern char rs_mul_names[MAX_CONFIG_SIZE][NAME_LEN];
extern char rs_div_names[MAX_CONFIG_SIZE][NAME_LEN];
extern config_args* _config_args_read;
extern RAT_entry RAT[NUM_OF_REGS];
extern float _regs[NUM_OF_REGS];
extern inst_queue* _iq_arr;
extern unsigned int _cycles;

extern load_buffer load_buffers[MAX_CONFIG_SIZE];
extern store_buffer store_buffers[MAX_CONFIG_SIZE];
extern bool used_memory_port_in_current_cycle;
extern bool received_halt_in_fetch; //means to stop do fetches
extern bool finished_issue; //means to stop handle issues


bool issue_instruction();
bool issue_memory_instruction(inst* inst);
int get_free_reservation_station_index(int opcode);
int get_specific_free_reservation_station_index(RS rses[], int num_rses);
int put_inst_in_RS(inst* instr);
void put_inst_in_specific_rs(RS res_stations[], int free_station_index, inst* instr);
void update_load_buffer(int index, inst* inst);
void update_store_buffer(int index, inst* inst);
int check_available_load_buffer();
int check_available_store_buffer();

//these 2 methods are used to correctly simulate the concurrency of dispatch and issue units, meaning
//we can't issue an instruction to an RS at the same cycle that the RS got vacated.
void enable_just_dispatched_rses();
void enable_just_dispatched_specific_rses(RS rses[], int num_rses);


void Issue()
{
	used_memory_port_in_current_cycle = false;
	if (received_halt_in_fetch && finished_issue)
	{
		return;
	}

	// try to issue an instruction
	if (issue_instruction())
	{
		// only if we succeeded, try issuing another
		issue_instruction();
	}
	enable_just_dispatched_rses();
}

bool issue_instruction()
{
	inst* instr = peek_queue_tail();
	if (instr == NULL)
	{
		return false; //will happen when queue is empty
	}
	if (instr->opcode == HALT_opcode)
	{
		finished_issue = true;
		return false;
	}
	int issued_successfully = NO_INSTANCE_AVAILABLE;
	if (instr->opcode == LD_opcode || instr->opcode == ST_opcode)
	{
		if(!used_memory_port_in_current_cycle)
		{
			issued_successfully = issue_memory_instruction(instr);
		}
	}

	else
	{
		issued_successfully = put_inst_in_RS(instr);
	}


	if (issued_successfully == NO_INSTANCE_AVAILABLE)
	{
		return false;
	}
	else
	{
		dequeue(_iq_arr);
	}
	return true;
}

bool issue_memory_instruction(inst* inst)
{
	int index;
	switch (inst->opcode)
	{
	case LD_opcode:
		index = check_available_load_buffer();
		if(index != NO_INSTANCE_AVAILABLE)
		{
			used_memory_port_in_current_cycle = true;
			update_load_buffer(index, inst);
		}
		break;
	case ST_opcode:
		index = check_available_store_buffer();
		if (index != NO_INSTANCE_AVAILABLE)
		{
			used_memory_port_in_current_cycle = true;
			update_store_buffer(index, inst);
		}
		break;
	default:
		index = NO_INSTANCE_AVAILABLE;
	}

	return index;
}

int check_available_load_buffer()
{
	for (int i = 0; i < _config_args_read->mem_nr_load_buffers; i++)
	{
		if (load_buffers[i].timer == INSTANCE_IS_FREE && !load_buffers[i].just_broadcasted)
		{
			return i;
		}
	}
	return NO_INSTANCE_AVAILABLE;
}

int check_available_store_buffer()
{
	for (int i = 0; i < _config_args_read->mem_nr_store_buffers; i++)
	{
		if (store_buffers[i].timer == INSTANCE_IS_FREE && !store_buffers[i].just_got_a_broadcast)
		{
			return i;
		}

		for (int f = 0; f <  _config_args_read->mem_nr_store_buffers ; f++)
		{
			store_buffers[f].just_got_a_broadcast = false;
		}
	}
	return NO_INSTANCE_AVAILABLE;
}

void update_load_buffer(int index, inst* inst)
{
	load_buffers[index].dst = inst->dst;
	load_buffers[index].imm = inst->imm;
	load_buffers[index].timer = _config_args_read->mem_delay;
	memset(RAT[inst->dst].rs_or_buff_name, 0, NAME_LEN);
	RAT[inst->dst].occupied = true;
	snprintf(RAT[inst->dst].rs_or_buff_name, NAME_LEN, "%s", load_buffers[index].buff_name);
	load_buffers[index].curr_inst = inst;
	load_buffers[index].curr_inst->inst_log->cycle_issued = _cycles;
}

void update_store_buffer(int index, inst* inst)
{
	//in case we need to wait 
	memset(store_buffers[index].src1_waiting, 0, NAME_LEN);
	if (RAT[inst->src1_index].occupied)
	{
		snprintf(store_buffers[index].src1_waiting, NAME_LEN, "%s", RAT[inst->src1_index].rs_or_buff_name);
	}
	else
	{
		store_buffers[index].src1 = _regs[inst->src1_index];
	}
	store_buffers[index].imm = inst->imm;
	store_buffers[index].timer = _config_args_read->mem_delay;
	store_buffers[index].curr_inst = inst;
	store_buffers[index].curr_inst->inst_log->cycle_issued = _cycles;
	store_buffers[index].curr_inst->inst_log->write_cdb = IRRELEVANT;

}

int get_free_reservation_station_index(int opcode)
{
	/*will return an RS from the correct kind for specified instruction
	return:
	NO_RS_AVAILABLE - meaning there's no available RS in this cycle, meaning we won't issue anything in the cycle
	otherwise - available RS*/
	switch (opcode)
	{
		case LD_opcode:
			return NO_INSTANCE_AVAILABLE;
		case ST_opcode:
			return NO_INSTANCE_AVAILABLE;
		case ADD_opcode:
			return get_specific_free_reservation_station_index(rs_add, _config_args_read->add_nr_reservation);
		case SUB_opcode:
			return get_specific_free_reservation_station_index(rs_add, _config_args_read->add_nr_reservation);
		case MULT_opcode:
			return get_specific_free_reservation_station_index(rs_mul, _config_args_read->mul_nr_reservation);
		case DIV_opcode:
			return get_specific_free_reservation_station_index(rs_div, _config_args_read->div_nr_reservation);
		case HALT_opcode:
			return NO_INSTANCE_AVAILABLE;
		default:
			return NO_INSTANCE_AVAILABLE;
	}
}

int get_specific_free_reservation_station_index(RS rses[], int num_rses)
{
	for (int i = 0; i < num_rses; i++)
	{
		if (!rses[i].occupied && !rses[i].just_broadcasted) 
		{
			return i;
		}
	}
	return NO_INSTANCE_AVAILABLE;
}

int put_inst_in_RS(inst* instr)
{
	int rs_index;
	switch (instr->opcode)
	{
		case ADD_opcode:
			// blank on purpose since we do the same thing for both ADD and SUB
		case SUB_opcode:
			rs_index = get_specific_free_reservation_station_index(rs_add, _config_args_read->add_nr_reservation);
			if (rs_index == NO_INSTANCE_AVAILABLE)
			{
				return NO_INSTANCE_AVAILABLE;
			}
			put_inst_in_specific_rs(rs_add, rs_index, instr);																		
			break;

		case MULT_opcode:
			rs_index = get_specific_free_reservation_station_index(rs_mul, _config_args_read->mul_nr_reservation);
			if (rs_index == NO_INSTANCE_AVAILABLE)
			{
				return NO_INSTANCE_AVAILABLE;
			}
			put_inst_in_specific_rs(rs_mul, rs_index, instr);
			break;

		case DIV_opcode:
			rs_index = get_specific_free_reservation_station_index(rs_div, _config_args_read->mul_nr_reservation);
			if (rs_index == NO_INSTANCE_AVAILABLE)
			{
				return NO_INSTANCE_AVAILABLE;
			}
			put_inst_in_specific_rs(rs_div, rs_index, instr);
			break;

		case HALT_opcode:
			finished_issue = true;
			break;

		default:
			break;
	}
	return SUCCESS;
}

void put_inst_in_specific_rs(RS res_stations[], int free_station_index, inst* instr)
{
	res_stations[free_station_index].occupied = true;
	res_stations[free_station_index].dst = instr->dst;
	res_stations[free_station_index].action_type = instr->opcode;

	//check if the registers are wait for values in RAT or they are ready
	memset(res_stations[free_station_index].rs_waiting0, 0, NAME_LEN);
	if (RAT[instr->src0_index].occupied)
	{
		snprintf(res_stations[free_station_index].rs_waiting0, NAME_LEN, "%s", RAT[instr->src0_index].rs_or_buff_name);
	}
	else
	{
		res_stations[free_station_index].src0 = _regs[instr->src0_index];
	}

	memset(res_stations[free_station_index].rs_waiting1, 0, NAME_LEN);
	if (RAT[instr->src1_index].occupied)
	{
		snprintf(res_stations[free_station_index].rs_waiting1, NAME_LEN, "%s", RAT[instr->src1_index].rs_or_buff_name);
	}
	else
	{
		res_stations[free_station_index].src1 = _regs[instr->src1_index];
	}
	res_stations[free_station_index].curr_inst = instr;
	res_stations[free_station_index].curr_inst->inst_log->cycle_issued = _cycles;
	memset(RAT[instr->dst].rs_or_buff_name, 0, NAME_LEN);
	snprintf(RAT[instr->dst].rs_or_buff_name, NAME_LEN, "%s", res_stations[free_station_index].name);
	RAT[instr->dst].occupied = true;
}

void enable_just_dispatched_rses()
{
	enable_just_dispatched_specific_rses(rs_add, _config_args_read->add_nr_reservation);
	enable_just_dispatched_specific_rses(rs_mul, _config_args_read->mul_nr_reservation);
	enable_just_dispatched_specific_rses(rs_div, _config_args_read->div_nr_reservation);
}

void enable_just_dispatched_specific_rses(RS rses[], int num_rses)
{
	for (int i = 0; i < num_rses; i++)
	{
		rses[i].just_broadcasted = false;
	}
}
