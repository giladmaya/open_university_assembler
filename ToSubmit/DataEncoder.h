/*
 ====================================================================================
 Name		: 	DataEncoder.h

 Author's	: 	Maya Gilad 302526850, Idan Levi 301242434

 Description: 	This file holds methods relevant to .string and .data line processing
 ====================================================================================
 */

#ifndef DATA_H_
#define DATA_H_

#include "Types.h"
#include <stdio.h>

/*
 * Description: Processes a data initialization line
 * Input:		1. Line information
 * 				2. Current DC value
 * 				3. Label value
 * 				4. Type of data (.string, .data)
 * 				5. Does a symbol exists
 */
void first_transition_process_data(transition_data* transition, char* label, char* type, bool is_symbol);

/*
 * Description: Processes a string
 * Input:		1. Line information
 * 				2. Current DC value
 */
void process_string(transition_data* transition);

/*
 * Description: Processes numbers definition
 * Input:		1. Line information
 * 				2. Current DC value
 */
void process_numbers(transition_data* transition);

/*
 * Description: Gets the next number from the .data line
 * Input:		1. Current transition data
 * 				2. Pointer to the resulted number
 * Output:		Is number valid
 */
bool get_next_number(transition_data* transition, int* number);

/*
 * Description: Adds new data found in code to the list
 * Input:		A new data node
 */
void add_data_node_to_list(data_node_ptr p_new_data);

/*
 * Description: Adds a string to the data list
 * Input:		A character of the string
 * Output:		Was the add successful
 */
bool add_string_data_to_list(char token, unsigned int address);

/*
 * Description: Adds a numeric data to the list
 * Input:		A number
 * Output:		Was the add successful
 */
bool add_numeric_data_to_list(int number, unsigned int address);

/*
 * Description: Updates data addresses according to code block size
 * Input:		Code block size
 */
void update_data_address(int ic_length);

/*
 * Description: Writes all data definitions into an output file
 * Input:		Output file
 */
void write_data_to_output_file(FILE* p_file);

/*
 * Description: Free memory list
 */
void free_data_node_list();

#endif /* DATA_H_ */
