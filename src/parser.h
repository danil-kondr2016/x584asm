#ifndef _parser_h_
#define _parser_h_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <sds.h>
#include "lexer.h"
#include "program.h"

enum variable {
	VAR_NONE,
	VAR_R0,
	VAR_R1,
	VAR_R2,
	VAR_R3,
	VAR_R4,
	VAR_R5,
	VAR_R6,
	VAR_R7,
	VAR_WR,
	VAR_XWR,
	VAR_DOP,
	VAR_WR_XWR,
};

enum reg {
	REG_NONE,
	REG_R0,
	REG_R1,
	REG_R2,
	REG_R3,
	REG_R4,
	REG_R5,
	REG_R6,
	REG_R7,
	REG_WR,
	REG_XWR,
	REG_DIP,
	REG_C,
	REG_NR0,
	REG_NR1,
	REG_NR2,
	REG_NR3,
	REG_NR4,
	REG_NR5,
	REG_NR6,
	REG_NR7,
	REG_NWR,
	REG_NXWR,
	REG_NDIP,
	REG_NC,
	REG_1,
	REG_0,
	REG_INVALID,
};

enum argument {
	ARG_ADD_REG,
	ARG_ADD_WR,
	ARG_ADD_XWR,
	ARG_ADD_DIP,
	ARG_ADD_C,
	ARG_ADD_1,
	ARG_ADD_NREG,
	ARG_ADD_NWR,
	ARG_ADD_NXWR,
	ARG_ADD_NDIP,
	ARG_ADD_NC,
	ARG_SUB_REG,
	ARG_SUB_WR,
	ARG_SUB_XWR,
	ARG_SUB_DIP,
	ARG_SUB_C,
	ARG_SUB_1,
	ARG_SUB_NREG,
	ARG_SUB_NWR,
	ARG_SUB_NXWR,
	ARG_SUB_NDIP,
	ARG_SUB_NC,
	ARG_0,
};

#define FLAG(x) (1 << (x))
#define ARG(x) (FLAG(ARG_##x))

#define X_WR_p_C           (ARG(ADD_WR)|ARG(ADD_C))
#define X_WR_p_1           (ARG(ADD_WR)|ARG(ADD_1))

enum op {
	OP_NONE,
	OP_ADD_SUB_NEG,
	OP_AND,
	OP_XOR,
	OP_NXOR,
	OP_OR,
	OP_SHL, OP_SHR,
	OP_SAL, OP_SAR,
	OP_ROL, OP_ROR,
	OP_SHL_X, OP_SHR_X,
	OP_SAL_X, OP_SAR_X,
	OP_ROL_X, OP_ROR_X,
	OP_NOP,
	OP_HALT,
};

struct parser {
	struct program *program;
	struct lexer *lexer;

	sds prev_token;
	sds token;
	sds next_token;

	int32_t prev_input;
	int32_t input;
	int32_t next_input;

	int prev_line, line, next_line;
	int prev_col, col, next_col;

	int32_t n_labels;
	int32_t address;
	int32_t error;
	uint32_t arg_add;
	uint8_t var;
	uint8_t reg;
	uint8_t arg1, arg2;
	uint8_t op;
	uint8_t carry;
	bool brk                 : 1;
	bool invalid_instruction : 1;
	bool is_program_valid    : 1;
};

bool parser_init(struct parser *parser, struct lexer *lexer, struct program *program);
bool parser_run(struct parser *parser);

#endif
