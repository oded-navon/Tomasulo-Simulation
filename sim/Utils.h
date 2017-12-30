#pragma once
#include <stdio.h>
#include <stdbool.h>
#define RS_NAME_LEN (10)
	
typedef struct {
	int dst;
	int src0;
	int src1;
	char rs_waiting0[RS_NAME_LEN];
	char rs_waiting1[RS_NAME_LEN];
	int action_type;
}RS;


typedef struct {
	int timer;
	int dst;
	int rs0;
	int rs1;
	int calc_type;
}calc_unit;