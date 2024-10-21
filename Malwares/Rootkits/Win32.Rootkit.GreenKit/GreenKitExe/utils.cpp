#include "stdafx.h"
#include "utils.h"

#include <string>

char* utils_getLastErrorStr(DWORD dwErrCode) {
	LPTSTR errorText = NULL;

	FormatMessage(
		// use system message tables to retrieve error text
		FORMAT_MESSAGE_FROM_SYSTEM
		// allocate buffer on local heap for error text
		| FORMAT_MESSAGE_ALLOCATE_BUFFER
		// Important! will fail otherwise, since we're not 
		// (and CANNOT) pass insertion parameters
		| FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,    // unused with FORMAT_MESSAGE_FROM_SYSTEM
		dwErrCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&errorText,  // output 
		0, // minimum size for output buffer
		NULL);   // arguments - see note 
	return (char *) errorText;
}

void utils_debugOutputErrCode(DWORD dwErrCode) {
	OutputDebugStringA(utils_getLastErrorStr(dwErrCode));
}