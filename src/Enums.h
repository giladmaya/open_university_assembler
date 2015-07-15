/*
 * Enums.h
 *
 *  Created on: Jul 9, 2015
 *      Author: student
 */

#ifndef ENUMS_H_
#define ENUMS_H_

/*
 * Operation codes
 */
enum OP_CODES
{
	MOV = 0,
	CMP,
	ADD,
	SUB,
	NOT,
	CLR,
	LEA,
	INC,
	DEC,
	JMP,
	BNE,
	RED,
	PRN,
	JSR,
	RTS,
	STOP
};

/*
 * Addressing methods
 */
enum ADDRESS_METHOD
{
	IMMEDIATE = 0,
	DIRECT = 1,
	COPY_PREVIOUS = 2,
	DIRECT_REGISTER = 3
};

/*
 * Error codes
 */
enum ERROR_CODES
{
	COMPILER_BAD_ALLOCATION
};

#endif /* ENUMS_H_ */
