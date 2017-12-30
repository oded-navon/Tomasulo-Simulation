#include "Main.h"


config_args* config_args_read;
inst* instructions[MAX_INST_NUM];
int memory_image_input[MEMORY_IMAGE_INPUT_SIZE];
CDB** cdb;

typedef enum {
	cleanup_all,
	cleanup_inst_and_config,
	cleanup_config
} cleanup_type;

void cleanup(cleanup_type clean_type);


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
	cleanup_type cleanup_ret = cleanup_all;

	config_args_read = malloc(sizeof(config_args));
	if (config_args_read == NULL)
	{
		return_value = FAIL;
		cleanup_ret = cleanup_config;
		goto cleanup;
	}

	if (!parse_file(config_file_path, config_parse, &config_args_read))
	{
		return_value = FAIL;
		cleanup_ret = cleanup_config;
		goto cleanup;
	}

	if (!parse_file(memory_in_path, memin_parse, &memory_image_input))
	{
		return_value = FAIL;
		cleanup_ret = cleanup_config;
		goto cleanup;
	}

	//instructions = malloc(sizeof(inst)*MAX_INST_NUM);
	if (instructions == NULL)
	{
		return_value = FAIL;
		cleanup_ret = cleanup_inst_and_config;
		goto cleanup;
	}
	for (int i = 0; i < MAX_INST_NUM; i++)
	{
		instructions[i] = malloc(sizeof(inst));
		if (instructions[i] == NULL)
		{
			return_value = FAIL;
			cleanup_ret = cleanup_inst_and_config;
			goto cleanup;
		}
	}

	if (!parse_file(trace_inst_path, inst_parse, instructions))
	{
		return_value = FAIL;
		cleanup_ret = cleanup_inst_and_config;
		goto cleanup;
	}

	cdb = malloc(sizeof(CDB)*MAX_CDB_NUM);
	if (cdb == NULL)
	{
		return_value = FAIL;
		cleanup_ret = cleanup_all;
		goto cleanup;
	}
	for (int i = 0; i < MAX_CDB_NUM; i++)
	{
		cdb[i] = malloc(sizeof(CDB));
		if (cdb[i] == NULL)
		{
			return_value = FAIL;
			cleanup_ret = cleanup_all;
			goto cleanup;
		}
	}

	if (!parse_file(trace_cdb_path, cdb_parse, cdb))
	{
		return_value = FAIL;
		cleanup_ret = cleanup_all;
		goto cleanup;
	}

cleanup:
	cleanup(cleanup_ret);

	return return_value;
}

void cleanup(cleanup_type clean_type)
{
	switch (clean_type)
	{
		case cleanup_all:
			for (int i = 0; i < MAX_CDB_NUM; i++)
			{
				free(cdb[i]);
			}
			free(cdb);
		case cleanup_inst_and_config:
			for (int i = 0; i < MAX_INST_NUM; i++)
			{
				free(instructions[i]);
			}
		case cleanup_config:
			free(config_args_read);
	}
}