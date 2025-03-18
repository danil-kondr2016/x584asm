#include "instrset.h"

static bool var_reg_same(struct instruction *instr);

static const struct {
	uint32_t arg_add;
	uint8_t var;
	uint8_t reg;
	uint16_t opcode;
} set[] = {
	{ARG(ADD_DI),  VAR_RF,  REG_RF,    0760},
	{ARG(ADD_REG), VAR_DOP, REG_RF,    0020},
	{ARG(ADD_REG), VAR_XWR, REG_RF,    0060},
	{ARG(ADD_DI),  VAR_WR,  REG_NONE,  0320},
	{ARG(ADD_DI),  VAR_XWR, REG_NONE,  0072},
	{ARG(ADD_DI),  VAR_DO,  REG_NONE,  0772},
	{0,            0,       0,         NOP},
};

static const struct {
	uint32_t arg_add;
	uint8_t var;
	uint8_t reg;
	uint16_t opcode;
} sum[] = {
	{ARG(ADD_REG)|ARG(ADD_WR)|ARG(ADD_C), VAR_XWR, REG_RF,   0160},
	{ARG(ADD_REG)|ARG(ADD_DI)|ARG(ADD_C), VAR_WR,  REG_RF,   0220},
	{ARG(ADD_REG)|ARG(ADD_DI)|ARG(ADD_C), VAR_XWR, REG_RF,   0260},
	{ARG(ADD_REG)|ARG(ADD_DI)|ARG(ADD_C), VAR_RF,  REG_RF,   0360},
	{ARG(ADD_REG)|ARG(ADD_XWR)|ARG(ADD_C),VAR_WR,  REG_RF,   0620},
	{ARG(ADD_REG)|ARG(ADD_XWR)|ARG(ADD_C),VAR_XWR, REG_RF,   0660},
	{ARG(ADD_XWR)|ARG(ADD_C),             VAR_RF,  REG_RF,   0720},
	{ARG(ADD_WR)|ARG(ADD_DI)|ARG(ADD_C),  VAR_XWR, REG_NONE, 0172},
	{ARG(ADD_WR)|ARG(ADD_DI)|ARG(ADD_C),  VAR_DO,  REG_NONE, 0372},
	{ARG(ADD_XWR)|ARG(ADD_DI)|ARG(ADD_C), VAR_WR,  REG_NONE, 0632},
	{ARG(ADD_XWR)|ARG(ADD_DI)|ARG(ADD_C), VAR_XWR, REG_NONE, 0672},
	{ARG(ADD_XWR)|ARG(ADD_C),             VAR_DO,  REG_NONE, 0732},
	{0,                                   0,       0,         NOP},
};

static const struct {
	uint8_t var;
	uint8_t a;
	uint8_t b;
	uint8_t code;
} alu_type[] = {
	{VAR_RF,  REG_RF,  REG_WR,  000},
	{VAR_WR,  REG_RF,  REG_WR,  010},
	{VAR_DO,  REG_DI,  REG_WR,  030},
	{VAR_WR,  REG_DI,  REG_WR,  031},
	{VAR_WR,  REG_DI,  REG_XWR, 033},
	{VAR_XWR, REG_DI,  REG_WR,  034},
	{VAR_XWR, REG_DI,  REG_XWR, 036},
	{VAR_DO,  REG_DI,  REG_XWR, 037},
	{0,       0,       0,       NOP},
};

#define ALU_ADD_A  0x01
#define ALU_ADD_NA 0x02
#define ALU_SUB_A  0x04
#define ALU_ADD_B  0x08
#define ALU_ADD_NB 0x10
#define ALU_SUB_B  0x20
#define ALU_SUB_1  0x40
#define ALU_ADD_C  0x80

#define ALU_OP_NC       1
#define ALU_OP_ADD      2
#define ALU_OP_AND_A_B  3
#define ALU_OP_XOR_A_B  4
#define ALU_OP_NXOR_A_B 5
#define ALU_OP_AND_NA_B 6
#define ALU_OP_AND_A_NB 7
#define ALU_OP_OR_A_NB  8
#define ALU_OP_OR_NA_B  9
#define ALU_OP_OR_A_B   10

static const struct {
	uint8_t alu_op;
	uint8_t alu_expr;
} alu_ops[] = {
	{ALU_OP_NC,       0x00},
	{ALU_OP_ADD,      ALU_ADD_B | ALU_SUB_A | ALU_SUB_1 | ALU_ADD_C},
	{ALU_OP_ADD,      ALU_ADD_A | ALU_SUB_B | ALU_SUB_1 | ALU_ADD_C},
	{ALU_OP_ADD,      ALU_ADD_A | ALU_ADD_B | ALU_ADD_C},
	{ALU_OP_ADD,      ALU_ADD_B | ALU_ADD_C},
	{ALU_OP_ADD,      ALU_ADD_NB | ALU_ADD_C},
	{ALU_OP_ADD,      ALU_ADD_A | ALU_ADD_C},
	{ALU_OP_ADD,      ALU_ADD_NA | ALU_ADD_C},
	{ALU_OP_AND_A_B,  0x00},
	{ALU_OP_XOR_A_B,  0x00},
	{ALU_OP_NXOR_A_B, 0x00},
	{ALU_OP_AND_NA_B, 0x00},
	{ALU_OP_AND_A_NB, 0x00},
	{ALU_OP_OR_A_NB,  0x00},
	{ALU_OP_OR_NA_B,  0x00},
	{ALU_OP_OR_A_B,   0x00},
	{0,               0},
};

static const struct {
	uint32_t arg_add;
	uint8_t op;
	uint8_t reg;
	uint16_t opcode;
} shift[] = {
	{ARG(ADD_WR)|ARG(SUB_DI)|ARG(SUB_1)|ARG(ADD_C), OP_ROL_X, REG_NONE, 0432},
	{ARG(ADD_WR)|ARG(ADD_DI)|ARG(ADD_C),            OP_ROL_X, REG_NONE, 0472},
	{ARG(ADD_WR)|ARG(SUB_REG)|ARG(SUB_1)|ARG(ADD_C),OP_ROL_X, REG_RF,   0420},
	{ARG(ADD_WR)|ARG(ADD_REG)|ARG(ADD_C),           OP_ROL_X, REG_RF,   0460},
	{ARG(ADD_WR)|ARG(ADD_C),                        OP_SAR_X, REG_NONE, 0520},
	{ARG(ADD_WR)|ARG(SUB_DI)|ARG(SUB_1)|ARG(ADD_C), OP_SAR_X, REG_NONE, 0132},
	{ARG(ADD_WR)|ARG(ADD_DI)|ARG(ADD_C),            OP_SAR_X, REG_NONE, 0572},
	{ARG(ADD_WR)|ARG(SUB_REG)|ARG(SUB_1)|ARG(ADD_C),OP_SAR_X, REG_RF,   0120},
	{ARG(ADD_WR)|ARG(ADD_REG)|ARG(ADD_C),           OP_SAR_X, REG_RF,   0560},
	{ARG(ADD_WR)|ARG(ADD_C),                        OP_SAR,   REG_NONE, 0035},
	{ARG(ADD_WR)|ARG(ADD_C),                        OP_ROR,   REG_NONE, 0075},
	{ARG(ADD_WR)|ARG(ADD_C),                        OP_SAL,   REG_NONE, 0135},
	{ARG(ADD_WR)|ARG(ADD_C),                        OP_ROL,   REG_NONE, 0175},
	{ARG(ADD_WR)|ARG(ADD_C),                        OP_SHR,   REG_NONE, 0435},
	{ARG(ADD_WR)|ARG(ADD_C),                        OP_SHL,   REG_NONE, 0535},
	{ARG(ADD_WR)|ARG(ADD_C),                        OP_ROR_X, REG_NONE, 0275},
	{ARG(ADD_WR)|ARG(ADD_C),                        OP_SAL_X, REG_NONE, 0335},
	{ARG(ADD_WR)|ARG(ADD_C),                        OP_ROL_X, REG_NONE, 0375},
	{ARG(ADD_WR)|ARG(ADD_C),                        OP_SHR_X, REG_NONE, 0635},
	{ARG(ADD_WR)|ARG(ADD_C),                        OP_SHL_X, REG_NONE, 0675},
	{0,                                             0,        0,        NOP},
};

static bool var_reg_same(struct instruction *instr)
{
	if (instr->var == VAR_R0 && instr->reg == REG_R0)
		return true;
	if (instr->var == VAR_R1 && instr->reg == REG_R1)
		return true;
	if (instr->var == VAR_R2 && instr->reg == REG_R2)
		return true;
	if (instr->var == VAR_R3 && instr->reg == REG_R3)
		return true;
	if (instr->var == VAR_R4 && instr->reg == REG_R4)
		return true;
	if (instr->var == VAR_R5 && instr->reg == REG_R5)
		return true;
	if (instr->var == VAR_R6 && instr->reg == REG_R6)
		return true;
	if (instr->var == VAR_R7 && instr->reg == REG_R7)
		return true;
	return false;
}

static int test_set(struct instruction *instr)
{
	int s = 0;
	int opcode = -1;
	for (int i = 0; set[i].arg_add; i++) {
		if (instr->arg_add != set[i].arg_add)
			continue;

		if (set[i].var == VAR_RF && set[i].reg == REG_RF) {
			if (var_reg_same(instr)) {
				switch (instr->var) {
				case VAR_R0: s = 0; break;
				case VAR_R1: s = 1; break;
				case VAR_R2: s = 2; break;
				case VAR_R3: s = 3; break;
				case VAR_R4: s = 4; break;
				case VAR_R5: s = 5; break;
				case VAR_R6: s = 6; break;
				case VAR_R7: s = 7; break;
				}
				opcode = set[i].opcode | s;
				break;
			}
		}
		else if (set[i].var != VAR_RF && set[i].reg == REG_RF) {
			if (instr->var != set[i].var)
				continue;
			switch (instr->reg) {
			case REG_R0: s = 0; break;
			case REG_R1: s = 1; break;
			case REG_R2: s = 2; break;
			case REG_R3: s = 3; break;
			case REG_R4: s = 4; break;
			case REG_R5: s = 5; break;
			case REG_R6: s = 6; break;
			case REG_R7: s = 7; break;
			}
			opcode = set[i].opcode | s;
		}
		else if (set[i].reg == REG_NONE) {
			if (instr->var != set[i].var)
				continue;
			opcode = set[i].opcode;
			break;
		}
	}

	return opcode;
}

static int test_sum(struct instruction *instr);
