KBOT - NT-kernel bot program
----------------------------
The program is designed to download through HTTP, files, save them to the VFS and registration module KLDR for injection produce in a given process.
 
Supported OS: XP - WIN7.
Supported Architectures: x86, AMD64 (EM64T).

Project statically link library (LIB), which links to the driver.

When you first start the program generates a 16-bit user ID and saves it to a file on the VFS \ USER.ID
In the future, the user ID is not changed and is transmitted every time a request to the management server.

The program provides two types of HTTP requests to the management server:
- Request configuration file;
- File request command;

The program supports the obfuscation of HTTP requests .
For this line of each request is encrypted using an algorithm RC6 and translated into a format BASE64.

The program supports validation of digital signatures and encryption of configuration files and command files .
To do this, the driver attachments public.key, containing the public RSA- key.
With this key decryption is performed and the signature verification of the resulting file .
If the file is not tested , it is ignored.

configuration file

The program works on the basis of the prescribed settings in the configuration file ( configuration file) .
Config file may be stored on VFS, or may be attached directly to the driver .
When you run the program first looks for a file in the VFS, and if it is not - use the attached configuration file.
Example of config file with the description : \ BkBuild \ kbot.ini
When a new configuration file , it is saved in a file on the VFS \ KBOT.INI existing KBOT.INI replaced .
File commands

A text file containing one or more of the following commands:

LOAD_FILE <HTTP link> [filename on VFS] - downloads a file from the link given in the VFS and stores with the given name

DELETE_FILE <file name on the VFS> - removes the file specified with VFS

SET_INJECT <file name on the VFS> <list of processes> - sets inject the specified file (usually DLL) in
specified in the list of processes. All set Inject apply immediately and are saved
file \ INJECTS.SYS at VFS, so as to be active after restarting the OS.
To remove inject use the command "SET_INJECT <file name on the VFS>" without a list of processes.

 Command names are case-sensitive parameters - no.
 
