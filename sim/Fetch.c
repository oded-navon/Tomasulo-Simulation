#include "Fetch.h"
#include "Queue.h"

extern int _current_inst_in_instructions;
extern int _num_of_inst;
extern inst* _instructions[MAX_INST_NUM];
extern inst_queue* _iq_arr;
extern bool received_halt_in_fetch; //means to stop do fetches

void fetch_instruction();

void fetch_instruction()
{
	if (!is_queue_full(_iq_arr) && (_current_inst_in_instructions < _num_of_inst))
	{
		if (_instructions[_current_inst_in_instructions]->opcode == HALT_opcode)
		{
			received_halt_in_fetch = true;
		}
		enqueue(_instructions[_current_inst_in_instructions]);
		_current_inst_in_instructions++;
	}
}

void Fetch()
{
	if (!received_halt_in_fetch)
	{
		fetch_instruction();
	}
	if (!received_halt_in_fetch)
	{
		fetch_instruction();
	}
}
