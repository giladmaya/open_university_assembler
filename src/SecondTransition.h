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

void second_transition_execute(FILE* pFile, char* file_name_without_extension);

void second_transition_process_line(transition_data* transition, compiler_output_files* output_files);

void second_transition_process_operation(transition_data* transition, compiler_output_files* p_output_files);

#endif /* SECONDTRANSITION_H_ */
