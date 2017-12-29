#include "ArgParser.h"
#include <string.h>
#include <stdlib.h>

#define MAX_STR_LEN (100)

bool parse_file_line(char* line, int* output_config_value, char** output_config_arg);
bool insert_arg_to_struct(struct config_args* output_config_args, char* arg, int arg_val);

/*enum config_args_enum {
	add_nr_units,
	mul_nr_units,
	div_nr_units,
	add_nr_reservation,
	mul_nr_reservation,
	div_nr_reservation,
	add_delay,
	mul_delay,
	div_delay,
	mem_delay,
	mem_nr_load_buffers,
	mem_nr_store_buffers
};*/

bool parse_config_file(char* config_file_path, struct config_args* output_config_args)
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
		if (!parse_file_line(line, &output_config_value, output_config_arg))
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

bool parse_file_line(char* line, int* output_config_value, char* output_config_arg)
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
	*output_config_value = atoi(arg_value);

	return true;
}

/*bool get_line(FILE* file, char** output_line)
{
	return (fscanf(file, "%[^\n]", *output_line) > 0);
}*/

bool insert_arg_to_struct(struct config_args* output_config_args, char* arg, int arg_val)
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