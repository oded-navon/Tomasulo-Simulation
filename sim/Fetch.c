#include "Fetch.h"
#include "Queue.h"

extern int current_inst_in_instructions;
extern int num_of_inst;
extern inst* _instructions[MAX_INST_NUM];
extern iq* iq_arr;

void fetch_instruction();

void fetch_instruction()
{
	if (is_queue_full(iq_arr) && (current_inst_in_instructions < num_of_inst))
	{
		enqueue(_instructions[current_inst_in_instructions]);
		current_inst_in_instructions++;
	}
}

void fetch()
{
	fetch_instruction(current_inst_in_instructions, num_of_inst, _instructions, iq_arr);
	fetch_instruction(current_inst_in_instructions, num_of_inst, _instructions, iq_arr);
}
