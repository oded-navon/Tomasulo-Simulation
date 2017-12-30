#pragma once
#include <stdio.h>
#include <stdbool.h>

#define RS_NAME_LEN (10)
<<<<<<< HEAD
#define MAX_ITEMS 16
#define TAG_LEN (10)

=======
#define TAG_LEN (10)
#define CDB_NAME_LEN (5)
#define MEMORY_IMAGE_INPUT_SIZE (4096)
#define MAX_INST_NUM (4096)
#define MAX_CDB_NUM (4096)
>>>>>>> dfcba872cdf65cd25d8a3dbb8197fd444db1fe4f

typedef struct {
	int dst;
	int src0;
	int src1;
	char rs_waiting0[RS_NAME_LEN];
	char rs_waiting1[RS_NAME_LEN];
	int action_type;
	bool occupied;
}RS;

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
	int timer;
	int dst;
<<<<<<< HEAD
	int rs0;
	int rs1;
	int calc_type;
}calc_unit;

typedef struct {
	inst inst_arr[MAX_ITEMS];
	int front;
	int last;
	int num_items;
}iq;

int peek(iq iq_arr);
void enqueue(iq data);

int dequeue(iq iq_arr);
bool is_queue_full(iq iq_arr);
=======
	int src0;
	int src1;
	calc_unit_type calc_type;
}calc_unit;

typedef enum {
	config_parse,
	memin_parse
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

typedef enum {
	ADD,
	MUL,
	DIV
}calc_unit_type;
>>>>>>> dfcba872cdf65cd25d8a3dbb8197fd444db1fe4f
