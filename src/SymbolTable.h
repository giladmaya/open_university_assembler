/*
 * SymbolTable.h
 *
 *  Created on: Jul 17, 2015
 *      Author: student
 */

#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

#include "Types.h"
#include <stdbool.h>

void add_symbol_to_list(symbol_node_ptr p_new_symbol);
symbol_node_ptr create_symbol(char* name, int address, bool is_extern, bool is_instruction);

#endif /* SYMBOLTABLE_H_ */