#ifndef __IODEV_H_
#define __IODEV_H_

NTSTATUS 
	KipDriverEntry( 
		IN PDRIVER_OBJECT DriverObject,
		IN PUNICODE_STRING RegistryPath
		);

#endif // __IODEV_H_