#include "stdafx.h"
#include "utils.h"
#include "windefs.h"

#include <string>

LPSTR utils_getLastErrorStr(DWORD dwErrCode) {
	LPTSTR errorText = NULL;

	FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM
		| FORMAT_MESSAGE_ALLOCATE_BUFFER
		| FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwErrCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&errorText,
		0,
		NULL);
	return errorText;
}

void utils_debugOutputErrCode(DWORD dwErrCode) {
	OutputDebugStringA(utils_getLastErrorStr(dwErrCode));
}