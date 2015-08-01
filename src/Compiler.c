/*
 * Compiler.c
 *
 *  Created on: Jul 14, 2015
 *      Author: student
 */

#include "FirstTransition.h"
#include "SecondTransition.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	FILE* p_file = fopen("/home/student/_Dev/Maman14Project/data/test.asm", "r");

	execute_first_transition(p_file, "test.as");

	fseek(p_file, 0, SEEK_SET);

	execute_second_transition(p_file, "test");

	fclose(p_file);

	exit(0);
}
