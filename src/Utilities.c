/*
 * Utilities.c
 *
 *  Created on: Jul 14, 2015
 *      Author: student
 */

#include <stdio.h>
#include <ctype.h>

void read_until_non_space(FILE* pFile){
	while (!feof(pFile) && (isspace(fgetc(pFile))));

	fseek(pFile, -1, SEEK_CUR);
}
