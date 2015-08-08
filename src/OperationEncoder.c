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

/* Global variables */
operation_information_node_ptr p_operation_head = NULL;

/*
 * Description: Process an operation line on second transition
 * Input:		1. Current transition data
 * 				2. Output files
 */
void second_transition_process_operation(transition_data* transition, compiler_output_files* p_output_files) {
	/* Gets all data about the current operation */
	decoded_operation* p_decoded_operation = get_decoded_operation(transition);

	/* Encode the operation */
	encode_operation(p_decoded_operation, &(transition->IC), p_output_files);

	update_transition_with_last_operation(transition, p_decoded_operation);
}

/*
 * Description: Process an operation line on first transition
 * Input: 		1. Current transition data
 * 				2. Label value (NULL if doesn't exist)
 * 				3. Does label exist
 */
void first_transition_process_operation(transition_data* transition, char* label, bool is_symbol) {
	/* Step 11 */
	if (is_symbol) {
			symbol_node_ptr p_symbol = create_symbol(label, transition->IC, false, false);

			add_symbol_to_list(p_symbol);
	}

	/* Gets all data about the current operation */
	decoded_operation* p_decoded_operation = get_decoded_operation(transition);

	if (p_decoded_operation != NULL) {
		/* Checks if the operands used in the operation are authorized */
		if (are_operand_methods_allowed_in_operation(p_decoded_operation)) {

			transition->IC += get_operation_size(transition, p_decoded_operation);

			update_transition_with_last_operation(transition, p_decoded_operation);

		} else {
			print_compiler_error("Unauthorized address methods", transition->current_line_information);
			transition->is_compiler_error = true;
		}
	}
}

/*
 * Description: Updates the transition with the first operand of the operation
 * Input:		1. Current transition data
 * 				2. Current decoded operation
 */
void update_transition_with_last_operation(transition_data* transition, decoded_operation* decoded_operation) {
	if (decoded_operation->source_operand != NULL) {
		replace_content(&transition->prev_operation_operand, decoded_operation->source_operand);
		transition->prev_operand_address_method = decoded_operation->source_operand_address_method;
	} else if (decoded_operation->target_operand != NULL) {
		replace_content(&transition->prev_operation_operand, decoded_operation->target_operand);
		transition->prev_operand_address_method = decoded_operation->target_operand_address_method;
	} else {
		if (transition->prev_operation_operand != NULL) {
			free(transition->prev_operation_operand);
			transition->prev_operation_operand = NULL;
		}
	}
}

/*
 * Description: Calculates how many memory words are used to encode the operation
 * Input:		1. Current transition data
 * 				2. Decoded operation
 * Output:		Memory words size for the encoded operation
 */
int get_operation_size(transition_data* transition, decoded_operation* current_operation) {
	int size = OPERAION_MIN_WORD_SIZE;

	/* If both operands are registers they share the same memory word */
	if ((current_operation->source_operand_address_method == DIRECT_REGISTER) &&
			(current_operation->target_operand_address_method == DIRECT_REGISTER)) {
		size++;
	} else {
		size += current_operation->operation->operands_number;
	}

	return size * current_operation->times;
}

/*
 * Description: Get all data of operation in line
 * Input:		1. Line information
 * 				2. Can use copy previous address method
 * 				3. Previous address method
 * Output:		Operation data
 */
decoded_operation* get_decoded_operation(transition_data* transition) {
	decoded_operation* current_operation;

	/* Get operation name from line */
	char* operation_name = get_operation_name(transition->current_line_information);

	/* Find operation in machine operation list */
	machine_operation_definition* p_operation_information = search_machine_operation_in_list(operation_name);

	/* The operation name isn't valid */
	if (p_operation_information == NULL) {
		print_compiler_error("Invalid operation name", transition->current_line_information);
		transition->is_compiler_error = true;
		return NULL;
	} else {
		int times;

		/* Gets the number of times to encode the operation */
		if (get_operation_times_counter(transition, &times)) {
			char* source_operand = NULL;
			char* target_operand = NULL;

			/* Get the operands to encode in the operation */
			if (get_operands(transition, p_operation_information->operands_number, &source_operand, &target_operand)) {
				/* Gets the address methods to use in the encoding */
				ADDRESS_METHOD source_operand_address_method = get_address_method(transition, source_operand);
				ADDRESS_METHOD target_operand_address_method = get_address_method(transition, target_operand);

				/* No error occurred while trying to get address method */
				if ((source_operand_address_method != INVALID_ADDRESS_METHOD) && (target_operand_address_method != INVALID_ADDRESS_METHOD)) {
					/* Replaces operands in case they're copy address */
					if (replace_operand_if_copy_address(transition, &source_operand, &source_operand_address_method) &&
							replace_operand_if_copy_address(transition, &target_operand, &target_operand_address_method)) {

						current_operation = allocate_memory(sizeof(decoded_operation));

						/* Initialize the operation according to data extracted from line */
						current_operation->operation = p_operation_information;
						current_operation->source_operand = source_operand;
						current_operation->source_operand_address_method = source_operand_address_method;
						current_operation->target_operand = target_operand;
						current_operation->target_operand_address_method = target_operand_address_method;
						current_operation->times = times;
					}
				}
			}
		}
	}

	return current_operation;
}

/*
 * Description: Replaces the operand in case it is copy address
 * Input:		1. Current transition data
 * 				2. Current operand
 * 				3. Current operand address method
 * Output:		Is usage of Copy Address valid
 */
bool replace_operand_if_copy_address(transition_data* transition, char** operand, ADDRESS_METHOD* operand_address_method) {
	bool is_valid = true;

	if (*operand_address_method == COPY_PREVIOUS) {
		if (transition->prev_operation_operand != NULL) {
			replace_content(operand, transition->prev_operation_operand);
			*operand_address_method = transition->prev_operand_address_method;
		} else {
			print_compiler_error("Invalid usage of $$. It must be used after an operation with operands", transition->current_line_information);
			transition->is_compiler_error = true;
			is_valid = false;
		}
	}

	return is_valid;
}

/*
 * Description: Reads the operands from the line
 * Input:		1. Current transition data
 * 				2. Number of operands to read
 * 				3. Returned source operand
 * 				4. Returned target operand
 * Output:		Are operands valid
 */
bool get_operands(transition_data* transition, int operands_count, char** source_operand, char** target_operand) {
	bool is_valid = true;

	switch (operands_count) {
		case NO_OPERANDS: {
			*source_operand = NULL;
			*target_operand = NULL;
			break;
		}
		case ONE_OPERAND: {
			*source_operand = NULL;
			*target_operand = get_next_operand(transition->current_line_information);
			break;
		}
		case TWO_OPERANDS: {
			*source_operand = get_next_operand(transition->current_line_information);

			skip_all_spaces(transition->current_line_information);

			if (transition->current_line_information->line_str[transition->current_line_information->current_index] != OPERAND_SEPERATOR) {
				print_compiler_error("Missing comma between two operands", transition->current_line_information);
				transition->is_compiler_error = true;
				is_valid = false;
			} else {
				transition->current_line_information->current_index++;
				*target_operand = get_next_operand(transition->current_line_information);
			}

			break;
		}
	}

	/* Searches for tokens after operation */
	if (is_valid && !is_end_of_data_in_line(transition->current_line_information)) {
		print_compiler_error("Found unexpected token on end of operation definition", transition->current_line_information);
		transition->is_compiler_error = true;
		is_valid = false;
	}

	/* The operands aren't valid. Releases memory allocated */
	if (!is_valid) {
		if (*source_operand != NULL) {
			free(*source_operand);
		}

		if (*target_operand != NULL) {
			free(*target_operand);
		}
	}

	return is_valid;
}

bool get_operation_times_counter(transition_data* transition, int* times) {
	/* This isn't a valid operation. An operation name must be followed by a number */
	if (!isdigit(transition->current_line_information->line_str[transition->current_line_information->current_index])) {
		print_compiler_error("An operation must be followed by a number", transition->current_line_information);
		transition->is_compiler_error = true;
		return false;
	} else {
		char* number;
		int number_length = 0;
		int start_index = transition->current_line_information->current_index;

		while (isdigit(transition->current_line_information->line_str[transition->current_line_information->current_index])) {
			number_length++;
			transition->current_line_information->current_index++;
		};

		number = allocate_string(number_length + 1);
		strncpy(number, transition->current_line_information->line_str + start_index, number_length);
		number[number_length] = END_OF_STRING;

		*times = atoi(number);

		if (times <= 0) {
			print_compiler_error("The number after operation must be Natural (1, 2, 3, ..)", transition->current_line_information);
			transition->is_compiler_error = true;
			return false;
		}
	}

	return true;
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

ADDRESS_METHOD get_address_method(transition_data* transition, char* operand) {
	if (operand == NULL) {
		return IMMEDIATE;
	} else {
		int operand_length = strlen(operand);

		if (operand_length > 0) {
			if (operand[0] == IMMEDIATE_TOKEN) {
				int i = 1;

				if ((operand[1] == MINUS_SIGN) || (operand[1] == PLUS_SIGN)) {
					i++;

					if (operand_length == 1) {
						print_compiler_error("Immediate token # must be followed by a valid number", transition->current_line_information);
						transition->is_compiler_error = true;
						return INVALID_ADDRESS_METHOD;
					}
				}

				for (; i < operand_length; i++) {
					if (!isdigit(operand[i])) {
						print_compiler_error("Immediate token # must be followed by a valid number", transition->current_line_information);
						transition->is_compiler_error = true;
						return INVALID_ADDRESS_METHOD;
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
			print_compiler_error("Operand cannot be an empty string", transition->current_line_information);
			transition->is_compiler_error = true;
			return INVALID_ADDRESS_METHOD;
		}
	}
}

/*
 * Description: Checks whether the address methods used in the operation are authorized
 * Input:		Current decoded operation
 * Output:		Is authorized
 */
bool are_operand_methods_allowed_in_operation(decoded_operation* current_operation) {
	/* This are invalid methods */
	if ((current_operation->source_operand_address_method == INVALID_ADDRESS_METHOD) ||
			(current_operation->target_operand_address_method == INVALID_ADDRESS_METHOD))
		return false;

	/*
	 * mov, add, and sub operation
	 * First operand: all methods are valid
	 * Second operand: only direct and direct register are valid
	 */
	if ((strcmp(current_operation->operation->name, MOV_OPERATION) == 0) ||
		(strcmp(current_operation->operation->name, ADD_OPERATION) == 0) ||
		(strcmp(current_operation->operation->name, SUB_OPERATION) == 0)) {
		return ((current_operation->target_operand_address_method == DIRECT) ||
				(current_operation->target_operand_address_method == DIRECT_REGISTER));
	}
	/*
	 * not, clr, inc, dec operation
	 * Only one operand.
	 * First operand: only direct and direct register are valid
	 */
	else if ((strcmp(current_operation->operation->name, NOT_OPERATION) == 0) ||
			(strcmp(current_operation->operation->name, CLR_OPERATION) == 0) ||
			(strcmp(current_operation->operation->name, INC_OPERATION) == 0) ||
			(strcmp(current_operation->operation->name, DEC_OPERATION) == 0)) {
		return ((current_operation->target_operand_address_method == DIRECT) ||
				(current_operation->target_operand_address_method == DIRECT_REGISTER));
	}
	/*
	 * lea operation
	 * First operand: only direct is valid
	 * Second operand: only direct and direct register are valid
	 */
	else if (strcmp(current_operation->operation->name, LEA_OPERATION) == 0) {
		return (current_operation->source_operand_address_method == DIRECT) &&
				((current_operation->target_operand_address_method == DIRECT) ||
						(current_operation->target_operand_address_method == DIRECT_REGISTER));
	}
	/*
	 * jmp, bne, red operation
	 * Only one operand
	 * First operand: not immediate
	 */
	else if ((strcmp(current_operation->operation->name, JMP_OPERATION) == 0) ||
			   (strcmp(current_operation->operation->name, BNE_OPERATION) == 0) ||
			   (strcmp(current_operation->operation->name, RED_OPERATION) == 0)) {
		return current_operation->target_operand_address_method != IMMEDIATE;
	}
	/*
	 * jsr operation
	 * Only one operand
	 * First operand: only direct
	 */
	else if (strcmp(current_operation->operation->name, JSR_OPERATION) == 0) {
		return current_operation->target_operand_address_method == DIRECT;
	}
	/*
	 * cmp, prn, rts, stop operation
	 * The method is irrelevant or no operands given
	 */
	else {
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


/*
 * Description: Searches for an operation name in a pre-defined list of valid operations
 * Input:		Operation name
 * Output:		If found returns machine operation information, otherwise NULL
 */
machine_operation_definition* search_machine_operation_in_list(char* operation) {
	operation_information_node_ptr p_current;

	if (p_operation_head == NULL) {
		init_operation_list();
	}

	p_current = p_operation_head;

	while (p_current != NULL) {
		if (strcmp(p_current->data.name, operation) == 0) {
			return &(p_current->data);
		}

		p_current = p_current->next;
	}

	return NULL;
}

void init_operation_list() {
	int op_code = 0;

	add_operation_to_list(MOV_OPERATION, op_code++, TWO_OPERANDS);
	add_operation_to_list(CMP_OPERATION, op_code++, TWO_OPERANDS);
	add_operation_to_list(ADD_OPERATION, op_code++, TWO_OPERANDS);
	add_operation_to_list(SUB_OPERATION, op_code++, TWO_OPERANDS);
	add_operation_to_list(NOT_OPERATION, op_code++, ONE_OPERAND);
	add_operation_to_list(CLR_OPERATION, op_code++, ONE_OPERAND);
	add_operation_to_list(LEA_OPERATION, op_code++, TWO_OPERANDS);
	add_operation_to_list(INC_OPERATION, op_code++, ONE_OPERAND);
	add_operation_to_list(DEC_OPERATION, op_code++, ONE_OPERAND);
	add_operation_to_list(JMP_OPERATION, op_code++, ONE_OPERAND);
	add_operation_to_list(BNE_OPERATION, op_code++, ONE_OPERAND);
	add_operation_to_list(RED_OPERATION, op_code++, ONE_OPERAND);
	add_operation_to_list(PRN_OPERATION, op_code++, ONE_OPERAND);
	add_operation_to_list(JSR_OPERATION, op_code++, ONE_OPERAND);
	add_operation_to_list(RTS_OPERATION, op_code++, NO_OPERANDS);
	add_operation_to_list(STOP_OPERATION, op_code++, NO_OPERANDS);
}

void add_operation_to_list(char* name, unsigned int code, int operands) {
	operation_information_node_ptr p_new = (operation_information_node_ptr)malloc(sizeof(operation_information_node));

	if (p_new == NULL) {
		/* Todo: bad alloc */
		/*??*/
		print_runtime_error("Could not allocate memory. Exit program");
	} else {
		p_new->data.name = name;
		p_new->data.code = code;
		p_new->data.operands_number = operands;
		p_new->next = NULL;

		if (p_operation_head == NULL) {
			p_operation_head = p_new;
		} else {
			p_new->next = p_operation_head;
			p_operation_head = p_new;
		}
	}
}
