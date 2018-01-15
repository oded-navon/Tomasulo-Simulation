#include "ArgParser.h"
#include <string.h>
#include <stdlib.h>

#define MAX_STR_LEN (100)
#define HALT_OPCODE (6)

bool parse_config_file_line(char* line, config_args* output_config_args);
bool insert_arg_to_struct(config_args* output_config_args, char* arg, int arg_val);
bool parse_memin_file_line(char* line, int* memory_word);
bool parse_file(char* file_path, parse_type parsing_type, void** output_object);
int convert_mem_to_inst(int* memory_image, inst** output_insts);

double parse_double_number(char* double_number);
int parse_int_number(char* int_number);
int parse_hex_number(char* hex_number);

extern config_args* _config_args_read;
extern int _memory_image_input[MEMORY_IMAGE_INPUT_SIZE];
extern inst* _instructions[MAX_INST_NUM];
extern int _num_of_inst;
extern char* _trace_cdb_file_path;

int parse_args(char* argv[])
{
	//command line input
	//sim cfg.txt memin.txt memout.txt regout.txt traceinst.txt tracecdb.txt
	char* config_file_path = argv[1];
	char* memory_in_path = argv[2];
	_trace_cdb_file_path = argv[6];
	//checking if the tracecdb file exists already, if it does- delete it since we append to it.
	FILE* _trace_file_check = fopen(_trace_cdb_file_path, "r");
	if (_trace_file_check != NULL)
	{
		fclose(fopen(_trace_cdb_file_path, "w"));
	}

	int return_value = SUCCESS;
	cleanup_type cleanup_ret = cleanup_config;

	_config_args_read = malloc(sizeof(config_args));
	if (_config_args_read == NULL)
	{
		return_value = FAIL;
		cleanup_ret = cleanup_config;
		goto cleanup_on_fail;
	}

	if (!parse_file(config_file_path, config_parse, &_config_args_read))
	{
		return_value = FAIL;
		cleanup_ret = cleanup_config;
		goto cleanup_on_fail;
	}

	if (!parse_file(memory_in_path, memin_parse, &_memory_image_input))
	{
		return_value = FAIL;
		cleanup_ret = cleanup_config;
		goto cleanup_on_fail;
	}

	for (int i = 0; i < MAX_INST_NUM; i++)
	{
		_instructions[i] = malloc(sizeof(inst));
		if (_instructions[i] == NULL)
		{
			return_value = FAIL;
			cleanup_ret = cleanup_inst_and_config;
			goto cleanup_on_fail;
		}
	}
	_num_of_inst = convert_mem_to_inst(_memory_image_input, _instructions);
	init_inst_ex_array();

	goto ret;

cleanup_on_fail:
	cleanup(cleanup_ret);
ret:
	return return_value;
}

int convert_mem_to_inst(int* memory_image, inst** output_insts)
{
	int i;
	for (i = 0; i < MAX_INST_NUM; i++)
	{
		// Check first what is the opcode of the inst, in case it's a HALT inst
		output_insts[i]->opcode = (memory_image[i] & inst_params_opcode) >> 24;
		
		if (output_insts[i]->opcode == HALT_OPCODE)
		{
			// The other fields of the struct are already initialized to 0 so it's ok to break
			break;
		}
		
		output_insts[i]->imm = memory_image[i] & inst_params_imm;
		output_insts[i]->src1_index = (memory_image[i] & inst_params_src1) >> 12;
		output_insts[i]->src0_index = (memory_image[i] & inst_params_src0) >> 16;
		output_insts[i]->dst = (memory_image[i] & inst_params_dst) >> 20;
	}

	// return the number of instructions found including HALT
	return i+1;
}



bool parse_file(char* file_path, parse_type parsing_type, void** output_object)
{
	bool return_value = true;
	FILE* file = fopen(file_path, "r");
	if (file == NULL)
	{
		return false;
	}

	char* line = (char*)malloc(MAX_STR_LEN);
	int cnt = 0;
	bool parsed_correctly = false;
	while (fgets(line, MAX_STR_LEN, file) != NULL)
	{
		int* num_output = 0;
		switch (parsing_type)
		{
			case config_parse:
				parsed_correctly = parse_config_file_line(line, *output_object);
				break;
			case memin_parse:
				num_output = ((int*)output_object) + cnt;
				parsed_correctly = parse_memin_file_line(line, num_output);
				break;
			default:
				parsed_correctly = false;
		}
		
		if (!parsed_correctly)
		{
			return_value = false;
			goto cleanup;
		}
		memset(line, 0, MAX_STR_LEN);
		cnt++;
	}

cleanup:
	fclose(file);
	free(line);
	return return_value;
}

bool parse_memin_file_line(char* line, int* memory_word)
{
	*memory_word = parse_hex_number(line);
	return true;
}

bool parse_config_file_line(char* line, config_args* output_config_args)
{
	char* token = strtok(line, " = ");
	if (token == NULL)
	{
		return false;
	}
	char output_config_arg[MAX_STR_LEN];
	strncpy(output_config_arg, token, sizeof(output_config_arg));

	char* arg_value = strtok(NULL, " = ");
	if (arg_value == NULL)
	{
		return false;
	}
	int output_config_value = parse_int_number(arg_value);

	return insert_arg_to_struct(output_config_args, output_config_arg, output_config_value);
}

bool insert_arg_to_struct(config_args* output_config_args, char* arg, int arg_val)
{
	if (strncmp(arg, "add_nr_units", MAX_STR_LEN) == 0)
	{
		output_config_args->add_nr_units = arg_val;
	}
	else if (strncmp(arg, "mul_nr_units", MAX_STR_LEN) == 0)
	{
		output_config_args->mul_nr_units = arg_val;
	}
	else if (strncmp(arg, "div_nr_units", MAX_STR_LEN) == 0)
	{
		output_config_args->div_nr_units = arg_val;
	}
	else if (strncmp(arg, "add_nr_reservation", MAX_STR_LEN) == 0)
	{
		output_config_args->add_nr_reservation = arg_val;
	}
	else if (strncmp(arg, "mul_nr_reservation", MAX_STR_LEN) == 0)
	{
		output_config_args->mul_nr_reservation = arg_val;
	}
	else if (strncmp(arg, "div_nr_reservation", MAX_STR_LEN) == 0)
	{
		output_config_args->div_nr_reservation = arg_val;
	}
	else if (strncmp(arg, "add_delay", MAX_STR_LEN) == 0)
	{
		output_config_args->add_delay = arg_val;
	}
	else if (strncmp(arg, "mul_delay", MAX_STR_LEN) == 0)
	{
		output_config_args->mul_delay = arg_val;
	}
	else if (strncmp(arg, "div_delay", MAX_STR_LEN) == 0)
	{
		output_config_args->div_delay = arg_val;
	}
	else if (strncmp(arg, "mem_delay", MAX_STR_LEN) == 0)
	{
		output_config_args->mem_delay = arg_val;
	}
	else if (strncmp(arg, "mem_nr_load_buffers", MAX_STR_LEN) == 0)
	{
		output_config_args->mem_nr_load_buffers = arg_val;
	}
	else if (strncmp(arg, "mem_nr_store_buffers", MAX_STR_LEN) == 0)
	{
		output_config_args->mem_nr_store_buffers = arg_val;
	}
	else
	{
		return false;
	}
	return true;
}

int parse_hex_number(char* hex_number)
{
	return (int)strtol(hex_number, NULL, 16);
}

int parse_int_number(char* int_number)
{
	return atoi(int_number);
}

double parse_double_number(char* double_number)
{
	return atof(double_number);
}