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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

/*
 * Global variables
 */
bool error = false;

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
			line_info* info = create_line_info(file_name, ++line_number, line);

			if (info != NULL) {
				process_line(info, &IC, &DC);

				free(info);
			}
		} else {
			/*TODO: error */
		}
	}
}

void process_line(line_info* info, unsigned int* ic, unsigned int* dc) {
	char* label = NULL;
	char* type = NULL;
	bool is_symbol = false;

	get_word(info, &label);

	/* The line isn't empty*/
	if (label != NULL) {
		/* This is a line with a single word */
		if (info->current_index == info->line_length) {
			print_compiler_error("Invalid syntax. A single word without any instruction or operation", info);
			error = true;
			free(label);

			return;
		}

		/*
		 * Step 3.
		 * The word is followed by ':'. It must be a label.
		 */
		if (info->line_str[info->current_index] == LABEL_COLON) {
			info->current_index++;

			/* Step 4 */
			is_symbol = true;

			get_word(info, &type);

			if (type == NULL) {
				print_compiler_error("A label without any instruction or operation after it", info);
				error = true;

				return;
			}
		} else {
			type = label;
		}

		/*
		 * Step 5.
		 * The label is followed by '.data' or '.string'
		 */
		if ((strcmp(type, DATA_OPERATION) == 0) || (strcmp(type, STRING_OPERATION) == 0)) {
			process_data(info, dc, label, type, is_symbol);
		}
		/*
		 * Step 8.
		 * The label is followed by '.extern' or '.entry'
		 */
		else if ((strcmp(type, EXTERN_OPERATION) == 0) || (strcmp(type, ENTRY_OPERATION) == 0)) {
			/* Step 9 */
			if (strcmp(type, EXTERN_OPERATION) == 0) {
				process_extern(info);
			}
		} else  {
			process_operation(info, ic, label, type, is_symbol);
		}

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

	get_word(info, &extern_name);

	if (extern_name != NULL) {
		p_symbol = create_symbol(extern_name, 0, true, true);

		if (p_symbol != NULL) {
				add_symbol_to_list(p_symbol);
		}
	}
}

void process_operation(line_info* info, unsigned int* ic, char* label, char* type, bool is_symbol) {
	char* operation = NULL;
	int operation_counter;

	/* Step 11 */
	if (is_symbol) {
		symbol_node_ptr p_symbol = create_symbol(label, 0, false, false);

		if (p_symbol != NULL) {
			add_symbol_to_list(p_symbol);
		}
	}

	/* Step 12 */
	get_operation(type, &operation, &operation_counter);

	if (is_valid_operation(operation)) {

	} else {
		/* TODO: invalid operation error */
	}


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
				add_data_to_list(token, *dc);
			}

			data_index++;
		}

		(*dc)++;
		add_data_to_list(STRING_DATA_END, *dc);
	}
}

void process_numbers(line_info* info, unsigned int* dc) {
	bool is_valid = true;
	skip_all_spaces(info);

	while ((info->current_index < info->line_length) && is_valid) {
		char* partial_line = NULL;

		is_valid = false;

		if (info->line_str[info->current_index] == MINUS_SIGN) {
			(*dc)++;
			add_data_to_list(MINUS_SIGN, *dc);
			info->current_index++;
		}

		while ((info->current_index < info->line_length) && isdigit(info->line_str[info->current_index])) {
			(*dc)++;
			add_data_to_list(info->line_str[info->current_index], *dc);
			info->current_index++;
		}

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
