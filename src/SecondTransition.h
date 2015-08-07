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

void second_transition_execute(FILE* pFile, char* file_name_without_extension, unsigned int previous_transition_ic, unsigned int previous_transition_dc);

void second_transition_process_line(transition_data* transition, compiler_output_files* output_files);

void second_transition_process_operation(transition_data* transition, compiler_output_files* p_output_files);

void write_code_and_data_size_to_output_file(unsigned int ic, unsigned int dc, FILE* output_file);

#endif /* SECONDTRANSITION_H_ */
