//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BK driver loader project. Version 2.9.2.
//	
// module: crtsup.c
// $Revision: 39 $
// $Date: 2012-05-02 15:38:09 +0400 (Ср, 02 май 2012) $
// description: 
//	BK helper library. String support routines.
//	Since we do no use CRT while building for different architectures and subsystems, some string manipulation routines 
//   have to be defined here.


#include "main.h"
#include "crtsup.h"

/***
*strcmp - compare two strings, returning less than, equal to, or greater than
*
*Purpose:
*       STRCMP compares two strings and returns an integer
*       to indicate whether the first is less than the second, the two are
*       equal, or whether the first is greater than the second.
*
*       Comparison is done byte by byte on an UNSIGNED basis, which is to
*       say that Null (0) is less than any other character (1-255).
*
*Entry:
*       const char * src - string for left-hand side of comparison
*       const char * dst - string for right-hand side of comparison
*
*Exit:
*       returns -1 if src <  dst
*       returns  0 if src == dst
*       returns +1 if src >  dst
*
*Exceptions:
*
*******************************************************************************/

int __cdecl __strcmp (
        const char * src,
        const char * dst
        )
{
        int ret = 0 ;

        while( ! (ret = *(unsigned char *)src - *(unsigned char *)dst) && *dst)
                ++src, ++dst;

        if ( ret < 0 )
                ret = -1 ;
        else if ( ret > 0 )
                ret = 1 ;

        return( ret );
}


/***
*wcscmp - compare two wchar_t strings,
*        returning less than, equal to, or greater than
*
*Purpose:
*       wcscmp compares two wide-character strings and returns an integer
*       to indicate whether the first is less than the second, the two are
*       equal, or whether the first is greater than the second.
*
*       Comparison is done wchar_t by wchar_t on an UNSIGNED basis, which is to
*       say that Null wchar_t(0) is less than any other character.
*
*Entry:
*       const wchar_t * src - string for left-hand side of comparison
*       const wchar_t * dst - string for right-hand side of comparison
*
*Exit:
*       returns -1 if src <  dst
*       returns  0 if src == dst
*       returns +1 if src >  dst
*
*Exceptions:
*
*******************************************************************************/

int __cdecl __wcscmp (
        const wchar_t * src,
        const wchar_t * dst
        )
{
        int ret = 0 ;

        while( ! (ret = (int)(*src - *dst)) && *dst)
                ++src, ++dst;

        if ( ret < 0 )
                ret = -1 ;
        else if ( ret > 0 )
                ret = 1 ;

        return( ret );
}



int __cdecl __stricmp (
        const char * dst,
        const char * src
        )
{
    int f, l;

    do
    {
        if ( ((f = (unsigned char)(*(dst++))) >= 'A') && (f <= 'Z') )
            f -= 'A' - 'a';
        if ( ((l = (unsigned char)(*(src++))) >= 'A') && (l <= 'Z') )
            l -= 'A' - 'a';
    }
    while ( f && (f == l) );

    return(f - l);
}

int __cdecl __wcsicmp (
        const wchar_t * dst,
        const wchar_t * src
        )
{
	wchar_t f,l;
	
	do  {
		f = __ascii_towlower(*dst);
		l = __ascii_towlower(*src);
		dst++;
		src++;
	} while ( (f) && (f == l) );
	
	return (int)(f - l);
}


/***
*char *strchr(string, c) - search a string for a character
*
*Purpose:
*       Searches a string for a given character, which may be the
*       null character '\0'.
*
*Entry:
*       char *string - string to search in
*       char c - character to search for
*
*Exit:
*       returns pointer to the first occurence of c in string
*       returns NULL if c does not occur in string
*
*Exceptions:
*
*******************************************************************************/

char * __cdecl __strchr (
        const char * string,
        int ch
        )
{
        while (*string && *string != (char)ch)
                string++;

        if (*string == (char)ch)
                return((char *)string);
        return(NULL);
}



/***
*wchar_t *wcschr(string, c) - search a string for a wchar_t character
*
*Purpose:
*       Searches a wchar_t string for a given wchar_t character,
*       which may be the null character L'\0'.
*
*Entry:
*       wchar_t *string - wchar_t string to search in
*       wchar_t c - wchar_t character to search for
*
*Exit:
*       returns pointer to the first occurence of c in string
*       returns NULL if c does not occur in string
*
*Exceptions:
*
*******************************************************************************/

wchar_t * __cdecl __wcschr (
        const wchar_t * string,
        wchar_t ch
        )
{
        while (*string && *string != (wchar_t)ch)
                string++;

        if (*string == (wchar_t)ch)
                return((wchar_t *)string);
        return(NULL);
}



/***
*strlen - return the length of a null-terminated string
*
*Purpose:
*       Finds the length in bytes of the given string, not including
*       the final null character.
*
*Entry:
*       const char * str - string whose length is to be computed
*
*Exit:
*       length of the string "str", exclusive of the final null byte
*
*Exceptions:
*
*******************************************************************************/

size_t __cdecl __strlen (
        const char * str
        )
{
        const char *eos = str;

        while( *eos++ ) ;

        return( eos - str - 1 );
}