#ifndef _reader_h_
#define _reader_h_

#include <stdio.h>
#include <stdint.h>

#define READER_BUF_SIZE 4096
#define READER_EOF ((int32_t)-1)

#define CRLF_EOF ((short)-1)
#define CRLF_NOT_SAVED ((short)-2)

struct reader {
	int32_t ungetc[READER_BUF_SIZE];
	char buf[READER_BUF_SIZE];
	char u8buf[4];
	short temp;
	short reserved;
	FILE *file;
	int buf_pos, buf_size;
	int u8buf_pos, u8buf_size;
	int ungetc_top;
};

int reader_open(struct reader *r, char *u8path);
int reader_close(struct reader *r);

int32_t reader_getc(struct reader *r);
int reader_ungetc(struct reader *r, int32_t ch);

#endif
