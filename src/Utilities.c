/*
 * Utilities.c
 *
 *  Created on: Jul 14, 2015
 *      Author: student
 */

#include <stdio.h>
#include <ctype.h>
#include "Types.h"

void print_compiler_error(char* message, line_info* info) {
	fprintf(stderr, "File: %s Line: %d Error: %s", info->file_name, info->line_number, message);
}

void skip_all_spaces(line_info* info) {
	int index = info->current_index;

	while ((index < info->line_length) && isspace(info->line_str[index])) {
		index++;
	}

	info->current_index = index;
}
