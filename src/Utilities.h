/*
 ====================================================================================
 Name		: 	Utilities.h

 Author's	: 	Maya Gilad 302526850, Idan Levi 301242434

 Description: 	This file holds utils methods used to process assembly line
 ====================================================================================
 */

#ifndef UTILITIES_H_
#define UTILITIES_H_

#include "Types.h"
#include "Enums.h"
#include <stdbool.h>

/*
 * Description: Checks if operand is a register
 * Input:		1. Operand
				2. Operand length
 * Output:		True if register, otherwise false
 */
bool is_register(char* operand);

/*
 * Description: Prints a compiler error to the user
 * Input:		1. Message
 * 				2. Current line
 */
void print_compiler_error(char* message, line_info* info);

/*
 * Description: Prints runtime error
 * Input:		Message
 */
void print_runtime_error(char* message);

/*
 * Description: Allocates a string
 * Input:		Number of tokens in string
 * Output:		Allocated string
 */
char* allocate_string(int string_length);

/*
 * Description: Allocates memory
 * Input:		Number of bytes to allocate
 * Output:		Pointer to number
 */
void* allocate_memory(int bytes);

/*
 * Description: Skips all the spaces until next word
 * Input:		Line information
 */
void skip_all_spaces(line_info* info);

/*
 * Description: Read next word from line
 * Input:		Current transition data
 * Output:		Extracted word
 */
char* get_next_word(transition_data* transition);

/*
 * Description: Skips the label
 * Input:		Line information
 */
void skip_label(line_info* info);

/*
 * Description: Creates a line information structure
 * Input:		1. File name
 * 				2. Line number
 * 				3. Line content
 * Output:		Pointer to line information
 */
line_info* create_line_info(char* file_name, int line_number, char* line_str);

/*
 * Description: Checks if this is an empty line or a comment
 * Input:		The line to check
 * Output:		Is this an empty line or a comment
 */
bool is_empty_or_comment(char* line);

/*
 * Description: Checks if there are tokens that aren't spaces after current index
 * Input:		Line information
 * Output:		True if found non-space token, otherwise false
 */
bool is_end_of_data_in_line(line_info* info);

/*
 * Description: Converts a number in base 10 to wanted base
 * Input:		1. Number in base 10
 * 				2. Wanted base
 * 				3. Minimal length of returned word
 * Output:		Number in wanted base
 */
char* convert_base10_to_target_base(unsigned int base10_number, int target_base, int minimal_returned_length);

/*
 * Description: Checks if the string is a valid label
 * Input:		String to check
 * Output:		True if label, otherwise false
 */
bool is_valid_label(char* str);

/*
 * Description: Checks if string is an operation name
 * Input:		String to check
 * Output:		True if string is an operation, otherwise false
 */
bool is_operation_name(char* str);

/*
 * Description: Checks if this is a valid operation line
 * Input:		1. Line information
 */
bool is_valid_is_operation_line (line_info* info);

/*
 * Description: Replaces the content of old string with current string
 * Input:		1. Old string
 * 				2. New string
 */
void replace_content(char** current_string, char* new_string);

/*
 * Description: Creates a transition data structure
 * Output:		Basic transition data
 */
transition_data* create_transition_data();

/*
 * Description: Creates an output file from given file name
 * Input:		1. File name without extension
 * 				2. File extension
 * Output:		Reference to created file
 */
FILE* create_output_file(char* file_name_without_extension, char* extension);

/*
 * Description: Checks if we've reached end of line
 * Input:		Line information
 * Output:		True if we've reached end of lines, otherwise false
 */
bool is_end_of_line(line_info* info);

#endif /* UTILITIES_H_ */
