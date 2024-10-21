#include "project.h"

VOID RtlUnicodeStringToAnsiStringInplace(IN PUNICODE_STRING uString, IN PANSI_STRING aString )
{
	unsigned int i = 0;
	aString->Buffer = (char*)uString->Buffer;
	aString->Length = uString->Length / sizeof(WCHAR);
	aString->MaximumLength = uString->MaximumLength;
	for ( i = 0; i < aString->Length; ++i )
	{
		aString->Buffer[i] = (char)uString->Buffer[i];
	}
	aString->Buffer[aString->Length] = 0;
}

//***********************************************************************************
// Name: RtxUpcaseUnicodeChar
//
// Description: 
//
// Return value: 
//
// NOTE: None
// **********************************************************************************
WCHAR RtxUpcaseUnicodeChar(WCHAR wc)
{
	if ((wc >= 'a')&&(wc <= 'z'))
		return (wc-0x20);
	return wc;
}
CHAR RtxUpcaseChar(CHAR c)
{
	if ((c >= 'a')&&(c <= 'z'))
		return (c-0x20);
	return c;
}
//***********************************************************************************
// Name: RtxFindSubstring
//
// Description: find substring (case insensitive )
//
// Return value: status
//
//
// NOTE: None
// **********************************************************************************
BOOLEAN
	RtxFindSubstring (
		IN CHAR* Source,
		IN CHAR* Pattern,
		IN BOOLEAN	bCaseInsensitive
		)
{
	unsigned i,j;
	CHAR *s1, *s2;
	CHAR *cp = Source;

	if ( !Pattern  )
		return FALSE;

	if ( !Source ){
		return FALSE;
	}

	while ( *cp )
	{
		CHAR SourceChar;
		CHAR PatternChar;

		s1 = cp;
		s2 = Pattern;

		while ( *s1 && *s2 ){
			SourceChar = *s1;
			PatternChar = *s2;
			if (bCaseInsensitive) {
				SourceChar = RtxUpcaseChar(SourceChar);
				PatternChar = RtxUpcaseChar(PatternChar);
			}
			if ( SourceChar != PatternChar ){
				break;
			}
			s1++, s2++;
		}

		if ( 0 == *s2 ){

			return TRUE;
		}

		cp++;
	}

	return FALSE;
}
//////////////////////////////////////////////////////////////////////////
NTSTATUS
	RtxFindCharInUnicodeStringWithShift(
		IN PUNICODE_STRING	Source,
		IN OUT PUNICODE_STRING	Dest,
		IN WCHAR		Chr,
		IN ULONG		Shift,
		IN BOOLEAN		CaseInSensitive
		)
{
	NTSTATUS ntStatus = STATUS_NOT_FOUND;
	PWSTR    szBuffer;
	WCHAR    TstChar;
	WCHAR    UCaseChr = CaseInSensitive ? RtxUpcaseUnicodeChar( Chr ) : Chr;

	/*
	* Sanity checks
	*/
	if ( !Source || !Dest )
		return STATUS_INVALID_PARAMETER;

	if ( Source->Length < Shift*sizeof(WCHAR) || 
		!Source->Buffer ||
		Source->MaximumLength < Shift*sizeof(WCHAR) )
		return ntStatus;

	szBuffer = Source->Buffer + Shift;
	while ( szBuffer < Source->Buffer + Source->Length / 2 ){
		TstChar = CaseInSensitive ? RtxUpcaseUnicodeChar( *szBuffer ) : *szBuffer;
		if ( TstChar == UCaseChr ){
			break;
		}
		szBuffer++;
	}

	if ( szBuffer < Source->Buffer + Source->Length / 2 ){
		szBuffer ++;
		Dest->Buffer = szBuffer;
		Dest->Length = Source->Length +
			(SHORT)((PUCHAR)Source->Buffer - (PUCHAR)szBuffer);
		Dest->MaximumLength = Dest->Length;
		ntStatus = STATUS_SUCCESS;
	}
	return ntStatus;
}
