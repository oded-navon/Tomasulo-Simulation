#include "Broadcast.h"
#include "Utils.h"

extern int _memory_image_input[MEMORY_IMAGE_INPUT_SIZE];
extern calc_unit add_units[MAX_CONFIG_SIZE];
extern calc_unit div_units[MAX_CONFIG_SIZE];
extern calc_unit mul_units[MAX_CONFIG_SIZE];
extern config_args* _config_args_read;
extern load_buffer load_buffers[MAX_CONFIG_SIZE];
extern store_buffer store_buffers[MAX_CONFIG_SIZE];

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
void clear_rs_inst(RS* inst_to_clear);
void broadcast_to_rs(int num_of_rs_stations, float operation_result, RS* rs_stations, char* unit_to_broadcast_name);
void broadcast_specific_calc_type(int num_of_calc_units, calc_unit* unit_to_broadcast);
void broadcast_memory();
float load_from_address(load_buffer* buff);
float store_at_address(store_buffer* buff);
void broadcast_to_store_buffers(char* unit_to_broadcast_name, float operation_result);
float translate_bits_to_single_precision(int mem_value_as_int);
float translate_float_to_single_precision(float val);

//Go over all of the calculation units and look for ready units
void Broadcast()
{
	broadcast_specific_calc_type(_config_args_read->mul_nr_units, mul_units);
	broadcast_specific_calc_type(_config_args_read->div_nr_units, div_units);
	broadcast_specific_calc_type(_config_args_read->add_nr_units, add_units);
	broadcast_memory();
}

//Broadcast goes over the RAT and the reservation stations and looks for the relevant tag
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
	switch (unit_to_broadcast->calc_type)
	{
		case ADD_calc_unit:
			return unit_to_broadcast->src0 + unit_to_broadcast->src1;
		case MUL_calc_unit:
			return unit_to_broadcast->src0 * unit_to_broadcast->src1;
		case DIV_calc_unit:
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

void broadcast_specific_calc_type(int num_of_calc_units, calc_unit* unit_to_broadcast)
{
	for (int i = 0; i < num_of_calc_units; i++)
	{
		if (unit_to_broadcast[i].timer == INSTANCE_IS_READY)
		{
			float operation_result = calculate_result(unit_to_broadcast);
			broadcast_result(unit_to_broadcast->rs_name, operation_result);
			unit_to_broadcast[i].timer = INSTANCE_IS_FREE;
		}
	}
}
void broadcast_memory()
{
	for (int i = 0; i < _config_args_read->mem_nr_load_buffers; i++)
	{
		if (load_buffers[i].timer == INSTANCE_IS_READY)
		{
			float mem_result = load_from_address(&load_buffers[i]);
			broadcast_result(load_buffers[i].buff_name, mem_result);
			load_buffers[i].timer = INSTANCE_IS_FREE;
		}
	}

	for (int i = 0; i < _config_args_read->mem_nr_store_buffers; i++)
	{
		if (store_buffers[i].timer == INSTANCE_IS_READY)
		{
			float mem_result = store_at_address(&load_buffers[i]);
			//broadcast_result(load_buffers[i].buff_name, mem_result);
			store_buffers[i].timer = INSTANCE_IS_FREE;
		}
	}
}

float load_from_address(load_buffer* buff)
{
	_regs[buff->dst] = translate_bits_to_single_precision(_memory_image_input[buff->imm]);
	return _regs[buff->dst];
}

float store_at_address(store_buffer* buff)
{
	_memory_image_input[buff->imm] = translate_float_to_single_precision(buff->src1);
	return buff->src1;
}

float translate_bits_to_single_precision(int mem_value_as_int)
{
	fp_repr fp = { .bin_repr = mem_value_as_int };
	return 1.0;
}

float translate_float_to_single_precision(float val)
{
	fp_repr fp = { .bin_repr = val };
	return 1.0;
}
