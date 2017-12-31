#include "Utils.h"

int memory_image_input[MEMORY_IMAGE_INPUT_SIZE];



inst_ex* _instructions_executed[MAX_INST_NUM];
config_args* _config_args_read;
int last_unoccupied_index_in_iq;
inst* _instructions[MAX_INST_NUM];
int _memory_image_input[MEMORY_IMAGE_INPUT_SIZE];
CDB** cdb;
unsigned int cycles;
int num_of_inst;
int num_of_cdb;
int current_inst_in_instructions;
iq* iq_arr;

RS RAT[16]; //mapping between register to reservation station
RS rs_add[MAX_CONFIG_SIZE];
RS rs_mul[MAX_CONFIG_SIZE];
RS rs_div[MAX_CONFIG_SIZE];

calc_unit add_units[MAX_CONFIG_SIZE]; //change to struct, if timer -1 we can runover, TODO look into starvation
calc_unit div_units[MAX_CONFIG_SIZE];
calc_unit mul_units[MAX_CONFIG_SIZE];