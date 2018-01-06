#include "Issue.h"
#include "Queue.h"

extern RS rs_add[MAX_CONFIG_SIZE];
extern RS rs_mul[MAX_CONFIG_SIZE];
extern RS rs_div[MAX_CONFIG_SIZE]; 
extern char rs_add_names[MAX_CONFIG_SIZE][RS_NAME_LEN];
extern char rs_mul_names[MAX_CONFIG_SIZE][RS_NAME_LEN];
extern char rs_div_names[MAX_CONFIG_SIZE][RS_NAME_LEN];
extern config_args* _config_args_read;
extern RAT_entry RAT[NUM_OF_REGS];
extern float _regs[NUM_OF_REGS];
extern inst_queue* _iq_arr;

bool issue_instruction();
int get_free_reservation_station_index(int opcode);
int get_specific_free_reservation_station_index(RS rses[], int num_rses);
int put_inst_in_RS(inst* instr);
void put_inst_in_specific_rs(RS* res_stations, int free_station_index, inst* instr);

void issue()
{
	// try to issue an instruction
	if (issue_instruction())
	{
		// only if we succeeded, try issuing another
		issue_instruction();
	}
}

bool issue_instruction()
{
	inst* instr = peek_queue_tail();

	int issued_successfully = put_inst_in_RS(instr);
	if (issued_successfully == NO_RS_AVAILABLE)
	{
		return false;
	}
	else
	{
		dequeue(_iq_arr);
	}
	return true;
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
			return NO_RS_AVAILABLE;
		case ST_opcode:
			return NO_RS_AVAILABLE;
		case ADD_opcode:
			return get_specific_free_reservation_station_index(rs_add, _config_args_read->add_nr_reservation);
		case SUB_opcode:
			return get_specific_free_reservation_station_index(rs_add, _config_args_read->add_nr_reservation);
		case MULT_opcode:
			return get_specific_free_reservation_station_index(rs_mul, _config_args_read->mul_nr_reservation);
		case DIV_opcode:
			return get_specific_free_reservation_station_index(rs_div, _config_args_read->div_nr_reservation);
		case HALT_opcode:
			return NO_RS_AVAILABLE;
		default:
			return NO_RS_AVAILABLE;
	}
}

int get_specific_free_reservation_station_index(RS rses[], int num_rses)
{
	for (int i = 0; i < num_rses; i++)
	{
		if (!rses[i].occupied) 
		{
			return i;
		}
	}
	return NO_RS_AVAILABLE;
}

int put_inst_in_RS(inst* instr)
{
	int rs_index;
	switch (instr->opcode)
	{
		case LD_opcode:
			break;

		case ST_opcode:
			break;

		case ADD_opcode:
			// blank on purpose since we do the same thing for both ADD and SUB
		case SUB_opcode:
			rs_index = get_specific_free_reservation_station_index(rs_add, _config_args_read->add_nr_reservation);
			if (rs_index == NO_RS_AVAILABLE)
			{
				return NO_RS_AVAILABLE;
			}
			put_inst_in_specific_rs(rs_add, rs_index, instr);
			break;

		case MULT_opcode:
			rs_index = get_specific_free_reservation_station_index(rs_mul, _config_args_read->mul_nr_reservation);
			if (rs_index == NO_RS_AVAILABLE)
			{
				return NO_RS_AVAILABLE;
			}
			put_inst_in_specific_rs(rs_mul, rs_index, instr);
			break;

		case DIV_opcode:
			rs_index = get_specific_free_reservation_station_index(rs_div, _config_args_read->mul_nr_reservation);
			if (rs_index == NO_RS_AVAILABLE)
			{
				return NO_RS_AVAILABLE;
			}
			put_inst_in_specific_rs(rs_div, rs_index, instr);
			break;

		case HALT_opcode:
			break;

		default:
			break;
	}
	return SUCCESS;
}

void put_inst_in_specific_rs(RS* res_stations, int free_station_index, inst* instr)
{
	res_stations[free_station_index].occupied = true;
	res_stations[free_station_index].dst = instr->dst;
	res_stations[free_station_index].action_type = instr->opcode;

	//check if the registers are wait for values in RAT or they are ready
	if (RAT[instr->src0].occupied)
	{
		memset(res_stations[free_station_index].rs_waiting0, 0 , RS_NAME_LEN);
		snprintf(res_stations[free_station_index].rs_waiting0, RS_NAME_LEN, "%s", RAT[instr->src0].rs);
	}
	else
	{
		res_stations[free_station_index].src0 = _regs[instr->src0];
	}
	
	if (RAT[instr->src1].occupied)
	{
		memset(res_stations[free_station_index].rs_waiting1, 0, RS_NAME_LEN);
		snprintf(res_stations[free_station_index].rs_waiting1, RS_NAME_LEN, "%s", RAT[instr->src1].rs);
	}
	else
	{
		res_stations[free_station_index].src1 = _regs[instr->src1];
	}
}
