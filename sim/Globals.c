#include "Utils.h"

config_args* _config_args_read;
int _last_unoccupied_index_in_iq;
inst* _instructions[MAX_INST_NUM];
int _memory_image_input[MEMORY_IMAGE_INPUT_SIZE];
CDB_ex** _cdb;
unsigned int _cycles;
int _num_of_inst;
int _num_of_cdb;
int _current_inst_in_instructions;
inst_queue* _iq_arr;
float _regs[NUM_OF_REGS];
cdb_free* _cdb_free;
char* _trace_cdb_file_path;

RAT_entry RAT[NUM_OF_REGS];
RS rs_add[MAX_CONFIG_SIZE];
RS rs_mul[MAX_CONFIG_SIZE];
RS rs_div[MAX_CONFIG_SIZE];

char rs_add_names[MAX_CONFIG_SIZE][NAME_LEN];
char rs_mul_names[MAX_CONFIG_SIZE][NAME_LEN];
char rs_div_names[MAX_CONFIG_SIZE][NAME_LEN];

calc_unit add_units[MAX_CONFIG_SIZE]; //TODO: look into starvation
calc_unit div_units[MAX_CONFIG_SIZE];
calc_unit mul_units[MAX_CONFIG_SIZE];

load_buffer load_buffers[MAX_CONFIG_SIZE];
store_buffer store_buffers[MAX_CONFIG_SIZE];

//this set of booleans will help decide when the program will end completely
bool received_halt_in_fetch; //means to stop do fetches
bool finished_issue; //means to stop handle issues
bool finished_execute; //means to stop executing
bool finished_dispatch; //means to stop dispatching
bool finished_broadcast; //means to stop broadcasting


