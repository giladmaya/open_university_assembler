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

/*
 * Knownn bugs:
 * 2. Fix encoding of external - Idan
 * 3. Fix weird empty lines
 * 4. Fix encoding of registry
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

		/* Create full file path */
		curr_file = allocate_string(strlen(argv[i])+strlen(FILE_EXT));
		strcpy (curr_file, argv[1]);
		strcat (curr_file, FILE_EXT);

		/* Open the file or die*/
		p_file = fopen(curr_file, "r");
		if (p_file == NULL) {
			print_runtime_error("Could not open source file");
		}

		execute_first_transition(p_file, curr_file);
		rewind(p_file);
		execute_second_transition(p_file, argv[i]);
		fclose(p_file);

	}
	
	exit(0);
}
