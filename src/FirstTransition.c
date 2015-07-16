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
	int IC = 0;
	int DC = 0;
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

void process_line(line_info* info, int* ic, int* dc) {

	symbol* p_symbol = NULL;
	char* label = NULL;
	char* type = NULL;
	bool is_symbol = false;

	get_word(info, &label);

	if (label != NULL){
		/* This is a line with a single word */
		if (info->current_index == info->line_length) {
			print_compiler_error("Invalid syntax. A single word without any instruction or operation", info);
			error = true;
			free(label);

			return;
		}

		/* The word is followed by ':'. It must be a label */
		if (info->line_str[info->current_index + 1] == LABEL_COLON) {
			is_symbol = true;
			info->current_index += 2;

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
				p_symbol = get_data_symbol(info, dc, label, type);
			}
		}
		/* The label is followed by '.extern'*/
		else if (strcmp(type, EXTERN_OPERATION) == 0){
			p_symbol = get_extern_symbol(label);

			/* TODO: insert to extern symbols */
		}
		/* The label isn't followed by '.entry'. Therefore it is an operation */
		else if (strcmp(type, ENTRY_OPERATION) != 0) {
			if (is_symbol) {
				p_symbol = get_operation_symbol(info, ic, label, type);
			}
		}
		else
		{
		}

		free(type);
	}
}

void get_word(line_info* info, char** word)
{
	int i, word_end_index, word_start_index, word_length;

	if (*word != NULL) {
		free(*word);
	}

	/* Finds first symbol token */
	for (i = info->current_index; i < info->line_length; i++) {
		if (!isspace(info->line_str[i])) {
			break;
		}
	}

	word_start_index = i;
	word_end_index = i;

	for (;i < info->line_length; i++) {
		if (!isspace(info->line_str[i]) && (info->line_str[i] != LABEL_COLON)) {
			word_end_index = i;
		}
		else
		{
			break;
		}

		info->current_index = i;
	}

	word_length = word_end_index - word_start_index + 1;

	*word = (char*)malloc(sizeof(char) * (word_length + 1));

	strncpy(*word, info->line_str + word_start_index, word_length);
}

symbol* get_data_symbol(line_info* info, int* dc, char* word, char* type) {
	symbol* p_symbol = (symbol*)malloc(sizeof(symbol));

	if (p_symbol == NULL) {
		/* TODO: bad allocation */
		error = true;
	} else {
		data_node_ptr p_data_node = NULL;

		p_symbol->is_instruction = true;
		p_symbol->is_external = false;
		p_symbol->address = *dc;

		/* TODO: implement 7 phase in algo */
		p_data_node = get_data(info, type, dc);
	}

	return p_symbol;
}

symbol* get_extern_symbol(char* label) {
	symbol* p_symbol = (symbol*)malloc(sizeof(symbol));

	if (p_symbol == NULL) {
		/* TODO: bad allocation */
	} else {
		p_symbol->is_external = true;
		p_symbol->address = 0;
		p_symbol->is_instruction = true;
	}

	return p_symbol;
}

symbol* get_operation_symbol(line_info* info, int* ic, char* word, char* type) {
	symbol* p_symbol = (symbol*)malloc(sizeof(symbol));

	if (p_symbol == NULL) {
		/* TODO: bad allocation */
	} else {
		p_symbol->is_instruction = false;
		p_symbol->is_external = false;
		p_symbol->address = 0;

		/* TODO: implement 12-13 phase in algo */
	}

	return p_symbol;
}

data_node_ptr get_data(line_info* info, char* type, int* dc) {
	data_node_ptr p_node = NULL;
	int current_address = *dc;

	skip_all_spaces(info);

	if (info->current_index > info->line_length) {
		print_compiler_error("Any data instruction must be followed by data initialization", info);
		error = true;
	} else if (strcmp(type, STRING_OPERATION) == 0) {
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
					data_node_ptr p_new_node = (data_node_ptr)malloc(sizeof(data_node));

					if (p_new_node == NULL) {
						/*TODO: bad alloc */
						error = true;
					} else {
						current_address++;

						p_new_node->value.value = token;
						p_new_node->value.address = current_address;
						p_new_node->next = NULL;

						if (p_node == NULL) {
							p_node = p_new_node;
						} else {
							data_node_ptr p_current_node = p_node;

							while (p_current_node->next != NULL) {
								p_current_node = p_current_node->next;
							}

							p_current_node->next = p_new_node;
						}
					}
				}
			}

			*dc = data_index;
		}

	} else {

	}

	return p_node;
}
