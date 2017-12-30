#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdbool.h>
#include "Utils.h"

#define CDB_NAME_LEN (5)

typedef enum {
	config_parse,
	memin_parse,
	inst_parse,
	cdb_parse
} parse_type;

typedef struct {
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
}config_args;





typedef struct {
	int cycle;
	int pc;
	char cdb_name[CDB_NAME_LEN];
	double data;
	char tag[TAG_LEN];
}CDB;

bool parse_file(char* file_path, parse_type parsing_type, void** output_object, int* counter);