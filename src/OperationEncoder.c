/*
 * OperationEncoder.c
 *
 *  Created on: Aug 4, 2015
 *      Author: student
 */

#include "Consts.h"
#include "OperationEncoder.h"
#include "Types.h"
#include "Utilities.h"
#include "SymbolTable.h"
#include "ExternEncoder.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Description: Get all data of operation in line
 * Input:		1. Line information
 * 				2. Can use copy previous address method
 * 				3. Previous address method
 * Output:		Operation data
 */
decoded_operation* get_operation_data(transition_data* transition) {
	char* operation_name = get_operation_name(transition->current_line_information);
	machine_operation_definition* p_operation_information = get_operation_info(operation_name);

	char* source_operand = NULL;
	char* target_operand = NULL;
	ADDRESS_METHOD source_adderss_method = IMMEDIATE;
	ADDRESS_METHOD target_adderss_method = IMMEDIATE;
	int times = get_operation_times_counter(transition->current_line_information);

	if (times != INVALID_OPEARTION_COUNTER) {
		switch (p_operation_information->operands_number) {
			case NO_OPERANDS: {
				/* Does nothing */
				break;
			}
			case ONE_OPERAND: {
				target_operand = get_next_operand(transition->current_line_information);
				target_adderss_method = get_address_method(transition->current_line_information, target_operand);
				break;
			}
			case TWO_OPERANDS: {
				source_operand = get_next_operand(transition->current_line_information);
				source_adderss_method = get_address_method(transition->current_line_information, source_operand);

				skip_all_spaces(transition->current_line_information);

				if (transition->current_line_information->line_str[transition->current_line_information->current_index] != OPERAND_SEPERATOR) {
					print_compiler_error("Missing comma between two operands", transition->current_line_information);
					return NULL;
				} else {
					transition->current_line_information->current_index++;
					target_operand = get_next_operand(transition->current_line_information);
					target_adderss_method = get_address_method(transition->current_line_information, target_operand);
				}

				break;
			}
		}

		/*
		 * Reads rest of line and verifies no invalid tokens exist
		 * Verifies the operands are valid for this operation
		 */
		if (is_end_of_data_in_line(transition->current_line_information) &&
			are_operands_valid(operation_name, source_adderss_method, target_adderss_method)) {
			/* If the operand is copy_previous it replaces */
			bool is_valid =
					replace_operand_method_if_needed(&source_adderss_method, &source_operand, transition);

			is_valid &= replace_operand_method_if_needed(&target_adderss_method, &target_operand, transition);

			if (!is_valid) {
				print_compiler_error("Invalid usage of Copy-Previous address method", transition->current_line_information);
			} else if (are_operands_valid(operation_name, source_adderss_method, target_adderss_method)) {
				decoded_operation* p_result_operation = (decoded_operation*)allocate_memory(sizeof(decoded_operation));

				p_result_operation->operation = p_operation_information;
				p_result_operation->source_operand = source_operand;
				p_result_operation->target_operand = target_operand;
				p_result_operation->source_operand_address_method = source_adderss_method;
				p_result_operation->target_operand_address_method = target_adderss_method;
				p_result_operation->times = times;

				if (p_operation_information->operands_number == TWO_OPERANDS) {
					replace_content(&(transition->prev_operation_operand), source_operand);
					transition->prev_operand_address_method = source_adderss_method;
				} else if (p_operation_information->operands_number == ONE_OPERAND){
					replace_content(&(transition->prev_operation_operand), target_operand);
					transition->prev_operand_address_method = target_adderss_method;
				}

				return p_result_operation;
			}
		} else {
			print_compiler_error("Invalid tokens after operands", transition->current_line_information);
		}
	}
	return NULL;
}

int get_operation_times_counter(line_info* p_info) {
	int times = INVALID_OPEARTION_COUNTER;

	/* This isn't a valid operation. An operation name must be followed by a number */
	if (!isdigit(p_info->line_str[p_info->current_index])) {
		print_compiler_error("An operation must be followed by a number", p_info);
	} else {
		char* number;
		int number_length = 0;
		int start_index = p_info->current_index;

		while (isdigit(p_info->line_str[p_info->current_index])) {
			number_length++;
			p_info->current_index++;
		};

		number = allocate_string(number_length + 1);
		strncpy(number, p_info->line_str + start_index, number_length);
		number[number_length] = END_OF_STRING;

		times = atoi(number);

		if (times <= 0) {
			times = INVALID_OPEARTION_COUNTER;
			print_compiler_error("The number after operation must be Natural (1, 2, 3, ..)", p_info);
		}
	}

	return times;
}

char* get_operation_name(line_info* p_info) {
	char* result_operation_name;
	char* operation_name;
	int operation_name_length = 0;

	skip_all_spaces(p_info);

	operation_name = p_info->line_str + p_info->current_index;

	while (isalpha(p_info->line_str[p_info->current_index])) {
		(p_info->current_index)++;
		operation_name_length++;
	}

	result_operation_name = allocate_string(operation_name_length + 1);

	strncpy(result_operation_name, operation_name, operation_name_length);
	result_operation_name[operation_name_length] = END_OF_STRING;

	return result_operation_name;
}

ADDRESS_METHOD get_address_method(line_info* p_info, char* operand) {
	int operand_length = strlen(operand);

	if (operand_length > 0) {
		if (operand[0] == IMMEDIATE_TOKEN) {
			int i = 1;

			if ((operand[1] == MINUS_SIGN) || (operand[1] == PLUS_SIGN)) {
				i++;

				if (operand_length == 1) {
					print_compiler_error("Immediate token # must be followed by a valid number", p_info);
					return INVALID;
				}
			}

			for (; i < operand_length; i++) {
				if (!isdigit(operand[i])) {
					print_compiler_error("Immediate token # must be followed by a valid number", p_info);
					return INVALID;
				}
			}

			return IMMEDIATE;
		} else if (strcmp(operand, COPY_PERVIOUS_STR) == 0) {
			return COPY_PREVIOUS;
		} else if (is_register(operand, operand_length)) {
			return DIRECT_REGISTER;
		} else {
			return DIRECT;
		}
	} else {
		print_compiler_error("Operand cannot be an empty string", p_info);
		return INVALID;
	}
}

/*
 * Checks whether the operands given after the operation name are valid
 */
bool are_operands_valid(char* operation_name, ADDRESS_METHOD source_method, ADDRESS_METHOD target_method) {
	/* This are invalid methods */
	if ((source_method == INVALID) || (target_method == INVALID))
		return false;

	/*
	 * mov, add, and sub operation
	 * First operand: all methods are valid
	 * Second operand: only direct and direct register are valid
	 */
	if ((strcmp(operation_name, MOV_OPERATION) == 0) ||
		(strcmp(operation_name, ADD_OPERATION) == 0) ||
		(strcmp(operation_name, SUB_OPERATION) == 0)) {
		return ((target_method == DIRECT) || (target_method == DIRECT_REGISTER));
	}
	/*
	 * not, clr, inc, dec operation
	 * Only one operand.
	 * First operand: only direct and direct register are valid
	 */
	else if ((strcmp(operation_name, NOT_OPERATION) == 0) ||
			(strcmp(operation_name, CLR_OPERATION) == 0) ||
			(strcmp(operation_name, INC_OPERATION) == 0) ||
			(strcmp(operation_name, DEC_OPERATION) == 0)) {
		return ((target_method == DIRECT) || (target_method == DIRECT_REGISTER));
	}
	/*
	 * lea operation
	 * First operand: only direct is valid
	 * Second operand: only direct and direct register are valid
	 */
	else if (strcmp(operation_name, LEA_OPERATION) == 0) {
		return (source_method == DIRECT) && ((target_method == DIRECT) || (target_method == DIRECT_REGISTER));
	}
	/*
	 * jmp, bne, red operation
	 * Only one operand
	 * First operand: not immediate
	 */
	else if ((strcmp(operation_name, JMP_OPERATION) == 0) ||
			   (strcmp(operation_name, BNE_OPERATION) == 0) ||
			   (strcmp(operation_name, RED_OPERATION) == 0)) {
		return target_method != IMMEDIATE;
	}
	/*
	 * jsr operation
	 * Only one operand
	 * First operand: only direct
	 */
	else if (strcmp(operation_name, JSR_OPERATION) == 0) {
		return target_method == DIRECT;
	}
	/*
	 * cmp, prn, rts, stop operation
	 * The method is irrelevant or no operands given
	 */
	else {
		return true;
	}
}

bool replace_operand_method_if_needed(ADDRESS_METHOD* current_address_method, char** current_operand, transition_data* transition) {
	if ((transition->prev_operation_operand == NULL) && (*current_address_method == COPY_PREVIOUS)) {
		return false;
	} else {
		if (*current_address_method == COPY_PREVIOUS) {
			*current_address_method = transition->prev_operand_address_method;
			replace_content(current_operand, transition->prev_operation_operand);
		}

		return true;
	}
}

bool encode_operation(decoded_operation* p_decoded_operation, unsigned int* ic, compiler_output_files* output_files) {
	encoded_operation coded_op;
	int i;

	coded_op.bits.source_operand_address_method =
			p_decoded_operation->source_operand_address_method;
	coded_op.bits.target_operand_address_method =
			p_decoded_operation->target_operand_address_method;
	coded_op.bits.group = p_decoded_operation->operation->operands_number;
	coded_op.bits.op_code = p_decoded_operation->operation->code;
	coded_op.bits.era = ABSOLUTE;
	coded_op.bits.rest = 0;

	for (i = 1; i <= p_decoded_operation->times; i++) {
		print_encoding_to_file(*ic + ADDRESS_START, coded_op.value, output_files->ob_file);

		(*ic)++;

		/*TODO: implement encode operands */
		if (p_decoded_operation->operation->operands_number > 0) {
			bool is_valid = encode_memory_word(p_decoded_operation, ic, output_files, NULL);

			if (!is_valid) {
				return is_valid;
			}
		}

	}

	return true;
}

bool encode_memory_word(decoded_operation* p_decoded_operation, unsigned int* ic, compiler_output_files* output_files, line_info* p_info) {
	bool is_valid;

	if ((p_decoded_operation->source_operand_address_method == DIRECT_REGISTER) &&
			(p_decoded_operation->target_operand_address_method == DIRECT_REGISTER)) {
		is_valid = encode_registers(
				p_decoded_operation->source_operand, p_decoded_operation->target_operand, *ic, p_info, output_files->ob_file);
		(*ic)++;
	} else {
		if (p_decoded_operation->operation->operands_number == 2) {

			if (p_decoded_operation->source_operand_address_method == DIRECT) {
				is_valid = encode_direct(p_decoded_operation->source_operand, *ic, p_info, output_files);
			} else if (p_decoded_operation->source_operand_address_method == DIRECT_REGISTER) {
				is_valid = encode_registers(p_decoded_operation->source_operand, NULL, *ic, p_info, output_files->ob_file);
			} else if (p_decoded_operation->source_operand_address_method == IMMEDIATE) {
				is_valid = encode_immediate(p_decoded_operation->source_operand, *ic, p_info, output_files->ob_file);
			}

			(*ic)++;
		}

		if (p_decoded_operation->target_operand_address_method == DIRECT) {
			is_valid = encode_direct(p_decoded_operation->target_operand, *ic, p_info, output_files);
		} else if (p_decoded_operation->target_operand_address_method == DIRECT_REGISTER) {
			is_valid = encode_registers(NULL, p_decoded_operation->target_operand, *ic, p_info, output_files->ob_file);
		} else if (p_decoded_operation->target_operand_address_method == IMMEDIATE) {
			is_valid = encode_immediate(p_decoded_operation->target_operand, *ic, p_info, output_files->ob_file);
		}

		(*ic)++;
	}

	return is_valid;
}

bool encode_direct(char* operand, unsigned int ic, line_info* p_info, compiler_output_files* output_files) {
	symbol_node_ptr p_symbol = search_symbol(operand);

	if (p_symbol == NULL) {
		print_compiler_error("Using unknown operand", p_info);
		return false;
	} else {
		memory_word word;

		word.non_register_address.operand_address = p_symbol->current_symbol.address;

		if (p_symbol->current_symbol.is_external) {
			word.non_register_address.era = EXTERNAL;

			write_extern_to_output_file(operand, ic + ADDRESS_START, output_files->extern_file);
		} else {

			word.non_register_address.era = RELOCATABLE;
		}

		word.non_register_address.rest = 0;

		print_encoding_to_file(ic + ADDRESS_START, word.value, output_files->ob_file);

		return true;
	}
}

bool encode_registers(char* source_register, char* target_register, unsigned int ic, line_info* p_info, FILE* p_file) {
	memory_word word;

	if (source_register != NULL) {
		word.register_address.source_register_address = atoi(source_register + 1);
	} else {
		word.register_address.source_register_address = NO_ADDRESS;
	}

	if (target_register != NULL) {
		word.register_address.target_register_address = atoi(target_register + 1);
	} else {
		word.register_address.target_register_address = NO_ADDRESS;
	}

	word.register_address.era = ABSOLUTE;
	word.register_address.rest = 0;

	print_encoding_to_file(ic + ADDRESS_START, word.value, p_file);

	return true;
}

bool encode_immediate(char* operand, unsigned int ic, line_info* p_info, FILE* p_file) {
	int number;
	memory_word word;

	number = atoi(operand + 1);

	word.non_register_address.operand_address = number;
	word.non_register_address.era = ABSOLUTE;
	word.non_register_address.rest = 0;

	print_encoding_to_file(ic + ADDRESS_START, word.value, p_file);

	return true;
}

void print_encoding_to_file(unsigned int address, unsigned int value, FILE* p_file) {
	char* base4_value;

	/* Print Address base value */
	base4_value = convert_base10_to_target_base(address, TARGET_BASE, TARGET_MEMORY_ADDRESS_WORD_LENGTH);
	fputs(base4_value, p_file);
	fputc(COLUMN_SEPARATOR, p_file);

	free(base4_value);

	/* Print operation base value */
	base4_value = convert_base10_to_target_base(value, TARGET_BASE, TARGET_OPERATION_WORD_LENGTH);
	fputs(base4_value, p_file);
	fputc(END_OF_LINE, p_file);
}
