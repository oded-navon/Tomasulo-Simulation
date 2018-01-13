#define _CRT_SECURE_NO_WARNINGS
#include "Tracer.h"
#include "Globals.c"


extern int _memory_image_input[MEMORY_IMAGE_INPUT_SIZE];
extern float _regs[NUM_OF_REGS];
extern unsigned int _cycles;
extern int _num_of_inst;
extern int _num_of_cdb;

bool write_memory_output(char* memout_file);
bool write_reg_output(char* regout_file);
bool write_inst_output(char * traceinst_file);

bool write_outputs(char * memout_file, char * regout_file, char * traceinst_file)
{
	if (!write_memory_output(memout_file))
	{
		return false;
	}
	if (!write_reg_output(regout_file))
	{
		return false;
	}
	if (!write_inst_output(traceinst_file))
	{
		return false;
	}
	return true;
}

bool write_memory_output(char * memout_file)
{
	bool return_value = true;
	FILE* file = fopen(memout_file, "w");
	if (file == NULL)
	{
		return false;
	}

	int check_ret = 0;
	for (int i = 0; i < MEMORY_IMAGE_INPUT_SIZE; i++)
	{
		check_ret = fprintf(file,"%08X\n", _memory_image_input[i]);
		if (check_ret < 0)
		{
			return_value = false;
			break;
		}
	}
	fclose(file);
	return return_value;
}

bool write_reg_output(char * regout_file)
{
	bool return_value = true;
	FILE* file = fopen(regout_file, "w");
	if (file == NULL)
	{
		return false;
	}

	int check_ret = 0;
	for (int i = 0; i < NUM_OF_REGS; i++)
	{
		check_ret = fprintf(file,"%f\n", _regs[i]);
		if (check_ret < 0)
		{
			return_value = false;
			break;
		}
	}
	fclose(file);
	return return_value;
}

bool write_inst_output(char * traceinst_file)
{
	bool return_value = true;
	FILE* file = fopen(traceinst_file, "w");
	if (file == NULL)
	{
		return false;
	}

	int check_ret = 0;
	for (int i = 0; i < _num_of_inst; i++)
	{
		char line_to_print[MAX_LINE_LEN];
		inst_ex* output_inst_arg = _instructions[i]->inst_log;
		
		check_ret = sprintf(line_to_print,
							"%08X %d %s %d %d %d %d\n", 
							output_inst_arg->inst_code, 
							output_inst_arg->pc, 
							output_inst_arg->tag, 
							output_inst_arg->cycle_issued, 
							output_inst_arg->cycle_ex_start, 
							output_inst_arg->cycle_ex_end, 
							output_inst_arg->write_cdb
							);

		if (check_ret < 0)
		{
			return_value = false;
			break;
		}

		check_ret = fprintf(file, line_to_print);
		if (check_ret < 0)
		{
			return_value = false;
			break;
		}
	}
	fclose(file);
	return return_value;
}

void write_cdb_trace_to_file(int cycle, int inst_pc, inst_opcodes unit_type, float data, char* tag)
{
	FILE* file = fopen(_trace_cdb_file_path, "a+");
	if (file == NULL)
	{
		return;
	}
	char line_to_print[MAX_LINE_LEN];
	char cdb_name[CDB_NAME_LEN];
	
	switch (unit_type)
	{
		case LD_opcode:
		case ST_opcode:
			snprintf(cdb_name, CDB_NAME_LEN, "MEM");
			break;
		case ADD_opcode:
		case SUB_opcode:
			snprintf(cdb_name, CDB_NAME_LEN, "ADD");
			break;
		case MULT_opcode:
			snprintf(cdb_name, CDB_NAME_LEN, "MUL");
			break;
		case DIV_opcode:
			snprintf(cdb_name, CDB_NAME_LEN, "DIV");
			break;
		case HALT_opcode:
			break;
	}
	sprintf(line_to_print,
		"%d %d %s %f %s\n",
		cycle,
		inst_pc,
		cdb_name,
		data,
		tag
	);
	fclose(file);
}