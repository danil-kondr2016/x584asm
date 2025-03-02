#include "lexer.h"
#include "utf8proc.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

static int _is_newline(int32_t codepoint)
{
	utf8proc_category_t category;
	if (codepoint == '\n')
		return 1;
	if (codepoint == 0x85)
		return 1;
	category = utf8proc_category(codepoint);
	switch (category) {
	case UTF8PROC_CATEGORY_ZL:
	case UTF8PROC_CATEGORY_ZP:
		return 1;
	}
	return 0;
}

static int32_t _l_getc(struct lexer *lexer)
{
	lexer->col++;
	lexer->input = reader_getc(lexer->reader);
	if (_is_newline(lexer->input)) {
		lexer->line++;
		lexer->col = 1;
	}
	return lexer->input;
}

int lexer_init(struct lexer *lexer, struct reader *reader)
{
	if (!lexer)
		return 0;
	if (!reader)
		return 0;
	memset(lexer, 0, sizeof(struct lexer));
	lexer->reader = reader;

	lexer_register(lexer, "РОН0", KW_RF0);
	lexer_register(lexer, "РОН1", KW_RF1);
	lexer_register(lexer, "РОН2", KW_RF2);
	lexer_register(lexer, "РОН3", KW_RF3);
	lexer_register(lexer, "РОН4", KW_RF4);
	lexer_register(lexer, "РОН5", KW_RF5);
	lexer_register(lexer, "РОН6", KW_RF6);
	lexer_register(lexer, "РОН7", KW_RF7);
	lexer_register(lexer, "РР", KW_WR);
	lexer_register(lexer, "РРР", KW_XWR);
	lexer_register(lexer, "ШИНвх", KW_DIP);
	lexer_register(lexer, "ШИНвых", KW_DOP);
	lexer_register(lexer, "ЕСЛИ", KW_IF);
	lexer_register(lexer, "ТО", KW_THEN);
	lexer_register(lexer, "ИНАЧЕ", KW_ELSE);
	lexer_register(lexer, "ИДИ_НА", KW_GOTO);
	lexer_register(lexer, "ПАЛУ3", KW_CO3);
	lexer_register(lexer, "ПАЛУ2", KW_CO2);
	lexer_register(lexer, "ПАЛУ1", KW_CO1);
	lexer_register(lexer, "ПАЛУ0", KW_CO0);
	lexer_register(lexer, "П", KW_C);
	lexer_register(lexer, "П2", KW_CO2);
	lexer_register(lexer, "П1", KW_CO1);
	lexer_register(lexer, "П0", KW_CO0);
	lexer_register(lexer, "СДЛ1", KW_SHL1);
	lexer_register(lexer, "СДЛ2", KW_SHL2);
	lexer_register(lexer, "СДП1", KW_SHR1);
	lexer_register(lexer, "СДП2", KW_SHR2);
	lexer_register(lexer, "РРР0", KW_XWR0);
	lexer_register(lexer, "РРР3", KW_XWR3);
	lexer_register(lexer, "А15", KW_A15);
	lexer_register(lexer, "В15", KW_B15);
	lexer_register(lexer, "НОП", KW_NOP);
	lexer_register(lexer, "СЛЛ", KW_SHL);
	lexer_register(lexer, "СЛП", KW_SHR);
	lexer_register(lexer, "САЛ", KW_SAL);
	lexer_register(lexer, "САП", KW_SAR);
	lexer_register(lexer, "СЦЛ", KW_ROL);
	lexer_register(lexer, "СЦП", KW_ROR);
	lexer_register(lexer, "ВВОД", KW_INPUT);
	lexer_register(lexer, "МЕТКИ", KW_LABELS);
	lexer_register(lexer, "ОСТАНОВ", KW_BREAK);
	lexer_register(lexer, "СТОП", KW_HALT);

	lexer_register(lexer, "RF0", KW_RF0);
	lexer_register(lexer, "RF1", KW_RF1);
	lexer_register(lexer, "RF2", KW_RF2);
	lexer_register(lexer, "RF3", KW_RF3);
	lexer_register(lexer, "RF4", KW_RF4);
	lexer_register(lexer, "RF5", KW_RF5);
	lexer_register(lexer, "RF6", KW_RF6);
	lexer_register(lexer, "RF7", KW_RF7);
	lexer_register(lexer, "WR", KW_WR);
	lexer_register(lexer, "XWR", KW_XWR);
	lexer_register(lexer, "DIP", KW_DIP);
	lexer_register(lexer, "DOP", KW_DOP);
	lexer_register(lexer, "IF", KW_IF);
	lexer_register(lexer, "THEN", KW_THEN);
	lexer_register(lexer, "ELSE", KW_ELSE);
	lexer_register(lexer, "GOTO", KW_GOTO);
	lexer_register(lexer, "CO3", KW_CO3);
	lexer_register(lexer, "CO2", KW_CO2);
	lexer_register(lexer, "CO1", KW_CO1);
	lexer_register(lexer, "CO0", KW_CO0);
	lexer_register(lexer, "C", KW_C);
	lexer_register(lexer, "C2", KW_CO2);
	lexer_register(lexer, "C1", KW_CO1);
	lexer_register(lexer, "C0", KW_CO0);
	lexer_register(lexer, "SHL1", KW_SHL1);
	lexer_register(lexer, "SHL2", KW_SHL2);
	lexer_register(lexer, "SHR1", KW_SHR1);
	lexer_register(lexer, "SHR2", KW_SHR2);
	lexer_register(lexer, "XWR0", KW_XWR0);
	lexer_register(lexer, "XWR3", KW_XWR3);
	lexer_register(lexer, "A15", KW_A15);
	lexer_register(lexer, "B15", KW_B15);
	lexer_register(lexer, "NOP", KW_NOP);
	lexer_register(lexer, "SHL", KW_SHL);
	lexer_register(lexer, "SHR", KW_SHR);
	lexer_register(lexer, "SAL", KW_SAL);
	lexer_register(lexer, "SAR", KW_SAR);
	lexer_register(lexer, "ROL", KW_ROL);
	lexer_register(lexer, "ROR", KW_ROR);
	lexer_register(lexer, "INPUT", KW_INPUT);
	lexer_register(lexer, "and", KW_AND);
	lexer_register(lexer, "or", KW_OR);
	lexer_register(lexer, "xor", KW_XOR);
	lexer_register(lexer, "labels", KW_LABELS);
	lexer_register(lexer, "break", KW_BREAK);
	lexer_register(lexer, "halt", KW_HALT);

	lexer->input = INPUT_NOT_SAVED;

	return 1;
}

int lexer_register(struct lexer *lexer, char *word, int32_t value)
{
	if (!lexer)
		return -1;
	if (!word)
		return -1;
	if (lexer->keywords_count >= N_KEYWORDS)
		return -1;

	printf("<L> Register %08X %s\n", value, word);
	lexer->keywords[lexer->keywords_count].word = sdsnew(word);
	lexer->keywords[lexer->keywords_count].value = value;
	return lexer->keywords_count++;
}

static void _skip_comment(struct lexer *lexer);

typedef int32_t (*codepoint_cb)(struct lexer *lexer, sds *token);
static int32_t _word_cb(struct lexer *lexer, sds *token);
static int32_t _number_cb(struct lexer *lexer, sds *token);
static int32_t _hex_cb(struct lexer *lexer, sds *token);
static int32_t _string_cb(struct lexer *lexer, sds *token);

static int32_t _collect(struct lexer *lexer, sds *token, codepoint_cb cb);
static int32_t _word(struct lexer *lexer, sds *token);
static int32_t _number(struct lexer *lexer, sds *token);
static int32_t _number0(struct lexer *lexer, sds *token);
static int32_t _hex(struct lexer *lexer, sds *token);
static int32_t _string(struct lexer *lexer, sds *token);

int32_t lexer_next(struct lexer *lexer, sds *token)
{
	int32_t result;
	int end = 0;
	utf8proc_category_t category;

	assert(lexer);
	assert(token);
	if (*token)
		sdsfree(*token);
	*token = NULL;

	do {
		if (lexer->input == INPUT_NOT_SAVED)
			_l_getc(lexer);

		category = utf8proc_category(lexer->input);
		switch (lexer->input) {
		case ':':
			_l_getc(lexer);
			if (lexer->input == '=') {
				lexer->input = INPUT_NOT_SAVED;
				result = RUNE_ASSIGN;
				end = 1;
			}
			else {
				result = ':';
				end = 1;
			}
			break;
		case '(':
			_l_getc(lexer);
			if (lexer->input == '*') {
				lexer->input = INPUT_NOT_SAVED;
				_skip_comment(lexer);
			}
			else {
				result = '(';
				end = 1;
			}
			break;
		case '"':
			return _string(lexer, token);
		case '0':
			_l_getc(lexer);
			if (lexer->input == 'x' || lexer->input == 'X') {
				lexer->input = INPUT_NOT_SAVED;
				return _hex(lexer, token);
			}
			else if (lexer->input >= '0' && lexer->input <= '9') {
				return _number0(lexer, token);
			}
			else {
				*token = sdsnew("0");
				printf("<L> %08X 0\n", RUNE_NUMBER);
				return RUNE_NUMBER;
			}
			break;
		case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			return _number(lexer, token);
		case '\n':
			lexer->input = INPUT_NOT_SAVED;
			continue;
		default:
			switch (category) {
			case UTF8PROC_CATEGORY_LU:
			case UTF8PROC_CATEGORY_LL:
			case UTF8PROC_CATEGORY_LT:
			case UTF8PROC_CATEGORY_LM:
			case UTF8PROC_CATEGORY_LO:
			case UTF8PROC_CATEGORY_PC: // _
				return _word(lexer, token);
			case UTF8PROC_CATEGORY_ZS:
			case UTF8PROC_CATEGORY_ZL:
			case UTF8PROC_CATEGORY_ZP:
				lexer->input = INPUT_NOT_SAVED;
				continue;
			default:
				result = lexer->input;
				lexer->input = INPUT_NOT_SAVED;
				end = 1;
			}
		}
	} while (!end);
	printf("<L> %08X", result);
	if (*token)
		printf(" %s", *token);
	printf("\n");
	return result;
}

static void _skip_comment(struct lexer *lexer)
{
	int end = 0;
	do {
		if (lexer->input == INPUT_NOT_SAVED)
			_l_getc(lexer);
		if (lexer->input == '*') {
			_l_getc(lexer);
			if (lexer->input == ')') {
				end = 1;
			}
		}
		lexer->input = INPUT_NOT_SAVED;
	}
	while (!end);
}

static int32_t _collect(struct lexer *lexer, sds *token, codepoint_cb cb)
{
	int end = 0;
	int32_t result;
	int32_t input = 0;
	utf8proc_category_t category;
	utf8proc_uint8_t utf8[4];
	utf8proc_ssize_t count;
	sds tmp;

	*token = sdsempty();
	do {
		input = cb(lexer, token);
		if (input == INPUT_EOF)
			break;
		if (input == INPUT_ERROR) {
			result = INPUT_ERROR;
			break;
		}

		count = utf8proc_encode_char(input, utf8);
		tmp = sdscatlen(*token, utf8, count);
		if (!tmp) {
			Die(X584ASM_FATAL_OUT_OF_MEMORY);
		}
		*token = tmp;
	} 
	while (!end);

	return result;
}

static int32_t _word_cb(struct lexer *lexer, sds *token)
{
	int32_t result = 0;
	utf8proc_category_t category;

	if (lexer->input == INPUT_NOT_SAVED)
		_l_getc(lexer);
	if (lexer->input == INPUT_EOF)
		return INPUT_EOF;
	
	category = utf8proc_category(lexer->input);
	switch (category) {
	case UTF8PROC_CATEGORY_LU:
	case UTF8PROC_CATEGORY_LL:
	case UTF8PROC_CATEGORY_LT:
	case UTF8PROC_CATEGORY_LM:
	case UTF8PROC_CATEGORY_LO:
	case UTF8PROC_CATEGORY_ND:
	case UTF8PROC_CATEGORY_NL:
	case UTF8PROC_CATEGORY_NO:
	case UTF8PROC_CATEGORY_PC: // _
		result = lexer->input;
		lexer->input = INPUT_NOT_SAVED;
		break;
	default:
		result = INPUT_EOF;
		break;
	}
	return result;
}

static int32_t _number_cb(struct lexer *lexer, sds *token)
{
	if (lexer->input == INPUT_NOT_SAVED)
		_l_getc(lexer);
	if (lexer->input >= '0' && lexer->input <= '9') {
		int32_t result = lexer->input;
		lexer->input = INPUT_NOT_SAVED;
		return result;
	}
	else  {
		return INPUT_EOF;
	}
}

static int32_t _hex_cb(struct lexer *lexer, sds *token)
{
	if (lexer->input == INPUT_NOT_SAVED)
		_l_getc(lexer);
	if (lexer->input >= '0' && lexer->input <= '9'
		|| lexer->input >= 'A' && lexer->input <= 'F'
		|| lexer->input >= 'a' && lexer->input <= 'f')
	{
		int32_t result = lexer->input;
		lexer->input = INPUT_NOT_SAVED;
		return result;
	}
	else {
		return INPUT_EOF;
	}
}

static int32_t _unicode(struct lexer *lexer, sds *token, int n);
static int32_t _string_cb(struct lexer *lexer, sds *token)
{
	int32_t result;
	if (lexer->input == INPUT_NOT_SAVED)
		_l_getc(lexer);

	result = lexer->input;
	lexer->input = INPUT_NOT_SAVED;
	switch (result) {
	case '\\':
		_l_getc(lexer);
		result = lexer->input;
		lexer->input = INPUT_NOT_SAVED;
		switch (result) {
		case 'r': return '\r';
		case 'n': return '\n';
		case 't': return '\t';
		case 'v': return '\v';
		case 'b': return '\b';
		case 'a': return '\a';
		case 'f': return '\f';
		case '\\': return '\\';
		case '\'': return '\'';
		case '"': return '"';
		case 'u': return _unicode(lexer, token, 4);
		case 'U': return _unicode(lexer, token, 8);
		default:
			  return Error(lexer->line, lexer->col, X584ASM_INVALID_ESCAPE_SEQUENCE);
		}
		break;
	case '"':
		return INPUT_EOF;
	case '\n':
		return Error(lexer->line, lexer->col, X584ASM_PREMATURE_END_OF_LINE);
	default:
		return result;
	}	
}

static int32_t _unicode(struct lexer *lexer, sds *token, int n)
{
	int32_t result = 0;
	int32_t buf_pos;

	for (int i = 0; i < n; i++) {
		_l_getc(lexer);

		result <<= 4;
		switch (lexer->input) {
		case '0': case '1': case '2': case '3':
		case '4': case '5': case '6': case '7':
		case '8': case '9':
			result += (lexer->input) - '0';
			break;
		case 'A': case 'B': case 'C': case 'D':
		case 'E': case 'F':
			result += (lexer->input) - 'A' + 10;
			break;
		case 'a': case 'b': case 'c': case 'd':
		case 'e': case 'f':
			result += (lexer->input) - 'a' + 10;
			break;
		default:
			return Error(lexer->line, lexer->col, X584ASM_INVALID_ESCAPE_SEQUENCE);
		}
	}
	return result;
}

static int32_t _word(struct lexer *lexer, sds *token)
{
	int32_t result = RUNE_WORD;
	int32_t ret;
	
	ret = _collect(lexer, token, _word_cb);
	if (ret == INPUT_ERROR)
		return INPUT_ERROR;

	utf8proc_uint8_t *norm_token, *norm_keyword;
	norm_token = utf8proc_NFKC_Casefold(*token);
	for (int i = 0; i < lexer->keywords_count; i++) {
		char *keyword = lexer->keywords[i].word;
		norm_keyword = utf8proc_NFKC_Casefold(keyword);
		if (strcmp(norm_token, norm_keyword) == 0) {
			result = lexer->keywords[i].value;
			// We don't need string token anymore
			sdsfree(*token);
			*token = NULL;
			break;
		}
		free(norm_keyword);
		norm_keyword = NULL;
	}
	if (norm_keyword)
		free(norm_keyword);
	free(norm_token);
	printf("<L> %08X", result);
	if (*token)
		printf(" %s", *token);
	printf("\n");
	return result;
}

static int32_t _number(struct lexer *lexer, sds *token)
{
	if (_collect(lexer, token, _number_cb) == INPUT_ERROR)
		return puts("<L> INPUT_ERROR"), INPUT_ERROR;
	printf("<L> %08X %s\n", RUNE_NUMBER, *token);
	return RUNE_NUMBER;
}

static int32_t _hex(struct lexer *lexer, sds *token)
{
	sds token1;
	sds token2;

	if (_collect(lexer, token, _hex_cb) == INPUT_ERROR)
		return puts("<L> INPUT_ERROR"), INPUT_ERROR;

	token1 = *token;
	token2 = sdsnew("0x");
	if (!token2) {
		Die(X584ASM_FATAL_OUT_OF_MEMORY);
	}
	token2 = sdscatsds(token2, token1);
	if (!token2) {
		Die(X584ASM_FATAL_OUT_OF_MEMORY);
	}
	sdsfree(token1);
	*token = token2;

	printf("<L> %08X %s\n", RUNE_HEX, *token);
	return RUNE_HEX;
}

static int32_t _number0(struct lexer *lexer, sds *token)
{
	sds token1;
	sds token2;

	if (_collect(lexer, token, _number_cb) == INPUT_ERROR)
		return puts("<L> INPUT_ERROR"), INPUT_ERROR;

	token1 = *token;
	token2 = sdsnew("0");
	if (!token2) {
		Die(X584ASM_FATAL_OUT_OF_MEMORY);
	}
	token2 = sdscatsds(token2, token1);
	if (!token2) {
		Die(X584ASM_FATAL_OUT_OF_MEMORY);
	}
	sdsfree(token1);
	*token = token2;

	printf("<L> %08X %s\n", RUNE_NUMBER, *token);
	return RUNE_NUMBER;
}

static int32_t _string(struct lexer *lexer, sds *token)
{
	lexer->input = INPUT_NOT_SAVED;
	if (_collect(lexer, token, _string_cb) == INPUT_ERROR)
		return puts("<L> INPUT_ERROR"), INPUT_ERROR;
	printf("<L> %08X %s\n", RUNE_STRING, *token);
	return RUNE_STRING;
}


int lexer_free(struct lexer *lexer)
{
	for (int i = 0; i < N_KEYWORDS; i++) {
		sdsfree(lexer->keywords[i].word);
	}
	memset(lexer, 0, sizeof(struct lexer));
}
