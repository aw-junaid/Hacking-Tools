#ifndef FIND_FILES_H
# define FIND_FILES_H


# include "windefs.h"

HANDLE WINAPI MyFindFirstFileA(LPCTSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData);
int WINAPI MyFindNextFileA(HANDLE hFindFile, LPWIN32_FIND_DATAA lpFindFileData);
HANDLE WINAPI MyFindFirstFileW(LPCTSTR lpFileName, LPWIN32_FIND_DATAW lpFindFileData);
int WINAPI MyFindNextFileW(HANDLE hFindFile, LPWIN32_FIND_DATAW lpFindFileData);

typedef BOOL(WINAPI *FNFW)(HANDLE hFindFile, LPWIN32_FIND_DATAW lpFindFileData);

#endif