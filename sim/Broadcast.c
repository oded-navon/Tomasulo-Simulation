#include "Broadcast.h"
#include "Utils.h"

extern calc_unit add_units[MAX_CONFIG_SIZE];
extern calc_unit div_units[MAX_CONFIG_SIZE];
extern calc_unit mul_units[MAX_CONFIG_SIZE];
extern config_args* _config_args_read;
float _regs[NUM_OF_REGS];
RAT_entry RAT[NUM_OF_REGS];
RS rs_add[MAX_CONFIG_SIZE];
RS rs_mul[MAX_CONFIG_SIZE];
RS rs_div[MAX_CONFIG_SIZE];

char rs_add_names[MAX_CONFIG_SIZE][RS_NAME_LEN];
char rs_mul_names[MAX_CONFIG_SIZE][RS_NAME_LEN];
char rs_div_names[MAX_CONFIG_SIZE][RS_NAME_LEN];

void broadcast_result(calc_unit* unit_to_broadcast);
float calculate_result(calc_unit* unit_to_broadcast);
void clear_rs_inst(RS* inst_to_clear);
void broadcast_to_rs(int num_of_rs_stations, float operation_result, RS* rs_stations, char* unit_to_broadcast_name);
void broadcast_specific_calc_type(int num_of_calc_units, calc_unit* unit_to_broadcast);

//Go over all of the calculation units and look for ready units
void Broadcast()
{
	broadcast_specific_calc_type(_config_args_read->mul_nr_units, mul_units);
	broadcast_specific_calc_type(_config_args_read->div_nr_units, div_units);
	broadcast_specific_calc_type(_config_args_read->add_nr_units, add_units);
}

//Broadcast goes over the RAT and the reservation stations and looks for the relevant tag
void broadcast_result(calc_unit* unit_to_broadcast)
{
	float operation_result = calculate_result(unit_to_broadcast);
	
	//Go over all the entries in the RAT table and handle the relevant entries, also insert the value to the regs table
	for (int i = 0; i < NUM_OF_REGS; i++)
	{
		if(RAT[i].occupied && strcmp(RAT[i].rs, unit_to_broadcast->dst_rs->name) == 0)
		{
			RAT[i].occupied = false;
			_regs[i] = operation_result;
		}	
	}
	//Go over all of the reservation stations and replace the stations with the relevant tag with the real value
	broadcast_to_rs(_config_args_read->add_nr_reservation, operation_result, rs_add, unit_to_broadcast->dst_rs->name);
	broadcast_to_rs(_config_args_read->div_nr_reservation, operation_result, rs_div, unit_to_broadcast->dst_rs->name);
	broadcast_to_rs(_config_args_read->mul_nr_reservation, operation_result, rs_mul, unit_to_broadcast->dst_rs->name);
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

void clear_rs_inst(RS* inst_to_clear)
{
	inst_to_clear->occupied = false;
}

void broadcast_to_rs(int num_of_rs_stations, float operation_result, RS* rs_stations, char* unit_to_broadcast_name)
{
	for (int i = 0; i < num_of_rs_stations; i++)
	{
		if (rs_stations[i].occupied && strcmp(rs_stations[i].rs_waiting0, unit_to_broadcast_name) == 0)
		{
			memset(rs_stations[i].rs_waiting0, 0, RS_NAME_LEN);
			rs_stations[i].src0 = operation_result;
		}
		if (rs_stations[i].occupied && strcmp(rs_stations[i].rs_waiting1, unit_to_broadcast_name) == 0)
		{
			memset(rs_stations[i].rs_waiting1, 0, RS_NAME_LEN);
			rs_stations[i].src1 = operation_result;
		}
	}
}

void broadcast_specific_calc_type(int num_of_calc_units, calc_unit* unit_to_broadcast)
{
	for (int i = 0; i < num_of_calc_units; i++)
	{
		if (unit_to_broadcast[i].timer == CALC_UNIT_IS_READY)
		{
			broadcast_result(&unit_to_broadcast[i]);
			clear_rs_inst(unit_to_broadcast[i].dst_rs);
			unit_to_broadcast->timer = CALC_UNIT_IS_FREE;
		}
	}
}
