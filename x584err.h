#ifndef _x584err_h_
#define _x584err_h_

#include <stdint.h>

#define INPUT_ERROR (-4)

enum error {
	X584ASM_FATAL_OUT_OF_MEMORY = 1,
	X584ASM_PREMATURE_END_OF_LINE,
	X584ASM_INVALID_ESCAPE_SEQUENCE,
	X584ASM_NO_LABELS_SPECIFIED,
	X584ASM_LABEL_ALREADY_DEFINED,
	X584ASM_COLON_EXPECTED,
	X584ASM_COMMA_EXPECTED,
	X584ASM_COMMA_OR_SEMICOLON_EXPECTED,
	X584ASM_FLAG_EXPECTED,
	X584ASM_INVALID_ADDRESS,
	X584ASM_INVALID_CARRY_VAL,
	X584ASM_INVALID_NUMBER,
	X584ASM_INVALID_OPCODE,
	X584ASM_LABEL_EXPECTED,
	X584ASM_LABEL_OR_ADDRESS_EXPECTED,
	X584ASM_LPAR_EXPECTED,
	X584ASM_OP_EXPECTED,
	X584ASM_OUT_OF_MEMORY,
	X584ASM_PREMATURE_EOF,
	X584ASM_RPAR_EXPECTED,
	X584ASM_SEMICOLON_EXPECTED,
	X584ASM_SHIFT_OP_EXPECTED,
	X584ASM_SYNTAX_ERROR,
	X584ASM_TERM_EXPECTED,
	X584ASM_THEN_EXPECTED,
	X584ASM_TOO_MANY_INSTRUCTIONS,
	X584ASM_TOO_MANY_LABELS,
	X584ASM_UNDEFINED_LABEL,
	X584ASM_WORD_EXPECTED,
	X584ASM_XOR_EXPECTED,
	X584ASM_UNEXPECTED_NUMBER,
	X584ASM_UNEXPECTED_WORD,
	X584ASM_UNEXPECTED_SYMBOL,

	X584ASM_ERROR_MAX
};

int Error(int line, int col, int error);
int Die(int error);

#endif
