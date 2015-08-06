/*
 * Enums.h
 *
 *  Created on: Jul 9, 2015
 *      Author: student
 */

#ifndef ENUMS_H_
#define ENUMS_H_

/*
 * Addressing methods
 */
typedef enum ADDRESS_METHOD
{
	INVALID = -1,
	IMMEDIATE = 0,
	DIRECT = 1,
	COPY_PREVIOUS = 2,
	DIRECT_REGISTER = 3
} ADDRESS_METHOD;

/*
 * Error codes
 */
typedef enum ERROR_CODES
{
	COMPILER_BAD_ALLOCATION
} ERROR_CODES;

typedef enum {
	ABSOLUTE = 0,
	EXTERNAL = 1,
	RELOCATABLE = 2
} ERA;

typedef enum {
	NO_OPERANDS = 0,
	ONE_OPERAND = 1,
	TWO_OPERANDS = 2
} OPERANDS_COUNT;

#endif /* ENUMS_H_ */
