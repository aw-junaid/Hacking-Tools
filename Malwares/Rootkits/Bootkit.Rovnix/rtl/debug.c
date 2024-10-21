#include "project.h"

#include <stdio.h>
#include <stdarg.h>
#include <inaddr.h>
#include "debug.h"
#include "kipioctl.h"

#define MSG_MAX_LEN 512

#if DBG
static DBG_LEVEL g_DebugLevel = DbgLevelError;
#else
static DBG_LEVEL g_DebugLevel = DbgLevelError;
#endif

//////////////////////////////////////////////////////////////////////////
PCHAR TruncatePath(PCHAR filename)
{
	PCHAR temp,curr=filename;
	while(temp = strchr(curr,'\\'))
		curr = ++temp;
	return curr;
}

//////////////////////////////////////////////////////////////////////////
VOID WriteMsg2(PCHAR *pBuff, int *pLeft, PCHAR fmt, va_list argptr)
{
	int res;
	if (*pLeft < 0)
		return;
	res = _vsnprintf(*pBuff,*pLeft,fmt,argptr);
	if ( res >= 0 )	{
		*pBuff+=res;
		*pLeft-=res;
	} else {
		*pBuff[0] = 0;
		*pLeft = -1;
	}
}

//////////////////////////////////////////////////////////////////////////
VOID _cdecl WriteMsg(PCHAR *pBuff, int *pLeft, PCHAR fmt, ...)
{
	va_list args;

	va_start(args,fmt);
	WriteMsg2(pBuff,pLeft,fmt,args);
	va_end(args);
}
//////////////////////////////////////////////////////////////////////////
VOID __cdecl 
	DebugLog(
		DBG_LEVEL level,
		PCHAR file,
		ULONG line,
		PCHAR func,
		PCHAR fmt,...) //ANY_LEVEL
{
	DBG_LEVEL DbgLevel;
	PCHAR DbgLevelName;

	va_list args;
	PCHAR buff;
	int left,res,len;
	CHAR Msg[MSG_MAX_LEN];

	DbgLevel = level & DbgLevelMax;

	if( DbgLevel < g_DebugLevel)
		return;

	buff = Msg;
	left = MSG_MAX_LEN-1;
	
#define CASE_DBG_LEVEL(x,y) case x: DbgLevelName = y; goto write;

	switch(DbgLevel)
	{
		CASE_DBG_LEVEL(DbgLevelNone,"None");
		CASE_DBG_LEVEL(DbgLevelTrace,"Trace");
		CASE_DBG_LEVEL(DbgLevelDebug,"Debug");
		CASE_DBG_LEVEL(DbgLevelInfo,"Info");
		CASE_DBG_LEVEL(DbgLevelError,"Error");
		CASE_DBG_LEVEL(DbgLevelCriticalTrace,"CritTrace");
		CASE_DBG_LEVEL(DbgLevelCriticalDebug,"CritDebug");
		CASE_DBG_LEVEL(DbgLevelCriticalInfo,"CritInfo");
		CASE_DBG_LEVEL(DbgLevelCriticalError,"CritError");
		CASE_DBG_LEVEL(DbgLevelBeforeBsod,"BSOD");
	write:
		WriteMsg(&buff,&left,"[%s,",DbgLevelName);
		break;
	default:
		DbgLevelName = "Unknown";
		WriteMsg(&buff,&left,"[%s(%i),",DbgLevelName,DbgLevel);
		break;
	}

	WriteMsg(&buff,&left,KIP_DRIVER_NAMEA ": ");
	WriteMsg(&buff,&left,"%s() ",func);

	if ( DbgLevel >= DbgLevelDebug && file ){
		WriteMsg(&buff,&left,":%s:%i,",TruncatePath(file),line);
	}

	WriteMsg(&buff,&left,"t%X] ",PsGetCurrentThreadId());

	va_start(args,fmt);
	WriteMsg2(&buff,&left,fmt,args);
	va_end(args);

	Msg[MSG_MAX_LEN-1] = 0;
	len = strlen(Msg);

	DbgPrint("%s",Msg);
	return;
}
//////////////////////////////////////////////////////////////////////////
NTSTATUS 
	DbgQueryLevel(
		IN PUNICODE_STRING RegistryPath
		)
{
	NTSTATUS ntStatus;
	OBJECT_ATTRIBUTES ObjectAttributes;
	UNICODE_STRING ParamsKey;
	UNICODE_STRING ValueName;
	ULONG DefaultDebugLevel = DbgLevelError;
	UCHAR buffer[sizeof( KEY_VALUE_PARTIAL_INFORMATION ) + sizeof(ULONG)];
	PKEY_VALUE_PARTIAL_INFORMATION ValuePartialInfo = 
		(PKEY_VALUE_PARTIAL_INFORMATION)buffer;
	HANDLE hKey,hKeyParam;
	ULONG ReturnLength = 0;

	InitializeObjectAttributes( 
		&ObjectAttributes,
		RegistryPath,
		OBJ_CASE_INSENSITIVE,
		NULL,NULL
		);

	ntStatus = 
		ZwOpenKey( 
			&hKey, 
			KEY_READ, 
			&ObjectAttributes 
			);
	if ( !NT_SUCCESS ( ntStatus ) ){
		return ntStatus;
	}

	RtlInitUnicodeString( &ParamsKey, L"Parameters" );
	InitializeObjectAttributes( 
		&ObjectAttributes,
		&ParamsKey,
		OBJ_CASE_INSENSITIVE,
		hKey,NULL
		);
	ntStatus = 
		ZwOpenKey( 
			&hKeyParam, 
			KEY_READ, 
			&ObjectAttributes 
			);
	//Close parent key
	ZwClose( hKey );

	if ( !NT_SUCCESS ( ntStatus ) ){		
		return ntStatus;
	}

	//query debug level value
	RtlInitUnicodeString( &ValueName, VALUE_DEBUG_LEVEL );
	ntStatus = 
		ZwQueryValueKey( 
			hKeyParam,
			&ValueName,
			KeyValuePartialInformation,
			buffer,
			sizeof(buffer),
			&ReturnLength 
			);

	if ( NT_SUCCESS ( ntStatus ) ){
		if ( ValuePartialInfo->Type == REG_DWORD &&
			 ValuePartialInfo->DataLength == sizeof(DefaultDebugLevel))
		{
			DefaultDebugLevel = *((PULONG)ValuePartialInfo->Data);
		}
	}

	g_DebugLevel = DefaultDebugLevel;
	ZwClose( hKeyParam );
	return ntStatus;
}