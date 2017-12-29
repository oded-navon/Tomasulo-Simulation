#include "Main.h"


int main(int argc, char* argv[])
{
    //command line input
	//sim cfg.txt memin.txt memout.txt regout.txt traceinst.txt tracecdb.txt
	char* config_file_path = argv[1];
	char* memory_in_path = argv[2];
	char* memory_out_path = argv[3];
	char* reg_out_path = argv[4];
	char* trace_inst_path = argv[5];
	char* trace_cdb_path = argv[6];

	struct config_args* config_args_read= malloc(sizeof(struct config_args));
	if (config_args_read == NULL)
	{
		return FAIL;
	}
	
	if (!parse_config_file(config_file_path, config_args_read))
	{
		return FAIL;
	}

	return SUCCESS;
}

