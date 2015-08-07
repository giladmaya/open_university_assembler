/*
 * ExternEncoder.c
 *
 *  Created on: Aug 7, 2015
 *      Author: student
 */

#include "ExternEncoder.h"
#include "Types.h"
#include "Consts.h"
#include "Utilities.h"
#include "SymbolTable.h"
#include <stdlib.h>
#include <stdio.h>

/*
 * Description: Creates the entry file if it doesn't exists
 * Input:		1. Output files
 * 				2. File name without extension
 */
void create_extern_output_file_if_needed(compiler_output_files* output_files, char* file_name_without_extension) {
	if (output_files->extern_file == NULL) {
		output_files->extern_file = create_output_file(file_name_without_extension, EXTERN_FILE_EXT);
	}
}

/*
 * Description: Process the extern definition in first transition.
 * 				Adds it into the symbol table
 * Input:		1. Line information
 */
void first_transition_process_extern(line_info* info) {
	symbol_node_ptr p_symbol = NULL;

	char* extern_name = get_next_word(info);

	if (extern_name != NULL) {
		p_symbol = create_symbol(extern_name, NO_ADDRESS, true, true);

		add_symbol_to_list(p_symbol);
	}
}

/*
 * Description: Write the extern into the extern output file
 * Input:		1. Extern name
 * 				2. Extern usage address
 * 				3. Output file
 */
void write_extern_to_output_file(char* extern_operand, unsigned int current_address, FILE* output_file) {
	char* base4_value;

	/* Add current extern to output file */
	fputs(extern_operand, output_file);
	fputc(COLUMN_SEPARATOR, output_file);

	/* Add current entry address to output file */
	base4_value = convert_base10_to_target_base(current_address, TARGET_BASE, TARGET_MEMORY_ADDRESS_WORD_LENGTH);
	fputs(base4_value, output_file);
	free(base4_value);

	/* New line */
	fputc(END_OF_LINE, output_file);
}