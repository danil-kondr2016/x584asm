#include "utils.h"

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
		int wpath_size;

		wpath_size = MultiByteToWideChar(CP_UTF8, 0, u8path, -1, 0, 0) + 1;
		wpath = calloc(wpath_size, sizeof(wchar_t));
		if (!wpath)
			return 0;
		MultiByteToWideChar(CP_UTF8, 0, u8path, -1, wpath, wpath_size);
		result = _wfopen(wpath, L"rb");
	}
#else
	result = fopen(u8path, "rb");
#endif
	return result;
}
