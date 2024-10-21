//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// KBot project.
//	
// module: kbotinc.h
// $Revision: 35 $
// $Date: 2012-08-23 16:20:51 +0400 (Чт, 23 авг 2012) $
// description: 
//	Project common structures and definitions.


#define			szGet			"GET"

// from kbot.c

PCHAR	g_KbotPublicKey;
PVOID	KBotAlloc(ULONG	Size);
VOID	KBotFree(PVOID	Mem);

// from sendrecv.c
NTSTATUS KBotRequest(PCHAR	Host, PCHAR	Uri, PCHAR	Method, PCHAR*	pBuffer, PULONG	pSize);

// from tasks.c
NTSTATUS ProcessTaskFile(PCHAR Buffer, ULONG bSize, BOOL bMustSucceed);

// from config.c
PKBOT_CONFIG volatile	g_KBotConfig;
KBOT_USER				g_KBotUserId;

NTSTATUS	KBotLoadConfig(VOID);
VOID		KBotGetUserId(VOID);

