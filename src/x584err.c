#include "x584err.h"

#include <stdio.h>
#include <stdlib.h>

static const char *_errors[] = {
	[X584ASM_COMMA_EXPECTED] = "',' expected",
	[X584ASM_TOO_MANY_LABELS] = "Too many labels (more than 1024)",
	[X584ASM_EMPTY_EXPECTED] = "<ПУСТО> expected",
	[X584ASM_FLAG_EXPECTED] = "Flag expected (ПАЛУ0,ПАЛУ1,ПАЛУ2,ПАЛУ3,!СДЛ1,!СДП1,!СДЛ2,!СДП2,РРР0,РРР3,А15,В15)",
	[X584ASM_INVALID_ADDRESS] = "Invalid address",
	[X584ASM_INVALID_CARRY_VAL] = "Invalid carry value",
	[X584ASM_INVALID_NUMBER] = "Invalid number",
	[X584ASM_INVALID_OPCODE] = "Invalid opcode",
	[X584ASM_LABEL_OR_ADDRESS_EXPECTED] = "Label or address expected",
	[X584ASM_LPAR_EXPECTED] = "'(' expected",
	[X584ASM_RPAR_EXPECTED] = "')' expected",
	[X584ASM_TERM_EXPECTED] = "Argument expected (РОН0-РОН7,РР,РРР,ШИНвх,Швх,П)",
	[X584ASM_THEN_EXPECTED] = "'then' or 'то' expected",
	[X584ASM_TOO_MANY_INSTRUCTIONS] = "Too many instructions",
	[X584ASM_XOR_EXPECTED] = "'xor' expected",
	[X584ASM_SYNTAX_ERROR] = "Syntax error",
	[X584ASM_UNEXPECTED_NUMBER] = "Unexpected number",
	[X584ASM_UNEXPECTED_WORD] = "Unexpected word",
	[X584ASM_UNEXPECTED_SYMBOL] = "Unexpected symbol",
	[X584ASM_UNEXPECTED_ANNOTATION] = "Unexpected annotation",
};

static const char *_fatals[] = {
	[X584ASM_FATAL_OUT_OF_MEMORY] = "out of memory",
	[X584ASM_FATAL_FAILED_TO_OPEN_INPUT_FILE] = "failed to open input file",
	[X584ASM_FATAL_FAILED_TO_OPEN_OUTPUT_FILE] = "failed to open output file",
	[X584ASM_FATAL_INPUT_NOT_SPECIFIED] = "input file not specified",
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
	if (error < X584ASM_FATAL_MAX && error > 0)
		fprintf(stderr, "! Fatal error: %s\n", _fatals[error]);
	else
		fprintf(stderr, "! Fatal error #%d\n", error);
	exit(1);
}
