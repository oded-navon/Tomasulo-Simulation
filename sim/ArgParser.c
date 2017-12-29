#include "ArgParser.h"
#include <string.h>
#include <stdlib.h>

#define MAX_STR_LEN (100)



bool parse_config_file_line(char* line, int* output_config_value, char* output_config_arg);
bool insert_arg_to_struct(config_args* output_config_args, char* arg, int arg_val);
bool parse_line(char* line, parse_type line_parsing_type, void* output);
bool parse_cdb_file_line(char* line, CDB* output_cdb);
bool parse_memin_file_line(char* line, int* memory_word);
bool parse_inst_file_line(char* line, inst* output_inst_arg);
double parse_double_number(char* double_number);
int parse_int_number(char* int_number);
int parse_hex_number(char* hex_number);

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
		parsed_correctly = parse_line(line, parsing_type, output_object[cnt]);
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

bool parse_line(char* line, parse_type line_parsing_type, void* output)
{
	switch (line_parsing_type)
	{
		case config_line:
			return parse_cdb_file_line(line, output);
		case memin_line:
			return parse_memin_file_line(line, output);
		case inst_line:
			return parse_inst_file_line(line, output);
		case cdb_line:
			return parse_cdb_file_line(line, output);
		default:
			return false;
	}
}


bool parse_cdb_file(char* cdb_file_path, CDB** output_cdb_args)
{
	bool return_value = true;
	FILE* cdb_file = fopen(cdb_file_path, "r");
	if (cdb_file == NULL)
	{
		return false;
	}

	char* line = (char*)malloc(MAX_STR_LEN);
	int cnt = 0;
	bool parse_res = false;
	while (fgets(line, MAX_STR_LEN, cdb_file) != NULL)
	{
		parse_res = parse_cdb_file_line(line, output_cdb_args[cnt]);
		if (!parse_res)
		{
			return_value = false;
			goto cleanup;
		}
		memset(line, 0, MAX_STR_LEN);
		cnt++;
	}

cleanup:
	fclose(cdb_file);
	free(line);
	return return_value;
}

bool parse_cdb_file_line(char* line, CDB* output_cdb)
{
	char* token = strtok(line, " ");
	if (token == NULL)
	{
		return false;
	}

	output_cdb->cycle = parse_int_number(token);

	token = strtok(NULL, " ");
	if (token == NULL)
	{
		return false;
	}
	output_cdb->pc = parse_int_number(token);

	token = strtok(NULL, " ");
	if (token == NULL)
	{
		return false;
	}
	strncpy(output_cdb->cdb_name, token, MAX_STR_LEN);

	token = strtok(NULL, " ");
	if (token == NULL)
	{
		return false;
	}
	output_cdb->data = parse_double_number(token);

	token = strtok(NULL, " ");
	if (token == NULL)
	{
		return false;
	}
	strncpy(output_cdb->tag, token, MAX_STR_LEN);

	return true;
}

bool parse_traceinst_file(char* traceinst_file_path, inst** output_inst_args)
{
	bool return_value = true;
	FILE* traceinst_file = fopen(traceinst_file_path, "r");
	if (traceinst_file == NULL)
	{
		return false;
	}

	char* line = (char*)malloc(MAX_STR_LEN);
	if (line == NULL)
	{
		return_value = false;
		goto cleanup_file;
	}
	int cnt = 0;
	bool is_parsed_correctly = false;
	while (fgets(line, MAX_STR_LEN, traceinst_file) != NULL)
	{
		is_parsed_correctly = parse_inst_file_line(line, output_inst_args[cnt]);
		if (!is_parsed_correctly)
		{
			return_value = false;
			goto cleanup;
		}
		memset(line, 0, MAX_STR_LEN);
		cnt++;
	}

cleanup:
	free(line);
cleanup_file:
	fclose(traceinst_file);
	
	return return_value;
}

bool parse_inst_file_line(char* line, inst* output_inst_arg)
{
	char* token = strtok(line, " ");
	if (token == NULL)
	{
		return false;
	}

	output_inst_arg->inst_code = parse_hex_number(token);

	token = strtok(NULL, " ");
	if (token == NULL)
	{
		return false;
	}
	output_inst_arg->pc = parse_int_number(token);

	token = strtok(NULL, " ");
	if (token == NULL)
	{
		return false;
	}
	strncpy(output_inst_arg->tag, token, MAX_STR_LEN);

	token = strtok(NULL, " ");
	if (token == NULL)
	{
		return false;
	}
	output_inst_arg->cycle_issued = parse_int_number(token);

	token = strtok(NULL, " ");
	if (token == NULL)
	{
		return false;
	}
	output_inst_arg->cycle_ex_start = parse_int_number(token);

	token = strtok(NULL, " ");
	if (token == NULL)
	{
		return false;
	}
	output_inst_arg->cycle_ex_end = parse_int_number(token);

	token = strtok(NULL, " ");
	if (token == NULL)
	{
		return false;
	}
	output_inst_arg->write_cdb = parse_int_number(token);

	return true;
}

bool parse_memin_file_line(char* line, int* memory_word)
{
	*memory_word= parse_hex_number(line);
	return true;
}

bool parse_memin_file(char* memin_file_path, int* output_memory_image)
{
	bool return_value = true;
	FILE* memin_file = fopen(memin_file_path, "r");
	if (memin_file == NULL)
	{
		return false;
	}

	char* line = (char*)malloc(MAX_STR_LEN);
	int cnt = 0;
	while (fgets(line, MAX_STR_LEN, memin_file) != NULL)
	{
		int number = parse_hex_number(line);
		output_memory_image[cnt] = number;

		memset(line, 0, MAX_STR_LEN);
		cnt++;
	}

	fclose(memin_file);
	free(line);
	return return_value;
}

bool parse_config_file(char* config_file_path, config_args* output_config_args)
{
	bool return_value = true;
	FILE* config_file = fopen(config_file_path, "r");
	if (config_file == NULL)
	{
		return false;
	}
	
	char* line = (char*) malloc(MAX_STR_LEN);
	while (fgets(line, MAX_STR_LEN, config_file) != NULL)
	{
		int output_config_value;
		char output_config_arg[MAX_STR_LEN];
		if (!parse_config_file_line(line, &output_config_value, output_config_arg))
		{
			return_value = false;
			goto cleanup;
		}
		
		if (!insert_arg_to_struct(output_config_args, output_config_arg, output_config_value))
		{
			return_value = false;
			goto cleanup;
		}

		memset(line, 0, MAX_STR_LEN);
	}

cleanup:
	fclose(config_file);
	free(line);
	return return_value;
}

bool parse_config_file_line(char* line, int* output_config_value, char* output_config_arg)
{
	char* token = strtok(line, " = ");
	if (token == NULL)
	{
		return false;
	}
	strncpy(output_config_arg, token, MAX_STR_LEN);

	char* arg_value = strtok(NULL, " = ");
	if (arg_value == NULL)
	{
		return false;
	}
	*output_config_value = parse_int_number(arg_value);

	return true;
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