#include "stdafx.h"
#include <windows.h>
#include "Dropper.h"
#include <shlobj.h>
#include <iostream>
#include <atlstr.h>
#include "shobjidl.h"
#include <shlguid.h>

/* Can be used to create shortcuts to files and directories.

Libraries required: libuuid, libole32
Remember to include the .lnk in the shortcut path.
*/
// %userprofile%\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup
int create_shortcut(CString filepath, char* shortcut_path)
{
	CoInitialize(NULL);
	IShellLink* pShellLink = NULL;
	HRESULT hres;
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_ALL,
		IID_IShellLink, (void**)&pShellLink);
	std::cout << std::hex << hres << std::endl;
	if (SUCCEEDED(hres))
	{
		pShellLink->SetPath(filepath);  // Path to the object we are referring to
		CString myfol = "Folder";
		pShellLink->SetDescription(myfol);
		pShellLink->SetIconLocation(filepath, 0);

		IPersistFile *pPersistFile;
		hres = pShellLink->QueryInterface(IID_IPersistFile, (void**)&pPersistFile);

		if (SUCCEEDED(hres))
		{
			// Ensure that the string is Unicode
			wchar_t wsz[MAX_PATH];
			//mbstowcs(wsz, shortcut_path, MAX_PATH);
			MultiByteToWideChar(CP_ACP, 0, shortcut_path, -1, wsz, MAX_PATH);
			MessageBoxA(NULL, "coucou mon  petit", NULL, NULL);
			/* Add code here to check return value of MultiByteToWideChar for success ... shouldn't ever fail.*/

			hres = pPersistFile->Save(wsz, TRUE);

			pPersistFile->Release();
		}
		else
		{
			std::cout << "Error 2" << std::endl;
			CoUninitialize();
			return 2;
		}
		pShellLink->Release();
		CoUninitialize();
	}
	else
	{
		std::cout << "Error 1" << std::endl;
		CoUninitialize();
		return 1;
	}
	return 0;
}

void CreateDesktopShortcut(CString strLinkName, CString strPath)
{
	LPITEMIDLIST pItemList = NULL;

	HRESULT hResult = SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &pItemList);
	if (SUCCEEDED(hResult))
	{
		CoInitialize(NULL);
		char szPath[_MAX_PATH];
		SHGetPathFromIDList(pItemList, (wchar_t*)szPath);
		MessageBoxA(NULL, szPath, NULL, NULL);
		CComPtr<IShellLink> pShellLink = NULL;
		hResult = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID *)&pShellLink);
		if (SUCCEEDED(hResult))
		{
			MessageBoxA(NULL, "MERDE", NULL, NULL);
			CComPtr<IPersistFile> pPersistFile = NULL;
			pShellLink->SetPath(strPath);
			hResult = pShellLink->QueryInterface(IID_IPersistFile, reinterpret_cast<void**>(&pPersistFile));
			if (SUCCEEDED(hResult))
			{
				USES_CONVERSION;
				strLinkName = "C:\\yolo.lnk";//(CString)szPath + "\\" + strLinkName + ".lnk";
				hResult = pPersistFile->Save(strLinkName, TRUE);
				MessageBoxA(NULL, "coucou mon  petit", NULL, NULL);
			}
		}
	}
}

HRESULT CreateLink(LPCWSTR lpszPathObj, LPCSTR lpszPathLink, LPCWSTR lpszDesc)
{
	HRESULT hres;
	IShellLink* psl;

	// Get a pointer to the IShellLink interface. It is assumed that CoInitialize
	// has already been called.
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);
	if (SUCCEEDED(hres))
	{
		IPersistFile* ppf;

		// Set the path to the shortcut target and add the description. 
		psl->SetPath(lpszPathObj);
		psl->SetDescription(lpszDesc);

		// Query IShellLink for the IPersistFile interface, used for saving the 
		// shortcut in persistent storage. 
		hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);

		if (SUCCEEDED(hres))
		{
			WCHAR wsz[MAX_PATH];

			// Ensure that the string is Unicode. 
			MultiByteToWideChar(CP_ACP, 0, lpszPathLink, -1, wsz, MAX_PATH);

			// Add code here to check return value from MultiByteWideChar 
			// for success.

			// Save the link by calling IPersistFile::Save. 
			hres = ppf->Save(wsz, TRUE);
			ppf->Release();
		}
		psl->Release();
	}
	return hres;
}