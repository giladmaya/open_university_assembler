/*
 * OperationEncoder.c
 *
 *  Created on: Aug 4, 2015
 *      Author: student
 */

#include "Consts.h"
#include "OperationEncoder.h"
#include "Types.h"
#include "Utilities.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

operation* get_operation_data(line_info* p_info, bool is_first, ADDRESS_METHOD previous_address_method) {
	operation* p_result_operation = NULL;
	char* operation_name = get_operation_name(p_info);
	operation_information* p_operation_information = get_operation_info(operation_name);

	if (p_operation_information == NULL) {
		print_compiler_error("Invalid operation name", p_info);
	} else {
		char* source_operand = NULL;
		char* target_operand = NULL;
		ADDRESS_METHOD source_adderss_method = IMMEDIATE;
		ADDRESS_METHOD target_adderss_method = IMMEDIATE;
		int times = get_operation_times_counter(p_info);

		if (times != INVALID_OPEARTION_COUNTER) {
			switch (p_operation_information->operands_number) {
				case NO_OPERANDS: {
					/* Does nothing */
					break;
				}
				case ONE_OPERAND: {
					source_operand = get_next_operand(p_info);
					source_adderss_method = get_address_method(p_info, source_operand);
					break;
				}
				case TWO_OPERANDS: {
					source_operand = get_next_operand(p_info);
					/* TODO: skip , */
					target_operand = get_next_operand(p_info);
					break;
				}
			}

			if (is_end_of_data_in_line(p_info) &&
					are_operands_valid(operation_name, source_adderss_method, target_adderss_method)) {
				/*TODO: CHANGE COPY PREV IF NEEDED */

				p_result_operation = (operation*)allocate_memory(sizeof(operation));

				p_result_operation->operation = p_operation_information;
				p_result_operation->source_operand = source_operand;
				p_result_operation->target_operand = target_operand;
			}
		}
	}

	return p_result_operation;
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

ADDRESS_METHOD get_address_method(line_info* p_info, char* operand) {
	int operand_length = strlen(operand);

	if (operand_length > 0) {
		if (operand[0] == IMMEDIATE_TOKEN) {
			int i = 1;

			if ((operand[1] == MINUS_SIGN) || (operand[1] == PLUS_SIGN)) {
				i++;

				if (operand_length == 1) {
					print_compiler_error("Immediate token # must be followed by a valid number", p_info);
					return INVALID;
				}
			}

			for (; i < operand_length; i++) {
				if (!isdigit(operand[i])) {
					print_compiler_error("Immediate token # must be followed by a valid number", p_info);
					return INVALID;
				}
			}

			return IMMEDIATE;
		} else if (strcmp(operand, COPY_PERVIOUS_STR) == 0) {
			return COPY_PREVIOUS;
		} else if (is_register(operand, operand_length)) {
			return DIRECT_REGISTER;
		} else {
			return DIRECT;
		}
	} else {
		print_compiler_error("Operand cannot be an empty string", p_info);
		return INVALID;
	}
}

/*
 * Checks whether the operands given after the operation name are valid
 */
bool are_operands_valid(char* operation_name, ADDRESS_METHOD source_method, ADDRESS_METHOD target_method) {
	/* This are invalid methods */
	if ((source_method == INVALID) || (target_method == INVALID))
		return false;

	/*
	 * mov, add, and sub operation
	 * First operand: all methods are valid
	 * Second operand: only direct and direct register are valid
	 */
	if ((strcmp(operation_name, MOV_OPERATION) == 0) ||
		(strcmp(operation_name, ADD_OPERATION) == 0) ||
		(strcmp(operation_name, SUB_OPERATION) == 0)) {
		return ((target_method == DIRECT) || (target_method == DIRECT_REGISTER));
	}
	/*
	 * not, clr, inc, dec operation
	 * Only one operand.
	 * First operand: only direct and direct register are valid
	 */
	else if ((strcmp(operation_name, NOT_OPERATION) == 0) ||
			(strcmp(operation_name, CLR_OPERATION) == 0) ||
			(strcmp(operation_name, INC_OPERATION) == 0) ||
			(strcmp(operation_name, DEC_OPERATION) == 0)) {
		return ((source_method == DIRECT) || (source_method == DIRECT_REGISTER));
	}
	/*
	 * lea operation
	 * First operand: only direct is valid
	 * Second operand: only direct and direct register are valid
	 */
	else if (strcmp(operation_name, LEA_OPERATION) == 0) {
		return (source_method == DIRECT) && ((target_method == DIRECT) || (target_method == DIRECT_REGISTER));
	}
	/*
	 * jmp, bne, red operation
	 * Only one operand
	 * First operand: not immediate
	 */
	else if ((strcmp(operation_name, JMP_OPERATION) == 0) ||
			   (strcmp(operation_name, BNE_OPERATION) == 0) ||
			   (strcmp(operation_name, RED_OPERATION) == 0)) {
		return source_method != IMMEDIATE;
	}
	/*
	 * jsr operation
	 * Only one operand
	 * First operand: only direct
	 */
	else if (strcmp(operation_name, JSR_OPERATION) == 0) {
		return source_method == DIRECT;
	}
	/*
	 * cmp, prn, rts, stop operation
	 * The method is irrelevant or no operands given
	 */
	else {
		return true;
	}
}
