#include "utils.h"
#include "x584err.h"

#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#include <wchar.h>
#endif

FILE *u8open(char *u8path, char *mode)
{
	FILE *result;
#ifdef _WIN32
	{
		wchar_t *wpath;
		wchar_t *wmode;
		int wpath_size;
		int wmode_size;

		wpath_size = MultiByteToWideChar(CP_UTF8, 0, u8path, -1, 0, 0) + 1;
		wpath = calloc(wpath_size, sizeof(wchar_t));
		if (!wpath)
			Die(X584ASM_FATAL_OUT_OF_MEMORY);
		MultiByteToWideChar(CP_UTF8, 0, u8path, -1, wpath, wpath_size);

		wmode_size = MultiByteToWideChar(CP_UTF8, 0, mode, -1, 0, 0) + 1;
		wmode = calloc(wmode_size, sizeof(wchar_t));
		if (!wmode)
			Die(X584ASM_FATAL_OUT_OF_MEMORY);
		MultiByteToWideChar(CP_UTF8, 0, mode, -1, wmode, wmode_size);

		result = _wfopen(wpath, wmode);
		free(wpath);
		free(wmode);
	}
#else
	result = fopen(u8path, mode);
#endif
	return result;
}
