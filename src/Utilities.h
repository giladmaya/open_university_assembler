/*
 * Utilities.h
 *
 *  Created on: Jul 14, 2015
 *      Author: student
 */

#ifndef UTILITIES_H_
#define UTILITIES_H_

void print_compiler_error(char* message, line_info* info);
void skip_all_spaces(line_info* info);
void get_word(line_info* info, char** word);
bool is_operation_name(char* symbol);

#endif /* UTILITIES_H_ */
