/*
 * Types.h
 *
 *  Created on: Jul 9, 2015
 *      Author: student
 */

#ifndef TYPES_H_
#define TYPES_H_

#include "Enums.h"
#include "Consts.h"
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
	symbol current_symbol;
	symbol_node_ptr next;
} symbol_node;

/*
 * Definition of a machine operation
 */
typedef union {
	struct {
		unsigned int era : OPERATION_ERA_BITS_LENGTH;
		unsigned int target_operand_address_method : OPERATION_ADDRESS_METHOD_BITS_LENGTH;
		unsigned int source_operand_address_method : OPERATION_ADDRESS_METHOD_BITS_LENGTH;
		unsigned int op_code : OPERATION_OP_CODE_BITS_LENGTH;
		unsigned int group : OPERATION_GROUP_BITS_LENGTH;
		unsigned int rest : REST_BITS_LENGTH;
	} bits;
	unsigned int value;
} encoded_operation;

/*
 * Data definition
 * A data is defined of a value (char or number) and an address
 */
typedef struct data {
	union {
		struct {
			unsigned int number : DATA_WORD_BITS_LENGTH;
			unsigned int rest : REST_BITS_LENGTH;
		} bits;
		unsigned int value;
	} encoded_data;
	unsigned int address;
} data_definition;

typedef struct data_node* data_node_ptr;

/*
 * Data Table Node
 */
typedef struct data_node {
	data_definition current_data;
	data_node_ptr next;
} data_node;

/*
 * Holds the line processing state
 */
typedef struct line_info {
	char* line_str;
	int line_length;
	int current_index;
	int line_number;
	char* file_name;
} line_info;

/*
 * Holds the definition of the machine operation as instructed in the manual
 */
typedef struct {
	char* name;
	unsigned int code;
	unsigned int operands_number;
} machine_operation_definition;

typedef struct operation_node* operation_information_node_ptr;

/*
 * Machine operation definitions node
 */
typedef struct operation_node {
	machine_operation_definition data;
	operation_information_node_ptr next;
} operation_information_node;

/*
 * Holds data of decoded operation
 */
typedef struct {
	machine_operation_definition* operation;
	int times;
	char* source_operand;
	char* target_operand;
	ADDRESS_METHOD source_operand_address_method;
	ADDRESS_METHOD target_operand_address_method;
} decoded_operation;

typedef union {
	struct {
		unsigned int era : OPERATION_ERA_BITS_LENGTH;
		unsigned int target_register_address : OPERAND_REGISTER_BITS_LENGTH;
		unsigned int source_register_address : OPERAND_REGISTER_BITS_LENGTH;
		unsigned int rest : REST_BITS_LENGTH;
	} register_address;
	struct {
		unsigned int era: OPERATION_ERA_BITS_LENGTH;
		unsigned int operand_address : OPERAND_NON_REGISTER_BITS_LENGTH;
		unsigned int rest : REST_BITS_LENGTH;
	} non_register_address;
	unsigned int value;
} memory_word;

/*
 * Holds output files
 */
typedef struct {
	FILE* extern_file;
	FILE* entry_file;
	FILE* ob_file;
} compiler_output_files;

/*
 * Holds transition data
 */
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
