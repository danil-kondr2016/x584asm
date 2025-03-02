#include "reader.h"
#include "lexer.h"
#include "sds.h"
#include "utf8proc.h"

int main(void)
{
	struct reader reader = {0};
	struct lexer lexer = {0};
	int32_t rune = 0;
	char utf8[8] = {0};
	sds token = NULL;

	reader_open(&reader, "test.txt");
	lexer_init(&lexer, &reader);

	while ((rune = lexer_next(&lexer, &token)) > 0) {
		switch (rune) {
		case RUNE_WORD:
			printf("WORD\t%s\n", token);
			break;
		case RUNE_NUMBER:
			printf("NUMBER\t%s\n", token);
			break;
		case RUNE_STRING:
			printf("STRING\t%s\n", token);
			break;
		case RUNE_ASSIGN:
			printf("ASSIGN\t:=\n", token);
			break;
#define CASE_KW(N) case KW_##N: puts("KEYWORD\t" #N); break
		CASE_KW(LABELS);
		CASE_KW(RF0);
		CASE_KW(RF1);
		CASE_KW(RF2);
		CASE_KW(RF3);
		CASE_KW(RF4);
		CASE_KW(RF5);
		CASE_KW(RF6);
		CASE_KW(RF7);
		CASE_KW(WR);
		CASE_KW(XWR);
		CASE_KW(DIP);
		CASE_KW(DOP);
		CASE_KW(C);
		CASE_KW(CO0);
		CASE_KW(CO1);
		CASE_KW(CO2);
		CASE_KW(CO3);
		CASE_KW(IF);
		CASE_KW(THEN);
		CASE_KW(ELSE);
		CASE_KW(GOTO);
		CASE_KW(INPUT);
		CASE_KW(SHL1);
		CASE_KW(SHL2);
		CASE_KW(SHR1);
		CASE_KW(SHR2);
		CASE_KW(SHL);
		CASE_KW(SHR);
		CASE_KW(SAL);
		CASE_KW(SAR);
		CASE_KW(ROL);
		CASE_KW(ROR);
		CASE_KW(AND);
		CASE_KW(OR);
		CASE_KW(XOR);
		CASE_KW(XWR0);
		CASE_KW(XWR3);
		CASE_KW(A15);
		CASE_KW(B15);
		CASE_KW(NOP);
		default:
			utf8proc_encode_char(rune, utf8);
			printf("RUNE\t%d\t%s\n", rune, utf8);
			break;
		}
	}

	return 0;
}
