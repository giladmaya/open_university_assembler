/*
 * Utilities.h
 *
 *  Created on: Jul 14, 2015
 *      Author: student
 */

#ifndef UTILITIES_H_
#define UTILITIES_H_

#include "Types.h"
#include <stdbool.h>

void print_compiler_error(char* message, line_info* info);
void skip_all_spaces(line_info* info);
void get_word(line_info* info, char** word);
line_info* create_line_info(char* file_name, int line_number, char* line_str);

void get_operation(char* word, char** operation, int* counter);
bool is_valid_operation(char* operation);
void init_operation_list();
void add_operation_to_list(char* name, unsigned int code);

#endif /* UTILITIES_H_ */
