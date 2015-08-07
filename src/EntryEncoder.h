/*
 * EntryEncoder.h
 *
 *  Created on: Aug 7, 2015
 *      Author: student
 */

#ifndef ENTRYENCODER_H_
#define ENTRYENCODER_H_

#include "Types.h"

void second_transition_process_entry(line_info* info, compiler_output_files* output_files);

void write_entry_to_output_file(char* entry_name, unsigned int address, FILE* output_file);
void create_entry_output_file_if_needed(compiler_output_files* output_files, char* file_name_without_extension);

#endif /* ENTRYENCODER_H_ */
