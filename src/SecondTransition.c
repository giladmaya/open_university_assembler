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

	} else if (strcmp(type, ENTRY_OPERATION) == 0) {
		process_and_write_entry(info);
	}
	else  {
		/* Implement step 7+8 */
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
