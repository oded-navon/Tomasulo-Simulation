#include "Issue.h"

extern RS rs_add[MAX_CONFIG_SIZE];
extern RS rs_mul[MAX_CONFIG_SIZE];
extern RS rs_div[MAX_CONFIG_SIZE];
extern config_args* _config_args_read;


int get_free_reservation_station_index(int opcode);
int issue_instruction();
int get_free_reservation_station_index(int opcode);
int get_specific_free_reservation_station_index(RS rses[], int num_rses);
void put_inst_in_RS(int rs_index, inst* instr, int type, int dst, int src0, int src1, char* src0_name, char* src1_name);

int issue()
{
	int res = issue_instruction();
	if (res == 1) res += issue_instruction();
	return res;
}

int issue_instruction()
{
	/*return 1 if issued instruction successfully, else 0*/

	//inst* inst = take_next_instruction_from_IQ();
	inst* instr = peek();
	int type = instr->opcode;
	int rs_index = get_free_reservation_station_index(type);
	if (rs_index == NO_RS_AVAILABLE)
	{
		return 0;
	}

	int dst = 0;
	int src0 = 0;
	int src1 = 0;
	char src0_name[RS_NAME_LEN];
	char src1_name[RS_NAME_LEN];
	put_inst_in_RS(rs_index, instr, type, dst, src0, src1, src0_name, src1_name);

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
		res = get_specific_free_reservation_station_index(rs_add, _config_args_read->add_nr_reservation);
		break;
	case 4:
		res = get_specific_free_reservation_station_index(rs_mul, _config_args_read->mul_nr_reservation);
		break;
	case 5:
		res = get_specific_free_reservation_station_index(rs_div, _config_args_read->div_nr_reservation);
		break;
	}

	return res;
}

int get_specific_free_reservation_station_index(RS rses[], int num_rses)
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
/*	inst** res = malloc(sizeof(inst)*MAX_INST_NUM);

return res;
}*/

void put_inst_in_RS(int rs_index, inst* instr, int type, int dst, int src0, int src1, char* src0_name, char* src1_name)
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
		//res = get_free_reservation_station_index(rs_mul, config_args_read->mul_nr_reservation);
		break;
	case 5:
		//res = get_free_reservation_station_index(rs_div, config_args_read->div_nr_reservation);
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


