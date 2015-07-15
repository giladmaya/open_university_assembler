/*
 * Utilities.c
 *
 *  Created on: Jul 14, 2015
 *      Author: student
 */

#include <stdio.h>

void print_compiler_error(char* message, int line_number, char* file_name) {
	fprintf(stderr, "File: %s Line: %d Error: %s", file_name, line_number, message);
}


