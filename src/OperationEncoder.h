/*
 * OperationEncoder.h
 *
 *  Created on: Aug 4, 2015
 *      Author: student
 */

#ifndef OPERATIONENCODER_H_
#define OPERATIONENCODER_H_

#include "Types.h"
#include <stdbool.h>
#include <stdio.h>

operation* get_operation_data(transition_data* transition);

char* get_operation_name(line_info* p_info);
int get_operation_times_counter(line_info* p_info);
bool are_operands_valid(char* operation_name, ADDRESS_METHOD first_operand, ADDRESS_METHOD second_operand);
ADDRESS_METHOD get_address_method(line_info* p_info, char* operand);
bool replace_operand_method_if_needed(ADDRESS_METHOD* current_address_method, char** current_operand, transition_data* transition);

bool encode_operation(operation* p_decoded_operation, unsigned int* ic, compiler_output_files* output_files);
bool encode_memory_word(operation* p_decoded_operation, unsigned int* ic, compiler_output_files* output_files, line_info* p_info);

bool encode_direct(char* operand, unsigned int ic, line_info* p_info, compiler_output_files* output_files);
bool encode_registers(char* source_register, char* target_register, unsigned int ic, line_info* p_info, FILE* p_file);
bool encode_immediate(char* operand, unsigned int ic, line_info* p_info, FILE* p_file);

void print_encoding_to_file(unsigned int ic, unsigned int value, FILE* p_file);

#endif /* OPERATIONENCODER_H_ */
