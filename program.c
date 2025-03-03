#include "program.h"

#include <stdlib.h>
#include <string.h>

#include "lexer.h"

int program_init(struct program *program)
{
	if (!program)
		return 0;

	memset(program, 0, sizeof(struct program));

	for (int i = 0; i < MAX_LABELS; i++) {
		program->label[i] = LABEL_INVALID;
	}

	for (int i = 0; i < N_INSTRUCTIONS; i++) {
		program->opcode[i] = NOP;
	}

	return 1;
}

int program_set_label(struct program *program, int label, int address)
{
	if (!program)
		return 0;
	if (label < 0 || label >= MAX_LABELS)
		return 0;
	if (address < 0 || address >= N_INSTRUCTIONS)
		return 0;

	program->label[label] = address;
	return 1;
}

int program_set_opcode(struct program *program, int address, int opcode, int brk, int carry)
{
	unsigned flags = 0;

	if (!program)
		return 0;
	if (address < 0 || address >= N_INSTRUCTIONS)
		return 0;

	if (brk)
		flags |= ATTR_BREAK;
	switch (carry) {
	case CARRY_UNUSED:
		break;
	case CARRY_VALUE_1:
		flags |= ATTR_CARRY;
	case CARRY_INDEFINITE:
	case CARRY_VALUE_0:
		flags |= ATTR_CUSED;
		break;
	}

	program->opcode[address] = (opcode & 0x1FF) | flags;
	return 1;
}

int program_set_if(struct program *program, int address, int32_t flag, int32_t a_then, int32_t a_else)
{
	int32_t tmp = 0;

	if (!program)
		return 0;
	if (address < 0 || address >= N_INSTRUCTIONS)
		return 0;
	if (a_then < 0 || a_then > LABEL_NEXT)
		return 0;
	if (a_else < 0 || a_else > LABEL_NEXT)
		return 0;

	switch (flag) {
	case KW_SHL1: case KW_SHL2: case KW_SHR1: case KW_SHR2:
	case ~KW_CO3: case ~KW_CO2: case ~KW_CO1: case ~KW_CO0:
	case ~KW_XWR0: case ~KW_XWR3: case ~KW_A15: case ~KW_B15:
		flag = ~flag;
		tmp = a_then;
		a_then = a_else;
		a_else = tmp;
	case ~KW_SHL1: case ~KW_SHL2: case ~KW_SHR1: case ~KW_SHR2:
	case KW_CO3: case KW_CO2: case KW_CO1: case KW_CO0:
	case KW_XWR0: case KW_XWR3: case KW_A15: case KW_B15:
		break;
	default:
		return 0;
	}

	program->control[address].type = KW_IF;
	program->control[address].If.flag = flag;
	program->control[address].If.a_then = a_then;
	program->control[address].If.a_else = a_else;
	return 1;
}

int program_set_goto(struct program *program, int address, int32_t a_goto)
{
	if (!program)
		return 0;
	if (address < 0 || address >= N_INSTRUCTIONS)
		return 0;
	if (a_goto < 0 || a_goto > LABEL_NEXT)
		return 0;

	program->control[address].type = KW_GOTO;
	program->control[address].Goto.a_goto = a_goto;
	return 0;
}

int program_set_input(struct program *program, int address, int value)
{
	if (!program)
		return 0;
	if (address < 0 || address >= N_INSTRUCTIONS)
		return 0;
	if (value < -32768 || value > 65535)
		return 0;
	if (value < 0)
		value &= 0xFFFF;

	program->control[address].type = KW_INPUT;
	program->control[address].Input.value = value;
	return 1;
}

int program_set_annotation(struct program *program, int address, char *annotation)
{
	if (!program)
		return 0;
	if (address < 0 || address >= N_INSTRUCTIONS)
		return 0;

	if (program->annotation[address]) {
		sdsfree(program->annotation[address]);
		program->annotation[address] = NULL;
	}

	if (annotation && *annotation) {
		program->annotation[address] = sdsnew(annotation);
	}

	return 1;
}

int program_move_annotation(struct program *program, int address, sds annotation)
{
	if (!program)
		return 0;
	if (address < 0 || address >= N_INSTRUCTIONS)
		return 0;

	if (program->annotation[address]) {
		sdsfree(program->annotation[address]);
		program->annotation[address] = NULL;
	}

	if (annotation && *annotation) {
		program->annotation[address] = annotation;
	}

	return 1;
}

