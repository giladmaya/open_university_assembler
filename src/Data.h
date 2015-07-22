/*
 * Data.h
 *
 *  Created on: Jul 17, 2015
 *      Author: student
 */

#ifndef DATA_H_
#define DATA_H_

#include "Types.h"

void add_data_node_to_list(data_node_ptr p_new_data);
void add_string_data_to_list(char token, unsigned int address);
void add_numeric_data_to_list(int number, unsigned int address);

#endif /* DATA_H_ */
