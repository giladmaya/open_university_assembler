/*
 * FirstTransition.h
 *
 *  Created on: Jul 14, 2015
 *      Author: student
 */

#ifndef FIRSTTRANSITION_H_
#define FIRSTTRANSITION_H_

#include "Types.h"
#include <stdio.h>
#include <stdbool.h>

bool first_transition_execute(FILE* pFile, char* file_name, unsigned int* IC, unsigned int* DC);

void first_transition_process_line(transition_data* transition);

#endif /* FIRSTTRANSITION_H_ */
