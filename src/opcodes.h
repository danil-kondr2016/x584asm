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
	{ARG(ADD_NC), VAR_##_var, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, 0000|(_x)},\
	{ARG(ADD_0),  VAR_##_var, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, 0000|(_x)},\
	\
	{ARG(ADD_##_b)|ARG(SUB_##_a)|ARG(SUB_1)|ARG(ADD_C), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, 0040|(_x)},\
	{ARG(ADD_##_b)|ARG(SUB_##_a)|ARG(SUB_1)|ARG(ADD_0), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, 0040|(_x)},\
	{ARG(ADD_##_b)|ARG(SUB_##_a)|ARG(SUB_1)|ARG(ADD_1), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, 0040|(_x)},\
	{ARG(ADD_##_b)|ARG(SUB_##_a)|ARG(SUB_1), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, 0040|(_x)},\
	{ARG(ADD_##_b)|ARG(SUB_##_a), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, 0040|(_x)},\
	\
	{ARG(ADD_##_a)|ARG(SUB_##_b)|ARG(SUB_1)|ARG(ADD_C), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, 0100|(_x)},\
	{ARG(ADD_##_a)|ARG(SUB_##_b)|ARG(SUB_1)|ARG(ADD_0), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, 0100|(_x)},\
	{ARG(ADD_##_a)|ARG(SUB_##_b)|ARG(SUB_1)|ARG(ADD_1), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, 0100|(_x)},\
	{ARG(ADD_##_a)|ARG(SUB_##_b)|ARG(SUB_1), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, 0100|(_x)},\
	{ARG(ADD_##_a)|ARG(SUB_##_b), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, 0100|(_x)},\
	\
	{ARG(ADD_##_a)|ARG(ADD_##_b)|ARG(ADD_C), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, 0140|(_x)},\
	{ARG(ADD_##_a)|ARG(ADD_##_b)|ARG(ADD_0), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, 0140|(_x)},\
	{ARG(ADD_##_a)|ARG(ADD_##_b)|ARG(ADD_1), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, 0140|(_x)},\
	{ARG(ADD_##_a)|ARG(ADD_##_b), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, 0140|(_x)},\
	\
	{ARG(ADD_##_b)|ARG(ADD_C), VAR_##_var, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, 0200|(_x)},\
	{ARG(ADD_##_b)|ARG(ADD_0), VAR_##_var, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, 0200|(_x)},\
	{ARG(ADD_##_b)|ARG(ADD_1), VAR_##_var, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, 0200|(_x)},\
	{ARG(ADD_##_b), VAR_##_var, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, 0200|(_x)},\
	\
	{ARG(ADD_N##_b)|ARG(ADD_C), VAR_##_var, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, 0240|(_x)},\
	{ARG(ADD_N##_b)|ARG(ADD_0), VAR_##_var, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, 0240|(_x)},\
	{ARG(ADD_N##_b)|ARG(ADD_1), VAR_##_var, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, 0240|(_x)},\
	{ARG(ADD_N##_b), VAR_##_var, 0, 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, 0240|(_x)},\
	\
	{ARG(ADD_##_a)|ARG(ADD_C), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, 0300|(_x)},\
	{ARG(ADD_##_a)|ARG(ADD_0), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, 0300|(_x)},\
	{ARG(ADD_##_a)|ARG(ADD_1), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, 0300|(_x)},\
	{ARG(ADD_##_a), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, 0300|(_x)},\
	\
	{ARG(ADD_N##_a)|ARG(ADD_C), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, 0340|(_x)},\
	{ARG(ADD_N##_a)|ARG(ADD_0), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, 0340|(_x)},\
	{ARG(ADD_N##_a)|ARG(ADD_1), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, 0340|(_x)},\
	{ARG(ADD_N##_a), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, 0340|(_x)},\
	\
	{0, VAR_##_var, 0, REG_##_a, REG_##_b, OP_AND, CARRY_UNUSED, 0400|(_x)},\
	{0, VAR_##_var, 0, REG_##_b, REG_##_a, OP_AND, CARRY_UNUSED, 0400|(_x)},\
	\
	{0, VAR_##_var, 0, REG_##_a, REG_##_b, OP_XOR, CARRY_UNUSED, 0440|(_x)},\
	{0, VAR_##_var, 0, REG_##_b, REG_##_a, OP_XOR, CARRY_UNUSED, 0440|(_x)},\
	\
	{0, VAR_##_var, 0, REG_##_a, REG_##_b, OP_NXOR, CARRY_UNUSED, 0500|(_x)},\
	{0, VAR_##_var, 0, REG_##_b, REG_##_a, OP_NXOR, CARRY_UNUSED, 0500|(_x)},\
	\
	{0, VAR_##_var, 0, REG_N##_a, REG_##_b, OP_AND, CARRY_UNUSED, 0540|(_x)},\
	{0, VAR_##_var, 0, REG_##_b, REG_N##_a, OP_AND, CARRY_UNUSED, 0540|(_x)},\
	\
	{0, VAR_##_var, 0, REG_##_a, REG_N##_b, OP_AND, CARRY_UNUSED, 0600|(_x)},\
	{0, VAR_##_var, 0, REG_N##_b, REG_##_a, OP_AND, CARRY_UNUSED, 0600|(_x)},\
	\
	{0, VAR_##_var, 0, REG_##_a, REG_N##_b, OP_OR, CARRY_UNUSED, 0640|(_x)},\
	{0, VAR_##_var, 0, REG_N##_b, REG_##_a, OP_OR, CARRY_UNUSED, 0640|(_x)},\
	\
	{0, VAR_##_var, 0, REG_N##_a, REG_##_b, OP_OR, CARRY_UNUSED, 0700|(_x)},\
	{0, VAR_##_var, 0, REG_##_b, REG_N##_a, OP_OR, CARRY_UNUSED, 0700|(_x)},\
	\
	{0, VAR_##_var, 0, REG_##_a, REG_##_b, OP_OR, CARRY_UNUSED, 0740|(_x)},\
	{0, VAR_##_var, 0, REG_##_b, REG_##_a, OP_OR, CARRY_UNUSED, 0740|(_x)}

#define SUM2(_var, _a, _b, _op) \
	{ARG(ADD_##_a)|ARG(ADD_##_b)|ARG(ADD_C), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, _op},\
	{ARG(ADD_##_a)|ARG(ADD_##_b)|ARG(ADD_0), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, _op},\
	{ARG(ADD_##_a)|ARG(ADD_##_b)|ARG(ADD_1), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, _op},\
	{ARG(ADD_##_a)|ARG(ADD_##_b), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, _op}

#define SUM1(_var, _a, _op) \
	{ARG(ADD_##_a)|ARG(ADD_C), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_INDEFINITE, _op},\
	{ARG(ADD_##_a)|ARG(ADD_0), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, _op},\
	{ARG(ADD_##_a)|ARG(ADD_1), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_1, _op},\
	{ARG(ADD_##_a), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_VALUE_0, _op}

#define SET(_var, _a, _op) \
	{ARG(ADD_##_a), VAR_##_var, ALU_REG(_a), 0, 0, OP_ADD_SUB_NEG, CARRY_UNUSED, _op}

#define SHIFT(_op, _opcode) \
	{ARG(ADD_WR) | ARG(ADD_C), VAR_WR, 0, 0, 0, OP_##_op, CARRY_INDEFINITE, _opcode},\
	{ARG(ADD_WR) | ARG(ADD_0), VAR_WR, 0, 0, 0, OP_##_op, CARRY_VALUE_0, _opcode},\
	{ARG(ADD_WR) | ARG(ADD_1), VAR_WR, 0, 0, 0, OP_##_op, CARRY_VALUE_1, _opcode},\
	{ARG(ADD_WR), VAR_WR, 0, 0, 0, OP_##_op, CARRY_VALUE_0, _opcode}

#define SHIFT1_X(_op, _opcode) \
	{ARG(ADD_WR) | ARG(ADD_C), VAR_WR_XWR, 0, 0, 0, OP_##_op##_X, CARRY_INDEFINITE, _opcode},\
	{ARG(ADD_WR) | ARG(ADD_0), VAR_WR_XWR, 0, 0, 0, OP_##_op##_X, CARRY_VALUE_0, _opcode},\
	{ARG(ADD_WR) | ARG(ADD_1), VAR_WR_XWR, 0, 0, 0, OP_##_op##_X, CARRY_VALUE_1, _opcode},\
	{ARG(ADD_WR), VAR_WR_XWR, 0, 0, 0, OP_##_op##_X, CARRY_VALUE_0, _opcode}

#define SHIFT_ARG_X(_arg, _reg, _op, _opcode) \
	{(_arg) | ARG(ADD_C), VAR_WR_XWR, REG_##_reg, 0, 0, OP_##_op##_X, CARRY_INDEFINITE, _opcode},\
	{(_arg) | ARG(ADD_0), VAR_WR_XWR, REG_##_reg, 0, 0, OP_##_op##_X, CARRY_VALUE_0, _opcode},\
	{(_arg) | ARG(ADD_1), VAR_WR_XWR, REG_##_reg, 0, 0, OP_##_op##_X, CARRY_VALUE_1, _opcode},\
	{(_arg), VAR_WR_XWR, REG_##_reg, 0, 0, OP_##_op##_X, CARRY_VALUE_0, _opcode}

#define SHIFT_ARG_SUB_X(_a, _r, _op, _opcode) \
	{ARG(ADD_WR)|ARG(SUB_##_a)|ARG(SUB_1)|ARG(ADD_C), VAR_WR_XWR, REG_##_r, 0, 0, OP_##_op##_X, CARRY_INDEFINITE, _opcode},\
	{ARG(ADD_WR)|ARG(SUB_##_a)|ARG(SUB_1)|ARG(ADD_0), VAR_WR_XWR, REG_##_r, 0, 0, OP_##_op##_X, CARRY_VALUE_0, _opcode},\
	{ARG(ADD_WR)|ARG(SUB_##_a)|ARG(SUB_1)|ARG(ADD_1), VAR_WR_XWR, REG_##_r, 0, 0, OP_##_op##_X, CARRY_VALUE_1, _opcode},\
	{ARG(ADD_WR)|ARG(SUB_##_a)|ARG(SUB_1), VAR_WR_XWR, REG_##_r, 0, 0, OP_##_op##_X, CARRY_VALUE_0, _opcode},\
	{ARG(ADD_WR)|ARG(SUB_##_a), VAR_WR_XWR, REG_##_r, 0, 0, OP_##_op##_X, CARRY_VALUE_1, _opcode}

#define OPCODE_WR_m_DIP_m_1_ROL 0432
#define OPCODE_WR_p_DIP_ROL     0472
#define OPCODE_WR_m_RF_m_1_ROL  0420
#define OPCODE_WR_p_RF_ROL      0460

#define OPCODE_WR_m_DIP_m_1_SAR 0132
#define OPCODE_WR_p_DIP_SAR     0572
#define OPCODE_WR_m_RF_m_1_SAR  0120
#define OPCODE_WR_p_RF_SAR      0560

#define SHIFT2_X(_op) \
	SHIFT_ARG_SUB_X(DIP, NONE, _op, OPCODE_WR_m_DIP_m_1_##_op),\
	SHIFT_ARG_X(ARG(ADD_WR)|ARG(ADD_DIP), NONE, _op, OPCODE_WR_p_DIP_##_op),\
	\
	SHIFT_ARG_SUB_X(R0, R0, _op, OPCODE_WR_m_RF_m_1_##_op|0),\
	SHIFT_ARG_SUB_X(R1, R1, _op, OPCODE_WR_m_RF_m_1_##_op|1),\
	SHIFT_ARG_SUB_X(R2, R2, _op, OPCODE_WR_m_RF_m_1_##_op|2),\
	SHIFT_ARG_SUB_X(R3, R3, _op, OPCODE_WR_m_RF_m_1_##_op|3),\
	SHIFT_ARG_SUB_X(R4, R4, _op, OPCODE_WR_m_RF_m_1_##_op|4),\
	SHIFT_ARG_SUB_X(R5, R5, _op, OPCODE_WR_m_RF_m_1_##_op|5),\
	SHIFT_ARG_SUB_X(R6, R6, _op, OPCODE_WR_m_RF_m_1_##_op|6),\
	SHIFT_ARG_SUB_X(R7, R7, _op, OPCODE_WR_m_RF_m_1_##_op|7),\
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
	SET(R0,  DIP, 0760),
	SET(R1,  DIP, 0761),
	SET(R2,  DIP, 0762),
	SET(R3,  DIP, 0763),
	SET(R4,  DIP, 0764),
	SET(R5,  DIP, 0765),
	SET(R6,  DIP, 0766),
	SET(R7,  DIP, 0767),
	SET(DOP, R0,  0020),
	SET(DOP, R1,  0021),
	SET(DOP, R2,  0022),
	SET(DOP, R3,  0023),
	SET(DOP, R4,  0024),
	SET(DOP, R5,  0025),
	SET(DOP, R6,  0026),
	SET(DOP, R7,  0027),
	SET(XWR, R0,  0060),
	SET(XWR, R1,  0061),
	SET(XWR, R2,  0062),
	SET(XWR, R3,  0063),
	SET(XWR, R4,  0064),
	SET(XWR, R5,  0065),
	SET(XWR, R6,  0066),
	SET(XWR, R7,  0067),
	SET(WR,  DIP, 0320),
	SET(WR,  DIP, 0321),
	SET(WR,  DIP, 0322),
	SET(WR,  DIP, 0323),
	SET(WR,  DIP, 0324),
	SET(WR,  DIP, 0325),
	SET(WR,  DIP, 0326),
	SET(WR,  DIP, 0327),
	SET(WR,  DIP, 0332),
	SET(XWR, DIP, 0072),
	SET(DOP, DIP, 0772),

	SUM2(XWR, R0,  WR,  0160),
	SUM2(XWR, R1,  WR,  0161),
	SUM2(XWR, R2,  WR,  0162),
	SUM2(XWR, R3,  WR,  0163),
	SUM2(XWR, R4,  WR,  0164),
	SUM2(XWR, R5,  WR,  0165),
	SUM2(XWR, R6,  WR,  0166),
	SUM2(XWR, R7,  WR,  0167),
	SUM2(WR,  R0,  DIP, 0220),
	SUM2(WR,  R1,  DIP, 0221),
	SUM2(WR,  R2,  DIP, 0222),
	SUM2(WR,  R3,  DIP, 0223),
	SUM2(WR,  R4,  DIP, 0224),
	SUM2(WR,  R5,  DIP, 0225),
	SUM2(WR,  R6,  DIP, 0226),
	SUM2(WR,  R7,  DIP, 0227),
	SUM2(XWR, R0,  DIP, 0260),
	SUM2(XWR, R1,  DIP, 0261),
	SUM2(XWR, R2,  DIP, 0262),
	SUM2(XWR, R3,  DIP, 0263),
	SUM2(XWR, R4,  DIP, 0264),
	SUM2(XWR, R5,  DIP, 0265),
	SUM2(XWR, R6,  DIP, 0266),
	SUM2(XWR, R7,  DIP, 0267),
	SUM2(R0,  R0,  DIP, 0360),
	SUM2(R1,  R1,  DIP, 0361),
	SUM2(R2,  R2,  DIP, 0362),
	SUM2(R3,  R3,  DIP, 0363),
	SUM2(R4,  R4,  DIP, 0364),
	SUM2(R5,  R5,  DIP, 0365),
	SUM2(R6,  R6,  DIP, 0366),
	SUM2(R7,  R7,  DIP, 0367),
	SUM2(WR,  R0,  XWR, 0620),
	SUM2(WR,  R1,  XWR, 0621),
	SUM2(WR,  R2,  XWR, 0622),
	SUM2(WR,  R3,  XWR, 0623),
	SUM2(WR,  R4,  XWR, 0624),
	SUM2(WR,  R5,  XWR, 0625),
	SUM2(WR,  R6,  XWR, 0626),
	SUM2(WR,  R7,  XWR, 0627),
	SUM2(XWR, R0,  XWR, 0660),
	SUM2(XWR, R1,  XWR, 0661),
	SUM2(XWR, R2,  XWR, 0662),
	SUM2(XWR, R3,  XWR, 0663),
	SUM2(XWR, R4,  XWR, 0664),
	SUM2(XWR, R5,  XWR, 0665),
	SUM2(XWR, R6,  XWR, 0666),
	SUM2(XWR, R7,  XWR, 0667),
	SUM2(XWR, WR,  DIP, 0172),
	SUM2(DOP, WR,  DIP, 0372),
	SUM2(WR,  XWR, DIP, 0632),
	SUM2(XWR, XWR, DIP, 0672),

	SUM1(R0,  XWR, 0720),
	SUM1(R1,  XWR, 0721),
	SUM1(R2,  XWR, 0722),
	SUM1(R3,  XWR, 0723),
	SUM1(R4,  XWR, 0724),
	SUM1(R5,  XWR, 0725),
	SUM1(R6,  XWR, 0726),
	SUM1(R7,  XWR, 0727),
	SUM1(DOP, XWR, 0732),
	SUM1(DOP, XWR, 0032),

	ALU(R0,  R0, WR,   0000),
	ALU(R1,  R1, WR,   0001),
	ALU(R2,  R2, WR,   0002),
	ALU(R3,  R3, WR,   0003),
	ALU(R4,  R4, WR,   0004),
	ALU(R5,  R5, WR,   0005),
	ALU(R6,  R6, WR,   0006),
	ALU(R7,  R7, WR,   0007),
	ALU(WR,  R0, WR,   0010),
	ALU(WR,  R1, WR,   0011),
	ALU(WR,  R2, WR,   0012),
	ALU(WR,  R3, WR,   0013),
	ALU(WR,  R4, WR,   0014),
	ALU(WR,  R5, WR,   0015),
	ALU(WR,  R6, WR,   0016),
	ALU(WR,  R7, WR,   0017),
	ALU(DOP, DIP, WR,  0030),
	ALU(WR,  DIP, WR,  0031),
	ALU(WR,  DIP, XWR, 0033),
	ALU(XWR, DIP, WR,  0034),
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

	SHIFT1_X(SAR, 0520),
	SHIFT1_X(SAR, 0521),
	SHIFT1_X(SAR, 0522),
	SHIFT1_X(SAR, 0523),
	SHIFT1_X(SAR, 0524),
	SHIFT1_X(SAR, 0525),
	SHIFT1_X(SAR, 0526),
	SHIFT1_X(SAR, 0527),
	SHIFT1_X(SAR, 0532),
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

