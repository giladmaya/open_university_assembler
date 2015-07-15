/*
 * FirstTransition.h
 *
 *  Created on: Jul 14, 2015
 *      Author: student
 */

#ifndef FIRSTTRANSITION_H_
#define FIRSTTRANSITION_H_

#include "Types.h"
#include <stdio.h>
#include <stdbool.h>

void execute_first_transition(FILE* pFile, char* file_name);
void process_line(char* line_str, int line_number, char* file_name, int* ic, int* dc);
void get_word(char* line, int line_length, int* current_index, char** word);
symbol* get_data_symbol(char* line_str, int line_length, int* current_index, int* dc, char* word, char* type);
symbol* get_operation_symbol(char* line_str, int line_length, int* current_index, int* ic, char* word, char* type);
symbol* get_extern_symbol(char* label);
bool is_operation_name(char* symbol);

#endif /* FIRSTTRANSITION_H_ */
