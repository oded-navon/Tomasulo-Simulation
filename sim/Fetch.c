#include "Fetch.h"
#include "Queue.h"

extern int _current_inst_in_instructions;
extern int _num_of_inst;
extern inst* _instructions[MAX_INST_NUM];
extern inst_queue* _iq_arr;

void fetch_instruction();

void fetch_instruction()
{
	if (!is_queue_full(_iq_arr) && (_current_inst_in_instructions < _num_of_inst))
	{
		enqueue(_instructions[_current_inst_in_instructions]);
		_current_inst_in_instructions++;
	}
}

void fetch()
{
	fetch_instruction();
	fetch_instruction();
}
