/*
 * SymbolTable.c
 *
 *  Created on: Jul 17, 2015
 *      Author: student
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
	if (p_symbol_head == NULL) {
		p_symbol_head = p_new_symbol;
		p_symbol_tail = p_new_symbol;
	} else {
		p_symbol_tail->next = p_new_symbol;
		p_symbol_tail = p_new_symbol;
	}
}

symbol_node_ptr create_symbol(char* name, unsigned int address, bool is_extern, bool is_instruction) {
	symbol_node_ptr p_node = (symbol_node_ptr)allocate_memory(sizeof(symbol_node));

	if (p_node != NULL) {
		p_node->current_symbol.name = name;
		p_node->current_symbol.is_external = is_extern;
		p_node->current_symbol.is_instruction = is_instruction;
		p_node->current_symbol.address = address;
		p_node->next = NULL;
	}

	return p_node;
}

symbol_node_ptr search_symbol(char* symbol_name) {
	symbol_node_ptr p_current = p_symbol_head;

	while ((p_current != NULL) && (strcmp(p_current->current_symbol.name, symbol_name) != 0)) {
		p_current = p_current->next;
	}

	return p_current;
}

void update_symbol_address(int ic_length) {
	symbol_node_ptr p_current = p_symbol_head;

	while (p_current != NULL) {
		if (!p_current->current_symbol.is_external) {
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
 * Description: Free list memory
 */
void free_symbol_list() {
	symbol_node_ptr p_cleaner_data = p_symbol_head;

	while (p_symbol_head) {

		p_cleaner_data = p_symbol_head;

		/* Move next */
		p_symbol_head = p_symbol_head->next;

		free (p_cleaner_data);
	}

	return;
}
