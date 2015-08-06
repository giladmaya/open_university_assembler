/*
 * SymbolTable.c
 *
 *  Created on: Jul 17, 2015
 *      Author: student
 */

#include "Types.h"
#include "Consts.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

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
	symbol_node_ptr p_node = (symbol_node_ptr)malloc(sizeof(symbol_node));

	if (p_node == NULL) {
		/* TODO: bad alloc */
	} else {
		p_node->data.name = name;
		p_node->data.is_external = is_extern;
		p_node->data.is_instruction = is_instruction;
		p_node->data.address = address + ADDRESS_START;
	}

	return p_node;
}

symbol_node_ptr search_symbol(char* symbol_name) {
	symbol_node_ptr p_current = p_symbol_head;

	while ((p_current != NULL) && (strcmp(p_current->data.name, symbol_name) != 0)) {
		p_current = p_current->next;
	}

	return p_current;
}

void calculate_final_data_address(int ic_length) {
	symbol_node_ptr p_current = p_symbol_head;

	while (p_current != NULL) {
		if ((p_current->data.is_instruction) && (!p_current->data.is_external)) {
			p_current->data.address += ic_length;
		}

		p_current = p_current->next;
	}
}
