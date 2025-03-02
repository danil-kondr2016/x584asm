#define ATTR_CUSED 0x2000
#define ATTR_CARRY 0x4000

struct opcode_data {
	uint32_t arg_add;
	uint8_t var;
	uint8_t reg;
	uint8_t arg1, arg2;
	uint8_t op;
	uint8_t carry, carry_val;
	uint32_t opcode;
};

#define ALU(_var, _a, _b, _x) \
	{ARG(ADD_NC), VAR_##_var, 0, 0, 0, OP_ADD_SUB_NEG, 0, 0, 0000|(_x)}, \
	{ARG(ADD_WR)|ARG(SUB_REG)|ARG(SUB_1)|ARG(ADD_C), VAR_##_var, REG_##_a, 0, 0, OP_ADD_SUB_NEG, 0, 0, 0040|(_x)},\
	{ARG(ADD_REG)|ARG(SUB_WR)|ARG(SUB_1)|ARG(ADD_C), VAR_##_var, REG_##_a, 0, 0, OP_ADD_SUB_NEG, 0, 0, 0100|(_x)},\
	{ARG(ADD_REG)|ARG(ADD_WR)|ARG(ADD_C), VAR_##_var, REG_##_a, 0, 0, OP_ADD_SUB_NEG, 0, 0, 0140|(_x)},\
	{ARG(ADD_WR)|ARG(ADD_C), VAR_##_var, 0, 0, 0, OP_ADD_SUB_NEG, 0, 0, 0200|(_x)},\
	{ARG(ADD_NWR)|ARG(ADD_C), VAR_##_var, 0, 0, 0, OP_ADD_SUB_NEG, 0, 0, 0240|(_x)},\
	{ARG(ADD_REG)|ARG(ADD_C), VAR_##_var, REG_##_a, 0, 0, OP_ADD_SUB_NEG, 0, 0, 0300|(_x)},\
	{ARG(ADD_NREG)|ARG(ADD_C), VAR_##_var, REG_##_a, 0, 0, OP_ADD_SUB_NEG, 0, 0, 0340|(_x)},\
	{0, VAR_##_var, 0, REG_##_a, REG_##_b, OP_AND, 0, 0, 0400|(_x)},\
	{0, VAR_##_var, 0, REG_##_a, REG_##_b, OP_XOR, 0, 0, 0440|(_x)},\
	{0, VAR_##_var, 0, REG_##_a, REG_##_b, OP_NXOR, 0, 0, 0500|(_x)},\
	{0, VAR_##_var, 0, REG_N##_a, REG_##_b, OP_AND, 0, 0, 0540|(_x)},\
	{0, VAR_##_var, 0, REG_##_a, REG_N##_b, OP_AND, 0, 0, 0600|(_x)},\
	{0, VAR_##_var, 0, REG_##_a, REG_N##_b, OP_OR, 0, 0, 0640|(_x)},\
	{0, VAR_##_var, 0, REG_N##_a, REG_N##_b, OP_OR, 0, 0, 0700|(_x)},\
	{0, VAR_##_var, 0, REG_##_a, REG_##_b, OP_OR, 0, 0, 0740|(_x)}

#define SUM2_REG1(_var, _b, _op) \
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_C), VAR_##_var, REG_R0, 0, 0, OP_ADD_SUB_NEG, 0, 0, (_op)|0000},\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_C), VAR_##_var, REG_R1, 0, 0, OP_ADD_SUB_NEG, 0, 0, (_op)|0001},\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_C), VAR_##_var, REG_R2, 0, 0, OP_ADD_SUB_NEG, 0, 0, (_op)|0002},\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_C), VAR_##_var, REG_R3, 0, 0, OP_ADD_SUB_NEG, 0, 0, (_op)|0003},\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_C), VAR_##_var, REG_R4, 0, 0, OP_ADD_SUB_NEG, 0, 0, (_op)|0004},\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_C), VAR_##_var, REG_R5, 0, 0, OP_ADD_SUB_NEG, 0, 0, (_op)|0005},\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_C), VAR_##_var, REG_R6, 0, 0, OP_ADD_SUB_NEG, 0, 0, (_op)|0006},\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_C), VAR_##_var, REG_R7, 0, 0, OP_ADD_SUB_NEG, 0, 0, (_op)|0007}

#define SUM2(_var, _a, _b, _op) \
	{ARG(ADD_##_a)|ARG(ADD_##_b)|ARG(ADD_C), VAR_##_var, REG_##_a, 0, 0, OP_ADD_SUB_NEG, 0, 0, _op}

#define SUM1(_var, _a, _op) \
	{ARG(ADD_##_a)|ARG(ADD_C), VAR_##_var, 0, 0, 0, OP_ADD_SUB_NEG, 0, 0, _op}

#define SUM2_VARREG_REG1(_b, _op) \
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_C), VAR_R0, REG_R0, 0, 0, OP_ADD_SUB_NEG, 0, 0, (_op)|0000},\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_C), VAR_R1, REG_R1, 0, 0, OP_ADD_SUB_NEG, 0, 0, (_op)|0001},\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_C), VAR_R2, REG_R2, 0, 0, OP_ADD_SUB_NEG, 0, 0, (_op)|0002},\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_C), VAR_R3, REG_R3, 0, 0, OP_ADD_SUB_NEG, 0, 0, (_op)|0003},\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_C), VAR_R4, REG_R4, 0, 0, OP_ADD_SUB_NEG, 0, 0, (_op)|0004},\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_C), VAR_R5, REG_R5, 0, 0, OP_ADD_SUB_NEG, 0, 0, (_op)|0005},\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_C), VAR_R6, REG_R6, 0, 0, OP_ADD_SUB_NEG, 0, 0, (_op)|0006},\
	{ARG(ADD_REG)|ARG(ADD_##_b)|ARG(ADD_C), VAR_R7, REG_R7, 0, 0, OP_ADD_SUB_NEG, 0, 0, (_op)|0007}

#define SUM1_VARREG(_b, _op) \
	{ARG(ADD_##_b)|ARG(ADD_C), VAR_R0, 0, 0, 0, OP_ADD_SUB_NEG, 0, 0, (_op)|0000},\
	{ARG(ADD_##_b)|ARG(ADD_C), VAR_R1, 0, 0, 0, OP_ADD_SUB_NEG, 0, 0, (_op)|0001},\
	{ARG(ADD_##_b)|ARG(ADD_C), VAR_R2, 0, 0, 0, OP_ADD_SUB_NEG, 0, 0, (_op)|0002},\
	{ARG(ADD_##_b)|ARG(ADD_C), VAR_R3, 0, 0, 0, OP_ADD_SUB_NEG, 0, 0, (_op)|0003},\
	{ARG(ADD_##_b)|ARG(ADD_C), VAR_R4, 0, 0, 0, OP_ADD_SUB_NEG, 0, 0, (_op)|0004},\
	{ARG(ADD_##_b)|ARG(ADD_C), VAR_R5, 0, 0, 0, OP_ADD_SUB_NEG, 0, 0, (_op)|0005},\
	{ARG(ADD_##_b)|ARG(ADD_C), VAR_R6, 0, 0, 0, OP_ADD_SUB_NEG, 0, 0, (_op)|0006},\
	{ARG(ADD_##_b)|ARG(ADD_C), VAR_R7, 0, 0, 0, OP_ADD_SUB_NEG, 0, 0, (_op)|0007}

#define SET_REG(_b, _op) \
	{ARG(ADD_##_b), VAR_R0, 0, 0, 0, OP_ADD_SUB_NEG, 0, 0, (_op)|0000},\
	{ARG(ADD_##_b), VAR_R1, 0, 0, 0, OP_ADD_SUB_NEG, 0, 0, (_op)|0001},\
	{ARG(ADD_##_b), VAR_R2, 0, 0, 0, OP_ADD_SUB_NEG, 0, 0, (_op)|0002},\
	{ARG(ADD_##_b), VAR_R3, 0, 0, 0, OP_ADD_SUB_NEG, 0, 0, (_op)|0003},\
	{ARG(ADD_##_b), VAR_R4, 0, 0, 0, OP_ADD_SUB_NEG, 0, 0, (_op)|0004},\
	{ARG(ADD_##_b), VAR_R5, 0, 0, 0, OP_ADD_SUB_NEG, 0, 0, (_op)|0005},\
	{ARG(ADD_##_b), VAR_R6, 0, 0, 0, OP_ADD_SUB_NEG, 0, 0, (_op)|0006},\
	{ARG(ADD_##_b), VAR_R7, 0, 0, 0, OP_ADD_SUB_NEG, 0, 0, (_op)|0007}

#define SET_TO_REG(_a, _op) \
	{ARG(ADD_REG), VAR_##_a, REG_R0, 0, 0, OP_ADD_SUB_NEG, 0, 0, (_op)|0000},\
	{ARG(ADD_REG), VAR_##_a, REG_R1, 0, 0, OP_ADD_SUB_NEG, 0, 0, (_op)|0001},\
	{ARG(ADD_REG), VAR_##_a, REG_R2, 0, 0, OP_ADD_SUB_NEG, 0, 0, (_op)|0002},\
	{ARG(ADD_REG), VAR_##_a, REG_R3, 0, 0, OP_ADD_SUB_NEG, 0, 0, (_op)|0003},\
	{ARG(ADD_REG), VAR_##_a, REG_R4, 0, 0, OP_ADD_SUB_NEG, 0, 0, (_op)|0004},\
	{ARG(ADD_REG), VAR_##_a, REG_R5, 0, 0, OP_ADD_SUB_NEG, 0, 0, (_op)|0005},\
	{ARG(ADD_REG), VAR_##_a, REG_R6, 0, 0, OP_ADD_SUB_NEG, 0, 0, (_op)|0006},\
	{ARG(ADD_REG), VAR_##_a, REG_R7, 0, 0, OP_ADD_SUB_NEG, 0, 0, (_op)|0007}

#define SET(_var, _a, _op) \
	{ARG(ADD_##_a), VAR_##_var, 0, 0, 0, OP_ADD_SUB_NEG, 0, 0, _op}

#define SHIFT(_op, _opcode) \
	{ARG(ADD_WR) | ARG(ADD_C), VAR_WR, 0, 0, 0, OP_##_op, 0, 0, _opcode}

#define SHIFT1_X(_op, _opcode) \
	{ARG(ADD_WR) | ARG(ADD_C), VAR_WR_XWR, 0, 0, 0, OP##_op##_X, 0, 0, _opcode}

static const struct opcode_data _opcodes[] = {
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
	SET_REG(XWR, 0060),
	SET(WR, DIP, 0320),
	SET(XWR, DIP, 0072),
	SET(DOP, DIP, 0772),

	{ARG(ADD_WR) | ARG(ADD_C), VAR_WR, 0, 0, 0, OP_SAR, 0, 0, 0035},
	{ARG(ADD_WR) | ARG(ADD_C), VAR_WR, 0, 0, 0, OP_ROR, 0, 0, 0075},
	{ARG(ADD_WR) | ARG(ADD_C), VAR_WR, 0, 0, 0, OP_ROR, 0, 0, 0475},
	{ARG(ADD_WR) | ARG(ADD_C), VAR_WR, 0, 0, 0, OP_SAL, 0, 0, 0135},
	{ARG(ADD_WR) | ARG(ADD_C), VAR_WR, 0, 0, 0, OP_ROL, 0, 0, 0175},
	{ARG(ADD_WR) | ARG(ADD_C), VAR_WR, 0, 0, 0, OP_ROL, 0, 0, 0575},
	{ARG(ADD_WR) | ARG(ADD_C), VAR_WR, 0, 0, 0, OP_SHR, 0, 0, 0435},
	{ARG(ADD_WR) | ARG(ADD_C), VAR_WR, 0, 0, 0, OP_SHL, 0, 0, 0535},

	{ARG(ADD_WR) | ARG(ADD_C), VAR_WR, 0, 0, 0, OP_SAR_X, 0, 0, 0235},
	{ARG(ADD_WR) | ARG(ADD_C), VAR_WR, 0, 0, 0, OP_ROR_X, 0, 0, 0275},
	{ARG(ADD_WR) | ARG(ADD_C), VAR_WR, 0, 0, 0, OP_ROR_X, 0, 0, 0675},
	{ARG(ADD_WR) | ARG(ADD_C), VAR_WR, 0, 0, 0, OP_SAL_X, 0, 0, 0335},
	{ARG(ADD_WR) | ARG(ADD_C), VAR_WR, 0, 0, 0, OP_ROL_X, 0, 0, 0375},
	{ARG(ADD_WR) | ARG(ADD_C), VAR_WR, 0, 0, 0, OP_ROL_X, 0, 0, 0775},
	{ARG(ADD_WR) | ARG(ADD_C), VAR_WR, 0, 0, 0, OP_SHR_X, 0, 0, 0635},
	{ARG(ADD_WR) | ARG(ADD_C), VAR_WR, 0, 0, 0, OP_SHL_X, 0, 0, 0735},

	{ARG(ADD_WR) | ARG(SUB_DIP) | ARG(SUB_1) | ARG(ADD_C), VAR_WR_XWR, 0, 0, 0, OP_ROL_X, 0, 0, 0432},
	{ARG(ADD_WR) | ARG(ADD_DIP) | ARG(ADD_C), VAR_WR_XWR, 0, 0, 0, OP_ROL_X, 0, 0, 0472},

	{ARG(ADD_WR) | ARG(SUB_REG) | ARG(SUB_1) | ARG(ADD_C), VAR_WR_XWR, REG_R0, 0, 0, OP_ROL_X, 0, 0, 0420},
	{ARG(ADD_WR) | ARG(SUB_REG) | ARG(SUB_1) | ARG(ADD_C), VAR_WR_XWR, REG_R1, 0, 0, OP_ROL_X, 0, 0, 0421},
	{ARG(ADD_WR) | ARG(SUB_REG) | ARG(SUB_1) | ARG(ADD_C), VAR_WR_XWR, REG_R2, 0, 0, OP_ROL_X, 0, 0, 0422},
	{ARG(ADD_WR) | ARG(SUB_REG) | ARG(SUB_1) | ARG(ADD_C), VAR_WR_XWR, REG_R3, 0, 0, OP_ROL_X, 0, 0, 0423},
	{ARG(ADD_WR) | ARG(SUB_REG) | ARG(SUB_1) | ARG(ADD_C), VAR_WR_XWR, REG_R4, 0, 0, OP_ROL_X, 0, 0, 0424},
	{ARG(ADD_WR) | ARG(SUB_REG) | ARG(SUB_1) | ARG(ADD_C), VAR_WR_XWR, REG_R5, 0, 0, OP_ROL_X, 0, 0, 0425},
	{ARG(ADD_WR) | ARG(SUB_REG) | ARG(SUB_1) | ARG(ADD_C), VAR_WR_XWR, REG_R6, 0, 0, OP_ROL_X, 0, 0, 0426},
	{ARG(ADD_WR) | ARG(SUB_REG) | ARG(SUB_1) | ARG(ADD_C), VAR_WR_XWR, REG_R7, 0, 0, OP_ROL_X, 0, 0, 0427},

	{ARG(ADD_WR) | ARG(ADD_REG) | ARG(ADD_C), VAR_WR_XWR, REG_R0, 0, 0, OP_ROL_X, 0, 0, 0460},
	{ARG(ADD_WR) | ARG(ADD_REG) | ARG(ADD_C), VAR_WR_XWR, REG_R1, 0, 0, OP_ROL_X, 0, 0, 0461},
	{ARG(ADD_WR) | ARG(ADD_REG) | ARG(ADD_C), VAR_WR_XWR, REG_R2, 0, 0, OP_ROL_X, 0, 0, 0462},
	{ARG(ADD_WR) | ARG(ADD_REG) | ARG(ADD_C), VAR_WR_XWR, REG_R3, 0, 0, OP_ROL_X, 0, 0, 0463},
	{ARG(ADD_WR) | ARG(ADD_REG) | ARG(ADD_C), VAR_WR_XWR, REG_R4, 0, 0, OP_ROL_X, 0, 0, 0464},
	{ARG(ADD_WR) | ARG(ADD_REG) | ARG(ADD_C), VAR_WR_XWR, REG_R5, 0, 0, OP_ROL_X, 0, 0, 0465},
	{ARG(ADD_WR) | ARG(ADD_REG) | ARG(ADD_C), VAR_WR_XWR, REG_R6, 0, 0, OP_ROL_X, 0, 0, 0466},
	{ARG(ADD_WR) | ARG(ADD_REG) | ARG(ADD_C), VAR_WR_XWR, REG_R7, 0, 0, OP_ROL_X, 0, 0, 0467},

	{ARG(ADD_WR) | ARG(SUB_DIP) | ARG(SUB_1) | ARG(ADD_C), VAR_WR_XWR, 0, 0, 0, OP_SAR_X, 0, 0, 0132},
	{ARG(ADD_WR) | ARG(ADD_DIP) | ARG(ADD_C), VAR_WR_XWR, 0, 0, 0, OP_SAR_X, 0, 0, 0572},

	{ARG(ADD_WR) | ARG(SUB_REG) | ARG(SUB_1) | ARG(ADD_C), VAR_WR_XWR, REG_R0, 0, 0, OP_SAR_X, 0, 0, 0120},
	{ARG(ADD_WR) | ARG(SUB_REG) | ARG(SUB_1) | ARG(ADD_C), VAR_WR_XWR, REG_R1, 0, 0, OP_SAR_X, 0, 0, 0121},
	{ARG(ADD_WR) | ARG(SUB_REG) | ARG(SUB_1) | ARG(ADD_C), VAR_WR_XWR, REG_R2, 0, 0, OP_SAR_X, 0, 0, 0122},
	{ARG(ADD_WR) | ARG(SUB_REG) | ARG(SUB_1) | ARG(ADD_C), VAR_WR_XWR, REG_R3, 0, 0, OP_SAR_X, 0, 0, 0123},
	{ARG(ADD_WR) | ARG(SUB_REG) | ARG(SUB_1) | ARG(ADD_C), VAR_WR_XWR, REG_R4, 0, 0, OP_SAR_X, 0, 0, 0124},
	{ARG(ADD_WR) | ARG(SUB_REG) | ARG(SUB_1) | ARG(ADD_C), VAR_WR_XWR, REG_R5, 0, 0, OP_SAR_X, 0, 0, 0125},
	{ARG(ADD_WR) | ARG(SUB_REG) | ARG(SUB_1) | ARG(ADD_C), VAR_WR_XWR, REG_R6, 0, 0, OP_SAR_X, 0, 0, 0126},
	{ARG(ADD_WR) | ARG(SUB_REG) | ARG(SUB_1) | ARG(ADD_C), VAR_WR_XWR, REG_R7, 0, 0, OP_SAR_X, 0, 0, 0127},

	{ARG(ADD_WR) | ARG(ADD_REG) | ARG(ADD_C), VAR_WR_XWR, REG_R0, 0, 0, OP_SAR_X, 0, 0, 0560},
	{ARG(ADD_WR) | ARG(ADD_REG) | ARG(ADD_C), VAR_WR_XWR, REG_R1, 0, 0, OP_SAR_X, 0, 0, 0561},
	{ARG(ADD_WR) | ARG(ADD_REG) | ARG(ADD_C), VAR_WR_XWR, REG_R2, 0, 0, OP_SAR_X, 0, 0, 0562},
	{ARG(ADD_WR) | ARG(ADD_REG) | ARG(ADD_C), VAR_WR_XWR, REG_R3, 0, 0, OP_SAR_X, 0, 0, 0563},
	{ARG(ADD_WR) | ARG(ADD_REG) | ARG(ADD_C), VAR_WR_XWR, REG_R4, 0, 0, OP_SAR_X, 0, 0, 0564},
	{ARG(ADD_WR) | ARG(ADD_REG) | ARG(ADD_C), VAR_WR_XWR, REG_R5, 0, 0, OP_SAR_X, 0, 0, 0565},
	{ARG(ADD_WR) | ARG(ADD_REG) | ARG(ADD_C), VAR_WR_XWR, REG_R6, 0, 0, OP_SAR_X, 0, 0, 0566},
	{ARG(ADD_WR) | ARG(ADD_REG) | ARG(ADD_C), VAR_WR_XWR, REG_R7, 0, 0, OP_SAR_X, 0, 0, 0567},
	
	{0, 0, 0, 0, 0, OP_NOP, 0, 0, NOP},
	{0, 0, 0, 0, 0, OP_HALT, 0, 0, NOP},
	{0, 0, 0, 0, 0, 0, 0, 0, 0},
};

