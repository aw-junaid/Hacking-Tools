Management Utility virtual filesystem

utility allows vfs.exe
- To copy the files to the virtual file system ;
- Read the files from the virtual file system ;
- Udlyat files in a virtual file system ;
- View the contents of the root directory of the virtual file system ;
- NT driver download image and run it ;

The utility supports the following commands :
DIR - view the contents of a directory (example : vfs dir)
COPY - copy the file to the file system or file system (example : vfs copy c: \ windows \ calc.exe vfs \ calc.exe
vfs copy vfs \ calc.exe c: \ test \ calc.exe)
DEL - delete a file (example : vfs del vfs \ calc.exe)
LOAD - load the specified driver (example : vfs load c: \ test \ sample.sys
vfs load vfs \ sample.sys)

( To address files found in a virtual file system , use the prefix "vfs \")
