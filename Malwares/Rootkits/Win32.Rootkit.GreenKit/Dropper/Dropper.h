#pragma once

#include "resource.h"
#include <atlstr.h>

int create_shortcut(CString filepath, char* shortcut_path);
void CreateDesktopShortcut(CString strLinkName, CString strPath);
HRESULT CreateLink(LPCWSTR lpszPathObj, LPCSTR lpszPathLink, LPCWSTR lpszDesc);