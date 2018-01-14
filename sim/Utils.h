#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define NAME_LEN (10)
#define MAX_ITEMS (16)
#define TAG_LEN (10)
#define NUM_OF_REGS (16)
#define MAX_LINE_LEN (128)

#define TAG_LEN (10)
#define CDB_NAME_LEN (10)
#define MEMORY_IMAGE_INPUT_SIZE (4096)
#define SUCCESS (0)
#define FAIL (-1)
#define MAX_INST_NUM (4096)
#define MAX_CDB_NUM (4096)
#define MAX_CONFIG_SIZE (64)
#define NO_INSTANCE_AVAILABLE (MAX_CONFIG_SIZE*2)
#define INSTANCE_IS_FREE (-1)
#define INSTANCE_IS_READY (0) //meaning we can perform it's operation


typedef enum {
	ADD_calc_unit,
	MUL_calc_unit,
	DIV_calc_unit
}calc_unit_type;

typedef enum {
	LD_opcode,
	ST_opcode,
	ADD_opcode,
	SUB_opcode,
	MULT_opcode,
	DIV_opcode,
	HALT_opcode
}inst_opcodes;

typedef enum {
	inst_params_imm = 4095,        // 00000000000000000000111111111111
	inst_params_src1 = 61440,      // 00000000000000001111000000000000
	inst_params_src0 = 983040,     // 00000000000011110000000000000000
	inst_params_dst = 15728640,    // 00000000111100000000000000000000
	inst_params_opcode = 251658240 // 00001111000000000000000000000000
}inst_params;

typedef enum {
	cleanup_all,
	cleanup_inst_and_config,
	cleanup_config
} cleanup_type;

typedef enum {
	config_parse,
	memin_parse
} parse_type;



typedef struct {
	bool add_cdb_is_free;
	bool mul_cdb_is_free;
	bool div_cdb_is_free;
	bool mem_cdb_is_free;
}cdb_free;


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
	float data;
	char tag[TAG_LEN];
}CDB_ex;

typedef struct {
	int opcode;
	int imm;
	int src0_index;
	int src1_index;
	int dst;
	inst_ex* inst_log;
} inst;

typedef struct {
	int dst;	//will contain index of dst reg
	float src0;	 //will contain value of _regs[src0]
	float src1;	 //will contain value of _regs[src1]
	int src0_index;
	int src1_index;
	char rs_waiting0[NAME_LEN];
	char rs_waiting1[NAME_LEN];
	inst_opcodes action_type;
	bool occupied;
	bool already_dispatched;
	char name[NAME_LEN];
	inst* curr_inst;
	//These flags are for simulating a concurrent processor operation
	//This flag is for making sure that we don't use the RS if we just broadcasted its execution result
	bool just_broadcasted;
	//This flag is for making sure that we don't dispatch the RS station in the same cycle that we got all of its data
	bool just_got_a_broadcast;
}RS;

typedef struct {
	int timer;
	float src0;	//will contain value of _reg[src0]
	float src1;	//will contain value of _reg[src1]
	int dst;	//will contain index of dst reg
	calc_unit_type calc_type;
	inst* curr_inst;
	char rs_name[NAME_LEN];
	bool just_broadcasted;
}calc_unit;

typedef struct queue_node {
	inst* node_inst;
	struct queue_node* next;
	struct queue_node* prev;
} queue_node_t;

typedef struct {
	queue_node_t* head;
	queue_node_t* tail;
	int num_items_in_queue;
	int queue_max_size;
}inst_queue;

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
	bool occupied;
	char rs_or_buff_name[NAME_LEN];
}RAT_entry;

typedef struct {
	int timer;
	int dst;
	int imm;
	char buff_name[NAME_LEN];
	inst* curr_inst;
	bool just_broadcasted;
}load_buffer;

typedef struct {
	int timer;
	int src1_index;
	float src1;
	int imm;
	char src1_waiting[NAME_LEN];
	char buff_name[NAME_LEN];
	inst* curr_inst;
	bool just_got_a_broadcast;
}store_buffer;



void cleanup(cleanup_type clean_type);
void init_regs();
void init_rs_names_arrays();
void init_buff_names_arrays();
void init_inst_ex_array();
void clear_all_ex_units();
void clear_all_buffers();
void clear_rs_inst(char* inst_to_clear_name);