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
 * Output:		Was add successful
 */
bool add_string_data_to_list(char data, unsigned int address) {
	data_node_ptr p_data = (data_node_ptr)allocate_memory(sizeof(data_node));

	if (p_data != NULL) {
		p_data->current_data.encoded_data.bits.number = data;
		p_data->current_data.encoded_data.bits.rest = NO_ADDRESS;
		p_data->current_data.address = address;
		p_data->next = NULL;

		add_data_node_to_list(p_data);

		return true;
	} else {
		return false;
	}
}

/*
 * Description: Adds a numeric data to the list
 * Input:		A number
 */
bool add_numeric_data_to_list(int number, unsigned int address) {
	data_node_ptr p_data = (data_node_ptr)allocate_memory(sizeof(data_node));

	if (p_data != NULL) {
		p_data->current_data.encoded_data.bits.number = number;
		p_data->current_data.encoded_data.bits.rest = NO_ADDRESS;
		p_data->current_data.address = address;
		p_data->next = NULL;

		add_data_node_to_list(p_data);

		return true;
	} else {
		return false;
	}
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
void first_transition_process_data(transition_data* transition, char* label, char* data_type, bool is_symbol) {

	/* Step 6 */
	if (is_symbol) {
		symbol_node_ptr p_searched_symbol = search_symbol(label);

		if (p_searched_symbol == NULL) {
			symbol_node_ptr p_symbol = create_symbol(label, transition->DC, false, true);

			if (p_symbol != NULL)
				add_symbol_to_list(p_symbol);
			else {
				transition->is_runtimer_error = true;

				free(label);

				return;
			}

		} else {
			print_compiler_error("Each label can be defined only once", transition->current_line_information);
			transition->is_compiler_error = true;
			return;
		}
	}

	skip_all_spaces(transition->current_line_information);

	/*
	 * Step 7.
	 * Extract data according to type
	 */
	if (is_end_of_data_in_line(transition->current_line_information)) {
		print_compiler_error("Missing data after .string or .data", transition->current_line_information);
		transition->is_compiler_error = true;
	}
	/* This is a string initialization */
	else if (strcmp(data_type, STRING_OPERATION) == 0) {
		process_string(transition);
	}
	/* This is a numeric data */
	else {
		process_numbers(transition);
	}
}


/*
 * Description: Processes a string
 * Input:		1. Line information
 * 				2. Current DC value
 */
void process_string(transition_data* transition) {
	if (transition->current_line_information->line_str[transition->current_line_information->current_index] != QUOTATION) {
		print_compiler_error("A string must start with a '\"' token", transition->current_line_information);
		transition->is_compiler_error = true;
	} else {
		bool success;

		/* Skip quotation mark */
		transition->current_line_information->current_index++;

		while (!is_end_of_line(transition->current_line_information)) {
			char token =
					transition->current_line_information->line_str[transition->current_line_information->current_index];

			if (token == END_OF_LINE) {
				print_compiler_error("A string must end with a '\"' token", transition->current_line_information);
				transition->is_compiler_error = true;
				break;
			} else if (token == QUOTATION) {
				transition->current_line_information->current_index++;
				break;
			} else if (token != QUOTATION) {
				success = add_string_data_to_list(token, transition->DC);

				if (!success) {
					transition->is_runtimer_error = true;
					return;
				} else {
					transition->DC++;
				}
			}

			transition->current_line_information->current_index++;
		}

		success = add_string_data_to_list(STRING_DATA_END, transition->DC);

		if (!success) {
			transition->is_runtimer_error = true;
			return;
		}

		transition->DC++;

		if (!is_end_of_data_in_line(transition->current_line_information)) {
			print_compiler_error("Invalid tokens after .string instruction", transition->current_line_information);
			transition->is_compiler_error = true;
		}
	}
}

/*
 * Description: Processes numbers definition
 * Input:		1. Line information
 * 				2. Current DC value
 */
void process_numbers(transition_data* transition) {
	bool should_process_next_number = true;
	bool success;

	skip_all_spaces(transition->current_line_information);

	if (is_end_of_data_in_line(transition->current_line_information)) {
		print_compiler_error("Invalid .data definition. Missing numbers.", transition->current_line_information);
		transition->is_compiler_error = true;
	}

	/* Processes all numbers in line */
	while (!is_end_of_line(transition->current_line_information) && should_process_next_number) {
		char* partial_line = NULL;
		int number;

		if (get_next_number(transition, &number))
		{
			success = add_numeric_data_to_list(number, transition->DC++);

			if (!success) {
				transition->is_runtimer_error = true;
				return;
			}

			/* Search the next ',' */
			partial_line =
					strchr(transition->current_line_information->line_str +
							transition->current_line_information->current_index, NUMBER_TOKEN_SEPERATOR);

			if (partial_line != NULL) {
				transition->current_line_information->current_index =
						partial_line - transition->current_line_information->line_str + 1;

				should_process_next_number = true;
			} else {
				skip_all_spaces(transition->current_line_information);
				should_process_next_number = false;
			}
		} else {
			should_process_next_number = false;
		}
	}

	if (!is_end_of_line(transition->current_line_information) || should_process_next_number) {
		print_compiler_error(".data syntax is invalid", transition->current_line_information);
		transition->is_compiler_error = true;
	}
}

/*
 * Description: Free list memory
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

/*
 * Description: Gets the next number from the .data line
 * Input:		1. Current transition data
 * 				2. Pointer to the resulted number
 * Output:		Is number valid
 */
bool get_next_number(transition_data* transition, int* number) {
	skip_all_spaces(transition->current_line_information);

	/* Reaches end of line while expecting a number */
	if (is_end_of_line(transition->current_line_information)) {
		print_compiler_error("Expected number definition", transition->current_line_information);
		transition->is_compiler_error = true;
	} else {
		int number_start_index = transition->current_line_information->current_index;
		int number_end_index = transition->current_line_information->current_index;

		/* First token of the number is a minus or a plus */
		if ((transition->current_line_information->line_str[number_start_index] == MINUS_SIGN) ||
				transition->current_line_information->line_str[number_start_index] == PLUS_SIGN) {
			number_end_index++;
		}

		/* Makes sure the next token is a digit */
		if (!is_end_of_line(transition->current_line_information) &&
				!isdigit(transition->current_line_information->line_str[number_end_index])) {
			print_compiler_error("A number must have digits in it", transition->current_line_information);
			transition->is_compiler_error = true;
		} else if (!is_end_of_line(transition->current_line_information)){
			char* number_string = NULL;

			/* Finds all digits */
			while (isdigit(transition->current_line_information->line_str[number_end_index])) {
				number_end_index++;
			}

			/* Last token wasn't part of the number */
			number_end_index--;

			number_string = allocate_string(number_end_index - number_start_index + 1);
			strncpy(number_string, transition->current_line_information->line_str + number_start_index, number_end_index - number_start_index + 1);
			number_string[number_end_index - number_start_index + 1] = END_OF_STRING;

			*number = atoi(number_string);
			free(number_string);

			transition->current_line_information->current_index = number_end_index + 1;

			return true;
		}
	}

	return false;
}
