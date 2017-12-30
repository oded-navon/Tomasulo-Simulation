#pragma once
#include <stdio.h>
#include <stdbool.h>
#define RS_NAME_LEN (10)
#define MAX_ITEMS 16
#define TAG_LEN (10)


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