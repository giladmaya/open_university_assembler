/*
 ====================================================================================
 Name		: 	OperationEncoder.c

 Author's	: 	Maya Gilad 302526850, Idan Levi 301242434

 Description: 	This file holds methods relevant to the operation line processing
 ====================================================================================
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
	encode_operation(transition, p_decoded_operation, p_output_files);

	/* Updates the transition with the source operand of the current operation */
	update_transition_with_last_operation(transition, p_decoded_operation);
}

/*
 * Description: Process an operation line on first transition
 * Input: 		1. Current transition data
 * 				2. Label value (NULL if doesn't exist)
 * 				3. Does label exist
 */
void first_transition_process_operation(transition_data* transition, char* label, bool is_symbol) {
	/* Checks if the line is valid for an operation */
	if (!is_valid_is_operation_line(transition->current_line_information)) {
		print_compiler_error("Line exceeds max length", transition->current_line_information);
		transition->is_compiler_error = true;
		return;
	}

	/* Step 11 - Check if there is a symbol in the beginning of line */
	if (is_symbol) {
		/* Checks if the symbol was defined in previous lines */
		symbol_node_ptr p_searched_symbol = search_symbol(label);

		if (p_searched_symbol == NULL) {

			/* Creates a new symbol and adds it to the symbol table, or die */
			symbol_node_ptr p_symbol = create_symbol(label, transition->IC, false, false);
			if (p_symbol != NULL) {
				add_symbol_to_list(p_symbol);
			} else {
				transition->is_runtimer_error = true;
				free(label);
				return;
			}
		} else {
			print_compiler_error("Each label can be defined only once", transition->current_line_information);
			transition->is_compiler_error = true;
			free(label);
			return;
		}
	}

	/* Gets all data about the current operation */
	decoded_operation* p_decoded_operation = get_decoded_operation(transition);

	if (p_decoded_operation != NULL) {
		/* Checks if the operands used in the operation are authorized */
		if (are_operand_methods_allowed_in_operation(p_decoded_operation)) {

			/* Calculate the operation's size */
			transition->IC += calculate_operation_size(transition, p_decoded_operation);

			/* Updates the transition with the source operand of the current operation */
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
		/* Updates the operand for copy-previous with the source operand of the current operation */
		replace_content(&transition->prev_operation_operand, decoded_operation->source_operand);
		transition->prev_operand_address_method = decoded_operation->source_operand_address_method;
	} else if (decoded_operation->target_operand != NULL) {
		/* Updates the operand for copy-previous with the target operand of the current operation */
		replace_content(&transition->prev_operation_operand, decoded_operation->target_operand);
		transition->prev_operand_address_method = decoded_operation->target_operand_address_method;
	} else {
		/*
		 * The current operation has no operands, therefore we cannot use copy previous in the
		 * next operation
		 */
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
 * Output:		How many memory words should be used to encode the operation
 */
int calculate_operation_size(transition_data* transition, decoded_operation* current_operation) {
	/* We need to encode the operation */
	int size = OPERAION_MIN_WORD_SIZE;

	/* If both operands are registers they share the same memory word */
	if ((current_operation->source_operand_address_method == DIRECT_REGISTER) &&
			(current_operation->target_operand_address_method == DIRECT_REGISTER)) {
		size++;
	} else {
		/* Each operand needs its own memory word */
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
	decoded_operation* current_operation = NULL;

	/* Get operation name from line, or die */
	char* operation_name = get_operation_name(transition);
	if (transition->is_runtimer_error) {
		return NULL;
	}

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

						if (current_operation == NULL) {
							transition->is_runtimer_error = true;
						} else {
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

	/* The operand is $$ */
	if (*operand_address_method == COPY_PREVIOUS) {
		/* The last operation had at least one operand */
		if (transition->prev_operation_operand != NULL) {
			/* Replaces the operand from $$ to the first operand of the last operation */
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
			/* There aren't any operands */
			*source_operand = NULL;
			*target_operand = NULL;
			break;
		}
		case ONE_OPERAND: {
			/* Reads the target operand from the line */
			*source_operand = NULL;
			*target_operand = get_next_operand(transition);
			break;
		}
		case TWO_OPERANDS: {
			/* Reads the source operand from the line */
			*source_operand = get_next_operand(transition);

			if (transition->is_runtimer_error) {
				is_valid = false;
				break;
			}

			skip_all_spaces(transition->current_line_information);

			/* There must be a comma between operands */
			if (transition->current_line_information->line_str[transition->current_line_information->current_index] != OPERAND_SEPERATOR) {
				print_compiler_error("Missing comma between two operands", transition->current_line_information);
				transition->is_compiler_error = true;
				is_valid = false;
			} else {
				/* Reads the target operand from the line */
				transition->current_line_information->current_index++;
				*target_operand = get_next_operand(transition);
			}

			break;
		}
	}

	if (transition->is_runtimer_error) {
		is_valid = false;
	}
	/* Searches for tokens after operation */
	else if (is_valid && !is_end_of_data_in_line(transition->current_line_information)) {
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

/*
 * Description: Gets how many times we need to encode the operation
 * Input:		1. Current transition
 * 				2. Out - how many times we need to encode the operation
 * Output:		Is the result valid
 */
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

		/* Searches for the position of the number in the line */
		while (isdigit(transition->current_line_information->line_str[transition->current_line_information->current_index])) {
			number_length++;
			transition->current_line_information->current_index++;
		};

		/* Creates a representation of the number */
		number = allocate_string(number_length + 1);
		strncpy(number, transition->current_line_information->line_str + start_index, number_length);
		number[number_length] = END_OF_STRING;

		/* Converts to valid integer */
		*times = atoi(number);

		/* This isn't a number or it is in invalid range */
		if (times <= 0) {
			print_compiler_error("The number after operation must be Natural (1, 2, 3, ..)", transition->current_line_information);
			transition->is_compiler_error = true;
			return false;
		}
	}

	return true;
}

/*
 * Description: Gets the operation name from the line
 * Input:		Line information
 * Output:		Operation name
 */
char* get_operation_name(transition_data* transition) {
	char* result_operation_name;
	char* operation_name;
	int operation_name_length = 0;

	skip_all_spaces(transition->current_line_information);

	operation_name = transition->current_line_information->line_str + transition->current_line_information->current_index;

	/* Searches for the position of the operation inside the line */
	while (isalpha(transition->current_line_information->line_str[transition->current_line_information->current_index])) {
		(transition->current_line_information->current_index)++;
		operation_name_length++;
	}

	result_operation_name = allocate_string(operation_name_length);

	if (result_operation_name != NULL) {
		/* Copies the operation's name */
		strncpy(result_operation_name, operation_name, operation_name_length);
		result_operation_name[operation_name_length] = END_OF_STRING;
	} else {
		transition->is_runtimer_error = true;
	}

	return result_operation_name;
}

/*
 * Description: Gets the address method according to the operand
 * Input:		1. Current transition
 * 				2. Current operand
 * Output:		Address method
 */
ADDRESS_METHOD get_address_method(transition_data* transition, char* operand) {
	/* The operation has no source or target operand. Use IMMEDIATE method for encoding */
	if (operand == NULL) {
		return IMMEDIATE;
	} else {
		int operand_length = strlen(operand);

		if (operand_length > 0) {
			/* Operand starts with a # */
			if (operand[0] == IMMEDIATE_TOKEN) {
				int i = 1;

				/* Operand has + or - on its' start */
				if ((operand[1] == MINUS_SIGN) || (operand[1] == PLUS_SIGN)) {
					i++;

					if (operand_length == 1) {
						print_compiler_error("Immediate token # must be followed by a valid number", transition->current_line_information);
						transition->is_compiler_error = true;
						return INVALID_ADDRESS_METHOD;
					}
				}

				/* Check all operand's tokens are valid digits */
				for (; i < operand_length; i++) {
					if (!isdigit(operand[i])) {
						print_compiler_error("Immediate token # must be followed by a valid number", transition->current_line_information);
						transition->is_compiler_error = true;
						return INVALID_ADDRESS_METHOD;
					}
				}

				return IMMEDIATE;
			}
			/* The operand is equal to $$ */
			else if (strcmp(operand, COPY_PERVIOUS_STR) == 0) {
				return COPY_PREVIOUS;
			}
			/* The operand is r0-r7 */
			else if (is_register(operand)) {
				return DIRECT_REGISTER;
			}
			/* The operand is a variable */
			else if (is_valid_label(operand)) {
				return DIRECT;
			} else {
				print_compiler_error("Operand isn't a valid label, register, number or $$", transition->current_line_information);
				transition->is_compiler_error = true;
				return INVALID_ADDRESS_METHOD;
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

/*
 * Description: Encodes an operation into its output file
 * Input:		1. Current transition data
 * 				2. Decoded operation
 * 				3. Output files
 * Output:		Was the operation encoded successfully
 */
bool encode_operation(transition_data* transition, decoded_operation* p_decoded_operation, compiler_output_files* output_files) {
	encoded_operation coded_op;
	int i;

	/* Initlaizes the encoded operation with its' values */
	coded_op.bits.source_operand_address_method =
			p_decoded_operation->source_operand_address_method;
	coded_op.bits.target_operand_address_method =
			p_decoded_operation->target_operand_address_method;
	coded_op.bits.group = p_decoded_operation->operation->operands_number;
	coded_op.bits.op_code = p_decoded_operation->operation->code;
	coded_op.bits.era = ABSOLUTE;
	coded_op.bits.rest = 0;

	/* Encode the operation */
	for (i = 1; i <= p_decoded_operation->times; i++) {
		print_encoding_to_file(transition->IC + ADDRESS_START, coded_op.value, output_files->ob_file);

		transition->IC++;

		if (p_decoded_operation->operation->operands_number > 0) {
			bool is_valid = encode_operands(transition, p_decoded_operation, output_files);

			if (!is_valid) {
				return is_valid;
			}
		}

	}

	return true;
}

/*
 * Description: Encodes operands into output files
 * Input:		1. Current transition data
 * 				2. Decoded operation
 * 				3. Output files
 * Output:		Were operands encoded successfully
 */
bool encode_operands(transition_data* transition, decoded_operation* p_decoded_operation, compiler_output_files* output_files) {
	bool is_valid;

	/* Both source and target operands are registers */
	if ((p_decoded_operation->source_operand_address_method == DIRECT_REGISTER) &&
			(p_decoded_operation->target_operand_address_method == DIRECT_REGISTER)) {
		is_valid = encode_registers(
				transition, p_decoded_operation->source_operand,
				p_decoded_operation->target_operand, output_files->ob_file);
		transition->IC++;
	} else {
		/* The operation has two operands */
		if (p_decoded_operation->operation->operands_number == 2) {
			/* Encode the source operand */
			if (p_decoded_operation->source_operand_address_method == DIRECT) {
				is_valid = encode_direct(transition, p_decoded_operation->source_operand, output_files);
			} else if (p_decoded_operation->source_operand_address_method == DIRECT_REGISTER) {
				is_valid = encode_registers(transition, p_decoded_operation->source_operand, NULL, output_files->ob_file);
			} else if (p_decoded_operation->source_operand_address_method == IMMEDIATE) {
				is_valid = encode_immediate(transition, p_decoded_operation->source_operand, output_files->ob_file);
			}

			transition->IC++;
		}

		/* Encode the target operand */
		if (p_decoded_operation->target_operand_address_method == DIRECT) {
			is_valid = encode_direct(transition, p_decoded_operation->target_operand, output_files);
		} else if (p_decoded_operation->target_operand_address_method == DIRECT_REGISTER) {
			is_valid = encode_registers(transition, NULL, p_decoded_operation->target_operand, output_files->ob_file);
		} else if (p_decoded_operation->target_operand_address_method == IMMEDIATE) {
			is_valid = encode_immediate(transition, p_decoded_operation->target_operand, output_files->ob_file);
		}

		transition->IC++;
	}

	return is_valid;
}

/*
 * Description: Encodes a direct operand
 * Input:		1. Current transition
 * 				2. Direct operand
 * 				3. Output files
 * Output:		Was operand encoded successfully
 */
bool encode_direct(transition_data* transition, char* operand, compiler_output_files* output_files) {
	/* This a variable. Therefore it must be a symbol */
	symbol_node_ptr p_symbol = search_symbol(operand);

	if (p_symbol == NULL) {
		print_compiler_error("Using unknown symbol", transition->current_line_information);
		transition->is_compiler_error = true;
		return false;
	} else {
		operand_memory_word word;

		word.non_register_address.operand_address = p_symbol->current_symbol.address;

		/* The symbol is external. Encode the location it is used into the ext file */
		if (p_symbol->current_symbol.is_external) {
			word.non_register_address.era = EXTERNAL;

			create_extern_output_file_if_needed(output_files, transition->current_line_information->file_name);

			if (output_files->extern_file == NULL) {
				transition->is_runtimer_error  = true;
				return false;
			}

			write_extern_to_output_file(operand, transition->IC + ADDRESS_START, output_files->extern_file);
		} else {
			word.non_register_address.era = RELOCATABLE;
		}

		word.non_register_address.rest = 0;

		print_encoding_to_file(transition->IC + ADDRESS_START, word.value, output_files->ob_file);

		return true;
	}
}

/*
 * Description: Encodes register operands
 * Input:		1. Current transition
 * 				2. Source register
 * 				3. Target register
 * 				4. Output file
 * Output:		Were operands encoded successfully
 */
bool encode_registers(transition_data* transition, char* source_register, char* target_register, FILE* p_file) {
	operand_memory_word word;

	/* The target register exists */
	if (source_register != NULL) {
		/* Encode the register's number */
		word.register_address.source_register_address = atoi(source_register + 1);
	} else {
		word.register_address.source_register_address = NO_ADDRESS;
	}

	/* The target register exists */
	if (target_register != NULL) {
		/* Encode the register's number */
		word.register_address.target_register_address = atoi(target_register + 1);
	} else {
		word.register_address.target_register_address = NO_ADDRESS;
	}

	word.register_address.era = ABSOLUTE;
	word.register_address.rest = 0;

	print_encoding_to_file(transition->IC + ADDRESS_START, word.value, p_file);

	return true;
}

/*
 * Description: Encodes a immediate number
 * Input:		1. Current transition
 * 				2. Immediate operand
 * 				3. Output file
 * Output:		Was operand encoded successfully
 */
bool encode_immediate(transition_data* transition, char* operand, FILE* p_file) {
	int number;
	operand_memory_word word;

	/* Convert the number into a integer */
	number = atoi(operand + 1);

	word.non_register_address.operand_address = number;
	word.non_register_address.era = ABSOLUTE;
	word.non_register_address.rest = 0;

	print_encoding_to_file(transition->IC + ADDRESS_START, word.value, p_file);

	return true;
}

/*
 * Description: Writes encoding into output file
 * Input:		1. Address in base 10
 * 				2. Memory word value in base 10
 * 				3. Output file
 */
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

	free(base4_value);

	fputc(END_OF_LINE, p_file);
}


/*
 * Description: Searches for an operation name in a pre-defined list of valid operations
 * Input:		Operation name
 * Output:		If found returns machine operation information, otherwise NULL
 */
machine_operation_definition* search_machine_operation_in_list(char* operation) {
	operation_information_node_ptr p_current = p_operation_head;

	/* Scan the operation list and search for the operation */
	while (p_current != NULL) {
		if (strcmp(p_current->data.name, operation) == 0) {
			return &(p_current->data);
		}

		p_current = p_current->next;
	}

	return NULL;
}

/*
 * Description: Initializes the operation list used for encoding
 * Output:		Did the initialization succeeded
 */
bool init_operation_list() {
	bool initialized = true;

	int op_code = 0;

	/* Initializes the operations */
	initialized &= add_operation_to_list(MOV_OPERATION, op_code++, TWO_OPERANDS);
	initialized &= add_operation_to_list(CMP_OPERATION, op_code++, TWO_OPERANDS);
	initialized &= add_operation_to_list(ADD_OPERATION, op_code++, TWO_OPERANDS);
	initialized &= add_operation_to_list(SUB_OPERATION, op_code++, TWO_OPERANDS);
	initialized &= add_operation_to_list(NOT_OPERATION, op_code++, ONE_OPERAND);
	initialized &= add_operation_to_list(CLR_OPERATION, op_code++, ONE_OPERAND);
	initialized &= add_operation_to_list(LEA_OPERATION, op_code++, TWO_OPERANDS);
	initialized &= add_operation_to_list(INC_OPERATION, op_code++, ONE_OPERAND);
	initialized &= add_operation_to_list(DEC_OPERATION, op_code++, ONE_OPERAND);
	initialized &= add_operation_to_list(JMP_OPERATION, op_code++, ONE_OPERAND);
	initialized &= add_operation_to_list(BNE_OPERATION, op_code++, ONE_OPERAND);
	initialized &= add_operation_to_list(RED_OPERATION, op_code++, ONE_OPERAND);
	initialized &= add_operation_to_list(PRN_OPERATION, op_code++, ONE_OPERAND);
	initialized &= add_operation_to_list(JSR_OPERATION, op_code++, ONE_OPERAND);
	initialized &= add_operation_to_list(RTS_OPERATION, op_code++, NO_OPERANDS);
	initialized &= add_operation_to_list(STOP_OPERATION, op_code++, NO_OPERANDS);

	return initialized;
}

/*
 * Description: Adds an operation into the list
 * Input:		1. Operation name
 * 				2. Op code
 * 				3. Number of operands
 * Output:		Did add successfully
 */
bool add_operation_to_list(char* name, unsigned int code, int operands) {
	bool added = false;

	operation_information_node_ptr p_new =
			(operation_information_node_ptr)allocate_memory(sizeof(operation_information_node));

	if (p_new != NULL) {
		p_new->data.name = name;
		p_new->data.code = code;
		p_new->data.operands_number = operands;
		p_new->next = NULL;

		/* first node input */
		if (p_operation_head == NULL) {
			p_operation_head = p_new;
		} else {
			p_new->next = p_operation_head;
			p_operation_head = p_new;
		}

		added = true;
	}

	return added;
}

/*
 * Description: Free memory for operations list
 */
void free_operation_list() {
	operation_information_node_ptr p_cleaner_data = p_operation_head;

	/* Clean nodes until no more nodes */
	while (p_operation_head) {
		p_cleaner_data = p_operation_head;
		p_operation_head = p_operation_head->next;
		free (p_cleaner_data);
	}
	return;
}

/*
 * Description: Reads the next operand from the line
 * Input:		Current transition
 * Output:		Operand
 */
char* get_next_operand(transition_data* transition) {
	char* operand = NULL;
	int i, operand_end_index, operand_start_index, operand_length;

	skip_all_spaces(transition->current_line_information);

	operand_end_index  = operand_start_index = i = transition->current_line_information->current_index;

	/* Find operand position */
	for (;i < transition->current_line_information->line_length; i++) {
		if (!isspace(transition->current_line_information->line_str[i]) &&
				(transition->current_line_information->line_str[i] != OPERAND_SEPERATOR)) {
			operand_end_index = i;
		}
		else
		{
			break;
		}
	}

	operand_length = operand_end_index - operand_start_index + 1;

	operand = allocate_string(operand_length);

	if (operand == NULL) {
		transition->is_runtimer_error = true;
	} else {
		/* Copy operand */
		strncpy(operand, transition->current_line_information->line_str + operand_start_index, operand_length);
		operand[operand_length] = '\0';

		transition->current_line_information->current_index = operand_end_index + 1;
	}

	return operand;
}
