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
void process_line(line_info* info, int* ic, int* dc);
void get_word(line_info* info, char** word);
symbol* get_data_symbol(line_info* info, int* dc, char* word, char* type);
symbol* get_operation_symbol(line_info* info, int* ic, char* word, char* type);
symbol* get_extern_symbol(char* label);
bool is_operation_name(char* symbol);
data_node_ptr get_data(line_info* info, char* type, int* dc);

#endif /* FIRSTTRANSITION_H_ */
