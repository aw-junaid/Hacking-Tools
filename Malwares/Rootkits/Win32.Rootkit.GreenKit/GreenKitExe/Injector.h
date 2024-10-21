#pragma once
#include "stdafx.h"
#include <Windows.h>

#define DLL_PATH "\\_greenkit_GreenKit.dll" // METTRE ICI LA DLL
#define CREATE_THREAD_ACCESS (PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ) 

BOOL HookProcess(HANDLE procName);
BOOL Inject(HANDLE hProc, LPSTR dllName);
BOOL InitPayload(HANDLE hProcess, LPCSTR lpPath, HMODULE hPayloadBase, HWND hwndDlg);
