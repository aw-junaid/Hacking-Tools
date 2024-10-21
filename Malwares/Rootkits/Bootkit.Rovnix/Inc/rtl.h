#ifndef __RTL_H_
#define __RTL_H_

//////////////////////////////////////////////////////////////////////////
// OS VERSION

#define NT351           0x0421 //1057
#define NT4FINAL        0x0565 //1381
#define WIN2K           0x0893 //2195
#define WINXP           0x0A28 //2600
#define NETRC1          0x0E4F //3663
#define NETRC2          0x0E86 //3718
#define WIN2K3          0x0ECE //3790
#define WIN2K8          0x1771 //6001
#define WIN2K8SP2       0x1772 //6002
#define WIN7            0x1DB0 //7600

VOID
	OsGetVersion(
		VOID
		);

ULONG
	OsGetMajorVersion(
		VOID
		);

ULONG
	OsGetMinorVersion(
		VOID
		);

ULONG
	OsGetServicePack(
		VOID
		);

ULONG
	OsGetBuildNumber(
		VOID
		);

//////////////////////////////////////////////////////////////////////////
// REGISTY INTERFACE

NTSTATUS
	RegOpenKey(
		IN PHANDLE hKey,
		IN PWCHAR szKeyName
		);

NTSTATUS
	RegOpenKeyW(
		IN PHANDLE hKey,
		IN PUNICODE_STRING usKeyName
		);

NTSTATUS
	RegGenericValue(
		IN HANDLE hKey,
		IN PWCHAR ValueName,
		IN PKEY_VALUE_PARTIAL_INFORMATION *ValueData,
		IN PUSHORT ValueSize
		);

NTSTATUS
	RegReadDword(
		IN HANDLE hKey,
		IN PWCHAR szValueName,
		OUT PULONG ValueData
		);

NTSTATUS
	RegReadStringValue(
		IN HANDLE hKey,
		IN PWCHAR szValueName,
		OUT PUNICODE_STRING ValueData
		);

NTSTATUS
	RegReadMultiSZValue(
		IN HANDLE hKey,
		IN PWCHAR szValueName,
		OUT PUNICODE_STRING  ValueData
		);

NTSTATUS
	RegReadStringFromMultiSZ(
		IN HANDLE hKey,
		IN PWCHAR szValueName,
		OUT PUNICODE_STRING ValueData
		);

//////////////////////////////////////////////////////////////////////////
// STRING FUNCTION

VOID 
	RtlUnicodeStringToAnsiStringInplace(
		IN PUNICODE_STRING uString, 
		IN PANSI_STRING aString 
		);

WCHAR RtxUpcaseUnicodeChar(WCHAR wc);
CHAR RtxUpcaseChar(CHAR c);

BOOLEAN
	RtxFindSubstring (
		IN CHAR* Source,
		IN CHAR* Pattern,
		IN BOOLEAN	bCaseInsensitive
		);

NTSTATUS
	RtxFindCharInUnicodeStringWithShift(
		IN PUNICODE_STRING	Source,
		IN OUT PUNICODE_STRING	Dest,
		IN WCHAR		Chr,
		IN ULONG		Shift,
		IN BOOLEAN		CaseInSensitive
		);

//////////////////////////////////////////////////////////////////////////
// errno to ntstatus

NTSTATUS RtlErroNoToStatus( int ErrNo );

#endif //__RTL_H_