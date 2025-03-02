#include "x584err.h"

#include <stdio.h>
#include <stdlib.h>

static const char *_errors[] = {
	[X584ASM_FATAL_OUT_OF_MEMORY] = "Fatal error: out of memory",
	[X584ASM_PREMATURE_END_OF_LINE] = "Premature end of line",
	[X584ASM_INVALID_ESCAPE_SEQUENCE] = "Invalid escape sequence",
	[X584ASM_COMMA_EXPECTED] = "',' expected",
	[X584ASM_COMMA_OR_SEMICOLON_EXPECTED] = "',' or ';' expected",
	[X584ASM_TOO_MANY_LABELS] = "Too many labels (more than 1024)",
	[X584ASM_NO_LABELS_SPECIFIED] = "No labels specified",
	[X584ASM_LABEL_ALREADY_DEFINED] = "Label already defined",
	[X584ASM_WORD_EXPECTED] = "Word expected",
	[X584ASM_COLON_EXPECTED] = "':' expected",
	[X584ASM_FLAG_EXPECTED] = "Flag expected (ПАЛУ0,ПАЛУ1,ПАЛУ2,ПАЛУ3,!СДЛ1,!СДП1,!СДЛ2,!СДП2,РРР0,РРР3,А15,В15)",
	[X584ASM_INVALID_ADDRESS] = "Invalid address",
	[X584ASM_INVALID_CARRY_VAL] = "Invalid carry value",
	[X584ASM_INVALID_NUMBER] = "Invalid number",
	[X584ASM_INVALID_OPCODE] = "Invalid opcode",
	[X584ASM_LABEL_EXPECTED] = "Label expected",
	[X584ASM_LABEL_OR_ADDRESS_EXPECTED] = "Label or address expected",
	[X584ASM_LPAR_EXPECTED] = "'(' expected",
	[X584ASM_OP_EXPECTED] = "Operator expected ('+','-','and','or','xor')",
	[X584ASM_PREMATURE_EOF] = "Premature end of file",
	[X584ASM_RPAR_EXPECTED] = "')' expected",
	[X584ASM_SEMICOLON_EXPECTED] = "';' expected",
	[X584ASM_SHIFT_OP_EXPECTED] = "Shift operation expected (СЛЛ,СЛП,САЛ,САП,СЦЛ,СЦП)",
	[X584ASM_TERM_EXPECTED] = "Argument expected (РОН0-РОН7,РР,РРР,ШИНвх,Швх,П)",
	[X584ASM_THEN_EXPECTED] = "'then' or 'то' expected",
	[X584ASM_TOO_MANY_INSTRUCTIONS] = "Too many instructions",
	[X584ASM_UNDEFINED_LABEL] = "Undefined label",
	[X584ASM_XOR_EXPECTED] = "'xor' expected",
	[X584ASM_SYNTAX_ERROR] = "Syntax error",
	[X584ASM_UNEXPECTED_NUMBER] = "Unexpected number",
	[X584ASM_UNEXPECTED_WORD] = "Unexpected word",
	[X584ASM_UNEXPECTED_SYMBOL] = "Unexpected symbol",
};

int Error(int line, int col, int error)
{
	fprintf(stderr, "! Line %d, column %d: error %d", line, col, error);
	if (error < X584ASM_ERROR_MAX && error > 0) {
		fputc(':', stderr);
		fputc(' ', stderr);
		fputs(_errors[error], stderr);
	}
	fputc('\n', stderr);
	return INPUT_ERROR;
}

int Die(int error)
{
	if (error < X584ASM_ERROR_MAX && error > 0)
		fprintf(stderr, "! %s\n", _errors[error]);
	else
		fprintf(stderr, "! Error %d\n", error);
	exit(1);
}
