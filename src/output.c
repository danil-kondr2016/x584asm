#include "program.h"
#include "utils.h"

#include "lexer.h"
#include "utf8proc.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

static char rune_to_cp1251(int32_t codepoint);
static void magic(FILE *);
static void instruction_v1(struct program *, int, FILE *);
static void magic_v2(FILE *);
static void operator_str_v2(struct program *, int, FILE *);
static void annotation_str_v2(struct program *, int, FILE *);
static void u8string_v2(FILE *, char *);
static int get_operator_str(struct program *, int, char *);

int program_output(struct program *program, char *u8path)
{
	FILE *output;

	output = u8open(u8path, "wb");
	if (!output) {
		int error = errno;
		fprintf(stderr, "! Failed to open file %s: %s\n", u8path,
				strerror(error));
		return 0;
	}

	magic(output);
	for (int i = 0; i < N_INSTRUCTIONS; i++) {
		instruction_v1(program, i, output);
	}
	magic_v2(output);
	for (int i = 0; i < N_INSTRUCTIONS; i++) {
		operator_str_v2(program, i, output);
		annotation_str_v2(program, i, output);
	}

	fflush(output);
	fclose(output);
	return 1;
}

static void magic(FILE *output)
{
	fputs("X584", output);
}

static int transcode(char *src, char *dst, int size)
{
	int len = 0;
	utf8proc_ssize_t pos = 0;
	utf8proc_int32_t rune;

	if (!src)
		return 0;
	if (!dst)
		return 0;
	if (!size)
		return 0;

	for (pos = utf8proc_iterate(src, -1, &rune); 
		rune; 
		pos += utf8proc_iterate(&src[pos], -1, &rune))
	{
		dst[len++] = rune_to_cp1251(rune);
		if (len >= size)
			break;
	}
	dst[len] = '\0';

	return len;
}

static void instruction_v1(struct program *program, int i, FILE *output)
{
	int opcode_a, opcode_b;
	char operator[256] = {0};
	char buf[130];
	char *str;
	int len;

	opcode_a = program->opcode[i] & 0xFF;
	opcode_b = (program->opcode[i] >> 8) & 0xFF;
	fputc(opcode_a, output);
	fputc(opcode_b, output);
	
	if (get_operator_str(program, i, operator) != 1) {
		str = program->annotation[i];
	}
	else {
		str = operator;
	}

	len = transcode(str, buf, 128);
	fputc(len, output);
	if (len)
		fwrite(buf, 1, len, output);
}

static void magic_v2(FILE *output)
{
	fputs("V2.0", output);
}

static void operator_str_v2(struct program *program, int i, FILE *output)
{
	char operator[256] = {0};

	get_operator_str(program, i, operator);
	u8string_v2(output, operator);
}

static void annotation_str_v2(struct program *program, int i, FILE *output)
{
	u8string_v2(output, program->annotation[i]);
}

static int get_operator_str(struct program *program, int i, char *operator)
{
	char buf[32] = {0};
	int32_t val = 0;

	operator[0] = '\0';
	switch (program->control[i].type) {
	case KW_IF:
		strcpy(operator, "если ");
		switch (program->control[i].If.flag) {
		case KW_CO3: strcat(operator, "ПАЛУ3 "); break;
		case KW_CO2: strcat(operator, "ПАЛУ2 "); break;
		case KW_CO1: strcat(operator, "ПАЛУ1 "); break;
		case KW_CO0: strcat(operator, "ПАЛУ0 "); break;
		case KW_XWR0: strcat(operator, "РРР0 "); break;
		case KW_XWR3: strcat(operator, "РРР3 "); break;
		case KW_A15: strcat(operator, "A15 "); break;
		case KW_B15: strcat(operator, "B15 "); break;
		case ~KW_SHL1: strcat(operator, "!СДЛ1 "); break;
		case ~KW_SHL2: strcat(operator, "!СДЛ2 "); break;
		case ~KW_SHR1: strcat(operator, "!СДП1 "); break;
		case ~KW_SHR2: strcat(operator, "!СДП2 "); break;
		default:
			       return 0;
		}

		strcat(operator, "то ");
		if (program->control[i].If.a_then == LABEL_NEXT) {
			val = i + 1;
		}
		else if (program->control[i].If.a_then >= LABEL_FIRST) {
			val = program->label[program->control[i].If.a_then - LABEL_FIRST];
		}
		else {
			val = program->control[i].If.a_then;
		}

		sprintf(buf, "%d", val);
		strcat(operator, buf);
		if (program->control[i].If.a_else != LABEL_NEXT) {
			strcat(operator, " иначе ");
			if (program->control[i].If.a_else >= LABEL_FIRST) {
				val = program->label[program->control[i].If.a_else - LABEL_FIRST];
			}
			else {
				val = program->control[i].If.a_else;
			}
			sprintf(buf, "%d", val);
			strcat(operator, buf);
		}
		return 1;
	case KW_GOTO:
		if (program->control[i].Goto.a_goto == LABEL_NEXT) {
			val = i + 1;
		}
		else if (program->control[i].Goto.a_goto >= LABEL_FIRST) {
			val = program->label[program->control[i].Goto.a_goto - LABEL_FIRST];
		}
		else {
			val = program->control[i].Goto.a_goto;
		}
		sprintf(operator, "goto %d", val);
		return 1;
	case KW_INPUT:
		sprintf(operator, "ввод %d", program->control[i].Input.value);
		return 2;
	default:
		return 0;
	}
}

static char rune_to_cp1251(int32_t codepoint)
{
	switch (codepoint) {
	case 0x0402: return 0x80;
	case 0x0403: return 0x81;
	case 0x201A: return 0x82;
	case 0x0453: return 0x83;
	case 0x201E: return 0x84;
	case 0x2026: return 0x85;
	case 0x2020: return 0x86;
	case 0x2021: return 0x87;
	case 0x20AC: return 0x88;
	case 0x2030: return 0x89;
	case 0x0409: return 0x8A;
	case 0x2039: return 0x8B;
	case 0x040A: return 0x8C;
	case 0x040C: return 0x8D;
	case 0x040B: return 0x8E;
	case 0x040F: return 0x8F;
	case 0x0452: return 0x90;
	case 0x2018: return 0x91;
	case 0x2019: return 0x92;
	case 0x201C: return 0x93;
	case 0x201D: return 0x94;
	case 0x2022: return 0x95;
	case 0x2013: return 0x96;
	case 0x2014: return 0x97;
	case 0x2122: return 0x99;
	case 0x0459: return 0x9A;
	case 0x203A: return 0x9B;
	case 0x045A: return 0x9C;
	case 0x045C: return 0x9D;
	case 0x045B: return 0x9E;
	case 0x045F: return 0x9F;
	case 0x00A0: return 0xA0;
	case 0x040E: return 0xA1;
	case 0x045E: return 0xA2;
	case 0x0408: return 0xA3;
	case 0x00A4: return 0xA4;
	case 0x0490: return 0xA5;
	case 0x00A6: return 0xA6;
	case 0x00A7: return 0xA7;
	case 0x0401: return 0xA8;
	case 0x00A9: return 0xA9;
	case 0x0404: return 0xAA;
	case 0x00AB: return 0xAB;
	case 0x00AC: return 0xAC;
	case 0x00AD: return 0xAD;
	case 0x00AE: return 0xAE;
	case 0x0407: return 0xAF;
	case 0x00B0: return 0xB0;
	case 0x00B1: return 0xB1;
	case 0x0406: return 0xB2;
	case 0x0456: return 0xB3;
	case 0x0491: return 0xB4;
	case 0x00B5: return 0xB5;
	case 0x00B6: return 0xB6;
	case 0x00B7: return 0xB7;
	case 0x0451: return 0xB8;
	case 0x2116: return 0xB9;
	case 0x0454: return 0xBA;
	case 0x00BB: return 0xBB;
	case 0x0458: return 0xBC;
	case 0x0405: return 0xBD;
	case 0x0455: return 0xBE;
	case 0x0457: return 0xBF;
	default:
		if (codepoint < 0)
			return '?';
		if (codepoint > 0x10FFFF)
			return '?';
		if (codepoint <= 0x7F)
			return (char)codepoint;
		if (codepoint >= 0x410 && codepoint <= 0x44F)
			return (char)(codepoint - 0x410 + 0xC0);
		return '?';
	}
}

static void u8string_v2(FILE *output, char *str)
{
	size_t len;

	if (!str) {
		fputc(0, output);
		return;
	}
	len = strlen(str);
	if (!len) {
		fputc(0, output);
		return;
	}

	do {
		int byte = (len & 0x7F);
		len >>= 7;
		if (len)
			byte |= 0x80;
		fputc(byte, output);	
	}
	while (len);
	fputs(str, output);
}
