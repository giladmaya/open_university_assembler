/*
 ====================================================================================
 Name		: 	OperationEncoder.h

 Author's	: 	Maya Gilad 302526850, Idan Levi 301242434

 Description: 	This file holds methods relevant to the operation line processing
 ====================================================================================
 */

#ifndef OPERATIONENCODER_H_
#define OPERATIONENCODER_H_

#include "Types.h"
#include <stdbool.h>
#include <stdio.h>

/*
 * Description: Process an operation line on first transition
 * Input: 		1. Current transition data
 * 				2. Label value (NULL if doesn't exist)
 * 				3. Does label exist
 */
void first_transition_process_operation(transition_data* transition, char* label, bool is_symbol);

/*
 * Description: Process an operation line on second transition
 * Input:		1. Current transition data
 * 				2. Output files
 */
void second_transition_process_operation(transition_data* transition, compiler_output_files* p_output_files);

/*
 * Description: Get all data of operation in line
 * Input:		1. Line information
 * 				2. Can use copy previous address method
 * 				3. Previous address method
 * Output:		Operation data
 */
decoded_operation* get_decoded_operation(transition_data* transition);

/*
 * Description: Gets the operation name from the line
 * Input:		Line information
 * Output:		Operation name
 */
char* get_operation_name(transition_data* transition);

/*
 * Description: Gets how many times we need to encode the operation
 * Input:		1. Current transition
 * 				2. Out - how many times we need to encode the operation
 * Output:		Is the result valid
 */
bool get_operation_times_counter(transition_data* transition, int* times);

/*
 * Description: Reads the operands from the line
 * Input:		1. Current transition data
 * 				2. Number of operands to read
 * 				3. Returned source operand
 * 				4. Returned target operand
 * Output:		Are operands valid
 */
bool get_operands(transition_data* transition, int operands_count, char** source_operand, char** target_operand);

/*
 * Description: Reads the next operand from the line
 * Input:		Current transition
 * Output:		Operand
 */
char* get_next_operand(transition_data* transition);

/*
 * Description: Gets the address method according to the operand
 * Input:		1. Current transition
 * 				2. Current operand
 * Output:		Address method
 */
ADDRESS_METHOD get_address_method(transition_data* transition, char* operand);

/*
 * Description: Replaces the operand in case it is copy address
 * Input:		1. Current transition data
 * 				2. Current operand
 * 				3. Current operand address method
 * Output:		Is usage of Copy Address valid
 */
bool replace_operand_if_copy_address(transition_data* transition, char** operand, ADDRESS_METHOD* operand_address_method);

/*
 * Description: Checks whether the address methods used in the operation are authorized
 * Input:		Current decoded operation
 * Output:		Is authorized
 */
bool are_operand_methods_allowed_in_operation(decoded_operation* current_operation);

/*
 * Description: Calculates how many memory words are used to encode the operation
 * Input:		1. Current transition data
 * 				2. Decoded operation
 * Output:		How many memory words should be used to encode the operation
 */
int calculate_operation_size(transition_data* transition, decoded_operation* current_operation);

/*
 * Description: Updates the transition with the first operand of the operation
 * Input:		1. Current transition data
 * 				2. Current decoded operation
 */
void update_transition_with_last_operation(transition_data* transition, decoded_operation* decoded_operation);

/*
 * Description: Encodes an operation into its output file
 * Input:		1. Current transition data
 * 				2. Decoded operation
 * 				3. Output files
 * Output:		Was the operation encoded successfully
 */
bool encode_operation(transition_data* transition, decoded_operation* p_decoded_operation, compiler_output_files* output_files);

/*
 * Description: Encodes operands into output files
 * Input:		1. Current transition data
 * 				2. Decoded operation
 * 				3. Output files
 * Output:		Were operands encoded successfully
 */
bool encode_operands(transition_data* transition, decoded_operation* p_decoded_operation, compiler_output_files* output_files);

/*
 * Description: Encodes a direct operand
 * Input:		1. Current transition
 * 				2. Direct operand
 * 				3. Output files
 * Output:		Was operand encoded successfully
 */
bool encode_direct(transition_data* transition, char* operand, compiler_output_files* output_files);

/*
 * Description: Encodes register operands
 * Input:		1. Current transition
 * 				2. Source register
 * 				3. Target register
 * 				3. Output file
 * Output:		Were operands encoded successfully
 */
bool encode_registers(transition_data* transition, char* source_register, char* target_register, FILE* p_file);

/*
 * Description: Encodes a immediate number
 * Input:		1. Current transition
 * 				2. Immediate operand
 * 				3. Output file
 * Output:		Was operand encoded successfully
 */
bool encode_immediate(transition_data* transition, char* operand, FILE* p_file);

/*
 * Description: Searches for an operation name in a pre-defined list of valid operations
 * Input:		Operation name
 * Output:		If found returns machine operation information, otherwise NULL
 */
machine_operation_definition* search_machine_operation_in_list(char* operation);

/*
 * Description: Initializes the operation list used for encoding
 * Output:		Did the initialization end
 */
bool init_operation_list();

/*
 * Description: Adds an operation into the list
 * Input:		1. Operation name
 * 				2. Op code
 * 				3. Number of operands
 * Output:		Did add successfully
 */
bool add_operation_to_list(char* name, unsigned int code, int operands);

/*
 * Description: Free memory list
 */
void free_operation_list();

#endif /* OPERATIONENCODER_H_ */
