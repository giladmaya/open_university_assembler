/*
 * SecondTransition.c
 *
 *  Created on: Aug 1, 2015
 *      Author: student
 */

#include "SecondTransition.h"
#include "Consts.h"
#include "SymbolTable.h"
#include "Utilities.h"
#include "Types.h"
#include "OperationEncoder.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

/* Global variables */
FILE* p_ob_file = NULL;
FILE* p_extern_file = NULL;
FILE* p_ent_file = NULL;

/*
 * Description: Executes the second transition
 * Input:		1. Input file
 * 				2. Name of input file
 */
void execute_second_transition(FILE* pFile, char* file_name_without_extension) {
	unsigned int IC;
	int line_number = 0;
	bool is_first_operation = true;
	ADDRESS_METHOD previous_address_method = IMMEDIATE;
	char line[MAX_LINE_LENGTH];

	char* file_name = allocate_memory(strlen(file_name_without_extension) + strlen(CODE_FILE_EXT));

	/* Creates code file output name */
	strcpy(file_name, file_name_without_extension);
	strcat(file_name, CODE_FILE_EXT);

	p_ob_file = fopen(file_name, WRITE_MODE);

	free(file_name);

	if (!p_ob_file) {
		print_runtime_error("Cannot open ob file for this input file");
	}

	/* Step 1 */
	IC = 0;

	/* Reads all code lines */
	while (!feof(pFile)) {
		/* Step 2 */
		if (fgets(line, MAX_LINE_LENGTH + 1, pFile)) {
			/* This isn't an empty line or a comment */
			if (!is_empty_or_comment(line)) {
				line_info* info = create_line_info(file_name_without_extension, ++line_number, line);

				process_line_transition_two(info, &IC, &is_first_operation, &previous_address_method);

				free(info);
			}
		} else {
			print_runtime_error("Failed due to unexpected error in reading input file");
		}
	}

	if (p_ent_file != NULL) {
		fclose(p_ent_file);
	}

	if (p_extern_file != NULL) {
		fclose(p_extern_file);
	}

	if (p_ob_file != NULL) {
		fclose(p_ob_file);
	}
}

/*
 * Description: Processes a line in input file
 * Input:		1. Line information
 * 				2. Current DC value
 * 				3. Is first operation
 * 				4. Previous address method
 */
void process_line_transition_two(line_info* info, unsigned int* ic, bool* is_first, ADDRESS_METHOD* p_prev_method) {
	char* type = NULL;
	int index;

	/*
	 * Step 3
	 * Skips label if exists
	 */
	skip_label(info);

	index = info->current_index;

	get_next_word(info, &type, true);

	/*
	 * Step 4
	 */
	if ((strcmp(type, DATA_OPERATION) == 0) || (strcmp(type, STRING_OPERATION) == 0)) {
		/* Skip */
	}
	/*
	 * Step 5
	 */
	else if (strcmp(type, EXTERN_OPERATION) == 0) {
		process_and_write_extern(info);
	} else if (strcmp(type, ENTRY_OPERATION) == 0) {
		process_and_write_entry(info);
	}
	else  {
		info->current_index = index;
		process_and_encode_operation(info, ic, is_first, p_prev_method);
	}

	if (type != NULL) {
		free(type);
	}
}

/*
 * Description: Processes an entry line and prints its content into output file
 * Input:		1. Line information
 */
void process_and_write_entry(line_info* info) {
	char* entry_name = NULL;
	symbol_node_ptr p_symbol;

	if (!p_ent_file) {
		char* file_name = allocate_memory(strlen(info->file_name) + strlen(ENTRY_FILE_EXT));

		/* Creates entry output file name */
		strcpy(file_name, info->file_name);
		strcat(file_name, ENTRY_FILE_EXT);

		p_ent_file = fopen(file_name, WRITE_MODE);

		free(file_name);

		if (!p_ent_file) {
			print_runtime_error("Cannot open entry output file");
		}
	}

	get_next_word(info, &entry_name, true);

	/* Search for the entry inside the symbol table */
	p_symbol = search_symbol(entry_name);

	if (p_symbol == NULL) {
		print_compiler_error("Invalid entry definition", info);
	} else {
		char* base4_value;

		/* Add current entry to output file */
		fputs(entry_name, p_ent_file);
		fputc(COLUMN_SEPARATOR, p_ent_file);

		/* Add current entry address to output file */
		base4_value = convert_base10_to_target_base(p_symbol->data.address, TARGET_BASE, TARGET_MEMORY_ADDRESS_WORD_LENGTH);
		fputs(base4_value, p_ent_file);
		fputc(END_OF_LINE, p_ent_file);
	}
}

/*
 * Description: Process an operation line
 * Input:		1. Line information
 * 				2. Current IC address
 * 				3. Is first operation
 * 				4. Previous address method
 */
void process_and_encode_operation(line_info* info, unsigned int* ic, bool* p_is_first, ADDRESS_METHOD* p_prev_address_method) {
	/* Gets all data about the current operation */
	operation* p_decoded_operation = get_operation_data(info, *p_is_first, *p_prev_address_method);

	if (p_decoded_operation == NULL) {
		return;
	}

	if (p_decoded_operation->operation->operands_number == NO_OPERANDS) {
		*p_is_first = true;
	} else {
		*p_prev_address_method = p_decoded_operation->source_operand_address_method;
		*p_is_first = false;
	}

	/* Encode the operation */
	encode_operation(p_decoded_operation, ic, p_ob_file);
}

/*
 * Description: Process an extern line
 * Input:		Line information
 */
void process_and_write_extern(line_info* info) {
	if (!p_extern_file) {
		char* file_name = allocate_string(strlen(info->file_name) + strlen(EXTERN_FILE_EXT));

		strcpy(file_name, info->file_name);
		strcat(file_name, EXTERN_FILE_EXT);

		p_extern_file = fopen(file_name, WRITE_MODE);

		free(file_name);

		if (!p_extern_file) {
			print_runtime_error("Cannot open extern output file");
		}
	}
}
