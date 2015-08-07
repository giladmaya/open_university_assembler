/*
 ====================================================================================
 Name		: 	Compiler.c

 Author's	: 	Maya Gilad, Idan Levi

 Description: 	This program will check if there is a path between 2 nodes
				in a directed graph (tree)
 Input		:	Arguments as file name without extension to be compiled

 Output		:	For each input file we create :
 	 	 	 	 1) ob  - decoded operations in base 4 and memory address
 	 	 	 	 2) ent - memory address of labels
 	 	 	 	 3) ext - memory address of each extern variable used in ob file

 Assumptions:
 ====================================================================================
 */

#include "FirstTransition.h"
#include "SecondTransition.h"
#include "Utilities.h"
#include "Consts.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* TODO : make the call in utils and with global */
#include "DataEncoder.h"
#include "SymbolTable.h"

/*
 * Description: Cleaning up memory between files, global call for all cleanups
 */
void memory_cleanup() {
	free_data_node_list();
	free_symbol_list();
	return;
}

/* end of crap code*/

/*
 * Known bugs:
 * 5. Release memory when ending in runtime error
 * 6. Release memory of data and symbol
 * 7. Write DC and IC size on start of ob file
 */

int main(int argc, char* argv[]) {
	int i;
	char* curr_file;
	FILE* p_file;

	/* Check if no arg's provided */
	if (argc == 1) {
		print_runtime_error("Expected an argument");

	}
	
	/* Run the Compiler for each file given as arg */
	for (i=1; i < argc; i++) {
		unsigned int ic;
		unsigned int dc;
		bool should_continue;

		/* Create full file path */
		curr_file = allocate_string(strlen(argv[i])+strlen(FILE_EXT));
		strcpy (curr_file, argv[i]);
		strcat (curr_file, FILE_EXT);

		/* Open the file or die*/
		p_file = fopen(curr_file, READ_ONLY_MODE);
		if (p_file == NULL) {
			print_runtime_error("Could not open source file");
		}

		first_transition_execute(p_file, curr_file);
		rewind(p_file);
		second_transition_execute(p_file, argv[i]);

		should_continue = first_transition_execute(p_file, curr_file, &ic, &dc);

		if (should_continue) {
			rewind(p_file);
			second_transition_execute(p_file, argv[i], ic, dc);
			fclose(p_file);
		} else {
			/* Stop */
		}

		memory_cleanup();
		fclose(p_file);
		free(curr_file);
	}
	
	exit(0);
}
