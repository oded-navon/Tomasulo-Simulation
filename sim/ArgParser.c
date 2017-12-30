#include "ArgParser.h"
#include <string.h>
#include <stdlib.h>

#define MAX_STR_LEN (100)


bool parse_config_file_line(char* line, config_args* output_config_args);
bool insert_arg_to_struct(config_args* output_config_args, char* arg, int arg_val);
bool parse_cdb_file_line(char* line, CDB* output_cdb);
bool parse_memin_file_line(char* line, int* memory_word);
bool parse_inst_file_line(char* line, inst* output_inst_arg);
double parse_double_number(char* double_number);
int parse_int_number(char* int_number);
int parse_hex_number(char* hex_number);


bool parse_file(char* file_path, parse_type parsing_type, void** output_object, int* counter)
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
			case inst_parse:
				parsed_correctly = parse_inst_file_line(line, ((inst**)output_object)[cnt]);
				break;
			case cdb_parse:
				parsed_correctly = parse_cdb_file_line(line, ((CDB**)output_object)[cnt]);
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
	if (counter != NULL)
	{
		*counter = cnt;
	}
	fclose(file);
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
	strncpy(output_cdb->cdb_name, token, sizeof(output_cdb->cdb_name));

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
	strncpy(output_cdb->tag, token, sizeof(output_cdb->tag));

	return true;
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
	strncpy(output_inst_arg->tag, token, sizeof(output_inst_arg->tag));

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