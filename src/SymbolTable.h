/*
 ====================================================================================
 Name		: 	SymbolTable.h

 Author's	: 	Maya Gilad 302526850, Idan Levi 301242434

 Description: 	This file holds methods relevant to working with the symbol table
 ====================================================================================
 */

#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

#include "Types.h"
#include <stdbool.h>

/*
 * Description: Adds a new symbol to the list
 * Input:		The new symbol
 */
void add_symbol_to_list(symbol_node_ptr p_new_symbol);

/*
 * Description: Creates a new symbol node
 * Input:		1. Symbol name
 * 				2. IC address
 * 				3. Is external
 * 				4. Is instruction
 * Output: 		New symbol node
 */
symbol_node_ptr create_symbol(char* name, int address, bool is_extern, bool is_instruction);

/*
 * Description: Search input symbol in symbol table
 * Input:		Symbol name
 * Output:		Current symbol node
 */
symbol_node_ptr search_symbol(char* symbol_name);

/*
 * Description: Updates symbol's address after first transition ends
 * Input:		IC value
 */
void update_symbol_address(int ic_length);

/*
 * Description: Free symbol list from memory
 */
void free_symbol_list();

#endif /* SYMBOLTABLE_H_ */
