#include "reader.h"
#include "lexer.h"
#include "parser.h"
#include "sds.h"
#include "utf8proc.h"

int main(void)
{
	struct reader reader = {0};
	struct lexer lexer = {0};
	struct parser parser = {0};
	int32_t rune = 0;
	char utf8[8] = {0};
	sds token = NULL;

	reader_open(&reader, "test.txt");
	lexer_init(&lexer, &reader);
	parser_init(&parser, &lexer);
	parser_run(&parser);
	parser_output(&parser, "output.x584");

	return 0;
}
