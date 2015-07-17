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
	unsigned int IC = 0;
	unsigned int DC = 0;
	int line_number = 0;
	char* line = (char*)malloc(sizeof(char) * MAX_LINE_LENGTH);

	if (line == NULL) {
		/* TODO: return error code */
		return;
	}

	while (!feof(pFile)) {
		if (fgets(line, MAX_LINE_LENGTH + 1, pFile)) {
			line_number++;

			line_info info;
			info.current_index = 0;
			info.line_number = line_number;
			info.line_str = line;
			info.file_name = file_name;
			info.line_length = strlen(line);

			process_line(&info, &IC, &DC);
		} else {
			/*TODO: error */
		}
	}

	if (line != NULL) {
		free(line);
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

		/* The word is followed by ':'. It must be a label */
		if (info->line_str[info->current_index] == LABEL_COLON) {
			is_symbol = true;
			info->current_index++;

			get_word(info, &type);

			if (type == NULL) {
				print_compiler_error("A label without any instruction or operation after it", info);
				error = true;

				return;
			}
		} else {
			type = label;
		}

		/* The label is followed by '.data' or '.string' */
		if ((strcmp(type, DATA_OPERATION) == 0) || (strcmp(type, STRING_OPERATION) == 0)) {

			if (is_symbol) {
				process_data(info, dc, label, type);
			}
		}
		/* The label is followed by '.extern'*/
		else if (strcmp(type, EXTERN_OPERATION) == 0){
			process_extern(info);
		}
		/* The label isn't followed by '.entry'. Therefore it is an operation */
		else if (strcmp(type, ENTRY_OPERATION) != 0) {
			if (is_symbol) {
				process_operation(info, ic, label);
			}
		}

		free(type);
	}
}

void process_data(line_info* info, unsigned int* dc, char* label, char* type) {
	symbol_node_ptr p_symbol = create_symbol(label, *dc, false, true);

	if (p_symbol != NULL) {
		skip_all_spaces(info);

		if (info->current_index > info->line_length) {
			print_compiler_error("Any data instruction must be followed by data initialization", info);
			error = true;
		} else if (strcmp(type, STRING_OPERATION) == 0) {
			process_string(info, dc);
		} else {

		}

		add_symbol_to_list(p_symbol);
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

void process_operation(line_info* info, unsigned int* ic, char* label) {
	symbol_node_ptr p_symbol = create_symbol(label, 0, false, false);

	if (p_symbol == NULL) {
		add_symbol_to_list(p_symbol);

		/* TODO: implement 12-13 phase in algo */
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
	}
}
