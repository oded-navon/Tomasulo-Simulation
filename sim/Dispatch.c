#include "Dispatch.h"

#define CALC_UNIT_IS_FREE (-1)



void dispatch_inst(config_args* config_argus, calc_unit* unit_to_distpatch_to, RS* inst_to_dispatch, calc_unit_type unit_type);
bool is_rs_inst_ready(RS* inst);

void Dispatch(config_args* config_argus, RS* rs_add, RS* rs_mul, RS* rs_div, calc_unit* add_units, calc_unit* div_units, calc_unit* mul_units)
{
	//go over all the units to find which are free
	//when you find a free unit, go over the RS to find a ready instruction
	//move the ready inst to the relevant unit

	// add units
	for (int i = 0; i < config_argus->add_nr_units; i++)
	{
		if (add_units[i].timer == CALC_UNIT_IS_FREE)
		{
			for (int j = 0; j < config_argus->add_nr_reservation; j++)
			{
				if (is_rs_inst_ready(&rs_add[j])) 
				{
					dispatch_inst(config_argus, &add_units[i], &rs_add[j], ADD);
					break;
				}
			}
		}
	}
}

void dispatch_inst(config_args* config_argus, calc_unit* unit_to_distpatch_to, RS* inst_to_dispatch, calc_unit_type unit_type)
{
	switch (unit_type)
	{
		case ADD:
			unit_to_distpatch_to->timer = config_argus->add_delay;
			break;
		case DIV:
			unit_to_distpatch_to->timer = config_argus->div_delay;
			break;
		case MUL:
			unit_to_distpatch_to->timer = config_argus->mul_delay;
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
/*
typedef struct {
int dst;
int src0;
int src1;
char rs_waiting0[RS_NAME_LEN];
char rs_waiting1[RS_NAME_LEN];
int action_type;
bool occupied;
}RS;


typedef struct {
int timer;
int dst;
int rs0;
int rs1;
int calc_type;
}calc_unit;
*/