/*
 ====================================================================================
 Name		: 	SecondTransition.h

 Author's	: 	Maya Gilad 302526850, Idan Levi 301242434

 Description: 	This file holds methods relevant to the second transition run
 ====================================================================================
 */

#ifndef SECONDTRANSITION_H_
#define SECONDTRANSITION_H_

#include "Types.h"
#include <stdio.h>

/*
 * Description: Executes the second transition
 * Input:		1. Input file handle
 * 				2. Name of input file
 * 				3. Code length in memory word
 * 				4. Data length in memory word
 */
void second_transition_execute(FILE* pFile, char* file_name_without_extension, unsigned int previous_transition_ic, unsigned int previous_transition_dc);

/*
 * Description: Processes a line in input file
 * Input:		1. Current transition data
 * 				2. Output files
 */
void second_transition_process_line(transition_data* transition, compiler_output_files* output_files);

/*
 * Description: Writes first line of ob file
 * Input:		1. IC value
 * 				2. DC value
 * 				3. Ob file handle
 */
void write_code_and_data_size_to_output_file(unsigned int ic, unsigned int dc, FILE* output_file);

#endif /* SECONDTRANSITION_H_ */
