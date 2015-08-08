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

void first_transition_process_operation(transition_data* transition, char* label, bool is_symbol);

void second_transition_process_operation(transition_data* transition, compiler_output_files* p_output_files);

decoded_operation* get_decoded_operation(transition_data* transition);
char* get_operation_name(line_info* p_info);
bool get_operation_times_counter(transition_data* transition, int* times);
bool get_operands(transition_data* transition, int operands_count, char** source_operand, char** target_operand);
ADDRESS_METHOD get_address_method(transition_data* transition, char* operand);
bool replace_operand_if_copy_address(transition_data* transition, char** operand, ADDRESS_METHOD* operand_address_method);

bool are_operand_methods_allowed_in_operation(decoded_operation* current_operation);

int get_operation_size(transition_data* transition, decoded_operation* current_operation);

void update_transition_with_last_operation(transition_data* transition, decoded_operation* decoded_operation);

bool encode_operation(decoded_operation* p_decoded_operation, unsigned int* ic, compiler_output_files* output_files);
bool encode_memory_word(decoded_operation* p_decoded_operation, unsigned int* ic, compiler_output_files* output_files, line_info* p_info);

bool encode_direct(char* operand, unsigned int ic, line_info* p_info, compiler_output_files* output_files);
bool encode_registers(char* source_register, char* target_register, unsigned int ic, line_info* p_info, FILE* p_file);
bool encode_immediate(char* operand, unsigned int ic, line_info* p_info, FILE* p_file);

machine_operation_definition* search_machine_operation_in_list(char* operation);
void init_operation_list();
void add_operation_to_list(char* name, unsigned int code, int operands);

void print_encoding_to_file(unsigned int ic, unsigned int value, FILE* p_file);

#endif /* OPERATIONENCODER_H_ */
