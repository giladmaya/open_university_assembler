/*
 ====================================================================================
 Name		: 	SymbolTable.c

 Author's	: 	Maya Gilad 302526850, Idan Levi 301242434

 Description: 	This file holds methods relevant to working with the symbol table
 ====================================================================================
 */

#include "Types.h"
#include "Consts.h"
#include "Utilities.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/* SymbolTable head and tail */
symbol_node_ptr p_symbol_head = NULL;
symbol_node_ptr p_symbol_tail = NULL;

/*
 * Description: Adds a new symbol to the list
 * Input:		The new symbol
 */
void add_symbol_to_list(symbol_node_ptr p_new_symbol) {
	/* List is empty */
	if (p_symbol_head == NULL) {
		p_symbol_head = p_new_symbol;
		p_symbol_tail = p_new_symbol;
	} else {
		p_symbol_tail->next = p_new_symbol;
		p_symbol_tail = p_new_symbol;
	}
}

/*
 * Description: Creates a new symbol node
 * Input:		1. Symbol name
 * 				2. IC address
 * 				3. Is external
 * 				4. Is instruction
 * Output: 		New symbol node
 */
symbol_node_ptr create_symbol(char* name, unsigned int address, bool is_extern, bool is_instruction) {
	symbol_node_ptr p_node = (symbol_node_ptr)allocate_memory(sizeof(symbol_node));

	/* Copy data into node */
	if (p_node != NULL) {
		p_node->current_symbol.name = name;
		p_node->current_symbol.is_external = is_extern;
		p_node->current_symbol.is_instruction = is_instruction;
		p_node->current_symbol.address = address;
		p_node->next = NULL;
	}

	return p_node;
}

/*
 * Description: Search input symbol in symbol table
 * Input:		Symbol name
 * Output:		Current symbol node
 */
symbol_node_ptr search_symbol(char* symbol_name) {
	symbol_node_ptr p_current = p_symbol_head;

	/* Scan symbol table */
	while ((p_current != NULL) && (strcmp(p_current->current_symbol.name, symbol_name) != 0)) {
		p_current = p_current->next;
	}

	return p_current;
}

/*
 * Description: Updates symbol's address after first transition ends
 * Input:		IC value
 */
void update_symbol_address(int ic_length) {
	symbol_node_ptr p_current = p_symbol_head;

	while (p_current != NULL) {
		/* The symbol isn't external */
		if (!p_current->current_symbol.is_external) {
			/* The symbol is an instruction (.data or .string) */
			if (p_current->current_symbol.is_instruction) {
				p_current->current_symbol.address += ic_length + ADDRESS_START;
			} else {
				p_current->current_symbol.address += ADDRESS_START;
			}
		}

		p_current = p_current->next;
	}
}

/*
 * Description: Free symbol list from memory
 */
void free_symbol_list() {
	symbol_node_ptr p_cleaner_data = p_symbol_head;

	/* Clean nodes until no more nodes */
	while (p_symbol_head) {
		p_cleaner_data = p_symbol_head;
		p_symbol_head = p_symbol_head->next;
		free (p_cleaner_data);
	}
	return;
}
