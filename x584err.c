#include "x584err.h"

#include <stdio.h>
#include <stdlib.h>

static const char *_errors[] = {
	[X584ASM_FATAL_OUT_OF_MEMORY] = "Fatal error: out of memory",
	[X584ASM_PREMATURE_END_OF_LINE] = "Premature end of line",
	[X584ASM_INVALID_ESCAPE_SEQUENCE] = "Invalid escape sequence"};


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
