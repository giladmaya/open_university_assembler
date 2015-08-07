/*
 * ExternEncoder.h
 *
 *  Created on: Aug 7, 2015
 *      Author: student
 */

#ifndef EXTERNENCODER_H_
#define EXTERNENCODER_H_

#include "Types.h"

void create_extern_output_file_if_needed(compiler_output_files* output_files, char* file_name_without_extension);

void first_transition_process_extern(transition_data* transition);

void write_extern_to_output_file(char* extern_operand, unsigned int current_address, FILE* output_file);

#endif /* EXTERNENCODER_H_ */
