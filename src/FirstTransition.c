/*
 * FirstTransition.c
 *
 *  Created on: Jul 9, 2015
 *      Author: student
 */

#include "FirstTransition.h"
#include "Consts.h"
#include "Types.h"
#include "Utilities.h"
#include "SymbolTable.h"
#include "Data.h"
#include "Enums.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

/*
 * Global variables
 */
bool error = false;
ADDRESS_METHOD last_operand_method;
bool is_first_operation = true;

/*
 * Executes the first transition of the assembly compiler
 */
void execute_first_transition(FILE* pFile, char* file_name) {
	unsigned int IC;
	unsigned int DC;
	int line_number = 0;
	char line[MAX_LINE_LENGTH];

	if (line == NULL) {
		/* TODO: bad alloc */
	}

	/* Step 1 */
	IC = 0;
	DC = 0;

	while (!feof(pFile)) {
		/* Step 2 */
		if (fgets(line, MAX_LINE_LENGTH + 1, pFile)) {
			if (!is_empty_or_comment(line)) {
				line_info* info = create_line_info(file_name, ++line_number, line);

				if (info != NULL) {
					process_line(info, &IC, &DC);

					free(info);
				}
			}
		} else {
			/*TODO: error */
		}
	}

	printf("IC: %d, DC: %d", IC, DC);
}

void process_line(line_info* info, unsigned int* ic, unsigned int* dc) {
	char* label = NULL;
	char* type = NULL;
	bool is_symbol = false;

	label = get_label(info);

	/*
	 * Step 3
	 * The first field is a label
	 */
	if (label != NULL) {
		/* Step 4 */
		is_symbol = true;
	}

	get_next_word(info, &type, true);

	/*
	 * Step 5
	 */
	if ((strcmp(type, DATA_OPERATION) == 0) || (strcmp(type, STRING_OPERATION) == 0)) {
		process_data(info, dc, label, type, is_symbol);
	}
	/*
	 * Step 8
	 */
	else if ((strcmp(type, EXTERN_OPERATION) == 0) || (strcmp(type, ENTRY_OPERATION) == 0)) {
		/* Step 9 */
		if (strcmp(type, EXTERN_OPERATION) == 0) {
			process_extern(info);
		}
	}
	/*
	 * Step 11
	 */
	else  {
		process_operation(info, ic, label, type, is_symbol);
	}

	if (type != NULL) {
		free(type);
	}
}

void process_data(line_info* info, unsigned int* dc, char* label, char* type, bool is_symbol) {
	/* Step 6 */
	if (is_symbol) {
		symbol_node_ptr p_symbol = create_symbol(label, *dc, false, true);

		if (p_symbol != NULL) {
			add_symbol_to_list(p_symbol);
		}
	}

	skip_all_spaces(info);

	/*
	 * Step 7.
	 * Extract data according to type
	 */
	if (info->current_index > info->line_length) {
		print_compiler_error("Any data instruction must be followed by data initialization", info);
		error = true;
	} else if (strcmp(type, STRING_OPERATION) == 0) {
		process_string(info, dc);
	} else {
		process_numbers(info, dc);
	}
}

void process_extern(line_info* info) {
	symbol_node_ptr p_symbol = NULL;

	char* extern_name = NULL;

	get_next_word(info, &extern_name, true);

	if (extern_name != NULL) {
		p_symbol = create_symbol(extern_name, 0, true, true);

		if (p_symbol != NULL) {
				add_symbol_to_list(p_symbol);
		}
	}
}

void process_operation(line_info* info, unsigned int* ic, char* label, char* type, bool is_symbol) {
	char* operation_name = NULL;
	int operation_counter;

	/* Step 11 */
	if (is_symbol) {
		symbol_node_ptr p_symbol = create_symbol(label, 0, false, false);

		if (p_symbol != NULL) {
			add_symbol_to_list(p_symbol);
		}
	}

	if (info->line_length > OPERATION_LINE_MAX_LENGTH) {
		print_compiler_error("Operation line is too long", info);
		error = true;
	} else {
		operation* operation_info;

		/* Step 12 */
		get_operation(type, &operation_name, &operation_counter);

		operation_info = get_operation_info(operation_name);

		if (operation_info != NULL) {
			int length = get_operation_size(info, operation_info, operation_counter);
			*ic += length;
		} else {
			print_compiler_error("Operation code doesn't exist", info);
			error = true;
		}
	}
}

int get_operation_size(line_info* info, operation* operation, int times) {
	int size = 1;

	/* Todo: check valid addressing modes */
	if (operation->operands_number > 0) {
		char* first_operand;
		ADDRESS_METHOD first_operand_method;

		first_operand = get_next_operand(info);
		first_operand_method = get_operand_method(first_operand);

		if (first_operand_method == COPY_PREVIOUS) {
			if (!is_first_operation) {
				first_operand_method = last_operand_method;
			} else {
				print_compiler_error("Using Copy-Previous address method on first operation", info);
				error = true;
			}
		}

		if (operation->operands_number == 1) {
			size++;
		} else {
			char* second_operand;
			ADDRESS_METHOD second_operand_method;

			/* Todo: check bounds */
			while (info->line_str[info->current_index] != OPERAND_SEPERATOR) {
				info->current_index++;
			}

			info->current_index++;

			second_operand = get_next_operand(info);
			second_operand_method = get_operand_method(second_operand);

			if (second_operand_method == COPY_PREVIOUS) {
				if (!is_first_operation) {
					second_operand_method = last_operand_method;
				} else {
					print_compiler_error("Using Copy-Previous address method on first operation", info);
					error = true;
				}
			}

			/* The operands share one memory word */
			if ((first_operand_method == DIRECT_REGISTER) && (second_operand_method == DIRECT_REGISTER)) {
				size++;
			}
			/* Each operand gets its own memory word */
			else {
				size += 2;
			}
		}

		if ((!error) && is_first_operation) {
			last_operand_method = first_operand_method;
			is_first_operation = false;
		}
	}

	return size * times;
}

void process_string(line_info* info, unsigned int* dc) {
	if (info->line_str[info->current_index] != QUOTATION) {
		print_compiler_error("A string must start with a '\"' token", info);
		error = true;
	} else {
		int data_index = info->current_index + 1;

		while (data_index < info->line_length) {
			char token = info->line_str[data_index];
			if (token == END_OF_LINE) {
				print_compiler_error("A string must end with a '\"' token", info);
				error = true;
				break;
			} else if (token == QUOTATION) {
				break;
			} else if (token != QUOTATION) {
				(*dc)++;
				add_string_data_to_list(token, *dc);
			}

			data_index++;
		}

		(*dc)++;
		add_string_data_to_list(STRING_DATA_END, *dc);
	}
}

void process_numbers(line_info* info, unsigned int* dc) {
	bool is_valid = true;
	skip_all_spaces(info);

	while ((info->current_index < info->line_length) && is_valid) {
		char* partial_line = NULL;
		char* number_str = NULL;
		int number_str_length;
		int number;
		int number_end_index = info->current_index;

		is_valid = false;

		if ((info->line_str[info->current_index] == MINUS_SIGN) ||
				(info->line_str[info->current_index] == PLUS_SIGN)){
			number_end_index++;
		}

		while ((number_end_index < info->line_length) && isdigit(info->line_str[number_end_index])) {
			number_end_index++;
		}

		number_str_length = number_end_index - info->current_index;

		number_str = (char*)malloc(sizeof(char) * (number_str_length + 1));
		strncpy(number_str, info->line_str + info->current_index, number_str_length);
		number_str[number_str_length] = '\0';

		number = atoi(number_str);

		add_numeric_data_to_list(number, (*dc)++);

		info->current_index = number_end_index;

		free(number_str);

		partial_line = strchr(info->line_str + info->current_index, NUMBER_TOKEN_SEPERATOR);

		if (partial_line != NULL) {
			info->current_index = partial_line - info->line_str + 1;

			is_valid = true;
		} else {
			skip_all_spaces(info);
		}
	}

	if (info->current_index < info->line_length) {
		print_compiler_error(".data syntax is invalid", info);
		error = true;
	}
}
