#pragma once
#include "Utils.h"

bool write_memory_output(char* memout_file); 
bool write_reg_output(char* regout_file);
void write_cdb_trace_to_file(int cycle, int inst_pc, inst_opcodes unit_type, float data, char* tag);