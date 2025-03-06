#include "parser.h"

#include <sds.h>

#include <stdlib.h>
#include <string.h>

#define NOP 154

#include "opcodes.h"

/*
 * Grammar of language:
 *
 * program = [label-decl] {instruction}
 *
 * instruction = [<label> ':'] opcode [ operator [ annotation ] ]
 *
 * opcode = [break] command 
 *
 * command = <halt> | <nop> | assign | xassign
 *
 * assign = variable ":=" expr carry
 *
 * variable = "РОН0"|"РОН1"|"РОН2"|"РОН3"|"РОН4"|"РОН5"|"РОН6"|"РОН7"|"РР"|"РРР"|"ШИНвых"
 *
 * expr = logexpr | nxorexpr | addexpr | shiftexpr
 *
 * logexpr = term logop term
 *
 * nxorexpr = '!' '(' term "xor" term ')'
 * 
 * term = reg | "П" | "!" term
 *
 * reg = "РОН0"|"РОН1"|"РОН2"|"РОН3"|"РОН4"|"РОН5"|"РОН6"|"РОН7"|"РР"|"РРР"|"ШИНвх"
 *
 * logop = "and"|"or"|"xor"
 *
 * addexpr = term { ('+' | '-' term) }
 *
 * shiftexpr = shiftop '(' addexpr ')'
 *
 * shiftop = "СЛЛ"|"СЛП"|"САЛ"|"САП"|"СЦЛ"|"СЦП"
 *
 * xassign = '(' "WR" "," "XWR" ")" ":=" shiftop '(' addexpr ',' "XWR" ')'
 *
 * carry = '(' <carry> '=' ('0' | '1') ')'
 *
 * operator = conditional | goto | input
 *
 * conditional = <if> <flag> <then> (<number>|<label>) [ <else> (<number>|<label>) ]
 *
 * goto = <goto> (<number>|<label>)
 *
 * input = <input> (<hex>|<number>|'-' <number>|<number> <number> <number> <number>)
 *
 * annotation = <string> {<string>}
 *
 */

static int32_t Consume(struct parser *parser);
static int Back(struct parser *parser);
static int Match(struct parser *parser, int32_t rune);
static void Panic(struct parser *parser, int error);
static void SeverePanic(struct parser *parser, int error);
static int GenerateOpcode(struct parser *parser);

static int32_t Consume(struct parser *parser)
{
	if (parser->next_input != INPUT_NOT_SAVED) {
		parser->prev_input = parser->input;
		parser->prev_token = parser->token;
		parser->input = parser->next_input;
		parser->token = parser->next_token;
		parser->next_input = INPUT_NOT_SAVED;
		parser->next_token = NULL;
		return parser->input;
	}

	if (parser->prev_token) {
		sdsfree(parser->prev_token);
		parser->prev_token = NULL;
	}
	parser->prev_token = parser->token;
	parser->prev_input = parser->input;
	parser->token = NULL;
	parser->input = lexer_next(parser->lexer, &parser->token);
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
		parser->input = parser->prev_input;
		parser->token = parser->prev_token;
		parser->prev_input = INPUT_NOT_SAVED;
		parser->prev_token = NULL;
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
		return label;
	}
	else {
		return 0;
	}
}


static void Panic(struct parser *parser, int error)
{
	if (error)
		Error(parser->lexer->line, parser->lexer->col, error);
	while (parser->input != ';' && parser->input != INPUT_EOF
			&& parser->input != ','
			&& parser->input != ':')
	{
		Consume(parser);
	}
}

static void SeverePanic(struct parser *parser, int error)
{
	if (error)
		Error(parser->lexer->line, parser->lexer->col, error);
	while (parser->input != ';' && parser->input != INPUT_EOF)
	{
		Consume(parser);
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

	if (parser->input == INPUT_EOF) {
		return 0;
	}
	if (parser->address >= N_INSTRUCTIONS) {
		Error(parser->lexer->line, parser->lexer->col,
			X584ASM_TOO_MANY_INSTRUCTIONS);
		parser->is_program_valid = false;
		return 0;
	}
	Label(parser);

	bool valid_opcode = false;
	bool valid_annotation = false;
	bool valid_operator = false;
	state = 1;
	do {
		switch (state) {
		case 1:
			ret = Opcode(parser);
			if (!ret)
				break;
			valid_opcode = true;
			state = 2;
			break;
		case 2:
			ret = Operator(parser);
			if (!ret) {
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

	if (!ret && !valid_opcode) {
		parser->invalid_instruction = true;
		if (!parser->error) {
			parser->error = X584ASM_SYNTAX_ERROR;
			SeverePanic(parser, X584ASM_SYNTAX_ERROR);
		}
		else if (parser->error != X584ASM_INVALID_OPCODE)
			SeverePanic(parser, parser->error);
	}

	if (!GenerateOpcode(parser)) {
		if (!parser->error) {
			Error(parser->lexer->line, parser->lexer->col,
					X584ASM_INVALID_OPCODE);
		}
	}
	parser->address++;
	return 1;
}

static int AddRegister(struct parser *parser, uint8_t id, int sub)
{
	int flag = -1;
	int invalid = 0;
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
	case REG_0: flag = ARG_0; id = 0; break;
	case REG_C: flag = sub ? ARG_SUB_C : ARG_ADD_C; id = 0; carry = 1; break;
	case REG_NC: flag = sub ? ARG_SUB_NC : ARG_ADD_NC; id = 0; carry = 1; break;
	}

	if ((flag > -1) && parser->arg_add & (1 << flag)) {
		parser->invalid_instruction = true;
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
		else { Back(parser); }
	}
	else if (parser->input == RUNE_NUMBER || parser->input == RUNE_HEX) {
		Error(parser->lexer->line, parser->lexer->col,
				X584ASM_UNEXPECTED_NUMBER);
		Consume(parser);
		result = REG_INVALID;
	}
	else if (parser->input == RUNE_WORD) {
		Error(parser->lexer->line, parser->lexer->col,
				X584ASM_UNEXPECTED_WORD);
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
	if (!Match(parser, RUNE_ASSIGN)) {
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
		return 0;
	}
	if (!Match(parser, ',')) {
		return 0;
	}
	if (!Match(parser, KW_XWR)) {
		return 0;
	}
	if (!Match(parser, ')')) {
		return 0;
	}
	if (!Match(parser, RUNE_ASSIGN)) {
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
		return 0;
	}
	if (!AddLogExpr(parser))
		return 0;
	if (!Match(parser, ',')) {
		parser->error = X584ASM_COMMA_EXPECTED;
		return 0;
	}
	if (!Match(parser, KW_XWR))
		return 0;
	if (!Match(parser, ')')) {
		parser->error = X584ASM_RPAR_EXPECTED;
		return 0;
	}

	return 1;
}

static int Expr(struct parser *parser)
{
	int ret;

	if (ret = AddLogExpr(parser))
		return ret;
	else if (ret = NXorExpr(parser))
		return ret;
	else if (ret = ShiftExpr(parser))
		return ret;
	return 0;
}

static int Opcode(struct parser *parser)
{
	int brk = 0, carry = 0, carry_val = 0;
	int invalid = 0;
	int ret = 0;

	if (Match(parser, KW_BREAK)) {
		brk = 1;
	}

	if (Match(parser, KW_NOP)) {
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
			if (parser->invalid_instruction)
				Panic(parser, X584ASM_INVALID_OPCODE);
			else
				SeverePanic(parser, X584ASM_INVALID_OPCODE);
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

	term = Term(parser);
	if (!term)
		return 0;

	parser->arg_add = 0;
	parser->reg = 0;
	AddRegister(parser, term, 0);
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
				SeverePanic(parser, X584ASM_TERM_EXPECTED);
				return 0;
			}
			AddRegister(parser, term, sub);
		}
		else if (parser->input == KW_AND || parser->input == KW_OR || parser->input == KW_XOR) {
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
			else {
				SeverePanic(parser, X584ASM_OP_EXPECTED);
				return 0;
			}
			Consume(parser);

			term = Term(parser);
			if (!term) {
				SeverePanic(parser, X584ASM_TERM_EXPECTED);
				return 0;
			}

			parser->arg2 = term;
			break;
		}
		else {
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
		SeverePanic(parser, X584ASM_TERM_EXPECTED);
		return 0;
	}
	parser->arg1 = term;

	if (Match(parser, KW_XOR)) {
		parser->op = OP_NXOR;
	}
	else {
		SeverePanic(parser, X584ASM_XOR_EXPECTED);
		return 0;
	}

	term = Term(parser);
	if (!term) {
		SeverePanic(parser, X584ASM_TERM_EXPECTED);
		return 0;
	}
	parser->arg2 = term;

	if (!Match(parser, ')')) {
		SeverePanic(parser, X584ASM_RPAR_EXPECTED);
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
		SeverePanic(parser, X584ASM_LPAR_EXPECTED);
		return 0;
	}

	if (!AddLogExpr(parser)) {
		SeverePanic(parser, X584ASM_INVALID_OPCODE);
		return 0;
	}

	if (!Match(parser, ')')) {
		SeverePanic(parser, X584ASM_LPAR_EXPECTED);
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
		Error(parser->lexer->line, parser->lexer->col,
				X584ASM_INVALID_OPCODE);
	}

	return 1;
}

static int Carry(struct parser *parser)
{
	int result = 0;

	if (!Match(parser, '('))
		return 0;
	if (!Match(parser, KW_C))
		return 0;
	if (!Match(parser, '='))
		return 0;
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
			Error(parser->lexer->line, parser->lexer->col,
					X584ASM_INVALID_CARRY_VAL);
			Consume(parser);
		}
	}
	if (!Match(parser, ')'))
		return 0;
	return result;
}

static int RegisterLabel(struct parser *parser)
{
	if (parser->n_labels >= MAX_LABELS) {
		Error(parser->lexer->line, parser->lexer->col,
			X584ASM_TOO_MANY_LABELS);
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
	int no_label = 0;
	int label = 0;

	if (parser->input == RUNE_WORD) {
		label = RegisterLabel(parser);
		Consume(parser);
	}
	else if (!(label = MatchLabel(parser))) {
		return 0;
	}


	if (!Match(parser, ':')) {
		SeverePanic(parser, X584ASM_COLON_EXPECTED);
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
			Error(parser->lexer->line, parser->lexer->col,
				X584ASM_INVALID_ADDRESS);
		}
		else {
			label = address;
		}
		Consume(parser);
	}
	else if (label = MatchLabel(parser)) {
		label = LABEL_ID(label) + LABEL_FIRST;
	}
	else if (parser->input == RUNE_WORD) {
		label = RegisterLabel(parser);
		Consume(parser);
	}
	else {
		Error(parser->lexer->line, parser->lexer->col,
			X584ASM_LABEL_EXPECTED);
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
	return 0;
}

static int Conditional(struct parser *parser)
{
	int32_t flag = 0, label_then = LABEL_NEXT, label_else = LABEL_NEXT;
	if (Match(parser, KW_C) || Match(parser, KW_CO3)) { flag = KW_CO3; }
	else if (Match(parser, KW_CO2)) { flag = KW_CO2; }
	else if (Match(parser, KW_CO1)) { flag = KW_CO1; }
	else if (Match(parser, KW_CO0)) { flag = KW_CO0; }
	else if (Match(parser, KW_XWR0)) { flag = KW_XWR0; }
	else if (Match(parser, KW_XWR3)) { flag = KW_XWR3; }
	else if (Match(parser, KW_A15)) { flag = KW_A15; }
	else if (Match(parser, KW_B15)) { flag = KW_B15; }
	else if (Match(parser, '!')) {
		if (Match(parser, KW_SHL1)) { flag = ~KW_SHL1; }
		else if (Match(parser, KW_SHL2)) { flag = ~KW_SHL2; }
		else if (Match(parser, KW_SHR1)) { flag = ~KW_SHR1; }
		else if (Match(parser, KW_SHR2)) { flag = ~KW_SHR2; }
		else {
			SeverePanic(parser, X584ASM_FLAG_EXPECTED);
			return 0;
		}
	}
	else {
		SeverePanic(parser, X584ASM_FLAG_EXPECTED);
		return 0;
	}
	if (!Match(parser, KW_THEN)) {
		SeverePanic(parser, X584ASM_THEN_EXPECTED);
		return 0;
	}
	label_then = GotoAddress(parser);
	if (!label_then) {
		SeverePanic(parser, X584ASM_LABEL_OR_ADDRESS_EXPECTED);
		return 0;
	}
	if (Match(parser, KW_ELSE)) {
		label_else = GotoAddress(parser);
		if (!label_else) {
			SeverePanic(parser, X584ASM_LABEL_OR_ADDRESS_EXPECTED);
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
		SeverePanic(parser, X584ASM_LABEL_OR_ADDRESS_EXPECTED);
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
	if (parser->input == RUNE_NUMBER) {
		if (sdslen(parser->token) == 16) {
			long val = strtol(parser->token, NULL, 2);
			if (!val && strcmp(parser->token, "0000000000000000") != 0) {
				SeverePanic(parser, X584ASM_INVALID_NUMBER);
				return 0;
			}
			input_num = val; // by definition val <= 65535 and val > 0
			Consume(parser);
		}
		else if (sdslen(parser->token) == 4) {
			long vals[4], n = 0;
			do {
				if (n == 4) {
					SeverePanic(parser, X584ASM_INVALID_NUMBER);
					return 0;
				}
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
			while (n < 5);
			if (n == 0) {
				// Number is not like binary.
				input_num = strtol(parser->token, NULL, 10);
				// Number is in range of 0 to 9999.
				if (!input_num && strcmp(parser->token, "0000") != 0) {
					SeverePanic(parser, X584ASM_INVALID_NUMBER);
					return 0;
				}
			}
			else if (n == 1) {
				// Number is like binary one.
				input_num = bin_vals[vals[0]];
				// Number is in range of 0 to 9999.
			}
			else if (n == 4) {
				// Number is written out of four four-digit binary numbers.
				input_num = (vals[0]<<12)|(vals[1]<<8)|(vals[2]<<4)|(vals[3]);
			}
			else {
				// n == 2 || n == 3. Error
				SeverePanic(parser, X584ASM_INVALID_NUMBER);
				return 0;
			}
		}
		else {
			input_num = strtol(parser->token, NULL, 10);
			if (input_num > 65535) {
				Error(parser->lexer->line, parser->lexer->col,
					X584ASM_INVALID_NUMBER);
				return 0;
			}
			Consume(parser);
		}
	}
	else if (parser->input == RUNE_HEX) {
		input_num = strtol(parser->token, NULL, 16);
		if (input_num > 65535) {
			Error(parser->lexer->line, parser->lexer->col,
				X584ASM_INVALID_NUMBER);
			return 0;
		}
		Consume(parser);
	}
	else if (Match(parser, '-')) {
		if (parser->input == RUNE_NUMBER) {
			input_num = strtol(parser->token, NULL, 10);
			if (input_num < 1 || input_num > 32768) {
				Error(parser->lexer->line, parser->lexer->col,
					X584ASM_INVALID_NUMBER);
				return 0;
			}
			input_num = -input_num;
			input_num &= 0xFFFF;
			Consume(parser);
		}
	}
	program_set_input(parser->program, parser->address, input_num);
	
	return 1;
}

static int Annotation(struct parser *parser)
{
	sds annotation = sdsempty();
	sds temp;
	int n = 0;

	if (!annotation) Die(X584ASM_OUT_OF_MEMORY);
	if (parser->input == RUNE_ANNOTATION) { 
		temp = sdscatsds(annotation, parser->token);
		if (!temp) Die(X584ASM_OUT_OF_MEMORY);
		annotation = temp;
		Consume(parser);
		program_move_annotation(parser->program, parser->address, annotation);
	}
	else {
		return 0;
	}


	return 1;
}

static int GenerateOpcode(struct parser *parser)
{
	int opcode, tmp;
#ifdef _DEBUG_FIND
	printf("@ OP VR ARGS_ADD A1 A2 RG BK CY\n");
	printf("@ %2d %2d %08X %2d %2d %2d %2d %2d\n",
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
		printf("? %2d %2d %08X %2d %2d %2d -- %2d\n",
			_opcodes[i].op, _opcodes[i].var, _opcodes[i].arg_add,
			_opcodes[i].arg1, _opcodes[i].arg2, _opcodes[i].reg,
			_opcodes[i].carry);
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
		program_set_opcode(parser->program, parser->address, 0777, 1, 0);
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
