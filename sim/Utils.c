#include "Utils.h"

extern int _memory_image_input[MEMORY_IMAGE_INPUT_SIZE];
extern config_args* _config_args_read;
extern inst* _instructions[MAX_INST_NUM];

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