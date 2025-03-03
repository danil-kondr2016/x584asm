#include "reader.h"
#include "lexer.h"
#include "parser.h"
#include "program.h"
#include "x584err.h"

#include <sds.h>
#include <utf8proc.h>
#include <cwalk.h>

#define OPTPARSE_API static
#define OPTPARSE_IMPLEMENTATION
#include <optparse.h>

#ifdef _WIN32
#include <windows.h>
#endif

int u8main(int argc, char **argv)
{
	struct reader reader = {0};
	struct program program = {0};
	struct lexer lexer = {0};
	struct parser parser = {0};
	struct optparse opt = {0};

	char *input = NULL;
	char *output = NULL;
	int option;

	optparse_init(&opt, argv);
	while ((option = optparse(&opt, "ho::")) != -1) {
		switch (option) {
		case 'h':
			printf("Usage: x584asm input [-o output]\n");
			return 0;
		case 'o':
			output = opt.optarg;
			break;
		case '?':
			fprintf(stderr, "! Fatal error: %s\n", opt.errmsg);
			return 1;
		}
	}

	input = optparse_arg(&opt);
	if (!input) {
		fprintf(stderr, "! Fatal error: Input file not specified\n");
		return 1;
	}

	if (!output) {
		const char *ext;
		size_t length;

		if (cwk_path_has_extension(input)) {
			cwk_path_get_extension(input, &ext, &length);
			if (strcmp(ext, ".asm") != 0) {
				fprintf(stderr, "! Fatal error: invalid extension\n");
				return 1;
			}
		}

		size_t output_size = 
			cwk_path_change_extension(input, "x584", NULL, 0);
		output = calloc(output_size+1, sizeof(char));
		if (!output)
			Die(X584ASM_FATAL_OUT_OF_MEMORY);
		cwk_path_change_extension(input, "x584", output, output_size+1);
	}

	if (!reader_open(&reader, input)) {
		return 1;
	}
	lexer_init(&lexer, &reader);
	parser_init(&parser, &lexer, &program);
	if (parser_run(&parser)) {
		return !program_output(&program, output);
	}

	return 1;
}

int main(int argc, char **argv)
{
#ifdef _WIN32
	int ret;
	int u8argc;
	char **u8argv;
	LPWSTR *wargv;
	LPCWSTR cmdline;

	cmdline = GetCommandLineW();
	wargv = CommandLineToArgvW(cmdline, &u8argc);
	u8argv = calloc(u8argc + 1, sizeof(char *));
	if (!u8argv)
		Die(X584ASM_FATAL_OUT_OF_MEMORY);
	for (int i = 0; i < u8argc; i++) {
		int size = 
			WideCharToMultiByte(CP_UTF8, 0, wargv[i], -1, 
				NULL, 0, NULL, NULL);
		u8argv[i] = calloc(size+1, sizeof(char));
		if (!u8argv[i])
			Die(X584ASM_FATAL_OUT_OF_MEMORY);
		WideCharToMultiByte(CP_UTF8, 0, 
				wargv[i], -1, u8argv[i], size, NULL, NULL);
	}
	u8argv[u8argc] = NULL;

	return u8main(u8argc, u8argv);
#else
	return u8main(argc, argv);
#endif
}
