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
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

/* Global variables */
FILE* p_ob_file = NULL;
FILE* p_extern_file = NULL;
FILE* p_ent_file = NULL;

void execute_second_transition(FILE* pFile, char* file_name_without_extension) {
	unsigned int IC;
	int line_number = 0;
	char line[MAX_LINE_LENGTH];

	char* file_name = (char*)malloc(sizeof(char) * (strlen(file_name_without_extension) + 4));

	if ((line == NULL) || !file_name) {
		/* TODO: bad alloc */
	}

    sprintf(file_name, "%s.ob", file_name_without_extension);
	p_ob_file = fopen(file_name, "w");

	free(file_name);

	if (!p_ob_file) {
		/*TODO: throw error and exit */
	}

	/* Step 1 */
	IC = 0;

	while (!feof(pFile)) {
		/* Step 2 */
		if (fgets(line, MAX_LINE_LENGTH + 1, pFile)) {
			if (!is_empty_or_comment(line)) {
				line_info* info = create_line_info(file_name_without_extension, ++line_number, line);

				if (info != NULL) {
					process_line_transition_two(info, &IC);

					free(info);
				}
			}
		} else {
			/*TODO: error */
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

void process_line_transition_two(line_info* info, unsigned int* ic) {
	char* type = NULL;

	/*
	 * Step 3
	 * Skips label if exists
	 */
	get_label(info);

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
		process_and_write_operation(info, type, ic);
	}

	if (type != NULL) {
		free(type);
	}
}

void process_and_write_entry(line_info* info) {
	char* entry_name = NULL;
	symbol_node_ptr p_symbol;

	if (!p_ent_file) {
		char* file_name = (char*)malloc(sizeof(char) * (strlen(info->file_name) + 4));

		if (!file_name) {
			/*TODO: bad alloc */
		}

		sprintf(file_name, "%s.ent", info->file_name);
		p_ent_file = fopen(file_name, "w");

		free(file_name);

		if (!p_ent_file) {
			/*TODO: error */
		}
	}

	get_next_word(info, &entry_name, true);

	p_symbol = search_symbol(entry_name);

	if (p_symbol == NULL) {
		/*TODO: error */
	} else {
		char* base4_value;

		fputs(entry_name, p_ent_file);
		fputc(' ', p_ent_file);

		base4_value = convert_base10_to_target_base(p_symbol->data.address, TARGET_BASE);
		fputs(base4_value, p_ent_file);
		fputc(END_OF_LINE, p_ent_file);
	}
}

void process_and_write_operation(line_info* info, char* word, unsigned int* ic) {
	coded_operation_union coded_op;
	coded_operation operation_bits;
	char* operation_name = NULL;
	char* base4_value;
	int operation_counter = 0;
	operation* operation_info;

	get_operation(word, &operation_name, &operation_counter);

	operation_info = get_operation_info(operation_name);

	operation_bits.group = operation_info->operands_number;
	operation_bits.op_code = operation_info->code;

	if (operation_info->operands_number > 0) {
		char* first_operand;
		ADDRESS_METHOD first_operand_method;

		first_operand = get_next_operand(info);
		first_operand_method = get_operand_method(first_operand);

		operation_bits.source_operand_address_method = first_operand_method;

		if (operation_info->operands_number == 2) {
			char* second_operand;
			ADDRESS_METHOD second_operand_method;

			second_operand = get_next_operand(info);
			second_operand_method = get_operand_method(second_operand);

			operation_bits.target_operand_address_method = second_operand_method;
		}
	}

	operation_bits.era = ABSOLUTE;
	operation_bits.rest = 0;

	coded_op.operation_value = 0;
	coded_op.operation_bits = operation_bits;

	base4_value = convert_base10_to_target_base(*ic + ADDRESS_START, TARGET_BASE);
	fputs(base4_value, p_ob_file);
	fputc(' ', p_ob_file);

	free(base4_value);

	base4_value = convert_base10_to_target_base(coded_op.operation_value, TARGET_BASE);

	fputs(base4_value, p_ob_file);
	fputc(END_OF_LINE, p_ob_file);
}

void process_and_write_extern(line_info* info) {
	if (!p_extern_file) {
		char* file_name = (char*)malloc(sizeof(char) * (strlen(info->file_name) + 4));

		if (!file_name) {
			/*TODO: bad alloc */
		}

		sprintf(file_name, "%s.ext", info->file_name);
		p_extern_file = fopen(file_name, "w");

		free(file_name);

		if (!p_extern_file) {
			/*TODO: error */
		}
	}
}
