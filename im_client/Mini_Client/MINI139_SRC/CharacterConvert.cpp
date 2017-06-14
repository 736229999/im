#include "stdafx.h"
#include "CharacterConvert.h"

// 65001 is utf-8.
wchar_t *CodePageToUnicode(int codePage, const char *src)
{
	if (!src) return 0;
	int srcLen = strlen(src);
	if (!srcLen)
	{
		wchar_t *w = new wchar_t[1];
		w[0] = 0;
		return w;
	}

	int requiredSize = MultiByteToWideChar(codePage,
		0,
		src,srcLen,0,0);

	if (!requiredSize)
	{
		return 0;
	}

	wchar_t *w = new wchar_t[requiredSize+1];
	w[requiredSize] = 0;

	int retval = MultiByteToWideChar(codePage,
		0,
		src,srcLen,w,requiredSize);
	if (!retval)
	{
		delete [] w;
		return 0;
	}

	return w;
}

char *UnicodeToCodePage(int codePage, const wchar_t *src)
{
	if (!src) return 0;
	int srcLen = wcslen(src);
	if (!srcLen)
	{
		char *x = new char[1];
		x[0] = '\0';
		return x;
	}

	int requiredSize = WideCharToMultiByte(codePage,
		0,
		src,srcLen,0,0,0,0);

	if (!requiredSize)
	{
		return 0;
	}

	char *x = new char[requiredSize+1];
	x[requiredSize] = 0;

	int retval = WideCharToMultiByte(codePage,
		0,
		src,srcLen,x,requiredSize,0,0);
	if (!retval)
	{
		delete [] x;
		return 0;
	}

	return x;
}

BOOL is_utf8(const char * string)
{
	if(!string)
		return FALSE;

	const unsigned char * bytes = (const unsigned char *)string;
	while(*bytes)
	{
		if( (// ASCII
			// use bytes[0] <= 0x7F to allow ASCII control characters
			bytes[0] == 0x09 ||
			bytes[0] == 0x0A ||
			bytes[0] == 0x0D ||
			(0x20 <= bytes[0] && bytes[0] <= 0x7E)
			)
			) {
				bytes += 1;
				continue;
		}

		if( (// non-overlong 2-byte
			(0xC2 <= bytes[0] && bytes[0] <= 0xDF) &&
			(0x80 <= bytes[1] && bytes[1] <= 0xBF)
			)
			) {
				bytes += 2;
				continue;
		}

		if( (// excluding overlongs
			bytes[0] == 0xE0 &&
			(0xA0 <= bytes[1] && bytes[1] <= 0xBF) &&
			(0x80 <= bytes[2] && bytes[2] <= 0xBF)
			) ||
			(// straight 3-byte
			((0xE1 <= bytes[0] && bytes[0] <= 0xEC) ||
			bytes[0] == 0xEE ||
			bytes[0] == 0xEF) &&
			(0x80 <= bytes[1] && bytes[1] <= 0xBF) &&
			(0x80 <= bytes[2] && bytes[2] <= 0xBF)
			) ||
			(// excluding surrogates
			bytes[0] == 0xED &&
			(0x80 <= bytes[1] && bytes[1] <= 0x9F) &&
			(0x80 <= bytes[2] && bytes[2] <= 0xBF)
			)
			) {
				bytes += 3;
				continue;
		}

		if( (// planes 1-3
			bytes[0] == 0xF0 &&
			(0x90 <= bytes[1] && bytes[1] <= 0xBF) &&
			(0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
			(0x80 <= bytes[3] && bytes[3] <= 0xBF)
			) ||
			(// planes 4-15
			(0xF1 <= bytes[0] && bytes[0] <= 0xF3) &&
			(0x80 <= bytes[1] && bytes[1] <= 0xBF) &&
			(0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
			(0x80 <= bytes[3] && bytes[3] <= 0xBF)
			) ||
			(// plane 16
			bytes[0] == 0xF4 &&
			(0x80 <= bytes[1] && bytes[1] <= 0x8F) &&
			(0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
			(0x80 <= bytes[3] && bytes[3] <= 0xBF)
			)
			) {
				bytes += 4;
				continue;
		}

		return FALSE;
	}

	return TRUE;
}