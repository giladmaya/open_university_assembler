/*
 * Utilities.c
 *
 *  Created on: Jul 14, 2015
 *      Author: student
 */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "Types.h"
#include "Consts.h"

void print_compiler_error(char* message, line_info* info) {
	fprintf(stderr, "File: %s Line: %d Error: %s", info->file_name, info->line_number, message);
}

void skip_all_spaces(line_info* info) {
	int index = info->current_index;

	while ((index < info->line_length) && isspace(info->line_str[index])) {
		index++;
	}

	info->current_index = index;
}

void get_word(line_info* info, char** word)
{
	int i, word_end_index, word_start_index, word_length;

	if (*word != NULL) {
		free(*word);
	}

	skip_all_spaces(info);

	word_end_index  = word_start_index = i = info->current_index;

	for (;i < info->line_length; i++) {
		if (!isspace(info->line_str[i]) && (info->line_str[i] != LABEL_COLON)) {
			word_end_index = i;
		}
		else
		{
			break;
		}
	}

	word_length = word_end_index - word_start_index + 1;

	*word = (char*)malloc(sizeof(char) * (word_length + 1));

	if (*word == NULL) {
		/* TODO: bad alloc */
	} else {
		strncpy(*word, info->line_str + word_start_index, word_length);
		(*word)[word_length] = '\0';

		info->current_index = word_end_index + 1;
	}
}
