#include "Main.h"

config_args* config_args_read;
inst** instructions;
int memory_image_input[MEMORY_IMAGE_INPUT_SIZE];
CDB** cdb;

int main(int argc, char* argv[])
{



	int return_value = SUCCESS;
	int ret_args = parse_args(argv);
	if (ret_args == -1)
	{
		return FAIL;
	}
	

	return return_value;
}

int parse_args(char* argv[])
{
	//command line input
	//sim cfg.txt memin.txt memout.txt regout.txt traceinst.txt tracecdb.txt
	char* config_file_path = argv[1];
	char* memory_in_path = argv[2];
	char* memory_out_path = argv[3];
	char* reg_out_path = argv[4];
	char* trace_inst_path = argv[5];
	char* trace_cdb_path = argv[6];

	int return_value = SUCCESS;
	
	config_args_read = malloc(sizeof(config_args));
	if (config_args_read == NULL)
	{
		return_value = FAIL;
		goto cleanup_config;
	}

	if (!parse_file(config_file_path, config_parse, &config_args_read))
	{
		return_value = FAIL;
		goto cleanup_config;
	}

	if (!parse_file(memory_in_path, memin_parse, &memory_image_input))
	{
		return_value = FAIL;
		goto cleanup_config;
	}

	instructions = malloc(sizeof(inst)*MAX_INST_NUM);
	if (instructions == NULL)
	{
		return_value = FAIL;
		goto cleanup_inst;
	}
	for (int i = 0; i < MAX_INST_NUM; i++)
	{
		instructions[i] = malloc(sizeof(inst));
		if (instructions[i] == NULL)
		{
			return_value = FAIL;
			goto cleanup_all;
		}
	}

	if (!parse_file(trace_inst_path, inst_parse, instructions))
	{
		return_value = FAIL;
		goto cleanup_inst;
	}

	cdb = malloc(sizeof(CDB)*MAX_CDB_NUM);
	if (cdb == NULL)
	{
		return_value = FAIL;
		goto cleanup_all;
	}
	for (int i = 0; i < MAX_CDB_NUM; i++)
	{
		cdb[i] = malloc(sizeof(CDB));
		if (cdb[i] == NULL)
		{
			return_value = FAIL;
			goto cleanup_all;
		}
	}

	if (!parse_file(trace_cdb_path, cdb_parse, cdb))
	{
		return_value = FAIL;
		goto cleanup_all;
	}


cleanup_all:
	for (int i = 0; i < MAX_CDB_NUM; i++)
	{
		free(cdb[i]);
	}
	free(cdb);
cleanup_inst:
	for (int i = 0; i < MAX_INST_NUM; i++)
	{
		free(instructions[i]);
	}
	free(instructions);
cleanup_config:
	free(config_args_read);

	return return_value;

}