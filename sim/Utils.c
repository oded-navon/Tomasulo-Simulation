#include "Utils.h"

extern int _memory_image_input[MEMORY_IMAGE_INPUT_SIZE];
extern config_args* _config_args_read;
extern inst* _instructions[MAX_INST_NUM];
extern float _regs[NUM_OF_REGS];

extern char rs_add_names[MAX_CONFIG_SIZE][RS_NAME_LEN];
extern char rs_mul_names[MAX_CONFIG_SIZE][RS_NAME_LEN];
extern char rs_div_names[MAX_CONFIG_SIZE][RS_NAME_LEN];

void cleanup(cleanup_type clean_type)
{
	switch (clean_type)
	{
	case cleanup_inst_and_config:
		for (int i = 0; i < MAX_INST_NUM; i++)
		{
			free(_instructions[i]);
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
	for (int i = 0; i < MAX_CONFIG_SIZE; i++)
	{
		snprintf(rs_add_names[i], RS_NAME_LEN, "ADD%d", i);
		snprintf(rs_mul_names[i], RS_NAME_LEN, "MUL%d", i);
		snprintf(rs_div_names[i], RS_NAME_LEN, "DIV%d", i);
	}
}

