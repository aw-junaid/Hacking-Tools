//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BK driver loader project. Version 2.9.2.
//	
// module: crtsup.h
// $Revision: 39 $
// $Date: 2012-05-02 15:38:09 +0400 (Ср, 02 май 2012) $
// description: 
//	BK helper library. String support routines.
//	Since we do no use CRT while building for different architectures and subsystems, some string manipulation routines 
//   have to be defined here.


#define __ascii_tolower(c)      ( (((c) >= 'A') && ((c) <= 'Z')) ? ((c) - 'A' + 'a') : (c) )
#define __ascii_toupper(c)      ( (((c) >= 'a') && ((c) <= 'z')) ? ((c) - 'a' + 'A') : (c) )
#define __ascii_towlower(c)     ( (((c) >= L'A') && ((c) <= L'Z')) ? ((c) - L'A' + L'a') : (c) )
#define __ascii_towupper(c)     ( (((c) >= L'a') && ((c) <= L'z')) ? ((c) - L'a' + L'A') : (c) )

size_t __cdecl __strlen (const char * str);
int __cdecl __strcmp (const char * src, const char * dst);
int __cdecl __wcscmp (const wchar_t * src, const wchar_t * dst);
int __cdecl __stricmp (const char * dst, const char * src);
int __cdecl __wcsicmp (const wchar_t * dst, const wchar_t * src);
char * __cdecl __strchr (const char * string, int ch);
wchar_t * __cdecl __wcschr (const wchar_t * string, wchar_t ch);


#ifdef _UNICODE
	#define	_lstrcmp	__wcscmp
	#define _lstrcmpi	__wcsicmp
#else
	#define _lstrcmp	__strcmp	
	#define _lstrcmpi	__stricmp
#endif