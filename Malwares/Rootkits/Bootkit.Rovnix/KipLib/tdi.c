//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Module: tdi.c
//	Author: Zufyxe, July 2010
//	Description:
//	 TDI based socket interface.
//

#include "project.h"
#include "lwip\sockets.h"
#include "lwip\netdb.h"

#ifdef _TDI_SUPPORT

#define		szDeviceTcp		L"\\Device\\Tcp"
#define		szDeviceUdp		L"\\Device\\Udp"

UNICODE_STRING		g_TcpDeviceName = RTL_CONSTANT_STRING(szDeviceTcp);
UNICODE_STRING		g_UdpDeviceName = RTL_CONSTANT_STRING(szDeviceUdp);


NTSTATUS	
	TdiCreateConnection(
		PHANDLE			Handle, 
		PFILE_OBJECT	*ConnectionObject
		)
{
	NTSTATUS			ntStatus;
	OBJECT_ATTRIBUTES	Attr; 
	IO_STATUS_BLOCK		IoStatus;
	CHAR				Buffer[sizeof(FILE_FULL_EA_INFORMATION) + TDI_CONNECTION_CONTEXT_LENGTH + 300] = {0};
	PFILE_FULL_EA_INFORMATION Ea = (PFILE_FULL_EA_INFORMATION)&Buffer;

	InitializeObjectAttributes(&Attr, &g_TcpDeviceName, OBJ_CASE_INSENSITIVE  | OBJ_KERNEL_HANDLE, 0, 0);

	Ea->NextEntryOffset = 0;
	Ea->Flags = 0;
	Ea->EaNameLength = TDI_CONNECTION_CONTEXT_LENGTH;
	Ea->EaValueLength = TDI_CONNECTION_CONTEXT_LENGTH;
	RtlCopyMemory(Ea->EaName, TdiConnectionContext, TDI_CONNECTION_CONTEXT_LENGTH);

	ntStatus = 
		ZwCreateFile(
			Handle, 
			FILE_READ_EA | FILE_WRITE_EA, 
			&Attr, 
			&IoStatus, 
			0, FILE_ATTRIBUTE_NORMAL, 
			0, FILE_OPEN_IF, 0, 
			Ea, sizeof(Buffer)
			);

	if (NT_SUCCESS(ntStatus)) {
		ntStatus =  
			ObReferenceObjectByHandle(
				*Handle, 
				GENERIC_READ | GENERIC_WRITE, 
				0, KernelMode, 
				(PVOID *)ConnectionObject, 
				0
				);
	}

	return(ntStatus);
}


NTSTATUS
	TdiCreateAddress(
		PHANDLE			Handle, 
		PFILE_OBJECT	*AddressObject,
		ULONG			Type,
		ULONG			Address,
		USHORT			Port
		)
{
	CHAR Buffer[sizeof (FILE_FULL_EA_INFORMATION) + TDI_TRANSPORT_ADDRESS_LENGTH + sizeof(TA_IP_ADDRESS)];
	NTSTATUS	ntStatus;
	OBJECT_ATTRIBUTES	Attr; 
	IO_STATUS_BLOCK		IoStatus;
	PTA_IP_ADDRESS		Sin;
	PFILE_FULL_EA_INFORMATION Ea = (PFILE_FULL_EA_INFORMATION)&Buffer;

	switch(Type)
	{
	case SOCK_STREAM:
		InitializeObjectAttributes(&Attr, &g_TcpDeviceName, OBJ_CASE_INSENSITIVE  | OBJ_KERNEL_HANDLE, 0, 0);
		break;
	case SOCK_DGRAM:
		InitializeObjectAttributes(&Attr, &g_UdpDeviceName, OBJ_CASE_INSENSITIVE  | OBJ_KERNEL_HANDLE, 0, 0);
		break;
	default:
		return(STATUS_INVALID_PARAMETER);
		break;
	}

	Ea->NextEntryOffset = 0;
	Ea->Flags = 0;
	Ea->EaNameLength = TDI_TRANSPORT_ADDRESS_LENGTH;
	Ea->EaValueLength = sizeof (TA_IP_ADDRESS);
	RtlCopyMemory(Ea->EaName, TdiTransportAddress, Ea->EaNameLength + 1);

	Sin = (PTA_IP_ADDRESS)(Ea->EaName + Ea->EaNameLength + 1);
	Sin->TAAddressCount = 1;
	Sin->Address[0].AddressLength = TDI_ADDRESS_LENGTH_IP;
	Sin->Address[0].AddressType = TDI_ADDRESS_TYPE_IP;
	Sin->Address[0].Address[0].sin_port = Port;
	Sin->Address[0].Address[0].in_addr = Address;
	RtlZeroMemory(Sin->Address[0].Address[0].sin_zero, sizeof Sin->Address[0].Address[0].sin_zero);
	
	ntStatus = ZwCreateFile(Handle, 0, &Attr, &IoStatus, 0, FILE_ATTRIBUTE_NORMAL, 0, FILE_OPEN, 0, Ea, sizeof(Buffer));

	if (NT_SUCCESS(ntStatus))
		ntStatus = ObReferenceObjectByHandle(*Handle, GENERIC_READ | GENERIC_WRITE, 0, KernelMode, (PVOID *)AddressObject, 0);

	return(ntStatus);
}



NTSTATUS
	TdiSetEventHandler(
		PFILE_OBJECT	AddressObject,
		LONG			EventType, 
		PVOID			EventHandler,
		PVOID			EventContext
		)
{
	NTSTATUS	ntStatus;
    KEVENT		Event; 
	PIRP		Irp;
    PDEVICE_OBJECT	DeviceObject = IoGetRelatedDeviceObject(AddressObject);
	IO_STATUS_BLOCK IoStatus;
    
	KeInitializeEvent(&Event, NotificationEvent, FALSE);

    if (!(Irp = TdiBuildInternalDeviceControlIrp(TDI_SET_EVENT_HANDLER, DeviceObject, AddressObject, &Event, &IoStatus)))
		return STATUS_INSUFFICIENT_RESOURCES;

    TdiBuildSetEventHandler(Irp, DeviceObject, AddressObject, 0, 0, EventType, EventHandler, EventContext);

    ntStatus = IoCallDriver(DeviceObject, Irp);

    if (ntStatus == STATUS_PENDING)
        ntStatus = KeWaitForSingleObject(&Event, UserRequest, KernelMode, FALSE, 0);

    return(ntStatus == STATUS_SUCCESS ? IoStatus.Status : ntStatus);
}


NTSTATUS
	TdiBind(
		PFILE_OBJECT	ConnectionObject, 
		HANDLE			Address
		)
{
	NTSTATUS	ntStatus;
    KEVENT		Event; 
	PIRP		Irp;
    PDEVICE_OBJECT	DeviceObject = IoGetRelatedDeviceObject(ConnectionObject);
    IO_STATUS_BLOCK IoStatus;

	KeInitializeEvent(&Event, NotificationEvent, FALSE);

    if (!(Irp = TdiBuildInternalDeviceControlIrp(TDI_ASSOCIATE_ADDRESS, DeviceObject, ConnectionObject, &Event, &IoStatus)))
		return STATUS_INSUFFICIENT_RESOURCES;

    TdiBuildAssociateAddress(Irp, DeviceObject, ConnectionObject, 0, 0, Address);

    ntStatus = IoCallDriver(DeviceObject, Irp);

    if (ntStatus == STATUS_PENDING)
        ntStatus = KeWaitForSingleObject(&Event, UserRequest, KernelMode, FALSE, 0);

    return(ntStatus == STATUS_SUCCESS ? IoStatus.Status : ntStatus);

}


NTSTATUS
	TdiQueryAddress(
		PFILE_OBJECT		AddressObject, 
		PTDI_ADDRESS_INFO	AddressInfo,
		PULONG				pInfoLength
		)
{
	NTSTATUS	ntStatus;
	PIRP		Irp;
	PMDL		Mdl;
    PDEVICE_OBJECT	DeviceObject = IoGetRelatedDeviceObject(AddressObject);
    IO_STATUS_BLOCK IoStatus;

    if (!(Irp = TdiBuildInternalDeviceControlIrp(TDI_QUERY_INFORMATION, DeviceObject, FileObject, NULL, &IoStatus)))
		return STATUS_INSUFFICIENT_RESOURCES;

	if (!(Mdl = IoAllocateMdl(AddressInfo, *pInfoLength, FALSE, FALSE, Irp)))
	{
		IoFreeIrp(Irp);
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	MmProbeAndLockPages(Mdl, KernelMode, IoModifyAccess);

    TdiBuildQueryInformation(Irp, DeviceObject, AddressObject, NULL, NULL, TDI_QUERY_ADDRESS_INFO, Mdl);

    ntStatus = IoCallDriver(DeviceObject, Irp);

    return(ntStatus == STATUS_SUCCESS ? IoStatus.Status : ntStatus);

}

NTSTATUS
	TdiConnect(
		PFILE_OBJECT		ConnectionObject, 
		ULONG				Addr, 
		USHORT				Port,
		PIO_STATUS_BLOCK	AsyncIoStatus
		)
{
	NTSTATUS	ntStatus;
	PIRP		Irp;
  	PDEVICE_OBJECT	DeviceObject = IoGetRelatedDeviceObject(ConnectionObject);
    TA_IP_ADDRESS	DstAddr = {1, {TDI_ADDRESS_LENGTH_IP, TDI_ADDRESS_TYPE_IP, {Port, Addr}}};
    TDI_CONNECTION_INFORMATION Dst = {0, 0, 0, 0, sizeof DstAddr, &DstAddr};

    KEVENT		Event; 
	IO_STATUS_BLOCK IoStatus = {0, (ULONG_PTR)&Event};
	PIO_STATUS_BLOCK	pIoStatus = &IoStatus;	

	if (AsyncIoStatus)
		pIoStatus = AsyncIoStatus;
	else
		KeInitializeEvent(&Event, NotificationEvent, FALSE);


    if (!(Irp = TdiBuildInternalDeviceControlIrp(TDI_CONNECT, DeviceObject, ConnectionObject, (PKEVENT)pIoStatus->Information, pIoStatus)))
		return STATUS_INSUFFICIENT_RESOURCES;

    TdiBuildConnect(Irp, DeviceObject, ConnectionObject, 0, 0, 0, &Dst, 0);

    ntStatus = IoCallDriver(DeviceObject, Irp);

    if ((ntStatus == STATUS_PENDING) && (!AsyncIoStatus))
		ntStatus = KeWaitForSingleObject(&Event, UserRequest, KernelMode, FALSE, 0);

    return(ntStatus);

}

NTSTATUS
	TdiDisconnect(
		PFILE_OBJECT ConnectionObject
		)
{
	NTSTATUS	ntStatus;
    KEVENT		Event; 
	PIRP		Irp;
    PDEVICE_OBJECT DeviceObject = IoGetRelatedDeviceObject(ConnectionObject);
    IO_STATUS_BLOCK IoStatus;

	KeInitializeEvent(&Event, NotificationEvent, FALSE);

    if (!(Irp = TdiBuildInternalDeviceControlIrp(TDI_DISCONNECT, DeviceObject, ConnectionObject, &Event, &IoStatus)))
		return STATUS_INSUFFICIENT_RESOURCES;

    TdiBuildDisconnect(Irp, DeviceObject, ConnectionObject, 0, 0, 0, TDI_DISCONNECT_RELEASE, 0, 0);

    ntStatus = IoCallDriver(DeviceObject, Irp);

    if (ntStatus == STATUS_PENDING)
        ntStatus = KeWaitForSingleObject(&Event, UserRequest, KernelMode, FALSE, 0);

    return(ntStatus == STATUS_SUCCESS ? IoStatus.Status : ntStatus);

}

NTSTATUS
	TdiSend(
		PFILE_OBJECT ConnectionObject, 
		PVOID		Data, 
		ULONG		Length
		)
{
	NTSTATUS	ntStatus;
    KEVENT	Event; 
	PIRP	Irp;
	PMDL	Mdl;
    PDEVICE_OBJECT	DeviceObject = IoGetRelatedDeviceObject(ConnectionObject);
    IO_STATUS_BLOCK IoStatus;
   

	KeInitializeEvent(&Event, NotificationEvent, FALSE);

    if (!(Irp = TdiBuildInternalDeviceControlIrp(TDI_SEND, DeviceObject, ConnectionObject, &Event, &IoStatus)))
		return STATUS_INSUFFICIENT_RESOURCES;

    if (!(Mdl = IoAllocateMdl(Data, Length, FALSE, FALSE, Irp)))
	{
		IoFreeIrp(Irp);
		return STATUS_INSUFFICIENT_RESOURCES;
	}

    MmProbeAndLockPages(Mdl, KernelMode, IoModifyAccess);
    TdiBuildSend(Irp, DeviceObject, ConnectionObject, 0, 0, Mdl, 0, Length);

	ntStatus = IoCallDriver(DeviceObject, Irp);

    if (ntStatus == STATUS_PENDING)
        ntStatus = KeWaitForSingleObject(&Event, UserRequest, KernelMode, FALSE, 0);

	return(ntStatus == STATUS_SUCCESS ? IoStatus.Status : ntStatus);

}

NTSTATUS
	TdiRecv(
		PFILE_OBJECT	ConnectionObject, 
		PVOID			Data, 
		ULONG			Length
		)
{
	NTSTATUS	ntStatus;
	IO_STATUS_BLOCK IoStatus;
	KEVENT			Event;
    PDEVICE_OBJECT DeviceObject = IoGetRelatedDeviceObject(ConnectionObject);
    PIRP	Irp;
	PMDL	Mdl;

	KeInitializeEvent(&Event, NotificationEvent, FALSE);

    if (!(Irp = TdiBuildInternalDeviceControlIrp(TDI_RECEIVE, DeviceObject, ConnectionObject, &Event, &IoStatus)))
		return STATUS_INSUFFICIENT_RESOURCES;

    if (!(Mdl = IoAllocateMdl(Data, Length, FALSE, FALSE, Irp)))
	{
		IoFreeIrp(Irp);
		return STATUS_INSUFFICIENT_RESOURCES;
	}

    MmProbeAndLockPages(Mdl, KernelMode, IoModifyAccess);

    TdiBuildReceive(Irp, DeviceObject, ConnectionObject, 0, 0, Mdl, TDI_RECEIVE_NORMAL, Length);

	ntStatus = IoCallDriver(DeviceObject, Irp);

	if (ntStatus == STATUS_PENDING)
		ntStatus = KeWaitForSingleObject(&Event, UserRequest, KernelMode, FALSE, 0);

	return(ntStatus == STATUS_SUCCESS ? IoStatus.Status : ntStatus);

}

NTSTATUS
	TdiEventDisconnect(
		PVOID	EventContext, 
		CONNECTION_CONTEXT cContext, 
		LONG	Unused0, 
		PVOID	Unused1, 
		LONG	Unused2, 
		PVOID	Unused3, 
		ULONG	Flags
		)
{
    //DbgPrint("Flags = %lx\n", Flags);
    KeSetEvent((PKEVENT)EventContext, 0, FALSE);

	UNREFERENCED_PARAMETER(Unused0);
    return STATUS_SUCCESS;

}

NTSTATUS
	TdiEventError(
		PVOID		Unused0, 
		NTSTATUS	Status
		)
{
    DbgPrint("Error = %lx\n", Status);
    return STATUS_SUCCESS;
}

NTSTATUS
	TdiEventReceive(
		PVOID	Unused0, 
		CONNECTION_CONTEXT Unused1, 
		ULONG	Flags, 
		ULONG	Indicated, 
		ULONG	Available, 
		PULONG	Taken, 
		PVOID	Unused2, 
		PIRP	*Irp
		)
{
//    DbgPrint("Receive Flags = %lx, Ind = %ld, Avl = %ld\n", Flags, Indicated, Available);
    *Taken = Available; *Irp = 0;
    return STATUS_SUCCESS;
}


NTSTATUS
	TdiSendto(
		PFILE_OBJECT AddressObject, 
		PVOID		Data, 
		ULONG		Length,
		ULONG		Addr,
		USHORT		Port,
		PKEVENT		WriteEvent
		)
{
    KEVENT	Event; 
	PIRP	Irp;
	PMDL	Mdl;
	NTSTATUS		ntStatus;
    PDEVICE_OBJECT	DeviceObject = IoGetRelatedDeviceObject(AddressObject);
    IO_STATUS_BLOCK IoStatus;
	TA_IP_ADDRESS	DstAddr = {1, {TDI_ADDRESS_LENGTH_IP, TDI_ADDRESS_TYPE_IP, {Port, Addr}}};
    TDI_CONNECTION_INFORMATION Dst = {0, 0, 0, 0, sizeof DstAddr, &DstAddr};
	PKEVENT	pEvent = WriteEvent;

	if (!WriteEvent)
	{
		KeInitializeEvent(&Event, NotificationEvent, FALSE);
		pEvent = &Event;
	}

    if (!(Irp = TdiBuildInternalDeviceControlIrp(TDI_SEND_DATAGRAM, DeviceObject, AddressObject, pEvent, &IoStatus)))
		return STATUS_INSUFFICIENT_RESOURCES;

    if (!(Mdl = IoAllocateMdl(Data, Length, FALSE, FALSE, Irp)))
	{
		IoFreeIrp(Irp);
		return STATUS_INSUFFICIENT_RESOURCES;
	}

    MmProbeAndLockPages(Mdl, KernelMode, IoModifyAccess);

    TdiBuildSendDatagram(Irp, DeviceObject, AddressObject, 0, 0, Mdl, Length, &Dst);

	ntStatus = IoCallDriver(DeviceObject, Irp);

    if ((ntStatus == STATUS_PENDING) && (!WriteEvent))
        ntStatus = KeWaitForSingleObject(pEvent, UserRequest, KernelMode, FALSE, 0);

    return(ntStatus == STATUS_SUCCESS ? IoStatus.Status : ntStatus);

}


NTSTATUS
	TdiRecvfrom(
		PFILE_OBJECT	AddressObject, 
		PVOID			Data, 
		ULONG			Length, 
		ULONG			Addr,
		USHORT			Port,
		PULONG			pRead,
		PKEVENT			ReadEvent
		)
{
	KEVENT	Event; 
	PIRP	Irp;
	PMDL	Mdl;
	NTSTATUS	ntStatus;
	IO_STATUS_BLOCK IoStatus;
    PDEVICE_OBJECT DeviceObject = IoGetRelatedDeviceObject(AddressObject);
	TA_IP_ADDRESS	SrcAddr = {1, {TDI_ADDRESS_LENGTH_IP, TDI_ADDRESS_TYPE_IP, {Port, Addr}}};
    TDI_CONNECTION_INFORMATION Src = {0, 0, 0, 0, sizeof SrcAddr, &SrcAddr};
	PKEVENT pEvent = ReadEvent;

	if (!ReadEvent)
	{
		KeInitializeEvent(&Event, NotificationEvent, FALSE);
		pEvent = &Event;
	}

    if (!(Irp = TdiBuildInternalDeviceControlIrp(TDI_RECEIVE_DATAGRAM, DeviceObject, AddressObject, pEvent, &IoStatus)))
		return STATUS_INSUFFICIENT_RESOURCES;

    if (!(Mdl = IoAllocateMdl(Data, Length, FALSE, FALSE, Irp)))
		return STATUS_INSUFFICIENT_RESOURCES;

    MmProbeAndLockPages(Mdl, KernelMode, IoModifyAccess);
	TdiBuildReceiveDatagram(Irp, DeviceObject, AddressObject, 0, 0, Mdl, Length, &Src, &Src, TDI_RECEIVE_NORMAL);

   	ntStatus = IoCallDriver(DeviceObject, Irp);

    if ((ntStatus == STATUS_PENDING) && (!ReadEvent))
        ntStatus = KeWaitForSingleObject(pEvent, UserRequest, KernelMode, FALSE, 0);

	if (ntStatus == STATUS_SUCCESS)
	{
		*pRead = IoStatus.Status;
	}

    return(ntStatus);

}



NTSTATUS
TdiTest()
{
    PFILE_OBJECT ConnectionFileObject, AddressFileObject;
    HANDLE		AddressHandle, ConnectionHandle;
    CHAR		Buffer[80], Data[] = "Hello from Gary";
    NTSTATUS	Status;
    KEVENT		Done; 
	
	KeInitializeEvent(&Done, NotificationEvent, FALSE);

    Status = TdiCreateConnection(&ConnectionHandle, &ConnectionFileObject);
    if (!NT_SUCCESS(Status)) return Status;

    Status = TdiCreateAddress(&AddressHandle, &AddressFileObject, SOCK_STREAM, 0, 0);
    if (!NT_SUCCESS(Status)) return Status;

    do 
	{
		IO_STATUS_BLOCK IoStatus = {0};
        KEVENT	Event;

		Status = TdiSetEventHandler(AddressFileObject, TDI_EVENT_DISCONNECT, TdiEventDisconnect, &Done);
        if (!NT_SUCCESS(Status)) break;

        Status = TdiSetEventHandler(AddressFileObject, TDI_EVENT_ERROR, TdiEventError, 0);
        if (!NT_SUCCESS(Status)) break;

        Status = TdiSetEventHandler(AddressFileObject, TDI_EVENT_RECEIVE, TdiEventReceive, 0);
        if (!NT_SUCCESS(Status)) break;

        Status = TdiBind(ConnectionFileObject, AddressHandle);
        if (!NT_SUCCESS(Status)) break;

        Status = TdiConnect(ConnectionFileObject, 0x41A1F6A8, 0x700, NULL);
        if (!NT_SUCCESS(Status)) break;

		KeInitializeEvent(&Event, NotificationEvent, FALSE);

//        Status = TdiRecv(ConnectionFileObject, Buffer, sizeof Buffer, &Event, &IoStatus);
//        if (!NT_SUCCESS(Status)) break;

        Status = TdiSend(ConnectionFileObject, Data, sizeof Data);
        if (!NT_SUCCESS(Status)) break;

        Status = TdiSend(ConnectionFileObject, Data, sizeof Data);
        if (!NT_SUCCESS(Status)) break;

        Status = TdiSend(ConnectionFileObject, Data, sizeof Data);
        if (!NT_SUCCESS(Status)) break;

        Status = TdiDisconnect(ConnectionFileObject);
        if (!NT_SUCCESS(Status)) break;

        Status = KeWaitForSingleObject(&Event, UserRequest, KernelMode, FALSE, 0);
        if (Status == STATUS_SUCCESS) Status = IoStatus.Status;

        DbgPrint("Status = %lx, IoStatus.Status = %lx, IoStatus.Information = %ld\n", Status, IoStatus.Status, IoStatus.Information);

        KeWaitForSingleObject(&Done, UserRequest, KernelMode, FALSE, 0);

    } while (0);

    ObDereferenceObject(ConnectionFileObject);

    ObDereferenceObject(AddressFileObject);

    ZwClose(ConnectionHandle);

    ZwClose(AddressHandle);

    return Status;

}


NTSTATUS
	TdiSendMessage(
	   ULONG	Ip,
	   USHORT	Port,
	   PCHAR	Message,
	   ULONG	Length
	   )
			   
{
    PFILE_OBJECT ConnectionFileObject, AddressFileObject;
    HANDLE		AddressHandle, ConnectionHandle;
    CHAR		Buffer[80], Data[] = "Hello from Gary";
    NTSTATUS	Status;
    KEVENT		Done; 
	
	KeInitializeEvent(&Done, NotificationEvent, FALSE);

    Status = TdiCreateConnection(&ConnectionHandle, &ConnectionFileObject);
    if (!NT_SUCCESS(Status)) return Status;

    Status = TdiCreateAddress(&AddressHandle, &AddressFileObject, SOCK_STREAM, 0, 0);
    if (!NT_SUCCESS(Status)) return Status;

    do 
	{
		IO_STATUS_BLOCK IoStatus;
        KEVENT	Event;

		Status = TdiSetEventHandler(AddressFileObject, TDI_EVENT_DISCONNECT, TdiEventDisconnect, &Done);
        if (!NT_SUCCESS(Status)) break;

        Status = TdiSetEventHandler(AddressFileObject, TDI_EVENT_ERROR, TdiEventError, 0);
        if (!NT_SUCCESS(Status)) break;

        Status = TdiSetEventHandler(AddressFileObject, TDI_EVENT_RECEIVE, TdiEventReceive, 0);
        if (!NT_SUCCESS(Status)) break;

        Status = TdiBind(ConnectionFileObject, AddressHandle);
        if (!NT_SUCCESS(Status)) break;

        Status = TdiConnect(ConnectionFileObject, Ip, Port, NULL);
        if (!NT_SUCCESS(Status)) break;

        Status = TdiSend(ConnectionFileObject, Message, Length);
        if (!NT_SUCCESS(Status)) break;

        Status = TdiDisconnect(ConnectionFileObject);
        if (!NT_SUCCESS(Status)) break;

        KeWaitForSingleObject(&Done, UserRequest, KernelMode, FALSE, 0);

    } while (0);

    ObDereferenceObject(ConnectionFileObject);

    ObDereferenceObject(AddressFileObject);

    ZwClose(ConnectionHandle);

    ZwClose(AddressHandle);

    return Status;

}

#endif //_TDI_SUPPORT