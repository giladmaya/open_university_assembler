/*
 * OperationEncoder.c
 *
 *  Created on: Aug 4, 2015
 *      Author: student
 */

#include "Consts.h"
#include "OperationEncoder.h"
#include "Types.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

operation* get_operation_data(line_info* p_info) {
	operation* p_operation = NULL;
	char* operation_name = get_operation_name(p_info);
	int times = get_operation_times_counter(p_info);

	if (times != INVALID_OPEARTION_COUNTER) {
		operation_information* p_opeation_information = get_operation_info(operation_name);


	}

	return p_operation;
}

int get_operation_times_counter(line_info* p_info) {
	int times = INVALID_OPEARTION_COUNTER;

	/* This isn't a valid operation. An operation name must be followed by a number */
	if (!isdigit(p_info->line_str[p_info->current_index])) {
		print_compiler_error("An operation must be followed by a number", p_info);
	} else {
		char* number;
		int number_length = 0;
		int start_index = p_info->current_index;

		while (isdigit(p_info->line_str[p_info->current_index])) {
			number_length++;
			p_info->current_index++;
		};

		number = allocate_string(number_length + 1);
		strncpy(number, p_info->line_str + start_index, number_length);
		number[number_length] = END_OF_STRING;

		times = atoi(number);

		if (times <= 0) {
			times = INVALID_OPEARTION_COUNTER;
			print_compiler_error("The number after operation must be Natural (1, 2, 3, ..)", p_info);
		}
	}

	return times;
}

char* get_operation_name(line_info* p_info) {
	char* result_operation_name;
	char* operation_name;
	int operation_name_length = 0;

	skip_all_spaces(p_info);

	operation_name = p_info->line_str + p_info->current_index;

	while (isalpha(p_info->line_str[p_info->current_index])) {
		(p_info->current_index)++;
		operation_name_length++;
	}

	result_operation_name = allocate_string(operation_name_length + 1);

	strncpy(result_operation_name, operation_name, operation_name_length);
	result_operation_name[operation_name_length] = END_OF_STRING;

	return result_operation_name;
}
