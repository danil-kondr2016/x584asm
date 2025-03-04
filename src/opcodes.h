#define ATTR_CUSED 0x2000
#define ATTR_CARRY 0x4000

struct opcode_data {
	uint32_t arg_add;
	uint8_t var;
	uint8_t reg;
	uint8_t arg1, arg2;
	uint8_t op;
	uint8_t carry;
	uint32_t opcode;
};

#define ARG_ADD_R0 ARG_ADD_REG
#define ARG_ADD_R1 ARG_ADD_REG
#define ARG_ADD_R2 ARG_ADD_REG
#define ARG_ADD_R3 ARG_ADD_REG
#define ARG_ADD_R4 ARG_ADD_REG
#define ARG_ADD_R5 ARG_ADD_REG
#define ARG_ADD_R6 ARG_ADD_REG
#define ARG_ADD_R7 ARG_ADD_REG

#define ARG_SUB_R0 ARG_SUB_REG
#define ARG_SUB_R1 ARG_SUB_REG
#define ARG_SUB_R2 ARG_SUB_REG
#define ARG_SUB_R3 ARG_SUB_REG
#define ARG_SUB_R4 ARG_SUB_REG
#define ARG_SUB_R5 ARG_SUB_REG
#define ARG_SUB_R6 ARG_SUB_REG
#define ARG_SUB_R7 ARG_SUB_REG

#define ARG_ADD_NR0 ARG_ADD_NREG
#define ARG_ADD_NR1 ARG_ADD_NREG
#define ARG_ADD_NR2 ARG_ADD_NREG
#define ARG_ADD_NR3 ARG_ADD_NREG
#define ARG_ADD_NR4 ARG_ADD_NREG
#define ARG_ADD_NR5 ARG_ADD_NREG
#define ARG_ADD_NR6 ARG_ADD_NREG
#define ARG_ADD_NR7 ARG_ADD_NREG

#define ARG_SUB_NR0 ARG_SUB_NREG
#define ARG_SUB_NR1 ARG_SUB_NREG
#define ARG_SUB_NR2 ARG_SUB_NREG
#define ARG_SUB_NR3 ARG_SUB_NREG
#define ARG_SUB_NR4 ARG_SUB_NREG
#define ARG_SUB_NR5 ARG_SUB_NREG
#define ARG_SUB_NR6 ARG_SUB_NREG
#define ARG_SUB_NR7 ARG_SUB_NREG

#define ALU_REG(_x) ALU_REG_##_x
#define ALU_REG_R0 REG_R0
#define ALU_REG_R1 REG_R1
#define ALU_REG_R2 REG_R2
#define ALU_REG_R3 REG_R3
#define ALU_REG_R4 REG_R4
#define ALU_REG_R5 REG_R5
#define ALU_REG_R6 REG_R6
#define ALU_REG_R7 REG_R7
#define ALU_REG_WR REG_NONE
#define ALU_REG_XWR REG_NONE
#define ALU_REG_DIP REG_NONE

#define ALU(_var, _a, _b, _x) \
	{ARG(ADD_NC), VAR_##_var, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, 0000|(_x)}, \
	\
	{ARG(ADD_##_b)|ARG(SUB_##_a)|ARG(SUB_1)|ARG(ADD_C), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, 0040|(_x)},\
	{ARG(ADD_##_b)|ARG(SUB_##_a)|ARG(SUB_1), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, 0040|(_x)},\
	{ARG(ADD_##_b)|ARG(SUB_##_a)|ARG(SUB_1)|ARG(ADD_1), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, 0040|(_x)},\
	{ARG(ADD_##_b)|ARG(SUB_##_a), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, 0040|(_x)},\
	\
	{ARG(ADD_##_a)|ARG(SUB_##_b)|ARG(SUB_1)|ARG(ADD_C), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, 0100|(_x)},\
	{ARG(ADD_##_a)|ARG(SUB_##_b)|ARG(SUB_1), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, 0100|(_x)},\
	{ARG(ADD_##_a)|ARG(SUB_##_b), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, 0100|(_x)},\
	{ARG(ADD_##_a)|ARG(SUB_##_b)|ARG(SUB_1)|ARG(ADD_1), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, 0100|(_x)},\
	\
	{ARG(ADD_##_a)|ARG(ADD_##_b)|ARG(ADD_C), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, 0140|(_x)},\
	{ARG(ADD_##_a)|ARG(ADD_##_b), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, 0140|(_x)},\
	{ARG(ADD_##_a)|ARG(ADD_##_b)|ARG(ADD_1), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, 0140|(_x)},\
	\
	{ARG(ADD_##_b)|ARG(ADD_C), VAR_##_var, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, 0200|(_x)},\
	{ARG(ADD_##_b), VAR_##_var, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, 0200|(_x)},\
	{ARG(ADD_##_b)|ARG(ADD_1), VAR_##_var, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, 0200|(_x)},\
	\
	{ARG(ADD_N##_b)|ARG(ADD_C), VAR_##_var, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, 0240|(_x)},\
	{ARG(ADD_N##_b), VAR_##_var, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, 0240|(_x)},\
	{ARG(ADD_N##_b)|ARG(ADD_1), VAR_##_var, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, 0240|(_x)},\
	\
	{ARG(ADD_##_a)|ARG(ADD_C), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, 0300|(_x)},\
	{ARG(ADD_##_a), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, 0300|(_x)},\
	{ARG(ADD_##_a)|ARG(ADD_1), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, 0300|(_x)},\
	\
	{ARG(ADD_N##_a)|ARG(ADD_C), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, 0340|(_x)},\
	{ARG(ADD_N##_a), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, 0340|(_x)},\
	{ARG(ADD_N##_a)|ARG(ADD_1), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, 0340|(_x)},\
	\
	{0, VAR_##_var, 0, REG_##_a, REG_##_b, OP_AND, CARRY_UNUSED, 0400|(_x)},\
	{0, VAR_##_var, 0, REG_##_a, REG_##_b, OP_XOR, CARRY_UNUSED, 0440|(_x)},\
	{0, VAR_##_var, 0, REG_##_a, REG_##_b, OP_NXOR, CARRY_UNUSED, 0500|(_x)},\
	{0, VAR_##_var, 0, REG_N##_a, REG_##_b, OP_AND, CARRY_UNUSED, 0540|(_x)},\
	{0, VAR_##_var, 0, REG_##_a, REG_N##_b, OP_AND, CARRY_UNUSED, 0600|(_x)},\
	{0, VAR_##_var, 0, REG_##_a, REG_N##_b, OP_OR, CARRY_UNUSED, 0640|(_x)},\
	{0, VAR_##_var, 0, REG_N##_a, REG_##_b, OP_OR, CARRY_UNUSED, 0700|(_x)},\
	{0, VAR_##_var, 0, REG_##_a, REG_##_b, OP_OR, CARRY_UNUSED, 0740|(_x)}

#define SUM2_REG1(_var, _b, _op) \
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_C), VAR_##_var, REG_R0, 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, (_op)|0000},\
	{ARG(ADD_REG)|ARG(ADD_##_b), VAR_##_var, REG_R0, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, (_op)|0000},\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_1), VAR_##_var, REG_R0, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, (_op)|0000},\
	\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_C), VAR_##_var, REG_R1, 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, (_op)|0001},\
	{ARG(ADD_REG)|ARG(ADD_##_b), VAR_##_var, REG_R1, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, (_op)|0001},\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_1), VAR_##_var, REG_R1, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, (_op)|0001},\
	\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_C), VAR_##_var, REG_R2, 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, (_op)|0002},\
	{ARG(ADD_REG)|ARG(ADD_##_b), VAR_##_var, REG_R2, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, (_op)|0002},\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_1), VAR_##_var, REG_R2, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, (_op)|0002},\
	\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_C), VAR_##_var, REG_R3, 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, (_op)|0003},\
	{ARG(ADD_REG)|ARG(ADD_##_b), VAR_##_var, REG_R3, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, (_op)|0003},\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_1), VAR_##_var, REG_R3, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, (_op)|0003},\
	\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_C), VAR_##_var, REG_R4, 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, (_op)|0004},\
	{ARG(ADD_REG)|ARG(ADD_##_b), VAR_##_var, REG_R4, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, (_op)|0004},\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_1), VAR_##_var, REG_R4, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, (_op)|0004},\
	\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_C), VAR_##_var, REG_R5, 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, (_op)|0005},\
	{ARG(ADD_REG)|ARG(ADD_##_b), VAR_##_var, REG_R5, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, (_op)|0005},\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_1), VAR_##_var, REG_R5, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, (_op)|0005},\
	\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_C), VAR_##_var, REG_R6, 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, (_op)|0006},\
	{ARG(ADD_REG)|ARG(ADD_##_b), VAR_##_var, REG_R6, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, (_op)|0006},\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_1), VAR_##_var, REG_R6, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, (_op)|0006},\
	\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_C), VAR_##_var, REG_R7, 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, (_op)|0007},\
	{ARG(ADD_REG)|ARG(ADD_##_b), VAR_##_var, REG_R7, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, (_op)|0007},\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_1), VAR_##_var, REG_R7, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, (_op)|0007}

#define SUM2(_var, _a, _b, _op) \
	{ARG(ADD_##_a)|ARG(ADD_##_b)|ARG(ADD_C), VAR_##_var, REG_##_a, 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, _op},\
	{ARG(ADD_##_a)|ARG(ADD_##_b), VAR_##_var, REG_##_a, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, _op},\
	{ARG(ADD_##_a)|ARG(ADD_##_b)|ARG(ADD_1), VAR_##_var, REG_##_a, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, _op}

#define SUM1(_var, _a, _op) \
	{ARG(ADD_##_a)|ARG(ADD_C), VAR_##_var, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, _op}

#define SUM2_VARREG_REG1(_b, _op) \
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_C), VAR_R0, REG_R0, 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, (_op)|0000},\
	{ARG(ADD_REG)|ARG(ADD_##_b), VAR_R0, REG_R0, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, (_op)|0000},\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_1), VAR_R0, REG_R0, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, (_op)|0000},\
	\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_C), VAR_R1, REG_R1, 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, (_op)|0001},\
	{ARG(ADD_REG)|ARG(ADD_##_b), VAR_R1, REG_R1, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, (_op)|0001},\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_1), VAR_R1, REG_R1, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, (_op)|0001},\
	\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_C), VAR_R2, REG_R2, 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, (_op)|0002},\
	{ARG(ADD_REG)|ARG(ADD_##_b), VAR_R2, REG_R2, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, (_op)|0002},\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_1), VAR_R2, REG_R2, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, (_op)|0002},\
	\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_C), VAR_R3, REG_R3, 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, (_op)|0003},\
	{ARG(ADD_REG)|ARG(ADD_##_b), VAR_R2, REG_R2, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, (_op)|0002},\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_1), VAR_R2, REG_R2, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, (_op)|0002},\
	\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_C), VAR_R4, REG_R4, 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, (_op)|0004},\
	{ARG(ADD_REG)|ARG(ADD_##_b), VAR_R2, REG_R2, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, (_op)|0002},\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_1), VAR_R2, REG_R2, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, (_op)|0002},\
	\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_C), VAR_R5, REG_R5, 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, (_op)|0005},\
	{ARG(ADD_REG)|ARG(ADD_##_b), VAR_R2, REG_R2, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, (_op)|0002},\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_1), VAR_R2, REG_R2, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, (_op)|0002},\
	\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_C), VAR_R6, REG_R6, 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, (_op)|0006},\
	{ARG(ADD_REG)|ARG(ADD_##_b), VAR_R2, REG_R2, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, (_op)|0002},\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_1), VAR_R2, REG_R2, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, (_op)|0002},\
	\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_C), VAR_R7, REG_R7, 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, (_op)|0007},\
	{ARG(ADD_REG)|ARG(ADD_##_b), VAR_R7, REG_R7, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, (_op)|0007},\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_1), VAR_R7, REG_R7, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, (_op)|0007}

#define SUM1_VARREG(_b, _op) \
	{ARG(ADD_##_b)|ARG(ADD_C), VAR_R0, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, (_op)|0000},\
	{ARG(ADD_##_b), VAR_R6, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, (_op)|0006},\
	{ARG(ADD_##_b)|ARG(ADD_1), VAR_R6, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, (_op)|0006},\
	\
	{ARG(ADD_##_b)|ARG(ADD_C), VAR_R1, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, (_op)|0001},\
	{ARG(ADD_##_b), VAR_R6, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, (_op)|0006},\
	{ARG(ADD_##_b)|ARG(ADD_1), VAR_R6, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, (_op)|0006},\
	\
	{ARG(ADD_##_b)|ARG(ADD_C), VAR_R2, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, (_op)|0002},\
	{ARG(ADD_##_b), VAR_R6, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, (_op)|0006},\
	{ARG(ADD_##_b)|ARG(ADD_1), VAR_R6, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, (_op)|0006},\
	\
	{ARG(ADD_##_b)|ARG(ADD_C), VAR_R3, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, (_op)|0003},\
	{ARG(ADD_##_b), VAR_R6, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, (_op)|0006},\
	{ARG(ADD_##_b)|ARG(ADD_1), VAR_R6, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, (_op)|0006},\
	\
	{ARG(ADD_##_b)|ARG(ADD_C), VAR_R4, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, (_op)|0004},\
	{ARG(ADD_##_b), VAR_R6, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, (_op)|0006},\
	{ARG(ADD_##_b)|ARG(ADD_1), VAR_R6, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, (_op)|0006},\
	\
	{ARG(ADD_##_b)|ARG(ADD_C), VAR_R5, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, (_op)|0005},\
	{ARG(ADD_##_b), VAR_R6, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, (_op)|0006},\
	{ARG(ADD_##_b)|ARG(ADD_1), VAR_R6, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, (_op)|0006},\
	\
	{ARG(ADD_##_b)|ARG(ADD_C), VAR_R6, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, (_op)|0006},\
	{ARG(ADD_##_b), VAR_R6, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, (_op)|0006},\
	{ARG(ADD_##_b)|ARG(ADD_1), VAR_R6, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, (_op)|0006},\
	\
	{ARG(ADD_##_b)|ARG(ADD_C), VAR_R7, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, (_op)|0007},\
	{ARG(ADD_##_b), VAR_R7, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, (_op)|0007},\
	{ARG(ADD_##_b)|ARG(ADD_1), VAR_R7, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, (_op)|0007}

#define SET_REG(_b, _op) \
	{ARG(ADD_##_b), VAR_R0, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_UNUSED, (_op)|0000},\
	{ARG(ADD_##_b), VAR_R1, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_UNUSED, (_op)|0001},\
	{ARG(ADD_##_b), VAR_R2, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_UNUSED, (_op)|0002},\
	{ARG(ADD_##_b), VAR_R3, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_UNUSED, (_op)|0003},\
	{ARG(ADD_##_b), VAR_R4, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_UNUSED, (_op)|0004},\
	{ARG(ADD_##_b), VAR_R5, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_UNUSED, (_op)|0005},\
	{ARG(ADD_##_b), VAR_R6, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_UNUSED, (_op)|0006},\
	{ARG(ADD_##_b), VAR_R7, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_UNUSED, (_op)|0007}

#define SET_TO_REG(_a, _op) \
	{ARG(ADD_REG), VAR_##_a, REG_R0, 0, 0, OP_ADD_SUB_NEG, CARRY_UNUSED, (_op)|0000},\
	{ARG(ADD_REG), VAR_##_a, REG_R1, 0, 0, OP_ADD_SUB_NEG, CARRY_UNUSED, (_op)|0001},\
	{ARG(ADD_REG), VAR_##_a, REG_R2, 0, 0, OP_ADD_SUB_NEG, CARRY_UNUSED, (_op)|0002},\
	{ARG(ADD_REG), VAR_##_a, REG_R3, 0, 0, OP_ADD_SUB_NEG, CARRY_UNUSED, (_op)|0003},\
	{ARG(ADD_REG), VAR_##_a, REG_R4, 0, 0, OP_ADD_SUB_NEG, CARRY_UNUSED, (_op)|0004},\
	{ARG(ADD_REG), VAR_##_a, REG_R5, 0, 0, OP_ADD_SUB_NEG, CARRY_UNUSED, (_op)|0005},\
	{ARG(ADD_REG), VAR_##_a, REG_R6, 0, 0, OP_ADD_SUB_NEG, CARRY_UNUSED, (_op)|0006},\
	{ARG(ADD_REG), VAR_##_a, REG_R7, 0, 0, OP_ADD_SUB_NEG, CARRY_UNUSED, (_op)|0007}

#define SET(_var, _a, _op) \
	{ARG(ADD_##_a), VAR_##_var, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_UNUSED, _op}

#define SHIFT(_op, _opcode) \
	{ARG(ADD_WR) | ARG(ADD_C), VAR_WR, 0, 0, 0, OP_##_op, CARRY_INDEFINITE, _opcode},\
	{ARG(ADD_WR), VAR_WR, 0, 0, 0, OP_##_op, CARRY_VALUE_0, _opcode},\
	{ARG(ADD_WR) | ARG(ADD_1), VAR_WR, 0, 0, 0, OP_##_op, CARRY_VALUE_1, _opcode}

#define SHIFT1_X(_op, _opcode) \
	{ARG(ADD_WR) | ARG(ADD_C), VAR_WR_XWR, 0, 0, 0, OP_##_op##_X, CARRY_INDEFINITE, _opcode},\
	{ARG(ADD_WR), VAR_WR_XWR, 0, 0, 0, OP_##_op##_X, CARRY_VALUE_0, _opcode},\
	{ARG(ADD_WR) | ARG(ADD_1), VAR_WR_XWR, 0, 0, 0, OP_##_op##_X, CARRY_VALUE_1, _opcode}

#define SHIFT_ARG_X(_arg, _reg, _op, _opcode) \
	{(_arg) | ARG(ADD_C), VAR_WR_XWR, REG_##_reg, 0, 0, OP_##_op##_X, CARRY_INDEFINITE, _opcode},\
	{(_arg), VAR_WR_XWR, REG_##_reg, 0, 0, OP_##_op##_X, CARRY_VALUE_0, _opcode},\
	{(_arg) | ARG(ADD_1), VAR_WR_XWR, REG_##_reg, 0, 0, OP_##_op##_X, CARRY_VALUE_1, _opcode}

#define OPCODE_WR_m_DIP_m_1_ROL 0432
#define OPCODE_WR_p_DIP_ROL     0472
#define OPCODE_WR_m_RF_m_1_ROL  0420
#define OPCODE_WR_p_RF_ROL      0460

#define OPCODE_WR_m_DIP_m_1_SAR 0132
#define OPCODE_WR_p_DIP_SAR     0572
#define OPCODE_WR_m_RF_m_1_SAR  0120
#define OPCODE_WR_p_RF_SAR      0560

#define SHIFT2_X(_op) \
	SHIFT_ARG_X(ARG(ADD_WR)|ARG(SUB_DIP)|ARG(SUB_1), NONE, _op, OPCODE_WR_m_DIP_m_1_##_op),\
	SHIFT_ARG_X(ARG(ADD_WR)|ARG(ADD_DIP), NONE, _op, OPCODE_WR_p_DIP_##_op),\
	\
	SHIFT_ARG_X(ARG(ADD_WR)|ARG(SUB_REG)|ARG(SUB_1), R0, _op, OPCODE_WR_m_RF_m_1_##_op|0),\
	SHIFT_ARG_X(ARG(ADD_WR)|ARG(SUB_REG)|ARG(SUB_1), R1, _op, OPCODE_WR_m_RF_m_1_##_op|1),\
	SHIFT_ARG_X(ARG(ADD_WR)|ARG(SUB_REG)|ARG(SUB_1), R2, _op, OPCODE_WR_m_RF_m_1_##_op|2),\
	SHIFT_ARG_X(ARG(ADD_WR)|ARG(SUB_REG)|ARG(SUB_1), R3, _op, OPCODE_WR_m_RF_m_1_##_op|3),\
	SHIFT_ARG_X(ARG(ADD_WR)|ARG(SUB_REG)|ARG(SUB_1), R4, _op, OPCODE_WR_m_RF_m_1_##_op|4),\
	SHIFT_ARG_X(ARG(ADD_WR)|ARG(SUB_REG)|ARG(SUB_1), R5, _op, OPCODE_WR_m_RF_m_1_##_op|5),\
	SHIFT_ARG_X(ARG(ADD_WR)|ARG(SUB_REG)|ARG(SUB_1), R6, _op, OPCODE_WR_m_RF_m_1_##_op|6),\
	SHIFT_ARG_X(ARG(ADD_WR)|ARG(SUB_REG)|ARG(SUB_1), R7, _op, OPCODE_WR_m_RF_m_1_##_op|7),\
	\
	SHIFT_ARG_X(ARG(ADD_WR)|ARG(ADD_REG), R0, _op, OPCODE_WR_p_RF_##_op|0),\
	SHIFT_ARG_X(ARG(ADD_WR)|ARG(ADD_REG), R1, _op, OPCODE_WR_p_RF_##_op|1),\
	SHIFT_ARG_X(ARG(ADD_WR)|ARG(ADD_REG), R2, _op, OPCODE_WR_p_RF_##_op|2),\
	SHIFT_ARG_X(ARG(ADD_WR)|ARG(ADD_REG), R3, _op, OPCODE_WR_p_RF_##_op|3),\
	SHIFT_ARG_X(ARG(ADD_WR)|ARG(ADD_REG), R4, _op, OPCODE_WR_p_RF_##_op|4),\
	SHIFT_ARG_X(ARG(ADD_WR)|ARG(ADD_REG), R5, _op, OPCODE_WR_p_RF_##_op|5),\
	SHIFT_ARG_X(ARG(ADD_WR)|ARG(ADD_REG), R6, _op, OPCODE_WR_p_RF_##_op|6),\
	SHIFT_ARG_X(ARG(ADD_WR)|ARG(ADD_REG), R7, _op, OPCODE_WR_p_RF_##_op|7)


static const struct opcode_data _opcodes[] = {
	SUM2_REG1(XWR, WR, 0160),
	SUM2_REG1(WR, DIP, 0220),
	SUM2_REG1(XWR, DIP, 0260),
	SUM2_VARREG_REG1(DIP, 0360),
	SUM2_REG1(WR, XWR, 0620),
	SUM2_REG1(XWR, XWR, 0660),
	SUM1_VARREG(XWR, 0720),
	SUM2(XWR, WR, DIP, 0172),
	SUM2(DOP, WR, DIP, 0372),
	SUM2(WR, XWR, DIP, 0632),
	SUM2(XWR, XWR, DIP, 0672),
	SUM1(DOP, XWR, 0732),
	SET_REG(DIP, 0760),
	SET_TO_REG(DOP, 0020),
	SET_TO_REG(XWR, 0060),
	SET(WR, DIP, 0320),
	SET(XWR, DIP, 0072),
	SET(DOP, DIP, 0772),

	ALU(R0, R0, WR, 0),
	ALU(R1, R1, WR, 1),
	ALU(R2, R2, WR, 2),
	ALU(R3, R3, WR, 3),
	ALU(R4, R4, WR, 4),
	ALU(R5, R5, WR, 5),
	ALU(R6, R6, WR, 6),
	ALU(R7, R7, WR, 7),
	ALU(WR, R0, WR, 0010),
	ALU(WR, R1, WR, 0011),
	ALU(WR, R2, WR, 0012),
	ALU(WR, R3, WR, 0013),
	ALU(WR, R4, WR, 0014),
	ALU(WR, R5, WR, 0015),
	ALU(WR, R6, WR, 0016),
	ALU(WR, R7, WR, 0017),
	ALU(DOP, DIP, WR, 0030),
	ALU(WR, DIP, WR, 0031),
	ALU(WR, DIP, XWR, 0033),
	ALU(XWR, DIP, WR, 0034),
	ALU(XWR, DIP, XWR, 0036),
	ALU(DOP, DIP, XWR, 0037),

	SHIFT(SAR, 0035),
	SHIFT(ROR, 0075),
	SHIFT(ROR, 0475),
	SHIFT(SAL, 0135),
	SHIFT(ROL, 0175),
	SHIFT(ROL, 0575),
	SHIFT(SHR, 0435),
	SHIFT(SHL, 0535),

	SHIFT1_X(SAR, 0235),
	SHIFT1_X(ROR, 0275),
	SHIFT1_X(ROR, 0675),
	SHIFT1_X(SAL, 0335),
	SHIFT1_X(ROL, 0375),
	SHIFT1_X(ROL, 0775),
	SHIFT1_X(SHR, 0635),
	SHIFT1_X(SHL, 0735),

	SHIFT2_X(ROL),
	SHIFT2_X(SAR),

	{0, 0, 0, 0, 0, OP_NOP, CARRY_UNUSED, NOP},
	{0, 0, 0, 0, 0, OP_HALT, CARRY_UNUSED, NOP},
	{0, 0, 0, 0, 0, 0, 0, 0},
};

