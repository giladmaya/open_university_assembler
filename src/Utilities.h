/*
 * Utilities.h
 *
 *  Created on: Jul 14, 2015
 *      Author: student
 */

#ifndef UTILITIES_H_
#define UTILITIES_H_

#include "Types.h"
#include "Enums.h"
#include <stdbool.h>

void print_compiler_error(char* message, line_info* info);
void print_runtime_error(char* message);

char* allocate_string(int string_length);
void* allocate_memory(int bytes);

void skip_all_spaces(line_info* info);

void get_next_word(line_info* info, char** word, bool skip_spaces);
char* get_label(line_info* info);

line_info* create_line_info(char* file_name, int line_number, char* line_str);
bool is_empty_or_comment(char* line);
bool is_end_of_data_in_line(line_info* info);

void get_operation(char* word, char** operation, int* counter);
char* get_next_operand(line_info* info);
ADDRESS_METHOD get_operand_method(char* operand);
bool is_register(char* operand, int length);

operation_information* get_operation_info(char* operation);
void init_operation_list();
void add_operation_to_list(char* name, unsigned int code, int operands);

char* convert_base10_to_target_base(unsigned int base10_number, int target_base);

bool is_valid_lable (char* str);


#endif /* UTILITIES_H_ */
