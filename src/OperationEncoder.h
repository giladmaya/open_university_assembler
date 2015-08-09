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

int calculate_operation_size(transition_data* transition, decoded_operation* current_operation);

void update_transition_with_last_operation(transition_data* transition, decoded_operation* decoded_operation);

bool encode_operation(transition_data* transition, decoded_operation* p_decoded_operation, compiler_output_files* output_files);
bool encode_operands(transition_data* transition, decoded_operation* p_decoded_operation, compiler_output_files* output_files);

bool encode_direct(transition_data* transition, char* operand, compiler_output_files* output_files);
bool encode_registers(transition_data* transition, char* source_register, char* target_register, FILE* p_file);
bool encode_immediate(transition_data* transition, char* operand, FILE* p_file);

machine_operation_definition* search_machine_operation_in_list(char* operation);
bool init_operation_list();
bool add_operation_to_list(char* name, unsigned int code, int operands);

void print_encoding_to_file(unsigned int ic, unsigned int value, FILE* p_file);

void free_operation_list();

#endif /* OPERATIONENCODER_H_ */
