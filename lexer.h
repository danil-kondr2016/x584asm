#ifndef _lexer_h_
#define _lexer_h_

#include <stdint.h>

#include "reader.h"
#include "sds.h"
#include "x584err.h"

#define INPUT_NOT_SAVED ((int32_t)(-2))
#define INPUT_EOF       ((int32_t)(-1))

#define N_KEYWORDS 2048
#define MAX_LABELS 1024
#define RUNE_LABEL(n) ((int32_t)(INT32_C(0x00210000) | ((n & 0xFFFF))))

#define RUNE_ASSIGN INT32_C(0xFDD0)
#define RUNE_WORD   INT32_C(0xFDD1)
#define RUNE_NUMBER INT32_C(0xFDD2)
#define RUNE_STRING INT32_C(0xFDD3)
#define RUNE_HEX    INT32_C(0xFDD4)

#define KW_START INT32_C(0x00110000)
enum keyword_type {
	KW_RF0 = KW_START, KW_RF1, KW_RF2, KW_RF3,
	KW_RF4, KW_RF5, KW_RF6, KW_RF7,
	KW_WR, KW_XWR, KW_DIP, KW_DOP,
	KW_IF, KW_THEN, KW_ELSE, KW_GOTO,
	KW_CO3, KW_CO2, KW_CO1, KW_CO0,
	KW_C, KW_SHL1, KW_SHR1, KW_SHL2, KW_SHR2,
	KW_XWR0, KW_XWR3, KW_A15, KW_B15, KW_NOP,
	KW_SHL, KW_SHR, KW_SAL, KW_SAR, KW_ROL, KW_ROR,
	KW_INPUT, KW_AND, KW_OR, KW_XOR, KW_LABELS,
	KW_BREAK, KW_HALT,
};

struct keyword {
	sds word;
	int32_t value;
};

struct lexer {
	struct keyword keywords[N_KEYWORDS];
	struct reader *reader;
	int32_t input;
	int keywords_count;
	int line, col;
};

int lexer_init(struct lexer *lexer, struct reader *reader);
int lexer_register(struct lexer *lexer, char *word, int32_t value);
int32_t lexer_next(struct lexer *lexer, sds *token);
int lexer_free(struct lexer *lexer);

#endif
