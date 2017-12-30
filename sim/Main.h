#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <stdbool.h>
#include "ArgParser.h"

#define SUCCESS (0)
#define FAIL (-1)
#define MEMORY_IMAGE_INPUT_SIZE (4096)
#define MAX_INST_NUM (4096)
#define MAX_CDB_NUM (4096)

int parse_args(char* argv[]);
typedef struct {
	int dst;
	int rs1;
	int rs2;
	int action_type;
}RS;

typedef struct {
	int timer;
	int r_d;
	int rs0;
	int rs1;
	int calc_type;
}calc_unit;