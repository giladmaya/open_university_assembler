/*
 ====================================================================================
 Name		: 	FirstTransition.h

 Author's	: 	Maya Gilad 302526850, Idan Levi 301242434

 Description: 	This file holds methods relevant to the first transition run
 ====================================================================================
 */

#ifndef FIRSTTRANSITION_H_
#define FIRSTTRANSITION_H_

#include "Types.h"
#include <stdio.h>
#include <stdbool.h>

/*
 * Description: Executes the first transition of the assembly compiler
 * Input:		1. Input file
 * 				2. File name
 * Output:		Was transition successful
 */
bool first_transition_execute(FILE* pFile, char* file_name, unsigned int* IC, unsigned int* DC);

/*
 * Description: Processes a line according to its' type
 * Input:		1. Line information
 * 				2. Current IC address
 * 				3. Current DC value
 */
void first_transition_process_line(transition_data* transition);

#endif /* FIRSTTRANSITION_H_ */
