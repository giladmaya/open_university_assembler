/*
 * Types.h
 *
 *  Created on: Jul 9, 2015
 *      Author: student
 */

#ifndef TYPES_H_
#define TYPES_H_

/**
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

typedef struct symbolNode* symbolNodePtr;

/**
 * A symbol node in the symbol table.
 */
typedef struct symbolNode
{
	symbol data;
	symbolNodePtr next;
} symbolNode;

/**
 * An instruction. The instruction is built out of 12 bits;
 */
typedef struct
{
	unsigned int era : 2;
	unsigned int target_operand_address_method : 2;
	unsigned int source_operand_address_method : 2;
	unsigned int op_code : 4;
	unsigned int group : 2;
} instruction;


#endif /* TYPES_H_ */
