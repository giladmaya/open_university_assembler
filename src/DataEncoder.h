/*
 * Data.h
 *
 *  Created on: Jul 17, 2015
 *      Author: student
 */

#ifndef DATA_H_
#define DATA_H_

#include "Types.h"
#include <stdio.h>

void first_transition_process_data(line_info* info, unsigned int* dc, char* word, char* type, bool is_symbol);
void process_string(line_info* info, unsigned int* dc);
void process_numbers(line_info* info, unsigned int* dc);

void add_data_node_to_list(data_node_ptr p_new_data);
void add_string_data_to_list(char token, unsigned int address);
void add_numeric_data_to_list(int number, unsigned int address);

void update_data_address(int ic_length);

void write_data_to_output_file(FILE* p_file);

#endif /* DATA_H_ */
