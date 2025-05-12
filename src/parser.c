#include "parser.h"

#include <sds.h>

#include <stdlib.h>
#include <string.h>

#include "opcodes.h"
#include <utf8proc.h>

static int32_t Consume(struct parser *parser);
static int Back(struct parser *parser);
static int Match(struct parser *parser, int32_t rune);
static void Panic(struct parser *parser, int error);
static void Fail(struct parser *parser, int error);
static int GenerateOpcode(struct parser *parser);

static int IsValidBinary(char *str)
{
	for (int i = 0; str[i]; i++) {
		if (str[i] != '0' && str[i] != '1')
			return 0;
	}
	return 1;
}

static int32_t Consume(struct parser *parser)
{
	if (parser->next_input != INPUT_NOT_SAVED) {
		parser->prev_input = parser->input;
		parser->prev_token = parser->token;
		parser->prev_line = parser->line;
		parser->prev_col = parser->col;
		parser->input = parser->next_input;
		parser->token = parser->next_token;
		parser->line = parser->next_line;
		parser->col = parser->next_col;
		parser->next_input = INPUT_NOT_SAVED;
		parser->next_token = NULL;
		parser->next_line = 0;
		parser->next_col = 0;
		return parser->input;
	}

	if (parser->prev_token) {
		sdsfree(parser->prev_token);
		parser->prev_token = NULL;
	}
	parser->prev_token = parser->token;
	parser->prev_input = parser->input;
	parser->prev_line = parser->line;
	parser->prev_col = parser->col;
	parser->token = NULL;
	parser->input = lexer_next(parser->lexer, &parser->token);
	parser->line = parser->lexer->line;
	parser->col = parser->lexer->col;
	return parser->input;
}

static int Back(struct parser *parser)
{
	if (parser->prev_input != INPUT_NOT_SAVED) {
		if (parser->next_token) {
			sdsfree(parser->next_token);
			parser->next_token = NULL;
		}
		parser->next_input = parser->input;
		parser->next_token = parser->token;
		parser->next_line = parser->line;
		parser->next_col = parser->col;
		parser->input = parser->prev_input;
		parser->token = parser->prev_token;
		parser->line = parser->prev_line;
		parser->col = parser->prev_col;
		parser->prev_input = INPUT_NOT_SAVED;
		parser->prev_token = NULL;
		parser->prev_line = 0;
		parser->prev_col = 0;
		return 1;
	}

	return 0;
}

static int Match(struct parser *parser, int32_t rune)
{
	if (parser->input == INPUT_NOT_SAVED) {
		Consume(parser);
	}
	if (parser->input == rune) {
		Consume(parser);
		return 1;
	}
	else {
		return 0;
	}
}

static int MatchLabel(struct parser *parser)
{
	if (parser->input == INPUT_NOT_SAVED) {
		Consume(parser);
	}
	if (IS_RUNE_LABEL(parser->input)) {
		int label = parser->input;
		Consume(parser);
		return LABEL_ID(label) + LABEL_FIRST;
	}
	else {
		return 0;
	}
}

static void Fail(struct parser *parser, int error)
{
	if (error) {
		parser->is_program_valid = false;
		Error(parser->line, parser->col, error);
	}
}

static void Panic(struct parser *parser, int error)
{
	parser->is_program_valid = false;
	if (error)
		Error(parser->line, parser->col, error);
	if (parser->input == '=') // if Panic called in this position, it should be invalid
		Consume(parser);
	if (error == X584ASM_UNEXPECTED_SYMBOL)
		Consume(parser);
	if (error == X584ASM_UNEXPECTED_WORD)
		Consume(parser);
	if (error == X584ASM_UNEXPECTED_NUMBER)
		Consume(parser);
	if (parser->input == KW_NOT_ENGLISH)
		Consume(parser);

	while (parser->input != RUNE_ASSIGN
			&& parser->input != '=' // like ":="
			&& parser->input != ':' // Maybe label
			&& parser->input != '(' // Maybe carry, shift or (WR,XWR)
			&& parser->input != KW_BREAK // Start of instruction
			&& parser->input != KW_HALT // Opcode
			&& parser->input != KW_NOP // Opcode
			&& parser->input != '<' // Should be <ПУСТО>
			&& parser->input != INPUT_EOF
			&& parser->input != KW_NOT_ENGLISH)
	{
		Consume(parser);
		if (parser->input == '(') {
			if ((parser->prev_input == KW_SHL || parser->prev_input == KW_SHR
			|| parser->prev_input == KW_SAL || parser->input == KW_SAR
			|| parser->prev_input == KW_ROL || parser->input == KW_ROR))
			{
				// Guaranteed to be shift, skip it
				Consume(parser);
			}
			else {
				Consume(parser);
				if (parser->prev_input != KW_C) {
					// Should be (WR,XWR)
					Back(parser);
					break;
				}
			}
		}
		else if (parser->input == '=' || parser->input == RUNE_ASSIGN) {
			if (parser->input == '=' && parser->prev_input == KW_C) // Should be П=0 or П=1
				Consume(parser);
			else if (parser->prev_input == KW_RF0 // Should be РОН = ... or РОН := ...
				|| parser->prev_input == KW_RF1
				|| parser->prev_input == KW_RF2
				|| parser->prev_input == KW_RF3
				|| parser->prev_input == KW_RF4
				|| parser->prev_input == KW_RF5
				|| parser->prev_input == KW_RF6
				|| parser->prev_input == KW_RF7
				|| parser->prev_input == KW_WR
				|| parser->prev_input == KW_XWR
				|| parser->prev_input == KW_DOP)
			{
				Back(parser);
				break;
			}
		}
	}

	switch (parser->input) {
	case ':': // Maybe label
		if (parser->prev_input == RUNE_WORD || IS_RUNE_LABEL(parser->prev_input))
			Back(parser);
		else 
			Consume(parser);
		break;
	}
}

static int AddRegister(struct parser *parser, uint8_t id, int sub);

static int Instruction(struct parser *parser);
static int Label(struct parser *parser);
static int Opcode(struct parser *parser);
static int Assign(struct parser *parser);
static int Variable(struct parser *parser);
static int Expr(struct parser *parser);
static int AddLogExpr(struct parser *parser);
static int NXorExpr(struct parser *parser);
static int ShiftExpr(struct parser *parser);
static int Term(struct parser *parser);
static int ShiftOp(struct parser *parser);
static int Carry(struct parser *parser);
static int Operator(struct parser *parser);
static int Conditional(struct parser *parser);
static int Goto(struct parser *parser);
static int Input(struct parser *parser);
static int Annotation(struct parser *parser);

static int Instruction(struct parser *parser)
{
	int ret, state;

	parser->var = VAR_NONE;
	parser->reg = REG_NONE;
	parser->op = OP_NONE;
	parser->arg_add = 0;
	parser->arg1 = 0;
	parser->arg2 = 0;
	parser->brk = 0;
	parser->carry = CARRY_UNUSED;
	parser->error = 0;
	parser->invalid_instruction = false;
	parser->panic = false;

	if (parser->input == INPUT_ERROR) {
		parser->is_program_valid = false;
		return 0;
	}
	if (parser->input == INPUT_EOF) {
		return 0;
	}
	if (parser->address >= N_INSTRUCTIONS) {
		Fail(parser, X584ASM_TOO_MANY_INSTRUCTIONS);
		parser->is_program_valid = false;
		return 0;
	}
	Label(parser);

	state = 1;
	do {
		switch (state) {
		case 1:
			ret = Opcode(parser);
			if (!ret)
				break;
			state = 2;
			break;
		case 2:
			ret = Operator(parser);
			if (!ret) {
				if (parser->error)
					break;
				ret = Annotation(parser);
				if (!ret)
					break;
				else
					state = 4;
			}
			else {
				state = 3;
			}
			break;
		case 3:
			ret = Annotation(parser);
			if (!ret)
				break;
			state = 0;
			break;
		case 4:
			ret = Operator(parser);
			if (!ret)
				break;
			state = 0;
			break;
		}
	}
	while (ret && state);

	if (!ret && parser->error) {
		parser->invalid_instruction = true;
		if (parser->panic) {
			Panic(parser, parser->error);
		}
		else {
			Fail(parser, parser->error);
		}
		
	}
	else {
		if (!GenerateOpcode(parser)) {
			if (!parser->error) {
				Fail(parser, X584ASM_INVALID_OPCODE);
			}
		}
		parser->address++;
	}

	if (parser->input == RUNE_NUMBER || parser->input == RUNE_HEX) {
		Panic(parser, X584ASM_UNEXPECTED_NUMBER);
	}
	else if (parser->input == RUNE_ANNOTATION) {
		Panic(parser, X584ASM_UNEXPECTED_ANNOTATION);
	}
	else if (parser->input == RUNE_WORD) {
		;//maybe label, don't touch
	}
	else if (parser->input == '<') {
		;//maybe <ПУСТО>
	}
	else if (parser->input == '(') {
		;//maybe (WR,XWR)
	}
	else if (parser->input == KW_NOT_ENGLISH) {
		Panic(parser, X584ASM_NON_ENGLISH_KEYWORD);
	}
	else if (parser->input <= 0x10FFFF && parser->input != INPUT_EOF) {
		Panic(parser, X584ASM_UNEXPECTED_SYMBOL);
	}

	return 1;
}

static int AddRegister(struct parser *parser, uint8_t id, int sub)
{
	int flag = -1;
	int carry = 0;

	switch (id) {
	case REG_R0: case REG_R1: case REG_R2: case REG_R3:
	case REG_R4: case REG_R5: case REG_R6: case REG_R7:
		flag = sub ? ARG_SUB_REG : ARG_ADD_REG;
		break;
	case REG_WR: flag = sub ? ARG_SUB_WR : ARG_ADD_WR; break;
	case REG_XWR: flag = sub ? ARG_SUB_XWR : ARG_ADD_XWR; break;
	case REG_DIP: flag = sub ? ARG_SUB_DIP : ARG_ADD_DIP; break;
	case REG_NR0: flag = sub ? ARG_SUB_NREG : ARG_ADD_NREG; id = REG_R0; break;
	case REG_NR1: flag = sub ? ARG_SUB_NREG : ARG_ADD_NREG; id = REG_R1; break;
	case REG_NR2: flag = sub ? ARG_SUB_NREG : ARG_ADD_NREG; id = REG_R2; break;
	case REG_NR3: flag = sub ? ARG_SUB_NREG : ARG_ADD_NREG; id = REG_R3; break;
	case REG_NR4: flag = sub ? ARG_SUB_NREG : ARG_ADD_NREG; id = REG_R4; break;
	case REG_NR5: flag = sub ? ARG_SUB_NREG : ARG_ADD_NREG; id = REG_R5; break;
	case REG_NR6: flag = sub ? ARG_SUB_NREG : ARG_ADD_NREG; id = REG_R6; break;
	case REG_NR7: flag = sub ? ARG_SUB_NREG : ARG_ADD_NREG; id = REG_R7; break;
	case REG_NWR: flag = sub ? ARG_SUB_NWR  : ARG_ADD_NWR; id = REG_WR; break;
	case REG_NXWR: flag = sub ? ARG_SUB_NXWR : ARG_ADD_NXWR; id = REG_XWR; break;
	case REG_NDIP: flag = sub ? ARG_SUB_NDIP : ARG_ADD_NDIP; id = REG_DIP; break;
	case REG_1: flag = sub ? ARG_SUB_1 : ARG_ADD_1; id = 0; break;
	case REG_0: flag = sub ? ARG_SUB_0 : ARG_ADD_0; id = 0; break;
	case REG_N0: flag = sub ? ARG_SUB_N0 : ARG_ADD_N0; id = 0; break;
	case REG_C: flag = sub ? ARG_SUB_C : ARG_ADD_C; id = 0; carry = 1; break;
	case REG_NC: flag = sub ? ARG_SUB_NC : ARG_ADD_NC; id = 0; carry = 1; break;
	}

	if ((flag == ARG_SUB_REG) 
		&& (parser->arg_add & ARG(ADD_REG))
		&& !(parser->arg_add & ARG(SUB_REG)))
	{
		if (id != parser->reg) {
			parser->invalid_instruction = true;
			parser->error = X584ASM_INVALID_OPCODE;
		}
		else
			parser->arg_add &= ~ARG(ADD_REG);
	}
	else if ((flag == ARG_ADD_REG)
		&& (parser->arg_add & ARG(SUB_REG))
		&& !(parser->arg_add & ARG(ADD_REG)))
	{
		if (id != parser->reg) {
			parser->invalid_instruction = true;
			parser->error = X584ASM_INVALID_OPCODE;
		}
		else {
			parser->arg_add &= ~ARG(SUB_REG);
		}
	}
	else if ((flag == ARG_ADD_NREG)
		&& (parser->arg_add & (ARG(ADD_REG)|ARG(SUB_REG)|ARG(SUB_NREG)))
		&& !(parser->arg_add & ARG(ADD_NREG))
		&& (id != parser->reg)) 
	{
		parser->invalid_instruction = true;
		parser->error = X584ASM_INVALID_OPCODE;
	}
	else if ((flag == ARG_SUB_NREG)
		&& (parser->arg_add & (ARG(ADD_REG)|ARG(SUB_REG)|ARG(ADD_NREG)))
		&& !(parser->arg_add & ARG(SUB_NREG))
		&& (id != parser->reg)) 
	{
		parser->invalid_instruction = true;
		parser->error = X584ASM_INVALID_OPCODE;
	}
	else if (flag == ARG_SUB_NC && (parser->arg_add & ARG(ADD_NC))) {
		parser->arg_add &= ~ARG(SUB_NC);
	}
	else if (flag == ARG_ADD_NC && (parser->arg_add & ARG(SUB_NC))) {
		parser->arg_add &= ~ARG(ADD_NC);
	}
	else if (flag == ARG_ADD_C && (parser->arg_add & ARG(SUB_C))) {
		parser->arg_add &= ~ARG(ADD_C);
	}
	else if (flag == ARG_SUB_C && (parser->arg_add & ARG(ADD_C))) {
		parser->arg_add &= ~ARG(ADD_C);
	}
	else if (flag == ARG_ADD_DIP && (parser->arg_add & ARG(SUB_DIP))) {
		parser->arg_add &= ~ARG(ADD_DIP);
	}
	else if (flag == ARG_SUB_DIP && (parser->arg_add & ARG(ADD_DIP))) {
		parser->arg_add &= ~ARG(SUB_DIP);
	}
	else if (flag == ARG_ADD_NDIP && (parser->arg_add & ARG(SUB_NDIP))) {
		parser->arg_add &= ~ARG(ADD_NDIP);
	}
	else if (flag == ARG_SUB_NDIP && (parser->arg_add & ARG(ADD_NDIP))) {
		parser->arg_add &= ~ARG(SUB_NDIP);
	}
	else if (flag == ARG_ADD_WR && (parser->arg_add & ARG(SUB_WR))) {
		parser->arg_add &= ~ARG(ADD_WR);
	}
	else if (flag == ARG_SUB_WR && (parser->arg_add & ARG(ADD_WR))) {
		parser->arg_add &= ~ARG(SUB_WR);
	}
	else if (flag == ARG_ADD_NWR && (parser->arg_add & ARG(SUB_NWR))) {
		parser->arg_add &= ~ARG(ADD_NWR);
	}
	else if (flag == ARG_SUB_NWR && (parser->arg_add & ARG(ADD_NWR))) {
		parser->arg_add &= ~ARG(SUB_NWR);
	}
	else if (flag == ARG_ADD_XWR && (parser->arg_add & ARG(SUB_XWR))) {
		parser->arg_add &= ~ARG(ADD_XWR);
	}
	else if (flag == ARG_SUB_XWR && (parser->arg_add & ARG(ADD_XWR))) {
		parser->arg_add &= ~ARG(SUB_XWR);
	}
	else if (flag == ARG_ADD_NXWR && (parser->arg_add & ARG(SUB_NXWR))) {
		parser->arg_add &= ~ARG(ADD_NXWR);
	}
	else if (flag == ARG_SUB_NXWR && (parser->arg_add & ARG(ADD_NXWR))) {
		parser->arg_add &= ~ARG(SUB_NXWR);
	}
	else if ((flag > -1) && parser->arg_add & (1 << flag)) {
		parser->invalid_instruction = true;
		parser->error = X584ASM_INVALID_OPCODE;
	}
	else if (id == REG_INVALID) {
		parser->invalid_instruction = true;
		parser->error = X584ASM_INVALID_OPCODE;
	}
	else {
		if (id != REG_WR && id != REG_XWR && id != REG_DIP && id)
			parser->reg = id;
		if (carry)
			parser->carry = CARRY_INDEFINITE;
		parser->arg_add |= (1 << flag);
	}

	return !parser->invalid_instruction;
}

static int Variable(struct parser *parser)
{
	int result = VAR_NONE;

	if (Match(parser, KW_RF0)) { result = VAR_R0; }
	else if (Match(parser, KW_RF1)) { result = VAR_R1; }
	else if (Match(parser, KW_RF2)) { result = VAR_R2; }
	else if (Match(parser, KW_RF3)) { result = VAR_R3; }
	else if (Match(parser, KW_RF4)) { result = VAR_R4; }
	else if (Match(parser, KW_RF5)) { result = VAR_R5; }
	else if (Match(parser, KW_RF6)) { result = VAR_R6; }
	else if (Match(parser, KW_RF7)) { result = VAR_R7; }
	else if (Match(parser, KW_WR)) { result = VAR_WR; }
	else if (Match(parser, KW_XWR)) { result = VAR_XWR; }
	else if (Match(parser, KW_DOP)) { result = VAR_DOP; }
	else if (parser->input == KW_NOT_ENGLISH) {
		parser->error = X584ASM_NON_ENGLISH_KEYWORD;
		parser->panic = true;
	}
	// else: token is already saved and ready to further processing
	
	parser->var = result;
	return result;
}

static int Term(struct parser *parser)
{
	int result = 0;

	if (Match(parser, KW_RF0)) { result = REG_R0; }
	else if (Match(parser, KW_RF1)) { result = REG_R1; }
	else if (Match(parser, KW_RF2)) { result = REG_R2; }
	else if (Match(parser, KW_RF3)) { result = REG_R3; }
	else if (Match(parser, KW_RF4)) { result = REG_R4; }
	else if (Match(parser, KW_RF5)) { result = REG_R5; }
	else if (Match(parser, KW_RF6)) { result = REG_R6; }
	else if (Match(parser, KW_RF7)) { result = REG_R7; }
	else if (Match(parser, KW_WR)) { result = REG_WR; }
	else if (Match(parser, KW_XWR)) { result = REG_XWR; }
	else if (Match(parser, KW_DIP)) { result = REG_DIP; }
	else if (Match(parser, KW_ALUCIN)) { result = REG_C; }
	else if (Match(parser, KW_C)) { result = REG_C; }
	else if (parser->input == RUNE_NUMBER && parser->token[0] == '1' && !parser->token[1])
		{ result = REG_1; Consume(parser); }
	else if (parser->input == RUNE_NUMBER && parser->token[0] == '0' && !parser->token[1])
		{ result = REG_0; Consume(parser); }
	else if (Match(parser, '!')) {
		if (Match(parser, KW_RF0)) { result = REG_NR0; }
		else if (Match(parser, KW_RF1)) { result = REG_NR1; }
		else if (Match(parser, KW_RF2)) { result = REG_NR2; }
		else if (Match(parser, KW_RF3)) { result = REG_NR3; }
		else if (Match(parser, KW_RF4)) { result = REG_NR4; }
		else if (Match(parser, KW_RF5)) { result = REG_NR5; }
		else if (Match(parser, KW_RF6)) { result = REG_NR6; }
		else if (Match(parser, KW_RF7)) { result = REG_NR7; }
		else if (Match(parser, KW_WR)) { result = REG_NWR; }
		else if (Match(parser, KW_XWR)) { result = REG_NXWR; }
		else if (Match(parser, KW_DIP)) { result = REG_NDIP; }
		else if (Match(parser, KW_C)) { result = REG_NC; }
		else if (parser->input == RUNE_NUMBER && parser->token[0] == '0' && !parser->token[1])
			{ result = REG_N0; Consume(parser); }
		else { Back(parser); }
	}
	else if (parser->input == RUNE_NUMBER || parser->input == RUNE_HEX) {
		Fail(parser, X584ASM_UNEXPECTED_NUMBER);
		Consume(parser);
		result = REG_INVALID;
	}
	else if (parser->input == RUNE_WORD) {
		Fail(parser, X584ASM_UNEXPECTED_WORD);
		Consume(parser);
		result = REG_INVALID;
	}
	else if (parser->input == KW_NOT_ENGLISH) {
		Fail(parser, X584ASM_NON_ENGLISH_KEYWORD);	
		Consume(parser);
		result = REG_INVALID;
	}

	return result;
}

static int Assign(struct parser *parser)
{
	if (!Variable(parser)) {
		return 0;
	}
	if (!Match(parser, RUNE_ASSIGN) && !Match(parser, '=')) {
		return 0;
	}
	if (!Expr(parser)) {
		return 0;
	}

	return 1;
}

static int XAssign(struct parser *parser)
{
	int shift_op = 0;

	if (!Match(parser, '(')) {
		return 0; 
	}
	if (!Match(parser, KW_WR)) {
		if (parser->input != KW_NOT_ENGLISH)
			parser->error = X584ASM_SYNTAX_ERROR;
		else
			parser->error = X584ASM_NON_ENGLISH_KEYWORD;
		parser->panic = true;
		return 0;
	}
	if (!Match(parser, ',')) {
		parser->error = X584ASM_SYNTAX_ERROR;
		parser->panic = true;
		return 0;
	}
	if (!Match(parser, KW_XWR)) {
		if (parser->input != KW_NOT_ENGLISH)
			parser->error = X584ASM_SYNTAX_ERROR;
		else
			parser->error = X584ASM_NON_ENGLISH_KEYWORD;
		parser->panic = true;
		return 0;
	}
	if (!Match(parser, ')')) {
		parser->error = X584ASM_SYNTAX_ERROR;
		parser->panic = true;
		return 0;
	}
	if (!Match(parser, RUNE_ASSIGN)) {
		parser->error = X584ASM_SYNTAX_ERROR;
		parser->panic = true;
		return 0;
	}

	parser->var = VAR_WR_XWR;
	shift_op = ShiftOp(parser);
	if (!shift_op)
		return 0;
	switch (shift_op) {
	case OP_SHL: parser->op = OP_SHL_X; break;
	case OP_SHR: parser->op = OP_SHR_X; break;
	case OP_SAL: parser->op = OP_SAL_X; break;
	case OP_SAR: parser->op = OP_SAR_X; break;
	case OP_ROL: parser->op = OP_ROL_X; break;
	case OP_ROR: parser->op = OP_ROR_X; break;
	}
	if (!Match(parser, '(')) {
		parser->error = X584ASM_LPAR_EXPECTED;
		parser->panic = true;
		return 0;
	}
	if (!AddLogExpr(parser)) {
		parser->panic = true;
		return 0;
	}
	if (!Match(parser, ',')) {
		parser->error = X584ASM_COMMA_EXPECTED;
		parser->panic = true;
		return 0;
	}
	if (!Match(parser, KW_XWR)) {
		if (parser->input != KW_NOT_ENGLISH)
			parser->error = X584ASM_SYNTAX_ERROR;
		else
			parser->error = X584ASM_NON_ENGLISH_KEYWORD;
		parser->panic = true;
		return 0;
	}
	if (!Match(parser, ')')) {
		parser->error = X584ASM_RPAR_EXPECTED;
		parser->panic = true;
		return 0;
	}

	return 1;
}

static int Expr(struct parser *parser)
{
	int ret;

	if ((ret = AddLogExpr(parser)) != 0)
		return ret;
	else if ((ret = NXorExpr(parser)) != 0)
		return ret;
	else if ((ret = ShiftExpr(parser)) != 0)
		return ret;
	return 0;
}

static int Opcode(struct parser *parser)
{
	int brk = 0;
	int ret = 0;

	if (Match(parser, KW_BREAK)) {
		parser->brk = true;
		brk = 1;
	}

	if (Match(parser, KW_NOP)) {
		parser->op = OP_NOP;
		program_set_opcode(parser->program, parser->address, NOP, brk, 0);
		return 1;
	}
	else if (Match(parser, '<')) {
		if (!Match(parser, KW_EMPTY)) {
			parser->error = X584ASM_EMPTY_EXPECTED;
			parser->panic = true;
			return 0;
		}
		if (!Match(parser, '>')) {
			parser->error = X584ASM_EMPTY_EXPECTED;
			parser->panic = true;
			return 0;
		}
		parser->op = OP_NOP;
		program_set_opcode(parser->program, parser->address, NOP, brk, 0);
		return 1;
	}
	else if (Match(parser, KW_HALT)) {
		parser->op = OP_HALT;
		program_set_opcode(parser->program, parser->address, NOP, 1, 0);
		return 1;
	}
	else {	
		if (parser->input == '(') {
			ret = XAssign(parser);
		}
		else {
			ret = Assign(parser);
		}

		if (!ret) {
			return 0;
		}
		else {
			parser->brk = brk;
			Carry(parser);		
		}
	}

	return 1;
}

static int AddLogExpr(struct parser *parser)
{
	int term;
	bool unary_minus = false;

	if (Match(parser, '-'))
		unary_minus = true;
	term = Term(parser);
	if (!term)
		return 0;

	parser->arg_add = 0;
	parser->reg = 0;
	AddRegister(parser, term, unary_minus ? 1 : 0);
	do {
		if (parser->input == '+' || parser->input == '-') {
			int sub = parser->input == '+' ? 0 : 1;

			if (!parser->op)
			       parser->op = OP_ADD_SUB_NEG;
			else if (parser->op != OP_SHL && parser->op != OP_SHL_X
					&& parser->op != OP_SHR && parser->op != OP_SHR_X
					&& parser->op != OP_SAL && parser->op != OP_SAL_X
					&& parser->op != OP_SAR && parser->op != OP_SAR_X
					&& parser->op != OP_ROL && parser->op != OP_ROL_X
					&& parser->op != OP_ROR && parser->op != OP_ROR_X
					&& parser->op != OP_ADD_SUB_NEG)
			{
				return 0;
			}

			Consume(parser);

			term = Term(parser);
			if (!term) {
				parser->error = X584ASM_TERM_EXPECTED;
				parser->panic = true;
				return 0;
			}
			AddRegister(parser, term, sub);
		}
		else if (parser->input == KW_AND 
				|| parser->input == KW_OR 
				|| parser->input == KW_XOR) {
			if (unary_minus) {
				// found logical expression, invalid
				parser->error = X584ASM_SYNTAX_ERROR;
				parser->panic = true;
				return 0;
			}
			if (parser->op != OP_NONE) {
				return 0;
			}

			parser->arg_add = 0;
			parser->reg = 0;
			parser->arg1 = term;

			if (parser->input == KW_AND) {			
				parser->op = OP_AND;
			}
			else if (parser->input == KW_OR) {
				parser->op = OP_OR;
			}
			else if (parser->input == KW_XOR) {
				parser->op = OP_XOR;
			}
			Consume(parser);

			term = Term(parser);
			if (!term) {
				parser->error = X584ASM_TERM_EXPECTED;
				parser->panic = true;
				return 0;
			}

			parser->arg2 = term;
			break;
		}
		else {
			switch (parser->input) {
			case ',': // maybe , РРР)
				if (parser->op != OP_SHL_X && parser->op != OP_SHR_X
					&& parser->op != OP_SAL_X && parser->op != OP_SAR_X
					&& parser->op != OP_ROL_X && parser->op != OP_ROR_X)
				{
					// definitely not
					parser->error = X584ASM_UNEXPECTED_SYMBOL;
					parser->panic = true;
					return 0;
				}
				break;
			case ')': // maybe inside shift expression
				if (parser->op != OP_SHL && parser->op != OP_SHR
					&& parser->op != OP_SAL && parser->op != OP_SAR
					&& parser->op != OP_ROL && parser->op != OP_ROR)
				{
					// definitely not
					parser->error = X584ASM_UNEXPECTED_SYMBOL;
					parser->panic = true;
					return 0;
				}
				break;
			case '(': // maybe (П=0) or (РР,РРР)
			case '<': // maybe <ПУСТО>
			case RUNE_WORD:
			case RUNE_ANNOTATION:
			case INPUT_EOF: // definitely end of file
				break;
			case RUNE_NUMBER:
			case RUNE_HEX:
				parser->error = X584ASM_UNEXPECTED_NUMBER;
				parser->panic = true;
				return 0;
			default:
				if (parser->input <= 0x10FFFF) {
					parser->error = X584ASM_UNEXPECTED_SYMBOL;
					parser->panic = true;
					return 0;
				}
				// every keyword can be start of new expression
			}
			// maybe nothing
			break;
		}
	}
	while (term);

	if (!parser->op)
		parser->op = OP_ADD_SUB_NEG;

	return 1;
}

static int NXorExpr(struct parser *parser)
{
	int term;

	if (!Match(parser, '!'))
		return 0;
	if (!Match(parser, '(')) {
		Back(parser);
		return 0;
	}
		
	term = Term(parser);
	if (!term) {
		parser->panic = true;
		parser->error = X584ASM_TERM_EXPECTED;
		return 0;
	}
	parser->arg1 = term;

	if (Match(parser, KW_XOR)) {
		parser->op = OP_NXOR;
	}
	else {
		parser->panic = true;
		if (parser->input == KW_NOT_ENGLISH)
			parser->error = X584ASM_NON_ENGLISH_KEYWORD;
		else
			parser->error = X584ASM_XOR_EXPECTED;
		return 0;
	}

	term = Term(parser);
	if (!term) {
		parser->error = X584ASM_TERM_EXPECTED;
		parser->panic = true;
		return 0;
	}
	parser->arg2 = term;

	if (!Match(parser, ')')) {
		parser->error = X584ASM_RPAR_EXPECTED;
		parser->panic = true;
		return 0;
	}

	return 1;
}

static int ShiftOp(struct parser *parser)
{
	int shift_op = OP_NONE;

	if (Match(parser, KW_SHL)) { shift_op = OP_SHL; }
	else if (Match(parser, KW_SHR)) { shift_op = OP_SHR; }
	else if (Match(parser, KW_SAL)) { shift_op = OP_SAL; }
	else if (Match(parser, KW_SAR)) { shift_op = OP_SAR; }
	else if (Match(parser, KW_ROL)) { shift_op = OP_ROL; }
	else if (Match(parser, KW_ROR)) { shift_op = OP_ROR; }
	else if (parser->input == KW_NOT_ENGLISH) {
		parser->error = X584ASM_NON_ENGLISH_KEYWORD;
		parser->panic = true;
		return 0;
	}

	return shift_op;
}

static int ShiftExpr(struct parser *parser)
{
	int shift_op = OP_NONE;

	shift_op = ShiftOp(parser);
	if (!shift_op) {
		return 0;
	}

	parser->op = shift_op;

	if (!Match(parser, '(')) {
		parser->error = X584ASM_LPAR_EXPECTED;
		parser->panic = true;
		return 0;
	}

	if (!AddLogExpr(parser)) {
		return 0;
	}

	if (!Match(parser, ')')) {
		parser->error = X584ASM_RPAR_EXPECTED;
		parser->panic = true;
		return 0;
	}

	if (parser->arg_add == X_WR_p_C) {
		parser->carry = CARRY_INDEFINITE;
	}
	else if (parser->arg_add == X_WR_p_1) {
		parser->carry = CARRY_VALUE_1;
	}
	else if (parser->arg_add == ARG(ADD_WR)) {
		parser->carry = CARRY_VALUE_0;
	}
	else {
		parser->invalid_instruction = true;
		Fail(parser, X584ASM_INVALID_OPCODE);
	}

	return 1;
}

static int Carry(struct parser *parser)
{
	int result = 0;
	bool has = false;

	if (parser->carry != CARRY_INDEFINITE)
		return 0;

	if (!Match(parser, '('))
		return 0;
	if (!Match(parser, KW_C)) {
		if (parser->input == KW_NOT_ENGLISH) {
			parser->error = X584ASM_NON_ENGLISH_KEYWORD;
			parser->panic = true;
			return 0;
		}
		Back(parser); // restore '('
		return 0;
	}
	if (!Match(parser, '=')) {
		parser->error = X584ASM_SYNTAX_ERROR;
		parser->panic = true;
		return 0;
	}
	if (parser->input == RUNE_NUMBER) {
		if (parser->token[0] == '0' && !parser->token[1]) {
			parser->carry = CARRY_VALUE_0;
		       	result = 1;
			Consume(parser);
		}
		else if (parser->token[0] == '1' && !parser->token[1]) {
			parser->carry = CARRY_VALUE_1;
			result = 1;
			Consume(parser);
		}
		else {
			Fail(parser, X584ASM_INVALID_CARRY_VAL);
			Consume(parser);
		}
		has = true;
	} 
	else if (parser->input != ')') {
		Fail(parser, X584ASM_INVALID_CARRY_VAL);
		Consume(parser);
		has = true;
	}

	if (!Match(parser, ')')) {
		parser->error = X584ASM_RPAR_EXPECTED;
		parser->panic = true;
		return 0;
	}
	else if (!has) {
		Fail(parser, X584ASM_INVALID_CARRY_VAL);
		Consume(parser);
		has = true;
	}
	return result;
}

static int RegisterLabel(struct parser *parser)
{
	if (parser->n_labels >= MAX_LABELS) {
		Fail(parser, X584ASM_TOO_MANY_LABELS);
		return 0;
	}
	else {
		int label = parser->n_labels;
		lexer_register(parser->lexer, parser->token,
				RUNE_LABEL(label));
		parser->n_labels++;
		return label + LABEL_FIRST;
	}
}

static int Label(struct parser *parser)
{
	int label = 0;

	if (parser->input == INPUT_NOT_SAVED)
		Consume(parser);
	if (parser->input == RUNE_WORD) {
		label = RegisterLabel(parser);
		Consume(parser);
	}
	else if (!(label = MatchLabel(parser))) {
		return 0;
	}


	if (!Match(parser, ':')) {
		parser->error = X584ASM_UNEXPECTED_WORD;
		parser->panic = true;
		return 0;
	}

	program_set_label(parser->program, LABEL_ID(label), parser->address);

	return 1;
}

static int32_t GotoAddress(struct parser *parser)
{
	int32_t label = LABEL_NEXT;

	if (parser->input == RUNE_NUMBER) {
		long address = strtol(parser->token, NULL, 10);
		if (address < 0 || address > N_INSTRUCTIONS) {
			Fail(parser, X584ASM_INVALID_ADDRESS);
		}
		else {
			label = address;
		}
		Consume(parser);
	}
	else if ((label = MatchLabel(parser)) != 0) {
		label = LABEL_ID(label);
	}
	else if (parser->input == RUNE_WORD) {
		label = RegisterLabel(parser);
		Consume(parser);
	}
	else {
		Fail(parser, X584ASM_LABEL_OR_ADDRESS_EXPECTED);
		label = INPUT_EOF;
	}

	return label;
}

static int Operator(struct parser *parser)
{
	if (Match(parser, KW_IF)) {
		return Conditional(parser);
	}
	else if (Match(parser, KW_GOTO)) {
		return Goto(parser);
	}
	else if (Match(parser, KW_INPUT)) {
		return Input(parser);
	}
	else if (parser->input == KW_NOT_ENGLISH) {
		parser->error = X584ASM_NON_ENGLISH_KEYWORD;
		parser->panic = true;
		return 0;
	}
	return 0;
}

static int Conditional(struct parser *parser)
{
	int32_t flag = 0, label_then = LABEL_NEXT, label_else = LABEL_NEXT;
	if (Match(parser, KW_C) || Match(parser, KW_CO3)) { flag = CF_CO3; }
	else if (Match(parser, KW_CO2)) { flag = CF_CO2; }
	else if (Match(parser, KW_CO1)) { flag = CF_CO1; }
	else if (Match(parser, KW_CO0)) { flag = CF_CO0; }
	else if (Match(parser, KW_XWR0)) { flag = CF_XWR0; }
	else if (Match(parser, KW_XWR3)) { flag = CF_XWR3; }
	else if (Match(parser, KW_A15)) { flag = CF_A15; }
	else if (Match(parser, KW_B15)) { flag = CF_B15; }
	else if (Match(parser, KW_SHL1)) { flag = CF_SHL1; }
	else if (Match(parser, KW_SHL2)) { flag = CF_SHL2; }
	else if (Match(parser, KW_SHR1)) { flag = CF_SHR1; }
	else if (Match(parser, KW_SHR2)) { flag = CF_SHR2; }
	else if (Match(parser, '!')) {
		if (Match(parser, KW_SHL1)) { flag = CF_NOT_SHL1; }
		else if (Match(parser, KW_SHL2)) { flag = CF_NOT_SHL2; }
		else if (Match(parser, KW_SHR1)) { flag = CF_NOT_SHR1; }
		else if (Match(parser, KW_SHR2)) { flag = CF_NOT_SHR2; }
		else if (Match(parser, KW_C) || Match(parser, KW_CO3)) { flag = CF_NOT_CO3; }
		else if (Match(parser, KW_CO2)) { flag = CF_NOT_CO2; }
		else if (Match(parser, KW_CO1)) { flag = CF_NOT_CO1; }
		else if (Match(parser, KW_CO0)) { flag = CF_NOT_CO0; }
		else if (Match(parser, KW_XWR0)) { flag = CF_NOT_XWR0; }
		else if (Match(parser, KW_XWR3)) { flag = CF_NOT_XWR3; }
		else if (Match(parser, KW_A15)) { flag = CF_NOT_A15; }
		else if (Match(parser, KW_B15)) { flag = CF_NOT_B15; }
		else {
			Consume(parser);
			if (parser->input != KW_NOT_ENGLISH)
				parser->error = X584ASM_FLAG_EXPECTED;
			else
				parser->error = X584ASM_NON_ENGLISH_KEYWORD;
			parser->panic = true;
			return 0;
		}
	}
	else {
		Consume(parser);
		if (parser->input != KW_NOT_ENGLISH)
			parser->error = X584ASM_FLAG_EXPECTED;
		else
			parser->error = X584ASM_NON_ENGLISH_KEYWORD;
		parser->panic = true;
		return 0;
	}
	if (!Match(parser, KW_THEN)) {
		if (parser->input != KW_NOT_ENGLISH)
			parser->error = X584ASM_THEN_EXPECTED;
		else
			parser->error = X584ASM_NON_ENGLISH_KEYWORD;
		parser->panic = true;
		return 0;
	}
	label_then = GotoAddress(parser);
	if (!label_then) {
		parser->error = X584ASM_LABEL_OR_ADDRESS_EXPECTED;
		parser->panic = true;
		return 0;
	}
	if (Match(parser, KW_ELSE)) {
		label_else = GotoAddress(parser);
		if (!label_else) {
			parser->error = X584ASM_LABEL_OR_ADDRESS_EXPECTED;
			parser->panic = true;
			return 0;
		}
	}
	program_set_if(parser->program, parser->address, flag, label_then, label_else);

	return 1;
}

static int Goto(struct parser *parser)
{
	int32_t label_to = LABEL_NEXT;
	label_to = GotoAddress(parser);
	if (!label_to) {
		parser->error = X584ASM_LABEL_OR_ADDRESS_EXPECTED;
		parser->panic = true;
		return 0;
	}
	program_set_goto(parser->program, parser->address, label_to);

	return 1;
}

static int Input(struct parser *parser)
{
	int32_t bin_vals[16] = {0,1,10,11,100,101,110,111,1000,1001,1010,1011,
		1100,1101,1110,1111};
	int32_t input_num = 0;
	enum control_input_format format = CI_NONE;

	if (parser->input == RUNE_NUMBER) {
		if (sdslen(parser->token) == 16) {
			if (!IsValidBinary(parser->token)) {
				parser->error = X584ASM_INVALID_NUMBER;
				parser->panic = true;
				return 0;
			}

			long val = strtol(parser->token, NULL, 2);
			if (!val && strcmp(parser->token, "0000000000000000") != 0) {
				parser->error = X584ASM_INVALID_NUMBER;
				parser->panic = true;
				return 0;
			}
			input_num = val; // by definition val <= 65535 and val > 0
			Consume(parser);
			format = CI_BINARY;	
		}
		else if (sdslen(parser->token) == 4) {
			long vals[4], n = 0;
			do {
				if (!IsValidBinary(parser->token))
					break;
				vals[n] = strtol(parser->token, NULL, 2);
				if (!vals[n] && strcmp(parser->token, "0000") != 0) {
					break;
				}
				n++;
				Consume(parser);
				if (parser->input != RUNE_NUMBER) {
					break;
				}
			}
			while (n < 4);
			if (n == 4 && parser->input == RUNE_NUMBER) {
				parser->error = X584ASM_INVALID_NUMBER;
				parser->panic = true;
				return 0;
			}
			if (n == 0) {
				// Number is not like binary.
				input_num = strtol(parser->token, NULL, 10);
				// Number is in range of 0 to 9999.
				if (!input_num && strcmp(parser->token, "0000") != 0) {
					parser->error = X584ASM_INVALID_NUMBER;
					parser->panic = true;
					return 0;
				}
			}
			else if (n == 1) {
				// Number is like binary one.
				input_num = bin_vals[vals[0]];
				// Number is in range of 0 to 9999.
				format = CI_UNSIGNED;
			}
			else if (n == 4) {
				// Number is written out of four four-digit binary numbers.
				input_num = (vals[0]<<12)|(vals[1]<<8)|(vals[2]<<4)|(vals[3]);
				format = CI_GROUPED_BINARY;
			}
			else {
				// n == 2 || n == 3. Error
				parser->error = X584ASM_INVALID_NUMBER;
				parser->panic = true;
				return 0;
			}
		}
		else {
			input_num = strtol(parser->token, NULL, 10);
			if (input_num > 65535) {
				parser->error = X584ASM_INVALID_NUMBER;
				parser->panic = true;
				return 0;
			}
			format = CI_UNSIGNED;
			Consume(parser);
		}
	}
	else if (parser->input == RUNE_HEX) {
		input_num = strtol(parser->token, NULL, 16);
		if (input_num > 65535) {
			parser->error = X584ASM_INVALID_NUMBER;
			parser->panic = true;
			return 0;
		}
		format = CI_HEX;
		Consume(parser);
	}
	else if (Match(parser, '-')) {
		if (parser->input == RUNE_NUMBER) {
			input_num = strtol(parser->token, NULL, 10);
			if (input_num < 1 || input_num > 32768) {
				parser->error = X584ASM_INVALID_NUMBER;
				parser->panic = true;
				return 0;
			}
			input_num = -input_num;
			input_num &= 0xFFFF;
			format = CI_SIGNED;
			Consume(parser);
		}
	}
	program_set_input(parser->program, parser->address, format, input_num);
	
	return 1;
}

static int Annotation(struct parser *parser)
{
	sds annotation = sdsempty();
	sds temp;

	if (!annotation) Die(X584ASM_FATAL_OUT_OF_MEMORY);
	if (parser->input == RUNE_ANNOTATION) { 
		temp = sdscatsds(annotation, parser->token);
		if (!temp) Die(X584ASM_FATAL_OUT_OF_MEMORY);
		annotation = temp;
		Consume(parser);
		program_move_annotation(parser->program, parser->address, &annotation);
	}
	else {
		return 0;
	}


	return 1;
}

static int GenerateOpcode(struct parser *parser)
{
	int opcode;
#ifdef _DEBUG_FIND
	printf("@ OP VR ARGS_ADD A1 A2 RG BK CY OPCODE\n");
	printf("@ %2d %2d %08X %2d %2d %2d %2d %2d ---\n",
		parser->op, parser->var, parser->arg_add,
		parser->arg1, parser->arg2, parser->reg,
		parser->brk, parser->carry);
#endif

	opcode = -1;
	for (int i = 0; _opcodes[i].op; i++) {
		if (parser->op != _opcodes[i].op)
			continue;
		if (parser->var != _opcodes[i].var)
			continue;
		if (parser->reg != _opcodes[i].reg)
			continue;
		if (parser->arg1 != _opcodes[i].arg1)
			continue;
		if (parser->arg2 != _opcodes[i].arg2)
			continue;
		if (parser->carry != _opcodes[i].carry) {
			if (_opcodes[i].carry == CARRY_UNUSED)
				continue;
			if (_opcodes[i].carry == CARRY_VALUE_0 && parser->carry == CARRY_VALUE_1)
				continue;
			if (_opcodes[i].carry == CARRY_VALUE_1 && parser->carry == CARRY_VALUE_0)
				continue;
		}

#ifdef _DEBUG_FIND
		printf("? %2d %2d %08X %2d %2d %2d -- %2d 0%03o\n",
			_opcodes[i].op, _opcodes[i].var, _opcodes[i].arg_add,
			_opcodes[i].arg1, _opcodes[i].arg2, _opcodes[i].reg,
			_opcodes[i].carry, _opcodes[i].opcode);
#endif

		if (parser->op == OP_ADD_SUB_NEG 
			|| parser->op == OP_SHL || parser->op == OP_SHR
			|| parser->op == OP_SAL || parser->op == OP_SAR
			|| parser->op == OP_ROL || parser->op == OP_ROR
			|| parser->op == OP_SHL_X || parser->op == OP_SHR_X
			|| parser->op == OP_SAL_X || parser->op == OP_SAR_X
			|| parser->op == OP_ROL_X || parser->op == OP_ROR_X)
		{
			if (parser->arg_add == _opcodes[i].arg_add) {
				opcode = _opcodes[i].opcode;
				if (parser->carry == CARRY_UNUSED) {
					parser->carry = _opcodes[i].carry;
				}
				break;
			}
		}
		else {
			opcode = _opcodes[i].opcode;
			break;
		}
	}

	if (opcode == -1) {
		parser->invalid_instruction = true;
	}
	if (parser->op == OP_HALT) {
		parser->brk = true;
	}

	if (parser->invalid_instruction) {
		program_set_opcode(parser->program, parser->address, NOP2, 1, 0);
		parser->is_program_valid = false;
		return 0;
	}
	else {
		program_set_opcode(parser->program, parser->address, opcode, parser->brk, parser->carry);
	}

	return 1;
}

bool parser_init(struct parser *parser, struct lexer *lexer, struct program *program)
{
	if (!parser)
		return 0;
	if (!lexer)
		return 0;
	if (!lexer->reader)
		return 0;
	if (!program)
		return 0;

	memset(parser, 0, sizeof(struct parser));
	parser->lexer = lexer;
	parser->prev_input = parser->input = parser->next_input
		= INPUT_NOT_SAVED;
	parser->prev_line = parser->line = parser->next_line = 1;
	parser->prev_col = parser->col = parser->next_col = 1;
	program_init(program);
	parser->program = program;
	parser->is_program_valid = true;

	return 1;
}

bool parser_run(struct parser *parser)
{
	while (parser->input != INPUT_EOF && Instruction(parser));
	return parser->is_program_valid;
}
