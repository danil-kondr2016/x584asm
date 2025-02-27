#ifndef _x584err_h_
#define _x584err_h_

#define INPUT_ERROR (-4)

enum error {
	X584ASM_FATAL_OUT_OF_MEMORY = 1,
	X584ASM_PREMATURE_END_OF_LINE,
	X584ASM_UNDEFINED_ESCAPE_SEQUENCE,
	X584ASM_INVALID_UNICODE_ESCAPE_SEQUENCE,
};

int Error(int line, int col, int error);
int Die(int error);

#endif
