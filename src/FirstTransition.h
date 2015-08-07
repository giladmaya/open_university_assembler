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

bool first_transition_execute(FILE* pFile, char* file_name);

void first_transition_process_line(line_info* info, unsigned int* ic, unsigned int* dc);

void process_operation(line_info* info,unsigned int* ic, char* label, char* type, bool is_symbol);
int get_operation_size(line_info* info, operation_information* operation, int times);

#endif /* FIRSTTRANSITION_H_ */
