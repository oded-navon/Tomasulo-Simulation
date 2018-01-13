#include "Execute.h"
#include "Utils.h"

extern calc_unit add_units[MAX_CONFIG_SIZE];
extern calc_unit div_units[MAX_CONFIG_SIZE];
extern calc_unit mul_units[MAX_CONFIG_SIZE];
extern config_args* _config_args_read;
extern load_buffer load_buffers[MAX_CONFIG_SIZE];
extern store_buffer store_buffers[MAX_CONFIG_SIZE];
void Execute()
{
	for (int i = 0; i < _config_args_read->mul_nr_units; i++)
	{
		if (mul_units[i].timer != INSTANCE_IS_FREE && mul_units[i].timer != INSTANCE_IS_READY)
		{
			mul_units[i].timer--;
		}
	}
	for (int i = 0; i < _config_args_read->div_nr_units; i++)
	{
		if (div_units[i].timer != INSTANCE_IS_FREE && div_units[i].timer != INSTANCE_IS_READY)
		{
			div_units[i].timer--;
		}
	}
	for (int i = 0; i < _config_args_read->add_nr_units; i++)
	{
		if (add_units[i].timer != INSTANCE_IS_FREE && add_units[i].timer != INSTANCE_IS_READY)
		{
			add_units[i].timer--;
		}
	}

	for (int i = 0; i < _config_args_read->mem_nr_load_buffers; i++)
	{
		if (load_buffers[i].timer != INSTANCE_IS_FREE && load_buffers[i].timer != INSTANCE_IS_FREE)
		{
			load_buffers[i].timer--;
		}
	}

	for (int i = 0; i < _config_args_read->mem_nr_store_buffers; i++)
	{
		if (store_buffers[i].timer != INSTANCE_IS_FREE && store_buffers[i].timer != INSTANCE_IS_FREE)
		{
			store_buffers[i].timer--;
		}
	}
}