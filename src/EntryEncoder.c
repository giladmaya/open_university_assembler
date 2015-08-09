/*
 ====================================================================================
 Name		: 	EntryEncoder.c

 Author's	: 	Maya Gilad 302526850, Idan Levi 301242434

 Description: 	This file holds methods relevant to .entry line processing
 ====================================================================================
 */

#include "Consts.h"
#include "EntryEncoder.h"
#include "Types.h"
#include "SymbolTable.h"
#include "Utilities.h"
#include <stdlib.h>
#include <stdio.h>

/*
 * Description: Processes an entry line in the second transition
 * Input:		1. Line information
 * 				2. Output files
 */
void second_transition_process_entry(transition_data* transition, compiler_output_files* output_files) {
	char* entry_name = get_next_word(transition);

	/* Search for the entry inside the symbol table */
	symbol_node_ptr p_symbol = search_symbol(entry_name);

	/* Entry doesn't exists in symbol table*/
	if (p_symbol == NULL) {
		print_compiler_error("Invalid entry definition. Label doesn't exists.", transition->current_line_information);
		transition->is_compiler_error = true;
	} else {
		create_entry_output_file_if_needed(output_files, transition->current_line_information->file_name);

		if (output_files->entry_file == NULL) {
			transition->is_runtimer_error = true;
			return;
		}

		write_entry_to_output_file(entry_name, p_symbol->current_symbol.address, output_files->entry_file);
	}
}

/*
 * Description: Processes an entry line in the first transition
 * Input:		1. Line information
 */
void first_transition_process_entry(transition_data* transition) {
	char* entry_name = get_next_word(transition);

	/* If entry was found*/
	if (entry_name != NULL) {
		/* Is entry valid? */
		if (is_valid_label(entry_name)) {
			/* Are we at line end?, if so, its an error, and if not, we are done */
			if (!is_end_of_data_in_line(transition->current_line_information)) {
				print_compiler_error("Invalid tokens in end of entry definition", transition->current_line_information);
				transition->is_compiler_error = true;
			}
		}
		/* Throw compiler error */
		else {
			print_compiler_error("Entry name must be a valid label", transition->current_line_information);
			transition->is_compiler_error = true;
		}
	}
	/* Throw runtime error */
	else if (!transition->is_runtimer_error) {
		print_compiler_error("Missing entry name", transition->current_line_information);
		transition->is_compiler_error = true;
	}
}

/*
 * Description: Creates the entry file if it doesn't exists
 * Input:		1. Output files
 * 				2. File name without extension
 */
void create_entry_output_file_if_needed(compiler_output_files* output_files, char* file_name_without_extension) {
	if (output_files->entry_file == NULL) {
		output_files->entry_file = create_output_file(file_name_without_extension, ENTRY_FILE_EXT);
	}
}

/*
 * Description: Write the entry into the entry output file
 * Input:		1. Entry name
 * 				2. Entry definition address
 * 				3. Output file
 */
void write_entry_to_output_file(char* entry_name, unsigned int address, FILE* output_file) {
	char* base4_value;

	/* Add current entry to output file */
	fputs(entry_name, output_file);
	fputc(COLUMN_SEPARATOR, output_file);

	/* Add current entry address to output file */
	base4_value = convert_base10_to_target_base(address, TARGET_BASE, TARGET_MEMORY_ADDRESS_WORD_LENGTH);
	fputs(base4_value, output_file);
	free(base4_value);

	/* New file */
	fputc(END_OF_LINE, output_file);
}
