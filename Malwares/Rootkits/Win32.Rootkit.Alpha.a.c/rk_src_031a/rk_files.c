#include "driver.h"
#include "files.h"


//----------------------------------------------------------------------
//
// HookDrive
//
// Hook the drive specified by determining which device object to 
// attach to. The algorithm used here is similar to the one used
// internally by NT to determine which device object a file system request
// is directed at.
//
//----------------------------------------------------------------------
BOOLEAN cmdHookDrive( IN char Drive, IN PDRIVER_OBJECT DriverObject )
{
    IO_STATUS_BLOCK     ioStatus;
    HANDLE              ntFileHandle;   
    OBJECT_ATTRIBUTES   objectAttributes;
    PDEVICE_OBJECT      fileSysDevice;
    PDEVICE_OBJECT      hookDevice;
    UNICODE_STRING      fileNameUnicodeString;
    WCHAR               filename[] = L"\\DosDevices\\A:\\";
    NTSTATUS            ntStatus;
    ULONG               i;
    PFILE_OBJECT        fileObject;
    PHOOK_EXTENSION     hookExtension;
    
	DbgPrint(("cmdHookDrive called\n"));

    //
    // Translate the drive letter to a 0-based integer
    //
    if ( Drive >= 'a' && Drive <= 'z' ) {

        Drive -= 'a';

    } else {

        Drive -= 'A';

    }

    //
    // Is it a legal drive letter?
    //
    if ( (unsigned char) Drive >= 26 )  {

        return FALSE;
    }

    //
    // Has this drive already been hooked?
    //
    if ( gDriveDevices[Drive] == NULL )  {

        //
        // Frob the name to make it refer to the drive specified in the input 
        // parameter.
        //
        filename[12] = 'A'+Drive;

        //
        // We have to figure out what device to hook - first open the volume's 
        // root directory
        //
        RtlInitUnicodeString( &fileNameUnicodeString, filename );
        InitializeObjectAttributes( &objectAttributes, &fileNameUnicodeString, 
                                    OBJ_CASE_INSENSITIVE, NULL, NULL );
        ntStatus = ZwCreateFile( &ntFileHandle, SYNCHRONIZE|FILE_ANY_ACCESS, 
                                 &objectAttributes, &ioStatus, NULL, 0, FILE_SHARE_READ|FILE_SHARE_WRITE, 
                                 FILE_OPEN, 
                                 FILE_SYNCHRONOUS_IO_NONALERT|FILE_DIRECTORY_FILE, 
                                 NULL, 0 );
        if( !NT_SUCCESS( ntStatus ) ) {

            DbgPrint(("Filemon: Could not open drive %c: %x\n", 'A'+Drive, ntStatus ));

            return FALSE;
        }

        DbgPrint(("Filemon:  opened the root directory!!! handle: %x\n", ntFileHandle));   

        //
        // Got the file handle, so now look-up the file-object it refers to
        //
        ntStatus = ObReferenceObjectByHandle( ntFileHandle, FILE_READ_DATA, 
                                              NULL, KernelMode, &fileObject, NULL );
        if( !NT_SUCCESS( ntStatus )) {

            DbgPrint(("Filemon: Could not get fileobject from handle: %c\n", 'A'+Drive ));
            ZwClose( ntFileHandle );

            return FALSE;
        }

        //  
        // Next, find out what device is associated with the file object by getting its related
        // device object
        //
        fileSysDevice = IoGetRelatedDeviceObject( fileObject );

        if ( ! fileSysDevice ) {

            DbgPrint(("Filemon: Could not get related device object: %c\n", 'A'+Drive ));

            ObDereferenceObject( fileObject );
            ZwClose( ntFileHandle );

            return FALSE;
        }

        //  
        // Check the device list to see if we've already attached to this particular device. 
        // This can happen when more than one drive letter is being handled by the same network
        // redirecter
        //  
        for( i = 0; i < 26; i++ ) {

            if( gDriveDevices[i] == fileSysDevice ) {

                //
                // If we're already watching it, associate this drive letter
                // with the others that are handled by the same network driver. This
                // enables us to intelligently update the hooking menus when the user
                // specifies that one of the group should not be watched -we mark all
                // of the related drives as unwatched as well
                //
                ObDereferenceObject( fileObject );

                ZwClose( ntFileHandle );

                gDriveMap[ Drive ]     = gDriveMap[i];
                gDriveDevices[ Drive ] = fileSysDevice;

                return TRUE;
            }
        }

        //
        // The file system's device hasn't been hooked already, so make a hooking device
        //  object that will be attached to it.
        //
        ntStatus = IoCreateDevice( DriverObject,
                                   sizeof(HOOK_EXTENSION),
                                   NULL,
                                   fileSysDevice->DeviceType,
                                   0,
                                   FALSE,
                                   &hookDevice );
        if ( !NT_SUCCESS(ntStatus) ) {

            DbgPrint(("Filemon: failed to create associated device: %c\n", 'A'+Drive ));   

            ObDereferenceObject( fileObject );
            ZwClose( ntFileHandle );

            return FALSE;
        }

        //
        // Clear the device's init flag as per NT DDK KB article on creating device 
        // objects from a dispatch routine
        //
        hookDevice->Flags &= ~DO_DEVICE_INITIALIZING;

        //
        // Setup the device extensions. The drive letter and file system object are stored
        // in the extension.
        //
        hookExtension = hookDevice->DeviceExtension;
        hookExtension->LogicalDrive = 'A'+Drive;
        hookExtension->FileSystem   = fileSysDevice;

        //
        // Finally, attach to the device. The second we're successfully attached, we may 
        // start receiving IRPs targetted at the device we've hooked.
        //
        ntStatus = IoAttachDeviceByPointer( hookDevice, fileSysDevice );
        if ( !NT_SUCCESS(ntStatus) )  {

            //
            // Couldn' attach for some reason
            //
            DbgPrint(("Filemon: Connect with Filesystem failed: %c (%x) =>%x\n", 
                      'A'+Drive, fileSysDevice, ntStatus ));

            //
            // Derefence the object and get out
            //
            ObDereferenceObject( fileObject );
            ZwClose( ntFileHandle );

            return FALSE;

        } else {

            // 
            // Make a new drive group for the device,l if it does not have one 
            // already
            // 
            DbgPrint(("Filemon: Successfully connected to Filesystem device %c\n", 'A'+Drive ));
            if( !gDriveMap[ Drive ] ) {

                gDriveMap[ Drive ] = ++gDriveGroup;
            }
        }
    
        //
        // Close the file and update the hooked drive list by entering a
        // pointer to the hook device object in it.
        //
        ObDereferenceObject( fileObject );

        ZwClose( ntFileHandle );

        gDriveDevices[Drive] = hookDevice;
    }

    return TRUE;
}


void cmdHookAllDrives(){
	char aDrive;
	DbgPrint(("cmdHookAllDrives called\n"));
	for(aDrive = 0; aDrive < 26; aDrive++ ){
		cmdHookDrive( (char) aDrive + 'A', gDriverObject );
	}
}
