/*
 * SecondTransition.h
 *
 *  Created on: Aug 1, 2015
 *      Author: student
 */

#ifndef SECONDTRANSITION_H_
#define SECONDTRANSITION_H_

#include "Types.h"
#include <stdio.h>

void execute_second_transition(FILE* pFile, char* file_name_without_extension);

void process_line_transition_two(line_info* info, unsigned int* ic);

void process_and_write_operation(line_info* info, char* word, unsigned int* ic);
void process_and_write_entry(line_info* info);
void process_and_write_extern(line_info* info);

#endif /* SECONDTRANSITION_H_ */
