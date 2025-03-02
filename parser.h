#ifndef _parser_h_
#define _parser_h_

#include <stddef.h>
#include <stdint.h>

#include "sds.h"
#include "lexer.h"

#define N_INSTRUCTIONS 1024
#define LABEL_NEXT (N_INSTRUCTIONS + MAX_LABELS)

struct instruction {
	uint32_t opcode;
	int32_t condition[9];
	sds annotation;
};

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

#define X_WR_m_REG_m_1_p_C (ARG(ADD_WR)|ARG(SUB_REG)|ARG(SUB_1)|ARG(ADD_C))
#define X_REG_m_WR_m_1_p_C (ARG(ADD_REG)|ARG(SUB_WR)|ARG(SUB_1)|ARG(ADD_C))
#define X_REG_p_WR_p_C     (ARG(ADD_REG)|ARG(ADD_WR)|ARG(ADD_C))
#define X_WR_p_C           (ARG(ADD_WR)|ARG(ADD_C))
#define X_NWR_p_C          (ARG(ADD_NWR)|ARG(ADD_C))
#define X_REG_p_C          (ARG(ADD_REG)|ARG(ADD_C))
#define X_NREG_p_C         (ARG(ADD_NREG)|ARG(ADD_C))
#define X_NC               (ARG(ADD_NC))

#define X_WR_m_REG_m_1_p_1 (ARG(ADD_WR)|ARG(SUB_REG)|ARG(SUB_1)|ARG(ADD_1))
#define X_REG_m_WR_m_1_p_1 (ARG(ADD_REG)|ARG(SUB_WR)|ARG(SUB_1)|ARG(ADD_1))
#define X_REG_p_WR_p_1     (ARG(ADD_REG)|ARG(ADD_WR)|ARG(ADD_1))
#define X_WR_p_1           (ARG(ADD_WR)|ARG(ADD_1))
#define X_NWR_p_1          (ARG(ADD_NWR)|ARG(ADD_1))
#define X_REG_p_1          (ARG(ADD_REG)|ARG(ADD_1))
#define X_NREG_p_1         (ARG(ADD_NREG)|ARG(ADD_1))

#define X_WR_m_DIP_m_1_p_C (ARG(ADD_WR)|ARG(SUB_DIP)|ARG(SUB_1)|ARG(ADD_C))
#define X_DIP_m_WR_m_1_p_C (ARG(ADD_DIP)|ARG(SUB_WR)|ARG(SUB_1)|ARG(ADD_C))
#define X_DIP_p_WR_p_C     (ARG(ADD_DIP)|ARG(ADD_WR)|ARG(ADD_C))
#define X_DIP_p_C          (ARG(ADD_DIP)|ARG(ADD_C))
#define X_NDIP_p_C         (ARG(ADD_NDIP)|ARG(ADD_C))

#define X_XWR_m_DIP_m_1_p_C (ARG(ADD_XWR)|ARG(SUB_DIP)|ARG(SUB_1)|ARG(ADD_C))
#define X_DIP_m_XWR_m_1_p_C (ARG(ADD_DIP)|ARG(SUB_XWR)|ARG(SUB_1)|ARG(ADD_C))
#define X_DIP_p_XWR_p_C     (ARG(ADD_DIP)|ARG(ADD_XWR)|ARG(ADD_C))
#define X_XWR_p_C           (ARG(ADD_XWR)|ARG(ADD_C))
#define X_NXWR_p_C          (ARG(ADD_NXWR)|ARG(ADD_C))

#define X_DIP               (ARG(ADD_DIP))
#define X_REG               (ARG(ADD_REG))
#define X_REG_p_XWR_p_C     (ARG(ADD_REG)|ARG(ADD_XWR)|ARG(ADD_C))
#define X_REG_p_DIP_p_C     (ARG(ADD_REG)|ARG(ADD_DIP)|ARG(ADD_C))

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
	struct instruction program[N_INSTRUCTIONS];
	int32_t labels[N_INSTRUCTIONS];
	struct lexer *lexer;

	sds prev_token;
	sds token;
	sds next_token;

	int32_t prev_input;
	int32_t input;
	int32_t next_input;

	int32_t n_labels;
	int32_t address;
	int32_t error;
	uint32_t arg_add;
	uint8_t var;
	uint8_t reg;
	uint8_t arg1, arg2;
	uint8_t op;
	uint8_t brk, carry, carry_val, invalid;
};

int parser_init(struct parser *parser, struct lexer *lexer);
int parser_run(struct parser *parser);
int parser_output(struct parser *parser, char *u8path);

#endif
