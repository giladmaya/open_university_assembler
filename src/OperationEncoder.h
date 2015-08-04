/*
 * OperationEncoder.h
 *
 *  Created on: Aug 4, 2015
 *      Author: student
 */

#ifndef OPERATIONENCODER_H_
#define OPERATIONENCODER_H_

#include "Types.h"
#include <stdbool.h>

operation* get_operation_data(line_info* p_info, bool is_first, ADDRESS_METHOD previous_address_method);

char* get_operation_name(line_info* p_info);
int get_operation_times_counter(line_info* p_info);
bool are_operands_valid(char* operation_name, ADDRESS_METHOD first_operand, ADDRESS_METHOD second_operand);
ADDRESS_METHOD get_address_method(line_info* p_info, char* operand);

#endif /* OPERATIONENCODER_H_ */
