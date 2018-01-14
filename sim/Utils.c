#include "Utils.h"


extern int _memory_image_input[MEMORY_IMAGE_INPUT_SIZE];
extern config_args* _config_args_read;
extern inst* _instructions[MAX_INST_NUM];
extern float _regs[NUM_OF_REGS];
extern int _num_of_inst;

extern char rs_add_names[MAX_CONFIG_SIZE][NAME_LEN];
extern char rs_mul_names[MAX_CONFIG_SIZE][NAME_LEN];
extern char rs_div_names[MAX_CONFIG_SIZE][NAME_LEN];

extern calc_unit add_units[MAX_CONFIG_SIZE];
extern calc_unit div_units[MAX_CONFIG_SIZE];
extern calc_unit mul_units[MAX_CONFIG_SIZE];

extern RS rs_add[MAX_CONFIG_SIZE];
extern RS rs_mul[MAX_CONFIG_SIZE];
extern RS rs_div[MAX_CONFIG_SIZE];

extern load_buffer load_buffers[MAX_CONFIG_SIZE];
extern store_buffer store_buffers[MAX_CONFIG_SIZE];

void cleanup(cleanup_type clean_type)
{
	switch (clean_type)
	{
	case cleanup_inst_and_config:
		for (int i = 0; i < MAX_INST_NUM; i++)
		{
			free(_instructions[i]->inst_log);
			free(_instructions[i]);
		}
		for (int i = 0; i < _num_of_inst; i++)
		{
			free(_instructions[i]->inst_log);
		}
	case cleanup_config:
		free(_config_args_read);
	}
}

void init_regs()
{
	for (int i = 0; i < NUM_OF_REGS; i++)
	{
		_regs[i] = (float)i;
	}
}

void init_rs_names_arrays()
{
	for (int i = 0; i < _config_args_read->add_nr_reservation; i++)
	{
		snprintf(rs_add[i].name, NAME_LEN, "ADD%d", i);
	}

	for (int i = 0; i < _config_args_read->div_nr_reservation; i++)
	{
		snprintf(rs_div[i].name, NAME_LEN, "DIV%d", i);
	}

	for (int i = 0; i < _config_args_read->mul_nr_reservation; i++)
	{
		snprintf(rs_mul[i].name, NAME_LEN, "MUL%d", i);
	}

}

void init_buff_names_arrays()
{
	for (int i = 0; i < _config_args_read->mem_nr_load_buffers; i++)
	{
		snprintf(load_buffers[i].buff_name, NAME_LEN, "LD%d", i);
	}

	for (int i = 0; i < _config_args_read->mem_nr_store_buffers; i++)
	{
		snprintf(store_buffers[i].buff_name, NAME_LEN, "STORE%d", i);
	}
}

void init_inst_ex_array()
{
	for (int i = 0; i < _num_of_inst; i++)
	{
		_instructions[i]->inst_log = malloc(sizeof(inst_ex));
		//For tracing the instructions
		_instructions[i]->inst_log->inst_code = _memory_image_input[i];
		_instructions[i]->inst_log->pc = i;
	}
}

void clear_all_ex_units()
{
	for (int i = 0; i < _config_args_read->mul_nr_units; i++)
	{
		mul_units[i].timer = -1;
	}
	for (int i = 0; i < _config_args_read->div_nr_units; i++)
	{
		div_units[i].timer = -1;
	}
	for (int i = 0; i < _config_args_read->add_nr_units; i++)
	{
		add_units[i].timer = -1;
	}
}

void clear_all_buffers()
{
	for (int i = 0; i < _config_args_read->mem_nr_load_buffers; i++)
	{
		load_buffers[i].timer = -1;
	}
	for (int i = 0; i < _config_args_read->mem_nr_store_buffers; i++)
	{
		store_buffers[i].timer = -1;
	}
}

void clear_rs_inst(char* inst_to_clear_name)
{
	for (int i = 0; i < _config_args_read->add_nr_reservation; i++)
	{
		if (strcmp(inst_to_clear_name, rs_add[i].name) == 0)
		{
			rs_add[i].occupied = false;
			rs_add[i].just_broadcasted = true;
			rs_add[i].already_dispatched = false;
		}
	}

	for (int i = 0; i < _config_args_read->div_nr_reservation; i++)
	{
		if (strcmp(inst_to_clear_name, rs_div[i].name) == 0)
		{
			rs_div[i].occupied = false;
			rs_div[i].just_broadcasted = true;
			rs_div[i].already_dispatched = false;
		}
	}

	for (int i = 0; i < _config_args_read->mul_nr_reservation; i++)
	{
		if (strcmp(inst_to_clear_name, rs_mul[i].name) == 0)
		{
			rs_mul[i].occupied = false;
			rs_mul[i].just_broadcasted = true;
			rs_mul[i].already_dispatched = false;
		}
	}
}

