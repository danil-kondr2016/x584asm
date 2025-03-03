#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#define REPLACEMENT 0xFFFD

#include "reader.h"
#include "utils.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "utf8proc.h"

int reader_open(struct reader *r, char *u8path)
{
	if (!r)
		return 0;
	memset(r, 0, sizeof(struct reader));
	r->temp = CRLF_NOT_SAVED;
	r->file = u8open(u8path, "rb");
	if (!r->file) {
		int error = errno;
		fprintf(stderr, "! Failed to open file %s: %s\n", u8path,
				strerror(errno));
		return 0;
	}

	return 1;
}

int reader_close(struct reader *r)
{
	if (!r)
		return 0;
	if (!r->file)
		return 0;

	if (fclose(r->file) == EOF)
		return 0;
	memset(r, 0, sizeof(struct reader));
	return 1;
}

static int32_t _read_rune(struct reader *r);
int32_t reader_getc(struct reader *r)
{
	if (!r)
		return READER_EOF;
	if (!r->file)
		return READER_EOF;

	if (r->ungetc_top > 0)
		return r->ungetc[--r->ungetc_top];

	return _read_rune(r);
}

int32_t reader_ungetc(struct reader *r, int32_t ch)
{
	if (!r)
		return 0;
	if (!r->file)
		return 0;
	if (r->ungetc_top >= READER_BUF_SIZE)
		return 0;
	r->ungetc[r->ungetc_top++] = ch;
	return 1;
}

static int32_t _read_rune(struct reader *r)
{
	static const signed char u8_bytes[256] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
		4, 4, 4, 4, 4,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
		
	int crlf = 0;
	short x = 0;
	int32_t result = 0;
	while (1) {
		if (r->temp == CRLF_NOT_SAVED) {
			if (r->buf_pos >= r->buf_size) {
				r->buf_pos = 0;
				r->buf_size = fread(r->buf, 1, READER_BUF_SIZE, r->file);
			}
			if (r->buf_size == 0)
				x = CRLF_EOF;
			else
				x = (short)(unsigned char)r->buf[r->buf_pos++];
		}
		else {
			x = r->temp;
			r->temp = CRLF_NOT_SAVED;
		}

		if (x <= 0x7F) {
			if (!crlf && x == '\r') {
				crlf = 1;
				continue;
			}
			else if (crlf && x == '\n') {
				result = '\n';
				break;
			}
			else if (crlf && x != '\n') {
				result = '\r';
				r->temp = x;
				break;
			}

			if (r->u8buf_size == 0){ 
				result = x;
				break;
			}
			else {
				r->u8buf_size = 0;
				r->u8buf_pos = 0;
				r->temp = x;
				result = REPLACEMENT;
				break;
			}
		}
		else if (x >= 0xC0 && x <= 0xF4) {
			if (r->u8buf_size == 0) {
				r->u8buf_pos = 0;
				r->u8buf_size = u8_bytes[x];
				r->u8buf[r->u8buf_pos++] = x;
			}
			else {
				r->u8buf_size = 0;
				r->u8buf_pos = 0;
				r->temp = x;
				result = REPLACEMENT;
				break;
			}
		}
		else if (x >= 0x80 && x <= 0xBF) {
			if (r->u8buf_size == 0) {
				result = REPLACEMENT;
				break;
			}
			else {
				r->u8buf[r->u8buf_pos++] = x;
			}
		}
		else {
			result = REPLACEMENT;
			break;
		}

		if (r->u8buf_pos >= r->u8buf_size) {
			utf8proc_iterate(r->u8buf, r->u8buf_size, &result);
			r->u8buf_size = 0;
			r->u8buf_pos = 0;
			break;
		}
	}

	return result;
}
