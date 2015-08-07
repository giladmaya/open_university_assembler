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

void first_transition_process_data(transition_data* transition, char* label, char* type, bool is_symbol);
void process_string(transition_data* transition);
void process_numbers(transition_data* transition);
bool get_next_number(transition_data* transition, int* number);

void add_data_node_to_list(data_node_ptr p_new_data);
void add_string_data_to_list(char token, unsigned int address);
void add_numeric_data_to_list(int number, unsigned int address);

void update_data_address(int ic_length);

void write_data_to_output_file(FILE* p_file);

void free_data_node_list();

#endif /* DATA_H_ */
