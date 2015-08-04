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

operation* get_operation_data(line_info* p_info);

char* get_operation_name(line_info* p_info);
int get_operation_times_counter(line_info* p_info);

bool is_valid_operation(operation* p_operation);

#endif /* OPERATIONENCODER_H_ */
