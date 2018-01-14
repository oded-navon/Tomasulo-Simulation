#include "Broadcast.h"
#include "Utils.h"
#include "Tracer.h"

extern int _memory_image_input[MEMORY_IMAGE_INPUT_SIZE];
extern calc_unit add_units[MAX_CONFIG_SIZE];
extern calc_unit div_units[MAX_CONFIG_SIZE];
extern calc_unit mul_units[MAX_CONFIG_SIZE];
extern config_args* _config_args_read;
extern load_buffer load_buffers[MAX_CONFIG_SIZE];
extern store_buffer store_buffers[MAX_CONFIG_SIZE];
extern cdb_free* _cdb_free;
extern unsigned int _cycles;
extern char* _trace_cdb_file_path;
extern bool received_halt_in_fetch; //means to stop do fetches
extern bool finished_issue; //means to stop handle issues
extern bool finished_dispatch; //means to stop dispatching
extern bool finished_execute; //means to stop executing
extern bool finished_broadcast; //means to stop broadcasting

float _regs[NUM_OF_REGS];
RAT_entry RAT[NUM_OF_REGS];
RS rs_add[MAX_CONFIG_SIZE];
RS rs_mul[MAX_CONFIG_SIZE];
RS rs_div[MAX_CONFIG_SIZE];

char rs_add_names[MAX_CONFIG_SIZE][NAME_LEN];
char rs_mul_names[MAX_CONFIG_SIZE][NAME_LEN];
char rs_div_names[MAX_CONFIG_SIZE][NAME_LEN];

void broadcast_result(char* rs_or_buff_name, float value);
float calculate_result(calc_unit* unit_to_broadcast);
void broadcast_to_rs(int num_of_rs_stations, float operation_result, RS* rs_stations, char* unit_to_broadcast_name);
bool broadcast_specific_calc_type(int num_of_calc_units, calc_unit* unit_to_broadcast);
bool broadcast_memory();
float load_from_address(load_buffer* buff);
float store_at_address(store_buffer* buff);
void broadcast_to_store_buffers(char* unit_to_broadcast_name, float operation_result);
float translate_int_to_float_presentation_for_mem(int mem_value_as_int);
int translate_float_to_int_presentation_for_mem(float val);

//Go over all of the calculation units and look for ready units
void Broadcast()
{
	finished_broadcast = received_halt_in_fetch && finished_issue && finished_dispatch && finished_execute && finished_broadcast;
	if(finished_broadcast)
	{
		printf("the program finished it's run. thank you and go fuck yourself");
		return;
	}
	finished_broadcast = broadcast_specific_calc_type(_config_args_read->mul_nr_units, mul_units);
	finished_broadcast = broadcast_specific_calc_type(_config_args_read->div_nr_units, div_units) && finished_broadcast;
	finished_broadcast = broadcast_specific_calc_type(_config_args_read->add_nr_units, add_units) && finished_broadcast;
	finished_broadcast = broadcast_memory() && finished_broadcast;

}

//Broadcast goes over the RAT and the reservation stations and looks for the relevant tag, which is the rs_or_buff_name parameter
void broadcast_result(char* rs_or_buff_name, float value)
{
	//Go over all the entries in the RAT table and handle the relevant entries, also insert the value to the regs table
	for (int i = 0; i < NUM_OF_REGS; i++)
	{
		if(RAT[i].occupied && strcmp(RAT[i].rs_or_buff_name, rs_or_buff_name) == 0)
		{
			RAT[i].occupied = false;
			_regs[i] = value;
		}	
	}
	//Go over all of the reservation stations and replace the stations with the relevant tag with the real value
	broadcast_to_rs(_config_args_read->add_nr_reservation, value, rs_add, rs_or_buff_name);
	broadcast_to_rs(_config_args_read->div_nr_reservation, value, rs_div, rs_or_buff_name);
	broadcast_to_rs(_config_args_read->mul_nr_reservation, value, rs_mul, rs_or_buff_name);
	broadcast_to_store_buffers(rs_or_buff_name, value);
}

float calculate_result(calc_unit* unit_to_broadcast)
{
	switch (unit_to_broadcast->curr_inst->opcode)
	{
		case ADD_opcode:
			return unit_to_broadcast->src0 + unit_to_broadcast->src1;
		case SUB_opcode:
			return unit_to_broadcast->src0 - unit_to_broadcast->src1;
		case MULT_opcode:
			return unit_to_broadcast->src0 * unit_to_broadcast->src1;
		case DIV_opcode:
			return unit_to_broadcast->src0 / unit_to_broadcast->src1;
		default:
			return 0.0;
	}
}

void broadcast_to_rs(int num_of_rs_stations, float operation_result, RS* rs_stations, char* unit_to_broadcast_name)
{
	for (int i = 0; i < num_of_rs_stations; i++)
	{
		if (rs_stations[i].occupied && strcmp(rs_stations[i].rs_waiting0, unit_to_broadcast_name) == 0)
		{
			memset(rs_stations[i].rs_waiting0, 0, NAME_LEN);
			rs_stations[i].src0 = operation_result;
		}
		if (rs_stations[i].occupied && strcmp(rs_stations[i].rs_waiting1, unit_to_broadcast_name) == 0)
		{
			memset(rs_stations[i].rs_waiting1, 0, NAME_LEN);
			rs_stations[i].src1 = operation_result;
		}
	}
}

void broadcast_to_store_buffers(char* unit_to_broadcast_name, float operation_result)
{
	for (int i = 0; i < _config_args_read->mem_nr_store_buffers; i++)
	{
		if (store_buffers[i].timer != INSTANCE_IS_FREE && strcmp(store_buffers[i].src1_waiting, unit_to_broadcast_name) == 0)
		{
			memset(store_buffers[i].src1_waiting, 0, NAME_LEN);
			store_buffers[i].src1 = operation_result;
		}
	}
}

bool broadcast_specific_calc_type(int num_of_calc_units, calc_unit* unit_to_broadcast)
{
	bool all_units_are_free = true;
	for (int i = 0; i < num_of_calc_units; i++)
	{
		if (unit_to_broadcast[i].timer == INSTANCE_IS_READY)
		{
			float operation_result = calculate_result(unit_to_broadcast);
			broadcast_result(unit_to_broadcast[i].rs_name, operation_result);
			unit_to_broadcast[i].timer = INSTANCE_IS_FREE;
			unit_to_broadcast[i].curr_inst->inst_log->write_cdb = _cycles;
			write_cdb_trace_to_file(_cycles, unit_to_broadcast[i].curr_inst->inst_log->pc, unit_to_broadcast[i].curr_inst->opcode, operation_result, unit_to_broadcast[i].curr_inst->inst_log->tag);
			//This 'break' causes the CDB to only take 1 value in each cycle. We find the first unit which is ready and broadcast
			//its result, and then break. So only the first ready unit is broadcasted in effect.
			all_units_are_free = false;
			clear_rs_inst(unit_to_broadcast[i].rs_name);

			break;
			//set_cdb_occupied(unit_to_broadcast->calc_type);
		}
	}
	return all_units_are_free;
}

/*void set_cdb_occupied(calc_unit_type unit_type)
{
	switch (unit_type)
	{
	case ADD_calc_unit:
		_cdb_free->add_cdb_is_free = false;
		break;
	case DIV_calc_unit:
		_cdb_free->div_cdb_is_free = false;
		break;
	case MUL_calc_unit:
		_cdb_free->mul_cdb_is_free = false;
		break;
	}
}

bool cdb_is_free(calc_unit_type unit_type)
{
	switch (unit_type)
	{
		case ADD_calc_unit:
			return _cdb_free->add_cdb_is_free;
		case DIV_calc_unit:
			return _cdb_free->div_cdb_is_free;
		case MUL_calc_unit:
			return _cdb_free->mul_cdb_is_free;
		default:
			return false;
	}
}

void reset_cdb()
{
	_cdb_free->add_cdb_is_free = true;
	_cdb_free->mul_cdb_is_free = true;
	_cdb_free->div_cdb_is_free = true;
	_cdb_free->mem_cdb_is_free = true;
}*/

bool broadcast_memory()
{
	bool all_units_are_free = true;	//this flag helps us determine when the program ended it's run.
	for (int i = 0; i < _config_args_read->mem_nr_load_buffers; i++)
	{
		if (load_buffers[i].timer == INSTANCE_IS_READY)
		{
			all_units_are_free = false;
			float mem_result = load_from_address(&load_buffers[i]);
			broadcast_result(load_buffers[i].buff_name, mem_result);
			load_buffers[i].timer = INSTANCE_IS_FREE;
			write_cdb_trace_to_file(_cycles, load_buffers[i].curr_inst->inst_log->pc , load_buffers[i].curr_inst->opcode, mem_result, load_buffers[i].curr_inst->inst_log->tag);
			//This 'return' simulates the CDB only taking 1 value in each cycle. 
			return all_units_are_free;
		}
	}

	for (int i = 0; i < _config_args_read->mem_nr_store_buffers; i++)
	{
		if (store_buffers[i].timer == INSTANCE_IS_READY)
		{
			all_units_are_free = false;
			float mem_result = store_at_address(&store_buffers[i]);
			//broadcast_result(load_buffers[i].buff_name, mem_result);
			store_buffers[i].timer = INSTANCE_IS_FREE;
			return all_units_are_free;
		}
	}
	return all_units_are_free;
}

float load_from_address(load_buffer* buff)
{
	_regs[buff->dst] = translate_int_to_float_presentation_for_mem(_memory_image_input[buff->imm]);
	return _regs[buff->dst];
}

float store_at_address(store_buffer* buff)
{
	_memory_image_input[buff->imm] = translate_float_to_int_presentation_for_mem(buff->src1);
	return buff->src1;
}

float translate_int_to_float_presentation_for_mem(int mem_value_as_int)
{
	void* void_cast = (void*) &mem_value_as_int;
	float* res = (float*)void_cast;
	return *res;
}

int translate_float_to_int_presentation_for_mem(float val)
{
	void* void_cast = (void*) &val;
	int* res = (int*)void_cast;
	return *res;
}
