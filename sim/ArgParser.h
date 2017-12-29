#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdbool.h>


struct config_args {
	int add_nr_units;
	int mul_nr_units;
	int div_nr_units;
	int add_nr_reservation;
	int mul_nr_reservation;
	int div_nr_reservation;
	int	add_delay;
	int	mul_delay;
	int	div_delay;
	int	mem_delay;
	int	mem_nr_load_buffers;
	int	mem_nr_store_buffers;
};

bool parse_config_file(char* config_file_path, struct config_args* output_config_args);
bool parse_memin_file(char* memin_file_path, int* output_memory_image);