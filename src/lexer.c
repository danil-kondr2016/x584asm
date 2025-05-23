#include "lexer.h"
#include "utf8proc.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

static inline int _is_hex_digit(int32_t x)
{
	return (x >= '0' && x <= '9') 
		|| (x >= 'A' && x <= 'F')
		|| (x >= 'a' && x <= 'f');
}

static inline int _is_newline(int32_t codepoint)
{
	utf8proc_category_t category;
	if (codepoint == '\n')
		return 1;
	if (codepoint == '\r')
		return 1;
	if (codepoint == '\f')
		return 1;
	if (codepoint == 0x85)
		return 1;
	category = utf8proc_category(codepoint);
	switch (category) {
	case UTF8PROC_CATEGORY_ZL:
	case UTF8PROC_CATEGORY_ZP:
		return 1;
	default:;
	}
	return 0;
}

static int32_t _l_getc(struct lexer *lexer)
{
	if (lexer->newline) {
		lexer->input_line++;
		lexer->input_col = 1;
		lexer->newline = false;
	}
	lexer->input_col++;
	lexer->input = reader_getc(lexer->reader);
	if (_is_newline(lexer->input)) {
		lexer->newline = true;
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
	lexer->input_line = 1;
	lexer->input_col = 1;
	lexer->line = lexer->input_line;
	lexer->col = lexer->input_col;

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
	lexer_register(lexer, "ВхПАЛУ", KW_ALUCIN);
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
	lexer_register(lexer, "и", KW_AND);
	lexer_register(lexer, "или", KW_OR);
	lexer_register(lexer, "искл_или", KW_XOR);
	lexer_register(lexer, "ВВОД", KW_INPUT);
	lexer_register(lexer, "ОСТАНОВ", KW_BREAK);
	lexer_register(lexer, "СТОП", KW_HALT);
	lexer_register(lexer, "ПУСТО", KW_EMPTY);

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
	lexer_register(lexer, "DI", KW_DIP);
	lexer_register(lexer, "DO", KW_DOP);
	lexer_register(lexer, "IF", KW_IF);
	lexer_register(lexer, "THEN", KW_THEN);
	lexer_register(lexer, "ELSE", KW_ELSE);
	lexer_register(lexer, "GOTO", KW_GOTO);
	lexer_register(lexer, "CO3", KW_CO3);
	lexer_register(lexer, "CO2", KW_CO2);
	lexer_register(lexer, "CO1", KW_CO1);
	lexer_register(lexer, "CO0", KW_CO0);
	lexer_register(lexer, "C", KW_C);
	lexer_register(lexer, "ALUCIN", KW_ALUCIN);
	lexer_register(lexer, "ALUCOUT", KW_CO3);
	lexer_register(lexer, "ALUCOUT0", KW_CO0);
	lexer_register(lexer, "ALUCOUT1", KW_CO1);
	lexer_register(lexer, "ALUCOUT2", KW_CO2);
	lexer_register(lexer, "C2", KW_CO2);
	lexer_register(lexer, "C1", KW_CO1);
	lexer_register(lexer, "C0", KW_CO0);
	lexer_register(lexer, "SL1", KW_SHL1);
	lexer_register(lexer, "SL2", KW_SHL2);
	lexer_register(lexer, "SR1", KW_SHR1);
	lexer_register(lexer, "SR2", KW_SHR2);
	lexer_register(lexer, "WRRT", KW_SHR1);
	lexer_register(lexer, "WRLFT", KW_SHL1);
	lexer_register(lexer, "XWRRT", KW_SHR2);
	lexer_register(lexer, "XWRLFT", KW_SHL2);
	lexer_register(lexer, "XWR0", KW_XWR0);
	lexer_register(lexer, "XWR3", KW_XWR3);
	lexer_register(lexer, "A15", KW_A15);
	lexer_register(lexer, "B15", KW_B15);
	lexer_register(lexer, "AMSB", KW_A15);
	lexer_register(lexer, "BMSB", KW_A15);
	lexer_register(lexer, "NOP", KW_NOP);
	lexer_register(lexer, "LSL", KW_SHL);
	lexer_register(lexer, "LSR", KW_SHR);
	lexer_register(lexer, "ASL", KW_SAL);
	lexer_register(lexer, "ASR", KW_SAR);
	lexer_register(lexer, "RSL", KW_ROL);
	lexer_register(lexer, "RSR", KW_ROR);
	lexer_register(lexer, "INPUT", KW_INPUT);
	lexer_register(lexer, "and", KW_AND);
	lexer_register(lexer, "or", KW_OR);
	lexer_register(lexer, "xor", KW_XOR);
	lexer_register(lexer, "break", KW_BREAK);
	lexer_register(lexer, "halt", KW_HALT);
	lexer_register(lexer, "EMPTY", KW_EMPTY);

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

	char *casefolded_word;

	casefolded_word = (char *)utf8proc_NFKC_Casefold((uint8_t *)word);
	if (!casefolded_word)
		Die(X584ASM_FATAL_OUT_OF_MEMORY);

	lexer->keywords[lexer->keywords_count].word = casefolded_word;
	lexer->keywords[lexer->keywords_count].value = value;
	return lexer->keywords_count++;
}

static void _skip_multiline_comment(struct lexer *lexer);
static void _skip_singleline_comment(struct lexer *lexer);

typedef int32_t (*codepoint_cb)(struct lexer *lexer, sds *token);
static int32_t _word_cb(struct lexer *lexer, sds *token);
static int32_t _number_cb(struct lexer *lexer, sds *token);
static int32_t _hex_cb(struct lexer *lexer, sds *token);
static int32_t _annotation_cb(struct lexer *lexer, sds *token);

static int32_t _collect(struct lexer *lexer, sds *token, codepoint_cb cb);
static int32_t _word(struct lexer *lexer, sds *token);
static int32_t _number(struct lexer *lexer, sds *token);
static int32_t _number0(struct lexer *lexer, sds *token);
static int32_t _hex(struct lexer *lexer, sds *token);
static int32_t _annotation(struct lexer *lexer, sds *token);

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

	lexer->line = lexer->input_line;
	lexer->col = lexer->input_col;

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
		case '/':
			_l_getc(lexer);
			if (lexer->input == '*') {
				lexer->input = INPUT_NOT_SAVED;
				_skip_multiline_comment(lexer);
			}
			else if (lexer->input == '/') {
				lexer->input = INPUT_NOT_SAVED;
				_skip_singleline_comment(lexer);
			}
			else {
				result = '/';
				end = 1;
			}
			break;
		case ';':
		case '#':
			lexer->input = INPUT_NOT_SAVED;
			result = _annotation(lexer, token);
			end = 1;
			break;
		case '0':
			_l_getc(lexer);
			if (lexer->input == 'x' || lexer->input == 'X') {
				lexer->input = INPUT_NOT_SAVED;
				result = _hex(lexer, token);
			}
			else if (lexer->input >= '0' && lexer->input <= '9') {
				result = _number0(lexer, token);
			}
			else {
				*token = sdsnew("0");
				result = RUNE_NUMBER;
			}
			end = 1;
			break;
		case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			result = _number(lexer, token);
			end = 1;
			break;
		case '\t':
		case '\r':
		case '\n':
		case '\v':
		case '\f':
		case 0xFEFF: // ZERO-WIDTH NO-BREAK SPACE
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
				result = _word(lexer, token);
				end = 1;
				break;
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

	return result;
}

static void _skip_multiline_comment(struct lexer *lexer)
{
	int end = 0;
	do {
		if (lexer->input == INPUT_NOT_SAVED)
			_l_getc(lexer);
		if (lexer->input == '*') {
			_l_getc(lexer);
			if (lexer->input == '/') {
				end = 1;
			}
		}
		lexer->input = INPUT_NOT_SAVED;
	}
	while (!end);
}

static void _skip_singleline_comment(struct lexer *lexer)
{
	int end = 0;
	do {
		if (lexer->input == INPUT_NOT_SAVED)
			_l_getc(lexer);
		if (_is_newline(lexer->input)) {
			end = 1;
		}
		else {
			lexer->input = INPUT_NOT_SAVED;
		}
	}
	while (!end);
}

static int32_t _collect(struct lexer *lexer, sds *token, codepoint_cb cb)
{
	int end = 0;
	int32_t result;
	int32_t input = 0;
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
	if (_is_hex_digit(lexer->input))
	{
		int32_t result = lexer->input;
		lexer->input = INPUT_NOT_SAVED;
		return result;
	}
	else {
		return INPUT_EOF;
	}
}

static int32_t _annotation_cb(struct lexer *lexer, sds *token)
{
	int32_t result;
	if (lexer->input == INPUT_NOT_SAVED)
		_l_getc(lexer);

	result = lexer->input;
	lexer->input = INPUT_NOT_SAVED;
	if (_is_newline(result))
		return INPUT_EOF;
	return result;
}

static int32_t _word(struct lexer *lexer, sds *token)
{
	int32_t result = RUNE_WORD;
	int32_t ret;
	
	ret = _collect(lexer, token, _word_cb);
	if (ret == INPUT_ERROR)
		return INPUT_ERROR;

	char *norm_token;
	norm_token = (char *)utf8proc_NFKC_Casefold((uint8_t *)*token);
	for (int i = 0; i < lexer->keywords_count; i++) {
		char *keyword = lexer->keywords[i].word;
		if (strcmp(norm_token, keyword) == 0) {
			result = lexer->keywords[i].value;
			// We don't need string token anymore
			sdsfree(*token);
			*token = NULL;
			break;
		}
	}

	free(norm_token);
	return result;
}

static int32_t _number(struct lexer *lexer, sds *token)
{
	if (_collect(lexer, token, _number_cb) == INPUT_ERROR)
		return INPUT_ERROR;
	return RUNE_NUMBER;
}

static int32_t _hex(struct lexer *lexer, sds *token)
{
	sds token1;
	sds token2;

	if (_collect(lexer, token, _hex_cb) == INPUT_ERROR)
		return INPUT_ERROR;

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

	return RUNE_HEX;
}

static int32_t _number0(struct lexer *lexer, sds *token)
{
	sds token1;
	sds token2;

	if (_collect(lexer, token, _number_cb) == INPUT_ERROR)
		return INPUT_ERROR;

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

	return RUNE_NUMBER;
}

static int32_t _annotation(struct lexer *lexer, sds *token)
{
	lexer->input = INPUT_NOT_SAVED;
	if (_collect(lexer, token, _annotation_cb) == INPUT_ERROR)
		return INPUT_ERROR;
	return RUNE_ANNOTATION;
}


int lexer_free(struct lexer *lexer)
{
	for (int i = 0; i < N_KEYWORDS; i++) {
		free(lexer->keywords[i].word);
	}
	memset(lexer, 0, sizeof(struct lexer));

	return 1;
}
