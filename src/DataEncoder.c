/*
 * Data.c
 *
 *  Created on: Jul 17, 2015
 *      Author: student
 */

#include "Types.h"
#include "Utilities.h"
#include "OperationEncoder.h"
#include "DataEncoder.h"
#include "Consts.h"
#include "SymbolTable.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* DataEncoder head and tail */
data_node_ptr p_data_head = NULL;
data_node_ptr p_data_tail = NULL;


/*
 * Description: Adds new data found in code to the list
 * Input:		A new data node
 */
void add_data_node_to_list(data_node_ptr p_new_data) {
	/* This is the first data */
	if (p_data_head == NULL) {
		p_data_head = p_new_data;
		p_data_tail = p_new_data;
	} else {
		/* Inserts the data in the end */
		p_data_tail->next = p_new_data;
		p_data_tail = p_new_data;
	}
}

/*
 * Description: Adds a string to the data list
 * Input:		A character of the string
 */
void add_string_data_to_list(char data, unsigned int address) {
	data_node_ptr p_data = (data_node_ptr)allocate_memory(sizeof(data_node));

	p_data->current_data.encoded_data.bits.number = data;
	p_data->current_data.encoded_data.bits.rest = NO_ADDRESS;
	p_data->current_data.address = address;
	p_data->next = NULL;

	add_data_node_to_list(p_data);
}

/*
 * Description: Adds a numeric data to the list
 * Input:		A number
 */
void add_numeric_data_to_list(int number, unsigned int address) {
	data_node_ptr p_data = (data_node_ptr)allocate_memory(sizeof(data_node));

	p_data->current_data.encoded_data.bits.number = number;
	p_data->current_data.encoded_data.bits.rest = NO_ADDRESS;
	p_data->current_data.address = address;
	p_data->next = NULL;

	add_data_node_to_list(p_data);
}

/*
 * Description: Writes all data definitions into an output file
 * Input:		Output file
 */
void write_data_to_output_file(FILE* output_file) {
	data_node_ptr p_current_data = p_data_head;

	while (p_current_data != NULL) {
		data_definition data = p_current_data->current_data;

		print_encoding_to_file(data.address, data.encoded_data.value, output_file);

		p_current_data = p_current_data->next;
	}
}

/*
 * Description: Updates data addresses according to code block size
 * Input:		Code block size
 */
void update_data_address(int ic_length) {
	data_node_ptr p_current = p_data_head;

	while (p_current != NULL) {
		p_current->current_data.address += ic_length + ADDRESS_START;

		p_current = p_current->next;
	}
}

/*
 * Description: Processes a data initialization line
 * Input:		1. Line information
 * 				2. Current DC value
 * 				3. Label value
 * 				4. Type of data (.string, .data)
 * 				5. Does a symbol exists
 */
void first_transition_process_data(line_info* info, unsigned int* dc, char* label, char* type, bool is_symbol) {
	/* Step 6 */
	if (is_symbol) {
		symbol_node_ptr p_symbol = create_symbol(label, *dc, false, true);

		add_symbol_to_list(p_symbol);
	}

	skip_all_spaces(info);

	/*
	 * Step 7.
	 * Extract data according to type
	 */
	if (info->current_index > info->line_length) {
		print_compiler_error("Any data instruction must be followed by data initialization", info);
		info->is_error = true;
	}
	/* This is a string initialization */
	else if (strcmp(type, STRING_OPERATION) == 0) {
		process_string(info, dc);
	}
	/* This is a numeric data */
	else {
		process_numbers(info, dc);
	}
}


/*
 * Description: Processes a string
 * Input:		1. Line information
 * 				2. Current DC value
 */
void process_string(line_info* info, unsigned int* dc) {
	if (info->line_str[info->current_index] != QUOTATION) {
		print_compiler_error("A string must start with a '\"' token", info);
		info->is_error = true;
	} else {
		int data_index = info->current_index + 1;

		while (data_index < info->line_length) {
			char token = info->line_str[data_index];
			if (token == END_OF_LINE) {
				print_compiler_error("A string must end with a '\"' token", info);
				info->is_error = true;
				break;
			} else if (token == QUOTATION) {
				break;
			} else if (token != QUOTATION) {
				add_string_data_to_list(token, *dc);
				(*dc)++;
			}

			data_index++;
		}

		add_string_data_to_list(STRING_DATA_END, *dc);
		(*dc)++;
	}
}

/*
 * Description: Processes numbers definition
 * Input:		1. Line information
 * 				2. Current DC value
 */
void process_numbers(line_info* info, unsigned int* dc) {
	bool is_valid = true;

	skip_all_spaces(info);

	/* Processes all numbers in line */
	while ((info->current_index < info->line_length) && is_valid) {
		char* partial_line = NULL;
		char* number_str = NULL;
		int number_str_length;
		int number;
		int number_end_index = info->current_index;

		is_valid = false;

		/* The number starts with a + or - sign */
		if ((info->line_str[info->current_index] == MINUS_SIGN) ||
				(info->line_str[info->current_index] == PLUS_SIGN)){
			number_end_index++;
		}

		/* Get digits */
		while ((number_end_index < info->line_length) && isdigit(info->line_str[number_end_index])) {
			number_end_index++;
		}

		number_str_length = number_end_index - info->current_index;

		number_str = allocate_string(number_str_length);

		strncpy(number_str, info->line_str + info->current_index, number_str_length);
		number_str[number_str_length] = END_OF_STRING;

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
		info->is_error = true;
	}
}

/*
 * Description: Free list memory
 * Input:		1. List Head
 */
void free_data_node_list() {
	data_node_ptr p_cleaner_data = p_data_head;

	while (p_data_head) {

		p_cleaner_data = p_data_head;

		/* Move next */
		p_data_head = p_data_head->next;

		free (p_cleaner_data);
	}

	return;
}
