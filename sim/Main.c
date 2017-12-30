#include "Main.h"
#include "Utils.h"

config_args* config_args_read;
inst** instructions;
iq iq_arr = {
	.front = 0,
	.last = -1,
	.num_items = 0
}; //our instruction queue


inst* instructions[MAX_INST_NUM];
int memory_image_input[MEMORY_IMAGE_INPUT_SIZE];
inst_ex* _instructions_executed[MAX_INST_NUM];
config_args* config_args_read;
inst** instructions;
inst* iq[16]; //our instruction queue
int last_unoccupied_index_in_iq = 0;
inst* _instructions[MAX_INST_NUM];
int _memory_image_input[MEMORY_IMAGE_INPUT_SIZE];
CDB** cdb;
unsigned int cycles = 0;
int num_of_inst;
int num_of_cdb;
int current_inst_in_instructions = 0;
RS RAT[16]; //mapping between register to reservation station
RS rs_add[MAX_CONFIG_SIZE];
RS rs_mul[MAX_CONFIG_SIZE];
RS rs_div[MAX_CONFIG_SIZE];

calc_unit add_units[MAX_CONFIG_SIZE]; //change to struct, if timer -1 we can runover, TODO look into starvation
calc_unit div_units[MAX_CONFIG_SIZE];
calc_unit mul_units[MAX_CONFIG_SIZE];


typedef enum {
	cleanup_all,
	cleanup_inst_and_config,
	cleanup_config
} cleanup_type;

void cleanup(cleanup_type clean_type);


void fetch_instruction()
{
	if (is_queue_full(iq_arr) && (current_inst_in_instructions < num_of_inst))
	{
		enqueue(iq_arr,instructions[current_inst_in_instructions]);
		current_inst_in_instructions++;
	}
}

void fetch()
{
	fetch_instruction();
	fetch_instruction();
}






int main(int argc, char* argv[])
{
	
	int return_value = SUCCESS;
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

		execute();

		//write_back();
		broadcast();

	}

	return return_value;
}


int get_free_reservation_station_index(int opcode)
{
	/*will return an RS from the correct kind for specified instruction
	return:
	null - meaning there's no available RS in this cycle, meaning we won't issue anything in the cycle
	otherwise - available RS*/
	int res;
	switch (opcode)
	{
		case 0:
		case 1:
			return NULL;
		case 2:
		case 3:
			res = get_free_reservation_station_index(rs_add, config_args_read->add_nr_reservation);
			break;
		case 4:
			res = get_free_reservation_station_index(rs_mul, config_args_read->mul_nr_reservation);
			break;
		case 5:
			res = get_free_reservation_station_index(rs_div, config_args_read->div_nr_reservation);
			break;
	}

	return res;
}

int get_free_reservation_station_index(RS rses[], int num_rses)
{
	for (int i = 0; i < num_rses; i++)
	{
		if (!rses[i].occupied) return i;
	}
	return NO_RS_AVAILABLE;
}
//inst** determine_where_inputs_come_from(inst inst)
//{
//	/*will determine wether the inputs for the current instruction are already ready, or are they gonna be produced by some of the instructions that haven't yet broadcasted their results
//	return value:
//	empty array - means that the inputs are ready
//	otherwise - all instructions we must wait for them to be completed*/
//
//	return res;
//}	  
	/*will determine whether the inputs for the current instruction are already ready, or are they gonna be produced by some of the instructions that haven't yet broadcasted their results
	return value:
	empty array - means that the inputs are ready
	otherwise - all instructions we must wait for them to be completed*/
	inst** res = malloc(sizeof(inst)*MAX_INST_NUM);

	return res;
}	  

int issue()
{
	int res = issue_instruction();
	if(res == 1) res += issue_instruction();
	return res;
}

void put_inst_in_RS(int rs_index,inst* instr,int type, int dst, int src0, int src1, char* src0_name, char* src1_name)
{
	switch (type)
	{
	case 0:
	case 1:
		return NULL;
	case 2:
	case 3:
		//res = get_free_reservation_station_index(rs_add, config_args_read->add_nr_reservation);
		/*typedef struct {
			int dst;
			int src0;
			int src1;
			char rs_waiting0[RS_NAME_LEN];
			char rs_waiting1[RS_NAME_LEN];
			int action_type;
			bool occupied;
		}RS;*/

		break;
	case 4:
		res = get_free_reservation_station_index(rs_mul, config_args_read->mul_nr_reservation);
		break;
	case 5:
		res = get_free_reservation_station_index(rs_div, config_args_read->div_nr_reservation);
		break;
	}
	//rs = &rs_type[i];
	//rs->status = BUSY;
	//rs->instr = curr;

	//if (curr->src1)
	//	rs->qj = dep_lookup(curr->src1);

	//if (curr->src2)
	//	rs->qk = dep_lookup(curr->src2);

	//curr->issue_time = cycles;
	//instr_proc++;
}

int issue_instruction()
{
	/*return 1 if issued instruction successfully, else 0*/
	
	//inst* inst = take_next_instruction_from_IQ();
	inst* instr = peek(iq_arr);
	int type = instr->inst_code;
	int rs_index = get_free_reservation_station_index(type);
	if (rs_index == NO_RS_AVAILABLE)	
	{
		return 0;
	}

	put_inst_in_RS(rs_index,instr,type);

	
	//tag_dest_register_of_inst(inst);
	//int i, rs_count;
	//Instruction *curr;
	//RS *rs_type, *rs;
	//curr = &iq[instr_proc];

	///* Based on the opcode type, assign a common looping pointer and a counter {{{ */
	//if (!(strcmp("ADD", curr->opcd)) || !(strcmp("SUB", curr->opcd))) {
	//	rs_count = NUM_INT_ADD_RS; rs_type = add_int_rs;
	//}
	//else if (!(strcmp("ADDD", curr->opcd)) || !(strcmp("SUBD", curr->opcd))) {
	//	rs_count = NUM_FLT_ADD_RS; rs_type = mul_int_rs;
	//}
	//else if (!(strcmp("MUL", curr->opcd)) || !(strcmp("DIV", curr->opcd))) {
	//	rs_count = NUM_INT_MUL_RS; rs_type = add_fp_rs;
	//}
	//else if (!(strcmp("MULD", curr->opcd)) || !(strcmp("DIVD", curr->opcd))) {
	//	rs_count = NUM_FLT_MUL_RS; rs_type = mul_fp_rs;
	//}
	//else if (!(strcmp("LD", curr->opcd))) {
	//	rs_count = NUM_LD_RS; rs_type = ld_rs;
	//}
	//else if (!(strcmp("SD", curr->opcd))) {
	//	rs_count = NUM_SD_RS; rs_type = sd_rs;
	//}
	///* }}} */

	///* Grab the first available RS of the type selected in the previous step {{{ */
	//for (i = 0; i < rs_count; i++)
	//	if (rs_type[i].status == AVAILABLE)
	//		break;

	///* If all the RS are occupied, simply return back without incrementing */
	///* the number of instructions executed. this causes the subsequent issue */
	///* call to process the same instruction for issuing */

	//if (i >= rs_count) return;

	///* }}} */

	///* now, the instruction is ready to be issued. we can get started */
	//rs = &rs_type[i];
	//rs->status = BUSY;
	//rs->instr = curr;

	//if (curr->src1)
	//	rs->qj = dep_lookup(curr->src1);

	//if (curr->src2)
	//	rs->qk = dep_lookup(curr->src2);

	//curr->issue_time = cycles;
	//instr_proc++;
}

int parse_args(char* argv[])
{
	//command line input
	//sim cfg.txt memin.txt memout.txt regout.txt traceinst.txt tracecdb.txt
	char* config_file_path = argv[1];
	char* memory_in_path = argv[2];
	char* memory_out_path = argv[3];
	char* reg_out_path = argv[4];
	char* trace_inst_path = argv[5];
	char* trace_cdb_path = argv[6];

	int return_value = SUCCESS;
	cleanup_type cleanup_ret = cleanup_config;

	_config_args_read = malloc(sizeof(config_args));
	if (_config_args_read == NULL)
	{
		return_value = FAIL;
		cleanup_ret = cleanup_config;
		goto cleanup;
	}

	if (!parse_file(config_file_path, config_parse, &_config_args_read, NULL))
	{
		return_value = FAIL;
		cleanup_ret = cleanup_config;
		goto cleanup;
	}

	if (!parse_file(memory_in_path, memin_parse, &_memory_image_input, NULL))
	{
		return_value = FAIL;
		cleanup_ret = cleanup_config;
		goto cleanup;
	}
	
	for (int i = 0; i < MAX_INST_NUM; i++)
	{
		_instructions[i] = malloc(sizeof(inst));
		if (_instructions[i] == NULL)
		{
			return_value = FAIL;
			cleanup_ret = cleanup_inst_and_config;
			goto cleanup;
		}
	}

	convert_mem_to_inst(_memory_image_input, _instructions);

	/*for (int i = 0; i < MAX_INST_NUM; i++)
	{
		_instructions_executed[i] = malloc(sizeof(inst_ex));
		if (_instructions_executed[i] == NULL)
		{
			return_value = FAIL;
			cleanup_ret = cleanup_inst_and_config;
			goto cleanup;
		}
	}

	if (!parse_file(trace_inst_path, inst_parse, _instructions_executed, &num_of_inst))
	{
		return_value = FAIL;
		cleanup_ret = cleanup_inst_and_config;
		goto cleanup;
	}

	cdb = malloc(sizeof(CDB)*MAX_CDB_NUM);
	if (cdb == NULL)
	{
		return_value = FAIL;
		cleanup_ret = cleanup_all;
		goto cleanup;
	}
	for (int i = 0; i < MAX_CDB_NUM; i++)
	{
		cdb[i] = malloc(sizeof(CDB));
		if (cdb[i] == NULL)
		{
			return_value = FAIL;
			cleanup_ret = cleanup_all;
			goto cleanup;
		}
	}

	if (!parse_file(trace_cdb_path, cdb_parse, cdb, &num_of_cdb))
	{
		return_value = FAIL;
		cleanup_ret = cleanup_all;
		goto cleanup;
	}*/

cleanup:
	cleanup(cleanup_ret);

	return return_value;
}

void cleanup(cleanup_type clean_type)
{
	switch (clean_type)
	{
		/*case cleanup_all:
			for (int i = 0; i < MAX_CDB_NUM; i++)
			{
				free(cdb[i]);
			}
			free(cdb);*/
		case cleanup_inst_and_config:
			for (int i = 0; i < MAX_INST_NUM; i++)
			{
				free(_instructions_executed[i]);
			}
		case cleanup_config:
			free(_config_args_read);
	}
}