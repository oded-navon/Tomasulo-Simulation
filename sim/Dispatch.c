#include "Dispatch.h"

#define CALC_UNIT_IS_FREE (-1)
#define NUM_OF_CALC_UNITS (3)

extern config_args* _config_args_read;
extern RS rs_add[MAX_CONFIG_SIZE];
extern RS rs_mul[MAX_CONFIG_SIZE];
extern RS rs_div[MAX_CONFIG_SIZE];
extern calc_unit add_units[MAX_CONFIG_SIZE]; 
extern calc_unit div_units[MAX_CONFIG_SIZE];
extern calc_unit mul_units[MAX_CONFIG_SIZE];

void dispatch_inst(calc_unit* unit_to_distpatch_to, RS* inst_to_dispatch, calc_unit_type unit_type);
bool is_rs_inst_ready(RS* inst);
void clear_rs_inst(RS* inst_to_clear);
void find_inst_to_dispatch(int num_of_calc_units, int num_of_rs_units, calc_unit* calc_unit_to_disp_to, RS* rs_unit_to_disp_from, calc_unit_type unit_type);

void Dispatch()
{
	//go over all the units to find which are free
	//when you find a free unit, go over the RS to find a ready instruction
	//move the ready inst to the relevant unit
	for (int i = 0 ; i < NUM_OF_CALC_UNITS ; i++)
	{
		find_inst_to_dispatch(_config_args_read->add_nr_units, _config_args_read->add_nr_reservation, add_units, rs_add, ADD_calc_unit);
		find_inst_to_dispatch(_config_args_read->div_nr_units, _config_args_read->div_nr_reservation, div_units, rs_div, DIV_calc_unit);
		find_inst_to_dispatch(_config_args_read->mul_nr_units, _config_args_read->mul_nr_reservation, mul_units, rs_mul, MUL_calc_unit);
	}
}

void find_inst_to_dispatch(int num_of_calc_units, int num_of_rs_units, calc_unit* calc_unit_to_disp_to, RS* rs_unit_to_disp_from, calc_unit_type unit_type)
{
	for (int i = 0; i < num_of_calc_units; i++)
	{
		if (calc_unit_to_disp_to[i].timer == CALC_UNIT_IS_FREE)
		{
			for (int j = 0; j < num_of_rs_units; j++)
			{
				if (is_rs_inst_ready(&rs_unit_to_disp_from[j]))
				{
					dispatch_inst(&calc_unit_to_disp_to[i], &rs_unit_to_disp_from[j], unit_type);
					break;
				}
			}
		}
	}
}

void dispatch_inst(calc_unit* unit_to_distpatch_to, RS* inst_to_dispatch, calc_unit_type unit_type)
{
	switch (unit_type)
	{
		case ADD_calc_unit:
			unit_to_distpatch_to->timer = _config_args_read->add_delay;
			break;
		case DIV_calc_unit:
			unit_to_distpatch_to->timer = _config_args_read->div_delay;
			break;
		case MUL_calc_unit:
			unit_to_distpatch_to->timer = _config_args_read->mul_delay;
			break;
	}

	unit_to_distpatch_to->calc_type = unit_type;
	unit_to_distpatch_to->src0 = inst_to_dispatch->src0;
	unit_to_distpatch_to->src1 = inst_to_dispatch->src1;
	unit_to_distpatch_to->dst = inst_to_dispatch->dst;

	clear_rs_inst(inst_to_dispatch);
}

void clear_rs_inst(RS* inst_to_clear)
{
	inst_to_clear->occupied = false;
}

bool is_rs_inst_ready(RS* inst)
{
	return (inst->occupied) && (inst->rs_waiting0 == NULL) && (inst->rs_waiting1 == NULL);
}