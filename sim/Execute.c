#include "Execute.h"
#include "Utils.h"

extern calc_unit add_units[MAX_CONFIG_SIZE];
extern calc_unit div_units[MAX_CONFIG_SIZE];
extern calc_unit mul_units[MAX_CONFIG_SIZE];
extern config_args* _config_args_read;

void Execute()
{
	for (int i = 0; i < _config_args_read->mul_nr_units; i++)
	{
		if (mul_units[i].timer != CALC_UNIT_IS_FREE && mul_units[i].timer != CALC_UNIT_IS_READY)
		{
			mul_units[i].timer--;
		}
	}
	for (int i = 0; i < _config_args_read->div_nr_units; i++)
	{
		if (div_units[i].timer != CALC_UNIT_IS_FREE && div_units[i].timer != CALC_UNIT_IS_READY)
		{
			div_units[i].timer--;
		}
	}
	for (int i = 0; i < _config_args_read->add_nr_units; i++)
	{
		if (add_units[i].timer != CALC_UNIT_IS_FREE && add_units[i].timer != CALC_UNIT_IS_READY)
		{
			add_units[i].timer--;
		}
	}
}