# Win64/Rovnix - Volume Boot Record Bootkit (Rovnix)

Bootloader for the drivers
--------------------------

Allows loading specially crafted drivers during Operating System start-up.
The driver is loaded before NT kernel initialization and before PatchGuard starts,
so it can patch any kernel code.

The driver is given control before any other drivers are loaded (including all
boot-load drivers), so it can monitor and interact with their loading process.
Digital signature for the driver is not required.

Supports all Windows OS, from XP to Windows 8 inclusive.
Supports 2 CPU architectures: x86 and AMD64 (EM64T).
Boot-loader is working under any NTFS types.

Assembled project has three major components:
- Initial Program Load (IPL);
- specially crafted driver that is loaded prior NT kernel;
- installation program (or installation library(DLL));

IPL code is metamorphic and consist of a number of blocks. During each project
compilation blocks are mixed in a random order.
IPL code is encrypted and dynamically decrypted only during execution.
Each newly compiled IPL code is different from the previous ones.
The driver is also encrypted when written to the disk and decrypted by IPL during
the start-up.

There is a size limit for the driver: due to the way IPL operates - the driver can't
be bigger than 100KB.

The project is compiled with MS Visual Studio 2005 and MS Windows 7 WDK.

Additional components
-------------------------

The driver may include the following additional components:

- Virtual File System Manager. Creates encrypted(RC6) virtual file system (VFS) inunformatted disk area.  Enables User-mode interface for working with the files stored in VFS.

- filters disk access. Blocks 'external' access to the sectors where IPL and VFS is located. Hides VFS

- DLL injector. Allows process loading(injection) of DLLs stored on VFS or attached to the file driver. Includes interface to manage injects in the user-mode.

- Driver loader. Enables interface for loading unsigned drivers.

- TCP/IP stack (including: ARP, ICMP, DNS). Allows BSD-socket network access for drivers and user-mode applications.

Project components
------------------
  1. IPL generator(\BkGen).
  2. Loader library(\BkLib).
  3. Installation program(\BkSetup).
  4. Installation library(SetupDll).
  5. Injection driver(\KLoader).
  6. VFS library(\FsLib).
  7. Unsigned drivers loader library(\DrvLdr).
  8. Loader and VFS protection filter library(\BkFilter).
  9. File attachment utility(\FJ).
  10. VFS manager tool/interface(\VFS).
  11. Batch files for assembling a loader sample with sample DLLs (\BkBuild).

IPL Generator
-------------
  Assembles into an executable file BkGen.exe - works under x86 only Creates VBR.COM when started and includes metamorphic loader code. Unique loader is generated at each execution

Loader library
------------------
  Assembles into a static library(.lib) - works under x86 and AMD64
  Includes functions required for loader installation and initialization.
  Imported by the installer and the driver. See bklib.h for more details.

Installation program
--------------------
  Searches loader file - VBR.COM during compilation and integrates it into resources.
  Compiles into an executable file BkSetup.exe - works under x86 only

Installation library
---------------------
  Assembles into a library file SetupDll.dll - works under x86 and x64
  The library exports one function: ULONG BkInstall(BOOL bReboot).
  Calling this function performs loader installation.
  The function returns Win32 error code if any issues are encountered.

Injection driver
----------------
  Assembles into an NT driver (kloader.sys) - works under x86 and x64
  Injects attached DLLs into specified processes. The list of DLLs and processes is specified in the configuration file for FJ utility.

VFS library
------------------------- 
  Being linked into Injector Driver (kloader.sys) - works under x86 and x64
  Creates VFS in unformatted hard disk sectors.
  If no or insufficient unformatted disk space is found the size of the last partition on the hard disk in decreased.
  VFS is a modified FAT16 partition. The cluster size is equal to the physical sector size on the disk.
  VFS maximum size is ~32MB.
  Filenames are in 8.3 format. Long filenames are not supported. No catalogue support.
  VFS is fully encrypted with RC6.

Unsigned drivers loader library
-------------------------------------------
  Being linked into Injector Driver (kloader.sys) - works under x86 and x64
  Allows loading and execution of unsigned NT kernel drivers.

Loader and VFS protection filter library
-----------------------------------------------------------------
  Being linked into Injector Driver (kloader.sys) - works under x86 and x64
  Filters low-level disk read/write calls.
  Disables any modifications to the disk sectors where the loader is stored.
  Blocks any modifications by external applications and drivers to the disk sectors hosting VFS.
  Returns zeros if anything 'external' attempts to read VFS - hiding it.
  
File attachment utility
-------------------------------
  Assembles into an executable file FJ.EXE - works under x86 only
  Used for attaching injectable DLLs to the driver file and for attaching the driver file to the installer.
  See \FJ\ReadMe.txt for more details.

Batch files for assembling a loader sample with sample DLLs (\BkBuild).
-----------------------------------------
BkBuild.bat - assembles the installer with attached drivers kloader.sys - for x86 and amd64 accordingly. Each driver is attached a DLL for injection.
BkSetup.cfg - configuration file for assembling the installation program and attaching the drivers to it.
setupdll.cfg- configuration file for assembling the installation library and attaching the drivers to it.
demo32.dll  - 32bit demo library
demo64.dll  - 64bit demo library

Assembly order
--------------
  1. Using Visual Studio 2005 compile the entire project. Compile for i386 first and then for amd64.
  2. Open the console(CMD.EXE). Go to \BkBuild folder and start BkBuild.exe
  3. Take the compiled loader from \BkBuild\Release folder.
