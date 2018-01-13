#include "Execute.h"
#include "Utils.h"

extern calc_unit add_units[MAX_CONFIG_SIZE];
extern calc_unit div_units[MAX_CONFIG_SIZE];
extern calc_unit mul_units[MAX_CONFIG_SIZE];
extern config_args* _config_args_read;
extern load_buffer load_buffers[MAX_CONFIG_SIZE];
extern store_buffer store_buffers[MAX_CONFIG_SIZE];
extern bool received_halt_in_fetch; //means to stop do fetches
extern bool finished_issue; //means to stop handle issues
extern bool finished_dispatch; //means to stop dispatching
extern bool finished_execute; //means to stop executing


void Execute()
{
	finished_execute = received_halt_in_fetch && finished_issue && finished_dispatch && finished_execute;
	if (finished_execute)
	{
		return;
	}

	bool at_least_one_unit_is_not_free = false;
	for (int i = 0; i < _config_args_read->mul_nr_units; i++)
	{
		if (mul_units[i].timer != INSTANCE_IS_FREE)
		{
			at_least_one_unit_is_not_free = true;
			if (mul_units[i].timer != INSTANCE_IS_READY)
			{
				mul_units[i].timer--;
			}
		}
	}

	for (int i = 0; i < _config_args_read->div_nr_units; i++)
	{
		if (div_units[i].timer != INSTANCE_IS_FREE)
		{
			at_least_one_unit_is_not_free = true;
			if (div_units[i].timer != INSTANCE_IS_READY)
			{
				div_units[i].timer--;
			}
		}
	}

	for (int i = 0; i < _config_args_read->add_nr_units; i++)
	{
		if (add_units[i].timer != INSTANCE_IS_FREE)
		{
			at_least_one_unit_is_not_free = true;
			if (add_units[i].timer != INSTANCE_IS_READY)
			{
				add_units[i].timer--;
			}
		}
	}

	for (int i = 0; i < _config_args_read->mem_nr_load_buffers; i++)
	{
		if (load_buffers[i].timer != INSTANCE_IS_FREE)
		{
			at_least_one_unit_is_not_free = true;
			if (load_buffers[i].timer != INSTANCE_IS_READY)
			{
				load_buffers[i].timer--;
			}
		}
	}

	for (int i = 0; i < _config_args_read->mem_nr_store_buffers; i++)
	{
		if (store_buffers[i].timer != INSTANCE_IS_FREE)
		{
			at_least_one_unit_is_not_free = true;
			if (store_buffers[i].timer != INSTANCE_IS_READY)
			{
				store_buffers[i].timer--;
			}
		}
	}

	if (!at_least_one_unit_is_not_free)
	{
		finished_execute = true;
	}
}