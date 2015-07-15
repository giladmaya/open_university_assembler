/*
 * FirstTransition.c
 *
 *  Created on: Jul 9, 2015
 *      Author: student
 */

#include "FirstTransition.h"
#include "Consts.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*
 * Executes the first transition of the assembly compiler
 */
void execute_first_transition(FILE* pFile) {
	int IC = 0;
	int DC = 0;

	while (feof(pFile)) {
		process_line(pFile, &IC, &DC);
	}
}

void process_line(FILE* pFile, int* ic, int* dc) {
	char* line = (char*)malloc(sizeof(char) * MAX_LINE_LENGTH);

	if (line == NULL) {
		/* TODO: return error code */
		return;
	}

	if (fgets(line, MAX_LINE_LENGTH + 1, pFile)) {
		int lineLength = strlen(line);
		int i, currentIndex = 0;

		for (i = 0; i < lineLength; i++) {
			if (!isspace(line[i])) {
				currentIndex = i;
				break;
			}
		}



	}
}
