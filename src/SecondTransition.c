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
#include "DataEncoder.h"
#include "EntryEncoder.h"
#include "ExternEncoder.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

/*
 * Description: Executes the second transition
 * Input:		1. Input file
 * 				2. Name of input file
 */
void second_transition_execute(FILE* pFile, char* file_name_without_extension, unsigned int previous_transition_ic, unsigned int previous_transition_dc) {
	transition_data* transition = create_transition_data();
	compiler_output_files output_files;
	int line_number = 0;

	output_files.ob_file = create_output_file(file_name_without_extension, CODE_FILE_EXT);
	output_files.entry_file = NULL;
	output_files.extern_file = NULL;

	write_code_and_data_size_to_output_file(previous_transition_ic, previous_transition_dc, output_files.ob_file);

	/* Step 1 */
	transition->IC = 0;

	/* Reads all code lines */
	while (!feof(pFile) && !transition->is_runtimer_error && !transition->is_compiler_error) {
		char line[MAX_LINE_LENGTH];

		/* Step 2 */
		if (fgets(line, MAX_LINE_LENGTH + 1, pFile)) {
			/* This isn't an empty line or a comment */
			if (!is_empty_or_comment(line)) {
				transition->current_line_information =
						create_line_info(file_name_without_extension, ++line_number, line);

				second_transition_process_line(transition, &output_files);

				free(transition->current_line_information);
			}
		}
	}

	if (!transition->is_runtimer_error && !transition->is_compiler_error) {
		write_data_to_output_file(output_files.ob_file);

		if (output_files.ob_file != NULL) {
			fclose(output_files.ob_file);
		}

		if (output_files.extern_file != NULL) {
			fclose(output_files.extern_file);
		}

		if (output_files.entry_file != NULL) {
			fclose(output_files.entry_file);
		}
	} else {
		if (output_files.ob_file != NULL) {
			char* full_name = allocate_string(strlen(file_name_without_extension) + strlen(CODE_FILE_EXT));

			fclose(output_files.ob_file);
			if (full_name != NULL) {
				strcpy(full_name, file_name_without_extension);
				strcat(full_name, CODE_FILE_EXT);

				remove(full_name);

				free(full_name);
			} else {
				print_runtime_error("Couldn't delete compilation files");
			}

		}

		if (output_files.extern_file != NULL) {
			char* full_name = allocate_string(strlen(file_name_without_extension) + strlen(EXTERN_FILE_EXT));

			fclose(output_files.extern_file);

			if (full_name != NULL) {
				strcpy(full_name, file_name_without_extension);
				strcat(full_name, EXTERN_FILE_EXT);

				remove(full_name);

				free(full_name);
			} else {
				print_runtime_error("Couldn't delete compilation files");
			}
		}

		if (output_files.entry_file != NULL) {
			char* full_name = allocate_string(strlen(file_name_without_extension) + strlen(ENTRY_FILE_EXT));

			fclose(output_files.entry_file);
			if (full_name != NULL) {
				strcpy(full_name, file_name_without_extension);
				strcat(full_name, ENTRY_FILE_EXT);

				remove(full_name);
				free(full_name);
			} else {
				print_runtime_error("Couldn't delete compilation files");
			}
		}
	}


}

/*
 * Description: Processes a line in input file
 * Input:		1. Line information
 * 				2. Current DC value
 * 				3. Is first operation
 * 				4. Previous address method
 */
void second_transition_process_line(transition_data* transition, compiler_output_files* output_files) {
	char* type = NULL;
	int index;

	/*
	 * Step 3
	 * Skips label if exists
	 */
	skip_label(transition->current_line_information);

	index = transition->current_line_information->current_index;

	type = get_next_word(transition->current_line_information);

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
		create_extern_output_file_if_needed(output_files, transition->current_line_information->file_name);
	} else if (strcmp(type, ENTRY_OPERATION) == 0) {
		second_transition_process_entry(transition, output_files);
	} else  {
		transition->current_line_information->current_index = index;
		second_transition_process_operation(transition, output_files);
	}

	if (type != NULL) {
		free(type);
	}
}


void write_code_and_data_size_to_output_file(unsigned int ic, unsigned int dc, FILE* output_file) {
	char* number = convert_base10_to_target_base(ic, TARGET_BASE, 0);
	fputs(number, output_file);
	fputc(' ', output_file);
	free(number);

	number = convert_base10_to_target_base(dc, TARGET_BASE, 0);
	fputs(number, output_file);
	free(number);

	fputc(END_OF_LINE, output_file);

}
