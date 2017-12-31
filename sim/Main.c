#include "Main.h"

#include "ArgParser.h"
#include "Fetch.h"
#include "Dispatch.h"
#include "Issue.h"
#include "Queue.h"
#include "Execute.h"
#include "Broadcast.h"
#include "Globals.c"

extern int last_unoccupied_index_in_iq;
extern unsigned int cycles;
extern int num_of_inst;
extern int num_of_cdb;
extern int current_inst_in_instructions;
extern iq* iq_arr;

//for logging the run
extern inst_ex* _instructions_executed[MAX_INST_NUM];
extern CDB** cdb;

//for input
extern int _memory_image_input[MEMORY_IMAGE_INPUT_SIZE];
extern config_args* _config_args_read;
extern inst* _instructions[MAX_INST_NUM];

//mapping between register to reservation station
extern RS RAT[16];

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
	iq_arr = malloc(sizeof(iq));
	iq_arr->front = 0;
	iq_arr->last = -1;
	iq_arr->num_items = 0;

	int ret_args = parse_args(argv);
	if (ret_args == -1)
	{
		return FAIL;
	}
	//while (instr_proc < instr_count) {
	while (current_inst_in_instructions <= num_of_inst) {

		cycles++;
		fetch();

		int number_of_instructions_issued = issue();

		Dispatch();

		Execute();

		//write_back();
		Broadcast();

	}

	cleanup(cleanup_inst_and_config);
	free(iq_arr);
	return return_value;
}


