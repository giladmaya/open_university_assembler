/*
 ====================================================================================
 Name		: 	FirstTransition.c

 Author's	: 	Maya Gilad, Idan Levi

 Description: 	A
 Input		:	A

 Output		:	A

 Assumptions:
 ====================================================================================
 */

#include "Consts.h"
#include "DataEncoder.h"
#include "Enums.h"
#include "ExternEncoder.h"
#include "FirstTransition.h"
#include "Types.h"
#include "Utilities.h"
#include "SymbolTable.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

/*
 * Global variables
 */
ADDRESS_METHOD last_operand_method;
bool can_use_copy_previous = false;

/*
 * Description: Executes the first transition of the assembly compiler
 * Input:		1. Input file
 * 				2. File name
 * Output:		Was transition successfull
 */
bool first_transition_execute(FILE* assembler_input_file, char* file_name_without_extension, unsigned int* IC, unsigned int* DC) {
	transition_data* transition = create_transition_data();
	int line_number = 0;
	bool success = true;

	/* Step 1 */
	transition->IC = 0;
	transition->DC = 0;

	/* Runs until end of file */
	while (!feof(assembler_input_file)) {
		char line[MAX_LINE_LENGTH];

		/* Step 2 */
		if (fgets(line, MAX_LINE_LENGTH + 1, assembler_input_file)) {

			/* This isn't an empty line or a comment */
			if (!is_empty_or_comment(line)) {
				line_info* info = create_line_info(file_name_without_extension, ++line_number, line);

				transition->current_line_information = info;

				/* Process the line */
				first_transition_process_line(transition);

				success &= !(info->is_error);

				free(info);
			}
		}
	}

	/* Changes the data address according to the code length */
	update_data_address(transition->IC);
	update_symbol_address(transition->IC);

	*IC = transition->IC;
	*DC = transition->DC;

	return success;
}

/*
 * Description: Processes a line according to its' type
 * Input:		1. Line information
 * 				2. Current IC address
 * 				3. Current DC value
 */
void first_transition_process_line(transition_data* transition) {
	char* label = NULL;
	char* line_type = NULL;
	bool is_symbol = false;

	/*
	 * Step 3
	 * The first field is a label
	 */
	if ((label = get_label(transition->current_line_information)) != NULL) {
		/* Step 4 */
		is_symbol = true;
	}

	line_type = get_next_word(transition->current_line_information);

	/*
	 * Step 5
	 */
	if ((strcmp(line_type, DATA_OPERATION) == 0) || (strcmp(line_type, STRING_OPERATION) == 0)) {
		first_transition_process_data(transition, label, line_type, is_symbol);
	}
	/*
	 * Step 8
	 */
	else if ((strcmp(line_type, EXTERN_OPERATION) == 0) || (strcmp(line_type, ENTRY_OPERATION) == 0)) {
		/* Step 9 */
		if (strcmp(line_type, EXTERN_OPERATION) == 0) {
			first_transition_process_extern(transition);
		}
	}
	/*
	 * Step 11
	 */
	else  {
		process_operation(transition, label, line_type, is_symbol);
	}

	if (line_type != NULL) {
		free(line_type);
	}
}

/*
 * Description: Process an operation definition
 * Input:		1. Line information
 * 				2. Current IC address
 * 				3. Label value
 * 				4. Type of operation
 * 				5. Does a symbol exists
 */
void process_operation(transition_data* transition, char* label, char* type, bool is_symbol) {
	char* operation_name = NULL;
	int operation_counter;

	/* Step 11 */
	if (is_symbol) {
		symbol_node_ptr p_symbol = create_symbol(label, transition->IC, false, false);

		add_symbol_to_list(p_symbol);
	}

	if (!is_valid_is_operation_line(transition->current_line_information->line_str)) {
		print_compiler_error("Operation line is too long", transition->current_line_information);
		transition->is_compiler_error = true;
	} else {
		machine_operation_definition* operation_info;

		/* Step 12 */
		get_operation(type, &operation_name, &operation_counter);

		/* Get operation definition from operations list */
		operation_info = get_operation_info(operation_name);

		if (operation_info != NULL) {
			/* Calculate operation size */
			int length = get_operation_size(transition->current_line_information, operation_info, operation_counter);
			transition->IC += length;
		} else {
			print_compiler_error("Operation code doesn't exist", transition->current_line_information);
			transition->is_compiler_error = true;
		}
	}
}

/*
 * Description: Calculates the operation's size according to its' definition
 * Input:		1. Line information
 * 				2. Operation definition
 * 				3. How many times to perform the operation
 * Output:		Memory words size for the encoded operation
 */
int get_operation_size(line_info* info, machine_operation_definition* operation, int times) {
	int size = OPERAION_MIN_WORD_SIZE;

	/* There are operands */
	if (operation->operands_number > NO_OPERANDS) {
		char* first_operand;
		ADDRESS_METHOD first_operand_method;

		/* Gets the first operand and its address method of the operation*/
		first_operand = get_next_operand(info);
		first_operand_method = get_operand_method(first_operand);

		/* This is a Copy Previous method */
		if (first_operand_method == COPY_PREVIOUS) {
			if (can_use_copy_previous) {
				first_operand_method = last_operand_method;
			} else {
				print_compiler_error("Using Copy-Previous address method on first operation", info);
				info->is_error = true;
			}
		}

		if (operation->operands_number == ONE_OPERAND) {
			size++;
		} else {
			char* second_operand;
			ADDRESS_METHOD second_operand_method;

			while ((info->current_index < info->line_length) &&
					(info->line_str[info->current_index] != OPERAND_SEPERATOR)) {
				info->current_index++;
			}

			info->current_index++;

			second_operand = get_next_operand(info);

			if (second_operand == NULL) {
				print_compiler_error("This operation must have two operands", info);
				info->is_error = true;
				return size;
			}

			second_operand_method = get_operand_method(second_operand);

			if (second_operand_method == COPY_PREVIOUS) {
				if (can_use_copy_previous) {
					second_operand_method = last_operand_method;
				} else {
					print_compiler_error("Using Copy-Previous address method on first operation", info);
					info->is_error = true;
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

		last_operand_method = first_operand_method;

		can_use_copy_previous = true;
	} else {
		/* This is an operation without operands so we treat it like our first operation */
		can_use_copy_previous = false;
	}

	return size * times;
}
