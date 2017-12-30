#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdbool.h>

#define TAG_LEN (10)
#define CDB_NAME_LEN (5)
#define MEMORY_IMAGE_INPUT_SIZE (4096)
#define MAX_INST_NUM (4096)
#define MAX_CDB_NUM (4096)

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

typedef enum {
	inst_params_imm = 4095,        // 00000000000000000000111111111111
	inst_params_src1 = 61440,      // 00000000000000001111000000000000
	inst_params_src0 = 983040,     // 00000000000011110000000000000000
	inst_params_dst = 15728640,    // 00000000111100000000000000000000
	inst_params_opcode = 251658240 // 00001111000000000000000000000000
}inst_params;

typedef struct {
	int opcode;
	int imm;
	int src0;
	int src1;
	int dst;
} inst;

typedef struct {
	int inst_code;
	int pc;
	char tag[TAG_LEN];
	int cycle_issued;
	int cycle_ex_start;
	int cycle_ex_end;
	int write_cdb;
}inst_ex;

typedef struct {
	int cycle;
	int pc;
	char cdb_name[CDB_NAME_LEN];
	double data;
	char tag[TAG_LEN];
}CDB;

bool parse_file(char* file_path, parse_type parsing_type, void** output_object, int* counter);
void convert_mem_to_inst(int* memory_image, inst** output_insts);