#ifndef __NDIS_HOOK_H_
#define __NDIS_HOOK_H_

// public functions

NTSTATUS	NdisSetHooks(VOID);

VOID
	NdisReinstallMiniportHook(
		IN PKIP_NDIS_ADAPTER pAdapter
		);

#endif //__NDIS_HOOK_H_