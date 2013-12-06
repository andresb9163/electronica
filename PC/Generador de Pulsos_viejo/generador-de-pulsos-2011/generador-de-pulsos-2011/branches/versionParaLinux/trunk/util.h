#ifndef _UTIL_H_
#define _UTIL_H_

#include "instruction_sheet.h"
#include "instruction.h"
#include "instruction_list.h"

instruction_list get_loops_insts(instruction initial_inst, instruction final_inst, instruction_sheet is);

void instruction_sheet_replace(instruction_sheet is, instruction initial, instruction final, instruction_list replacement);

instruction_list repeat_insts(instruction_list list, unsigned int n_times);

void convert_loops(instruction_sheet inst_sheet);

#endif
