/*
 * Consts.h
 *
 *  Created on: Jul 14, 2015
 *      Author: student
 */

#ifndef CONSTS_H_
#define CONSTS_H_

#define MAX_LINE_LENGTH 200
#define ENTRY_OPERATION ".entry"
#define STRING_OPERATION ".string"
#define DATA_OPERATION ".data"
#define EXTERN_OPERATION ".extern"
#define LABEL_COLON ':'
#define QUOTATION '\"'
#define END_OF_LINE '\n'
#define MINUS_SIGN '-'
#define NUMBER_TOKEN_SEPERATOR ','
#define STRING_DATA_END 0

/*
 * Operations
 */
#define MOV_OPERATION "MOV"
#define CMP_OPERATION "CMP"
#define ADD_OPERATION "ADD"
#define SUB_OPERATION "SUB"
#define NOT_OPERATION "NOT"
#define CLR_OPERATION "CLR"
#define LEA_OPERATION "LEA"
#define INC_OPERATION "INC"
#define DEC_OPERATION "DEC"
#define JMP_OPERATION "JMP"
#define BNE_OPERATION "BNE"
#define RED_OPERATION "RED"
#define PRN_OPERATION "PRN"
#define JSR_OPERATION "JSR"
#define RTS_OPERATION "RTS"
#define STOP_OPERATION "STOP"

#endif /* CONSTS_H_ */
