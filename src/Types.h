/*
 * Types.h
 *
 *  Created on: Jul 9, 2015
 *      Author: student
 */

#ifndef TYPES_H_
#define TYPES_H_

#include "Enums.h"
#include <stdbool.h>
#include <stdio.h>

/*
 * A symbol in the code.
 * Can be an instruction or an operation.
 */
typedef struct
{
	char* name;
	int is_instruction;
	int is_external;
	unsigned int address;
} symbol;

typedef struct symbol_node* symbol_node_ptr;

/*
 * A symbol node in the symbol table.
 */
typedef struct symbol_node {
	symbol data;
	symbol_node_ptr next;
} symbol_node;

/*
 * An instruction. The instruction is built out of 12 bits;
 */
typedef struct {
	unsigned int era : 2;
	unsigned int target_operand_address_method : 2;
	unsigned int source_operand_address_method : 2;
	unsigned int op_code : 4;
	unsigned int group : 2;
	unsigned int rest : 20;
} coded_operation;

typedef union {
	coded_operation operation_bits;
	unsigned int operation_value;
} coded_operation_union;

typedef struct {
	unsigned int number : 12;
	unsigned int rest : 22;
} data_value;

typedef union {
	data_value value;
	unsigned int numberic_value;
} data_value_bits;

typedef struct data {
	data_value_bits value_bits;
	unsigned int address;
} data;

typedef struct data_node* data_node_ptr;

typedef struct data_node {
	data value;
	data_node_ptr next;
} data_node;

typedef struct line_info {
	char* line_str;
	int line_length;
	int current_index;
	int line_number;
	char* file_name;
	bool is_error;
} line_info;

typedef struct {
	char* name;
	unsigned int code;
	unsigned int operands_number;
} operation_information;

typedef struct operation_node* operation_information_node_ptr;

typedef struct operation_node {
	operation_information data;
	operation_information_node_ptr next;
} operation_information_node;

typedef struct {
	operation_information* operation;
	int times;
	char* source_operand;
	char* target_operand;
	ADDRESS_METHOD source_operand_address_method;
	ADDRESS_METHOD target_operand_address_method;
} operation;

typedef struct {
	unsigned int era : 2;
	unsigned int target_register_address : 5;
	unsigned int source_register_address : 5;
	unsigned int rest : 20;
} register_memory_word;

typedef struct {
	unsigned int era: 2;
	unsigned int operand_address : 10;
	unsigned int rest : 20;
} non_register_memory_word;

typedef union {
	register_memory_word register_address;
	non_register_memory_word non_register_address;
	unsigned int value;
} memory_word;

typedef struct {
	FILE* extern_file;
	FILE* entry_file;
	FILE* ob_file;
} compiler_output_files;

typedef struct {
	char* prev_operation_operand;
	ADDRESS_METHOD prev_operand_address_method;
	unsigned int IC;
	unsigned int DC;
	bool is_compiler_error;
	bool is_runtimer_error;
	line_info* current_line_information;
} transition_data;

#endif /* TYPES_H_ */
