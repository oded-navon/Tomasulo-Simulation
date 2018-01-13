#include "Dispatch.h"
#include "Utils.h"

extern config_args* _config_args_read;
extern RS rs_add[MAX_CONFIG_SIZE];
extern RS rs_mul[MAX_CONFIG_SIZE];
extern RS rs_div[MAX_CONFIG_SIZE];
extern calc_unit add_units[MAX_CONFIG_SIZE]; 
extern calc_unit div_units[MAX_CONFIG_SIZE];
extern calc_unit mul_units[MAX_CONFIG_SIZE];
extern unsigned int _cycles;
extern bool received_halt_in_fetch; //means to stop do fetches
extern bool finished_issue; //means to stop handle issues
extern bool finished_dispatch; //means to stop dispatching

void dispatch_inst(calc_unit* unit_to_distpatch_to, RS* inst_to_dispatch, calc_unit_type unit_type);
bool is_rs_inst_ready(RS* inst);
void find_inst_to_dispatch(int num_of_calc_units, int num_of_rs_units, calc_unit* calc_unit_to_disp_to, RS* rs_unit_to_disp_from, calc_unit_type unit_type);
bool check_if_at_least_one_reservation_station_is_occupied();
void Dispatch()
{
	finished_dispatch = received_halt_in_fetch && finished_issue && (finished_dispatch || check_if_at_least_one_reservation_station_is_occupied());
	if (finished_dispatch)
	{
		return;
	}

	//dispatch instructions for every unit type
	find_inst_to_dispatch(_config_args_read->add_nr_units, _config_args_read->add_nr_reservation, add_units, rs_add, ADD_calc_unit);
	find_inst_to_dispatch(_config_args_read->div_nr_units, _config_args_read->div_nr_reservation, div_units, rs_div, DIV_calc_unit);
	find_inst_to_dispatch(_config_args_read->mul_nr_units, _config_args_read->mul_nr_reservation, mul_units, rs_mul, MUL_calc_unit);

}

bool check_if_at_least_one_reservation_station_is_occupied()
{
	for (int j = 0; j < _config_args_read->add_nr_reservation; j++)
	{
		if (rs_add[j].occupied)
		{
			return true;
		}
	}

	for (int j = 0; j < _config_args_read->mul_nr_reservation; j++)
	{
		if (rs_mul[j].occupied)
		{
			return true;
		}
	}

	for (int j = 0; j < _config_args_read->div_nr_reservation; j++)
	{
		if (rs_div[j].occupied)
		{
			return true;
		}
	}

	return false;
}

//go over all the units to find which are free
//when you find a free unit, go over the RS to find a ready instruction
//move the ready inst to the relevant unit
void find_inst_to_dispatch(int num_of_calc_units, int num_of_rs_units, calc_unit* calc_unit_to_disp_to, RS* rs_unit_to_disp_from, calc_unit_type unit_type)
{
	// Go over all the calculation units of the same kind
	for (int i = 0; i < num_of_calc_units; i++)
	{
		//Look for one which is free
		if (calc_unit_to_disp_to[i].timer == INSTANCE_IS_FREE)
		{
			//When you find a free one, look for a free reservation station
			for (int j = 0; j < num_of_rs_units; j++)
			{
				if (is_rs_inst_ready(&rs_unit_to_disp_from[j]))
				{
					//After you find a free one, dispatch it
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
	snprintf(unit_to_distpatch_to->rs_name, NAME_LEN, inst_to_dispatch->name);
	unit_to_distpatch_to->curr_inst = inst_to_dispatch->curr_inst;
	unit_to_distpatch_to->curr_inst->inst_log->cycle_ex_start = _cycles;
	snprintf(unit_to_distpatch_to->curr_inst->inst_log->tag, TAG_LEN, unit_to_distpatch_to->rs_name);
	clear_rs_inst(inst_to_dispatch);
}



bool is_rs_inst_ready(RS* inst)
{
	return (inst->occupied) && (*(inst->rs_waiting0) == '\0') && (*(inst->rs_waiting1) == '\0');
}