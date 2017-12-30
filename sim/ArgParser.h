#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdbool.h>
#define TAG_LEN (10)
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
	int inst_code;
	int pc;
	char tag[TAG_LEN];
	int cycle_issued;
	int cycle_ex_start;
	int cycle_ex_end;
	int write_cdb;
}inst;

typedef struct {
	int cycle;
	int pc;
	char cdb_name[CDB_NAME_LEN];
	double data;
	char tag[TAG_LEN];
}CDB;

bool parse_config_file(char* config_file_path, config_args* output_config_args);
bool parse_memin_file(char* memin_file_path, int* output_memory_image);
bool parse_traceinst_file(char* traceinst_file_path, inst** output_inst_args);
bool parse_cdb_file(char* cdb_file_path, CDB** output_cdb_args);
bool parse_file(char* file_path, parse_type parsing_type, void** output_object);