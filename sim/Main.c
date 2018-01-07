#include "Main.h"

#include "ArgParser.h"
#include "Fetch.h"
#include "Dispatch.h"
#include "Issue.h"
#include "Queue.h"
#include "Execute.h"
#include "Broadcast.h"
#include "Globals.c"

extern int _last_unoccupied_index_in_iq;
extern unsigned int _cycles;
extern int _num_of_inst;
extern int _num_of_cdb;
extern int _current_inst_in_instructions;
extern inst_queue* _iq_arr;

//for logging the run
extern inst_ex* _instructions_executed[MAX_INST_NUM];
extern CDB** _cdb;

//for input
extern int _memory_image_input[MEMORY_IMAGE_INPUT_SIZE];
extern config_args* _config_args_read;
extern inst* _instructions[MAX_INST_NUM];

// RAT - Register Alias Table
// Mapping between registers and reservation station
// When issuing an inst, we check the RAT first to see if the register's value is ready or if it's waiting
// for another inst to complete
extern RAT_entry RAT[NUM_OF_REGS];

// The registers file
extern float _regs[NUM_OF_REGS];

// Reservation tables for each unit type
extern RS rs_add[MAX_CONFIG_SIZE];
extern RS rs_mul[MAX_CONFIG_SIZE];
extern RS rs_div[MAX_CONFIG_SIZE];

// calculation units
extern calc_unit add_units[MAX_CONFIG_SIZE]; //TODO: look into starvation
extern calc_unit div_units[MAX_CONFIG_SIZE];
extern calc_unit mul_units[MAX_CONFIG_SIZE];


int main(int argc, char* argv[])
{
	int return_value = SUCCESS;
	_iq_arr = malloc(sizeof(inst_queue));
	_iq_arr->num_items_in_queue = 0;

	int ret_args = parse_args(argv);
	if (ret_args == -1)
	{
		return FAIL;
	}

	init_rs_names_arrays();
	init_regs();
	clear_all_ex_units();

	_cycles = 0;
	//while (instr_proc < instr_count) {
	while (true){ //_current_inst_in_instructions <= _num_of_inst) {

		Fetch();
		
		Issue();
		
		Dispatch();

		Execute();

		Broadcast();

		_cycles++;

	}

	cleanup(cleanup_inst_and_config);
	free(_iq_arr);
	return return_value;
}


