#pragma once
#include "Utils.h"

bool write_outputs(char * memout_file, char * regout_file, char * traceinst_file);
void write_cdb_trace_to_file(int cycle, int inst_pc, inst_opcodes unit_type, float data, char* tag);