/*
 ====================================================================================
 Name		: 	ExternEncoder.h

 Author's	: 	Maya Gilad 302526850, Idan Levi 301242434

 Description: 	This file holds methods relevant to .extern line processing
 ====================================================================================
 */

#ifndef EXTERNENCODER_H_
#define EXTERNENCODER_H_

#include "Types.h"
#include <stdio.h>

/*
 * Description: Creates the entry file if it doesn't exists
 * Input:		1. Output files
 * 				2. File name without extension
 */
void create_extern_output_file_if_needed(compiler_output_files* output_files, char* file_name_without_extension);

/*
 * Description: Process the extern definition in first transition.
 * 				Adds it into the symbol table
 * Input:		1. Current transition
 */
void first_transition_process_extern(transition_data* transition);

/*
 * Description: Write the extern into the extern output file
 * Input:		1. Extern name
 * 				2. Extern usage address
 * 				3. Output file
 */
void write_extern_to_output_file(char* extern_operand, unsigned int current_address, FILE* output_file);

#endif /* EXTERNENCODER_H_ */
