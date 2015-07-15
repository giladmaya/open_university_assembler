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

	while (feof(pFile)) {
		if (fgets(line, MAX_LINE_LENGTH + 1, pFile)) {
			line_number++;
			process_line(line, line_number, file_name, &IC, &DC);
		} else {
			/*TODO: error */
		}
	}

	if (line != NULL) {
		free(line);
	}
}

void process_line(char* line_str, int line_number, char* file_name, int* ic, int* dc) {

	char* label = NULL;
	int line_length = strlen(line_str);
	int current_index = 0;

	get_word(line_str, line_length, &current_index, &label);

	if (label != NULL)	{
		/* This is a line with a single word */
		if (current_index == line_length) {
			print_compiler_error("Invalid syntax. A single word without any instruction or operation", line_number, file_name);
			error = true;
			free(label);

			return;
		}
		/* The word is followed by ':'. It must be a label */
		else if (line_str[current_index + 1] == LABEL_COLON) {
			symbol* p_symbol = NULL;
			char* type = NULL;

			get_word(line_str, line_length, &current_index, &type);

			if (type == NULL) {
				print_compiler_error("A label without any content after it", line_number, file_name);
				error = true;

				return;
			}

			/* The label is followed by '.data' or '.string' */
			if ((strcmp(type, DATA_OPERATION) == 0) || (strcmp(type, STRING_OPERATION) == 0)) {
				p_symbol = get_data_symbol(line_str, line_length, &current_index, dc, label, type);
			}
			/* The label is followed by '.extern'*/
			else if (strcmp(type, EXTERN_OPERATION) == 0){
				p_symbol = get_extern_symbol(label);
			}
			/* The label isn't followed by '.entry'. Therefore it is an operation */
			else if (strcmp(type, ENTRY_OPERATION) != 0) {
				p_symbol = get_operation_symbol(line_str, line_length, &current_index, ic, label, type);
			}
			else
			{
				free(label);
			}

			free(type);
		}
	}
}

void get_word(char* line, int line_length, int* current_index, char** word)
{
	int i, word_end_index, word_start_index, word_length;

	if (*word != NULL) {
		free(*word);
	}

	/* Finds first symbol token */
	for (i = *current_index; i < line_length; i++) {
		if (!isspace(line[i])) {
			break;
		}
	}

	word_start_index = i;
	word_end_index = i;

	for (;i < line_length; i++) {
		if (!isspace(line[i]) && (line[i] != LABEL_COLON)) {
			word_end_index = i;
		}
		else
		{
			break;
		}

		*current_index = i;
	}

	word_length = word_end_index - word_start_index + 1;

	*word = (char*)malloc(sizeof(char) * (word_length + 1));

	strncpy(*word, line + word_start_index, word_length);
}

symbol* get_data_symbol(char* line_str, int line_length, int* current_index, int* dc, char* word, char* type) {
	symbol* p_symbol = (symbol*)malloc(sizeof(symbol));

	if (p_symbol == NULL) {
		/* TODO: bad allocation */
	} else {
		p_symbol->is_instruction = true;
		p_symbol->is_external = false;
		p_symbol->address = *dc;

		/* TODO: implement 7 phase in algo */
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

symbol* get_operation_symbol(char* line_str, int line_length, int* current_index, int* ic, char* word, char* type) {
	symbol* p_symbol = (symbol*)malloc(sizeof(symbol));

	if (p_symbol == NULL) {
		/* TODO: bad allocation */
	} else {
		p_symbol->is_instruction = false;
		p_symbol->is_external = false;
		p_symbol->address = *ic;

		/* TODO: implement 12-13 phase in algo */
	}

	return p_symbol;
}
