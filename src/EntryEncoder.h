/*
 ====================================================================================
 Name		: 	EntryEncoder.h

 Author's	: 	Maya Gilad 302526850, Idan Levi 301242434

 Description: 	This file holds methods relevant to .entry line processing
 ====================================================================================
 */

#ifndef ENTRYENCODER_H_
#define ENTRYENCODER_H_

#include "Types.h"

/*
 * Description: Processes an entry line in the second transition
 * Input:		1. Line information
 * 				2. Output files
 */
void first_transition_process_entry(transition_data* transition);

/*
 * Description: Processes an entry line in the second transition
 * Input:		1. Line information
 * 				2. Output files
 */
void second_transition_process_entry(transition_data* transition, compiler_output_files* output_files);

/*
 * Description: Write the entry into the entry output file
 * Input:		1. Entry name
 * 				2. Entry definition address
 * 				3. Output file
 */
void write_entry_to_output_file(char* entry_name, unsigned int address, FILE* output_file);

/*
 * Description: Creates the entry file if it doesn't exists
 * Input:		1. Output files
 * 				2. File name without extension
 */
void create_entry_output_file_if_needed(compiler_output_files* output_files, char* file_name_without_extension);

#endif /* ENTRYENCODER_H_ */
