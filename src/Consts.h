/*
 * Consts.h
 *
 *  Created on: Jul 14, 2015
 *      Author: student
 */

#ifndef CONSTS_H_
#define CONSTS_H_

#define MAX_ARGC 2
#define MAX_FILE_PATH 100

#define MAX_LINE_LENGTH 200

#define TARGET_BASE 4

#define ADDRESS_START 100

#define END_OF_LINE '\n'
#define END_OF_STRING '\0'
#define COMMENT ';'

#define QUOTATION '\"'

#define MINUS_SIGN '-'
#define PLUS_SIGN '+'
#define NUMBER_TOKEN_SEPERATOR ','
#define STRING_DATA_END 0

#define LABEL_END_TOKEN ':'
#define LABEL_MAX_LENGTH 30
#define OPERATION_LINE_MAX_LENGTH 80
#define OPERAND_SEPERATOR ','
#define IMMEDIATE_TOKEN '#'
#define COPY_PERVIOUS_STR "$$"
#define REGISTER_FIRST_TOKEN 'R'
#define REGISTERS_COUNT 8

/*
 * Instruction
 */
#define ENTRY_OPERATION ".entry"
#define STRING_OPERATION ".string"
#define DATA_OPERATION ".data"
#define EXTERN_OPERATION ".extern"

/*
 * Operations
 */
#define MOV_OPERATION "mov"
#define CMP_OPERATION "cmp"
#define ADD_OPERATION "add"
#define SUB_OPERATION "sub"
#define NOT_OPERATION "not"
#define CLR_OPERATION "clr"
#define LEA_OPERATION "lea"
#define INC_OPERATION "inc"
#define DEC_OPERATION "dec"
#define JMP_OPERATION "jmp"
#define BNE_OPERATION "bne"
#define RED_OPERATION "red"
#define PRN_OPERATION "prn"
#define JSR_OPERATION "jsr"
#define RTS_OPERATION "rts"
#define STOP_OPERATION "stop"

#endif /* CONSTS_H_ */
