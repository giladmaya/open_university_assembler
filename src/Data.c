/*
 * Data.c
 *
 *  Created on: Jul 17, 2015
 *      Author: student
 */

#include "Types.h"
#include <stdio.h>
#include <stdlib.h>

/* Global variables */
data_node_ptr p_data_head = NULL;
data_node_ptr p_data_tail = NULL;

/*
 * Description: Adds new data found in code to the list
 */
void add_data_node_to_list(data_node_ptr p_new_data) {
	if (p_data_head == NULL) {
		p_data_head = p_new_data;
		p_data_tail = p_new_data;
	} else {
		p_data_tail->next = p_new_data;
		p_data_tail = p_new_data;
	}
}

void add_string_data_to_list(char data, unsigned int address) {
	data_node_ptr p_data = (data_node_ptr)malloc(sizeof(data_node));

	if (p_data == NULL) {
		/* TODO: bad alloc */
	} else {
		p_data->value.value = data;
		p_data->value.is_number = false;
		p_data->value.address = address;
		p_data->next = NULL;

		add_data_node_to_list(p_data);
	}
}

void add_numeric_data_to_list(int number, unsigned int address) {
	data_node_ptr p_data = (data_node_ptr)malloc(sizeof(data_node));

	if (p_data == NULL) {
		/* TODO: bad alloc */
	} else {
		p_data->value.number = number;
		p_data->value.is_number = true;
		p_data->value.address = address;
		p_data->next = NULL;

		add_data_node_to_list(p_data);
	}
}
