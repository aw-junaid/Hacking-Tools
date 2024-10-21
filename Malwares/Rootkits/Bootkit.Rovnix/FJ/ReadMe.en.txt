Utility to concatenate files FJ
---------------------------------

Designed to attach one or more executable files (EXE, DLL or SYS) file or driver-installer.
File-installer (the driver) must support attachments.

As an installer file can be used:
- Driver KLoader.sys, for attaching thereto DLL for injection produce
- Module BkSetup.exe, to attach to it the driver to download.

How it works:
Attachment files are compressed and appended to the file-installer, the structure of the module-installer changes so attached the files themselves in the memory module load-installer. further, code in the module-installer decompresses attachments.
 
Configuration file:
The list of files to attach and their parameters is given special configuration file . An example configuration file is included.
