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
#include "OperationEncoder.h"
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

	if (transition->prev_operation_operand != NULL) {
		free(transition->prev_operation_operand);
	}

	free(transition);

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
		transition->current_line_information->current_index -= strlen(line_type);
		first_transition_process_operation(transition, label, is_symbol);
	}

	if (line_type != NULL) {
		free(line_type);
	}
}
