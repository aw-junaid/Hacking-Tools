#include "stdafx.h"
#include "Injector.h"

BOOL fileExists(TCHAR * file)
{
    WIN32_FIND_DATA FindFileData;
    HANDLE handle = FindFirstFile(file, &FindFileData);
    BOOL found = handle != INVALID_HANDLE_VALUE;
    if (found)
        FindClose(handle);
    return found;
}

BOOL HookProcess(HANDLE procName)
{
    TCHAR currentDir[MAX_PATH];
    TCHAR dllDir[MAX_PATH];
    BOOL isSuccessful = TRUE;
    GetCurrentDirectory(MAX_PATH, currentDir);

    strcpy(dllDir, currentDir);
    strcat(dllDir, DLL_PATH);
    
    if (!fileExists(dllDir))
        return FALSE;

    return Inject(procName, dllDir);
}

//use it with procName
BOOL Inject(HANDLE hProc, LPSTR dllName)
{
    if (!hProc)
        return FALSE;

    if (!dllName)
        return FALSE;
    
   TCHAR DLL_NAME[MAX_PATH] = {0}; 
   GetFullPathName(dllName, MAX_PATH, DLL_NAME, NULL); 
   HANDLE hThread = 0;
   HMODULE hLib = 0; 
   HMODULE hInjected;
   LPVOID RemoteString, LoadLibAddy; 

   LoadLibAddy = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA"); 

   // Allocate space in the target process for our DLL
   RemoteString = (LPVOID)VirtualAllocEx(hProc, NULL, strlen(DLL_NAME), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

   // Write the string name of our <strong class="highlight">DLL</strong> in the memory allocated 
   WriteProcessMemory(hProc, (LPVOID)RemoteString, DLL_NAME, strlen(DLL_NAME), NULL);

   // Load our DLL
   hThread = CreateRemoteThread(hProc, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibAddy, (LPVOID)RemoteString, NULL, NULL);

   // Locate address our payload was loaded
   if (hThread != 0) {
       WaitForSingleObject(hThread, INFINITE);
       GetExitCodeThread(hThread, (LPDWORD)&hInjected);
       CloseHandle(hThread);
   } 
   
   return TRUE; 
}

LPVOID GetPayloadExportAddr(LPCSTR lpPath, HMODULE hPayloadBase, LPCSTR lpFunctionName)
{
    // Load payload in our own virtual address space
    HMODULE hLoaded = LoadLibrary(lpPath);

    if (!hLoaded)
        return NULL;
    else
    {
        //Now we use GetProcAddress to get the address of the exported function
        void* lpFunc = GetProcAddress(hLoaded, lpFunctionName);
        //We get the address of the offset of the function from the base of the DLL
        DWORD dwOffset = (char*)lpFunc - (char*)hLoaded;

        FreeLibrary(hLoaded);
        //Adding this will tell us where to call the CreateRemoteThread
        DWORD ret = (DWORD) hPayloadBase + dwOffset;
        return (LPVOID)&ret;
    }
}

BOOL InitPayload(HANDLE hProcess, LPCSTR lpPath, HMODULE hPayloadBase, HWND hwndDlg)
{
    void* lpInit = GetPayloadExportAddr(lpPath, hPayloadBase, "InitGreenKit"); //Put name of our function to call here
    if (!lpInit)
        return FALSE;
    else
    {
        HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0,
            (LPTHREAD_START_ROUTINE)lpInit, hwndDlg, 0, NULL);

        if (!hThread)
            return FALSE;
        else
            CloseHandle(hThread);
    }

    return TRUE;
}

BOOL CleanupPayload(HANDLE hProcess, LPCSTR lpPath, HMODULE hPayloadBase) {
    void* lpCleanup = GetPayloadExportAddr(lpPath, hPayloadBase, "Cleanup");
    if (!lpCleanup)
        return FALSE;
    else
    {
        HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0,
            (LPTHREAD_START_ROUTINE) lpCleanup, NULL, 0, NULL);

        if (!hThread)
            return FALSE;
        else
            CloseHandle(hThread);   
    }
    return TRUE;
}

