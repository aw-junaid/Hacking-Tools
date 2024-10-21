
#ifndef __RK_DEFENSE_H__
#define __RK_DEFNESE_H__

int SetupFakeValueMap( HANDLE pHandle, HANDLE hKey );

ULONG GetNumberOfValues( HANDLE hKey );
ULONG GetNumberOfSubkeys( HANDLE hKey );

void FreeTrackHandle( HANDLE theHandle );
ULONG GetRegValueMapping( HANDLE hKey, ULONG realIndex);
ULONG GetRegSubkeyMapping( HANDLE hKey, ULONG realIndex);

void WatchProcessHandle( HANDLE hFile );
HANDLE CheckForRedirectedFile( HANDLE hFile );
void SetTrojanRedirectFile( HANDLE hFile );


void InitDefenseSystem();

#endif