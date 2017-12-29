#include "Main.h"


int main(int argc, char* argv[])
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

	config_args* config_args_read = malloc(sizeof(config_args));
	if (config_args_read == NULL)
	{
		return_value = FAIL;
		goto cleanup_config;
	}
	
	if (!parse_config_file(config_file_path, config_args_read))
	{
		return_value = FAIL;
		goto cleanup_config;
	}

	int memory_image_input[MEMORY_IMAGE_INPUT_SIZE];
	if (!parse_memin_file(memory_in_path, memory_image_input))
	{
		return_value = FAIL;
		goto cleanup_config;
	}

	inst** instructions = malloc(sizeof(inst)*MAX_INST_NUM);
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

	if (!parse_traceinst_file(trace_inst_path, instructions))
	{
		return_value = FAIL;
		goto cleanup_inst;
	}

	CDB** cdb = malloc(sizeof(CDB)*MAX_CDB_NUM);
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

	if (!parse_cdb_file(trace_cdb_path, cdb))
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

