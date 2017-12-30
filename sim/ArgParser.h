#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdbool.h>
#include "Utils.h"

#define CDB_NAME_LEN (5)





bool parse_file(char* file_path, parse_type parsing_type, void** output_object, int* counter);
void convert_mem_to_inst(int* memory_image, inst** output_insts);