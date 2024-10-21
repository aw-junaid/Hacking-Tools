comment * ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

        winextra.inc is the second part of the windows.inc file that is
        called by an "include" directive at the end of windows.inc.

        This technique of splitting the windows.inc file is used to avoid
        the 32k line count limit in ML.EXE versions 6.14 and 6.15. The later
        32 bit versions of ML.EXE are not effected by this limit.

        This file is covered by the copyright in windows.inc.

        ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ *

IFNDEF _winextra_
_winextra_ equ <1>

;;;; ole

;OLE Structures and Equates
;----------------------------------------------------------------------------

CLSCTX_ALL              equ CLSCTX_INPROC_SERVER or CLSCTX_INPROC_HANDLER or CLSCTX_LOCAL_SERVER
CLSCTX_INPROC           equ CLSCTX_INPROC_SERVER or CLSCTX_INPROC_HANDLER
CLSCTX_SERVER           equ  CLSCTX_INPROC_SERVER or CLSCTX_LOCAL_SERVER 

REGCLS_SINGLEUSE        equ 0
REGCLS_MULTIPLEUSE      equ 1
REGCLS_MULTI_SEPARATE   equ 2

MARSHALINTERFACE_MIN    equ 500
CWCSTORAGENAME          equ 32
STGM_DIRECT             equ 0
STGM_TRANSACTED         equ 00010000h
STGM_SIMPLE             equ 08000000h
STGM_READ               equ 0
STGM_WRITE              equ 00000001h
STGM_READWRITE          equ 00000002h
STGM_SHARE_DENY_NONE    equ 00000040h
STGM_SHARE_DENY_READ    equ 00000030h
STGM_SHARE_DENY_WRITE   equ 00000020h
STGM_SHARE_EXCLUSIVE    equ 00000010h
STGM_PRIORITY           equ 00040000h
STGM_DELETEONRELEASE    equ 04000000h
STGM_CREATE             equ 00001000h
STGM_CONVERT            equ 00020000h
STGM_FAILIFTHERE        equ 0

 ; IID   STRUCT DWORD
 ;     Data1        dd ?
 ;     Data2        dw ?
 ;     Data3        dw ?
 ;     Data4        db 8 dup(?)
 ; IID ENDS

BFFM_SETSTATUSTEXT      equ  WM_USER + 100
BFFM_SETSELECTION       equ  WM_USER + 102

CMIC_VALID_SEE_FLAGS    equ  SEE_VALID_CMIC_FLAGS     ; Internal 

COLORSCHEME struct
   dwSize dd ?
   clrBtnHighlight COLORREF ?
   clrBtnShadow COLORREF ?
COLORSCHEME ends

NMMOUSE struct
    hdr NMHDR <>
    dwItemSpec dd ?
    dwItemData dd ?
    pt POINT <>
    dwHitInfo dd ?
NMMOUSE ends

NMCLICK equ <NMMOUSE>

HOVER_DEFAULT   equ 0FFFFFFFFh

TRACKMOUSEEVENT struct
    cbSize dd ?
    dwFlags dd ?
    hwndTrack dd ?
    dwHoverTime dd ?
TRACKMOUSEEVENT ends

TBSTYLE_AUTOSIZE equ 10h
TB_SETCOLORSCHEME     equ  CCM_SETCOLORSCHEME
SWP_NOSENDCHANGING  equ 400h
CC_ANYCOLOR              equ 100h
TCN_FOCUSCHANGE        equ TCN_FIRST - 4

STREAM_SEEK_SET	equ  0
STREAM_SEEK_CUR	equ 1
STREAM_SEEK_END	equ  2

GTL_DEFAULT	 equ 0
GTL_USECRLF	 equ 1
GTL_PRECISE	equ 2
GTL_CLOSE equ 4
GTL_NUMCHARS equ 8
GTL_NUMBYTES equ 16

TM_PLAINTEXT			equ 1
TM_RICHTEXT				equ 2
TM_SINGLELEVELUNDO		equ 4
TM_MULTILEVELUNDO		equ 8
TM_SINGLECODEPAGE		equ 16
TM_MULTICODEPAGE		equ 32

hWndOwner equ hwndOwner

TO_ADVANCEDTYPOGRAPHY equ	1
TO_SIMPLELINEBREAK	equ	2
TCS_FLATBUTTONS         equ 8

TTN_GETDISPINFO equ TTN_FIRST
TTF_DI_SETITEM          equ 8000h
RBBS_USECHEVRON     equ 200h
GT_DEFAULT	equ	0
GT_USECRLF	equ	1

DLLVERSIONINFO struct
        cbSize dd ?
        dwMajorVersion dd ?
        dwMinorVersion dd ?
        dwBuildNumber dd ?
        dwPlatformID dd ?
DLLVERSIONINFO ends

BTNS_SHOWTEXT   equ 40h
I_IMAGENONE equ -2

DTM_FIRST   equ 1000h
DTM_SETFORMAT equ DTM_FIRST + 5
DTM_SETSYSTEMTIME   equ DTM_FIRST + 2
DTM_GETSYSTEMTIME   equ DTM_FIRST + 1
MAX_COMPUTERNAME_LENGTH equ 15

;;;; end ole


;;;; shlobj

ADDURL_SILENT                    equ 0001h
AD_APPLY_ALL                     equ (AD_APPLY_SAVE or AD_APPLY_HTMLGEN or AD_APPLY_REFRESH)
AD_APPLY_BUFFERED_REFRESH        equ 00000010h
AD_APPLY_DYNAMICREFRESH          equ 00000020h
AD_APPLY_FORCE                   equ 00000008h
AD_APPLY_HTMLGEN                 equ 00000002h
AD_APPLY_REFRESH                 equ 00000004h
AD_APPLY_SAVE                    equ 00000001h
AD_GETWP_BMP                     equ 00000000h
AD_GETWP_IMAGE                   equ 00000001h
AD_GETWP_LAST_APPLIED            equ 00000002h
BFFM_ENABLEOK                    equ (WM_USER + 101)
BFFM_INITIALIZED                 equ 1
BFFM_IUNKNOWN                    equ 5
BFFM_SELCHANGED                  equ 2
BFFM_SETEXPANDED                 equ (WM_USER + 106)
BFFM_SETOKTEXT                   equ (WM_USER + 105)
BFFM_SETSELECTIONA               equ (WM_USER + 102)
BFFM_SETSELECTIONW               equ (WM_USER + 103)
BFFM_SETSTATUSTEXTA              equ (WM_USER + 100)
BFFM_SETSTATUSTEXTW              equ (WM_USER + 104)
BFFM_VALIDATEFAILEDA             equ 3
BFFM_VALIDATEFAILEDW             equ 4
BIF_BROWSEFORCOMPUTER            equ 1000h
BIF_BROWSEFORPRINTER             equ 2000h
BIF_BROWSEINCLUDEFILES           equ 4000h
BIF_BROWSEINCLUDEURLS            equ 0080h
BIF_DONTGOBELOWDOMAIN            equ 0002h
BIF_EDITBOX                      equ 0010h
BIF_NEWDIALOGSTYLE               equ 0040h
BIF_NONEWFOLDERBUTTON            equ 0200h
BIF_NOTRANSLATETARGETS           equ 0400h
BIF_RETURNFSANCESTORS            equ 0008h
BIF_RETURNONLYFSDIRS             equ 0001h
BIF_SHAREABLE                    equ 8000h
BIF_STATUSTEXT                   equ 0004h
BIF_UAHINT                       equ 0100h
BIF_USENEWUI                     equ (BIF_NEWDIALOGSTYLE or BIF_EDITBOX)
BIF_VALIDATE                     equ 0020h
CABINETSTATE_VERSION             equ 2
CDB2GVF_SHOWALLFILES             equ 00000001h
CDB2N_CONTEXTMENU_DONE           equ 00000001h
CDB2N_CONTEXTMENU_START          equ 00000002h
CDBOSC_STATECHANGE               equ 00000004h
CLOSEPROPS_DISCARD               equ 0001h
CLOSEPROPS_NONE                  equ 0000h
CMF_EXTENDEDVERBS                equ 00000100h
CMF_FINDHACK                     equ 00000080h
CMIC_MASK_CONTROL_DOWN           equ 40000000h
CMIC_MASK_SHIFT_DOWN             equ 10000000h
COMPONENT_DEFAULT_LEFT           equ 0FFFFh
COMPONENT_DEFAULT_TOP            equ 0FFFFh
COMPONENT_TOP                    equ 3fffffffh
COMP_ELEM_CHECKED                equ 00000002h
COMP_ELEM_CURITEMSTATE           equ 00004000h
COMP_ELEM_DIRTY                  equ 00000004h
COMP_ELEM_FRIENDLYNAME           equ 00000400h
COMP_ELEM_NOSCROLL               equ 00000008h
COMP_ELEM_ORIGINAL_CSI           equ 00001000h
COMP_ELEM_POS_LEFT               equ 00000010h
COMP_ELEM_POS_TOP                equ 00000020h
COMP_ELEM_POS_ZINDEX             equ 00000100h
COMP_ELEM_RESTORED_CSI           equ 00002000h
COMP_ELEM_SIZE_HEIGHT            equ 00000080h
COMP_ELEM_SIZE_WIDTH             equ 00000040h
COMP_ELEM_SOURCE                 equ 00000200h
COMP_ELEM_SUBSCRIBEDURL          equ 00000800h
COMP_ELEM_TYPE                   equ 00000001h
COMP_TYPE_CFHTML                 equ 4
COMP_TYPE_CONTROL                equ 3
COMP_TYPE_HTMLDOC                equ 0
COMP_TYPE_MAX                    equ 4
COMP_TYPE_PICTURE                equ 1
COMP_TYPE_WEBSITE                equ 2
CSIDL_ADMINTOOLS                 equ 0030h
CSIDL_ALTSTARTUP                 equ 001dh
CSIDL_APPDATA                    equ 001ah
CSIDL_BITBUCKET                  equ 000ah
CSIDL_CDBURN_AREA                equ 003bh
CSIDL_COMMON_ADMINTOOLS          equ 002fh
CSIDL_COMMON_ALTSTARTUP          equ 001eh
CSIDL_COMMON_APPDATA             equ 0023h
CSIDL_COMMON_DESKTOPDIRECTORY    equ 0019h
CSIDL_COMMON_DOCUMENTS           equ 002eh
CSIDL_COMMON_FAVORITES           equ 001fh
CSIDL_COMMON_MUSIC               equ 0035h
CSIDL_COMMON_OEM_LINKS           equ 003ah
CSIDL_COMMON_PICTURES            equ 0036h
CSIDL_COMMON_PROGRAMS            equ 0017h
CSIDL_COMMON_STARTMENU           equ 0016h
CSIDL_COMMON_STARTUP             equ 0018h
CSIDL_COMMON_TEMPLATES           equ 002dh
CSIDL_COMMON_VIDEO               equ 0037h
CSIDL_COMPUTERSNEARME            equ 003dh
CSIDL_CONNECTIONS                equ 0031h
CSIDL_CONTROLS                   equ 0003h
CSIDL_COOKIES                    equ 0021h
CSIDL_DESKTOP                    equ 0000h
CSIDL_DESKTOPDIRECTORY           equ 0010h
CSIDL_DRIVES                     equ 0011h
CSIDL_FAVORITES                  equ 0006h
CSIDL_FLAG_CREATE                equ 8000h
CSIDL_FLAG_DONT_UNEXPAND         equ 2000h
CSIDL_FLAG_DONT_VERIFY           equ 4000h
CSIDL_FLAG_MASK                  equ 0FF00h
CSIDL_FLAG_NO_ALIAS              equ 1000h
CSIDL_FLAG_PER_USER_INIT         equ 0800h
CSIDL_FONTS                      equ 0014h
CSIDL_HISTORY                    equ 0022h
CSIDL_INTERNET                   equ 0001h
CSIDL_INTERNET_CACHE             equ 0020h
CSIDL_LOCAL_APPDATA              equ 001ch
CSIDL_MYDOCUMENTS                equ CSIDL_PERSONAL
CSIDL_MYMUSIC                    equ 000dh
CSIDL_MYPICTURES                 equ 0027h
CSIDL_MYVIDEO                    equ 000eh
CSIDL_NETHOOD                    equ 0013h
CSIDL_NETWORK                    equ 0012h
CSIDL_PERSONAL                   equ 0005h
CSIDL_PRINTERS                   equ 0004h
CSIDL_PRINTHOOD                  equ 001bh
CSIDL_PROFILE                    equ 0028h
CSIDL_PROGRAMS                   equ 0002h
CSIDL_PROGRAM_FILES              equ 0026h
CSIDL_PROGRAM_FILESX86           equ 002ah
CSIDL_PROGRAM_FILES_COMMON       equ 002bh
CSIDL_PROGRAM_FILES_COMMONX86    equ 002ch
CSIDL_RECENT                     equ 0008h
CSIDL_RESOURCES                  equ 0038h
CSIDL_RESOURCES_LOCALIZED        equ 0039h
CSIDL_SENDTO                     equ 0009h
CSIDL_STARTMENU                  equ 000bh
CSIDL_STARTUP                    equ 0007h
CSIDL_SYSTEM                     equ 0025h
CSIDL_SYSTEMX86                  equ 0029h
CSIDL_TEMPLATES                  equ 0015h
CSIDL_WINDOWS                    equ 0024h
DBC_GS_IDEAL                     equ 0
DBC_GS_SIZEDOWN                  equ 1
DBC_HIDE                         equ 0
DBC_SHOW                         equ 1
DBC_SHOWOBSCURE                  equ 2
DFM_CMD_COPY                     equ -3
DFM_CMD_DELETE                   equ -1
DFM_CMD_LINK                     equ -4
DFM_CMD_MODALPROP                equ -12
DFM_CMD_MOVE                     equ -2
DFM_CMD_NEWFOLDER                equ -6
DFM_CMD_PASTE                    equ -7
DFM_CMD_PASTELINK                equ -10
DFM_CMD_PASTESPECIAL             equ -11
DFM_CMD_PROPERTIES               equ -5
DFM_CMD_RENAME                   equ -13
DFM_CMD_VIEWDETAILS              equ -9
DFM_CMD_VIEWLIST                 equ -8
DFM_GETDEFSTATICID               equ 14
DFM_GETHELPTEXT                  equ 5
DFM_GETHELPTEXTW                 equ 11
DFM_GETVERBA                     equ 16
DFM_GETVERBW                     equ 15
DFM_INVOKECOMMAND                equ 2
DFM_INVOKECOMMANDEX              equ 12
DFM_MAPCOMMANDNAME               equ 13
DFM_MERGECONTEXTMENU             equ 1
DFM_MERGECONTEXTMENU_BOTTOM      equ 17
DFM_MERGECONTEXTMENU_TOP         equ 10
DFM_VALIDATECMD                  equ 9
DFM_WM_DRAWITEM                  equ 7
DFM_WM_INITMENUPOPUP             equ 8
DFM_WM_MEASUREITEM               equ 6
DVASPECT_COPY                    equ 3
DVASPECT_LINK                    equ 4
DVASPECT_SHORTNAME               equ 2
DWFAF_AUTOHIDE                   equ 0010h
DWFAF_GROUP1                     equ 0002h
DWFAF_GROUP2                     equ 0004h
DWFAF_HIDDEN                     equ 0001h
DWFRF_DELETECONFIGDATA           equ 0001h
DWFRF_NORMAL                     equ 0000h
EXP_DARWIN_ID_SIG                equ 0A0000006h
EXP_LOGO3_ID_SIG                 equ 0A0000007h
EXP_PROPERTYSTORAGE_SIG          equ 0A0000009h
EXP_SPECIAL_FOLDER_SIG           equ 0A0000005h
EXP_SZ_ICON_SIG                  equ 0A0000007h
EXP_SZ_LINK_SIG                  equ 0A0000001h
FCIDM_BROWSERFIRST               equ 0a000h
FCIDM_BROWSERLAST                equ 0bf00h
FCIDM_GLOBALFIRST                equ 8000h
FCIDM_GLOBALLAST                 equ 9fffh
FCIDM_MENU_EDIT                  equ FCIDM_GLOBALFIRST+0040h
FCIDM_MENU_EXPLORE               equ FCIDM_GLOBALFIRST+0150h
FCIDM_MENU_FAVORITES             equ FCIDM_GLOBALFIRST+0170h
FCIDM_MENU_FILE                  equ FCIDM_GLOBALFIRST+0000h
FCIDM_MENU_FIND                  equ FCIDM_GLOBALFIRST+0140h
FCIDM_MENU_HELP                  equ FCIDM_GLOBALFIRST+0100h
FCIDM_MENU_TOOLS                 equ FCIDM_GLOBALFIRST+00c0h
FCIDM_MENU_TOOLS_SEP_GOTO        equ FCIDM_GLOBALFIRST+00c1h
FCIDM_MENU_VIEW                  equ FCIDM_GLOBALFIRST+0080h
FCIDM_MENU_VIEW_SEP_OPTIONS      equ FCIDM_GLOBALFIRST+0081h
FCIDM_SHVIEWFIRST                equ 0000h
FCIDM_SHVIEWLAST                 equ 7fffh
FCIDM_STATUS                     equ FCIDM_BROWSERFIRST + 1
FCIDM_TOOLBAR                    equ FCIDM_BROWSERFIRST + 0
FCSM_CLSID                       equ 00000008h
FCSM_FLAGS                       equ 00000040h
FCSM_ICONFILE                    equ 00000010h
FCSM_INFOTIP                     equ 00000004h
FCSM_LOGO                        equ 00000020h
FCSM_VIEWID                      equ 00000001h
FCSM_WEBVIEWTEMPLATE             equ 00000002h
FCS_FLAG_DRAGDROP                equ 2
FCS_FORCEWRITE                   equ 00000002h
FCS_READ                         equ 00000001h
FCS_WRITE                        equ (FCS_READ or FCS_FORCEWRITE)
FOF_NORECURSEREPARSE             equ 8000h
FOF_NORECURSION                  equ 1000h
FOF_NO_CONNECTED_ELEMENTS        equ 2000h
FOF_WANTNUKEWARNING              equ 4000h
FVSIF_CANVIEWIT                  equ 40000000h
FVSIF_NEWFAILED                  equ 08000000h
FVSIF_NEWFILE                    equ 80000000h
FVSIF_PINNED                     equ 00000002h
FVSIF_RECT                       equ 00000001h
GADOF_DIRTY                      equ 00000001h
GETPROPS_NONE                    equ 0000h
GIL_ASYNC                        equ 0020h
GIL_CHECKSHIELD                  equ 0200h
GIL_DEFAULTICON                  equ 0040h
GIL_DONTCACHE                    equ 0010h
GIL_FORCENOSHIELD                equ 0400h
GIL_FORSHELL                     equ 0002h
GIL_FORSHORTCUT                  equ 0080h
GIL_NOTFILENAME                  equ 0008h
GIL_OPENICON                     equ 0001h
GIL_PERCLASS                     equ 0004h
GIL_PERINSTANCE                  equ 0002h
GIL_SHIELD                       equ 0200h
GIL_SIMULATEDOC                  equ 0001h
IDC_OFFLINE_HAND                 equ 103
IDC_PANTOOL_HAND_CLOSED          equ 105
IDC_PANTOOL_HAND_OPEN            equ 104
IDO_SHGIOI_LINK                  equ 0FFFFFFEh
IDO_SHGIOI_SHARE                 equ 0FFFFFFFh
IDO_SHGIOI_SLOWFILE              equ 0FFFFFFFDh
IEIFLAG_ASPECT                   equ 0004h
IEIFLAG_ASYNC                    equ 0001h
IEIFLAG_CACHE                    equ 0002h
IEIFLAG_GLEAM                    equ 0010h
IEIFLAG_OFFLINE                  equ 0008h
IEIFLAG_ORIGSIZE                 equ 0040h
IEIFLAG_SCREEN                   equ 0020h
IRTIR_TASK_FINISHED              equ 4
IRTIR_TASK_NOT_RUNNING           equ 0
IRTIR_TASK_PENDING               equ 3
IRTIR_TASK_RUNNING               equ 1
IRTIR_TASK_SUSPENDED             equ 2
ISFBVIEWMODE_LARGEICONS          equ 0002h
ISFBVIEWMODE_LOGOS               equ 0003h
ISFBVIEWMODE_SMALLICONS          equ 0001h
ISFB_MASK_BKCOLOR                equ 00000002h
ISFB_MASK_COLORS                 equ 00000020h
ISFB_MASK_IDLIST                 equ 00000010h
ISFB_MASK_SHELLFOLDER            equ 00000008h
ISFB_MASK_STATE                  equ 00000001h
ISFB_MASK_VIEWMODE               equ 00000004h
ISFB_STATE_ALLOWRENAME           equ 00000002h
ISFB_STATE_BTNMINSIZE            equ 00000100h
ISFB_STATE_CHANNELBAR            equ 00000010h
ISFB_STATE_DEBOSSED              equ 00000001h
ISFB_STATE_DEFAULT               equ 00000000h
ISFB_STATE_FULLOPEN              equ 00000040h
ISFB_STATE_NONAMESORT            equ 00000080h
ISFB_STATE_NOSHOWTEXT            equ 00000004h
ISFB_STATE_QLINKSMODE            equ 00000020h
ISIOI_ICONFILE                   equ 00000001h
ISIOI_ICONINDEX                  equ 00000002h
IS_FULLSCREEN                    equ 00000002h
IS_NORMAL                        equ 00000001h
IS_SPLIT                         equ 00000004h
IS_VALIDSIZESTATEBITS            equ (IS_NORMAL or IS_SPLIT or IS_FULLSCREEN)
IS_VALIDSTATEBITS                equ (IS_NORMAL or IS_SPLIT or IS_FULLSCREEN or 80000000h or 40000000h
ITSAT_DEFAULT_LPARAM             equ 0ffffffffh
ITSAT_DEFAULT_PRIORITY           equ 10000000h
ITSAT_MAX_PRIORITY               equ 7fffffffh
ITSAT_MIN_PRIORITY               equ 00000000h
ITSSFLAG_COMPLETE_ON_DESTROY     equ 0000h
ITSSFLAG_FLAGS_MASK              equ 0003h
ITSSFLAG_KILL_ON_DESTROY         equ 0001h
ITSSFLAG_SUPPORTS_TERMINATE      equ 0002h
ITSSFLAG_TASK_PLACEINBACK        equ 00000002h
ITSSFLAG_TASK_PLACEINFRONT       equ 00000001h
ITSSFLAG_THREAD_POOL_TIMEOUT     equ 0020h
ITSSFLAG_THREAD_TERMINATE_TIMEOUT equ 0010h
ITSS_THREAD_DESTROY_DEFAULT_TIMEOUT equ 60000
ITSS_THREAD_TERMINATE_TIMEOUT    equ (INFINITE)
ITSS_THREAD_TIMEOUT_NO_CHANGE    equ (INFINITE - 1)
KF_FLAG_CREATE                   equ 00008000h
KF_FLAG_DEFAULT_PATH             equ 00000400h
KF_FLAG_DONT_UNEXPAND            equ 00002000h
KF_FLAG_DONT_VERIFY              equ 00004000h
KF_FLAG_INIT                     equ 00000800h
KF_FLAG_NOT_PARENT_RELATIVE      equ 00000200h
KF_FLAG_NO_ALIAS                 equ 00001000h
KF_FLAG_SIMPLE_IDLIST            equ 00000100h
MAX_COLUMN_DESC_LEN              equ 128
MAX_COLUMN_NAME_LEN              equ 80
MM_ADDSEPARATOR                  equ 00000001h
MM_DONTREMOVESEPS                equ 00000004h
MM_SUBMENUSHAVEIDS               equ 00000002h
NT_CONSOLE_PROPS_SIG             equ A0000002h
NT_FE_CONSOLE_PROPS_SIG          equ A0000004h
NUM_POINTS                       equ 3
OFASI_EDIT                       equ 0001h
OFASI_OPENDESKTOP                equ 0002h
OI_ASYNC                         equ 0FFFFEEEEh
OI_DEFAULT                       equ 00000000h
OPENPROPS_INHIBITPIF             equ 8000h
OPENPROPS_NONE                   equ 0000h
PANE_NAVIGATION                  equ 5
PANE_NONE                        equ -1
PANE_OFFLINE                     equ 2
PANE_PRINTER                     equ 3
PANE_PRIVACY                     equ 7
PANE_PROGRESS                    equ 6
PANE_SSL                         equ 4
PANE_ZONE                        equ 1
PCS_FATAL                        equ 80000000h
PCS_PATHTOOLONG                  equ 00000008h
PCS_REMOVEDCHAR                  equ 00000002h
PCS_REPLACEDCHAR                 equ 00000001h
PCS_TRUNCATED                    equ 00000004h
PDTIMER_PAUSE                    equ 00000002h
PDTIMER_RESET                    equ 00000001h
PDTIMER_RESUME                   equ 00000003h
PIDISF_CACHEDSTICKY              equ 00000002h
PIDISF_CACHEIMAGES               equ 00000010h
PIDISF_FOLLOWALLLINKS            equ 00000020h
PIDISF_RECENTLYCHANGED           equ 00000001h
PIDISM_DONTWATCH                 equ 2
PIDISM_GLOBAL                    equ 0
PIDISM_WATCH                     equ 1
PID_INTSITE_AUTHOR               equ 3
PID_INTSITE_CODEPAGE             equ 18
PID_INTSITE_COMMENT              equ 8
PID_INTSITE_CONTENTCODE          equ 11
PID_INTSITE_CONTENTLEN           equ 10
PID_INTSITE_DESCRIPTION          equ 7
PID_INTSITE_FLAGS                equ 9
PID_INTSITE_ICONFILE             equ 21
PID_INTSITE_ICONINDEX            equ 20
PID_INTSITE_LASTMOD              equ 5
PID_INTSITE_LASTVISIT            equ 4
PID_INTSITE_RECURSE              equ 12
PID_INTSITE_SUBSCRIPTION         equ 14
PID_INTSITE_TITLE                equ 16
PID_INTSITE_TRACKING             equ 19
PID_INTSITE_URL                  equ 15
PID_INTSITE_VISITCOUNT           equ 6
PID_INTSITE_WATCH                equ 13
PID_INTSITE_WHATSNEW             equ 2
PID_IS_AUTHOR                    equ 11
PID_IS_COMMENT                   equ 13
PID_IS_DESCRIPTION               equ 12
PID_IS_HOTKEY                    equ 6
PID_IS_ICONFILE                  equ 9
PID_IS_ICONINDEX                 equ 8
PID_IS_NAME                      equ 4
PID_IS_SHOWCMD                   equ 7
PID_IS_URL                       equ 2
PID_IS_WHATSNEW                  equ 10
PID_IS_WORKINGDIR                equ 5
PIFDEFFILESIZE                   equ 80
PIFDEFPATHSIZE                   equ 64
PIFMAXFILEPATH                   equ 260
PIFNAMESIZE                      equ 30
PIFPARAMSSIZE                    equ 64
PIFSHDATASIZE                    equ 64
PIFSHPROGSIZE                    equ 64
PIFSTARTLOCSIZE                  equ 63
PPCF_ADDARGUMENTS                equ 00000003h
PPCF_ADDQUOTES                   equ 00000001h
PPCF_FORCEQUALIFY                equ 00000040h
PPCF_LONGESTPOSSIBLE             equ 00000080h
PPCF_NODIRECTORIES               equ 00000010h
PRF_DONTFINDLNK                  equ 0008h
PRF_FIRSTDIRDEF                  equ 0004h
PRF_REQUIREABSOLUTE              equ 0010h
PRF_TRYPROGRAMEXTENSIONS         equ (0002h or PRF_VERIFYEXISTS)
PRF_VERIFYEXISTS                 equ 0001h
PROGDLG_AUTOTIME                 equ 00000002h
PROGDLG_MARQUEEPROGRESS          equ 00000020h
PROGDLG_MODAL                    equ 00000001h
PROGDLG_NOCANCEL                 equ 00000040h
PROGDLG_NOMINIMIZE               equ 00000008h
PROGDLG_NOPROGRESSBAR            equ 00000010h
PROGDLG_NORMAL                   equ 00000000h
PROGDLG_NOTIME                   equ 00000004h
QCMINFO_PLACE_AFTER              equ 1
QCMINFO_PLACE_BEFORE             equ 0
QIF_CACHED                       equ 00000001h
QIF_DONTEXPANDFOLDER             equ 00000002h
QITIPF_DEFAULT                   equ 00000000h
QITIPF_LINKNOTARGET              equ 00000002h
QITIPF_LINKUSETARGET             equ 00000004h
QITIPF_SINGLELINE                equ 00000010h
QITIPF_USENAME                   equ 00000001h
QITIPF_USESLOWTIP                equ 00000008h
SBSP_HELPMODE                    equ 0040h
SBSP_NOTRANSFERHIST              equ 0080h
SCHEME_CREATE                    equ 0080h
SCHEME_DISPLAY                   equ 0001h
SCHEME_DONOTUSE                  equ 0040h
SCHEME_EDIT                      equ 0002h
SCHEME_GLOBAL                    equ 0008h
SCHEME_LOCAL                     equ 0004h
SCHEME_REFRESH                   equ 0010h
SCHEME_UPDATE                    equ 0020h
SETPROPS_NONE                    equ 0000h
SFGAO_CANMONIKER                 equ 00400000h
SFVM_ADDOBJECT                   equ 00000003h
SFVM_ADDPROPERTYPAGES            equ 47
SFVM_BACKGROUNDENUM              equ 32
SFVM_BACKGROUNDENUMDONE          equ 48
SFVM_COLUMNCLICK                 equ 24
SFVM_DEFITEMCOUNT                equ 26
SFVM_DEFVIEWMODE                 equ 27
SFVM_DIDDRAGDROP                 equ 36
SFVM_FSNOTIFY                    equ 14
SFVM_GETANIMATION                equ 68
SFVM_GETBUTTONINFO               equ 5
SFVM_GETBUTTONS                  equ 6
SFVM_GETDETAILSOF                equ 23
SFVM_GETHELPTEXT                 equ 3
SFVM_GETHELPTOPIC                equ 63
SFVM_GETNOTIFY                   equ 49
SFVM_GETPANE                     equ 59
SFVM_GETSELECTEDOBJECTS          equ 00000009h
SFVM_GETSORTDEFAULTS             equ 53
SFVM_GETTOOLTIPTEXT              equ 4
SFVM_GETZONE                     equ 58
SFVM_INITMENUPOPUP               equ 7
SFVM_INVOKECOMMAND               equ 2
SFVM_MERGEMENU                   equ 1
SFVM_QUERYFSNOTIFY               equ 25
SFVM_REARRANGE                   equ 00000001h
SFVM_REMOVEOBJECT                equ 00000006h
SFVM_SETCLIPBOARD                equ 00000010h
SFVM_SETISFV                     equ 39
SFVM_SETITEMPOS                  equ 0000000eh
SFVM_SETPOINTS                   equ 00000017h
SFVM_SIZE                        equ 57
SFVM_THISIDLIST                  equ 41
SFVM_UNMERGEMENU                 equ 28
SFVM_UPDATEOBJECT                equ 00000007h
SFVM_UPDATESTATUSBAR             equ 31
SFVM_WINDOWCREATED               equ 15
SFVSOC_INVALIDATE_ALL            equ 00000001h
SFVS_SELECT_ALLITEMS             equ 1h
SFVS_SELECT_INVERT               equ 2h
SFVS_SELECT_NONE                 equ 0h
SHARD_PATHA                      equ 00000002h
SHARD_PATHW                      equ 00000003h
SHARD_PIDL                       equ 00000001h
SHCDF_UPDATEITEM                 equ 00000001h
SHCIDS_ALLFIELDS                 equ 80000000h
SHCIDS_COLUMNMASK                equ 0000FFFFh
SHCNEE_MSI_CHANGE                equ 4
SHCNEE_MSI_UNINSTALL             equ 5
SHCNEE_ORDERCHANGED              equ 2
SHCNE_ALLEVENTS                  equ 7FFFFFFFh
SHCNE_ASSOCCHANGED               equ 08000000h
SHCNE_ATTRIBUTES                 equ 00000800h
SHCNE_CREATE                     equ 00000002h
SHCNE_DELETE                     equ 00000004h
SHCNE_DISKEVENTS                 equ 0002381Fh
SHCNE_DRIVEADD                   equ 00000100h
SHCNE_DRIVEADDGUI                equ 00010000h
SHCNE_DRIVEREMOVED               equ 00000080h
SHCNE_EXTENDED_EVENT             equ 04000000h
SHCNE_FREESPACE                  equ 00040000h
SHCNE_GLOBALEVENTS               equ 0C0581E0h
SHCNE_INTERRUPT                  equ 80000000h
SHCNE_MEDIAINSERTED              equ 00000020h
SHCNE_MEDIAREMOVED               equ 00000040h
SHCNE_MKDIR                      equ 00000008h
SHCNE_NETSHARE                   equ 00000200h
SHCNE_NETUNSHARE                 equ 00000400h
SHCNE_RENAMEFOLDER               equ 00020000h
SHCNE_RENAMEITEM                 equ 00000001h
SHCNE_RMDIR                      equ 00000010h
SHCNE_SERVERDISCONNECT           equ 00004000h
SHCNE_UPDATEDIR                  equ 00001000h
SHCNE_UPDATEIMAGE                equ 00008000h
SHCNE_UPDATEITEM                 equ 00002000h
SHCNF_DWORD                      equ 0003h
SHCNF_FLUSH                      equ 1000h
SHCNF_FLUSHNOWAIT                equ 2000h
SHCNF_IDLIST                     equ 0000h
SHCNF_NOTIFYRECURSIVE            equ 10000h
SHCNF_PATHA                      equ 0001h
SHCNF_PATHW                      equ 0005h
SHCNF_PRINTERA                   equ 0002h
SHCNF_PRINTERW                   equ 0006h
SHCNF_TYPE                       equ 00FFh
SHCNRF_InterruptLevel            equ 0001h
SHCNRF_NewDelivery               equ 8000h
SHCNRF_RecursiveInterrupt        equ 1000h
SHCNRF_ShellLevel                equ 0002h
SHDID_COMPUTER_AUDIO             equ 19
SHDID_COMPUTER_CDROM             equ 10
SHDID_COMPUTER_DRIVE35           equ 5
SHDID_COMPUTER_DRIVE525          equ 6
SHDID_COMPUTER_FIXED             equ 8
SHDID_COMPUTER_IMAGING           equ 18
SHDID_COMPUTER_NETDRIVE          equ 9
SHDID_COMPUTER_OTHER             equ 12
SHDID_COMPUTER_RAMDISK           equ 11
SHDID_COMPUTER_REMOVABLE         equ 7
SHDID_COMPUTER_SHAREDDOCS        equ 20
SHDID_FS_DIRECTORY               equ 3
SHDID_FS_FILE                    equ 2
SHDID_FS_OTHER                   equ 4
SHDID_MOBILE_DEVICE              equ 21
SHDID_NET_DOMAIN                 equ 13
SHDID_NET_OTHER                  equ 17
SHDID_NET_RESTOFNET              equ 16
SHDID_NET_SERVER                 equ 14
SHDID_NET_SHARE                  equ 15
SHDID_ROOT_REGITEM               equ 1
SHELLSTATEVERSION_IE4            equ 9
SHELLSTATEVERSION_WIN2K          equ 10
SHFMT_CANCEL                     equ 0FFFFFFFEh
SHFMT_ERROR                      equ 0FFFFFFFFh
SHFMT_ID_DEFAULT                 equ 0FFFFh
SHFMT_NOFORMAT                   equ 0FFFFFFFDh
SHFMT_OPT_FULL                   equ 0001h
SHFMT_OPT_SYSONLY                equ 0002h
SHGDFIL_DESCRIPTIONID            equ 3
SHGDFIL_FINDDATA                 equ 1
SHGDFIL_NETRESOURCE              equ 2
SHIMSTCAPFLAG_LOCKABLE           equ 0001h
SHIMSTCAPFLAG_PURGEABLE          equ 0002h
SHOP_FILEPATH                    equ 00000002h
SHOP_PRINTERNAME                 equ 00000001h
SHOP_VOLUMEGUID                  equ 00000004h
SHPPFW_ASKDIRCREATE              equ 00000002h
SHPPFW_DEFAULT                   equ SHPPFW_DIRCREATE
SHPPFW_DIRCREATE                 equ 00000001h
SHPPFW_IGNOREFILENAME            equ 00000004h
SHPPFW_MEDIACHECKONLY            equ 00000010h
SHPPFW_NONE                      equ 00000000h
SHPPFW_NOWRITECHECK              equ 00000008h
SIOM_ICONINDEX                   equ 2
SIOM_OVERLAYINDEX                equ 1
SIOM_RESERVED_LINK               equ 1
SIOM_RESERVED_SHARED             equ 0
SIOM_RESERVED_SLOWFILE           equ 2
SSF_AUTOCHECKSELECT              equ 00800000h
SSF_DESKTOPHTML                  equ 00000200h
SSF_DONTPRETTYPATH               equ 00000800h
SSF_DOUBLECLICKINWEBVIEW         equ 00000080h
SSF_FILTER                       equ 00010000h
SSF_HIDDENFILEEXTS               equ 00000004h
SSF_HIDEICONS                    equ 00004000h
SSF_ICONSONLY                    equ 01000000h
SSF_MAPNETDRVBUTTON              equ 00001000h
SSF_NOCONFIRMRECYCLE             equ 00008000h
SSF_NONETCRAWLING                equ 00100000h
SSF_SEPPROCESS                   equ 00080000h
SSF_SERVERADMINUI                equ 00000004h
SSF_SHOWALLOBJECTS               equ 00000001h
SSF_SHOWATTRIBCOL                equ 00000100h
SSF_SHOWCOMPCOLOR                equ 00000008h
SSF_SHOWEXTENSIONS               equ 00000002h
SSF_SHOWINFOTIP                  equ 00002000h
SSF_SHOWSTARTPAGE                equ 00400000h
SSF_SHOWSUPERHIDDEN              equ 00040000h
SSF_SHOWSYSFILES                 equ 00000020h
SSF_SHOWTYPEOVERLAY              equ 02000000h
SSF_SORTCOLUMNS                  equ 00000010h
SSF_STARTPANELON                 equ 00200000h
SSF_WEBVIEW                      equ 00020000h
SSF_WIN95CLASSIC                 equ 00000400h
SSM_CLEAR                        equ 0000h
SSM_REFRESH                      equ 0002h
SSM_SET                          equ 0001h
SSM_UPDATE                       equ 0004h
TBIF_APPEND                      equ 0
TBIF_DEFAULT                     equ 00000000h
TBIF_INTERNETBAR                 equ 00010000h
TBIF_NOTOOLBAR                   equ 00030000h
TBIF_PREPEND                     equ 1
TBIF_REPLACE                     equ 2
TBIF_STANDARDTOOLBAR             equ 00020000h
VALIDATEUNC_CONNECT              equ 0001h
VALIDATEUNC_NOUI                 equ 0002h
VALIDATEUNC_PERSIST              equ 0008h
VALIDATEUNC_PRINT                equ 0004h
VALIDATEUNC_VALID                equ 0007h
WPSTYLE_CENTER                   equ 0
WPSTYLE_MAX                      equ 3
WPSTYLE_STRETCH                  equ 2
WPSTYLE_TILE                     equ 1

;;;; end shlobj

;;;; vfw

comment * ------------------------

    Equates from Video for Windows

        ------------------------ *

BI_1632                     equ 32333631h
ICERR_OK                    equ 0
ICERR_DONTDRAW              equ 1
ICERR_NEWPALETTE            equ 2
ICERR_GOTOKEYFRAME          equ 3
ICERR_STOPDRAWING           equ 4
ICERR_UNSUPPORTED           equ -1
ICERR_BADFORMAT             equ -2
ICERR_MEMORY                equ -3
ICERR_INTERNAL              equ -4
ICERR_BADFLAGS              equ -5
ICERR_BADPARAM              equ -6
ICERR_BADSIZE               equ -7
ICERR_BADHANDLE             equ -8
ICERR_CANTUPDATE            equ -9
ICERR_ABORT                 equ -10
ICERR_ERROR                 equ -100
ICERR_BADBITDEPTH           equ -200
ICERR_BADIMAGESIZE          equ -201
ICERR_CUSTOM                equ -400
ICMODE_COMPRESS             equ 1
ICMODE_DECOMPRESS           equ 2
ICMODE_FASTDECOMPRESS       equ 3
ICMODE_QUERY                equ 4
ICMODE_FASTCOMPRESS         equ 5
ICMODE_DRAW                 equ 8
ICMODE_INTERNALF_FUNCTION32 equ 8000h
ICMODE_INTERNALF_MASK       equ 8000h
AVIIF_LIST                  equ 00000001h
AVIIF_TWOCC                 equ 00000002h
AVIIF_KEYFRAME              equ 00000010h
ICQUALITY_LOW               equ 0
ICQUALITY_HIGH              equ 10000
ICQUALITY_DEFAULT           equ -1
ICM_USER                    equ DRV_USER+0000h
ICM_RESERVED                equ ICM_RESERVED_LOW
ICM_RESERVED_LOW            equ DRV_USER+1000h
ICM_RESERVED_HIGH           equ DRV_USER+2000h
ICM_GETSTATE                equ ICM_RESERVED+0
ICM_SETSTATE                equ ICM_RESERVED+1
ICM_GETINFO                 equ ICM_RESERVED+2
ICM_CONFIGURE               equ ICM_RESERVED+10
ICM_ABOUT                   equ ICM_RESERVED+11
ICM_GETERRORTEXT            equ ICM_RESERVED+12
ICM_GETFORMATNAME           equ ICM_RESERVED+20
ICM_ENUMFORMATS             equ ICM_RESERVED+21
ICM_GETDEFAULTQUALITY       equ ICM_RESERVED+30
ICM_GETQUALITY              equ ICM_RESERVED+31
ICM_SETQUALITY              equ ICM_RESERVED+32
ICM_SET                     equ ICM_RESERVED+40
ICM_GET                     equ ICM_RESERVED+41
ICM_COMPRESS_GET_FORMAT     equ ICM_USER+4
ICM_COMPRESS_GET_SIZE       equ ICM_USER+5
ICM_COMPRESS_QUERY          equ ICM_USER+6
ICM_COMPRESS_BEGIN          equ ICM_USER+7
ICM_COMPRESS                equ ICM_USER+8
ICM_COMPRESS_END            equ ICM_USER+9
ICM_DECOMPRESS_GET_FORMAT   equ ICM_USER+10
ICM_DECOMPRESS_QUERY        equ ICM_USER+11
ICM_DECOMPRESS_BEGIN        equ ICM_USER+12
ICM_DECOMPRESS              equ ICM_USER+13
ICM_DECOMPRESS_END          equ ICM_USER+14
ICM_DECOMPRESS_SET_PALETTE  equ ICM_USER+29
ICM_DECOMPRESS_GET_PALETTE  equ ICM_USER+30
ICM_DRAW_QUERY              equ ICM_USER+31
ICM_DRAW_BEGIN              equ ICM_USER+15
ICM_DRAW_GET_PALETTE        equ ICM_USER+16
ICM_DRAW_UPDATE             equ ICM_USER+17
ICM_DRAW_START              equ ICM_USER+18
ICM_DRAW_STOP               equ ICM_USER+19
ICM_DRAW_BITS               equ ICM_USER+20
ICM_DRAW_END                equ ICM_USER+21
ICM_DRAW_GETTIME            equ ICM_USER+32
ICM_DRAW                    equ ICM_USER+33
ICM_DRAW_WINDOW             equ ICM_USER+34
ICM_DRAW_SETTIME            equ ICM_USER+35
ICM_DRAW_REALIZE            equ ICM_USER+36
ICM_DRAW_FLUSH              equ ICM_USER+37
ICM_DRAW_RENDERBUFFER       equ ICM_USER+38
ICM_DRAW_START_PLAY         equ ICM_USER+39
ICM_DRAW_STOP_PLAY          equ ICM_USER+40
ICM_DRAW_SUGGESTFORMAT      equ ICM_USER+50
ICM_DRAW_CHANGEPALETTE      equ ICM_USER+51
ICM_DRAW_IDLE               equ ICM_USER+52
ICM_GETBUFFERSWANTED        equ ICM_USER+41
ICM_GETDEFAULTKEYFRAMERATE  equ ICM_USER+42
ICM_DECOMPRESSEX_BEGIN      equ ICM_USER+60
ICM_DECOMPRESSEX_QUERY      equ ICM_USER+61
ICM_DECOMPRESSEX            equ ICM_USER+62
ICM_DECOMPRESSEX_END        equ ICM_USER+63
ICM_COMPRESS_FRAMES_INFO    equ ICM_USER+70
ICM_COMPRESS_FRAMES         equ ICM_USER+71
ICM_SET_STATUS_PROC         equ ICM_USER+72
VIDCF_QUALITY               equ 0001h
VIDCF_CRUNCH                equ 0002h
VIDCF_TEMPORAL              equ 0004h
VIDCF_COMPRESSFRAMES        equ 0008h
VIDCF_DRAW                  equ 0010h
VIDCF_FASTTEMPORALC         equ 0020h
VIDCF_FASTTEMPORALD         equ 0080h
ICCOMPRESS_KEYFRAME         equ 00000001h
ICCOMPRESSFRAMES_PADDING    equ 00000001h
ICSTATUS_START              equ 0
ICSTATUS_STATUS             equ 1
ICSTATUS_END                equ 2
ICSTATUS_ERROR              equ 3
ICSTATUS_YIELD              equ 4
ICDECOMPRESS_HURRYUP        equ 80000000h
ICDECOMPRESS_UPDATE         equ 40000000h
ICDECOMPRESS_PREROLL        equ 20000000h
ICDECOMPRESS_NULLFRAME      equ 10000000h
ICDECOMPRESS_NOTKEYFRAME    equ 08000000h
ICDRAW_QUERY                equ 00000001h
ICDRAW_FULLSCREEN           equ 00000002h
ICDRAW_HDC                  equ 00000004h
ICDRAW_ANIMATE              equ 00000008h
ICDRAW_CONTINUE             equ 00000010h
ICDRAW_MEMORYDC             equ 00000020h
ICDRAW_UPDATING             equ 00000040h
ICDRAW_RENDER               equ 00000080h
ICDRAW_BUFFER               equ 00000100h
ICDRAW_HURRYUP              equ 80000000h
ICDRAW_UPDATE               equ 40000000h
ICDRAW_PREROLL              equ 20000000h
ICDRAW_NULLFRAME            equ 10000000h
ICDRAW_NOTKEYFRAME          equ 08000000h
ICINSTALL_UNICODE           equ 8000h
ICINSTALL_FUNCTION          equ 0001h
ICINSTALL_DRIVER            equ 0002h
ICINSTALL_HDRV              equ 0004h
ICINSTALL_DRIVERW           equ 8002h
ICMF_CONFIGURE_QUERY        equ 00000001h
ICMF_ABOUT_QUERY            equ 00000001h
ICMF_COMPVARS_VALID         equ 00000001h
ICMF_CHOOSE_KEYFRAME        equ 0001h
ICMF_CHOOSE_DATARATE        equ 0002h
ICMF_CHOOSE_PREVIEW         equ 0004h
ICMF_CHOOSE_ALLCOMPRESSORS  equ 0008h
DDF_0001                    equ 0001h
DDF_UPDATE                  equ 0002h
DDF_SAME_HDC                equ 0004h
DDF_SAME_DRAW               equ 0008h
DDF_DONTDRAW                equ 0010h
DDF_ANIMATE                 equ 0020h
DDF_BUFFER                  equ 0040h
DDF_JUSTDRAWIT              equ 0080h
DDF_FULLSCREEN              equ 0100h
DDF_BACKGROUNDPAL           equ 0200h
DDF_NOTKEYFRAME             equ 0400h
DDF_HURRYUP                 equ 0800h
DDF_HALFTONE                equ 1000h
DDF_2000                    equ 2000h
DDF_PREROLL                 equ DDF_DONTDRAW
DDF_SAME_DIB                equ DDF_SAME_DRAW
DDF_SAME_SIZE               equ DDF_SAME_DRAW
AVIF_HASINDEX               equ 00000010h
AVIF_MUSTUSEINDEX           equ 00000020h
AVIF_ISINTERLEAVED          equ 00000100h
AVIF_WASCAPTUREFILE         equ 00010000h
AVIF_COPYRIGHTED            equ 00020000h
AVI_HEADERSIZE              equ 2048
AVISF_DISABLED              equ 00000001h
AVISF_VIDEO_PALCHANGES      equ 00010000h
AVIIF_FIRSTPART             equ 00000020h
AVIIF_LASTPART              equ 00000040h
AVIIF_MIDPART               equ AVIIF_LASTPART or AVIIF_FIRSTPART
AVIIF_NOTIME                equ 00000100h
AVIIF_COMPUSE               equ 0FFF0000h
AVIGETFRAMEF_BESTDISPLAYFMT equ 1
AVISTREAMINFO_DISABLED      equ 00000001h
AVISTREAMINFO_FORMATCHANGES equ 00010000h
AVIFILEINFO_HASINDEX        equ 00000010h
AVIFILEINFO_MUSTUSEINDEX    equ 00000020h
AVIFILEINFO_ISINTERLEAVED   equ 00000100h
AVIFILEINFO_WASCAPTUREFILE  equ 00010000h
AVIFILEINFO_COPYRIGHTED     equ 00020000h
AVIFILECAPS_CANREAD         equ 00000001h
AVIFILECAPS_CANWRITE        equ 00000002h
AVIFILECAPS_ALLKEYFRAMES    equ 00000010h
AVIFILECAPS_NOCOMPRESSION   equ 00000020h
AVICOMPRESSF_INTERLEAVE     equ 00000001h
AVICOMPRESSF_DATARATE       equ 00000002h
AVICOMPRESSF_KEYFRAMES      equ 00000004h
AVICOMPRESSF_VALID          equ 00000008h
AVIFILEHANDLER_CANREAD      equ 0001h
AVIFILEHANDLER_CANWRITE     equ 0002h
AVIFILEHANDLER_CANACCEPTNONRGB  equ 0004h
FIND_DIR                    equ 0000000Fh
FIND_NEXT                   equ 00000001h
FIND_PREV                   equ 00000004h
FIND_FROM_START             equ 00000008h
FIND_TYPE                   equ 000000F0h
FIND_KEY                    equ 00000010h
FIND_ANY                    equ 00000020h
FIND_FORMAT                 equ 00000040h
FIND_RET                    equ 0000F000h
FIND_POS                    equ 00000000h
FIND_LENGTH                 equ 00001000h
FIND_OFFSET                 equ 00002000h
FIND_SIZE                   equ 00003000h
FIND_INDEX                  equ 00004000h
AVIStreamFindKeyFrame       equ AVIStreamFindSample
FindKeyFrame                equ FindSample
AVIStreamClose              equ AVIStreamRelease
AVIFileClose                equ AVIFileRelease
AVIStreamInit               equ AVIFileInit
AVIStreamExit               equ AVIFileExit
SEARCH_NEAREST              equ FIND_PREV
SEARCH_BACKWARD             equ FIND_PREV
SEARCH_FORWARD              equ FIND_NEXT
SEARCH_KEY                  equ FIND_KEY
SEARCH_ANY                  equ FIND_ANY
MCIWNDOPENF_NEW             equ 0001h
MCIWNDF_NOAUTOSIZEWINDOW    equ 0001h
MCIWNDF_NOPLAYBAR           equ 0002h
MCIWNDF_NOAUTOSIZEMOVIE     equ 0004h
MCIWNDF_NOMENU              equ 0008h
MCIWNDF_SHOWNAME            equ 0010h
MCIWNDF_SHOWPOS             equ 0020h
MCIWNDF_SHOWMODE            equ 0040h
MCIWNDF_SHOWALL             equ 0070h
MCIWNDF_NOTIFYMODE          equ 0100h
MCIWNDF_NOTIFYPOS           equ 0200h
MCIWNDF_NOTIFYSIZE          equ 0400h
MCIWNDF_NOTIFYERROR         equ 1000h
MCIWNDF_NOTIFYALL           equ 1F00h
MCIWNDF_NOTIFYANSI          equ 0080h
MCIWNDF_NOTIFYMEDIAA        equ 0880h
MCIWNDF_NOTIFYMEDIAW        equ 0800h
MCIWNDF_RECORD              equ 2000h
MCIWNDF_NOERRORDLG          equ 4000h
MCIWNDF_NOOPEN              equ 8000h
MCIWNDM_GETDEVICEID         equ WM_USER+100
MCIWNDM_GETSTART            equ WM_USER+103
MCIWNDM_GETLENGTH           equ WM_USER+104
MCIWNDM_GETEND              equ WM_USER+105
MCIWNDM_EJECT               equ WM_USER+107
MCIWNDM_SETZOOM             equ WM_USER+108
MCIWNDM_GETZOOM             equ WM_USER+109
MCIWNDM_SETVOLUME           equ WM_USER+110
MCIWNDM_GETVOLUME           equ WM_USER+111
MCIWNDM_SETSPEED            equ WM_USER+112
MCIWNDM_GETSPEED            equ WM_USER+113
MCIWNDM_SETREPEAT           equ WM_USER+114
MCIWNDM_GETREPEAT           equ WM_USER+115
MCIWNDM_REALIZE             equ WM_USER+118
MCIWNDM_VALIDATEMEDIA       equ WM_USER+121
MCIWNDM_PLAYFROM            equ WM_USER+122
MCIWNDM_PLAYTO              equ WM_USER+123
MCIWNDM_GETPALETTE          equ WM_USER+126
MCIWNDM_SETPALETTE          equ WM_USER+127
MCIWNDM_SETTIMERS           equ WM_USER+129
MCIWNDM_SETACTIVETIMER      equ WM_USER+130
MCIWNDM_SETINACTIVETIMER    equ WM_USER+131
MCIWNDM_GETACTIVETIMER      equ WM_USER+132
MCIWNDM_GETINACTIVETIMER    equ WM_USER+133
MCIWNDM_CHANGESTYLES        equ WM_USER+135
MCIWNDM_GETSTYLES           equ WM_USER+136
MCIWNDM_GETALIAS            equ WM_USER+137
MCIWNDM_PLAYREVERSE         equ WM_USER+139
MCIWNDM_GET_SOURCE          equ WM_USER+140
MCIWNDM_PUT_SOURCE          equ WM_USER+141
MCIWNDM_GET_DEST            equ WM_USER+142
MCIWNDM_PUT_DEST            equ WM_USER+143
MCIWNDM_CAN_PLAY            equ WM_USER+144
MCIWNDM_CAN_WINDOW          equ WM_USER+145
MCIWNDM_CAN_RECORD          equ WM_USER+146
MCIWNDM_CAN_SAVE            equ WM_USER+147
MCIWNDM_CAN_EJECT           equ WM_USER+148
MCIWNDM_CAN_CONFIG          equ WM_USER+149
MCIWNDM_PALETTEKICK         equ WM_USER+150
MCIWNDM_OPENINTERFACE       equ WM_USER+151
MCIWNDM_SETOWNER            equ WM_USER+152
MCIWNDM_SENDSTRINGA         equ WM_USER+101
MCIWNDM_GETPOSITIONA        equ WM_USER+102
MCIWNDM_GETMODEA            equ WM_USER+106
MCIWNDM_SETTIMEFORMATA      equ WM_USER+119
MCIWNDM_GETTIMEFORMATA      equ WM_USER+120
MCIWNDM_GETFILENAMEA        equ WM_USER+124
MCIWNDM_GETDEVICEA          equ WM_USER+125
MCIWNDM_GETERRORA           equ WM_USER+128
MCIWNDM_NEWA                equ WM_USER+134
MCIWNDM_RETURNSTRINGA       equ WM_USER+138
MCIWNDM_OPENA               equ WM_USER+153
MCIWNDM_SENDSTRINGW         equ WM_USER+201
MCIWNDM_GETPOSITIONW        equ WM_USER+202
MCIWNDM_GETMODEW            equ WM_USER+206
MCIWNDM_SETTIMEFORMATW      equ WM_USER+219
MCIWNDM_GETTIMEFORMATW      equ WM_USER+220
MCIWNDM_GETFILENAMEW        equ WM_USER+224
MCIWNDM_GETDEVICEW          equ WM_USER+225
MCIWNDM_GETERRORW           equ WM_USER+228
MCIWNDM_NEWW                equ WM_USER+234
MCIWNDM_RETURNSTRINGW       equ WM_USER+238
MCIWNDM_OPENW               equ WM_USER+252
MCIWNDM_NOTIFYMODE          equ WM_USER+200
MCIWNDM_NOTIFYPOS           equ WM_USER+201
MCIWNDM_NOTIFYSIZE          equ WM_USER+202
MCIWNDM_NOTIFYMEDIA         equ WM_USER+203
MCIWNDM_NOTIFYERROR         equ WM_USER+205
MCIWND_START                equ -1
MCIWND_END                  equ -2
DV_ERR_OK                   equ 0
DV_ERR_BASE                 equ 1
DV_ERR_NONSPECIFIC          equ DV_ERR_BASE
DV_ERR_BADFORMAT            equ DV_ERR_BASE+1
DV_ERR_STILLPLAYING         equ DV_ERR_BASE+2
DV_ERR_UNPREPARED           equ DV_ERR_BASE+3
DV_ERR_SYNC                 equ DV_ERR_BASE+4
DV_ERR_TOOMANYCHANNELS      equ DV_ERR_BASE+5
DV_ERR_NOTDETECTED          equ DV_ERR_BASE+6
DV_ERR_BADINSTALL           equ DV_ERR_BASE+7
DV_ERR_CREATEPALETTE        equ DV_ERR_BASE+8
DV_ERR_SIZEFIELD            equ DV_ERR_BASE+9
DV_ERR_PARAM1               equ DV_ERR_BASE+10
DV_ERR_PARAM2               equ DV_ERR_BASE+11
DV_ERR_CONFIG1              equ DV_ERR_BASE+12
DV_ERR_CONFIG2              equ DV_ERR_BASE+13
DV_ERR_FLAGS                equ DV_ERR_BASE+14
DV_ERR_13                   equ DV_ERR_BASE+15
DV_ERR_NOTSUPPORTED         equ DV_ERR_BASE+16
DV_ERR_NOMEM                equ DV_ERR_BASE+17
DV_ERR_ALLOCATED            equ DV_ERR_BASE+18
DV_ERR_BADDEVICEID          equ DV_ERR_BASE+19
DV_ERR_INVALHANDLE          equ DV_ERR_BASE+20
DV_ERR_BADERRNUM            equ DV_ERR_BASE+21
DV_ERR_NO_BUFFERS           equ DV_ERR_BASE+22
DV_ERR_MEM_CONFLICT         equ DV_ERR_BASE+23
DV_ERR_IO_CONFLICT          equ DV_ERR_BASE+24
DV_ERR_DMA_CONFLICT         equ DV_ERR_BASE+25
DV_ERR_INT_CONFLICT         equ DV_ERR_BASE+26
DV_ERR_PROTECT_ONLY         equ DV_ERR_BASE+27
DV_ERR_LASTERROR            equ DV_ERR_BASE+27
DV_ERR_USER_MSG             equ DV_ERR_BASE+1000
DV_VM_OPEN                  equ MM_DRVM_OPEN
DV_VM_CLOSE                 equ MM_DRVM_CLOSE
DV_VM_DATA                  equ MM_DRVM_DATA
DV_VM_ERROR                 equ MM_DRVM_ERROR
VHDR_DONE                   equ 00000001h
VHDR_PREPARED               equ 00000002h
VHDR_INQUEUE                equ 00000004h
VHDR_KEYFRAME               equ 00000008h
VHDR_VALID                  equ 0000000Fh
VCAPS_OVERLAY               equ 00000001h
VCAPS_SRC_CAN_CLIP          equ 00000002h
VCAPS_DST_CAN_CLIP          equ 00000004h
VCAPS_CAN_SCALE             equ 00000008h
VIDEO_EXTERNALIN            equ 0001h
VIDEO_EXTERNALOUT           equ 0002h
VIDEO_IN                    equ 0004h
VIDEO_OUT                   equ 0008h
VIDEO_DLG_QUERY             equ 0010h
VIDEO_CONFIGURE_QUERY       equ 8000h
VIDEO_CONFIGURE_SET         equ 1000h
VIDEO_CONFIGURE_GET         equ 2000h
VIDEO_CONFIGURE_QUERYSIZE   equ 0001h
VIDEO_CONFIGURE_CURRENT     equ 0010h
VIDEO_CONFIGURE_NOMINAL     equ 0020h
VIDEO_CONFIGURE_MIN         equ 0040h
VIDEO_CONFIGURE_MAX         equ 0080h
DVM_USER                    equ 4000h
DVM_CONFIGURE_START         equ 1000h
DVM_CONFIGURE_END           equ 1FFFh
DVM_PALETTE                 equ DVM_CONFIGURE_START+1
DVM_FORMAT                  equ DVM_CONFIGURE_START+2
DVM_PALETTERGB555           equ DVM_CONFIGURE_START+3
DVM_SRC_RECT                equ DVM_CONFIGURE_START+4
DVM_DST_RECT                equ DVM_CONFIGURE_START+5
WM_CAP_START                equ WM_USER
WM_CAP_UNICODE_START        equ WM_USER+100
WM_CAP_GET_CAPSTREAMPTR     equ WM_CAP_START+1
WM_CAP_SET_CALLBACK_ERRORW  equ WM_CAP_UNICODE_START+2
WM_CAP_SET_CALLBACK_STATUSW equ WM_CAP_UNICODE_START+3
WM_CAP_SET_CALLBACK_ERRORA  equ WM_CAP_START+2
WM_CAP_SET_CALLBACK_STATUSA equ WM_CAP_START+3
WM_CAP_SET_CALLBACK_YIELD   equ WM_CAP_START+4
WM_CAP_SET_CALLBACK_FRAME   equ WM_CAP_START+5
WM_CAP_SET_CALLBACK_VIDEOSTREAM equ WM_CAP_START+6
WM_CAP_SET_CALLBACK_WAVESTREAM equ WM_CAP_START+7
WM_CAP_GET_USER_DATA        equ WM_CAP_START+8
WM_CAP_SET_USER_DATA        equ WM_CAP_START+9
WM_CAP_DRIVER_CONNECT       equ WM_CAP_START+10
WM_CAP_DRIVER_DISCONNECT    equ WM_CAP_START+11
WM_CAP_DRIVER_GET_NAMEA     equ WM_CAP_START+12
WM_CAP_DRIVER_GET_VERSIONA  equ WM_CAP_START+13
WM_CAP_DRIVER_GET_NAMEW     equ WM_CAP_UNICODE_START+12
WM_CAP_DRIVER_GET_VERSIONW  equ WM_CAP_UNICODE_START+13
WM_CAP_DRIVER_GET_CAPS      equ WM_CAP_START+14
WM_CAP_FILE_SET_CAPTURE_FILEA equ WM_CAP_START+20
WM_CAP_FILE_GET_CAPTURE_FILEA equ WM_CAP_START+21
WM_CAP_FILE_SAVEASA         equ WM_CAP_START+23
WM_CAP_FILE_SAVEDIBA        equ WM_CAP_START+25
WM_CAP_FILE_SET_CAPTURE_FILEW equ WM_CAP_UNICODE_START+20
WM_CAP_FILE_GET_CAPTURE_FILEW equ WM_CAP_UNICODE_START+21
WM_CAP_FILE_SAVEASW         equ WM_CAP_UNICODE_START+23
WM_CAP_FILE_SAVEDIBW        equ WM_CAP_UNICODE_START+25
WM_CAP_FILE_ALLOCATE        equ WM_CAP_START+22
WM_CAP_FILE_SET_INFOCHUNK   equ WM_CAP_START+24
WM_CAP_EDIT_COPY            equ WM_CAP_START+30
WM_CAP_SET_AUDIOFORMAT      equ WM_CAP_START+35
WM_CAP_GET_AUDIOFORMAT      equ WM_CAP_START+36
WM_CAP_DLG_VIDEOFORMAT      equ WM_CAP_START+41
WM_CAP_DLG_VIDEOSOURCE      equ WM_CAP_START+42
WM_CAP_DLG_VIDEODISPLAY     equ WM_CAP_START+43
WM_CAP_GET_VIDEOFORMAT      equ WM_CAP_START+44
WM_CAP_SET_VIDEOFORMAT      equ WM_CAP_START+45
WM_CAP_DLG_VIDEOCOMPRESSION equ WM_CAP_START+46
WM_CAP_SET_PREVIEW          equ WM_CAP_START+50
WM_CAP_SET_OVERLAY          equ WM_CAP_START+51
WM_CAP_SET_PREVIEWRATE      equ WM_CAP_START+52
WM_CAP_SET_SCALE            equ WM_CAP_START+53
WM_CAP_GET_STATUS           equ WM_CAP_START+54
WM_CAP_SET_SCROLL           equ WM_CAP_START+55
WM_CAP_GRAB_FRAME           equ WM_CAP_START+60
WM_CAP_GRAB_FRAME_NOSTOP    equ WM_CAP_START+61
WM_CAP_SEQUENCE             equ WM_CAP_START+62
WM_CAP_SEQUENCE_NOFILE      equ WM_CAP_START+63
WM_CAP_SET_SEQUENCE_SETUP   equ WM_CAP_START+64
WM_CAP_GET_SEQUENCE_SETUP   equ WM_CAP_START+65
WM_CAP_SET_MCI_DEVICEA      equ WM_CAP_START+66
WM_CAP_GET_MCI_DEVICEA      equ WM_CAP_START+67
WM_CAP_SET_MCI_DEVICEW      equ WM_CAP_UNICODE_START+66
WM_CAP_GET_MCI_DEVICEW      equ WM_CAP_UNICODE_START+67
WM_CAP_STOP                 equ WM_CAP_START+68
WM_CAP_ABORT                equ WM_CAP_START+69
WM_CAP_SINGLE_FRAME_OPEN    equ WM_CAP_START+70
WM_CAP_SINGLE_FRAME_CLOSE   equ WM_CAP_START+71
WM_CAP_SINGLE_FRAME         equ WM_CAP_START+72
WM_CAP_PAL_OPENA            equ WM_CAP_START+80
WM_CAP_PAL_SAVEA            equ WM_CAP_START+81
WM_CAP_PAL_OPENW            equ WM_CAP_UNICODE_START+80
WM_CAP_PAL_SAVEW            equ WM_CAP_UNICODE_START+81
WM_CAP_PAL_PASTE            equ WM_CAP_START+82
WM_CAP_PAL_AUTOCREATE       equ WM_CAP_START+83
WM_CAP_PAL_MANUALCREATE     equ WM_CAP_START+84
WM_CAP_SET_CALLBACK_CAPCONTROL equ WM_CAP_START+85
WM_CAP_UNICODE_END          equ WM_CAP_PAL_SAVEW
WM_CAP_END                  equ WM_CAP_UNICODE_END
AVSTREAMMASTER_AUDIO        equ 0
AVSTREAMMASTER_NONE         equ 1
CONTROLCALLBACK_PREROLL     equ 1
CONTROLCALLBACK_CAPTURING   equ 2
IDS_CAP_BEGIN               equ 300
IDS_CAP_END                 equ 301
IDS_CAP_INFO                equ 401
IDS_CAP_OUTOFMEM            equ 402
IDS_CAP_FILEEXISTS          equ 403
IDS_CAP_ERRORPALOPEN        equ 404
IDS_CAP_ERRORPALSAVE        equ 405
IDS_CAP_ERRORDIBSAVE        equ 406
IDS_CAP_DEFAVIEXT           equ 407
IDS_CAP_DEFPALEXT           equ 408
IDS_CAP_CANTOPEN            equ 409
IDS_CAP_SEQ_MSGSTART        equ 410
IDS_CAP_SEQ_MSGSTOP         equ 411
IDS_CAP_VIDEDITERR          equ 412
IDS_CAP_READONLYFILE        equ 413
IDS_CAP_WRITEERROR          equ 414
IDS_CAP_NODISKSPACE         equ 415
IDS_CAP_SETFILESIZE         equ 416
IDS_CAP_SAVEASPERCENT       equ 417
IDS_CAP_DRIVER_ERROR        equ 418
IDS_CAP_WAVE_OPEN_ERROR     equ 419
IDS_CAP_WAVE_ALLOC_ERROR    equ 420
IDS_CAP_WAVE_PREPARE_ERROR  equ 421
IDS_CAP_WAVE_ADD_ERROR      equ 422
IDS_CAP_WAVE_SIZE_ERROR     equ 423
IDS_CAP_VIDEO_OPEN_ERROR    equ 424
IDS_CAP_VIDEO_ALLOC_ERROR   equ 425
IDS_CAP_VIDEO_PREPARE_ERROR equ 426
IDS_CAP_VIDEO_ADD_ERROR     equ 427
IDS_CAP_VIDEO_SIZE_ERROR    equ 428
IDS_CAP_FILE_OPEN_ERROR     equ 429
IDS_CAP_FILE_WRITE_ERROR    equ 430
IDS_CAP_RECORDING_ERROR     equ 431
IDS_CAP_RECORDING_ERROR2    equ 432
IDS_CAP_AVI_INIT_ERROR      equ 433
IDS_CAP_NO_FRAME_CAP_ERROR  equ 434
IDS_CAP_NO_PALETTE_WARN     equ 435
IDS_CAP_MCI_CONTROL_ERROR   equ 436
IDS_CAP_MCI_CANT_STEP_ERROR equ 437
IDS_CAP_NO_AUDIO_CAP_ERROR  equ 438
IDS_CAP_AVI_DRAWDIB_ERROR   equ 439
IDS_CAP_COMPRESSOR_ERROR    equ 440
IDS_CAP_AUDIO_DROP_ERROR    equ 441
IDS_CAP_AUDIO_DROP_COMPERROR equ 442
IDS_CAP_STAT_LIVE_MODE      equ 500
IDS_CAP_STAT_OVERLAY_MODE   equ 501
IDS_CAP_STAT_CAP_INIT       equ 502
IDS_CAP_STAT_CAP_FINI       equ 503
IDS_CAP_STAT_PALETTE_BUILD  equ 504
IDS_CAP_STAT_OPTPAL_BUILD   equ 505
IDS_CAP_STAT_I_FRAMES       equ 506
IDS_CAP_STAT_L_FRAMES       equ 507
IDS_CAP_STAT_CAP_L_FRAMES   equ 508
IDS_CAP_STAT_CAP_AUDIO      equ 509
IDS_CAP_STAT_VIDEOCURRENT   equ 510
IDS_CAP_STAT_VIDEOAUDIO     equ 511
IDS_CAP_STAT_VIDEOONLY      equ 512
IDS_CAP_STAT_FRAMESDROPPED  equ 513

;;;; end vfw


;;;; wintrust

WTPF_TRUSTTEST                   equ 00000020h
WTPF_TESTCANBEVALID              equ 00000080h
WTPF_IGNOREEXPIRATION            equ 00000100h
WTPF_IGNOREREVOKATION            equ 00000200h
WTPF_OFFLINEOK_IND               equ 00000400h
WTPF_OFFLINEOK_COM               equ 00000800h
WTPF_OFFLINEOKNBU_IND            equ 00001000h
WTPF_OFFLINEOKNBU_COM            equ 00002000h
WTPF_VERIFY_V1_OFF               equ 00010000h
WTPF_IGNOREREVOCATIONONTS        equ 00020000h
WTPF_ALLOWONLYPERTRUST           equ 00040000h
TRUSTERROR_STEP_WVTPARAMS        equ 0
TRUSTERROR_STEP_FILEIO           equ 2
TRUSTERROR_STEP_SIP              equ 3
TRUSTERROR_STEP_SIPSUBJINFO      equ 5
TRUSTERROR_STEP_CATALOGFILE      equ 6
TRUSTERROR_STEP_CERTSTORE        equ 7
TRUSTERROR_STEP_MESSAGE          equ 8
TRUSTERROR_STEP_MSG_SIGNERCOUNT  equ 9
TRUSTERROR_STEP_MSG_INNERCNTTYPE equ 10
TRUSTERROR_STEP_MSG_INNERCNT     equ 11
TRUSTERROR_STEP_MSG_STORE        equ 12
TRUSTERROR_STEP_MSG_SIGNERINFO   equ 13
TRUSTERROR_STEP_MSG_SIGNERCERT   equ 14
TRUSTERROR_STEP_MSG_CERTCHAIN    equ 15
TRUSTERROR_STEP_MSG_COUNTERSIGINFO equ 16
TRUSTERROR_STEP_MSG_COUNTERSIGCERT equ 17
TRUSTERROR_STEP_VERIFY_MSGHASH   equ 18
TRUSTERROR_STEP_VERIFY_MSGINDIRECTDATA equ 19
TRUSTERROR_STEP_FINAL_WVTINIT    equ 30
TRUSTERROR_STEP_FINAL_INITPROV   equ 31
TRUSTERROR_STEP_FINAL_OBJPROV    equ 32
TRUSTERROR_STEP_FINAL_SIGPROV    equ 33
TRUSTERROR_STEP_FINAL_CERTPROV   equ 34
TRUSTERROR_STEP_FINAL_CERTCHKPROV equ 35
TRUSTERROR_STEP_FINAL_POLICYPROV equ 36
TRUSTERROR_STEP_FINAL_UIPROV     equ 37
TRUSTERROR_MAX_STEPS             equ 38
WT_CURRENT_VERSION               equ 00000200h
WT_ADD_ACTION_ID_RET_RESULT_FLAG equ 1h
DWACTION_ALLOCANDFILL            equ 1
DWACTION_FREE                    equ 2
SPC_SP_AGENCY_INFO_STRUCT        equ 2000
SPC_MINIMAL_CRITERIA_STRUCT      equ 2001
SPC_FINANCIAL_CRITERIA_STRUCT    equ 2002
SPC_INDIRECT_DATA_CONTENT_STRUCT equ 2003
SPC_PE_IMAGE_DATA_STRUCT         equ 2004
SPC_LINK_STRUCT                  equ 2005
SPC_STATEMENT_TYPE_STRUCT        equ 2006
SPC_SP_OPUS_INFO_STRUCT          equ 2007
SPC_CAB_DATA_STRUCT              equ 2008
SPC_JAVA_CLASS_DATA_STRUCT       equ 2009
SPC_SIGINFO_STRUCT               equ 2130
CAT_NAMEVALUE_STRUCT             equ 2221
CAT_MEMBERINFO_STRUCT            equ 2222
SPC_UUID_LENGTH                  equ 16
WIN_CERT_REVISION_1_0            equ 0100h
WIN_CERT_REVISION_2_0            equ 0200h
WIN_CERT_TYPE_X509               equ 0001h
WIN_CERT_TYPE_PKCS_SIGNED_DATA   equ 0002h
WIN_CERT_TYPE_RESERVED_1         equ 0003h
WIN_CERT_TYPE_TS_STACK_SIGNED    equ 0004h

;;;; end wintrust


;;;; winspool

DSPRINT_PUBLISH                  equ 00000001h
DSPRINT_UPDATE                   equ 00000002h
DSPRINT_UNPUBLISH                equ 00000004h
DSPRINT_REPUBLISH                equ 00000008h
DSPRINT_PENDING                  equ 80000000h
PRINTER_STATUS_SERVER_UNKNOWN    equ 00800000h
PRINTER_STATUS_POWER_SAVE        equ 01000000h
PRINTER_ATTRIBUTE_ENABLE_DEVQ    equ 00000080h
PRINTER_ATTRIBUTE_KEEPPRINTEDJOBS equ 00000100h
PRINTER_ATTRIBUTE_DO_COMPLETE_FIRST equ 00000200h
PRINTER_ATTRIBUTE_RAW_ONLY       equ 00001000h
PRINTER_ATTRIBUTE_PUBLISHED      equ 00002000h
PRINTER_ATTRIBUTE_FAX            equ 00004000h
PRINTER_ATTRIBUTE_TS             equ 00008000h
JOB_CONTROL_SENT_TO_PRINTER      equ 6
JOB_CONTROL_LAST_PAGE_EJECTED    equ 7
JOB_STATUS_DELETED               equ 00000100h
JOB_STATUS_BLOCKED_DEVQ          equ 00000200h
JOB_STATUS_RESTART               equ 00000800h
JOB_STATUS_COMPLETE              equ 00001000h
DRIVER_KERNELMODE                equ 00000001h
DRIVER_USERMODE                  equ 00000002h
DPD_DELETE_UNUSED_FILES          equ 00000001h
DPD_DELETE_SPECIFIC_VERSION      equ 00000002h
DPD_DELETE_ALL_FILES             equ 00000004h
APD_STRICT_UPGRADE               equ 00000001h
APD_STRICT_DOWNGRADE             equ 00000002h
APD_COPY_ALL_FILES               equ 00000004h
APD_COPY_NEW_FILES               equ 00000008h
APD_COPY_FROM_DIRECTORY          equ 00000010h
DI_MEMORYMAP_WRITE               equ 00000001h
FORM_USER                        equ 00000000h
FORM_PRINTER                     equ 00000002h
NORMAL_PRINT                     equ 00000000h
REVERSE_PRINT                    equ 00000001h
PORT_STATUS_TYPE_ERROR           equ 1
PORT_STATUS_TYPE_WARNING         equ 2
PORT_STATUS_TYPE_INFO            equ 3
PORT_STATUS_OFFLINE              equ 1
PORT_STATUS_PAPER_JAM            equ 2
PORT_STATUS_PAPER_OUT            equ 3
PORT_STATUS_OUTPUT_BIN_FULL      equ 4
PORT_STATUS_PAPER_PROBLEM        equ 5
PORT_STATUS_NO_TONER             equ 6
PORT_STATUS_DOOR_OPEN            equ 7
PORT_STATUS_USER_INTERVENTION    equ 8
PORT_STATUS_OUT_OF_MEMORY        equ 9
PORT_STATUS_TONER_LOW            equ 10
PORT_STATUS_WARMING_UP           equ 11
PORT_STATUS_POWER_SAVE           equ 12
PRINTER_ENUM_HIDE                equ 01000000h
SPOOL_FILE_PERSISTENT            equ 00000001h
SPOOL_FILE_TEMPORARY             equ 00000002h
PRINTER_NOTIFY_TYPE              equ 00h
JOB_NOTIFY_TYPE                  equ 01h
PRINTER_NOTIFY_FIELD_SERVER_NAME equ 00h
PRINTER_NOTIFY_FIELD_PRINTER_NAME equ 01h
PRINTER_NOTIFY_FIELD_SHARE_NAME  equ 02h
PRINTER_NOTIFY_FIELD_PORT_NAME   equ 03h
PRINTER_NOTIFY_FIELD_DRIVER_NAME equ 04h
PRINTER_NOTIFY_FIELD_COMMENT     equ 05h
PRINTER_NOTIFY_FIELD_LOCATION    equ 06h
PRINTER_NOTIFY_FIELD_DEVMODE     equ 07h
PRINTER_NOTIFY_FIELD_SEPFILE     equ 08h
PRINTER_NOTIFY_FIELD_PRINT_PROCESSOR equ 09h
PRINTER_NOTIFY_FIELD_PARAMETERS  equ 0Ah
PRINTER_NOTIFY_FIELD_DATATYPE    equ 0Bh
PRINTER_NOTIFY_FIELD_SECURITY_DESCRIPTOR equ 0Ch
PRINTER_NOTIFY_FIELD_ATTRIBUTES  equ 0Dh
PRINTER_NOTIFY_FIELD_PRIORITY    equ 0Eh
PRINTER_NOTIFY_FIELD_DEFAULT_PRIORITY equ 0Fh
PRINTER_NOTIFY_FIELD_START_TIME  equ 10h
PRINTER_NOTIFY_FIELD_UNTIL_TIME  equ 11h
PRINTER_NOTIFY_FIELD_STATUS      equ 12h
PRINTER_NOTIFY_FIELD_STATUS_STRING equ 13h
PRINTER_NOTIFY_FIELD_CJOBS       equ 14h
PRINTER_NOTIFY_FIELD_AVERAGE_PPM equ 15h
PRINTER_NOTIFY_FIELD_TOTAL_PAGES equ 16h
PRINTER_NOTIFY_FIELD_PAGES_PRINTED equ 17h
PRINTER_NOTIFY_FIELD_TOTAL_BYTES equ 18h
PRINTER_NOTIFY_FIELD_BYTES_PRINTED equ 19h
PRINTER_NOTIFY_FIELD_OBJECT_GUID equ 1Ah
JOB_NOTIFY_FIELD_PRINTER_NAME    equ 00h
JOB_NOTIFY_FIELD_MACHINE_NAME    equ 01h
JOB_NOTIFY_FIELD_PORT_NAME       equ 02h
JOB_NOTIFY_FIELD_USER_NAME       equ 03h
JOB_NOTIFY_FIELD_NOTIFY_NAME     equ 04h
JOB_NOTIFY_FIELD_DATATYPE        equ 05h
JOB_NOTIFY_FIELD_PRINT_PROCESSOR equ 06h
JOB_NOTIFY_FIELD_PARAMETERS      equ 07h
JOB_NOTIFY_FIELD_DRIVER_NAME     equ 08h
JOB_NOTIFY_FIELD_DEVMODE         equ 09h
JOB_NOTIFY_FIELD_STATUS          equ 0Ah
JOB_NOTIFY_FIELD_STATUS_STRING   equ 0Bh
JOB_NOTIFY_FIELD_SECURITY_DESCRIPTOR equ 0Ch
JOB_NOTIFY_FIELD_DOCUMENT        equ 0Dh
JOB_NOTIFY_FIELD_PRIORITY        equ 0Eh
JOB_NOTIFY_FIELD_POSITION        equ 0Fh
JOB_NOTIFY_FIELD_SUBMITTED       equ 10h
JOB_NOTIFY_FIELD_START_TIME      equ 11h
JOB_NOTIFY_FIELD_UNTIL_TIME      equ 12h
JOB_NOTIFY_FIELD_TIME            equ 13h
JOB_NOTIFY_FIELD_TOTAL_PAGES     equ 14h
JOB_NOTIFY_FIELD_PAGES_PRINTED   equ 15h
JOB_NOTIFY_FIELD_TOTAL_BYTES     equ 16h
JOB_NOTIFY_FIELD_BYTES_PRINTED   equ 17h
PRINTER_NOTIFY_OPTIONS_REFRESH   equ 01h
PRINTER_NOTIFY_INFO_DISCARDED    equ 01h
BIDI_ACCESS_ADMINISTRATOR        equ 1h
BIDI_ACCESS_USER                 equ 2h
ERROR_BIDI_STATUS_OK             equ 0
ERROR_BIDI_NOT_SUPPORTED         equ ERROR_NOT_SUPPORTED
ERROR_BIDI_ERROR_BASE            equ 13000
ERROR_BIDI_STATUS_WARNING        equ (ERROR_BIDI_ERROR_BASE + 1)
ERROR_BIDI_SCHEMA_READ_ONLY      equ (ERROR_BIDI_ERROR_BASE + 2)
ERROR_BIDI_SERVER_OFFLINE        equ (ERROR_BIDI_ERROR_BASE + 3)
ERROR_BIDI_DEVICE_OFFLINE        equ (ERROR_BIDI_ERROR_BASE + 4)
ERROR_BIDI_SCHEMA_NOT_SUPPORTED  equ (ERROR_BIDI_ERROR_BASE + 5)
PRINTER_CHANGE_FAILED_CONNECTION_PRINTER equ 00000008h
PRINTER_CHANGE_SET_PRINTER_DRIVER equ 20000000h

;;;; end winspool

;;;; opengl

; ------------------------- equates for opengl ----------------------------

GL_VERSION_1_1                     equ  1
GL_ACCUM                           equ  00100h
GL_LOAD                            equ  00101h
GL_RETURN                          equ  00102h
GL_MULT                            equ  00103h
GL_ADD                             equ  00104h
GL_NEVER                           equ  00200h
GL_LESS                            equ  00201h
GL_EQUAL                           equ  00202h
GL_LEQUAL                          equ  00203h
GL_GREATER                         equ  00204h
GL_NOTEQUAL                        equ  00205h
GL_GEQUAL                          equ  00206h
GL_ALWAYS                          equ  00207h
GL_CURRENT_BIT                     equ  000000001h
GL_POINT_BIT                       equ  000000002h
GL_LINE_BIT                        equ  000000004h
GL_POLYGON_BIT                     equ  000000008h
GL_POLYGON_STIPPLE_BIT             equ  000000010h
GL_PIXEL_MODE_BIT                  equ  000000020h
GL_LIGHTING_BIT                    equ  000000040h
GL_FOG_BIT                         equ  000000080h
GL_DEPTH_BUFFER_BIT                equ  000000100h
GL_ACCUM_BUFFER_BIT                equ  000000200h
GL_STENCIL_BUFFER_BIT              equ  000000400h
GL_VIEWPORT_BIT                    equ  000000800h
GL_TRANSFORM_BIT                   equ  000001000h
GL_ENABLE_BIT                      equ  000002000h
GL_COLOR_BUFFER_BIT                equ  000004000h
GL_HINT_BIT                        equ  000008000h
GL_EVAL_BIT                        equ  000010000h
GL_LIST_BIT                        equ  000020000h
GL_TEXTURE_BIT                     equ  000040000h
GL_SCISSOR_BIT                     equ  000080000h
GL_ALL_ATTRIB_BITS                 equ  0000fffffh
GL_POINTS                          equ  00000h
GL_LINES                           equ  00001h
GL_LINE_LOOP                       equ  00002h
GL_LINE_STRIP                      equ  00003h
GL_TRIANGLES                       equ  00004h
GL_TRIANGLE_STRIP                  equ  00005h
GL_TRIANGLE_FAN                    equ  00006h
GL_QUADS                           equ  00007h
GL_QUAD_STRIP                      equ  00008h
GL_POLYGON                         equ  00009h
GL_ZERO                            equ  0
GL_ONE                             equ  1
GL_SRC_COLOR                       equ  00300h
GL_ONE_MINUS_SRC_COLOR             equ  00301h
GL_SRC_ALPHA                       equ  00302h
GL_ONE_MINUS_SRC_ALPHA             equ  00303h
GL_DST_ALPHA                       equ  00304h
GL_ONE_MINUS_DST_ALPHA             equ  00305h
GL_DST_COLOR                       equ  00306h
GL_ONE_MINUS_DST_COLOR             equ  00307h
GL_SRC_ALPHA_SATURATE              equ  00308h
GL_TRUE                            equ  1
GL_FALSE                           equ  0
GL_CLIP_PLANE0                     equ  03000h
GL_CLIP_PLANE1                     equ  03001h
GL_CLIP_PLANE2                     equ  03002h
GL_CLIP_PLANE3                     equ  03003h
GL_CLIP_PLANE4                     equ  03004h
GL_CLIP_PLANE5                     equ  03005h
GL_BYTE                            equ  01400h
GL_UNSIGNED_BYTE                   equ  01401h
GL_SHORT                           equ  01402h
GL_UNSIGNED_SHORT                  equ  01403h
GL_INT                             equ  01404h
GL_UNSIGNED_INT                    equ  01405h
GL_FLOAT                           equ  01406h
GL_2_BYTES                         equ  01407h
GL_3_BYTES                         equ  01408h
GL_4_BYTES                         equ  01409h
GL_DOUBLE                          equ  0140Ah
GL_NONE                            equ  0
GL_FRONT_LEFT                      equ  00400h
GL_FRONT_RIGHT                     equ  00401h
GL_BACK_LEFT                       equ  00402h
GL_BACK_RIGHT                      equ  00403h
GL_FRONT                           equ  00404h
GL_BACK                            equ  00405h
GL_LEFT                            equ  00406h
GL_RIGHT                           equ  00407h
GL_FRONT_AND_BACK                  equ  00408h
GL_AUX0                            equ  00409h
GL_AUX1                            equ  0040Ah
GL_AUX2                            equ  0040Bh
GL_AUX3                            equ  0040Ch
GL_NO_ERROR                        equ  0
GL_INVALID_ENUM                    equ  00500h
GL_INVALID_VALUE                   equ  00501h
GL_INVALID_OPERATION               equ  00502h
GL_STACK_OVERFLOW                  equ  00503h
GL_STACK_UNDERFLOW                 equ  00504h
GL_OUT_OF_MEMORY                   equ  00505h
GL_2D                              equ  00600h
GL_3D                              equ  00601h
GL_3D_COLOR                        equ  00602h
GL_3D_COLOR_TEXTURE                equ  00603h
GL_4D_COLOR_TEXTURE                equ  00604h
GL_PASS_THROUGH_TOKEN              equ  00700h
GL_POINT_TOKEN                     equ  00701h
GL_LINE_TOKEN                      equ  00702h
GL_POLYGON_TOKEN                   equ  00703h
GL_BITMAP_TOKEN                    equ  00704h
GL_DRAW_PIXEL_TOKEN                equ  00705h
GL_COPY_PIXEL_TOKEN                equ  00706h
GL_LINE_RESET_TOKEN                equ  00707h
GL_EXP                             equ  00800h
GL_EXP2                            equ  00801h
GL_CW                              equ  00900h
GL_CCW                             equ  00901h
GL_COEFF                           equ  00A00h
GL_ORDER                           equ  00A01h
GL_DOMAIN                          equ  00A02h
GL_CURRENT_COLOR                   equ  00B00h
GL_CURRENT_INDEX                   equ  00B01h
GL_CURRENT_NORMAL                  equ  00B02h
GL_CURRENT_TEXTURE_COORDS          equ  00B03h
GL_CURRENT_RASTER_COLOR            equ  00B04h
GL_CURRENT_RASTER_INDEX            equ  00B05h
GL_CURRENT_RASTER_TEXTURE_COORDS   equ  00B06h
GL_CURRENT_RASTER_POSITION         equ  00B07h
GL_CURRENT_RASTER_POSITION_VALID   equ  00B08h
GL_CURRENT_RASTER_DISTANCE         equ  00B09h
GL_POINT_SMOOTH                    equ  00B10h
GL_POINT_SIZE                      equ  00B11h
GL_POINT_SIZE_RANGE                equ  00B12h
GL_POINT_SIZE_GRANULARITY          equ  00B13h
GL_LINE_SMOOTH                     equ  00B20h
GL_LINE_WIDTH                      equ  00B21h
GL_LINE_WIDTH_RANGE                equ  00B22h
GL_LINE_WIDTH_GRANULARITY          equ  00B23h
GL_LINE_STIPPLE                    equ  00B24h
GL_LINE_STIPPLE_PATTERN            equ  00B25h
GL_LINE_STIPPLE_REPEAT             equ  00B26h
GL_LIST_MODE                       equ  00B30h
GL_MAX_LIST_NESTING                equ  00B31h
GL_LIST_BASE                       equ  00B32h
GL_LIST_INDEX                      equ  00B33h
GL_POLYGON_MODE                    equ  00B40h
GL_POLYGON_SMOOTH                  equ  00B41h
GL_POLYGON_STIPPLE                 equ  00B42h
GL_EDGE_FLAG                       equ  00B43h
GL_CULL_FACE                       equ  00B44h
GL_CULL_FACE_MODE                  equ  00B45h
GL_FRONT_FACE                      equ  00B46h
GL_LIGHTING                        equ  00B50h
GL_LIGHT_MODEL_LOCAL_VIEWER        equ  00B51h
GL_LIGHT_MODEL_TWO_SIDE            equ  00B52h
GL_LIGHT_MODEL_AMBIENT             equ  00B53h
GL_SHADE_MODEL                     equ  00B54h
GL_COLOR_MATERIAL_FACE             equ  00B55h
GL_COLOR_MATERIAL_PARAMETER        equ  00B56h
GL_COLOR_MATERIAL                  equ  00B57h
GL_FOG                             equ  00B60h
GL_FOG_INDEX                       equ  00B61h
GL_FOG_DENSITY                     equ  00B62h
GL_FOG_START                       equ  00B63h
GL_FOG_END                         equ  00B64h
GL_FOG_MODE                        equ  00B65h
GL_FOG_COLOR                       equ  00B66h
GL_DEPTH_RANGE                     equ  00B70h
GL_DEPTH_TEST                      equ  00B71h
GL_DEPTH_WRITEMASK                 equ  00B72h
GL_DEPTH_CLEAR_VALUE               equ  00B73h
GL_DEPTH_FUNC                      equ  00B74h
GL_ACCUM_CLEAR_VALUE               equ  00B80h
GL_STENCIL_TEST                    equ  00B90h
GL_STENCIL_CLEAR_VALUE             equ  00B91h
GL_STENCIL_FUNC                    equ  00B92h
GL_STENCIL_VALUE_MASK              equ  00B93h
GL_STENCIL_FAIL                    equ  00B94h
GL_STENCIL_PASS_DEPTH_FAIL         equ  00B95h
GL_STENCIL_PASS_DEPTH_PASS         equ  00B96h
GL_STENCIL_REF                     equ  00B97h
GL_STENCIL_WRITEMASK               equ  00B98h
GL_MATRIX_MODE                     equ  00BA0h
GL_NORMALIZE                       equ  00BA1h
GL_VIEWPORT                        equ  00BA2h
GL_MODELVIEW_STACK_DEPTH           equ  00BA3h
GL_PROJECTION_STACK_DEPTH          equ  00BA4h
GL_TEXTURE_STACK_DEPTH             equ  00BA5h
GL_MODELVIEW_MATRIX                equ  00BA6h
GL_PROJECTION_MATRIX               equ  00BA7h
GL_TEXTURE_MATRIX                  equ  00BA8h
GL_ATTRIB_STACK_DEPTH              equ  00BB0h
GL_CLIENT_ATTRIB_STACK_DEPTH       equ  00BB1h
GL_ALPHA_TEST                      equ  00BC0h
GL_ALPHA_TEST_FUNC                 equ  00BC1h
GL_ALPHA_TEST_REF                  equ  00BC2h
GL_DITHER                          equ  00BD0h
GL_BLEND_DST                       equ  00BE0h
GL_BLEND_SRC                       equ  00BE1h
GL_BLEND                           equ  00BE2h
GL_LOGIC_OP_MODE                   equ  00BF0h
GL_INDEX_LOGIC_OP                  equ  00BF1h
GL_COLOR_LOGIC_OP                  equ  00BF2h
GL_AUX_BUFFERS                     equ  00C00h
GL_DRAW_BUFFER                     equ  00C01h
GL_READ_BUFFER                     equ  00C02h
GL_SCISSOR_BOX                     equ  00C10h
GL_SCISSOR_TEST                    equ  00C11h
GL_INDEX_CLEAR_VALUE               equ  00C20h
GL_INDEX_WRITEMASK                 equ  00C21h
GL_COLOR_CLEAR_VALUE               equ  00C22h
GL_COLOR_WRITEMASK                 equ  00C23h
GL_INDEX_MODE                      equ  00C30h
GL_RGBA_MODE                       equ  00C31h
GL_DOUBLEBUFFER                    equ  00C32h
GL_STEREO                          equ  00C33h
GL_RENDER_MODE                     equ  00C40h
GL_PERSPECTIVE_CORRECTION_HINT     equ  00C50h
GL_POINT_SMOOTH_HINT               equ  00C51h
GL_LINE_SMOOTH_HINT                equ  00C52h
GL_POLYGON_SMOOTH_HINT             equ  00C53h
GL_FOG_HINT                        equ  00C54h
GL_TEXTURE_GEN_S                   equ  00C60h
GL_TEXTURE_GEN_T                   equ  00C61h
GL_TEXTURE_GEN_R                   equ  00C62h
GL_TEXTURE_GEN_Q                   equ  00C63h
GL_PIXEL_MAP_I_TO_I                equ  00C70h
GL_PIXEL_MAP_S_TO_S                equ  00C71h
GL_PIXEL_MAP_I_TO_R                equ  00C72h
GL_PIXEL_MAP_I_TO_G                equ  00C73h
GL_PIXEL_MAP_I_TO_B                equ  00C74h
GL_PIXEL_MAP_I_TO_A                equ  00C75h
GL_PIXEL_MAP_R_TO_R                equ  00C76h
GL_PIXEL_MAP_G_TO_G                equ  00C77h
GL_PIXEL_MAP_B_TO_B                equ  00C78h
GL_PIXEL_MAP_A_TO_A                equ  00C79h
GL_PIXEL_MAP_I_TO_I_SIZE           equ  00CB0h
GL_PIXEL_MAP_S_TO_S_SIZE           equ  00CB1h
GL_PIXEL_MAP_I_TO_R_SIZE           equ  00CB2h
GL_PIXEL_MAP_I_TO_G_SIZE           equ  00CB3h
GL_PIXEL_MAP_I_TO_B_SIZE           equ  00CB4h
GL_PIXEL_MAP_I_TO_A_SIZE           equ  00CB5h
GL_PIXEL_MAP_R_TO_R_SIZE           equ  00CB6h
GL_PIXEL_MAP_G_TO_G_SIZE           equ  00CB7h
GL_PIXEL_MAP_B_TO_B_SIZE           equ  00CB8h
GL_PIXEL_MAP_A_TO_A_SIZE           equ  00CB9h
GL_UNPACK_SWAP_BYTES               equ  00CF0h
GL_UNPACK_LSB_FIRST                equ  00CF1h
GL_UNPACK_ROW_LENGTH               equ  00CF2h
GL_UNPACK_SKIP_ROWS                equ  00CF3h
GL_UNPACK_SKIP_PIXELS              equ  00CF4h
GL_UNPACK_ALIGNMENT                equ  00CF5h
GL_PACK_SWAP_BYTES                 equ  00D00h
GL_PACK_LSB_FIRST                  equ  00D01h
GL_PACK_ROW_LENGTH                 equ  00D02h
GL_PACK_SKIP_ROWS                  equ  00D03h
GL_PACK_SKIP_PIXELS                equ  00D04h
GL_PACK_ALIGNMENT                  equ  00D05h
GL_MAP_COLOR                       equ  00D10h
GL_MAP_STENCIL                     equ  00D11h
GL_INDEX_SHIFT                     equ  00D12h
GL_INDEX_OFFSET                    equ  00D13h
GL_RED_SCALE                       equ  00D14h
GL_RED_BIAS                        equ  00D15h
GL_ZOOM_X                          equ  00D16h
GL_ZOOM_Y                          equ  00D17h
GL_GREEN_SCALE                     equ  00D18h
GL_GREEN_BIAS                      equ  00D19h
GL_BLUE_SCALE                      equ  00D1Ah
GL_BLUE_BIAS                       equ  00D1Bh
GL_ALPHA_SCALE                     equ  00D1Ch
GL_ALPHA_BIAS                      equ  00D1Dh
GL_DEPTH_SCALE                     equ  00D1Eh
GL_DEPTH_BIAS                      equ  00D1Fh
GL_MAX_EVAL_ORDER                  equ  00D30h
GL_MAX_LIGHTS                      equ  00D31h
GL_MAX_CLIP_PLANES                 equ  00D32h
GL_MAX_TEXTURE_SIZE                equ  00D33h
GL_MAX_PIXEL_MAP_TABLE             equ  00D34h
GL_MAX_ATTRIB_STACK_DEPTH          equ  00D35h
GL_MAX_MODELVIEW_STACK_DEPTH       equ  00D36h
GL_MAX_NAME_STACK_DEPTH            equ  00D37h
GL_MAX_PROJECTION_STACK_DEPTH      equ  00D38h
GL_MAX_TEXTURE_STACK_DEPTH         equ  00D39h
GL_MAX_VIEWPORT_DIMS               equ  00D3Ah
GL_MAX_CLIENT_ATTRIB_STACK_DEPTH   equ  00D3Bh
GL_SUBPIXEL_BITS                   equ  00D50h
GL_INDEX_BITS                      equ  00D51h
GL_RED_BITS                        equ  00D52h
GL_GREEN_BITS                      equ  00D53h
GL_BLUE_BITS                       equ  00D54h
GL_ALPHA_BITS                      equ  00D55h
GL_DEPTH_BITS                      equ  00D56h
GL_STENCIL_BITS                    equ  00D57h
GL_ACCUM_RED_BITS                  equ  00D58h
GL_ACCUM_GREEN_BITS                equ  00D59h
GL_ACCUM_BLUE_BITS                 equ  00D5Ah
GL_ACCUM_ALPHA_BITS                equ  00D5Bh
GL_NAME_STACK_DEPTH                equ  00D70h
GL_AUTO_NORMAL                     equ  00D80h
GL_MAP1_COLOR_4                    equ  00D90h
GL_MAP1_INDEX                      equ  00D91h
GL_MAP1_NORMAL                     equ  00D92h
GL_MAP1_TEXTURE_COORD_1            equ  00D93h
GL_MAP1_TEXTURE_COORD_2            equ  00D94h
GL_MAP1_TEXTURE_COORD_3            equ  00D95h
GL_MAP1_TEXTURE_COORD_4            equ  00D96h
GL_MAP1_VERTEX_3                   equ  00D97h
GL_MAP1_VERTEX_4                   equ  00D98h
GL_MAP2_COLOR_4                    equ  00DB0h
GL_MAP2_INDEX                      equ  00DB1h
GL_MAP2_NORMAL                     equ  00DB2h
GL_MAP2_TEXTURE_COORD_1            equ  00DB3h
GL_MAP2_TEXTURE_COORD_2            equ  00DB4h
GL_MAP2_TEXTURE_COORD_3            equ  00DB5h
GL_MAP2_TEXTURE_COORD_4            equ  00DB6h
GL_MAP2_VERTEX_3                   equ  00DB7h
GL_MAP2_VERTEX_4                   equ  00DB8h
GL_MAP1_GRID_DOMAIN                equ  00DD0h
GL_MAP1_GRID_SEGMENTS              equ  00DD1h
GL_MAP2_GRID_DOMAIN                equ  00DD2h
GL_MAP2_GRID_SEGMENTS              equ  00DD3h
GL_TEXTURE_1D                      equ  00DE0h
GL_TEXTURE_2D                      equ  00DE1h
GL_FEEDBACK_BUFFER_POINTER         equ  00DF0h
GL_FEEDBACK_BUFFER_SIZE            equ  00DF1h
GL_FEEDBACK_BUFFER_TYPE            equ  00DF2h
GL_SELECTION_BUFFER_POINTER        equ  00DF3h
GL_SELECTION_BUFFER_SIZE           equ  00DF4h
GL_TEXTURE_WIDTH                   equ  01000h
GL_TEXTURE_HEIGHT                  equ  01001h
GL_TEXTURE_INTERNAL_FORMAT         equ  01003h
GL_TEXTURE_BORDER_COLOR            equ  01004h
GL_TEXTURE_BORDER                  equ  01005h
GL_DONT_CARE                       equ  01100h
GL_FASTEST                         equ  01101h
GL_NICEST                          equ  01102h
GL_LIGHT0                          equ  04000h
GL_LIGHT1                          equ  04001h
GL_LIGHT2                          equ  04002h
GL_LIGHT3                          equ  04003h
GL_LIGHT4                          equ  04004h
GL_LIGHT5                          equ  04005h
GL_LIGHT6                          equ  04006h
GL_LIGHT7                          equ  04007h
GL_AMBIENT                         equ  01200h
GL_DIFFUSE                         equ  01201h
GL_SPECULAR                        equ  01202h
GL_POSITION                        equ  01203h
GL_SPOT_DIRECTION                  equ  01204h
GL_SPOT_EXPONENT                   equ  01205h
GL_SPOT_CUTOFF                     equ  01206h
GL_CONSTANT_ATTENUATION            equ  01207h
GL_LINEAR_ATTENUATION              equ  01208h
GL_QUADRATIC_ATTENUATION           equ  01209h
GL_COMPILE                         equ  01300h
GL_COMPILE_AND_EXECUTE             equ  01301h
GL_CLEAR                           equ  01500h
GL_AND                             equ  01501h
GL_AND_REVERSE                     equ  01502h
GL_COPY                            equ  01503h
GL_AND_INVERTED                    equ  01504h
GL_NOOP                            equ  01505h
GL_XOR                             equ  01506h
GL_OR                              equ  01507h
GL_NOR                             equ  01508h
GL_EQUIV                           equ  01509h
GL_INVERT                          equ  0150Ah
GL_OR_REVERSE                      equ  0150Bh
GL_COPY_INVERTED                   equ  0150Ch
GL_OR_INVERTED                     equ  0150Dh
GL_NAND                            equ  0150Eh
GL_SET                             equ  0150Fh
GL_EMISSION                        equ  01600h
GL_SHININESS                       equ  01601h
GL_AMBIENT_AND_DIFFUSE             equ  01602h
GL_COLOR_INDEXES                   equ  01603h
GL_MODELVIEW                       equ  01700h
GL_PROJECTION                      equ  01701h
GL_TEXTURE                         equ  01702h
GL_COLOR                           equ  01800h
GL_DEPTH                           equ  01801h
GL_STENCIL                         equ  01802h
GL_COLOR_INDEX                     equ  01900h
GL_STENCIL_INDEX                   equ  01901h
GL_DEPTH_COMPONENT                 equ  01902h
GL_RED                             equ  01903h
GL_GREEN                           equ  01904h
GL_BLUE                            equ  01905h
GL_ALPHA                           equ  01906h
GL_RGB                             equ  01907h
GL_RGBA                            equ  01908h
GL_LUMINANCE                       equ  01909h
GL_LUMINANCE_ALPHA                 equ  0190Ah
GL_BITMAP                          equ  01A00h
GL_POINT                           equ  01B00h
GL_LINE                            equ  01B01h
GL_FILL                            equ  01B02h
GL_RENDER                          equ  01C00h
GL_FEEDBACK                        equ  01C01h
GL_SELECT                          equ  01C02h
GL_FLAT                            equ  01D00h
GL_SMOOTH                          equ  01D01h
GL_KEEP                            equ  01E00h
GL_REPLACE                         equ  01E01h
GL_INCR                            equ  01E02h
GL_DECR                            equ  01E03h
GL_VENDOR                          equ  01F00h
GL_RENDERER                        equ  01F01h
GL_VERSION                         equ  01F02h
GL_EXTENSIONS                      equ  01F03h
GL_S                               equ  02000h
GL_T                               equ  02001h
GL_R                               equ  02002h
GL_Q                               equ  02003h
GL_MODULATE                        equ  02100h
GL_DECAL                           equ  02101h
GL_TEXTURE_ENV_MODE                equ  02200h
GL_TEXTURE_ENV_COLOR               equ  02201h
GL_TEXTURE_ENV                     equ  02300h
GL_EYE_LINEAR                      equ  02400h
GL_OBJECT_LINEAR                   equ  02401h
GL_SPHERE_MAP                      equ  02402h
GL_TEXTURE_GEN_MODE                equ  02500h
GL_OBJECT_PLANE                    equ  02501h
GL_EYE_PLANE                       equ  02502h
GL_NEAREST                         equ  02600h
GL_LINEAR                          equ  02601h
GL_NEAREST_MIPMAP_NEAREST          equ  02700h
GL_LINEAR_MIPMAP_NEAREST           equ  02701h
GL_NEAREST_MIPMAP_LINEAR           equ  02702h
GL_LINEAR_MIPMAP_LINEAR            equ  02703h
GL_TEXTURE_MAG_FILTER              equ  02800h
GL_TEXTURE_MIN_FILTER              equ  02801h
GL_TEXTURE_WRAP_S                  equ  02802h
GL_TEXTURE_WRAP_T                  equ  02803h
GL_CLAMP                           equ  02900h
GL_REPEAT                          equ  02901h
GL_CLIENT_PIXEL_STORE_BIT          equ  000000001h
GL_CLIENT_VERTEX_ARRAY_BIT         equ  000000002h
GL_CLIENT_ALL_ATTRIB_BITS          equ  0ffffffffh
GL_POLYGON_OFFSET_FACTOR           equ  08038h
GL_POLYGON_OFFSET_UNITS            equ  02A00h
GL_POLYGON_OFFSET_POINT            equ  02A01h
GL_POLYGON_OFFSET_LINE             equ  02A02h
GL_POLYGON_OFFSET_FILL             equ  08037h
GL_ALPHA4                          equ  0803Bh
GL_ALPHA8                          equ  0803Ch
GL_ALPHA12                         equ  0803Dh
GL_ALPHA16                         equ  0803Eh
GL_LUMINANCE4                      equ  0803Fh
GL_LUMINANCE8                      equ  08040h
GL_LUMINANCE12                     equ  08041h
GL_LUMINANCE16                     equ  08042h
GL_LUMINANCE4_ALPHA4               equ  08043h
GL_LUMINANCE6_ALPHA2               equ  08044h
GL_LUMINANCE8_ALPHA8               equ  08045h
GL_LUMINANCE12_ALPHA4              equ  08046h
GL_LUMINANCE12_ALPHA12             equ  08047h
GL_LUMINANCE16_ALPHA16             equ  08048h
GL_INTENSITY                       equ  08049h
GL_INTENSITY4                      equ  0804Ah
GL_INTENSITY8                      equ  0804Bh
GL_INTENSITY12                     equ  0804Ch
GL_INTENSITY16                     equ  0804Dh
GL_R3_G3_B2                        equ  02A10h
GL_RGB4                            equ  0804Fh
GL_RGB5                            equ  08050h
GL_RGB8                            equ  08051h
GL_RGB10                           equ  08052h
GL_RGB12                           equ  08053h
GL_RGB16                           equ  08054h
GL_RGBA2                           equ  08055h
GL_RGBA4                           equ  08056h
GL_RGB5_A1                         equ  08057h
GL_RGBA8                           equ  08058h
GL_RGB10_A2                        equ  08059h
GL_RGBA12                          equ  0805Ah
GL_RGBA16                          equ  0805Bh
GL_TEXTURE_RED_SIZE                equ  0805Ch
GL_TEXTURE_GREEN_SIZE              equ  0805Dh
GL_TEXTURE_BLUE_SIZE               equ  0805Eh
GL_TEXTURE_ALPHA_SIZE              equ  0805Fh
GL_TEXTURE_LUMINANCE_SIZE          equ  08060h
GL_TEXTURE_INTENSITY_SIZE          equ  08061h
GL_PROXY_TEXTURE_1D                equ  08063h
GL_PROXY_TEXTURE_2D                equ  08064h
GL_TEXTURE_PRIORITY                equ  08066h
GL_TEXTURE_RESIDENT                equ  08067h
GL_TEXTURE_BINDING_1D              equ  08068h
GL_TEXTURE_BINDING_2D              equ  08069h
GL_VERTEX_ARRAY                    equ  08074h
GL_NORMAL_ARRAY                    equ  08075h
GL_COLOR_ARRAY                     equ  08076h
GL_INDEX_ARRAY                     equ  08077h
GL_TEXTURE_COORD_ARRAY             equ  08078h
GL_EDGE_FLAG_ARRAY                 equ  08079h
GL_VERTEX_ARRAY_SIZE               equ  0807Ah
GL_VERTEX_ARRAY_TYPE               equ  0807Bh
GL_VERTEX_ARRAY_STRIDE             equ  0807Ch
GL_NORMAL_ARRAY_TYPE               equ  0807Eh
GL_NORMAL_ARRAY_STRIDE             equ  0807Fh
GL_COLOR_ARRAY_SIZE                equ  08081h
GL_COLOR_ARRAY_TYPE                equ  08082h
GL_COLOR_ARRAY_STRIDE              equ  08083h
GL_INDEX_ARRAY_TYPE                equ  08085h
GL_INDEX_ARRAY_STRIDE              equ  08086h
GL_TEXTURE_COORD_ARRAY_SIZE        equ  08088h
GL_TEXTURE_COORD_ARRAY_TYPE        equ  08089h
GL_TEXTURE_COORD_ARRAY_STRIDE      equ  0808Ah
GL_EDGE_FLAG_ARRAY_STRIDE          equ  0808Ch
GL_VERTEX_ARRAY_POINTER            equ  0808Eh
GL_NORMAL_ARRAY_POINTER            equ  0808Fh
GL_COLOR_ARRAY_POINTER             equ  08090h
GL_INDEX_ARRAY_POINTER             equ  08091h
GL_TEXTURE_COORD_ARRAY_POINTER     equ  08092h
GL_EDGE_FLAG_ARRAY_POINTER         equ  08093h
GL_V2F                             equ  02A20h
GL_V3F                             equ  02A21h
GL_C4UB_V2F                        equ  02A22h
GL_C4UB_V3F                        equ  02A23h
GL_C3F_V3F                         equ  02A24h
GL_N3F_V3F                         equ  02A25h
GL_C4F_N3F_V3F                     equ  02A26h
GL_T2F_V3F                         equ  02A27h
GL_T4F_V4F                         equ  02A28h
GL_T2F_C4UB_V3F                    equ  02A29h
GL_T2F_C3F_V3F                     equ  02A2Ah
GL_T2F_N3F_V3F                     equ  02A2Bh
GL_T2F_C4F_N3F_V3F                 equ  02A2Ch
GL_T4F_C4F_N3F_V4F                 equ  02A2Dh
GL_EXT_vertex_array                equ  1
GL_EXT_bgra                        equ  1
GL_EXT_paletted_texture            equ  1
GL_WIN_swap_hint                   equ  1
GL_WIN_draw_range_elements         equ  1
GL_VERTEX_ARRAY_EXT                equ  08074h
GL_NORMAL_ARRAY_EXT                equ  08075h
GL_COLOR_ARRAY_EXT                 equ  08076h
GL_INDEX_ARRAY_EXT                 equ  08077h
GL_TEXTURE_COORD_ARRAY_EXT         equ  08078h
GL_EDGE_FLAG_ARRAY_EXT             equ  08079h
GL_VERTEX_ARRAY_SIZE_EXT           equ  0807Ah
GL_VERTEX_ARRAY_TYPE_EXT           equ  0807Bh
GL_VERTEX_ARRAY_STRIDE_EXT         equ  0807Ch
GL_VERTEX_ARRAY_COUNT_EXT          equ  0807Dh
GL_NORMAL_ARRAY_TYPE_EXT           equ  0807Eh
GL_NORMAL_ARRAY_STRIDE_EXT         equ  0807Fh
GL_NORMAL_ARRAY_COUNT_EXT          equ  08080h
GL_COLOR_ARRAY_SIZE_EXT            equ  08081h
GL_COLOR_ARRAY_TYPE_EXT            equ  08082h
GL_COLOR_ARRAY_STRIDE_EXT          equ  08083h
GL_COLOR_ARRAY_COUNT_EXT           equ  08084h
GL_INDEX_ARRAY_TYPE_EXT            equ  08085h
GL_INDEX_ARRAY_STRIDE_EXT          equ  08086h
GL_INDEX_ARRAY_COUNT_EXT           equ  08087h
GL_TEXTURE_COORD_ARRAY_SIZE_EXT    equ  08088h
GL_TEXTURE_COORD_ARRAY_TYPE_EXT    equ  08089h
GL_TEXTURE_COORD_ARRAY_STRIDE_EXT  equ  0808Ah
GL_TEXTURE_COORD_ARRAY_COUNT_EXT   equ  0808Bh
GL_EDGE_FLAG_ARRAY_STRIDE_EXT      equ  0808Ch
GL_EDGE_FLAG_ARRAY_COUNT_EXT       equ  0808Dh
GL_VERTEX_ARRAY_POINTER_EXT        equ  0808Eh
GL_NORMAL_ARRAY_POINTER_EXT        equ  0808Fh
GL_COLOR_ARRAY_POINTER_EXT         equ  08090h
GL_INDEX_ARRAY_POINTER_EXT         equ  08091h
GL_TEXTURE_COORD_ARRAY_POINTER_EXT equ  08092h
GL_EDGE_FLAG_ARRAY_POINTER_EXT     equ  08093h
GL_DOUBLE_EXT                      equ  GL_DOUBLE
GL_BGR_EXT                         equ  080E0h
GL_BGRA_EXT                        equ  080E1h
GL_COLOR_TABLE_FORMAT_EXT          equ  080D8h
GL_COLOR_TABLE_WIDTH_EXT           equ  080D9h
GL_COLOR_TABLE_RED_SIZE_EXT        equ  080DAh
GL_COLOR_TABLE_GREEN_SIZE_EXT      equ  080DBh
GL_COLOR_TABLE_BLUE_SIZE_EXT       equ  080DCh
GL_COLOR_TABLE_ALPHA_SIZE_EXT      equ  080DDh
GL_COLOR_TABLE_LUMINANCE_SIZE_EXT  equ  080DEh
GL_COLOR_TABLE_INTENSITY_SIZE_EXT  equ  080DFh
GL_COLOR_INDEX1_EXT                equ  080E2h
GL_COLOR_INDEX2_EXT                equ  080E3h
GL_COLOR_INDEX4_EXT                equ  080E4h
GL_COLOR_INDEX8_EXT                equ  080E5h
GL_COLOR_INDEX12_EXT               equ  080E6h
GL_COLOR_INDEX16_EXT               equ  080E7h
GL_MAX_ELEMENTS_VERTICES_WIN       equ  080E8h
GL_MAX_ELEMENTS_INDICES_WIN        equ  080E9h
GL_PHONG_WIN                       equ  080EAh
GL_PHONG_HINT_WIN                  equ  080EBh
GL_FOG_SPECULAR_TEXTURE_WIN        equ  080ECh
GL_LOGIC_OP                        equ  GL_INDEX_LOGIC_OP
GL_TEXTURE_COMPONENTS              equ  GL_TEXTURE_INTERNAL_FORMAT

GLU_VERSION_1_1                 equ  1
GLU_VERSION_1_2                 equ  1
GLU_INVALID_ENUM                equ  100900
GLU_INVALID_VALUE               equ  100901
GLU_OUT_OF_MEMORY               equ  100902
GLU_INCOMPATIBLE_GL_VERSION     equ  100903
GLU_VERSION                     equ  100800
GLU_EXTENSIONS                  equ  100801
GLU_TRUE                        equ  GL_TRUE
GLU_FALSE                       equ  GL_FALSE
GLU_SMOOTH                      equ  100000
GLU_FLAT                        equ  100001
GLU_NONE                        equ  100002
GLU_POINT                       equ  100010
GLU_LINE                        equ  100011
GLU_FILL                        equ  100012
GLU_SILHOUETTE                  equ  100013
GLU_OUTSIDE                     equ  100020
GLU_INSIDE                      equ  100021
GLU_TESS_MAX_COORD              equ  1.0e150
GLU_TESS_WINDING_RULE           equ  100140
GLU_TESS_BOUNDARY_ONLY          equ  100141
GLU_TESS_TOLERANCE              equ  100142
GLU_TESS_WINDING_ODD            equ  100130
GLU_TESS_WINDING_NONZERO        equ  100131
GLU_TESS_WINDING_POSITIVE       equ  100132
GLU_TESS_WINDING_NEGATIVE       equ  100133
GLU_TESS_WINDING_ABS_GEQ_TWO    equ  100134
GLU_TESS_BEGIN                  equ  100100
GLU_TESS_VERTEX                 equ  100101
GLU_TESS_END                    equ  100102
GLU_TESS_ERROR                  equ  100103
GLU_TESS_EDGE_FLAG              equ  100104
GLU_TESS_COMBINE                equ  100105
GLU_TESS_BEGIN_DATA             equ  100106
GLU_TESS_VERTEX_DATA            equ  100107
GLU_TESS_END_DATA               equ  100108
GLU_TESS_ERROR_DATA             equ  100109
GLU_TESS_EDGE_FLAG_DATA         equ  100110
GLU_TESS_COMBINE_DATA           equ  100111
GLU_TESS_ERROR1                 equ  100151
GLU_TESS_ERROR2                 equ  100152
GLU_TESS_ERROR3                 equ  100153
GLU_TESS_ERROR4                 equ  100154
GLU_TESS_ERROR5                 equ  100155
GLU_TESS_ERROR6                 equ  100156
GLU_TESS_ERROR7                 equ  100157
GLU_TESS_ERROR8                 equ  100158
GLU_TESS_MISSING_BEGIN_POLYGON  equ  GLU_TESS_ERROR1
GLU_TESS_MISSING_BEGIN_CONTOUR  equ  GLU_TESS_ERROR2
GLU_TESS_MISSING_END_POLYGON    equ  GLU_TESS_ERROR3
GLU_TESS_MISSING_END_CONTOUR    equ  GLU_TESS_ERROR4
GLU_TESS_COORD_TOO_LARGE        equ  GLU_TESS_ERROR5
GLU_TESS_NEED_COMBINE_CALLBACK  equ  GLU_TESS_ERROR6
GLU_AUTO_LOAD_MATRIX            equ  100200
GLU_CULLING                     equ  100201
GLU_SAMPLING_TOLERANCE          equ  100203
GLU_DISPLAY_MODE                equ  100204
GLU_PARAMETRIC_TOLERANCE        equ  100202
GLU_SAMPLING_METHOD             equ  100205
GLU_U_STEP                      equ  100206
GLU_V_STEP                      equ  100207
GLU_PATH_LENGTH                 equ  100215
GLU_PARAMETRIC_ERROR            equ  100216
GLU_DOMAIN_DISTANCE             equ  100217
GLU_MAP1_TRIM_2                 equ  100210
GLU_MAP1_TRIM_3                 equ  100211
GLU_OUTLINE_POLYGON             equ  100240
GLU_OUTLINE_PATCH               equ  100241
GLU_NURBS_ERROR1                equ  100251
GLU_NURBS_ERROR2                equ  100252
GLU_NURBS_ERROR3                equ  100253
GLU_NURBS_ERROR4                equ  100254
GLU_NURBS_ERROR5                equ  100255
GLU_NURBS_ERROR6                equ  100256
GLU_NURBS_ERROR7                equ  100257
GLU_NURBS_ERROR8                equ  100258
GLU_NURBS_ERROR9                equ  100259
GLU_NURBS_ERROR10               equ  100260
GLU_NURBS_ERROR11               equ  100261
GLU_NURBS_ERROR12               equ  100262
GLU_NURBS_ERROR13               equ  100263
GLU_NURBS_ERROR14               equ  100264
GLU_NURBS_ERROR15               equ  100265
GLU_NURBS_ERROR16               equ  100266
GLU_NURBS_ERROR17               equ  100267
GLU_NURBS_ERROR18               equ  100268
GLU_NURBS_ERROR19               equ  100269
GLU_NURBS_ERROR20               equ  100270
GLU_NURBS_ERROR21               equ  100271
GLU_NURBS_ERROR22               equ  100272
GLU_NURBS_ERROR23               equ  100273
GLU_NURBS_ERROR24               equ  100274
GLU_NURBS_ERROR25               equ  100275
GLU_NURBS_ERROR26               equ  100276
GLU_NURBS_ERROR27               equ  100277
GLU_NURBS_ERROR28               equ  100278
GLU_NURBS_ERROR29               equ  100279
GLU_NURBS_ERROR30               equ  100280
GLU_NURBS_ERROR31               equ  100281
GLU_NURBS_ERROR32               equ  100282
GLU_NURBS_ERROR33               equ  100283
GLU_NURBS_ERROR34               equ  100284
GLU_NURBS_ERROR35               equ  100285
GLU_NURBS_ERROR36               equ  100286
GLU_NURBS_ERROR37               equ  100287
GLU_CW                          equ  100120
GLU_CCW                         equ  100121
GLU_INTERIOR                    equ  100122
GLU_EXTERIOR                    equ  100123
GLU_UNKNOWN                     equ  100124
GLU_BEGIN                       equ  GLU_TESS_BEGIN
GLU_VERTEX                      equ  GLU_TESS_VERTEX
GLU_END                         equ  GLU_TESS_END
GLU_ERROR                       equ  GLU_TESS_ERROR
GLU_EDGE_FLAG                   equ  GLU_TESS_EDGE_FLAG

;;;; end opengl


;;;; urlmon

__REQUIRED_RPCNDR_H_VERSION__    equ 440
MKSYS_URLMONIKER                 equ 6
URL_MK_LEGACY                    equ 0
URL_MK_UNIFORM                   equ 1
URL_MK_NO_CANONICALIZE           equ 2
FIEF_FLAG_FORCE_JITUI            equ 1h
FIEF_FLAG_PEEK                   equ 2h
FIEF_FLAG_SKIP_INSTALLED_VERSION_CHECK equ 4h
FMFD_DEFAULT                     equ 00000000h
FMFD_URLASFILENAME               equ 00000001h
URLMON_OPTION_USERAGENT          equ 10000001h
URLMON_OPTION_USERAGENT_REFRESH  equ 10000002h
URLMON_OPTION_URL_ENCODING       equ 10000004h
CF_NULL                          equ 0
WININETINFO_OPTION_LOCK_HANDLE   equ 65534
URLOSTRM_USECACHEDCOPY_ONLY      equ 1h
URLOSTRM_USECACHEDCOPY           equ 2h
URLOSTRM_GETNEWESTVERSION        equ 3h
PROTOCOLFLAG_NO_PICS_CHECK       equ 00000001h
MUTZ_NOSAVEDFILECHECK            equ 00000001h
MUTZ_ENFORCERESTRICTED           equ 00000100h
MAX_SIZE_SECURITY_ID             equ 512
URLACTION_MIN                    equ 00001000h
URLACTION_DOWNLOAD_MIN           equ 00001000h
URLACTION_DOWNLOAD_SIGNED_ACTIVEX equ 00001001h
URLACTION_DOWNLOAD_UNSIGNED_ACTIVEX equ 00001004h
URLACTION_DOWNLOAD_CURR_MAX      equ 00001004h
URLACTION_DOWNLOAD_MAX           equ 000011FFh
URLACTION_ACTIVEX_MIN            equ 00001200h
URLACTION_ACTIVEX_RUN            equ 00001200h
URLPOLICY_ACTIVEX_CHECK_LIST     equ 00010000h
URLACTION_ACTIVEX_OVERRIDE_OBJECT_SAFETY equ 00001201h
URLACTION_ACTIVEX_OVERRIDE_DATA_SAFETY equ 00001202h
URLACTION_ACTIVEX_OVERRIDE_SCRIPT_SAFETY equ 00001203h
URLACTION_SCRIPT_OVERRIDE_SAFETY equ 00001401h
URLACTION_ACTIVEX_CONFIRM_NOOBJECTSAFETY equ 00001204h
URLACTION_ACTIVEX_TREATASUNTRUSTED equ 00001205h
URLACTION_ACTIVEX_NO_WEBOC_SCRIPT equ 00001206h
URLACTION_ACTIVEX_CURR_MAX       equ 00001206h
URLACTION_ACTIVEX_MAX            equ 000013ffh
URLACTION_SCRIPT_MIN             equ 00001400h
URLACTION_SCRIPT_RUN             equ 00001400h
URLACTION_SCRIPT_JAVA_USE        equ 00001402h
URLACTION_SCRIPT_SAFE_ACTIVEX    equ 00001405h
URLACTION_CROSS_DOMAIN_DATA      equ 00001406h
URLACTION_SCRIPT_PASTE           equ 00001407h
URLACTION_SCRIPT_CURR_MAX        equ 00001407h
URLACTION_SCRIPT_MAX             equ 000015ffh
URLACTION_HTML_MIN               equ 00001600h
URLACTION_HTML_SUBMIT_FORMS      equ 00001601h
URLACTION_HTML_SUBMIT_FORMS_FROM equ 00001602h
URLACTION_HTML_SUBMIT_FORMS_TO   equ 00001603h
URLACTION_HTML_FONT_DOWNLOAD     equ 00001604h
URLACTION_HTML_JAVA_RUN          equ 00001605h
URLACTION_HTML_USERDATA_SAVE     equ 00001606h
URLACTION_HTML_SUBFRAME_NAVIGATE equ 00001607h
URLACTION_HTML_META_REFRESH      equ 00001608h
URLACTION_HTML_MIXED_CONTENT     equ 00001609h
URLACTION_HTML_MAX               equ 000017ffh
URLACTION_SHELL_MIN              equ 00001800h
URLACTION_SHELL_INSTALL_DTITEMS  equ 00001800h
URLACTION_SHELL_MOVE_OR_COPY     equ 00001802h
URLACTION_SHELL_FILE_DOWNLOAD    equ 00001803h
URLACTION_SHELL_VERB             equ 00001804h
URLACTION_SHELL_WEBVIEW_VERB     equ 00001805h
URLACTION_SHELL_SHELLEXECUTE     equ 00001806h
URLACTION_SHELL_CURR_MAX         equ 00001806h
URLACTION_SHELL_MAX              equ 000019ffh
URLACTION_NETWORK_MIN            equ 00001A00h
URLACTION_CREDENTIALS_USE        equ 00001A00h
URLPOLICY_CREDENTIALS_SILENT_LOGON_OK equ 00000000h
URLPOLICY_CREDENTIALS_MUST_PROMPT_USER equ 00010000h
URLPOLICY_CREDENTIALS_CONDITIONAL_PROMPT equ 00020000h
URLPOLICY_CREDENTIALS_ANONYMOUS_ONLY equ 00030000h
URLACTION_AUTHENTICATE_CLIENT    equ 00001A01h
URLPOLICY_AUTHENTICATE_CLEARTEXT_OK equ 00000000h
URLPOLICY_AUTHENTICATE_CHALLENGE_RESPONSE equ 00010000h
URLPOLICY_AUTHENTICATE_MUTUAL_ONLY equ 00030000h
URLACTION_COOKIES                equ 00001A02h
URLACTION_COOKIES_SESSION        equ 00001A03h
URLACTION_CLIENT_CERT_PROMPT     equ 00001A04h
URLACTION_COOKIES_THIRD_PARTY    equ 00001A05h
URLACTION_COOKIES_SESSION_THIRD_PARTY equ 00001A06h
URLACTION_COOKIES_ENABLED        equ 00001A10h
URLACTION_NETWORK_CURR_MAX       equ 00001A10h
URLACTION_NETWORK_MAX            equ 00001Bffh
URLACTION_JAVA_MIN               equ 00001C00h
URLACTION_JAVA_PERMISSIONS       equ 00001C00h
URLPOLICY_JAVA_PROHIBIT          equ 00000000h
URLPOLICY_JAVA_HIGH              equ 00010000h
URLPOLICY_JAVA_MEDIUM            equ 00020000h
URLPOLICY_JAVA_LOW               equ 00030000h
URLPOLICY_JAVA_CUSTOM            equ 00800000h
URLACTION_JAVA_CURR_MAX          equ 00001C00h
URLACTION_JAVA_MAX               equ 00001Cffh
URLACTION_INFODELIVERY_MIN       equ 00001D00h
URLACTION_INFODELIVERY_NO_ADDING_CHANNELS equ 00001D00h
URLACTION_INFODELIVERY_NO_EDITING_CHANNELS equ 00001D01h
URLACTION_INFODELIVERY_NO_REMOVING_CHANNELS equ 00001D02h
URLACTION_INFODELIVERY_NO_ADDING_SUBSCRIPTIONS equ 00001D03h
URLACTION_INFODELIVERY_NO_EDITING_SUBSCRIPTIONS equ 00001D04h
URLACTION_INFODELIVERY_NO_REMOVING_SUBSCRIPTIONS equ 00001D05h
URLACTION_INFODELIVERY_NO_CHANNEL_LOGGING equ 00001D06h
URLACTION_INFODELIVERY_CURR_MAX  equ 00001D06h
URLACTION_INFODELIVERY_MAX       equ 00001Dffh
URLACTION_CHANNEL_SOFTDIST_MIN   equ 00001E00h
URLACTION_CHANNEL_SOFTDIST_PERMISSIONS equ 00001E05h
URLPOLICY_CHANNEL_SOFTDIST_PROHIBIT equ 00010000h
URLPOLICY_CHANNEL_SOFTDIST_PRECACHE equ 00020000h
URLPOLICY_CHANNEL_SOFTDIST_AUTOINSTALL equ 00030000h
URLACTION_CHANNEL_SOFTDIST_MAX   equ 00001Effh
URLPOLICY_ALLOW                  equ 00h
URLPOLICY_QUERY                  equ 01h
URLPOLICY_DISALLOW               equ 03h
URLPOLICY_NOTIFY_ON_ALLOW        equ 10h
URLPOLICY_NOTIFY_ON_DISALLOW     equ 20h
URLPOLICY_LOG_ON_ALLOW           equ 40h
URLPOLICY_LOG_ON_DISALLOW        equ 80h
URLPOLICY_MASK_PERMISSIONS       equ 0fh
URLPOLICY_DONTCHECKDLGBOX        equ 100h
URLZONE_ESC_FLAG                 equ 100h
SOFTDIST_FLAG_USAGE_EMAIL        equ 00000001h
SOFTDIST_FLAG_USAGE_PRECACHE     equ 00000002h
SOFTDIST_FLAG_USAGE_AUTOINSTALL  equ 00000004h
SOFTDIST_FLAG_DELETE_SUBSCRIPTION equ 00000008h
SOFTDIST_ADSTATE_NONE            equ 00000000h
SOFTDIST_ADSTATE_AVAILABLE       equ 00000001h
SOFTDIST_ADSTATE_DOWNLOADED      equ 00000002h
SOFTDIST_ADSTATE_INSTALLED       equ 00000003h
CONFIRMSAFETYACTION_LOADOBJECT   equ 00000001h
FMFD_ENABLEMIMESNIFFING          equ 00000002h
FMFD_IGNOREMIMETEXTPLAIN         equ 00000004h
URLMON_OPTION_USE_BINDSTRINGCREDS equ 10000008h
URLMON_OPTION_USE_BROWSERAPPSDOCUMENTS equ 10000010h
MK_S_ASYNCHRONOUS                equ 000401E8h
S_ASYNCHRONOUS                   equ MK_S_ASYNCHRONOUS
INET_E_INVALID_URL               equ 800C0002h
INET_E_NO_SESSION                equ 800C0003h
INET_E_CANNOT_CONNECT            equ 800C0004h
INET_E_RESOURCE_NOT_FOUND        equ 800C0005h
INET_E_OBJECT_NOT_FOUND          equ 800C0006h
INET_E_DATA_NOT_AVAILABLE        equ 800C0007h
INET_E_DOWNLOAD_FAILURE          equ 800C0008h
INET_E_AUTHENTICATION_REQUIRED   equ 800C0009h
INET_E_NO_VALID_MEDIA            equ 800C000Ah
INET_E_CONNECTION_TIMEOUT        equ 800C000Bh
INET_E_INVALID_REQUEST           equ 800C000Ch
INET_E_UNKNOWN_PROTOCOL          equ 800C000Dh
INET_E_SECURITY_PROBLEM          equ 800C000Eh
INET_E_CANNOT_LOAD_DATA          equ 800C000Fh
INET_E_INVALID_CERTIFICATE       equ 800C0019h
INET_E_REDIRECT_FAILED           equ 800C0014h
INET_E_REDIRECT_TO_DIR           equ 800C0015h
INET_E_CANNOT_LOCK_REQUEST       equ 800C0016h
INET_E_USE_EXTEND_BINDING        equ 800C0017h
INET_E_TERMINATED_BIND           equ 800C0018h
INET_E_ERROR_FIRST               equ 800C0002h
INET_E_CODE_DOWNLOAD_DECLINED    equ 800C0100h
INET_E_RESULT_DISPATCHED         equ 800C0200h
INET_E_CANNOT_REPLACE_SFP_FILE   equ 800C0300h
INET_E_CODE_INSTALL_SUPPRESSED   equ 800C0400h
INET_E_CODE_INSTALL_BLOCKED_BY_HASH_POLICY equ 800C0500h
INET_E_ERROR_LAST                equ INET_E_CODE_INSTALL_BLOCKED_BY_HASH_POLICY
Uri_CREATE_ALLOW_RELATIVE        equ 00000001h
Uri_CREATE_ALLOW_IMPLICIT_WILDCARD_SCHEME equ 00000002h
Uri_CREATE_ALLOW_IMPLICIT_FILE_SCHEME equ 00000004h
Uri_CREATE_NOFRAG                equ 00000008h
Uri_CREATE_NO_CANONICALIZE       equ 00000010h
Uri_CREATE_CANONICALIZE          equ 00000100h
Uri_CREATE_FILE_USE_DOS_PATH     equ 00000020h
Uri_CREATE_DECODE_EXTRA_INFO     equ 00000040h
Uri_CREATE_NO_DECODE_EXTRA_INFO  equ 00000080h
Uri_CREATE_CRACK_UNKNOWN_SCHEMES equ 00000200h
Uri_CREATE_NO_CRACK_UNKNOWN_SCHEMES equ 00000400h
Uri_CREATE_PRE_PROCESS_HTML_URI  equ 00000800h
Uri_CREATE_NO_PRE_PROCESS_HTML_URI equ 00001000h
Uri_CREATE_IE_SETTINGS           equ 00002000h
Uri_CREATE_NO_IE_SETTINGS        equ 00004000h
Uri_CREATE_NO_ENCODE_FORBIDDEN_CHARACTERS equ 00008000h
Uri_DISPLAY_NO_FRAGMENT          equ 00000001h
Uri_PUNYCODE_IDN_HOST            equ 00000002h
Uri_DISPLAY_IDN_HOST             equ 00000004h
Uri_ENCODING_USER_INFO_AND_PATH_IS_PERCENT_ENCODED_UTF8 equ 00000001h
Uri_ENCODING_USER_INFO_AND_PATH_IS_CP equ 00000002h
Uri_ENCODING_HOST_IS_IDN         equ 00000004h
Uri_ENCODING_HOST_IS_PERCENT_ENCODED_UTF8 equ 00000008h
Uri_ENCODING_HOST_IS_PERCENT_ENCODED_CP equ 00000010h
Uri_ENCODING_QUERY_AND_FRAGMENT_IS_PERCENT_ENCODED_UTF8 equ 00000020h
Uri_ENCODING_QUERY_AND_FRAGMENT_IS_CP equ 00000040h
Uri_ENCODING_RFC                 equ Uri_ENCODING_USER_INFO_AND_PATH_IS_PERCENT_ENCODED_UTF8 or Uri_ENCODING_HOST_IS_PERCENT_ENCODED_UTF8 or Uri_ENCODING_QUERY_AND_FRAGMENT_IS_PERCENT_ENCODED_UTF8
UriBuilder_USE_ORIGINAL_FLAGS    equ 00000001h
SET_FEATURE_ON_THREAD            equ 00000001h
SET_FEATURE_ON_PROCESS           equ 00000002h
SET_FEATURE_IN_REGISTRY          equ 00000004h
SET_FEATURE_ON_THREAD_LOCALMACHINE equ 00000008h
SET_FEATURE_ON_THREAD_INTRANET   equ 00000010h
SET_FEATURE_ON_THREAD_TRUSTED    equ 00000020h
SET_FEATURE_ON_THREAD_INTERNET   equ 00000040h
SET_FEATURE_ON_THREAD_RESTRICTED equ 00000080h
GET_FEATURE_FROM_THREAD          equ 00000001h
GET_FEATURE_FROM_PROCESS         equ 00000002h
GET_FEATURE_FROM_REGISTRY        equ 00000004h
GET_FEATURE_FROM_THREAD_LOCALMACHINE equ 00000008h
GET_FEATURE_FROM_THREAD_INTRANET equ 00000010h
GET_FEATURE_FROM_THREAD_TRUSTED  equ 00000020h
GET_FEATURE_FROM_THREAD_INTERNET equ 00000040h
GET_FEATURE_FROM_THREAD_RESTRICTED equ 00000080h
INET_E_USE_DEFAULT_SETTING       equ 800C0012h
INET_E_DEFAULT_ACTION            equ INET_E_USE_DEFAULT_PROTOCOLHANDLER
INET_E_QUERYOPTION_UNKNOWN       equ 800C0013h
INET_E_REDIRECTING               equ 800C0014h
MUTZ_ISFILE                      equ 00000002h
MUTZ_ACCEPT_WILDCARD_SCHEME      equ 00000080h
MUTZ_RESERVED                    equ 00000200h
MUTZ_REQUIRESAVEDFILECHECK       equ 00000400h
MUTZ_DONT_UNESCAPE               equ 00000800h
MUTZ_DONT_USE_CACHE              equ 00001000h
MUTZ_FORCE_INTRANET_FLAGS        equ 00002000h
MUTZ_IGNORE_ZONE_MAPPINGS        equ 00004000h
URLACTION_ACTIVEX_OVERRIDE_REPURPOSEDETECTION equ 00001207h
URLACTION_ACTIVEX_OVERRIDE_OPTIN equ 00001208h
URLACTION_ACTIVEX_SCRIPTLET_RUN  equ 00001209h
URLACTION_ACTIVEX_DYNSRC_VIDEO_AND_ANIMATION equ 0000120Ah
URLACTION_ALLOW_XDOMAIN_SUBFRAME_RESIZE equ 00001408h
URLACTION_HTML_INCLUDE_FILE_PATH equ 0000160Ah
URLACTION_SHELL_EXECUTE_HIGHRISK equ 00001806h
URLACTION_SHELL_EXECUTE_MODRISK  equ 00001807h
URLACTION_SHELL_EXECUTE_LOWRISK  equ 00001808h
URLACTION_SHELL_POPUPMGR         equ 00001809h
URLACTION_SHELL_RTF_OBJECTS_LOAD equ 0000180Ah
URLACTION_SHELL_ENHANCED_DRAGDROP_SECURITY equ 0000180Bh
URLACTION_SHELL_EXTENSIONSECURITY equ 0000180Ch
URLACTION_SHELL_SECURE_DRAGSOURCE equ 0000180Dh
URLACTION_BEHAVIOR_MIN           equ 00002000h
URLACTION_BEHAVIOR_RUN           equ 00002000h
URLPOLICY_BEHAVIOR_CHECK_LIST    equ 00010000h
URLACTION_FEATURE_MIN            equ 00002100h
URLACTION_FEATURE_MIME_SNIFFING  equ 00002100h
URLACTION_FEATURE_ZONE_ELEVATION equ 00002101h
URLACTION_FEATURE_WINDOW_RESTRICTIONS equ 00002102h
URLACTION_FEATURE_SCRIPT_STATUS_BAR equ 00002103h
URLACTION_FEATURE_FORCE_ADDR_AND_STATUS equ 00002104h
URLACTION_FEATURE_BLOCK_INPUT_PROMPTS equ 00002105h
URLACTION_AUTOMATIC_DOWNLOAD_UI_MIN equ 00002200h
URLACTION_AUTOMATIC_DOWNLOAD_UI  equ 00002200h
URLACTION_AUTOMATIC_ACTIVEX_UI   equ 00002201h
URLACTION_ALLOW_RESTRICTEDPROTOCOLS equ 00002300h
URLACTION_ALLOW_APEVALUATION     equ 00002301h
URLACTION_WINDOWS_BROWSER_APPLICATIONS equ 00002400h
URLACTION_XPS_DOCUMENTS          equ 00002401h
URLACTION_LOOSE_XAML             equ 00002402h
URLACTION_LOWRIGHTS              equ 00002500h
URLACTION_WINFX_SETUP            equ 00002600h
GetUrlPolicyPermissions          equ dw          dw & URLPOLICY_MASK_PERMISSIONS
SECURITY_IE_STATE_GREEN          equ 00000000h
SECURITY_IE_STATE_RED            equ 00000001h

;;;; end urlmon

;;;; mapi

;---------------------------------------[ mapi.h ]---------------------------------------

MAPI_OLE equ 1
MAPI_OLE_STATIC equ 2
MAPI_ORIG equ 0
MAPI_TO equ 1
MAPI_CC equ 2
MAPI_BCC equ 3
MAPI_UNREAD equ 1
MAPI_RECEIPT_REQUESTED equ 2
MAPI_SENT equ 4
MAPI_LOGON_UI equ 1
MAPI_PASSWORD_UI equ 20000h
MAPI_NEW_SESSION equ 2
MAPI_FORCE_DOWNLOAD equ 1000h
MAPI_EXTENDED equ 20h
MAPI_DIALOG equ 8
MAPI_UNREAD_ONLY equ 20h
MAPI_GUARANTEE_FIFO equ 100h
MAPI_LONG_MSGID equ 4000h
MAPI_PEEK equ 80h
MAPI_SUPPRESS_ATTACH equ 800h
MAPI_ENVELOPE_ONLY equ 40h
MAPI_BODY_AS_FILE equ 200h
MAPI_AB_NOMODIFY equ 400h
SUCCESS_SUCCESS equ 0
MAPI_USER_ABORT equ 1
MAPI_E_USER_ABORT equ MAPI_USER_ABORT
MAPI_E_FAILURE equ 2
MAPI_E_LOGON_FAILURE equ 3
MAPI_E_LOGIN_FAILURE equ MAPI_E_LOGON_FAILURE
MAPI_E_DISK_FULL equ 4
MAPI_E_INSUFFICIENT_MEMORY equ 5
MAPI_E_ACCESS_DENIED equ 6
MAPI_E_TOO_MANY_SESSIONS equ 8
MAPI_E_TOO_MANY_FILES equ 9
MAPI_E_TOO_MANY_RECIPIENTS equ 10
MAPI_E_ATTACHMENT_NOT_FOUND equ 11
MAPI_E_ATTACHMENT_OPEN_FAILURE equ 12
MAPI_E_ATTACHMENT_WRITE_FAILURE equ 13
MAPI_E_UNKNOWN_RECIPIENT equ 14
MAPI_E_BAD_RECIPTYPE equ 15
MAPI_E_NO_MESSAGES equ 16
MAPI_E_INVALID_MESSAGE equ 17
MAPI_E_TEXT_TOO_LARGE equ 18
MAPI_E_INVALID_SESSION equ 19
MAPI_E_TYPE_NOT_SUPPORTED equ 20
MAPI_E_AMBIGUOUS_RECIPIENT equ 21
MAPI_E_AMBIG_RECIP equ MAPI_E_AMBIGUOUS_RECIPIENT
MAPI_E_MESSAGE_IN_USE equ 22
MAPI_E_NETWORK_FAILURE equ 23
MAPI_E_INVALID_EDITFIELDS equ 24
MAPI_E_INVALID_RECIPS equ 25
MAPI_E_NOT_SUPPORTED equ 26

MapiFileDesc STRUCT
    ulReserved dd ?
    flFlags dd ?
    nPosition dd ?
    lpszPathName dd ?
    lpszFileName dd ?
    lpFileType dd ?
MapiFileDesc ENDS

MapiFileTagExt STRUCT
    ulReserved dd ?
    cbTag dd ?
    lpTag dd ?
    cbEncoding dd ?
    lpEncoding dd ?
MapiFileTagExt ENDS

MapiRecipDesc STRUCT
    ulReserved dd ?
    ulRecipClass dd ?
    lpszName dd ?
    lpszAddress dd ?
    ulEIDSize dd ?
    lpEntryID dd ?
MapiRecipDesc ENDS

MapiMessage STRUCT
    ulReserved dd ?
    lpszSubject dd ?
    lpszNoteText dd ?
    lpszMessageType dd ?
    lpszDateReceived dd ?
    lpszConversationID dd ?
    flFlags dd ?
    lpOriginator dd ?
    nRecipCount dd ?
    lpRecips dd ?
    nFileCount dd ?
    lpFiles dd ?
MapiMessage ENDS

;-------------------------------------[ mapicode.h ]-------------------------------------

MAPI_E_CALL_FAILED equ E_FAIL
MAPI_E_NOT_ENOUGH_MEMORY equ E_OUTOFMEMORY
MAPI_E_INVALID_PARAMETER equ E_INVALIDARG
MAPI_E_INTERFACE_NOT_SUPPORTED equ E_NOINTERFACE
MAPI_E_NO_ACCESS equ E_ACCESSDENIED
MAPI_E_NO_SUPPORT equ 80040102h
MAPI_E_BAD_CHARWIDTH equ 80040103h
MAPI_E_STRING_TOO_LONG equ 80040105h
MAPI_E_UNKNOWN_FLAGS equ 80040106h
MAPI_E_INVALID_ENTRYID equ 80040107h
MAPI_E_INVALID_OBJECT equ 80040108h
MAPI_E_OBJECT_CHANGED equ 80040109h
MAPI_E_OBJECT_DELETED equ 8004010Ah
MAPI_E_BUSY equ 8004010Bh
MAPI_E_NOT_ENOUGH_DISK equ 8004010Dh
MAPI_E_NOT_ENOUGH_RESOURCES equ 8004010Eh
MAPI_E_NOT_FOUND equ 8004010Fh
MAPI_E_VERSION equ 80040110h
MAPI_E_LOGON_FAILED equ 80040111h
MAPI_E_SESSION_LIMIT equ 80040112h
MAPI_E_USER_CANCEL equ 80040113h
MAPI_E_UNABLE_TO_ABORT equ 80040114h
MAPI_E_NETWORK_ERROR equ 80040115h
MAPI_E_DISK_ERROR equ 80040116h
MAPI_E_TOO_COMPLEX equ 80040117h
MAPI_E_BAD_COLUMN equ 80040118h
MAPI_E_EXTENDED_ERROR equ 80040119h
MAPI_E_COMPUTED equ 8004011Ah
MAPI_E_CORRUPT_DATA equ 8004011Bh
MAPI_E_UNCONFIGURED equ 8004011Ch
MAPI_E_FAILONEPROVIDER equ 8004011Dh
MAPI_E_UNKNOWN_CPID equ 8004011Eh
MAPI_E_UNKNOWN_LCID equ 8004011Fh
MAPI_E_PASSWORD_CHANGE_REQUIRED equ 80040120h
MAPI_E_PASSWORD_EXPIRED equ 80040121h
MAPI_E_INVALID_WORKSTATION_ACCOUNT equ 80040122h
MAPI_E_INVALID_ACCESS_TIME equ 80040123h
MAPI_E_ACCOUNT_DISABLED equ 80040124h
MAPI_E_END_OF_SESSION equ 80040200h
MAPI_E_UNKNOWN_ENTRYID equ 80040201h
MAPI_E_MISSING_REQUIRED_COLUMN equ 80040202h
MAPI_W_NO_SERVICE equ 40203h
MAPI_E_BAD_VALUE equ 80040301h
MAPI_E_INVALID_TYPE equ 80040302h
MAPI_E_TYPE_NO_SUPPORT equ 80040303h
MAPI_E_UNEXPECTED_TYPE equ 80040304h
MAPI_E_TOO_BIG equ 80040305h
MAPI_E_DECLINE_COPY equ 80040306h
MAPI_E_UNEXPECTED_ID equ 80040307h
MAPI_W_ERRORS_RETURNED equ 40380h
MAPI_E_UNABLE_TO_COMPLETE equ 80040400h
MAPI_E_TIMEOUT equ 80040401h
MAPI_E_TABLE_EMPTY equ 80040402h
MAPI_E_TABLE_TOO_BIG equ 80040403h
MAPI_E_INVALID_BOOKMARK equ 80040405h
MAPI_W_POSITION_CHANGED equ 40481h
MAPI_W_APPROX_COUNT equ 40482h
MAPI_E_WAIT equ 80040500h
MAPI_E_CANCEL equ 80040501h
MAPI_E_NOT_ME equ 80040502h
MAPI_W_CANCEL_MESSAGE equ 40580h
MAPI_E_CORRUPT_STORE equ 80040600h
MAPI_E_NOT_IN_QUEUE equ 80040601h
MAPI_E_NO_SUPPRESS equ 80040602h
MAPI_E_COLLISION equ 80040604h
MAPI_E_NOT_INITIALIZED equ 80040605h
MAPI_E_NON_STANDARD equ 80040606h
MAPI_E_NO_RECIPIENTS equ 80040607h
MAPI_E_SUBMITTED equ 80040608h
MAPI_E_HAS_FOLDERS equ 80040609h
MAPI_E_HAS_MESSAGES equ 8004060Ah
MAPI_E_FOLDER_CYCLE equ 8004060Bh
MAPI_W_PARTIAL_COMPLETION equ 40680h
MAPI_E_AMBIGUOUS_RECIP equ 80040700h

;-----------------------------[ mapidefs.h ]-----------------------------
MAPI_DIM equ 1
MAPI_NT_SERVICE equ 10000h
MAPI_MODIFY equ 1
MAPI_ACCESS_MODIFY equ 1
MAPI_ACCESS_READ equ 2
MAPI_ACCESS_DELETE equ 4
MAPI_ACCESS_CREATE_HIERARCHY equ 8
MAPI_ACCESS_CREATE_CONTENTS equ 10h
MAPI_ACCESS_CREATE_ASSOCIATED equ 20h
MAPI_UNICODE equ 80000000h
fMapiUnicode equ 0
hrSuccess equ 0
MAPI_P1 equ 10000000h
MAPI_SUBMITTED equ 80000000h
MAPI_SHORTTERM equ 80h
MAPI_NOTRECIP equ 40h
MAPI_THISSESSION equ 20h
MAPI_NOW equ 10h
MAPI_NOTRESERVED equ 08h
MAPI_COMPOUND equ 80h
MAPI_ONE_OFF_UID equ { 81h, 2bh, 1fh, 0a4h, 0beh, 0a3h, 10h, 19h,09dh, 6eh, 0h, 0ddh, 01h, 0fh, 54h, 02h }
MAPI_ONE_OFF_UNICODE equ 8000h
MAPI_ONE_OFF_NO_RICH_INFO equ 1h
MAPI_STORE equ 1
MAPI_ADDRBOOK equ 2
MAPI_FOLDER equ 3
MAPI_ABCONT equ 4
MAPI_MESSAGE equ 5
MAPI_MAILUSER equ 6
MAPI_ATTACH equ 7
MAPI_DISTLIST equ 8
MAPI_PROFSECT equ 9
MAPI_STATUS equ 0Ah
MAPI_SESSION equ 0Bh
MAPI_FORMINFO equ 0Ch
cchProfileNameMax equ 64
cchProfilePassMax equ 64
MV_FLAG equ 1000h
PT_UNSPECIFIED equ 0
PT_NULL equ 1
PT_I2 equ 2
PT_LONG equ 3
PT_R4 equ 4
PT_DOUBLE equ 5
PT_CURRENCY equ 6
PT_APPTIME equ 7
PT_ERROR equ 10
PT_BOOLEAN equ 11
PT_OBJECT equ 13
PT_I8 equ 20
PT_STRING8 equ 30
PT_UNICODE equ 31
PT_SYSTIME equ 64
PT_CLSID equ 72
PT_BINARY equ 258
PT_SHORT equ PT_I2
PT_I4 equ PT_LONG
PT_FLOAT equ PT_R4
PT_R8 equ PT_DOUBLE
PT_LONGLONG equ PT_I8
PROP_TYPE_MASK equ 0FFFFh
PROP_ID_NULL equ 0
PROP_ID_INVALID equ 0FFFFh
PR_NULL equ (PROP_ID_NULL shl 16) +PT_NULL
PT_MV_I2 equ MV_FLAG+PT_I2
PT_MV_LONG equ MV_FLAG+PT_LONG
PT_MV_R4 equ MV_FLAG+PT_R4
PT_MV_DOUBLE equ MV_FLAG+PT_DOUBLE
PT_MV_CURRENCY equ MV_FLAG+PT_CURRENCY
PT_MV_APPTIME equ MV_FLAG+PT_APPTIME
PT_MV_SYSTIME equ MV_FLAG+PT_SYSTIME
PT_MV_STRING8 equ MV_FLAG+PT_STRING8
PT_MV_BINARY equ MV_FLAG+PT_BINARY
PT_MV_UNICODE equ MV_FLAG+PT_UNICODE
PT_MV_CLSID equ MV_FLAG+PT_CLSID
PT_MV_I8 equ MV_FLAG+PT_I8
PT_MV_SHORT equ PT_MV_I2
PT_MV_I4 equ PT_MV_LONG
PT_MV_FLOAT equ PT_MV_R4
PT_MV_R8 equ PT_MV_DOUBLE
PT_MV_LONGLONG equ PT_MV_I8
MV_INSTANCE equ 2000h
MVI_FLAG equ MV_FLAG + MV_INSTANCE
fnevCriticalError equ 1
fnevNewMail equ 2
fnevObjectCreated equ 4
fnevObjectDeleted equ 8
fnevObjectModified equ 10h
fnevObjectMoved equ 20h
fnevObjectCopied equ 40h
fnevSearchComplete equ 80h
fnevTableModified equ 100h
fnevStatusObjectModified equ 200h
fnevReservedForMapi equ 40000000h
fnevExtended equ 80000000h
TABLE_CHANGED equ 1
TABLE_ERROR equ 2
TABLE_ROW_ADDED equ 3
TABLE_ROW_DELETED equ 4
TABLE_ROW_MODIFIED equ 5
TABLE_SORT_DONE equ 6
TABLE_RESTRICT_DONE equ 7
TABLE_SETCOL_DONE equ 8
TABLE_RELOAD equ 9
MAPI_TOP_LEVEL equ 01h
MAPI_ERROR_VERSION equ 0
KEEP_OPEN_READONLY equ 1
KEEP_OPEN_READWRITE equ 2
FORCE_SAVE equ 4
MAPI_CREATE equ 2
STREAM_APPEND equ 4
MAPI_MOVE equ 1
MAPI_NOREPLACE equ 2
MAPI_DECLINE_OK equ 4
MAPI_USE_DEFAULT equ 40h
MAPI_NO_STRINGS equ 1
MAPI_NO_IDS equ 2
MNID_ID equ 0
MNID_STRING equ 1
TBLSTAT_COMPLETE equ 0
TBLSTAT_QCHANGED equ 7
TBLSTAT_SORTING equ 9
TBLSTAT_SORT_ERROR equ 10
TBLSTAT_SETTING_COLS equ 11
TBLSTAT_SETCOL_ERROR equ 13
TBLSTAT_RESTRICTING equ 14
TBLSTAT_RESTRICT_ERROR equ 15
TBLTYPE_SNAPSHOT equ 0
TBLTYPE_KEYSET equ 1
TBLTYPE_DYNAMIC equ 2
TABLE_SORT_ASCEND equ 0
TABLE_SORT_DESCEND equ 1
TABLE_SORT_COMBINE equ 2
BOOKMARK_BEGINNING equ 0
BOOKMARK_CURRENT equ 1
BOOKMARK_END equ 2
FL_FULLSTRING equ 0
FL_SUBSTRING equ 1
FL_PREFIX equ 2
FL_IGNORECASE equ 10000h
FL_IGNORENONSPACE equ 20000h
FL_LOOSE equ 40000h
RES_AND equ 0
RES_OR equ 1
RES_NOT equ 2
RES_CONTENT equ 3
RES_PROPERTY equ 4
RES_COMPAREPROPS equ 5
RES_BITMASK equ 6
RES_SIZE equ 7
RES_EXIST equ 8
RES_SUBRESTRICTION equ 9
RES_COMMENT equ 0Ah
RELOP_LT equ 0
RELOP_LE equ 1
RELOP_GT equ 2
RELOP_GE equ 3
RELOP_EQ equ 4
RELOP_NE equ 5
RELOP_RE equ 6
BMR_EQZ equ 0
BMR_NEZ equ 1
TBL_ALL_COLUMNS equ 1
TBL_LEAF_ROW equ 1
TBL_EMPTY_CATEGORY equ 2
TBL_EXPANDED_CATEGORY equ 3
TBL_COLLAPSED_CATEGORY equ 4
TBL_NOWAIT equ 1
TBL_ASYNC equ 1
TBL_BATCH equ 2
DIR_BACKWARD equ 1
TBL_NOADVANCE equ 1
PS_PROFILE_PROPERTIES_INIT equ {098h, 15h, 0ACh, 08h, 0AAh, 0B0h, 10h, 1Ah,8Ch, 093h, 08h, 0h, 2Bh, 2Ah, 56h, 0C2h }
MAPI_STORE_PROVIDER equ 33
MAPI_AB equ 34
MAPI_AB_PROVIDER equ 35
MAPI_TRANSPORT_PROVIDER equ 36
MAPI_SPOOLER equ 37
MAPI_PROFILE_PROVIDER equ 38
MAPI_SUBSYSTEM equ 39
MAPI_HOOK_PROVIDER equ 40
STATUS_VALIDATE_STATE equ 1
STATUS_SETTINGS_DIALOG equ 2
STATUS_CHANGE_PASSWORD equ 4
STATUS_FLUSH_QUEUES equ 8
STATUS_DEFAULT_OUTBOUND equ 1
STATUS_DEFAULT_STORE equ 2
STATUS_PRIMARY_IDENTITY equ 4
STATUS_SIMPLE_STORE equ 8
STATUS_XP_PREFER_LAST equ 10h
STATUS_NO_PRIMARY_IDENTITY equ 20h
STATUS_NO_DEFAULT_STORE equ 40h
STATUS_TEMP_SECTION equ 80h
STATUS_OWN_STORE equ 100h
STATUS_NEED_IPM_TREE equ 800h
STATUS_PRIMARY_STORE equ 1000h
STATUS_SECONDARY_STORE equ 2000h
STATUS_AVAILABLE equ 1
STATUS_OFFLINE equ 2
STATUS_FAILURE equ 4
STATUS_INBOUND_ENABLED equ 10000h
STATUS_INBOUND_ACTIVE equ 20000h
STATUS_INBOUND_FLUSH equ 40000h
STATUS_OUTBOUND_ENABLED equ 100000h
STATUS_OUTBOUND_ACTIVE equ 200000h
STATUS_OUTBOUND_FLUSH equ 400000h
STATUS_REMOTE_ACCESS equ 800000h
SUPPRESS_UI equ 1
REFRESH_XP_HEADER_CACHE equ 10000h
PROCESS_XP_HEADER_CACHE equ 20000h
FORCE_XP_CONNECT equ 40000h
FORCE_XP_DISCONNECT equ 80000h
CONFIG_CHANGED equ 100000h
ABORT_XP_HEADER_OPERATION equ 200000h
SHOW_XP_SESSION_UI equ 400000h
UI_READONLY equ 1
FLUSH_UPLOAD equ 2
FLUSH_DOWNLOAD equ 4
FLUSH_FORCE equ 8
FLUSH_NO_UI equ 10h
FLUSH_ASYNC_OK equ 20h
MAPI_BEST_ACCESS equ 10h
CONVENIENT_DEPTH equ 1
SEARCH_RUNNING equ 1
SEARCH_REBUILD equ 2
SEARCH_RECURSIVE equ 4
SEARCH_FOREGROUND equ 8
STOP_SEARCH equ 1
RESTART_SEARCH equ 2
RECURSIVE_SEARCH equ 4
SHALLOW_SEARCH equ 8
FOREGROUND_SEARCH equ 10h
BACKGROUND_SEARCH equ 20h
AB_RECIPIENTS equ 1
AB_SUBCONTAINERS equ 2
AB_MODIFIABLE equ 4
AB_UNMODIFIABLE equ 8
AB_FIND_ON_OPEN equ 10h
AB_NOT_DEFAULT equ 20h
CREATE_CHECK_DUP_STRICT equ 1
CREATE_CHECK_DUP_LOOSE equ 2
CREATE_REPLACE equ 4
MAPI_UNRESOLVED equ 0
MAPI_AMBIGUOUS equ 1
MAPI_RESOLVED equ 2
MAPI_SEND_NO_RICH_INFO equ 10000h
MAPI_DIAG_NO_DIAGNOSTIC equ -1 
MAPI_DIAG_OR_NAME_UNRECOGNIZED equ 0 
MAPI_DIAG_OR_NAME_AMBIGUOUS equ 1 
MAPI_DIAG_MTS_CONGESTED equ 2 
MAPI_DIAG_LOOP_DETECTED equ 3 
MAPI_DIAG_RECIPIENT_UNAVAILABLE equ 4 
MAPI_DIAG_MAXIMUM_TIME_EXPIRED equ 5 
MAPI_DIAG_EITS_UNSUPPORTED equ 6 
MAPI_DIAG_CONTENT_TOO_LONG equ 7 
MAPI_DIAG_IMPRACTICAL_TO_CONVERT equ 8 
MAPI_DIAG_PROHIBITED_TO_CONVERT equ 9 
MAPI_DIAG_CONVERSION_UNSUBSCRIBED equ 10 
MAPI_DIAG_PARAMETERS_INVALID equ 11 
MAPI_DIAG_CONTENT_SYNTAX_IN_ERROR equ 12 
MAPI_DIAG_LENGTH_CONSTRAINT_VIOLATD equ 13 
MAPI_DIAG_NUMBER_CONSTRAINT_VIOLATD equ 14 
MAPI_DIAG_CONTENT_TYPE_UNSUPPORTED equ 15 
MAPI_DIAG_TOO_MANY_RECIPIENTS equ 16 
MAPI_DIAG_NO_BILATERAL_AGREEMENT equ 17 
MAPI_DIAG_CRITICAL_FUNC_UNSUPPORTED equ 18 
MAPI_DIAG_CONVERSION_LOSS_PROHIB equ 19 
MAPI_DIAG_LINE_TOO_LONG equ 20 
MAPI_DIAG_PAGE_TOO_LONG equ 21 
MAPI_DIAG_PICTORIAL_SYMBOL_LOST equ 22 
MAPI_DIAG_PUNCTUATION_SYMBOL_LOST equ 23 
MAPI_DIAG_ALPHABETIC_CHARACTER_LOST equ 24 
MAPI_DIAG_MULTIPLE_INFO_LOSSES equ 25 
MAPI_DIAG_REASSIGNMENT_PROHIBITED equ 26 
MAPI_DIAG_REDIRECTION_LOOP_DETECTED equ 27 
MAPI_DIAG_EXPANSION_PROHIBITED equ 28 
MAPI_DIAG_SUBMISSION_PROHIBITED equ 29 
MAPI_DIAG_EXPANSION_FAILED equ 30 
MAPI_DIAG_RENDITION_UNSUPPORTED equ 31 
MAPI_DIAG_MAIL_ADDRESS_INCORRECT equ 32 
MAPI_DIAG_MAIL_OFFICE_INCOR_OR_INVD equ 33 
MAPI_DIAG_MAIL_ADDRESS_INCOMPLETE equ 34 
MAPI_DIAG_MAIL_RECIPIENT_UNKNOWN equ 35 
MAPI_DIAG_MAIL_RECIPIENT_DECEASED equ 36 
MAPI_DIAG_MAIL_ORGANIZATION_EXPIRED equ 37 
MAPI_DIAG_MAIL_REFUSED equ 38 
MAPI_DIAG_MAIL_UNCLAIMED equ 39 
MAPI_DIAG_MAIL_RECIPIENT_MOVED equ 40 
MAPI_DIAG_MAIL_RECIPIENT_TRAVELLING equ 41 
MAPI_DIAG_MAIL_RECIPIENT_DEPARTED equ 42 
MAPI_DIAG_MAIL_NEW_ADDRESS_UNKNOWN equ 43 
MAPI_DIAG_MAIL_FORWARDING_UNWANTED equ 44 
MAPI_DIAG_MAIL_FORWARDING_PROHIB equ 45 
MAPI_DIAG_SECURE_MESSAGING_ERROR equ 46 
MAPI_DIAG_DOWNGRADING_IMPOSSIBLE equ 47 
MAPI_MH_DP_PUBLIC_UA equ 0
MAPI_MH_DP_PRIVATE_UA equ 1
MAPI_MH_DP_MS equ 2
MAPI_MH_DP_ML equ 3
MAPI_MH_DP_PDAU equ 4
MAPI_MH_DP_PDS_PATRON equ 5
MAPI_MH_DP_OTHER_AU equ 6
FOLDER_ROOT equ 0
FOLDER_GENERIC equ 1
FOLDER_SEARCH equ 2
MESSAGE_MOVE equ 1
MESSAGE_DIALOG equ 2
OPEN_IF_EXISTS equ 1
DEL_MESSAGES equ 1
FOLDER_DIALOG equ 2
DEL_FOLDERS equ 4
DEL_ASSOCIATED equ 8
FOLDER_MOVE equ 1
COPY_SUBFOLDERS equ 10h
MSGSTATUS_HIGHLIGHTED equ 01h
MSGSTATUS_TAGGED equ 2
MSGSTATUS_HIDDEN equ 4
MSGSTATUS_DELMARKED equ 8
MSGSTATUS_REMOTE_DOWNLOAD equ 01000h
MSGSTATUS_REMOTE_DELETE equ 02000h
RECURSIVE_SORT equ 2
FLDSTATUS_HIGHLIGHTED equ 1
FLDSTATUS_TAGGED equ 2
FLDSTATUS_HIDDEN equ 4
FLDSTATUS_DELMARKED equ 8
STORE_ENTRYID_UNIQUE equ 1
STORE_READONLY equ 2
STORE_SEARCH_OK equ 4
STORE_MODIFY_OK equ 8
STORE_CREATE_OK equ 10h
STORE_ATTACH_OK equ 20h
STORE_OLE_OK equ 40h
STORE_SUBMIT_OK equ 80h
STORE_NOTIFY_OK equ 100h
STORE_MV_PROPS_OK equ 200h
STORE_CATEGORIZE_OK equ 400h
STORE_RTF_OK equ 800h
STORE_RESTRICTION_OK equ 1000h
STORE_SORT_OK equ 2000h
STORE_PUBLIC_FOLDERS equ 4000h
STORE_UNCOMPRESSED_RTF equ 8000h
STORE_HAS_SEARCHES equ 1000000h
LOGOFF_NO_WAIT equ 1
LOGOFF_ORDERLY equ 2
LOGOFF_PURGE equ 4
LOGOFF_ABORT equ 8
LOGOFF_QUIET equ 10h
LOGOFF_COMPLETE equ 10000h
LOGOFF_INBOUND equ 20000h
LOGOFF_OUTBOUND equ 40000h
LOGOFF_OUTBOUND_QUEUE equ 80000h
MSG_LOCKED equ 1
MSG_UNLOCKED equ 0
FOLDER_IPM_SUBTREE_VALID equ 1
FOLDER_IPM_INBOX_VALID equ 2
FOLDER_IPM_OUTBOX_VALID equ 4
FOLDER_IPM_WASTEBASKET_VALID equ 8
FOLDER_IPM_SENTMAIL_VALID equ 10h
FOLDER_VIEWS_VALID equ 20h
FOLDER_COMMON_VIEWS_VALID equ 40h
FOLDER_FINDER_VALID equ 80h
FORCE_SUBMIT equ 1
MSGFLAG_READ equ 1
MSGFLAG_UNMODIFIED equ 2
MSGFLAG_SUBMIT equ 4
MSGFLAG_UNSENT equ 8
MSGFLAG_HASATTACH equ 10h
MSGFLAG_FROMME equ 20h
MSGFLAG_ASSOCIATED equ 40h
MSGFLAG_RESEND equ 80h
MSGFLAG_RN_PENDING equ 100h
MSGFLAG_NRN_PENDING equ 200h
SUBMITFLAG_LOCKED equ 1
SUBMITFLAG_PREPROCESS equ 2
MODRECIP_ADD equ 2
MODRECIP_MODIFY equ 4
MODRECIP_REMOVE equ 8
SUPPRESS_RECEIPT equ 1
CLEAR_READ_FLAG equ 4
GENERATE_RECEIPT_ONLY equ 10h
CLEAR_RN_PENDING equ 20h
CLEAR_NRN_PENDING equ 40h
ATTACH_DIALOG equ 1
SECURITY_SIGNED equ 1
SECURITY_ENCRYPTED equ 2
PRIO_URGENT equ 1
PRIO_NORMAL equ 0
PRIO_NONURGENT equ -1
SENSITIVITY_NONE equ 0
SENSITIVITY_PERSONAL equ 1
SENSITIVITY_PRIVATE equ 2
SENSITIVITY_COMPANY_CONFIDENTIAL equ 3
IMPORTANCE_LOW equ 0
IMPORTANCE_NORMAL equ 1
IMPORTANCE_HIGH equ 2
NO_ATTACHMENT equ 0
ATTACH_BY_VALUE equ 1
ATTACH_BY_REFERENCE equ 2
ATTACH_BY_REF_RESOLVE equ 3
ATTACH_BY_REF_ONLY equ 4
ATTACH_EMBEDDED_MSG equ 5
ATTACH_OLE equ 6
ADRPARM_HELP_CTX equ 0
DIALOG_MODAL equ 1
DIALOG_SDI equ 2
DIALOG_OPTIONS equ 4
ADDRESS_ONE equ 8
AB_SELECTONLY equ 10h
AB_RESOLVE equ 20h
DT_MAILUSER equ 0
DT_DISTLIST equ 1
DT_FORUM equ 2
DT_AGENT equ 3
DT_ORGANIZATION equ 4
DT_PRIVATE_DISTLIST equ 5
DT_REMOTE_MAILUSER equ 6
DT_MODIFIABLE equ 10000h
DT_GLOBAL equ 20000h
DT_LOCAL equ 30000h
DT_WAN equ 40000h
DT_NOT_SPECIFIC equ 50000h
DT_FOLDER equ 1000000h
DT_FOLDER_LINK equ 2000000h
DT_FOLDER_SPECIAL equ 4000000h
MAPI_DEFERRED_ERRORS equ 8h
MAPI_ASSOCIATED equ 40h
MDB_NO_DIALOG equ 1h
MDB_WRITE equ 4h
MDB_TEMPORARY equ 20h
MDB_NO_MAIL equ 80h
AB_NO_DIALOG equ 1
MAPI_ENABLED equ 0
MAPI_DISABLED equ 1
DT_MULTILINE equ 1
DT_EDITABLE equ 2
DT_REQUIRED equ 4
DT_SET_IMMEDIATE equ 8
DT_PASSWORD_EDIT equ 10h
DT_ACCEPT_DBCS equ 20h
DT_SET_SELECTION equ 40h
DTCT_LABEL equ 0
DTCT_EDIT equ 1
DTCT_LBX equ 2
DTCT_COMBOBOX equ 3
DTCT_DDLBX equ 4
DTCT_CHECKBOX equ 5
DTCT_GROUPBOX equ 6
DTCT_BUTTON equ 7
DTCT_PAGE equ 8
DTCT_RADIOBUTTON equ 9
DTCT_MVLISTBOX equ 0Bh
DTCT_MVDDLBX equ 0Ch
MAPI_NO_HBAR equ 1
MAPI_NO_VBAR equ 2
UI_SERVICE equ 2
SERVICE_UI_ALWAYS equ 2
SERVICE_UI_ALLOWED equ 10h
UI_CURRENT_PROVIDER_FIRST equ 4

CY UNION
  STRUCT
    Lo DWORD ?
    Hi DWORD ?
  ENDS
  int64 QWORD ?
CY ENDS

CURRENCY TEXTEQU <CY>

ENTRYID STRUCT
    abFlags db 4 dup(?)
    ab db MAPI_DIM dup(?)
ENTRYID ENDS

MAPIUID STRUCT
    ab db 16 dup(?)
MAPIUID ENDS

SPropTagArray STRUCT
    cValues dd ?
    aulPropTag dd MAPI_DIM dup(?)
SPropTagArray ENDS

SBinary STRUCT
    cb dd ?
    lpb dd ?
SBinary ENDS

SShortArray STRUCT
    cValues dd ?
    lpi dd ?
SShortArray ENDS

SGuidArray STRUCT
    cValues dd ?
    lpguid dd ?
SGuidArray ENDS

SRealArray STRUCT
    cValues dd ?
    lpflt dd ?
SRealArray ENDS

SLongArray STRUCT
    cValues dd ?
    lpl dd ?
SLongArray ENDS

SLargeIntegerArray STRUCT
    cValues dd ?
    lpli dd ?
SLargeIntegerArray ENDS

SDateTimeArray STRUCT
    cValues dd ?
	 lpft dd ?
SDateTimeArray ENDS

SAppTimeArray STRUCT
    cValues dd ?
    lpat dd ?
SAppTimeArray ENDS

SCurrencyArray STRUCT
    cValues dd ?
    lpcur dd ?
SCurrencyArray ENDS

SBinaryArray STRUCT
    cValues dd ?
    lpbin dd ?
SBinaryArray ENDS

SDoubleArray STRUCT
    cValues dd ?
    lpdbl dd ?
SDoubleArray ENDS

SWStringArray STRUCT
    cValues dd ?
    lppszW dd ?
SWStringArray ENDS

SLPSTRArray STRUCT
    cValues dd ?
    lppszA dd ?
SLPSTRArray ENDS

UPV UNION DWORD
    i dw ?
    l dd ?
    ul dd ?
    flt dd ?
    dbl dq ?
    b dw ?
    cur CURRENCY <>
    at dq ?
    ft FILETIME <>
    lpszA dd ?
    bin SBinary <>
    lpszW dd ?
    lpguid dd ?
    li LARGE_INTEGER <>
    MVi SShortArray <>
    MVl SLongArray <>
    MVflt SRealArray <>
    MVdbl SDoubleArray <>
    MVcur SCurrencyArray <>
    MVat SAppTimeArray <>
    MVft SDateTimeArray <>
    MVbin SBinaryArray <>
    MVszA SLPSTRArray <>
    MVszW SWStringArray <>
    MVguid SGuidArray <>
    MVli SLargeIntegerArray <>
    err SCODE ?
    x LONG ?
UPV ENDS

SPropValue STRUCT
    ulPropTag dd ?
    dwAlignPad dd ?
    Value UPV <>
SPropValue ENDS

SPropProblem STRUCT
    ulIndex dd ?
    ulPropTag dd ?
    scode SCODE ?
SPropProblem ENDS

SPropProblemArray STRUCT
    cProblem dd ?
    aProblem SPropProblem MAPI_DIM dup(<>)
SPropProblemArray ENDS

FLATENTRY STRUCT
    cb dd ?
    abEntry db MAPI_DIM dup(?)
FLATENTRY ENDS

FLATENTRYLIST STRUCT
    cEntries dd ?
    cbEntries dd ?
    abEntries db MAPI_DIM dup(?)
FLATENTRYLIST ENDS

MTSID STRUCT
    cb dd ?
    ab db MAPI_DIM dup(?)
MTSID ENDS

FLATMTSIDLIST STRUCT
    cMTSIDs dd ?
    cbMTSIDs dd ?
    abMTSIDs db MAPI_DIM dup(?)
FLATMTSIDLIST ENDS

ADRENTRY STRUCT
    ulReserved1 dd ?
    cValues dd ?
    rgPropVals dd ?
ADRENTRY ENDS

ADRLIST STRUCT
    cEntries dd ?
    aEntries ADRENTRY MAPI_DIM dup(<>)
ADRLIST ENDS

SRow STRUCT
    ulAdrEntryPad dd ?
    cValues dd ?
    lpProps dd ?
SRow ENDS

SRowSet STRUCT
    cRows dd ?
    aRow SRow MAPI_DIM dup(<>)
SRowSet ENDS

MAPIERROR STRUCT
    ulVersion dd ?
    lpszError dd ?
    lpszComponent dd ?
    ulLowLevelError dd ?
    ulContext dd ?
MAPIERROR ENDS

ERROR_NOTIFICATION STRUCT
    cbEntryID dd ?
    lpEntryID dd ?
    scode SCODE ?
    ulFlags dd ?
    lpMAPIError dd ?
ERROR_NOTIFICATION ENDS

NEWMAIL_NOTIFICATION STRUCT
    cbEntryID dd ?
    lpEntryID dd ?
    cbParentID dd ?
    lpParentID dd ?
    ulFlags dd ?
    lpszMessageClass dd ?
    ulMessageFlags dd ?
NEWMAIL_NOTIFICATION ENDS

OBJECT_NOTIFICATION STRUCT
    cbEntryID dd ?
    lpEntryID dd ?
    ulObjType dd ?
    cbParentID dd ?
    lpParentID dd ?
    cbOldID dd ?
    lpOldID dd ?
    cbOldParentID dd ?
    lpOldParentID dd ?
    lpPropTagArray dd ?
OBJECT_NOTIFICATION ENDS

TABLE_NOTIFICATION STRUCT
    ulTableEvent dd ?
    hResult dd ?
    propIndex SPropValue <>
    propPrior SPropValue <>
    row SRow <>
    ulPad dd ?
TABLE_NOTIFICATION ENDS

EXTENDED_NOTIFICATION STRUCT
    ulEvent dd ?
    cb dd ?
    pbEventParameters dd ?
EXTENDED_NOTIFICATION ENDS

STATUS_OBJECT_NOTIFICATION STRUCT
    cbEntryID dd ?
    lpEntryID dd ?
    cValues dd ?
    lpPropVals dd ?
STATUS_OBJECT_NOTIFICATION ENDS

NOTIFICATION STRUCT
    ulEventType dd ?
    ulAlignPad dd ?
    union info
        err ERROR_NOTIFICATION <>
        newmail NEWMAIL_NOTIFICATION <>
        obj OBJECT_NOTIFICATION <>
        tab TABLE_NOTIFICATION <>
        ext EXTENDED_NOTIFICATION <>
        statobj STATUS_OBJECT_NOTIFICATION <>
    ends
NOTIFICATION ENDS

MAPINAMEID STRUCT
    lpguid dd ?
    ulKind dd ?
    union Kind
        lID dd ?
        lpwstrName dd ?
    ends
MAPINAMEID ENDS

SSortOrder STRUCT
    ulPropTag dd ?
    ulOrder dd ?
SSortOrder ENDS

SSortOrderSet STRUCT
    cSorts dd ?
    cCategories dd ?
    cExpanded dd ?
    aSort SSortOrder MAPI_DIM dup(<>)
SSortOrderSet ENDS

SAndRestriction STRUCT
    cRes dd ?
    lpRes dd ?
SAndRestriction ENDS

SOrRestriction STRUCT
    cRes dd ?
    lpRes dd ?
SOrRestriction ENDS

SNotRestriction STRUCT
    ulReserved dd ?
    lpRes dd ?
SNotRestriction ENDS

SContentRestriction STRUCT
    ulFuzzyLevel dd ?
    ulPropTag dd ?
    lpProp dd ?
SContentRestriction ends

SBitMaskRestriction STRUCT
    relBMR dd ?
    ulPropTag dd ?
    ulMask dd ?
SBitMaskRestriction ENDS

SPropertyRestriction STRUCT
    relop dd ?
    ulPropTag dd ?
    lpProp dd ?
SPropertyRestriction ENDS

SComparePropsRestriction STRUCT
    relop dd ?
    ulPropTag1 dd ?
    ulPropTag2 dd ?
SComparePropsRestriction ENDS

SSizeRestriction STRUCT
    relop dd ?
    ulPropTag dd ?
    cb dd ?
SSizeRestriction ENDS

SExistRestriction STRUCT
    ulReserved1 dd ?
    ulPropTag dd ?
    ulReserved2 dd ?
SExistRestriction ENDS

SSubRestriction STRUCT
    ulSubObject dd ?
    lpRes dd ?
SSubRestriction ENDS

SCommentRestriction STRUCT
    cValues dd ?
    lpRes dd ?
    lpProp dd ?
SCommentRestriction ENDS

SRestriction STRUCT
    rt dd ?
    union res
        resCompareProps SComparePropsRestriction <>
        resAnd SAndRestriction <>
        resOr SOrRestriction <>
        resNot SNotRestriction <>
        resContent SContentRestriction <>
        resProperty SPropertyRestriction <>
        resBitMask SBitMaskRestriction <>
        resSize SSizeRestriction <>
        resExist SExistRestriction <>
        resSub SSubRestriction <>
        resComment SCommentRestriction <>
	ends
SRestriction ENDS

FlagList STRUCT
    cFlags dd ?
    ulFlag dd MAPI_DIM dup(?)
FlagList ENDS

ADRPARM STRUCT
    cbABContEntryID dd ?
    lpABContEntryID dd ?
    ulFlags dd ?
    lpReserved dd ?
    ulHelpContext dd ?
    lpszHelpFileName dd ?
    lpfnABSDI dd ?
    lpfnDismiss dd ?
    lpvDismissContext dd ?
    lpszCaption dd ?
    lpszNewEntryTitle dd ?
    lpszDestWellsTitle dd ?
    cDestFields dd ?
    nDestFieldFocus dd ?
    lppszDestTitles dd ?
    lpulDestComps dd ?
    lpContRestriction dd ?
    lpHierRestriction dd ?
ADRPARM ENDS

DTBLLABEL STRUCT
    ulbLpszLabelName dd ?
    ulFlags dd ?
DTBLLABEL ENDS

DTBLEDIT STRUCT
    ulbLpszCharsAllowed dd ?
    ulFlags dd ?
    ulNumCharsAllowed dd ?
    ulPropTag dd ?
DTBLEDIT ENDS

DTBLLBX STRUCT
    ulFlags dd ?
    ulPRSetProperty dd ?
    ulPRTableName dd ?
DTBLLBX ENDS

DTBLCOMBOBOX STRUCT
    ulbLpszCharsAllowed dd ?
    ulFlags dd ?
    ulNumCharsAllowed dd ?
    ulPRPropertyName dd ?
    ulPRTableName dd ?
DTBLCOMBOBOX ENDS

DTBLDDLBX STRUCT
    ulFlags dd ?
    ulPRDisplayProperty dd ?
    ulPRSetProperty dd ?
    ulPRTableName dd ?
DTBLDDLBX ENDS

DTBLCHECKBOX STRUCT
    ulbLpszLabel dd ?
    ulFlags dd ?
    ulPRPropertyName dd ?
DTBLCHECKBOX ENDS

DTBLGROUPBOX STRUCT
    ulbLpszLabel dd ?
    ulFlags dd ?
DTBLGROUPBOX ENDS

DTBLBUTTON STRUCT
    ulbLpszLabel dd ?
    ulFlags dd ?
    ulPRControl dd ?
DTBLBUTTON ENDS

DTBLPAGE STRUCT
    ulbLpszLabel dd ?
    ulFlags dd ?
    ulbLpszComponent dd ?
    ulContext dd ?
DTBLPAGE ENDS

DTBLRADIOBUTTON STRUCT
    ulbLpszLabel dd ?
    ulFlags dd ?
    ulcButtons dd ?
    ulPropTag dd ?
    lReturnValue dd ?
DTBLRADIOBUTTON ENDS

DTBLMVLISTBOX STRUCT
    ulFlags dd ?
    ulMVPropTag dd ?
DTBLMVLISTBOX ENDS

DTBLMVDDLBX STRUCT
    ulFlags dd ?
    ulMVPropTag dd ?
DTBLMVDDLBX ENDS

;-----------------------------------[ mapiform.h ]-------------------------------------
HFRMREG_DEFAULT equ 0
HFRMREG_LOCAL equ 1
HFRMREG_PERSONAL equ 2
HFRMREG_FOLDER equ 3
SAVE_FORMAT_TEXT equ 1
SAVE_FORMAT_RICHTEXT equ 2
VCSTATUS_NEXT equ 1
VCSTATUS_PREV equ 2
VCSTATUS_MODAL equ 4
VCSTATUS_INTERACTIVE equ 8
VCSTATUS_READONLY equ 10h
VCSTATUS_DELETE equ 10000h
VCSTATUS_COPY equ 20000h
VCSTATUS_MOVE equ 40000h
VCSTATUS_SUBMIT equ 80000h
VCSTATUS_DELETE_IS_MOVE equ 100000h
VCSTATUS_SAVE equ 200000h
VCSTATUS_NEW_MESSAGE equ 400000h
VCDIR_NEXT equ VCSTATUS_NEXT
VCDIR_PREV equ VCSTATUS_PREV
VCDIR_DELETE equ VCSTATUS_DELETE
VCDIR_MOVE equ VCSTATUS_MOVE
FPST_VANILLA equ 0
FPST_ENUM_PROP equ 1
MAPI_MESSAGE_BEHAVIOR_IPM equ 0
MAPI_MESSAGE_BEHAVIOR_FOLDER equ 1
MAPIFORM_SELECT_ALL_REGISTRIES equ 0
MAPIFORM_SELECT_FOLDER_REGISTRY_ONLY equ 1
MAPIFORM_SELECT_NON_FOLDER_REGISTRY_ONLY equ 2
FORMPROPSET_UNION equ 0
FORMPROPSET_INTERSECTION equ 1
MAPIFORM_EXACTMATCH equ 20h
MAPIFORM_CPU_X86 equ 1
MAPIFORM_CPU_MIP equ 2
MAPIFORM_CPU_AXP equ 3
MAPIFORM_CPU_PPC equ 4
MAPIFORM_CPU_M68 equ 5
MAPIFORM_OS_WIN_31 equ 1
MAPIFORM_OS_WINNT_35 equ 2
MAPIFORM_OS_WIN_95 equ 3
MAPIFORM_OS_MAC_7x equ 4
MAPIFORM_OS_WINNT_40 equ 5
MAPIFORM_INSTALL_DIALOG equ MAPI_DIALOG
MAPIFORM_INSTALL_OVERWRITEONCONFLICT equ 10h

SAVEOPTS_SAVEIFDIRTY equ 0
SAVEOPTS_NOSAVE equ 1
SAVEOPTS_PROMPTSAVE equ 2

FORMPRINTSETUP STRUCT
    ulFlags dd ?
    hDevMode dd ?
    hDevNames dd ?
    ulFirstPageNumber dd ?
    fPrintAttachments dd ?
FORMPRINTSETUP ENDS

SMAPIFormPropEnumVal STRUCT
    pszDisplayName dd ?
    nVal dd ?
SMAPIFormPropEnumVal ENDS

SMAPIFormProp STRUCT
    ulFlags dd ?
    nPropType dd ?
    nmid MAPINAMEID <>
    pszDisplayName dd ?
    nSpecialType dd ?
    union u
        struct sl
            nmidIdx MAPINAMEID <>
            cfpevAvailable dd ?
            pfpevAvailable dd ?
		  ends
	 ends
SMAPIFormProp ENDS

SMAPIFormPropArray STRUCT
    cProps dd ?
    ulPad dd ?
    aFormProp SMAPIFormProp MAPI_DIM dup(<>)
SMAPIFormPropArray ENDS

SMAPIVerb STRUCT
    lVerb dd ?
    szVerbname dd ?
    fuFlags dd ?
    grfAttribs dd ?
    ulFlags dd ?
SMAPIVerb ENDS

SMAPIVerbArray STRUCT
    cMAPIVerb dd ?
    aMAPIVerb SMAPIVerb MAPI_DIM dup(<>)
SMAPIVerbArray ENDS


SMessageClassArray STRUCT
    cValues dd ?
    aMessageClass dd MAPI_DIM dup(?)
SMessageClassArray ENDS

SMAPIFormInfoArray STRUCT
    cForms dd ?
    aFormInfo dd MAPI_DIM dup(?)
SMAPIFormInfoArray ENDS

;--------------------------------[ mapinls.h ]---------------------------
NORM_IGNOREKANATYPE equ 10000h
NORM_IGNOREWIDTH equ 20000h

;---------------------------[ mapioid.h ]-------------------------------
OID_TAG         equ 0Ah
OID_ENCODING    equ 0Bh
CB_OID_1        equ 9
CB_OID_2        equ 10
CB_OID_3        equ 11
CB_OID_4        equ 12

;--------------------------[ mapispi.h ]--------------------------------

CURRENT_SPI_VERSION equ 10010h
PDK1_SPI_VERSION equ 10000h
PDK2_SPI_VERSION equ 10008h
PDK3_SPI_VERSION equ 10010h
NOTIFY_SYNC equ 40000000h
NOTIFY_CANCELED equ 80000000h
CALLBACK_DISCONTINUE equ 80000000h
NOTIFY_NEWMAIL equ 1
NOTIFY_READYTOSEND equ 2
NOTIFY_SENTDEFERRED equ 4
NOTIFY_CRITSEC equ 1000h
NOTIFY_NONCRIT equ 2000h
NOTIFY_CONFIG_CHANGE equ 4000h
NOTIFY_CRITICAL_ERROR equ 10000000h
NOTIFY_NEWMAIL_RECEIVED equ 20000000h
STATUSROW_UPDATE equ 10000000h
STGSTRM_RESET equ 0
STGSTRM_CURRENT equ 10000000h
STGSTRM_MODIFY equ 2
STGSTRM_CREATE equ 1000h
MAPI_NON_READ equ 1
FILL_ENTRY equ 1
DEINIT_NORMAL equ 1
DEINIT_HURRY equ 80000000h
LOGON_NO_DIALOG equ 1
LOGON_NO_CONNECT equ 4
LOGON_NO_INBOUND equ 8
LOGON_NO_OUTBOUND equ 10h
LOGON_SP_IDLE equ 10000h
LOGON_SP_POLL equ 20000h
LOGON_SP_RESOLVE equ 40000h
OPTION_TYPE_RECIPIENT equ 1
OPTION_TYPE_MESSAGE equ 2
NOTIFY_ABORT_DEFERRED equ 40000000h
NOTIFY_CANCEL_MESSAGE equ 80000000h
NOTIFY_BEGIN_INBOUND equ 1
NOTIFY_END_INBOUND equ 10000h
NOTIFY_BEGIN_OUTBOUND equ 2
NOTIFY_END_OUTBOUND equ 20000h
NOTIFY_BEGIN_INBOUND_FLUSH equ 4
NOTIFY_END_INBOUND_FLUSH equ 40000h
NOTIFY_BEGIN_OUTBOUND_FLUSH equ 8
NOTIFY_END_OUTBOUND_FLUSH equ 80000h
LOGOFF_NORMAL equ 1
LOGOFF_HURRY equ 80000000h
BEGIN_DEFERRED equ 1
END_RESEND_NOW equ 10000h
END_RESEND_LATER equ 20000h
END_DONT_RESEND equ 40000h
LOGON_SP_TRANSPORT equ 1
LOGON_SP_PROMPT equ 2
LOGON_SP_NEWPW equ 4
LOGON_CHANGED equ 8
DIALOG_FOLDER equ 1
DIALOG_MESSAGE equ 2
DIALOG_PROP equ 4
DIALOG_ATTACH equ 8
DIALOG_MOVE equ 10h
DIALOG_COPY equ 20h
DIALOG_DELETE equ 40h
DIALOG_ALLOW_CANCEL equ 80h
DIALOG_CONFIRM_CANCEL equ 100h
NEEDS_PREPROCESSING equ 1
NEEDS_SPOOLER equ 2
CHECK_SENDER equ 1
NON_STANDARD equ 10000h
MSG_SERVICE_UI_READ_ONLY equ 8
SERVICE_LOGON_FAILED equ 20h
MSG_SERVICE_INSTALL equ 1
MSG_SERVICE_CREATE equ 2
MSG_SERVICE_CONFIGURE equ 3
MSG_SERVICE_DELETE equ 4
MSG_SERVICE_UNINSTALL equ 5
MSG_SERVICE_PROVIDER_CREATE equ 6
MSG_SERVICE_PROVIDER_DELETE equ 7

NOTIFKEY STRUCT
    cb dd ?
    ab db MAPI_DIM dup(?)
NOTIFKEY ENDS

OPTIONDATA STRUCT
    ulFlags dd ?
    lpRecipGUID dd ?
    lpszAdrType dd ?
    lpszDLLName dd ?
    ulOrdinal dd ?
    cbOptionsData dd ?
    lpbOptionsData dd ?
    cOptionsProps dd ?
    lpOptionsProps dd ?
OPTIONDATA ENDS

;--------------------------------[ mapitags.h ]-----------------------------------

PR_ACKNOWLEDGEMENT_MODE equ (PT_LONG shl 16)+ 1
PR_ALTERNATE_RECIPIENT_ALLOWED equ ( PT_BOOLEAN shl 16)+   2
PR_AUTHORIZING_USERS equ ( PT_BINARY shl 16)+    3
PR_AUTO_FORWARD_COMMENT equ ( PT_TSTRING shl 16)+   4
PR_AUTO_FORWARD_COMMENT_W equ ( PT_UNICODE shl 16)+   4
PR_AUTO_FORWARD_COMMENT_A equ ( PT_STRING8 shl 16)+   4
PR_AUTO_FORWARDED equ ( PT_BOOLEAN shl 16)+   5
PR_CONTENT_CONFIDENTIALITY_ALGORITHM_ID equ ( PT_BINARY shl 16)+    6
PR_CONTENT_CORRELATOR equ ( PT_BINARY shl 16)+    7
PR_CONTENT_IDENTIFIER equ ( PT_TSTRING shl 16)+   8
PR_CONTENT_IDENTIFIER_W equ ( PT_UNICODE shl 16)+   8
PR_CONTENT_IDENTIFIER_A equ ( PT_STRING8 shl 16)+   8
PR_CONTENT_LENGTH equ ( PT_LONG shl 16)+      9
PR_CONTENT_RETURN_REQUESTED equ ( PT_BOOLEAN shl 16)+   0Ah
PR_CONVERSATION_KEY equ ( PT_BINARY shl 16)+    0Bh
PR_CONVERSION_EITS equ ( PT_BINARY shl 16)+    0Ch
PR_CONVERSION_WITH_LOSS_PROHIBITED equ ( PT_BOOLEAN shl 16)+   0Dh
PR_CONVERTED_EITS equ ( PT_BINARY shl 16)+    0Eh
PR_DEFERRED_DELIVERY_TIME equ ( PT_SYSTIME shl 16)+   0Fh
PR_DELIVER_TIME equ ( PT_SYSTIME shl 16)+   10h
PR_DISCARD_REASON equ ( PT_LONG shl 16)+      11h
PR_DISCLOSURE_OF_RECIPIENTS equ ( PT_BOOLEAN shl 16)+   12h
PR_DL_EXPANSION_HISTORY equ ( PT_BINARY shl 16)+    13h
PR_DL_EXPANSION_PROHIBITED equ ( PT_BOOLEAN shl 16)+   14h
PR_EXPIRY_TIME equ ( PT_SYSTIME shl 16)+   15h
PR_IMPLICIT_CONVERSION_PROHIBITED equ ( PT_BOOLEAN shl 16)+   16h
PR_IMPORTANCE equ ( PT_LONG shl 16)+      17h
PR_IPM_ID equ ( PT_BINARY shl 16)+    18h
PR_LATEST_DELIVERY_TIME equ ( PT_SYSTIME shl 16)+   19h
PR_MESSAGE_CLASS equ ( PT_TSTRING shl 16)+   1Ah
PR_MESSAGE_CLASS_W equ ( PT_UNICODE shl 16)+   1Ah
PR_MESSAGE_CLASS_A equ ( PT_STRING8 shl 16)+   1Ah
PR_MESSAGE_DELIVERY_ID equ ( PT_BINARY shl 16)+    1Bh
PR_MESSAGE_SECURITY_LABEL equ ( PT_BINARY shl 16)+    1Eh
PR_OBSOLETED_IPMS equ ( PT_BINARY shl 16)+    1Fh
PR_ORIGINALLY_INTENDED_RECIPIENT_NAME equ ( PT_BINARY shl 16)+    20h
PR_ORIGINAL_EITS equ ( PT_BINARY shl 16)+    21h
PR_ORIGINATOR_CERTIFICATE equ ( PT_BINARY shl 16)+    22h
PR_ORIGINATOR_DELIVERY_REPORT_REQUESTED equ ( PT_BOOLEAN shl 16)+   23h
PR_ORIGINATOR_RETURN_ADDRESS equ ( PT_BINARY shl 16)+    24h
PR_PARENT_KEY equ ( PT_BINARY shl 16)+    25h
PR_PRIORITY equ ( PT_LONG shl 16)+      26h
PR_ORIGIN_CHECK equ ( PT_BINARY shl 16)+    27h
PR_PROOF_OF_SUBMISSION_REQUESTED equ ( PT_BOOLEAN shl 16)+   28h
PR_READ_RECEIPT_REQUESTED equ ( PT_BOOLEAN shl 16)+   29h
PR_RECEIPT_TIME equ ( PT_SYSTIME shl 16)+   2Ah
PR_RECIPIENT_REASSIGNMENT_PROHIBITED equ ( PT_BOOLEAN shl 16)+   2Bh
PR_REDIRECTION_HISTORY equ ( PT_BINARY shl 16)+    2Ch
PR_RELATED_IPMS equ ( PT_BINARY shl 16)+    2Dh
PR_ORIGINAL_SENSITIVITY equ ( PT_LONG shl 16)+      2Eh
PR_LANGUAGES equ ( PT_TSTRING shl 16)+   2Fh
PR_LANGUAGES_W equ ( PT_UNICODE shl 16)+   2Fh
PR_LANGUAGES_A equ ( PT_STRING8 shl 16)+   2Fh
PR_REPLY_TIME equ ( PT_SYSTIME shl 16)+   30h
PR_REPORT_TAG equ ( PT_BINARY shl 16)+    31h
PR_REPORT_TIME equ ( PT_SYSTIME shl 16)+   32h
PR_RETURNED_IPM equ ( PT_BOOLEAN shl 16)+   33h
PR_SECURITY equ ( PT_LONG shl 16)+      34h
PR_INCOMPLETE_COPY equ ( PT_BOOLEAN shl 16)+   35h
PR_SENSITIVITY equ ( PT_LONG shl 16)+      36h
PR_SUBJECT equ ( PT_TSTRING shl 16)+   37h
PR_SUBJECT_W equ ( PT_UNICODE shl 16)+   37h
PR_SUBJECT_A equ ( PT_STRING8 shl 16)+   37h
PR_SUBJECT_IPM equ ( PT_BINARY shl 16)+    38h
PR_CLIENT_SUBMIT_TIME equ ( PT_SYSTIME shl 16)+   39h
PR_REPORT_NAME equ ( PT_TSTRING shl 16)+   3Ah
PR_REPORT_NAME_W equ ( PT_UNICODE shl 16)+   3Ah
PR_REPORT_NAME_A equ ( PT_STRING8 shl 16)+   3Ah
PR_SENT_REPRESENTING_SEARCH_KEY equ ( PT_BINARY shl 16)+    3Bh
PR_X400_CONTENT_TYPE equ ( PT_BINARY shl 16)+    3Ch
PR_SUBJECT_PREFIX equ ( PT_TSTRING shl 16)+   3Dh
PR_SUBJECT_PREFIX_W equ ( PT_UNICODE shl 16)+   3Dh
PR_SUBJECT_PREFIX_A equ ( PT_STRING8 shl 16)+   3Dh
PR_NON_RECEIPT_REASON equ ( PT_LONG shl 16)+      3Eh
PR_RECEIVED_BY_ENTRYID equ ( PT_BINARY shl 16)+    3Fh
PR_RECEIVED_BY_NAME equ ( PT_TSTRING shl 16)+   40h
PR_RECEIVED_BY_NAME_W equ ( PT_UNICODE shl 16)+   40h
PR_RECEIVED_BY_NAME_A equ ( PT_STRING8 shl 16)+   40h
PR_SENT_REPRESENTING_ENTRYID equ ( PT_BINARY shl 16)+    41h
PR_SENT_REPRESENTING_NAME equ ( PT_TSTRING shl 16)+   42h
PR_SENT_REPRESENTING_NAME_W equ ( PT_UNICODE shl 16)+   42h
PR_SENT_REPRESENTING_NAME_A equ ( PT_STRING8 shl 16)+   42h
PR_RCVD_REPRESENTING_ENTRYID equ ( PT_BINARY shl 16)+    43h
PR_RCVD_REPRESENTING_NAME equ ( PT_TSTRING shl 16)+   44h
PR_RCVD_REPRESENTING_NAME_W equ ( PT_UNICODE shl 16)+   44h
PR_RCVD_REPRESENTING_NAME_A equ ( PT_STRING8 shl 16)+   44h
PR_REPORT_ENTRYID equ ( PT_BINARY shl 16)+    45h
PR_READ_RECEIPT_ENTRYID equ ( PT_BINARY shl 16)+    46h
PR_MESSAGE_SUBMISSION_ID equ ( PT_BINARY shl 16)+    47h
PR_PROVIDER_SUBMIT_TIME equ ( PT_SYSTIME shl 16)+   48h
PR_ORIGINAL_SUBJECT equ ( PT_TSTRING shl 16)+   49h
PR_ORIGINAL_SUBJECT_W equ ( PT_UNICODE shl 16)+   49h
PR_ORIGINAL_SUBJECT_A equ ( PT_STRING8 shl 16)+   49h
PR_DISC_VAL equ ( PT_BOOLEAN shl 16)+   4Ah
PR_ORIG_MESSAGE_CLASS equ ( PT_TSTRING shl 16)+   4Bh
PR_ORIG_MESSAGE_CLASS_W equ ( PT_UNICODE shl 16)+   4Bh
PR_ORIG_MESSAGE_CLASS_A equ ( PT_STRING8 shl 16)+   4Bh
PR_ORIGINAL_AUTHOR_ENTRYID equ ( PT_BINARY shl 16)+    4Ch
PR_ORIGINAL_AUTHOR_NAME equ ( PT_TSTRING shl 16)+   4Dh
PR_ORIGINAL_AUTHOR_NAME_W equ ( PT_UNICODE shl 16)+   4Dh
PR_ORIGINAL_AUTHOR_NAME_A equ ( PT_STRING8 shl 16)+   4Dh
PR_ORIGINAL_SUBMIT_TIME equ ( PT_SYSTIME shl 16)+   4Eh
PR_REPLY_RECIPIENT_ENTRIES equ ( PT_BINARY shl 16)+    4Fh
PR_REPLY_RECIPIENT_NAMES equ ( PT_TSTRING shl 16)+   50h
PR_REPLY_RECIPIENT_NAMES_W equ ( PT_UNICODE shl 16)+   50h
PR_REPLY_RECIPIENT_NAMES_A equ ( PT_STRING8 shl 16)+   50h
PR_RECEIVED_BY_SEARCH_KEY equ ( PT_BINARY shl 16)+    51h
PR_RCVD_REPRESENTING_SEARCH_KEY equ ( PT_BINARY shl 16)+    52h
PR_READ_RECEIPT_SEARCH_KEY equ ( PT_BINARY shl 16)+    53h
PR_REPORT_SEARCH_KEY equ ( PT_BINARY shl 16)+    54h
PR_ORIGINAL_DELIVERY_TIME equ ( PT_SYSTIME shl 16)+   55h
PR_ORIGINAL_AUTHOR_SEARCH_KEY equ ( PT_BINARY shl 16)+    56h
PR_MESSAGE_TO_ME equ ( PT_BOOLEAN shl 16)+   57h
PR_MESSAGE_CC_ME equ ( PT_BOOLEAN shl 16)+   58h
PR_MESSAGE_RECIP_ME equ ( PT_BOOLEAN shl 16)+   59h
PR_ORIGINAL_SENDER_NAME equ ( PT_TSTRING shl 16)+   5Ah
PR_ORIGINAL_SENDER_NAME_W equ ( PT_UNICODE shl 16)+   5Ah
PR_ORIGINAL_SENDER_NAME_A equ ( PT_STRING8 shl 16)+   5Ah
PR_ORIGINAL_SENDER_ENTRYID equ ( PT_BINARY shl 16)+    5Bh
PR_ORIGINAL_SENDER_SEARCH_KEY equ ( PT_BINARY shl 16)+    5Ch
PR_ORIGINAL_SENT_REPRESENTING_NAME equ ( PT_TSTRING shl 16)+   5Dh
PR_ORIGINAL_SENT_REPRESENTING_NAME_W equ ( PT_UNICODE shl 16)+   5Dh
PR_ORIGINAL_SENT_REPRESENTING_NAME_A equ ( PT_STRING8 shl 16)+   5Dh
PR_ORIGINAL_SENT_REPRESENTING_ENTRYID equ ( PT_BINARY shl 16)+    5Eh
PR_ORIGINAL_SENT_REPRESENTING_SEARCH_KEY equ ( PT_BINARY shl 16)+    5Fh
PR_START_DATE equ ( PT_SYSTIME shl 16)+   60h
PR_END_DATE equ ( PT_SYSTIME shl 16)+   61h
PR_OWNER_APPT_ID equ ( PT_LONG shl 16)+      62h
PR_RESPONSE_REQUESTED equ ( PT_BOOLEAN shl 16)+   63h
PR_SENT_REPRESENTING_ADDRTYPE equ ( PT_TSTRING shl 16)+   64h
PR_SENT_REPRESENTING_ADDRTYPE_W equ ( PT_UNICODE shl 16)+   64h
PR_SENT_REPRESENTING_ADDRTYPE_A equ ( PT_STRING8 shl 16)+   64h
PR_SENT_REPRESENTING_EMAIL_ADDRESS equ ( PT_TSTRING shl 16)+   65h
PR_SENT_REPRESENTING_EMAIL_ADDRESS_W equ ( PT_UNICODE shl 16)+   65h
PR_SENT_REPRESENTING_EMAIL_ADDRESS_A equ ( PT_STRING8 shl 16)+   65h
PR_ORIGINAL_SENDER_ADDRTYPE equ ( PT_TSTRING shl 16)+   66h
PR_ORIGINAL_SENDER_ADDRTYPE_W equ ( PT_UNICODE shl 16)+   66h
PR_ORIGINAL_SENDER_ADDRTYPE_A equ ( PT_STRING8 shl 16)+   66h
PR_ORIGINAL_SENDER_EMAIL_ADDRESS equ ( PT_TSTRING shl 16)+   67h
PR_ORIGINAL_SENDER_EMAIL_ADDRESS_W equ ( PT_UNICODE shl 16)+   67h
PR_ORIGINAL_SENDER_EMAIL_ADDRESS_A equ ( PT_STRING8 shl 16)+   67h
PR_ORIGINAL_SENT_REPRESENTING_ADDRTYPE equ ( PT_TSTRING shl 16)+   68h
PR_ORIGINAL_SENT_REPRESENTING_ADDRTYPE_W equ ( PT_UNICODE shl 16)+   68h
PR_ORIGINAL_SENT_REPRESENTING_ADDRTYPE_A equ ( PT_STRING8 shl 16)+   68h
PR_ORIGINAL_SENT_REPRESENTING_EMAIL_ADDRESS equ ( PT_TSTRING shl 16)+   69h
PR_ORIGINAL_SENT_REPRESENTING_EMAIL_ADDRESS_W equ ( PT_UNICODE shl 16)+   69h
PR_ORIGINAL_SENT_REPRESENTING_EMAIL_ADDRESS_A equ ( PT_STRING8 shl 16)+   69h
PR_CONVERSATION_TOPIC equ ( PT_TSTRING shl 16)+   70h
PR_CONVERSATION_TOPIC_W equ ( PT_UNICODE shl 16)+   70h
PR_CONVERSATION_TOPIC_A equ ( PT_STRING8 shl 16)+   70h
PR_CONVERSATION_INDEX equ ( PT_BINARY shl 16)+    71h
PR_ORIGINAL_DISPLAY_BCC equ ( PT_TSTRING shl 16)+   72h
PR_ORIGINAL_DISPLAY_BCC_W equ ( PT_UNICODE shl 16)+   72h
PR_ORIGINAL_DISPLAY_BCC_A equ ( PT_STRING8 shl 16)+   72h
PR_ORIGINAL_DISPLAY_CC equ ( PT_TSTRING shl 16)+   73h
PR_ORIGINAL_DISPLAY_CC_W equ ( PT_UNICODE shl 16)+   73h
PR_ORIGINAL_DISPLAY_CC_A equ ( PT_STRING8 shl 16)+   73h
PR_ORIGINAL_DISPLAY_TO equ ( PT_TSTRING shl 16)+   74h
PR_ORIGINAL_DISPLAY_TO_W equ ( PT_UNICODE shl 16)+   74h
PR_ORIGINAL_DISPLAY_TO_A equ ( PT_STRING8 shl 16)+   74h
PR_RECEIVED_BY_ADDRTYPE equ ( PT_TSTRING shl 16)+   75h
PR_RECEIVED_BY_ADDRTYPE_W equ ( PT_UNICODE shl 16)+   75h
PR_RECEIVED_BY_ADDRTYPE_A equ ( PT_STRING8 shl 16)+   75h
PR_RECEIVED_BY_EMAIL_ADDRESS equ ( PT_TSTRING shl 16)+   76h
PR_RECEIVED_BY_EMAIL_ADDRESS_W equ ( PT_UNICODE shl 16)+   76h
PR_RECEIVED_BY_EMAIL_ADDRESS_A equ ( PT_STRING8 shl 16)+   76h
PR_RCVD_REPRESENTING_ADDRTYPE equ ( PT_TSTRING shl 16)+   77h
PR_RCVD_REPRESENTING_ADDRTYPE_W equ ( PT_UNICODE shl 16)+   77h
PR_RCVD_REPRESENTING_ADDRTYPE_A equ ( PT_STRING8 shl 16)+   77h
PR_RCVD_REPRESENTING_EMAIL_ADDRESS equ ( PT_TSTRING shl 16)+   78h
PR_RCVD_REPRESENTING_EMAIL_ADDRESS_W equ ( PT_UNICODE shl 16)+   78h
PR_RCVD_REPRESENTING_EMAIL_ADDRESS_A equ ( PT_STRING8 shl 16)+   78h
PR_ORIGINAL_AUTHOR_ADDRTYPE equ ( PT_TSTRING shl 16)+   79h
PR_ORIGINAL_AUTHOR_ADDRTYPE_W equ ( PT_UNICODE shl 16)+   79h
PR_ORIGINAL_AUTHOR_ADDRTYPE_A equ ( PT_STRING8 shl 16)+   79h
PR_ORIGINAL_AUTHOR_EMAIL_ADDRESS equ ( PT_TSTRING shl 16)+   7Ah
PR_ORIGINAL_AUTHOR_EMAIL_ADDRESS_W equ ( PT_UNICODE shl 16)+   7Ah
PR_ORIGINAL_AUTHOR_EMAIL_ADDRESS_A equ ( PT_STRING8 shl 16)+   7Ah
PR_ORIGINALLY_INTENDED_RECIP_ADDRTYPE equ ( PT_TSTRING shl 16)+   7Bh
PR_ORIGINALLY_INTENDED_RECIP_ADDRTYPE_W equ ( PT_UNICODE shl 16)+   7Bh
PR_ORIGINALLY_INTENDED_RECIP_ADDRTYPE_A equ ( PT_STRING8 shl 16)+   7Bh
PR_ORIGINALLY_INTENDED_RECIP_EMAIL_ADDRESS equ ( PT_TSTRING shl 16)+   7Ch
PR_ORIGINALLY_INTENDED_RECIP_EMAIL_ADDRESS_W equ ( PT_UNICODE shl 16)+   7Ch
PR_ORIGINALLY_INTENDED_RECIP_EMAIL_ADDRESS_A equ ( PT_STRING8 shl 16)+   7Ch
PR_TRANSPORT_MESSAGE_HEADERS equ (PT_TSTRING shl 16)+    7Dh
PR_TRANSPORT_MESSAGE_HEADERS_W equ (PT_UNICODE shl 16)+    7Dh
PR_TRANSPORT_MESSAGE_HEADERS_A equ (PT_STRING8 shl 16)+    7Dh
PR_DELEGATION equ (PT_BINARY shl 16)+     7Eh
PR_TNEF_CORRELATION_KEY equ (PT_BINARY shl 16)+     7Fh
PR_BODY equ ( PT_TSTRING shl 16)+   1000h
PR_BODY_W equ ( PT_UNICODE shl 16)+   1000h
PR_BODY_A equ ( PT_STRING8 shl 16)+   1000h
PR_REPORT_TEXT equ ( PT_TSTRING shl 16)+   1001h
PR_REPORT_TEXT_W equ ( PT_UNICODE shl 16)+   1001h
PR_REPORT_TEXT_A equ ( PT_STRING8 shl 16)+   1001h
PR_ORIGINATOR_AND_DL_EXPANSION_HISTORY equ ( PT_BINARY shl 16)+    1002h
PR_REPORTING_DL_NAME equ ( PT_BINARY shl 16)+    1003h
PR_REPORTING_MTA_CERTIFICATE equ ( PT_BINARY shl 16)+    1004h
PR_RTF_SYNC_BODY_CRC equ ( PT_LONG shl 16)+      1006h
PR_RTF_SYNC_BODY_COUNT equ ( PT_LONG shl 16)+      1007h
PR_RTF_SYNC_BODY_TAG equ ( PT_TSTRING shl 16)+   1008h
PR_RTF_SYNC_BODY_TAG_W equ ( PT_UNICODE shl 16)+   1008h
PR_RTF_SYNC_BODY_TAG_A equ ( PT_STRING8 shl 16)+   1008h
PR_RTF_COMPRESSED equ ( PT_BINARY shl 16)+    1009h
PR_RTF_SYNC_PREFIX_COUNT equ ( PT_LONG shl 16)+      1010h
PR_RTF_SYNC_TRAILING_COUNT equ ( PT_LONG shl 16)+      1011h
PR_ORIGINALLY_INTENDED_RECIP_ENTRYID equ ( PT_BINARY shl 16)+    1012h
PR_CONTENT_INTEGRITY_CHECK equ ( PT_BINARY shl 16)+    0C00h
PR_EXPLICIT_CONVERSION equ ( PT_LONG shl 16)+      0C01h
PR_IPM_RETURN_REQUESTED equ ( PT_BOOLEAN shl 16)+   0C02h
PR_MESSAGE_TOKEN equ ( PT_BINARY shl 16)+    0C03h
PR_NDR_REASON_CODE equ ( PT_LONG shl 16)+      0C04h
PR_NDR_DIAG_CODE equ ( PT_LONG shl 16)+      0C05h
PR_NON_RECEIPT_NOTIFICATION_REQUESTED equ ( PT_BOOLEAN shl 16)+   0C06h
PR_DELIVERY_POINT equ ( PT_LONG shl 16)+      0C07h
PR_ORIGINATOR_NON_DELIVERY_REPORT_REQUESTED equ ( PT_BOOLEAN shl 16)+   0C08h
PR_ORIGINATOR_REQUESTED_ALTERNATE_RECIPIENT equ ( PT_BINARY shl 16)+    0C09h
PR_PHYSICAL_DELIVERY_BUREAU_FAX_DELIVERY equ ( PT_BOOLEAN shl 16)+   0C0Ah
PR_PHYSICAL_DELIVERY_MODE equ ( PT_LONG shl 16)+      0C0Bh
PR_PHYSICAL_DELIVERY_REPORT_REQUEST equ ( PT_LONG shl 16)+      0C0Ch
PR_PHYSICAL_FORWARDING_ADDRESS equ ( PT_BINARY shl 16)+    0C0Dh
PR_PHYSICAL_FORWARDING_ADDRESS_REQUESTED equ ( PT_BOOLEAN shl 16)+   0C0Eh
PR_PHYSICAL_FORWARDING_PROHIBITED equ ( PT_BOOLEAN shl 16)+   0C0Fh
PR_PHYSICAL_RENDITION_ATTRIBUTES equ ( PT_BINARY shl 16)+    0C10h
PR_PROOF_OF_DELIVERY equ ( PT_BINARY shl 16)+    0C11h
PR_PROOF_OF_DELIVERY_REQUESTED equ ( PT_BOOLEAN shl 16)+   0C12h
PR_RECIPIENT_CERTIFICATE equ ( PT_BINARY shl 16)+    0C13h
PR_RECIPIENT_NUMBER_FOR_ADVICE equ ( PT_TSTRING shl 16)+   0C14h
PR_RECIPIENT_NUMBER_FOR_ADVICE_W equ ( PT_UNICODE shl 16)+   0C14h
PR_RECIPIENT_NUMBER_FOR_ADVICE_A equ ( PT_STRING8 shl 16)+   0C14h
PR_RECIPIENT_TYPE equ ( PT_LONG shl 16)+      0C15h
PR_REGISTERED_MAIL_TYPE equ ( PT_LONG shl 16)+      0C16h
PR_REPLY_REQUESTED equ ( PT_BOOLEAN shl 16)+   0C17h
PR_REQUESTED_DELIVERY_METHOD equ ( PT_LONG shl 16)+      0C18h
PR_SENDER_ENTRYID equ ( PT_BINARY shl 16)+    0C19h
PR_SENDER_NAME equ ( PT_TSTRING shl 16)+   0C1Ah
PR_SENDER_NAME_W equ ( PT_UNICODE shl 16)+   0C1Ah
PR_SENDER_NAME_A equ ( PT_STRING8 shl 16)+   0C1Ah
PR_SUPPLEMENTARY_INFO equ ( PT_TSTRING shl 16)+   0C1Bh
PR_SUPPLEMENTARY_INFO_W equ ( PT_UNICODE shl 16)+   0C1Bh
PR_SUPPLEMENTARY_INFO_A equ ( PT_STRING8 shl 16)+   0C1Bh
PR_TYPE_OF_MTS_USER equ ( PT_LONG shl 16)+      0C1Ch
PR_SENDER_SEARCH_KEY equ ( PT_BINARY shl 16)+    0C1Dh
PR_SENDER_ADDRTYPE equ ( PT_TSTRING shl 16)+   0C1Eh
PR_SENDER_ADDRTYPE_W equ ( PT_UNICODE shl 16)+   0C1Eh
PR_SENDER_ADDRTYPE_A equ ( PT_STRING8 shl 16)+   0C1Eh
PR_SENDER_EMAIL_ADDRESS equ ( PT_TSTRING shl 16)+   0C1Fh
PR_SENDER_EMAIL_ADDRESS_W equ ( PT_UNICODE shl 16)+   0C1Fh
PR_SENDER_EMAIL_ADDRESS_A equ ( PT_STRING8 shl 16)+   0C1Fh
PR_CURRENT_VERSION equ ( PT_I8 shl 16)+        0E00h
PR_DELETE_AFTER_SUBMIT equ ( PT_BOOLEAN shl 16)+   0E01h
PR_DISPLAY_BCC equ ( PT_TSTRING shl 16)+   0E02h
PR_DISPLAY_BCC_W equ ( PT_UNICODE shl 16)+   0E02h
PR_DISPLAY_BCC_A equ ( PT_STRING8 shl 16)+   0E02h
PR_DISPLAY_CC equ ( PT_TSTRING shl 16)+   0E03h
PR_DISPLAY_CC_W equ ( PT_UNICODE shl 16)+   0E03h
PR_DISPLAY_CC_A equ ( PT_STRING8 shl 16)+   0E03h
PR_DISPLAY_TO equ ( PT_TSTRING shl 16)+   0E04h
PR_DISPLAY_TO_W equ ( PT_UNICODE shl 16)+   0E04h
PR_DISPLAY_TO_A equ ( PT_STRING8 shl 16)+   0E04h
PR_PARENT_DISPLAY equ ( PT_TSTRING shl 16)+   0E05h
PR_PARENT_DISPLAY_W equ ( PT_UNICODE shl 16)+   0E05h
PR_PARENT_DISPLAY_A equ ( PT_STRING8 shl 16)+   0E05h
PR_MESSAGE_DELIVERY_TIME equ ( PT_SYSTIME shl 16)+   0E06h
PR_MESSAGE_FLAGS equ ( PT_LONG shl 16)+      0E07h
PR_MESSAGE_SIZE equ ( PT_LONG shl 16)+      0E08h
PR_PARENT_ENTRYID equ ( PT_BINARY shl 16)+    0E09h
PR_SENTMAIL_ENTRYID equ ( PT_BINARY shl 16)+    0E0Ah
PR_CORRELATE equ ( PT_BOOLEAN shl 16)+   0E0Ch
PR_CORRELATE_MTSID equ ( PT_BINARY shl 16)+    0E0Dh
PR_DISCRETE_VALUES equ ( PT_BOOLEAN shl 16)+   0E0Eh
PR_RESPONSIBILITY equ ( PT_BOOLEAN shl 16)+   0E0Fh
PR_SPOOLER_STATUS equ ( PT_LONG shl 16)+      0E10h
PR_TRANSPORT_STATUS equ ( PT_LONG shl 16)+      0E11h
PR_MESSAGE_RECIPIENTS equ ( PT_OBJECT shl 16)+    0E12h
PR_MESSAGE_ATTACHMENTS equ ( PT_OBJECT shl 16)+    0E13h
PR_SUBMIT_FLAGS equ ( PT_LONG shl 16)+      0E14h
PR_RECIPIENT_STATUS equ ( PT_LONG shl 16)+      0E15h
PR_TRANSPORT_KEY equ ( PT_LONG shl 16)+      0E16h
PR_MSG_STATUS equ ( PT_LONG shl 16)+      0E17h
PR_MESSAGE_DOWNLOAD_TIME equ ( PT_LONG shl 16)+      0E18h
PR_CREATION_VERSION equ ( PT_I8 shl 16)+        0E19h
PR_MODIFY_VERSION equ ( PT_I8 shl 16)+        0E1Ah
PR_HASATTACH equ ( PT_BOOLEAN shl 16)+   0E1Bh
PR_BODY_CRC equ ( PT_LONG shl 16)+      0E1Ch
PR_NORMALIZED_SUBJECT equ ( PT_TSTRING shl 16)+   0E1Dh
PR_NORMALIZED_SUBJECT_W equ ( PT_UNICODE shl 16)+   0E1Dh
PR_NORMALIZED_SUBJECT_A equ ( PT_STRING8 shl 16)+   0E1Dh
PR_RTF_IN_SYNC equ ( PT_BOOLEAN shl 16)+   0E1Fh
PR_ATTACH_SIZE equ ( PT_LONG shl 16)+      0E20h
PR_ATTACH_NUM equ ( PT_LONG shl 16)+      0E21h
PR_PREPROCESS equ ( PT_BOOLEAN shl 16)+   0E22h
PR_ORIGINATING_MTA_CERTIFICATE equ ( PT_BINARY shl 16)+    0E25h
PR_PROOF_OF_SUBMISSION equ ( PT_BINARY shl 16)+    0E26h
PR_ENTRYID equ ( PT_BINARY shl 16)+    0FFFh
PR_OBJECT_TYPE equ ( PT_LONG shl 16)+      0FFEh
PR_ICON equ ( PT_BINARY shl 16)+    0FFDh
PR_MINI_ICON equ ( PT_BINARY shl 16)+    0FFCh
PR_STORE_ENTRYID equ ( PT_BINARY shl 16)+    0FFBh
PR_STORE_RECORD_KEY equ ( PT_BINARY shl 16)+    0FFAh
PR_RECORD_KEY equ ( PT_BINARY shl 16)+    0FF9h
PR_MAPPING_SIGNATURE equ ( PT_BINARY shl 16)+    0FF8h
PR_ACCESS_LEVEL equ ( PT_LONG shl 16)+      0FF7h
PR_INSTANCE_KEY equ ( PT_BINARY shl 16)+    0FF6h
PR_ROW_TYPE equ ( PT_LONG shl 16)+      0FF5h
PR_ACCESS equ ( PT_LONG shl 16)+      0FF4h
PR_ROWID equ ( PT_LONG shl 16)+      3000h
PR_DISPLAY_NAME equ ( PT_TSTRING shl 16)+   3001h
PR_DISPLAY_NAME_W equ ( PT_UNICODE shl 16)+   3001h
PR_DISPLAY_NAME_A equ ( PT_STRING8 shl 16)+   3001h
PR_ADDRTYPE equ ( PT_TSTRING shl 16)+   3002h
PR_ADDRTYPE_W equ ( PT_UNICODE shl 16)+   3002h
PR_ADDRTYPE_A equ ( PT_STRING8 shl 16)+   3002h
PR_EMAIL_ADDRESS equ ( PT_TSTRING shl 16)+   3003h
PR_EMAIL_ADDRESS_W equ ( PT_UNICODE shl 16)+   3003h
PR_EMAIL_ADDRESS_A equ ( PT_STRING8 shl 16)+   3003h
PR_COMMENT equ ( PT_TSTRING shl 16)+   3004h
PR_COMMENT_W equ ( PT_UNICODE shl 16)+   3004h
PR_COMMENT_A equ ( PT_STRING8 shl 16)+   3004h
PR_DEPTH equ ( PT_LONG shl 16)+      3005h
PR_PROVIDER_DISPLAY equ ( PT_TSTRING shl 16)+   3006h
PR_PROVIDER_DISPLAY_W equ ( PT_UNICODE shl 16)+   3006h
PR_PROVIDER_DISPLAY_A equ ( PT_STRING8 shl 16)+   3006h
PR_CREATION_TIME equ ( PT_SYSTIME shl 16)+   3007h
PR_LAST_MODIFICATION_TIME equ ( PT_SYSTIME shl 16)+   3008h
PR_RESOURCE_FLAGS equ ( PT_LONG shl 16)+      3009h
PR_PROVIDER_DLL_NAME equ ( PT_TSTRING shl 16)+   300Ah
PR_PROVIDER_DLL_NAME_W equ ( PT_UNICODE shl 16)+   300Ah
PR_PROVIDER_DLL_NAME_A equ ( PT_STRING8 shl 16)+   300Ah
PR_SEARCH_KEY equ ( PT_BINARY shl 16)+    300Bh
PR_PROVIDER_UID equ ( PT_BINARY shl 16)+    300Ch
PR_PROVIDER_ORDINAL equ ( PT_LONG shl 16)+      300Dh
PR_FORM_VERSION equ (PT_TSTRING shl 16)+    3301h
PR_FORM_VERSION_W equ (PT_UNICODE shl 16)+    3301h
PR_FORM_VERSION_A equ (PT_STRING8 shl 16)+    3301h
PR_FORM_CLSID equ (PT_CLSID shl 16)+      3302h
PR_FORM_CONTACT_NAME equ (PT_TSTRING shl 16)+    3303h
PR_FORM_CONTACT_NAME_W equ (PT_UNICODE shl 16)+    3303h
PR_FORM_CONTACT_NAME_A equ (PT_STRING8 shl 16)+    3303h
PR_FORM_CATEGORY equ (PT_TSTRING shl 16)+    3304h
PR_FORM_CATEGORY_W equ (PT_UNICODE shl 16)+    3304h
PR_FORM_CATEGORY_A equ (PT_STRING8 shl 16)+    3304h
PR_FORM_CATEGORY_SUB equ (PT_TSTRING shl 16)+    3305h
PR_FORM_CATEGORY_SUB_W equ (PT_UNICODE shl 16)+    3305h
PR_FORM_CATEGORY_SUB_A equ (PT_STRING8 shl 16)+    3305h
PR_FORM_HOST_MAP equ (PT_MV_LONG shl 16)+    3306h
PR_FORM_HIDDEN equ (PT_BOOLEAN shl 16)+    3307h
PR_FORM_DESIGNER_NAME equ (PT_TSTRING shl 16)+    3308h
PR_FORM_DESIGNER_NAME_W equ (PT_UNICODE shl 16)+    3308h
PR_FORM_DESIGNER_NAME_A equ (PT_STRING8 shl 16)+    3308h
PR_FORM_DESIGNER_GUID equ (PT_CLSID shl 16)+      3309h
PR_FORM_MESSAGE_BEHAVIOR equ (PT_LONG shl 16)+       330Ah
PR_DEFAULT_STORE equ ( PT_BOOLEAN shl 16)+   3400h
PR_STORE_SUPPORT_MASK equ ( PT_LONG shl 16)+      340Dh
PR_STORE_STATE equ ( PT_LONG shl 16)+      340Eh
PR_IPM_SUBTREE_SEARCH_KEY equ ( PT_BINARY shl 16)+    3410h
PR_IPM_OUTBOX_SEARCH_KEY equ ( PT_BINARY shl 16)+    3411h
PR_IPM_WASTEBASKET_SEARCH_KEY equ ( PT_BINARY shl 16)+    3412h
PR_IPM_SENTMAIL_SEARCH_KEY equ ( PT_BINARY shl 16)+    3413h
PR_MDB_PROVIDER equ ( PT_BINARY shl 16)+    3414h
PR_RECEIVE_FOLDER_SETTINGS equ ( PT_OBJECT shl 16)+    3415h
PR_VALID_FOLDER_MASK equ ( PT_LONG shl 16)+      35DFh
PR_IPM_SUBTREE_ENTRYID equ ( PT_BINARY shl 16)+    35E0h
PR_IPM_OUTBOX_ENTRYID equ ( PT_BINARY shl 16)+    35E2h
PR_IPM_WASTEBASKET_ENTRYID equ ( PT_BINARY shl 16)+    35E3h
PR_IPM_SENTMAIL_ENTRYID equ ( PT_BINARY shl 16)+    35E4h
PR_VIEWS_ENTRYID equ ( PT_BINARY shl 16)+    35E5h
PR_COMMON_VIEWS_ENTRYID equ ( PT_BINARY shl 16)+    35E6h
PR_FINDER_ENTRYID equ ( PT_BINARY shl 16)+    35E7h
PR_CONTAINER_FLAGS equ ( PT_LONG shl 16)+      3600h
PR_FOLDER_TYPE equ ( PT_LONG shl 16)+      3601h
PR_CONTENT_COUNT equ ( PT_LONG shl 16)+      3602h
PR_CONTENT_UNREAD equ ( PT_LONG shl 16)+      3603h
PR_CREATE_TEMPLATES equ ( PT_OBJECT shl 16)+    3604h
PR_DETAILS_TABLE equ ( PT_OBJECT shl 16)+    3605h
PR_SEARCH equ ( PT_OBJECT shl 16)+    3607h
PR_SELECTABLE equ ( PT_BOOLEAN shl 16)+   3609h
PR_SUBFOLDERS equ ( PT_BOOLEAN shl 16)+   360Ah
PR_STATUS equ ( PT_LONG shl 16)+      360Bh
PR_ANR equ ( PT_TSTRING shl 16)+   360Ch
PR_ANR_W equ ( PT_UNICODE shl 16)+   360Ch
PR_ANR_A equ ( PT_STRING8 shl 16)+   360Ch
PR_CONTENTS_SORT_ORDER equ ( PT_MV_LONG shl 16)+   360Dh
PR_CONTAINER_HIERARCHY equ ( PT_OBJECT shl 16)+    360Eh
PR_CONTAINER_CONTENTS equ ( PT_OBJECT shl 16)+    360Fh
PR_FOLDER_ASSOCIATED_CONTENTS equ ( PT_OBJECT shl 16)+    3610h
PR_DEF_CREATE_DL equ ( PT_BINARY shl 16)+    3611h
PR_DEF_CREATE_MAILUSER equ ( PT_BINARY shl 16)+    3612h
PR_CONTAINER_CLASS equ ( PT_TSTRING shl 16)+   3613h
PR_CONTAINER_CLASS_W equ ( PT_UNICODE shl 16)+   3613h
PR_CONTAINER_CLASS_A equ ( PT_STRING8 shl 16)+   3613h
PR_CONTAINER_MODIFY_VERSION equ ( PT_I8 shl 16)+        3614h
PR_AB_PROVIDER_ID equ ( PT_BINARY shl 16)+    3615h
PR_DEFAULT_VIEW_ENTRYID equ ( PT_BINARY shl 16)+    3616h
PR_ASSOC_CONTENT_COUNT equ ( PT_LONG shl 16)+      3617h
PR_ATTACHMENT_X400_PARAMETERS equ ( PT_BINARY shl 16)+    3700h
PR_ATTACH_DATA_OBJ equ ( PT_OBJECT shl 16)+    3701h
PR_ATTACH_DATA_BIN equ ( PT_BINARY shl 16)+    3701h
PR_ATTACH_ENCODING equ ( PT_BINARY shl 16)+    3702h
PR_ATTACH_EXTENSION equ ( PT_TSTRING shl 16)+   3703h
PR_ATTACH_EXTENSION_W equ ( PT_UNICODE shl 16)+   3703h
PR_ATTACH_EXTENSION_A equ ( PT_STRING8 shl 16)+   3703h
PR_ATTACH_FILENAME equ ( PT_TSTRING shl 16)+   3704h
PR_ATTACH_FILENAME_W equ ( PT_UNICODE shl 16)+   3704h
PR_ATTACH_FILENAME_A equ ( PT_STRING8 shl 16)+   3704h
PR_ATTACH_METHOD equ ( PT_LONG shl 16)+      3705h
PR_ATTACH_LONG_FILENAME equ ( PT_TSTRING shl 16)+   3707h
PR_ATTACH_LONG_FILENAME_W equ ( PT_UNICODE shl 16)+   3707h
PR_ATTACH_LONG_FILENAME_A equ ( PT_STRING8 shl 16)+   3707h
PR_ATTACH_PATHNAME equ ( PT_TSTRING shl 16)+   3708h
PR_ATTACH_PATHNAME_W equ ( PT_UNICODE shl 16)+   3708h
PR_ATTACH_PATHNAME_A equ ( PT_STRING8 shl 16)+   3708h
PR_ATTACH_RENDERING equ ( PT_BINARY shl 16)+    3709h
PR_ATTACH_TAG equ ( PT_BINARY shl 16)+    370Ah
PR_RENDERING_POSITION equ ( PT_LONG shl 16)+      370Bh
PR_ATTACH_TRANSPORT_NAME equ ( PT_TSTRING shl 16)+   370Ch
PR_ATTACH_TRANSPORT_NAME_W equ ( PT_UNICODE shl 16)+   370Ch
PR_ATTACH_TRANSPORT_NAME_A equ ( PT_STRING8 shl 16)+   370Ch
PR_ATTACH_LONG_PATHNAME equ ( PT_TSTRING shl 16)+   370Dh
PR_ATTACH_LONG_PATHNAME_W equ ( PT_UNICODE shl 16)+   370Dh
PR_ATTACH_LONG_PATHNAME_A equ ( PT_STRING8 shl 16)+   370Dh
PR_ATTACH_MIME_TAG equ ( PT_TSTRING shl 16)+   370Eh
PR_ATTACH_MIME_TAG_W equ ( PT_UNICODE shl 16)+   370Eh
PR_ATTACH_MIME_TAG_A equ ( PT_STRING8 shl 16)+   370Eh
PR_ATTACH_ADDITIONAL_INFO equ ( PT_BINARY shl 16)+    370Fh
PR_DISPLAY_TYPE equ ( PT_LONG shl 16)+      3900h
PR_TEMPLATEID equ ( PT_BINARY shl 16)+    3902h
PR_PRIMARY_CAPABILITY equ ( PT_BINARY shl 16)+    3904h
PR_7BIT_DISPLAY_NAME equ ( PT_STRING8 shl 16)+   39FFh
PR_ACCOUNT equ ( PT_TSTRING shl 16)+   3A00h
PR_ACCOUNT_W equ ( PT_UNICODE shl 16)+   3A00h
PR_ACCOUNT_A equ ( PT_STRING8 shl 16)+   3A00h
PR_ALTERNATE_RECIPIENT equ ( PT_BINARY shl 16)+    3A01h
PR_CALLBACK_TELEPHONE_NUMBER equ ( PT_TSTRING shl 16)+   3A02h
PR_CALLBACK_TELEPHONE_NUMBER_W equ ( PT_UNICODE shl 16)+   3A02h
PR_CALLBACK_TELEPHONE_NUMBER_A equ ( PT_STRING8 shl 16)+   3A02h
PR_CONVERSION_PROHIBITED equ ( PT_BOOLEAN shl 16)+   3A03h
PR_DISCLOSE_RECIPIENTS equ ( PT_BOOLEAN shl 16)+   3A04h
PR_GENERATION equ ( PT_TSTRING shl 16)+   3A05h
PR_GENERATION_W equ ( PT_UNICODE shl 16)+   3A05h
PR_GENERATION_A equ ( PT_STRING8 shl 16)+   3A05h
PR_GIVEN_NAME equ ( PT_TSTRING shl 16)+   3A06h
PR_GIVEN_NAME_W equ ( PT_UNICODE shl 16)+   3A06h
PR_GIVEN_NAME_A equ ( PT_STRING8 shl 16)+   3A06h
PR_GOVERNMENT_ID_NUMBER equ ( PT_TSTRING shl 16)+   3A07h
PR_GOVERNMENT_ID_NUMBER_W equ ( PT_UNICODE shl 16)+   3A07h
PR_GOVERNMENT_ID_NUMBER_A equ ( PT_STRING8 shl 16)+   3A07h
PR_BUSINESS_TELEPHONE_NUMBER equ ( PT_TSTRING shl 16)+   3A08h
PR_BUSINESS_TELEPHONE_NUMBER_W equ ( PT_UNICODE shl 16)+   3A08h
PR_BUSINESS_TELEPHONE_NUMBER_A equ ( PT_STRING8 shl 16)+   3A08h
PR_OFFICE_TELEPHONE_NUMBER equ PR_BUSINESS_TELEPHONE_NUMBER
PR_OFFICE_TELEPHONE_NUMBER_W equ PR_BUSINESS_TELEPHONE_NUMBER_W
PR_OFFICE_TELEPHONE_NUMBER_A equ PR_BUSINESS_TELEPHONE_NUMBER_A
PR_HOME_TELEPHONE_NUMBER equ ( PT_TSTRING shl 16)+   3A09h
PR_HOME_TELEPHONE_NUMBER_W equ ( PT_UNICODE shl 16)+   3A09h
PR_HOME_TELEPHONE_NUMBER_A equ ( PT_STRING8 shl 16)+   3A09h
PR_INITIALS equ ( PT_TSTRING shl 16)+   3A0Ah
PR_INITIALS_W equ ( PT_UNICODE shl 16)+   3A0Ah
PR_INITIALS_A equ ( PT_STRING8 shl 16)+   3A0Ah
PR_KEYWORD equ ( PT_TSTRING shl 16)+   3A0Bh
PR_KEYWORD_W equ ( PT_UNICODE shl 16)+   3A0Bh
PR_KEYWORD_A equ ( PT_STRING8 shl 16)+   3A0Bh
PR_LANGUAGE equ ( PT_TSTRING shl 16)+   3A0Ch
PR_LANGUAGE_W equ ( PT_UNICODE shl 16)+   3A0Ch
PR_LANGUAGE_A equ ( PT_STRING8 shl 16)+   3A0Ch
PR_LOCATION equ ( PT_TSTRING shl 16)+   3A0Dh
PR_LOCATION_W equ ( PT_UNICODE shl 16)+   3A0Dh
PR_LOCATION_A equ ( PT_STRING8 shl 16)+   3A0Dh
PR_MAIL_PERMISSION equ ( PT_BOOLEAN shl 16)+   3A0Eh
PR_MHS_COMMON_NAME equ ( PT_TSTRING shl 16)+   3A0Fh
PR_MHS_COMMON_NAME_W equ ( PT_UNICODE shl 16)+   3A0Fh
PR_MHS_COMMON_NAME_A equ ( PT_STRING8 shl 16)+   3A0Fh
PR_ORGANIZATIONAL_ID_NUMBER equ ( PT_TSTRING shl 16)+   3A10h
PR_ORGANIZATIONAL_ID_NUMBER_W equ ( PT_UNICODE shl 16)+   3A10h
PR_ORGANIZATIONAL_ID_NUMBER_A equ ( PT_STRING8 shl 16)+   3A10h
PR_SURNAME equ ( PT_TSTRING shl 16)+   3A11h
PR_SURNAME_W equ ( PT_UNICODE shl 16)+   3A11h
PR_SURNAME_A equ ( PT_STRING8 shl 16)+   3A11h
PR_ORIGINAL_ENTRYID equ ( PT_BINARY shl 16)+    3A12h
PR_ORIGINAL_DISPLAY_NAME equ ( PT_TSTRING shl 16)+   3A13h
PR_ORIGINAL_DISPLAY_NAME_W equ ( PT_UNICODE shl 16)+   3A13h
PR_ORIGINAL_DISPLAY_NAME_A equ ( PT_STRING8 shl 16)+   3A13h
PR_ORIGINAL_SEARCH_KEY equ ( PT_BINARY shl 16)+    3A14h
PR_POSTAL_ADDRESS equ ( PT_TSTRING shl 16)+   3A15h
PR_POSTAL_ADDRESS_W equ ( PT_UNICODE shl 16)+   3A15h
PR_POSTAL_ADDRESS_A equ ( PT_STRING8 shl 16)+   3A15h
PR_COMPANY_NAME equ ( PT_TSTRING shl 16)+   3A16h
PR_COMPANY_NAME_W equ ( PT_UNICODE shl 16)+   3A16h
PR_COMPANY_NAME_A equ ( PT_STRING8 shl 16)+   3A16h
PR_TITLE equ ( PT_TSTRING shl 16)+   3A17h
PR_TITLE_W equ ( PT_UNICODE shl 16)+   3A17h
PR_TITLE_A equ ( PT_STRING8 shl 16)+   3A17h
PR_DEPARTMENT_NAME equ ( PT_TSTRING shl 16)+   3A18h
PR_DEPARTMENT_NAME_W equ ( PT_UNICODE shl 16)+   3A18h
PR_DEPARTMENT_NAME_A equ ( PT_STRING8 shl 16)+   3A18h
PR_OFFICE_LOCATION equ ( PT_TSTRING shl 16)+   3A19h
PR_OFFICE_LOCATION_W equ ( PT_UNICODE shl 16)+   3A19h
PR_OFFICE_LOCATION_A equ ( PT_STRING8 shl 16)+   3A19h
PR_PRIMARY_TELEPHONE_NUMBER equ ( PT_TSTRING shl 16)+   3A1Ah
PR_PRIMARY_TELEPHONE_NUMBER_W equ ( PT_UNICODE shl 16)+   3A1Ah
PR_PRIMARY_TELEPHONE_NUMBER_A equ ( PT_STRING8 shl 16)+   3A1Ah
PR_BUSINESS2_TELEPHONE_NUMBER equ ( PT_TSTRING shl 16)+   3A1Bh
PR_BUSINESS2_TELEPHONE_NUMBER_W equ ( PT_UNICODE shl 16)+   3A1Bh
PR_BUSINESS2_TELEPHONE_NUMBER_A equ ( PT_STRING8 shl 16)+   3A1Bh
PR_OFFICE2_TELEPHONE_NUMBER equ PR_BUSINESS2_TELEPHONE_NUMBER
PR_OFFICE2_TELEPHONE_NUMBER_W equ PR_BUSINESS2_TELEPHONE_NUMBER_W
PR_OFFICE2_TELEPHONE_NUMBER_A equ PR_BUSINESS2_TELEPHONE_NUMBER_A
PR_MOBILE_TELEPHONE_NUMBER equ ( PT_TSTRING shl 16)+   3A1Ch
PR_MOBILE_TELEPHONE_NUMBER_W equ ( PT_UNICODE shl 16)+   3A1Ch
PR_MOBILE_TELEPHONE_NUMBER_A equ ( PT_STRING8 shl 16)+   3A1Ch
PR_CELLULAR_TELEPHONE_NUMBER equ PR_MOBILE_TELEPHONE_NUMBER
PR_CELLULAR_TELEPHONE_NUMBER_W equ PR_MOBILE_TELEPHONE_NUMBER_W
PR_CELLULAR_TELEPHONE_NUMBER_A equ PR_MOBILE_TELEPHONE_NUMBER_A
PR_RADIO_TELEPHONE_NUMBER equ ( PT_TSTRING shl 16)+   3A1Dh
PR_RADIO_TELEPHONE_NUMBER_W equ ( PT_UNICODE shl 16)+   3A1Dh
PR_RADIO_TELEPHONE_NUMBER_A equ ( PT_STRING8 shl 16)+   3A1Dh
PR_CAR_TELEPHONE_NUMBER equ ( PT_TSTRING shl 16)+   3A1Eh
PR_CAR_TELEPHONE_NUMBER_W equ ( PT_UNICODE shl 16)+   3A1Eh
PR_CAR_TELEPHONE_NUMBER_A equ ( PT_STRING8 shl 16)+   3A1Eh
PR_OTHER_TELEPHONE_NUMBER equ ( PT_TSTRING shl 16)+   3A1Fh
PR_OTHER_TELEPHONE_NUMBER_W equ ( PT_UNICODE shl 16)+   3A1Fh
PR_OTHER_TELEPHONE_NUMBER_A equ ( PT_STRING8 shl 16)+   3A1Fh
PR_TRANSMITABLE_DISPLAY_NAME equ ( PT_TSTRING shl 16)+   3A20h
PR_TRANSMITABLE_DISPLAY_NAME_W equ ( PT_UNICODE shl 16)+   3A20h
PR_TRANSMITABLE_DISPLAY_NAME_A equ ( PT_STRING8 shl 16)+   3A20h
PR_PAGER_TELEPHONE_NUMBER equ ( PT_TSTRING shl 16)+   3A21h
PR_PAGER_TELEPHONE_NUMBER_W equ ( PT_UNICODE shl 16)+   3A21h
PR_PAGER_TELEPHONE_NUMBER_A equ ( PT_STRING8 shl 16)+   3A21h
PR_BEEPER_TELEPHONE_NUMBER equ PR_PAGER_TELEPHONE_NUMBER
PR_BEEPER_TELEPHONE_NUMBER_W equ PR_PAGER_TELEPHONE_NUMBER_W
PR_BEEPER_TELEPHONE_NUMBER_A equ PR_PAGER_TELEPHONE_NUMBER_A
PR_USER_CERTIFICATE equ ( PT_BINARY shl 16)+    3A22h
PR_PRIMARY_FAX_NUMBER equ ( PT_TSTRING shl 16)+   3A23h
PR_PRIMARY_FAX_NUMBER_W equ ( PT_UNICODE shl 16)+   3A23h
PR_PRIMARY_FAX_NUMBER_A equ ( PT_STRING8 shl 16)+   3A23h
PR_BUSINESS_FAX_NUMBER equ ( PT_TSTRING shl 16)+   3A24h
PR_BUSINESS_FAX_NUMBER_W equ ( PT_UNICODE shl 16)+   3A24h
PR_BUSINESS_FAX_NUMBER_A equ ( PT_STRING8 shl 16)+   3A24h
PR_HOME_FAX_NUMBER equ ( PT_TSTRING shl 16)+   3A25h
PR_HOME_FAX_NUMBER_W equ ( PT_UNICODE shl 16)+   3A25h
PR_HOME_FAX_NUMBER_A equ ( PT_STRING8 shl 16)+   3A25h
PR_COUNTRY equ ( PT_TSTRING shl 16)+   3A26h
PR_COUNTRY_W equ ( PT_UNICODE shl 16)+   3A26h
PR_COUNTRY_A equ ( PT_STRING8 shl 16)+   3A26h
PR_BUSINESS_ADDRESS_COUNTRY equ PR_COUNTRY
PR_BUSINESS_ADDRESS_COUNTRY_W equ PR_COUNTRY_W
PR_BUSINESS_ADDRESS_COUNTRY_A equ PR_COUNTRY_A
PR_LOCALITY equ ( PT_TSTRING shl 16)+   3A27h
PR_LOCALITY_W equ ( PT_UNICODE shl 16)+   3A27h
PR_LOCALITY_A equ ( PT_STRING8 shl 16)+   3A27h
PR_BUSINESS_ADDRESS_CITY equ PR_LOCALITY
PR_BUSINESS_ADDRESS_CITY_W equ PR_LOCALITY_W
PR_BUSINESS_ADDRESS_CITY_A equ PR_LOCALITY_A
PR_STATE_OR_PROVINCE equ ( PT_TSTRING shl 16)+   3A28h
PR_STATE_OR_PROVINCE_W equ ( PT_UNICODE shl 16)+   3A28h
PR_STATE_OR_PROVINCE_A equ ( PT_STRING8 shl 16)+   3A28h
PR_BUSINESS_ADDRESS_STATE_OR_PROVINCE equ PR_STATE_OR_PROVINCE
PR_BUSINESS_ADDRESS_STATE_OR_PROVINCE_W equ PR_STATE_OR_PROVINCE_W
PR_BUSINESS_ADDRESS_STATE_OR_PROVINCE_A equ PR_STATE_OR_PROVINCE_A
PR_STREET_ADDRESS equ ( PT_TSTRING shl 16)+   3A29h
PR_STREET_ADDRESS_W equ ( PT_UNICODE shl 16)+   3A29h
PR_STREET_ADDRESS_A equ ( PT_STRING8 shl 16)+   3A29h
PR_BUSINESS_ADDRESS_STREET equ PR_STREET_ADDRESS
PR_BUSINESS_ADDRESS_STREET_W equ PR_STREET_ADDRESS_W
PR_BUSINESS_ADDRESS_STREET_A equ PR_STREET_ADDRESS_A
PR_POSTAL_CODE equ ( PT_TSTRING shl 16)+   3A2Ah
PR_POSTAL_CODE_W equ ( PT_UNICODE shl 16)+   3A2Ah
PR_POSTAL_CODE_A equ ( PT_STRING8 shl 16)+   3A2Ah
PR_BUSINESS_ADDRESS_POSTAL_CODE equ PR_POSTAL_CODE
PR_BUSINESS_ADDRESS_POSTAL_CODE_W equ PR_POSTAL_CODE_W
PR_BUSINESS_ADDRESS_POSTAL_CODE_A equ PR_POSTAL_CODE_A
PR_POST_OFFICE_BOX equ ( PT_TSTRING shl 16)+   3A2Bh
PR_POST_OFFICE_BOX_W equ ( PT_UNICODE shl 16)+   3A2Bh
PR_POST_OFFICE_BOX_A equ ( PT_STRING8 shl 16)+   3A2Bh
PR_BUSINESS_ADDRESS_POST_OFFICE_BOX equ PR_POST_OFFICE_BOX
PR_BUSINESS_ADDRESS_POST_OFFICE_BOX_W equ PR_POST_OFFICE_BOX_W
PR_BUSINESS_ADDRESS_POST_OFFICE_BOX_A equ PR_POST_OFFICE_BOX_A
PR_TELEX_NUMBER equ ( PT_TSTRING shl 16)+   3A2Ch
PR_TELEX_NUMBER_W equ ( PT_UNICODE shl 16)+   3A2Ch
PR_TELEX_NUMBER_A equ ( PT_STRING8 shl 16)+   3A2Ch
PR_ISDN_NUMBER equ ( PT_TSTRING shl 16)+   3A2Dh
PR_ISDN_NUMBER_W equ ( PT_UNICODE shl 16)+   3A2Dh
PR_ISDN_NUMBER_A equ ( PT_STRING8 shl 16)+   3A2Dh
PR_ASSISTANT_TELEPHONE_NUMBER equ ( PT_TSTRING shl 16)+   3A2Eh
PR_ASSISTANT_TELEPHONE_NUMBER_W equ ( PT_UNICODE shl 16)+   3A2Eh
PR_ASSISTANT_TELEPHONE_NUMBER_A equ ( PT_STRING8 shl 16)+   3A2Eh
PR_HOME2_TELEPHONE_NUMBER equ ( PT_TSTRING shl 16)+   3A2Fh
PR_HOME2_TELEPHONE_NUMBER_W equ ( PT_UNICODE shl 16)+   3A2Fh
PR_HOME2_TELEPHONE_NUMBER_A equ ( PT_STRING8 shl 16)+   3A2Fh
PR_ASSISTANT equ ( PT_TSTRING shl 16)+   3A30h
PR_ASSISTANT_W equ ( PT_UNICODE shl 16)+   3A30h
PR_ASSISTANT_A equ ( PT_STRING8 shl 16)+   3A30h
PR_SEND_RICH_INFO equ ( PT_BOOLEAN shl 16)+   3A40h
PR_WEDDING_ANNIVERSARY equ ( PT_SYSTIME shl 16)+ 3A41h
PR_BIRTHDAY equ ( PT_SYSTIME shl 16)+ 3A42h
PR_HOBBIES equ ( PT_TSTRING shl 16)+ 3A43h
PR_HOBBIES_W equ ( PT_UNICODE shl 16)+ 3A43h
PR_HOBBIES_A equ ( PT_STRING8 shl 16)+ 3A43h
PR_MIDDLE_NAME equ ( PT_TSTRING shl 16)+ 3A44h
PR_MIDDLE_NAME_W equ ( PT_UNICODE shl 16)+ 3A44h
PR_MIDDLE_NAME_A equ ( PT_STRING8 shl 16)+ 3A44h
PR_DISPLAY_NAME_PREFIX equ ( PT_TSTRING shl 16)+ 3A45h
PR_DISPLAY_NAME_PREFIX_W equ ( PT_UNICODE shl 16)+ 3A45h
PR_DISPLAY_NAME_PREFIX_A equ ( PT_STRING8 shl 16)+ 3A45h
PR_PROFESSION equ ( PT_TSTRING shl 16)+ 3A46h
PR_PROFESSION_W equ ( PT_UNICODE shl 16)+ 3A46h
PR_PROFESSION_A equ ( PT_STRING8 shl 16)+ 3A46h
PR_PREFERRED_BY_NAME equ ( PT_TSTRING shl 16)+ 3A47h
PR_PREFERRED_BY_NAME_W equ ( PT_UNICODE shl 16)+ 3A47h
PR_PREFERRED_BY_NAME_A equ ( PT_STRING8 shl 16)+ 3A47h
PR_SPOUSE_NAME equ ( PT_TSTRING shl 16)+ 3A48h
PR_SPOUSE_NAME_W equ ( PT_UNICODE shl 16)+ 3A48h
PR_SPOUSE_NAME_A equ ( PT_STRING8 shl 16)+ 3A48h
PR_COMPUTER_NETWORK_NAME equ ( PT_TSTRING shl 16)+ 3A49h
PR_COMPUTER_NETWORK_NAME_W equ ( PT_UNICODE shl 16)+ 3A49h
PR_COMPUTER_NETWORK_NAME_A equ ( PT_STRING8 shl 16)+ 3A49h
PR_CUSTOMER_ID equ ( PT_TSTRING shl 16)+ 3A4Ah
PR_CUSTOMER_ID_W equ ( PT_UNICODE shl 16)+ 3A4Ah
PR_CUSTOMER_ID_A equ ( PT_STRING8 shl 16)+ 3A4Ah
PR_TTYTDD_PHONE_NUMBER equ ( PT_TSTRING shl 16)+ 3A4Bh
PR_TTYTDD_PHONE_NUMBER_W equ ( PT_UNICODE shl 16)+ 3A4Bh
PR_TTYTDD_PHONE_NUMBER_A equ ( PT_STRING8 shl 16)+ 3A4Bh
PR_FTP_SITE equ ( PT_TSTRING shl 16)+ 3A4Ch
PR_FTP_SITE_W equ ( PT_UNICODE shl 16)+ 3A4Ch
PR_FTP_SITE_A equ ( PT_STRING8 shl 16)+ 3A4Ch
PR_GENDER equ ( PT_SHORT shl 16)+ 3A4Dh
PR_MANAGER_NAME equ ( PT_TSTRING shl 16)+ 3A4Eh
PR_MANAGER_NAME_W equ ( PT_UNICODE shl 16)+ 3A4Eh
PR_MANAGER_NAME_A equ ( PT_STRING8 shl 16)+ 3A4Eh
PR_NICKNAME equ ( PT_TSTRING shl 16)+ 3A4Fh
PR_NICKNAME_W equ ( PT_UNICODE shl 16)+ 3A4Fh
PR_NICKNAME_A equ ( PT_STRING8 shl 16)+ 3A4Fh
PR_PERSONAL_HOME_PAGE equ ( PT_TSTRING shl 16)+ 3A50h
PR_PERSONAL_HOME_PAGE_W equ ( PT_UNICODE shl 16)+ 3A50h
PR_PERSONAL_HOME_PAGE_A equ ( PT_STRING8 shl 16)+ 3A50h
PR_BUSINESS_HOME_PAGE equ ( PT_TSTRING shl 16)+ 3A51h
PR_BUSINESS_HOME_PAGE_W equ ( PT_UNICODE shl 16)+ 3A51h
PR_BUSINESS_HOME_PAGE_A equ ( PT_STRING8 shl 16)+ 3A51h
PR_CONTACT_VERSION equ ( PT_CLSID shl 16)+ 3A52h
PR_CONTACT_ENTRYIDS equ ( PT_MV_BINARY shl 16)+ 3A53h
PR_CONTACT_ADDRTYPES equ ( PT_MV_TSTRING shl 16)+ 3A54h
PR_CONTACT_ADDRTYPES_W equ ( PT_MV_UNICODE shl 16)+ 3A54h
PR_CONTACT_ADDRTYPES_A equ ( PT_MV_STRING8 shl 16)+ 3A54h
PR_CONTACT_DEFAULT_ADDRESS_INDEX equ ( PT_LONG shl 16)+ 3A55h
PR_CONTACT_EMAIL_ADDRESSES equ ( PT_MV_TSTRING shl 16)+ 3A56h
PR_CONTACT_EMAIL_ADDRESSES_W equ ( PT_MV_UNICODE shl 16)+ 3A56h
PR_CONTACT_EMAIL_ADDRESSES_A equ ( PT_MV_STRING8 shl 16)+ 3A56h
PR_COMPANY_MAIN_PHONE_NUMBER equ ( PT_TSTRING shl 16)+ 3A57h
PR_COMPANY_MAIN_PHONE_NUMBER_W equ ( PT_UNICODE shl 16)+ 3A57h
PR_COMPANY_MAIN_PHONE_NUMBER_A equ ( PT_STRING8 shl 16)+ 3A57h
PR_CHILDRENS_NAMES equ ( PT_MV_TSTRING shl 16)+ 3A58h
PR_CHILDRENS_NAMES_W equ ( PT_MV_UNICODE shl 16)+ 3A58h
PR_CHILDRENS_NAMES_A equ ( PT_MV_STRING8 shl 16)+ 3A58h
PR_HOME_ADDRESS_CITY equ ( PT_TSTRING shl 16)+ 3A59h
PR_HOME_ADDRESS_CITY_W equ ( PT_UNICODE shl 16)+ 3A59h
PR_HOME_ADDRESS_CITY_A equ ( PT_STRING8 shl 16)+ 3A59h
PR_HOME_ADDRESS_COUNTRY equ ( PT_TSTRING shl 16)+ 3A5Ah
PR_HOME_ADDRESS_COUNTRY_W equ ( PT_UNICODE shl 16)+ 3A5Ah
PR_HOME_ADDRESS_COUNTRY_A equ ( PT_STRING8 shl 16)+ 3A5Ah
PR_HOME_ADDRESS_POSTAL_CODE equ ( PT_TSTRING shl 16)+ 3A5Bh
PR_HOME_ADDRESS_POSTAL_CODE_W equ ( PT_UNICODE shl 16)+ 3A5Bh
PR_HOME_ADDRESS_POSTAL_CODE_A equ ( PT_STRING8 shl 16)+ 3A5Bh
PR_HOME_ADDRESS_STATE_OR_PROVINCE equ ( PT_TSTRING shl 16)+ 3A5Ch
PR_HOME_ADDRESS_STATE_OR_PROVINCE_W equ ( PT_UNICODE shl 16)+ 3A5Ch
PR_HOME_ADDRESS_STATE_OR_PROVINCE_A equ ( PT_STRING8 shl 16)+ 3A5Ch
PR_HOME_ADDRESS_STREET equ ( PT_TSTRING shl 16)+ 3A5Dh
PR_HOME_ADDRESS_STREET_W equ ( PT_UNICODE shl 16)+ 3A5Dh
PR_HOME_ADDRESS_STREET_A equ ( PT_STRING8 shl 16)+ 3A5Dh
PR_HOME_ADDRESS_POST_OFFICE_BOX equ ( PT_TSTRING shl 16)+ 3A5Eh
PR_HOME_ADDRESS_POST_OFFICE_BOX_W equ ( PT_UNICODE shl 16)+ 3A5Eh
PR_HOME_ADDRESS_POST_OFFICE_BOX_A equ ( PT_STRING8 shl 16)+ 3A5Eh
PR_OTHER_ADDRESS_CITY equ ( PT_TSTRING shl 16)+ 3A5Fh
PR_OTHER_ADDRESS_CITY_W equ ( PT_UNICODE shl 16)+ 3A5Fh
PR_OTHER_ADDRESS_CITY_A equ ( PT_STRING8 shl 16)+ 3A5Fh
PR_OTHER_ADDRESS_COUNTRY equ ( PT_TSTRING shl 16)+ 3A60h
PR_OTHER_ADDRESS_COUNTRY_W equ ( PT_UNICODE shl 16)+ 3A60h
PR_OTHER_ADDRESS_COUNTRY_A equ ( PT_STRING8 shl 16)+ 3A60h
PR_OTHER_ADDRESS_POSTAL_CODE equ ( PT_TSTRING shl 16)+ 3A61h
PR_OTHER_ADDRESS_POSTAL_CODE_W equ ( PT_UNICODE shl 16)+ 3A61h
PR_OTHER_ADDRESS_POSTAL_CODE_A equ ( PT_STRING8 shl 16)+ 3A61h
PR_OTHER_ADDRESS_STATE_OR_PROVINCE equ ( PT_TSTRING shl 16)+ 3A62h
PR_OTHER_ADDRESS_STATE_OR_PROVINCE_W equ ( PT_UNICODE shl 16)+ 3A62h
PR_OTHER_ADDRESS_STATE_OR_PROVINCE_A equ ( PT_STRING8 shl 16)+ 3A62h
PR_OTHER_ADDRESS_STREET equ ( PT_TSTRING shl 16)+ 3A63h
PR_OTHER_ADDRESS_STREET_W equ ( PT_UNICODE shl 16)+ 3A63h
PR_OTHER_ADDRESS_STREET_A equ ( PT_STRING8 shl 16)+ 3A63h
PR_OTHER_ADDRESS_POST_OFFICE_BOX equ ( PT_TSTRING shl 16)+ 3A64h
PR_OTHER_ADDRESS_POST_OFFICE_BOX_W equ ( PT_UNICODE shl 16)+ 3A64h
PR_OTHER_ADDRESS_POST_OFFICE_BOX_A equ ( PT_STRING8 shl 16)+ 3A64h
PR_STORE_PROVIDERS equ ( PT_BINARY shl 16)+    3D00h
PR_AB_PROVIDERS equ ( PT_BINARY shl 16)+    3D01h
PR_TRANSPORT_PROVIDERS equ ( PT_BINARY shl 16)+    3D02h
PR_DEFAULT_PROFILE equ ( PT_BOOLEAN shl 16)+   3D04h
PR_AB_SEARCH_PATH equ ( PT_MV_BINARY shl 16)+ 3D05h
PR_AB_DEFAULT_DIR equ ( PT_BINARY shl 16)+    3D06h
PR_AB_DEFAULT_PAB equ ( PT_BINARY shl 16)+    3D07h
PR_FILTERING_HOOKS equ ( PT_BINARY shl 16)+    3D08h
PR_SERVICE_NAME equ ( PT_TSTRING shl 16)+   3D09h
PR_SERVICE_NAME_W equ ( PT_UNICODE shl 16)+   3D09h
PR_SERVICE_NAME_A equ ( PT_STRING8 shl 16)+   3D09h
PR_SERVICE_DLL_NAME equ ( PT_TSTRING shl 16)+   3D0Ah
PR_SERVICE_DLL_NAME_W equ ( PT_UNICODE shl 16)+   3D0Ah
PR_SERVICE_DLL_NAME_A equ ( PT_STRING8 shl 16)+   3D0Ah
PR_SERVICE_ENTRY_NAME equ ( PT_STRING8 shl 16)+   3D0Bh
PR_SERVICE_UID equ ( PT_BINARY shl 16)+    3D0Ch
PR_SERVICE_EXTRA_UIDS equ ( PT_BINARY shl 16)+    3D0Dh
PR_SERVICES equ ( PT_BINARY shl 16)+    3D0Eh
PR_SERVICE_SUPPORT_FILES equ ( PT_MV_TSTRING shl 16)+ 3D0Fh
PR_SERVICE_SUPPORT_FILES_W equ ( PT_MV_UNICODE shl 16)+ 3D0Fh
PR_SERVICE_SUPPORT_FILES_A equ ( PT_MV_STRING8 shl 16)+ 3D0Fh
PR_SERVICE_DELETE_FILES equ ( PT_MV_TSTRING shl 16)+ 3D10h
PR_SERVICE_DELETE_FILES_W equ ( PT_MV_UNICODE shl 16)+ 3D10h
PR_SERVICE_DELETE_FILES_A equ ( PT_MV_STRING8 shl 16)+ 3D10h
PR_AB_SEARCH_PATH_UPDATE equ ( PT_BINARY shl 16)+     3D11h
PR_PROFILE_NAME equ ( PT_TSTRING shl 16)+   3D12h
PR_PROFILE_NAME_A equ ( PT_STRING8 shl 16)+   3D12h
PR_PROFILE_NAME_W equ ( PT_UNICODE shl 16)+   3D12h
PR_IDENTITY_DISPLAY equ ( PT_TSTRING shl 16)+   3E00h
PR_IDENTITY_DISPLAY_W equ ( PT_UNICODE shl 16)+   3E00h
PR_IDENTITY_DISPLAY_A equ ( PT_STRING8 shl 16)+   3E00h
PR_IDENTITY_ENTRYID equ ( PT_BINARY shl 16)+    3E01h
PR_RESOURCE_METHODS equ ( PT_LONG shl 16)+      3E02h
PR_RESOURCE_TYPE equ ( PT_LONG shl 16)+      3E03h
PR_STATUS_CODE equ ( PT_LONG shl 16)+      3E04h
PR_IDENTITY_SEARCH_KEY equ ( PT_BINARY shl 16)+    3E05h
PR_OWN_STORE_ENTRYID equ ( PT_BINARY shl 16)+    3E06h
PR_RESOURCE_PATH equ ( PT_TSTRING shl 16)+   3E07h
PR_RESOURCE_PATH_W equ ( PT_UNICODE shl 16)+   3E07h
PR_RESOURCE_PATH_A equ ( PT_STRING8 shl 16)+   3E07h
PR_STATUS_STRING equ ( PT_TSTRING shl 16)+   3E08h
PR_STATUS_STRING_W equ ( PT_UNICODE shl 16)+   3E08h
PR_STATUS_STRING_A equ ( PT_STRING8 shl 16)+   3E08h
PR_X400_DEFERRED_DELIVERY_CANCEL equ ( PT_BOOLEAN shl 16)+   3E09h
PR_HEADER_FOLDER_ENTRYID equ ( PT_BINARY shl 16)+    3E0Ah
PR_REMOTE_PROGRESS equ ( PT_LONG shl 16)+      3E0Bh
PR_REMOTE_PROGRESS_TEXT equ ( PT_TSTRING shl 16)+   3E0Ch
PR_REMOTE_PROGRESS_TEXT_W equ ( PT_UNICODE shl 16)+   3E0Ch
PR_REMOTE_PROGRESS_TEXT_A equ ( PT_STRING8 shl 16)+   3E0Ch
PR_REMOTE_VALIDATE_OK equ ( PT_BOOLEAN shl 16)+   3E0Dh
PR_CONTROL_FLAGS equ ( PT_LONG shl 16)+      3F00h
PR_CONTROL_STRUCTURE equ ( PT_BINARY shl 16)+    3F01h
PR_CONTROL_TYPE equ ( PT_LONG shl 16)+      3F02h
PR_DELTAX equ ( PT_LONG shl 16)+      3F03h
PR_DELTAY equ ( PT_LONG shl 16)+      3F04h
PR_XPOS equ ( PT_LONG shl 16)+      3F05h
PR_YPOS equ ( PT_LONG shl 16)+      3F06h
PR_CONTROL_ID equ ( PT_BINARY shl 16)+    3F07h
PR_INITIAL_DETAILS_PANE equ ( PT_LONG shl 16)+      3F08h
PROP_ID_SECURE_MIN equ 67F0h
PROP_ID_SECURE_MAX equ 67FFh

;----------------------------------[ mapiutil.h ]------------------------
IPROP_READONLY equ 1
IPROP_READWRITE equ 2
IPROP_CLEAN equ 10000h
IPROP_DIRTY equ 20000h
PRILOWEST equ -32768
PRIHIGHEST equ 32767
PRIUSER equ 0
IRONULL equ 0
FIROWAIT equ 1
FIROINTERVAL equ 2
FIROPERBLOCK equ 4
FIRODISABLED equ 20h
FIROONCEONLY equ 40h
IRCNULL equ 0
FIRCPFN equ 1
FIRCPV equ 2
FIRCPRI equ 4
FIRCCSEC equ 8
FIRCIRO equ 10h
MAPI_FORCE_CREATE equ 1
MAPI_FULL_IPM_TREE equ 2
RTF_SYNC_RTF_CHANGED equ 1
RTF_SYNC_BODY_CHANGED equ 2

DTCTL STRUCT
    ulCtlType dd ?
    ulCtlFlags dd ?
    lpbNotif dd ?
    cbNotif dd ?
    lpszFilter dd ?
    ulItemID dd ?
    union ctl
        lpv dd ?
        lplabel dd ?
        lpedit dd ?
        lplbx dd ?
        lpcombobox dd ?
        lpddlbx dd ?
        lpcheckbox dd ?
        lpgroupbox dd ?
        lpbutton dd ?
        lpradiobutton dd ?
        lpmvlbx dd ?
        lpmvddlbx dd ?
        lppage dd ?
	  ends
DTCTL ENDS

DTPAGE STRUCT
    cctl dd ?
    lpszResourceName dd ?
    union
        lpszComponent dd ?
        ulItemID dd ?
    ends
    lpctl dd ?
DTPAGE ENDS

;-----------------------------------[mapiwz.h]-------------------------------
WIZ_QUERYNUMPAGES equ WM_USER+10
WIZ_NEXT equ WM_USER +11
WIZ_PREV equ WM_USER +12
MAPI_PW_FIRST_PROFILE equ 1
MAPI_PW_LAUNCHED_BY_CONFIG equ 2
MAPI_PW_ADD_SERVICE_ONLY equ 4
MAPI_PW_PROVIDER_UI_ONLY equ 8
MAPI_PW_HIDE_SERVICES_LIST equ 10h
PR_WIZARD_NO_PST_PAGE equ (PT_BOOLEAN shl 16)+6700h
LAUNCHWIZARDENTRYNAME equ "LAUNCHWIZARD"

;--------------------------------[ mapix.h]-----------------------------
MAPI_ALLOW_OTHERS equ 8
MAPI_EXPLICIT_PROFILE equ 10h
MAPI_SERVICE_UI_ALWAYS equ 2000h
MAPI_NO_MAIL equ 8000h
MAPI_TIMEOUT_SHORT equ 100000h
MAPI_SIMPLE_DEFAULT equ (MAPI_LOGON_UI + MAPI_FORCE_DOWNLOAD + MAPI_ALLOW_OTHERS)
MAPI_SIMPLE_EXPLICIT equ (MAPI_NEW_SESSION + MAPI_FORCE_DOWNLOAD + MAPI_EXPLICIT_PROFILE)
MAPI_INIT_VERSION equ 0
MAPI_MULTITHREAD_NOTIFICATIONS equ 1
MAPI_LOGOFF_SHARED equ 1
MAPI_LOGOFF_UI equ 2
MAPI_DEFAULT_STORE equ 1
MAPI_SIMPLE_STORE_TEMPORARY equ 2
MAPI_SIMPLE_STORE_PERMANENT equ 3
MAPI_PRIMARY_STORE equ 4
MAPI_SECONDARY_STORE equ 5
MAPI_POST_MESSAGE equ 1
MAPI_NEW_MESSAGE equ 2
MAPI_DEFAULT_SERVICES equ 1
SERVICE_DEFAULT_STORE equ 1
SERVICE_SINGLE_COPY equ 2
SERVICE_CREATE_WITH_STORE equ 4
SERVICE_PRIMARY_IDENTITY equ 8
SERVICE_NO_PRIMARY_IDENTITY equ 20h

MAPIINIT_0 STRUCT
    ulVersion dd    ?
    ulFlags dd      ?
MAPIINIT_0 ENDS

;;;; end mapi


;;;; winioctl

FILE_DEVICE_BEEP                 equ 00000001h
FILE_DEVICE_CD_ROM               equ 00000002h
FILE_DEVICE_CD_ROM_FILE_SYSTEM   equ 00000003h
FILE_DEVICE_CONTROLLER           equ 00000004h
FILE_DEVICE_DATALINK             equ 00000005h
FILE_DEVICE_DFS                  equ 00000006h
FILE_DEVICE_DISK                 equ 00000007h
FILE_DEVICE_DISK_FILE_SYSTEM     equ 00000008h
FILE_DEVICE_FILE_SYSTEM          equ 00000009h
FILE_DEVICE_INPORT_PORT          equ 0000000ah
FILE_DEVICE_KEYBOARD             equ 0000000bh
FILE_DEVICE_MAILSLOT             equ 0000000ch
FILE_DEVICE_MIDI_IN              equ 0000000dh
FILE_DEVICE_MIDI_OUT             equ 0000000eh
FILE_DEVICE_MOUSE                equ 0000000fh
FILE_DEVICE_MULTI_UNC_PROVIDER   equ 00000010h
FILE_DEVICE_NAMED_PIPE           equ 00000011h
FILE_DEVICE_NETWORK              equ 00000012h
FILE_DEVICE_NETWORK_BROWSER      equ 00000013h
FILE_DEVICE_NETWORK_FILE_SYSTEM  equ 00000014h
FILE_DEVICE_NULL                 equ 00000015h
FILE_DEVICE_PARALLEL_PORT        equ 00000016h
FILE_DEVICE_PHYSICAL_NETCARD     equ 00000017h
FILE_DEVICE_PRINTER              equ 00000018h
FILE_DEVICE_SCANNER              equ 00000019h
FILE_DEVICE_SERIAL_MOUSE_PORT    equ 0000001ah
FILE_DEVICE_SERIAL_PORT          equ 0000001bh
FILE_DEVICE_SCREEN               equ 0000001ch
FILE_DEVICE_SOUND                equ 0000001dh
FILE_DEVICE_STREAMS              equ 0000001eh
FILE_DEVICE_TAPE                 equ 0000001fh
FILE_DEVICE_TAPE_FILE_SYSTEM     equ 00000020h
FILE_DEVICE_TRANSPORT            equ 00000021h
FILE_DEVICE_UNKNOWN              equ 00000022h
FILE_DEVICE_VIDEO                equ 00000023h
FILE_DEVICE_VIRTUAL_DISK         equ 00000024h
FILE_DEVICE_WAVE_IN              equ 00000025h
FILE_DEVICE_WAVE_OUT             equ 00000026h
FILE_DEVICE_8042_PORT            equ 00000027h
FILE_DEVICE_NETWORK_REDIRECTOR   equ 00000028h
FILE_DEVICE_BATTERY              equ 00000029h
FILE_DEVICE_BUS_EXTENDER         equ 0000002ah
FILE_DEVICE_MODEM                equ 0000002bh
FILE_DEVICE_VDM                  equ 0000002ch
FILE_DEVICE_MASS_STORAGE         equ 0000002dh
FILE_DEVICE_SMB                  equ 0000002eh
FILE_DEVICE_KS                   equ 0000002fh
FILE_DEVICE_CHANGER              equ 00000030h
FILE_DEVICE_SMARTCARD            equ 00000031h
FILE_DEVICE_ACPI                 equ 00000032h
FILE_DEVICE_DVD                  equ 00000033h
FILE_DEVICE_FULLSCREEN_VIDEO     equ 00000034h
FILE_DEVICE_DFS_FILE_SYSTEM      equ 00000035h
FILE_DEVICE_DFS_VOLUME           equ 00000036h
FILE_DEVICE_SERENUM              equ 00000037h
FILE_DEVICE_TERMSRV              equ 00000038h
FILE_DEVICE_KSEC                 equ 00000039h
METHOD_BUFFERED                  equ 0
METHOD_IN_DIRECT                 equ 1
METHOD_OUT_DIRECT                equ 2
METHOD_NEITHER                   equ 3
FILE_ANY_ACCESS                  equ 0
FILE_SPECIAL_ACCESS              equ (FILE_ANY_ACCESS)
FILE_READ_ACCESS                 equ 1
FILE_WRITE_ACCESS                equ 2
IOCTL_STORAGE_BASE               equ FILE_DEVICE_MASS_STORAGE
RECOVERED_WRITES_VALID           equ 00000001h
UNRECOVERED_WRITES_VALID         equ 00000002h
RECOVERED_READS_VALID            equ 00000004h
UNRECOVERED_READS_VALID          equ 00000008h
WRITE_COMPRESSION_INFO_VALID     equ 00000010h
READ_COMPRESSION_INFO_VALID      equ 00000020h
TAPE_RETURN_STATISTICS           equ 0
TAPE_RETURN_ENV_INFO             equ 1
TAPE_RESET_STATISTICS            equ 2
MEDIA_ERASEABLE                  equ 00000001h
MEDIA_WRITE_ONCE                 equ 00000002h
MEDIA_READ_ONLY                  equ 00000004h
MEDIA_READ_WRITE                 equ 00000008h
MEDIA_WRITE_PROTECTED            equ 00000100h
MEDIA_CURRENTLY_MOUNTED          equ 80000000h
IOCTL_DISK_BASE                  equ FILE_DEVICE_DISK
PARTITION_ENTRY_UNUSED           equ 00h
PARTITION_FAT_12                 equ 01h
PARTITION_XENIX_1                equ 02h
PARTITION_XENIX_2                equ 03h
PARTITION_FAT_16                 equ 04h
PARTITION_EXTENDED               equ 05h
PARTITION_HUGE                   equ 06h
PARTITION_IFS                    equ 07h
PARTITION_FAT32                  equ 0Bh
PARTITION_FAT32_XINT13           equ 0Ch
PARTITION_XINT13                 equ 0Eh
PARTITION_XINT13_EXTENDED        equ 0Fh
PARTITION_PREP                   equ 41h
PARTITION_LDM                    equ 42h
PARTITION_UNIX                   equ 63h
VALID_NTFT                       equ 0C0h
PARTITION_NTFT                   equ 80h
HIST_NO_OF_BUCKETS               equ 24
HISTOGRAM_BUCKET_SIZE            equ sizeof(HISTOGRAM_BUCKET)
DISK_HISTOGRAM_SIZE              equ sizeof(DISK_HISTOGRAM)
DISK_LOGGING_START               equ 0
DISK_LOGGING_STOP                equ 1
DISK_LOGGING_DUMP                equ 2
DISK_BINNING                     equ 3
CAP_ATA_ID_CMD                   equ 1
CAP_ATAPI_ID_CMD                 equ 2
CAP_SMART_CMD                    equ 4
ATAPI_ID_CMD                     equ 0A1h
ID_CMD                           equ 0ECh
SMART_CMD                        equ 0B0h
SMART_CYL_LOW                    equ 4Fh
SMART_CYL_HI                     equ 0C2h
SMART_NO_ERROR                   equ 0
SMART_IDE_ERROR                  equ 1
SMART_INVALID_FLAG               equ 2
SMART_INVALID_COMMAND            equ 3
SMART_INVALID_BUFFER             equ 4
SMART_INVALID_DRIVE              equ 5
SMART_INVALID_IOCTL              equ 6
SMART_ERROR_NO_MEM               equ 7
SMART_INVALID_REGISTER           equ 8
SMART_NOT_SUPPORTED              equ 9
SMART_NO_IDE_DEVICE              equ 10
READ_ATTRIBUTE_BUFFER_SIZE       equ 512
IDENTIFY_BUFFER_SIZE             equ 512
READ_THRESHOLD_BUFFER_SIZE       equ 512
READ_ATTRIBUTES                  equ 0D0h
READ_THRESHOLDS                  equ 0D1h
ENABLE_DISABLE_AUTOSAVE          equ 0D2h
SAVE_ATTRIBUTE_VALUES            equ 0D3h
EXECUTE_OFFLINE_DIAGS            equ 0D4h
ENABLE_SMART                     equ 0D8h
DISABLE_SMART                    equ 0D9h
RETURN_SMART_STATUS              equ 0DAh
ENABLE_DISABLE_AUTO_OFFLINE      equ 0DBh
IOCTL_CHANGER_BASE               equ FILE_DEVICE_CHANGER
MAX_VOLUME_ID_SIZE               equ 36
MAX_VOLUME_TEMPLATE_SIZE         equ 40
VENDOR_ID_LENGTH                 equ 8
PRODUCT_ID_LENGTH                equ 16
REVISION_LENGTH                  equ 4
SERIAL_NUMBER_LENGTH             equ 32
CHANGER_BAR_CODE_SCANNER_INSTALLED equ 00000001h
CHANGER_INIT_ELEM_STAT_WITH_RANGE equ 00000002h
CHANGER_CLOSE_IEPORT             equ 00000004h
CHANGER_OPEN_IEPORT              equ 00000008h
CHANGER_STATUS_NON_VOLATILE      equ 00000010h
CHANGER_EXCHANGE_MEDIA           equ 00000020h
CHANGER_CLEANER_SLOT             equ 00000040h
CHANGER_LOCK_UNLOCK              equ 00000080h
CHANGER_CARTRIDGE_MAGAZINE       equ 00000100h
CHANGER_MEDIUM_FLIP              equ 00000200h
CHANGER_POSITION_TO_ELEMENT      equ 00000400h
CHANGER_REPORT_IEPORT_STATE      equ 00000800h
CHANGER_STORAGE_DRIVE            equ 00001000h
CHANGER_STORAGE_IEPORT           equ 00002000h
CHANGER_STORAGE_SLOT             equ 00004000h
CHANGER_STORAGE_TRANSPORT        equ 00008000h
CHANGER_DRIVE_CLEANING_REQUIRED  equ 00010000h
CHANGER_PREDISMOUNT_EJECT_REQUIRED equ 00020000h
CHANGER_CLEANER_ACCESS_NOT_VALID equ 00040000h
CHANGER_PREMOUNT_EJECT_REQUIRED  equ 00080000h
CHANGER_VOLUME_IDENTIFICATION    equ 00100000h
CHANGER_VOLUME_SEARCH            equ 00200000h
CHANGER_VOLUME_ASSERT            equ 00400000h
CHANGER_VOLUME_REPLACE           equ 00800000h
CHANGER_VOLUME_UNDEFINE          equ 01000000h
CHANGER_SERIAL_NUMBER_VALID      equ 04000000h
CHANGER_DEVICE_REINITIALIZE_CAPABLE equ 08000000h
CHANGER_KEYPAD_ENABLE_DISABLE    equ 10000000h
CHANGER_DRIVE_EMPTY_ON_DOOR_ACCESS equ 20000000h
CHANGER_RESERVED_BIT             equ 80000000h
CHANGER_PREDISMOUNT_ALIGN_TO_SLOT equ 80000001h
CHANGER_PREDISMOUNT_ALIGN_TO_DRIVE equ 80000002h
CHANGER_CLEANER_AUTODISMOUNT     equ 80000004h
CHANGER_TRUE_EXCHANGE_CAPABLE    equ 80000008h
CHANGER_SLOTS_USE_TRAYS          equ 80000010h
CHANGER_RTN_MEDIA_TO_ORIGINAL_ADDR equ 80000020h
CHANGER_CLEANER_OPS_NOT_SUPPORTED equ 80000040h
CHANGER_IEPORT_USER_CONTROL_OPEN equ 80000080h
CHANGER_IEPORT_USER_CONTROL_CLOSE equ 80000100h
CHANGER_MOVE_EXTENDS_IEPORT      equ 80000200h
CHANGER_MOVE_RETRACTS_IEPORT     equ 80000400h
CHANGER_TO_TRANSPORT             equ 01h
CHANGER_TO_SLOT                  equ 02h
CHANGER_TO_IEPORT                equ 04h
CHANGER_TO_DRIVE                 equ 08h
LOCK_UNLOCK_IEPORT               equ 01h
LOCK_UNLOCK_DOOR                 equ 02h
LOCK_UNLOCK_KEYPAD               equ 04h
LOCK_ELEMENT                     equ 0
UNLOCK_ELEMENT                   equ 1
EXTEND_IEPORT                    equ 2
RETRACT_IEPORT                   equ 3
ELEMENT_STATUS_FULL              equ 00000001h
ELEMENT_STATUS_IMPEXP            equ 00000002h
ELEMENT_STATUS_EXCEPT            equ 00000004h
ELEMENT_STATUS_ACCESS            equ 00000008h
ELEMENT_STATUS_EXENAB            equ 00000010h
ELEMENT_STATUS_INENAB            equ 00000020h
ELEMENT_STATUS_LUN_VALID         equ 00001000h
ELEMENT_STATUS_ID_VALID          equ 00002000h
ELEMENT_STATUS_NOT_BUS           equ 00008000h
ELEMENT_STATUS_INVERT            equ 00400000h
ELEMENT_STATUS_SVALID            equ 00800000h
ELEMENT_STATUS_PVOLTAG           equ 10000000h
ELEMENT_STATUS_AVOLTAG           equ 20000000h
ERROR_LABEL_UNREADABLE           equ 00000001h
ERROR_LABEL_QUESTIONABLE         equ 00000002h
ERROR_SLOT_NOT_PRESENT           equ 00000004h
ERROR_DRIVE_NOT_INSTALLED        equ 00000008h
ERROR_TRAY_MALFUNCTION           equ 00000010h
ERROR_INIT_STATUS_NEEDED         equ 00000011h
ERROR_UNHANDLED_ERROR            equ 0FFFFFFFFh
SEARCH_ALL                       equ 0h
SEARCH_PRIMARY                   equ 1h
SEARCH_ALTERNATE                 equ 2h
SEARCH_ALL_NO_SEQ                equ 4h
SEARCH_PRI_NO_SEQ                equ 5h
SEARCH_ALT_NO_SEQ                equ 6h
ASSERT_PRIMARY                   equ 8h
ASSERT_ALTERNATE                 equ 9h
REPLACE_PRIMARY                  equ 0Ah
REPLACE_ALTERNATE                equ 0Bh
UNDEFINE_PRIMARY                 equ 0Ch
UNDEFINE_ALTERNATE               equ 0Dh
SERIAL_LSRMST_ESCAPE             equ 0
SERIAL_LSRMST_LSR_DATA           equ 1
SERIAL_LSRMST_LSR_NODATA         equ 2
SERIAL_LSRMST_MST                equ 3
SERIAL_IOC_FCR_FIFO_ENABLE       equ 00000001
SERIAL_IOC_FCR_RCVR_RESET        equ 00000002
SERIAL_IOC_FCR_XMIT_RESET        equ 00000004
SERIAL_IOC_FCR_DMA_MODE          equ 00000008
SERIAL_IOC_FCR_RES1              equ 00000010
SERIAL_IOC_FCR_RES2              equ 00000020
SERIAL_IOC_FCR_RCVR_TRIGGER_LSB  equ 00000040
SERIAL_IOC_FCR_RCVR_TRIGGER_MSB  equ 00000080
SERIAL_IOC_MCR_DTR               equ 00000001
SERIAL_IOC_MCR_RTS               equ 00000002
SERIAL_IOC_MCR_OUT1              equ 00000004
SERIAL_IOC_MCR_OUT2              equ 00000008
SERIAL_IOC_MCR_LOOP              equ 00000010
USN_PAGE_SIZE                    equ 1000h
USN_REASON_DATA_OVERWRITE        equ 00000001h
USN_REASON_DATA_EXTEND           equ 00000002h
USN_REASON_DATA_TRUNCATION       equ 00000004h
USN_REASON_NAMED_DATA_OVERWRITE  equ 00000010h
USN_REASON_NAMED_DATA_EXTEND     equ 00000020h
USN_REASON_NAMED_DATA_TRUNCATION equ 00000040h
USN_REASON_FILE_CREATE           equ 00000100h
USN_REASON_FILE_DELETE           equ 00000200h
USN_REASON_EA_CHANGE             equ 00000400h
USN_REASON_SECURITY_CHANGE       equ 00000800h
USN_REASON_RENAME_OLD_NAME       equ 00001000h
USN_REASON_RENAME_NEW_NAME       equ 00002000h
USN_REASON_INDEXABLE_CHANGE      equ 00004000h
USN_REASON_BASIC_INFO_CHANGE     equ 00008000h
USN_REASON_HARD_LINK_CHANGE      equ 00010000h
USN_REASON_COMPRESSION_CHANGE    equ 00020000h
USN_REASON_ENCRYPTION_CHANGE     equ 00040000h
USN_REASON_OBJECT_ID_CHANGE      equ 00080000h
USN_REASON_REPARSE_POINT_CHANGE  equ 00100000h
USN_REASON_STREAM_CHANGE         equ 00200000h
USN_REASON_CLOSE                 equ 80000000h
USN_DELETE_FLAG_DELETE           equ 00000001h
USN_DELETE_FLAG_NOTIFY           equ 00000002h
USN_DELETE_VALID_FLAGS           equ 00000003h
USN_SOURCE_DATA_MANAGEMENT       equ 00000001h
USN_SOURCE_AUXILIARY_DATA        equ 00000002h
USN_SOURCE_REPLICATION_MANAGEMENT equ 00000004h
VOLUME_IS_DIRTY                  equ 00000001h
VOLUME_UPGRADE_SCHEDULED         equ 00000002h
FILESYSTEM_STATISTICS_TYPE_NTFS  equ 1
FILESYSTEM_STATISTICS_TYPE_FAT   equ 2
FILE_SET_ENCRYPTION              equ 00000001h
FILE_CLEAR_ENCRYPTION            equ 00000002h
STREAM_SET_ENCRYPTION            equ 00000003h
STREAM_CLEAR_ENCRYPTION          equ 00000004h
MAXIMUM_ENCRYPTION_VALUE         equ 00000004h
ENCRYPTION_FORMAT_DEFAULT        equ 1
COMPRESSION_FORMAT_SPARSE        equ 4000h
COPYFILE_SIS_LINK                equ 0001h
COPYFILE_SIS_REPLACE             equ 0002h
COPYFILE_SIS_FLAGS               equ 0003h
NSS_CONTROL_MARK                 equ 1
NSS_CONTROL_UNMARK               equ 2
NSS_CONTROL_ISNSSFILE            equ 4

;;;; end winioctl


;;;; tapi

TAPI_CURRENT_VERSION             equ 00020002h
LINE_ADDRESSSTATE                equ 0
LINE_CALLINFO                    equ 1
LINE_CALLSTATE                   equ 2
LINE_CLOSE                       equ 3
LINE_DEVSPECIFIC                 equ 4
LINE_DEVSPECIFICFEATURE          equ 5
LINE_GATHERDIGITS                equ 6
LINE_GENERATE                    equ 7
LINE_LINEDEVSTATE                equ 8
LINE_MONITORDIGITS               equ 9
LINE_MONITORMEDIA                equ 10
LINE_MONITORTONE                 equ 11
LINE_REPLY                       equ 12
LINE_REQUEST                     equ 13
PHONE_BUTTON                     equ 14
PHONE_CLOSE                      equ 15
PHONE_DEVSPECIFIC                equ 16
PHONE_REPLY                      equ 17
PHONE_STATE                      equ 18
LINE_CREATE                      equ 19
PHONE_CREATE                     equ 20
LINE_AGENTSPECIFIC               equ 21
LINE_AGENTSTATUS                 equ 22
LINE_APPNEWCALL                  equ 23
LINE_PROXYREQUEST                equ 24
LINE_REMOVE                      equ 25
PHONE_REMOVE                     equ 26
LINE_AGENTSESSIONSTATUS          equ 27
LINE_QUEUESTATUS                 equ 28
LINE_AGENTSTATUSEX               equ 29
LINE_GROUPSTATUS                 equ 30
LINE_PROXYSTATUS                 equ 31
LINE_APPNEWCALLHUB               equ 32
LINE_CALLHUBCLOSE                equ 33
LINE_DEVSPECIFICEX               equ 34
INITIALIZE_NEGOTIATION           equ 0FFFFFFFFh
LINEADDRCAPFLAGS_FWDNUMRINGS     equ 00000001h
LINEADDRCAPFLAGS_PICKUPGROUPID   equ 00000002h
LINEADDRCAPFLAGS_SECURE          equ 00000004h
LINEADDRCAPFLAGS_BLOCKIDDEFAULT  equ 00000008h
LINEADDRCAPFLAGS_BLOCKIDOVERRIDE equ 00000010h
LINEADDRCAPFLAGS_DIALED          equ 00000020h
LINEADDRCAPFLAGS_ORIGOFFHOOK     equ 00000040h
LINEADDRCAPFLAGS_DESTOFFHOOK     equ 00000080h
LINEADDRCAPFLAGS_FWDCONSULT      equ 00000100h
LINEADDRCAPFLAGS_SETUPCONFNULL   equ 00000200h
LINEADDRCAPFLAGS_AUTORECONNECT   equ 00000400h
LINEADDRCAPFLAGS_COMPLETIONID    equ 00000800h
LINEADDRCAPFLAGS_TRANSFERHELD    equ 00001000h
LINEADDRCAPFLAGS_TRANSFERMAKE    equ 00002000h
LINEADDRCAPFLAGS_CONFERENCEHELD  equ 00004000h
LINEADDRCAPFLAGS_CONFERENCEMAKE  equ 00008000h
LINEADDRCAPFLAGS_PARTIALDIAL     equ 00010000h
LINEADDRCAPFLAGS_FWDSTATUSVALID  equ 00020000h
LINEADDRCAPFLAGS_FWDINTEXTADDR   equ 00040000h
LINEADDRCAPFLAGS_FWDBUSYNAADDR   equ 00080000h
LINEADDRCAPFLAGS_ACCEPTTOALERT   equ 00100000h
LINEADDRCAPFLAGS_CONFDROP        equ 00200000h
LINEADDRCAPFLAGS_PICKUPCALLWAIT  equ 00400000h
LINEADDRCAPFLAGS_PREDICTIVEDIALER equ 00800000h
LINEADDRCAPFLAGS_QUEUE           equ 01000000h
LINEADDRCAPFLAGS_ROUTEPOINT      equ 02000000h
LINEADDRCAPFLAGS_HOLDMAKESNEW    equ 04000000h
LINEADDRCAPFLAGS_NOINTERNALCALLS equ 08000000h
LINEADDRCAPFLAGS_NOEXTERNALCALLS equ 10000000h
LINEADDRCAPFLAGS_SETCALLINGID    equ 20000000h
LINEADDRCAPFLAGS_ACDGROUP        equ 40000000h
LINEADDRCAPFLAGS_NOPSTNADDRESSTRANSLATION equ 80000000h
LINEADDRESSMODE_ADDRESSID        equ 00000001h
LINEADDRESSMODE_DIALABLEADDR     equ 00000002h
LINEADDRESSSHARING_PRIVATE       equ 00000001h
LINEADDRESSSHARING_BRIDGEDEXCL   equ 00000002h
LINEADDRESSSHARING_BRIDGEDNEW    equ 00000004h
LINEADDRESSSHARING_BRIDGEDSHARED equ 00000008h
LINEADDRESSSHARING_MONITORED     equ 00000010h
LINEADDRESSSTATE_OTHER           equ 00000001h
LINEADDRESSSTATE_DEVSPECIFIC     equ 00000002h
LINEADDRESSSTATE_INUSEZERO       equ 00000004h
LINEADDRESSSTATE_INUSEONE        equ 00000008h
LINEADDRESSSTATE_INUSEMANY       equ 00000010h
LINEADDRESSSTATE_NUMCALLS        equ 00000020h
LINEADDRESSSTATE_FORWARD         equ 00000040h
LINEADDRESSSTATE_TERMINALS       equ 00000080h
LINEADDRESSSTATE_CAPSCHANGE      equ 00000100h
LINEADDRESSTYPE_PHONENUMBER      equ 00000001h
LINEADDRESSTYPE_SDP              equ 00000002h
LINEADDRESSTYPE_EMAILNAME        equ 00000004h
LINEADDRESSTYPE_DOMAINNAME       equ 00000008h
LINEADDRESSTYPE_IPADDRESS        equ 00000010h
LINEADDRFEATURE_FORWARD          equ 00000001h
LINEADDRFEATURE_MAKECALL         equ 00000002h
LINEADDRFEATURE_PICKUP           equ 00000004h
LINEADDRFEATURE_SETMEDIACONTROL  equ 00000008h
LINEADDRFEATURE_SETTERMINAL      equ 00000010h
LINEADDRFEATURE_SETUPCONF        equ 00000020h
LINEADDRFEATURE_UNCOMPLETECALL   equ 00000040h
LINEADDRFEATURE_UNPARK           equ 00000080h
LINEADDRFEATURE_PICKUPHELD       equ 00000100h
LINEADDRFEATURE_PICKUPGROUP      equ 00000200h
LINEADDRFEATURE_PICKUPDIRECT     equ 00000400h
LINEADDRFEATURE_PICKUPWAITING    equ 00000800h
LINEADDRFEATURE_FORWARDFWD       equ 00001000h
LINEADDRFEATURE_FORWARDDND       equ 00002000h
LINEAGENTFEATURE_SETAGENTGROUP   equ 00000001h
LINEAGENTFEATURE_SETAGENTSTATE   equ 00000002h
LINEAGENTFEATURE_SETAGENTACTIVITY equ 00000004h
LINEAGENTFEATURE_AGENTSPECIFIC   equ 00000008h
LINEAGENTFEATURE_GETAGENTACTIVITYLIST equ 00000010h
LINEAGENTFEATURE_GETAGENTGROUP   equ 00000020h
LINEAGENTSTATE_LOGGEDOFF         equ 00000001h
LINEAGENTSTATE_NOTREADY          equ 00000002h
LINEAGENTSTATE_READY             equ 00000004h
LINEAGENTSTATE_BUSYACD           equ 00000008h
LINEAGENTSTATE_BUSYINCOMING      equ 00000010h
LINEAGENTSTATE_BUSYOUTBOUND      equ 00000020h
LINEAGENTSTATE_BUSYOTHER         equ 00000040h
LINEAGENTSTATE_WORKINGAFTERCALL  equ 00000080h
LINEAGENTSTATE_UNKNOWN           equ 00000100h
LINEAGENTSTATE_UNAVAIL           equ 00000200h
LINEAGENTSTATUS_GROUP            equ 00000001h
LINEAGENTSTATUS_STATE            equ 00000002h
LINEAGENTSTATUS_NEXTSTATE        equ 00000004h
LINEAGENTSTATUS_ACTIVITY         equ 00000008h
LINEAGENTSTATUS_ACTIVITYLIST     equ 00000010h
LINEAGENTSTATUS_GROUPLIST        equ 00000020h
LINEAGENTSTATUS_CAPSCHANGE       equ 00000040h
LINEAGENTSTATUS_VALIDSTATES      equ 00000080h
LINEAGENTSTATUS_VALIDNEXTSTATES  equ 00000100h
LINEAGENTSTATEEX_NOTREADY        equ 00000001h
LINEAGENTSTATEEX_READY           equ 00000002h
LINEAGENTSTATEEX_BUSYACD         equ 00000004h
LINEAGENTSTATEEX_BUSYINCOMING    equ 00000008h
LINEAGENTSTATEEX_BUSYOUTGOING    equ 00000010h
LINEAGENTSTATEEX_UNKNOWN         equ 00000020h
LINEAGENTSTATEEX_RELEASED        equ 00000040h
LINEAGENTSTATUSEX_NEWAGENT       equ 00000001h
LINEAGENTSTATUSEX_STATE          equ 00000002h
LINEAGENTSTATUSEX_UPDATEINFO     equ 00000004h
LINEAGENTSESSIONSTATE_NOTREADY   equ 00000001h
LINEAGENTSESSIONSTATE_READY      equ 00000002h
LINEAGENTSESSIONSTATE_BUSYONCALL equ 00000004h
LINEAGENTSESSIONSTATE_BUSYWRAPUP equ 00000008h
LINEAGENTSESSIONSTATE_ENDED      equ 00000010h
LINEAGENTSESSIONSTATE_RELEASED   equ 00000020h
LINEAGENTSESSIONSTATUS_NEWSESSION equ 00000001h
LINEAGENTSESSIONSTATUS_STATE     equ 00000002h
LINEAGENTSESSIONSTATUS_UPDATEINFO equ 00000004h
LINEQUEUESTATUS_UPDATEINFO       equ 00000001h
LINEQUEUESTATUS_NEWQUEUE         equ 00000002h
LINEQUEUESTATUS_QUEUEREMOVED     equ 00000004h
LINEGROUPSTATUS_NEWGROUP         equ 00000001h
LINEGROUPSTATUS_GROUPREMOVED     equ 00000002h
LINEPROXYSTATUS_OPEN             equ 00000001h
LINEPROXYSTATUS_CLOSE            equ 00000002h
LINEPROXYSTATUS_ALLOPENFORACD    equ 00000004h
LINEANSWERMODE_NONE              equ 00000001h
LINEANSWERMODE_DROP              equ 00000002h
LINEANSWERMODE_HOLD              equ 00000004h
LINEBEARERMODE_VOICE             equ 00000001h
LINEBEARERMODE_SPEECH            equ 00000002h
LINEBEARERMODE_MULTIUSE          equ 00000004h
LINEBEARERMODE_DATA              equ 00000008h
LINEBEARERMODE_ALTSPEECHDATA     equ 00000010h
LINEBEARERMODE_NONCALLSIGNALING  equ 00000020h
LINEBEARERMODE_PASSTHROUGH       equ 00000040h
LINEBEARERMODE_RESTRICTEDDATA    equ 00000080h
LINEBUSYMODE_STATION             equ 00000001h
LINEBUSYMODE_TRUNK               equ 00000002h
LINEBUSYMODE_UNKNOWN             equ 00000004h
LINEBUSYMODE_UNAVAIL             equ 00000008h
LINECALLCOMPLCOND_BUSY           equ 00000001h
LINECALLCOMPLCOND_NOANSWER       equ 00000002h
LINECALLCOMPLMODE_CAMPON         equ 00000001h
LINECALLCOMPLMODE_CALLBACK       equ 00000002h
LINECALLCOMPLMODE_INTRUDE        equ 00000004h
LINECALLCOMPLMODE_MESSAGE        equ 00000008h
LINECALLFEATURE_ACCEPT           equ 00000001h
LINECALLFEATURE_ADDTOCONF        equ 00000002h
LINECALLFEATURE_ANSWER           equ 00000004h
LINECALLFEATURE_BLINDTRANSFER    equ 00000008h
LINECALLFEATURE_COMPLETECALL     equ 00000010h
LINECALLFEATURE_COMPLETETRANSF   equ 00000020h
LINECALLFEATURE_DIAL             equ 00000040h
LINECALLFEATURE_DROP             equ 00000080h
LINECALLFEATURE_GATHERDIGITS     equ 00000100h
LINECALLFEATURE_GENERATEDIGITS   equ 00000200h
LINECALLFEATURE_GENERATETONE     equ 00000400h
LINECALLFEATURE_HOLD             equ 00000800h
LINECALLFEATURE_MONITORDIGITS    equ 00001000h
LINECALLFEATURE_MONITORMEDIA     equ 00002000h
LINECALLFEATURE_MONITORTONES     equ 00004000h
LINECALLFEATURE_PARK             equ 00008000h
LINECALLFEATURE_PREPAREADDCONF   equ 00010000h
LINECALLFEATURE_REDIRECT         equ 00020000h
LINECALLFEATURE_REMOVEFROMCONF   equ 00040000h
LINECALLFEATURE_SECURECALL       equ 00080000h
LINECALLFEATURE_SENDUSERUSER     equ 00100000h
LINECALLFEATURE_SETCALLPARAMS    equ 00200000h
LINECALLFEATURE_SETMEDIACONTROL  equ 00400000h
LINECALLFEATURE_SETTERMINAL      equ 00800000h
LINECALLFEATURE_SETUPCONF        equ 01000000h
LINECALLFEATURE_SETUPTRANSFER    equ 02000000h
LINECALLFEATURE_SWAPHOLD         equ 04000000h
LINECALLFEATURE_UNHOLD           equ 08000000h
LINECALLFEATURE_RELEASEUSERUSERINFO equ 10000000h
LINECALLFEATURE_SETTREATMENT     equ 20000000h
LINECALLFEATURE_SETQOS           equ 40000000h
LINECALLFEATURE_SETCALLDATA      equ 80000000h
LINECALLFEATURE2_NOHOLDCONFERENCE equ 00000001h
LINECALLFEATURE2_ONESTEPTRANSFER equ 00000002h
LINECALLFEATURE2_COMPLCAMPON     equ 00000004h
LINECALLFEATURE2_COMPLCALLBACK   equ 00000008h
LINECALLFEATURE2_COMPLINTRUDE    equ 00000010h
LINECALLFEATURE2_COMPLMESSAGE    equ 00000020h
LINECALLFEATURE2_TRANSFERNORM    equ 00000040h
LINECALLFEATURE2_TRANSFERCONF    equ 00000080h
LINECALLFEATURE2_PARKDIRECT      equ 00000100h
LINECALLFEATURE2_PARKNONDIRECT   equ 00000200h
LINECALLHUBTRACKING_NONE         equ 00000000h
LINECALLHUBTRACKING_PROVIDERLEVEL equ 00000001h
LINECALLHUBTRACKING_ALLCALLS     equ 00000002h
LINECALLINFOSTATE_OTHER          equ 00000001h
LINECALLINFOSTATE_DEVSPECIFIC    equ 00000002h
LINECALLINFOSTATE_BEARERMODE     equ 00000004h
LINECALLINFOSTATE_RATE           equ 00000008h
LINECALLINFOSTATE_MEDIAMODE      equ 00000010h
LINECALLINFOSTATE_APPSPECIFIC    equ 00000020h
LINECALLINFOSTATE_CALLID         equ 00000040h
LINECALLINFOSTATE_RELATEDCALLID  equ 00000080h
LINECALLINFOSTATE_ORIGIN         equ 00000100h
LINECALLINFOSTATE_REASON         equ 00000200h
LINECALLINFOSTATE_COMPLETIONID   equ 00000400h
LINECALLINFOSTATE_NUMOWNERINCR   equ 00000800h
LINECALLINFOSTATE_NUMOWNERDECR   equ 00001000h
LINECALLINFOSTATE_NUMMONITORS    equ 00002000h
LINECALLINFOSTATE_TRUNK          equ 00004000h
LINECALLINFOSTATE_CALLERID       equ 00008000h
LINECALLINFOSTATE_CALLEDID       equ 00010000h
LINECALLINFOSTATE_CONNECTEDID    equ 00020000h
LINECALLINFOSTATE_REDIRECTIONID  equ 00040000h
LINECALLINFOSTATE_REDIRECTINGID  equ 00080000h
LINECALLINFOSTATE_DISPLAY        equ 00100000h
LINECALLINFOSTATE_USERUSERINFO   equ 00200000h
LINECALLINFOSTATE_HIGHLEVELCOMP  equ 00400000h
LINECALLINFOSTATE_LOWLEVELCOMP   equ 00800000h
LINECALLINFOSTATE_CHARGINGINFO   equ 01000000h
LINECALLINFOSTATE_TERMINAL       equ 02000000h
LINECALLINFOSTATE_DIALPARAMS     equ 04000000h
LINECALLINFOSTATE_MONITORMODES   equ 08000000h
LINECALLINFOSTATE_TREATMENT      equ 10000000h
LINECALLINFOSTATE_QOS            equ 20000000h
LINECALLINFOSTATE_CALLDATA       equ 40000000h
LINECALLORIGIN_OUTBOUND          equ 00000001h
LINECALLORIGIN_INTERNAL          equ 00000002h
LINECALLORIGIN_EXTERNAL          equ 00000004h
LINECALLORIGIN_UNKNOWN           equ 00000010h
LINECALLORIGIN_UNAVAIL           equ 00000020h
LINECALLORIGIN_CONFERENCE        equ 00000040h
LINECALLORIGIN_INBOUND           equ 00000080h
LINECALLPARAMFLAGS_SECURE        equ 00000001h
LINECALLPARAMFLAGS_IDLE          equ 00000002h
LINECALLPARAMFLAGS_BLOCKID       equ 00000004h
LINECALLPARAMFLAGS_ORIGOFFHOOK   equ 00000008h
LINECALLPARAMFLAGS_DESTOFFHOOK   equ 00000010h
LINECALLPARAMFLAGS_NOHOLDCONFERENCE equ 00000020h
LINECALLPARAMFLAGS_PREDICTIVEDIAL equ 00000040h
LINECALLPARAMFLAGS_ONESTEPTRANSFER equ 00000080h
LINECALLPARTYID_BLOCKED          equ 00000001h
LINECALLPARTYID_OUTOFAREA        equ 00000002h
LINECALLPARTYID_NAME             equ 00000004h
LINECALLPARTYID_ADDRESS          equ 00000008h
LINECALLPARTYID_PARTIAL          equ 00000010h
LINECALLPARTYID_UNKNOWN          equ 00000020h
LINECALLPARTYID_UNAVAIL          equ 00000040h
LINECALLPRIVILEGE_NONE           equ 00000001h
LINECALLPRIVILEGE_MONITOR        equ 00000002h
LINECALLPRIVILEGE_OWNER          equ 00000004h
LINECALLREASON_DIRECT            equ 00000001h
LINECALLREASON_FWDBUSY           equ 00000002h
LINECALLREASON_FWDNOANSWER       equ 00000004h
LINECALLREASON_FWDUNCOND         equ 00000008h
LINECALLREASON_PICKUP            equ 00000010h
LINECALLREASON_UNPARK            equ 00000020h
LINECALLREASON_REDIRECT          equ 00000040h
LINECALLREASON_CALLCOMPLETION    equ 00000080h
LINECALLREASON_TRANSFER          equ 00000100h
LINECALLREASON_REMINDER          equ 00000200h
LINECALLREASON_UNKNOWN           equ 00000400h
LINECALLREASON_UNAVAIL           equ 00000800h
LINECALLREASON_INTRUDE           equ 00001000h
LINECALLREASON_PARKED            equ 00002000h
LINECALLREASON_CAMPEDON          equ 00004000h
LINECALLREASON_ROUTEREQUEST      equ 00008000h
LINECALLSELECT_LINE              equ 00000001h
LINECALLSELECT_ADDRESS           equ 00000002h
LINECALLSELECT_CALL              equ 00000004h
LINECALLSELECT_DEVICEID          equ 00000008h
LINECALLSELECT_CALLID            equ 00000010h
LINECALLSTATE_IDLE               equ 00000001h
LINECALLSTATE_OFFERING           equ 00000002h
LINECALLSTATE_ACCEPTED           equ 00000004h
LINECALLSTATE_DIALTONE           equ 00000008h
LINECALLSTATE_DIALING            equ 00000010h
LINECALLSTATE_RINGBACK           equ 00000020h
LINECALLSTATE_BUSY               equ 00000040h
LINECALLSTATE_SPECIALINFO        equ 00000080h
LINECALLSTATE_CONNECTED          equ 00000100h
LINECALLSTATE_PROCEEDING         equ 00000200h
LINECALLSTATE_ONHOLD             equ 00000400h
LINECALLSTATE_CONFERENCED        equ 00000800h
LINECALLSTATE_ONHOLDPENDCONF     equ 00001000h
LINECALLSTATE_ONHOLDPENDTRANSFER equ 00002000h
LINECALLSTATE_DISCONNECTED       equ 00004000h
LINECALLSTATE_UNKNOWN            equ 00008000h
LINECALLTREATMENT_SILENCE        equ 00000001h
LINECALLTREATMENT_RINGBACK       equ 00000002h
LINECALLTREATMENT_BUSY           equ 00000003h
LINECALLTREATMENT_MUSIC          equ 00000004h
LINECARDOPTION_PREDEFINED        equ 00000001h
LINECARDOPTION_HIDDEN            equ 00000002h
LINECONNECTEDMODE_ACTIVE         equ 00000001h
LINECONNECTEDMODE_INACTIVE       equ 00000002h
LINECONNECTEDMODE_ACTIVEHELD     equ 00000004h
LINECONNECTEDMODE_INACTIVEHELD   equ 00000008h
LINECONNECTEDMODE_CONFIRMED      equ 00000010h
LINEDEVCAPFLAGS_CROSSADDRCONF    equ 00000001h
LINEDEVCAPFLAGS_HIGHLEVCOMP      equ 00000002h
LINEDEVCAPFLAGS_LOWLEVCOMP       equ 00000004h
LINEDEVCAPFLAGS_MEDIACONTROL     equ 00000008h
LINEDEVCAPFLAGS_MULTIPLEADDR     equ 00000010h
LINEDEVCAPFLAGS_CLOSEDROP        equ 00000020h
LINEDEVCAPFLAGS_DIALBILLING      equ 00000040h
LINEDEVCAPFLAGS_DIALQUIET        equ 00000080h
LINEDEVCAPFLAGS_DIALDIALTONE     equ 00000100h
LINEDEVCAPFLAGS_MSP              equ 00000200h
LINEDEVCAPFLAGS_CALLHUB          equ 00000400h
LINEDEVCAPFLAGS_CALLHUBTRACKING  equ 00000800h
LINEDEVCAPFLAGS_PRIVATEOBJECTS   equ 00001000h
LINEDEVCAPFLAGS_LOCAL            equ 00002000h
LINEDEVSTATE_OTHER               equ 00000001h
LINEDEVSTATE_RINGING             equ 00000002h
LINEDEVSTATE_CONNECTED           equ 00000004h
LINEDEVSTATE_DISCONNECTED        equ 00000008h
LINEDEVSTATE_MSGWAITON           equ 00000010h
LINEDEVSTATE_MSGWAITOFF          equ 00000020h
LINEDEVSTATE_INSERVICE           equ 00000040h
LINEDEVSTATE_OUTOFSERVICE        equ 00000080h
LINEDEVSTATE_MAINTENANCE         equ 00000100h
LINEDEVSTATE_OPEN                equ 00000200h
LINEDEVSTATE_CLOSE               equ 00000400h
LINEDEVSTATE_NUMCALLS            equ 00000800h
LINEDEVSTATE_NUMCOMPLETIONS      equ 00001000h
LINEDEVSTATE_TERMINALS           equ 00002000h
LINEDEVSTATE_ROAMMODE            equ 00004000h
LINEDEVSTATE_BATTERY             equ 00008000h
LINEDEVSTATE_SIGNAL              equ 00010000h
LINEDEVSTATE_DEVSPECIFIC         equ 00020000h
LINEDEVSTATE_REINIT              equ 00040000h
LINEDEVSTATE_LOCK                equ 00080000h
LINEDEVSTATE_CAPSCHANGE          equ 00100000h
LINEDEVSTATE_CONFIGCHANGE        equ 00200000h
LINEDEVSTATE_TRANSLATECHANGE     equ 00400000h
LINEDEVSTATE_COMPLCANCEL         equ 00800000h
LINEDEVSTATE_REMOVED             equ 01000000h
LINEDEVSTATUSFLAGS_CONNECTED     equ 00000001h
LINEDEVSTATUSFLAGS_MSGWAIT       equ 00000002h
LINEDEVSTATUSFLAGS_INSERVICE     equ 00000004h
LINEDEVSTATUSFLAGS_LOCKED        equ 00000008h
LINEDIALTONEMODE_NORMAL          equ 00000001h
LINEDIALTONEMODE_SPECIAL         equ 00000002h
LINEDIALTONEMODE_INTERNAL        equ 00000004h
LINEDIALTONEMODE_EXTERNAL        equ 00000008h
LINEDIALTONEMODE_UNKNOWN         equ 00000010h
LINEDIALTONEMODE_UNAVAIL         equ 00000020h
LINEDIGITMODE_PULSE              equ 00000001h
LINEDIGITMODE_DTMF               equ 00000002h
LINEDIGITMODE_DTMFEND            equ 00000004h
LINEDISCONNECTMODE_NORMAL        equ 00000001h
LINEDISCONNECTMODE_UNKNOWN       equ 00000002h
LINEDISCONNECTMODE_REJECT        equ 00000004h
LINEDISCONNECTMODE_PICKUP        equ 00000008h
LINEDISCONNECTMODE_FORWARDED     equ 00000010h
LINEDISCONNECTMODE_BUSY          equ 00000020h
LINEDISCONNECTMODE_NOANSWER      equ 00000040h
LINEDISCONNECTMODE_BADADDRESS    equ 00000080h
LINEDISCONNECTMODE_UNREACHABLE   equ 00000100h
LINEDISCONNECTMODE_CONGESTION    equ 00000200h
LINEDISCONNECTMODE_INCOMPATIBLE  equ 00000400h
LINEDISCONNECTMODE_UNAVAIL       equ 00000800h
LINEDISCONNECTMODE_NODIALTONE    equ 00001000h
LINEDISCONNECTMODE_NUMBERCHANGED equ 00002000h
LINEDISCONNECTMODE_OUTOFORDER    equ 00004000h
LINEDISCONNECTMODE_TEMPFAILURE   equ 00008000h
LINEDISCONNECTMODE_QOSUNAVAIL    equ 00010000h
LINEDISCONNECTMODE_BLOCKED       equ 00020000h
LINEDISCONNECTMODE_DONOTDISTURB  equ 00040000h
LINEDISCONNECTMODE_CANCELLED     equ 00080000h
LINEERR_ALLOCATED                equ 80000001h
LINEERR_BADDEVICEID              equ 80000002h
LINEERR_BEARERMODEUNAVAIL        equ 80000003h
LINEERR_CALLUNAVAIL              equ 80000005h
LINEERR_COMPLETIONOVERRUN        equ 80000006h
LINEERR_CONFERENCEFULL           equ 80000007h
LINEERR_DIALBILLING              equ 80000008h
LINEERR_DIALDIALTONE             equ 80000009h
LINEERR_DIALPROMPT               equ 8000000Ah
LINEERR_DIALQUIET                equ 8000000Bh
LINEERR_INCOMPATIBLEAPIVERSION   equ 8000000Ch
LINEERR_INCOMPATIBLEEXTVERSION   equ 8000000Dh
LINEERR_INIFILECORRUPT           equ 8000000Eh
LINEERR_INUSE                    equ 8000000Fh
LINEERR_INVALADDRESS             equ 80000010h
LINEERR_INVALADDRESSID           equ 80000011h
LINEERR_INVALADDRESSMODE         equ 80000012h
LINEERR_INVALADDRESSSTATE        equ 80000013h
LINEERR_INVALAPPHANDLE           equ 80000014h
LINEERR_INVALAPPNAME             equ 80000015h
LINEERR_INVALBEARERMODE          equ 80000016h
LINEERR_INVALCALLCOMPLMODE       equ 80000017h
LINEERR_INVALCALLHANDLE          equ 80000018h
LINEERR_INVALCALLPARAMS          equ 80000019h
LINEERR_INVALCALLPRIVILEGE       equ 8000001Ah
LINEERR_INVALCALLSELECT          equ 8000001Bh
LINEERR_INVALCALLSTATE           equ 8000001Ch
LINEERR_INVALCALLSTATELIST       equ 8000001Dh
LINEERR_INVALCARD                equ 8000001Eh
LINEERR_INVALCOMPLETIONID        equ 8000001Fh
LINEERR_INVALCONFCALLHANDLE      equ 80000020h
LINEERR_INVALCONSULTCALLHANDLE   equ 80000021h
LINEERR_INVALCOUNTRYCODE         equ 80000022h
LINEERR_INVALDEVICECLASS         equ 80000023h
LINEERR_INVALDEVICEHANDLE        equ 80000024h
LINEERR_INVALDIALPARAMS          equ 80000025h
LINEERR_INVALDIGITLIST           equ 80000026h
LINEERR_INVALDIGITMODE           equ 80000027h
LINEERR_INVALDIGITS              equ 80000028h
LINEERR_INVALEXTVERSION          equ 80000029h
LINEERR_INVALGROUPID             equ 8000002Ah
LINEERR_INVALLINEHANDLE          equ 8000002Bh
LINEERR_INVALLINESTATE           equ 8000002Ch
LINEERR_INVALLOCATION            equ 8000002Dh
LINEERR_INVALMEDIALIST           equ 8000002Eh
LINEERR_INVALMEDIAMODE           equ 8000002Fh
LINEERR_INVALMESSAGEID           equ 80000030h
LINEERR_INVALPARAM               equ 80000032h
LINEERR_INVALPARKID              equ 80000033h
LINEERR_INVALPARKMODE            equ 80000034h
LINEERR_INVALPOINTER             equ 80000035h
LINEERR_INVALPRIVSELECT          equ 80000036h
LINEERR_INVALRATE                equ 80000037h
LINEERR_INVALREQUESTMODE         equ 80000038h
LINEERR_INVALTERMINALID          equ 80000039h
LINEERR_INVALTERMINALMODE        equ 8000003Ah
LINEERR_INVALTIMEOUT             equ 8000003Bh
LINEERR_INVALTONE                equ 8000003Ch
LINEERR_INVALTONELIST            equ 8000003Dh
LINEERR_INVALTONEMODE            equ 8000003Eh
LINEERR_INVALTRANSFERMODE        equ 8000003Fh
LINEERR_LINEMAPPERFAILED         equ 80000040h
LINEERR_NOCONFERENCE             equ 80000041h
LINEERR_NODEVICE                 equ 80000042h
LINEERR_NODRIVER                 equ 80000043h
LINEERR_NOMEM                    equ 80000044h
LINEERR_NOREQUEST                equ 80000045h
LINEERR_NOTOWNER                 equ 80000046h
LINEERR_NOTREGISTERED            equ 80000047h
LINEERR_OPERATIONFAILED          equ 80000048h
LINEERR_OPERATIONUNAVAIL         equ 80000049h
LINEERR_RATEUNAVAIL              equ 8000004Ah
LINEERR_RESOURCEUNAVAIL          equ 8000004Bh
LINEERR_REQUESTOVERRUN           equ 8000004Ch
LINEERR_STRUCTURETOOSMALL        equ 8000004Dh
LINEERR_TARGETNOTFOUND           equ 8000004Eh
LINEERR_TARGETSELF               equ 8000004Fh
LINEERR_UNINITIALIZED            equ 80000050h
LINEERR_USERUSERINFOTOOBIG       equ 80000051h
LINEERR_REINIT                   equ 80000052h
LINEERR_ADDRESSBLOCKED           equ 80000053h
LINEERR_BILLINGREJECTED          equ 80000054h
LINEERR_INVALFEATURE             equ 80000055h
LINEERR_NOMULTIPLEINSTANCE       equ 80000056h
LINEERR_INVALAGENTID             equ 80000057h
LINEERR_INVALAGENTGROUP          equ 80000058h
LINEERR_INVALPASSWORD            equ 80000059h
LINEERR_INVALAGENTSTATE          equ 8000005Ah
LINEERR_INVALAGENTACTIVITY       equ 8000005Bh
LINEERR_DIALVOICEDETECT          equ 8000005Ch
LINEERR_USERCANCELLED            equ 8000005Dh
LINEERR_INVALADDRESSTYPE         equ 8000005Eh
LINEERR_INVALAGENTSESSIONSTATE   equ 8000005Fh
LINEERR_DISCONNECTED             equ 0X80000060
LINEERR_SERVICE_NOT_RUNNING      equ 80000061h
LINEFEATURE_DEVSPECIFIC          equ 00000001h
LINEFEATURE_DEVSPECIFICFEAT      equ 00000002h
LINEFEATURE_FORWARD              equ 00000004h
LINEFEATURE_MAKECALL             equ 00000008h
LINEFEATURE_SETMEDIACONTROL      equ 00000010h
LINEFEATURE_SETTERMINAL          equ 00000020h
LINEFEATURE_SETDEVSTATUS         equ 00000040h
LINEFEATURE_FORWARDFWD           equ 00000080h
LINEFEATURE_FORWARDDND           equ 00000100h
LINEFORWARDMODE_UNCOND           equ 00000001h
LINEFORWARDMODE_UNCONDINTERNAL   equ 00000002h
LINEFORWARDMODE_UNCONDEXTERNAL   equ 00000004h
LINEFORWARDMODE_UNCONDSPECIFIC   equ 00000008h
LINEFORWARDMODE_BUSY             equ 00000010h
LINEFORWARDMODE_BUSYINTERNAL     equ 00000020h
LINEFORWARDMODE_BUSYEXTERNAL     equ 00000040h
LINEFORWARDMODE_BUSYSPECIFIC     equ 00000080h
LINEFORWARDMODE_NOANSW           equ 00000100h
LINEFORWARDMODE_NOANSWINTERNAL   equ 00000200h
LINEFORWARDMODE_NOANSWEXTERNAL   equ 00000400h
LINEFORWARDMODE_NOANSWSPECIFIC   equ 00000800h
LINEFORWARDMODE_BUSYNA           equ 00001000h
LINEFORWARDMODE_BUSYNAINTERNAL   equ 00002000h
LINEFORWARDMODE_BUSYNAEXTERNAL   equ 00004000h
LINEFORWARDMODE_BUSYNASPECIFIC   equ 00008000h
LINEFORWARDMODE_UNKNOWN          equ 00010000h
LINEFORWARDMODE_UNAVAIL          equ 00020000h
LINEGATHERTERM_BUFFERFULL        equ 00000001h
LINEGATHERTERM_TERMDIGIT         equ 00000002h
LINEGATHERTERM_FIRSTTIMEOUT      equ 00000004h
LINEGATHERTERM_INTERTIMEOUT      equ 00000008h
LINEGATHERTERM_CANCEL            equ 00000010h
LINEGENERATETERM_DONE            equ 00000001h
LINEGENERATETERM_CANCEL          equ 00000002h
LINEINITIALIZEEXOPTION_USEHIDDENWINDOW equ 00000001h
LINEINITIALIZEEXOPTION_USEEVENT  equ 00000002h
LINEINITIALIZEEXOPTION_USECOMPLETIONPORT equ 00000003h
LINEINITIALIZEEXOPTION_CALLHUBTRACKING equ 80000000h
LINELOCATIONOPTION_PULSEDIAL     equ 00000001h
LINEMAPPER                       equ 0FFFFFFFFh
LINEMEDIACONTROL_NONE            equ 00000001h
LINEMEDIACONTROL_START           equ 00000002h
LINEMEDIACONTROL_RESET           equ 00000004h
LINEMEDIACONTROL_PAUSE           equ 00000008h
LINEMEDIACONTROL_RESUME          equ 00000010h
LINEMEDIACONTROL_RATEUP          equ 00000020h
LINEMEDIACONTROL_RATEDOWN        equ 00000040h
LINEMEDIACONTROL_RATENORMAL      equ 00000080h
LINEMEDIACONTROL_VOLUMEUP        equ 00000100h
LINEMEDIACONTROL_VOLUMEDOWN      equ 00000200h
LINEMEDIACONTROL_VOLUMENORMAL    equ 00000400h
LINEMEDIAMODE_UNKNOWN            equ 00000002h
LINEMEDIAMODE_INTERACTIVEVOICE   equ 00000004h
LINEMEDIAMODE_AUTOMATEDVOICE     equ 00000008h
LINEMEDIAMODE_DATAMODEM          equ 00000010h
LINEMEDIAMODE_G3FAX              equ 00000020h
LINEMEDIAMODE_TDD                equ 00000040h
LINEMEDIAMODE_G4FAX              equ 00000080h
LINEMEDIAMODE_DIGITALDATA        equ 00000100h
LINEMEDIAMODE_TELETEX            equ 00000200h
LINEMEDIAMODE_VIDEOTEX           equ 00000400h
LINEMEDIAMODE_TELEX              equ 00000800h
LINEMEDIAMODE_MIXED              equ 00001000h
LINEMEDIAMODE_ADSI               equ 00002000h
LINEMEDIAMODE_VOICEVIEW          equ 00004000h
LINEMEDIAMODE_VIDEO              equ 00008000h
LAST_LINEMEDIAMODE               equ 00008000h
LINEOFFERINGMODE_ACTIVE          equ 00000001h
LINEOFFERINGMODE_INACTIVE        equ 00000002h
LINEOPENOPTION_SINGLEADDRESS     equ 80000000h
LINEOPENOPTION_PROXY             equ 40000000h
LINEPARKMODE_DIRECTED            equ 00000001h
LINEPARKMODE_NONDIRECTED         equ 00000002h
LINEPROXYREQUEST_SETAGENTGROUP   equ 00000001h
LINEPROXYREQUEST_SETAGENTSTATE   equ 00000002h
LINEPROXYREQUEST_SETAGENTACTIVITY equ 00000003h
LINEPROXYREQUEST_GETAGENTCAPS    equ 00000004h
LINEPROXYREQUEST_GETAGENTSTATUS  equ 00000005h
LINEPROXYREQUEST_AGENTSPECIFIC   equ 00000006h
LINEPROXYREQUEST_GETAGENTACTIVITYLIST equ 00000007h
LINEPROXYREQUEST_GETAGENTGROUPLIST equ 00000008h
LINEPROXYREQUEST_CREATEAGENT     equ 00000009h
LINEPROXYREQUEST_SETAGENTMEASUREMENTPERIOD equ 0000000Ah
LINEPROXYREQUEST_GETAGENTINFO    equ 0000000Bh
LINEPROXYREQUEST_CREATEAGENTSESSION equ 0000000Ch
LINEPROXYREQUEST_GETAGENTSESSIONLIST equ 0000000Dh
LINEPROXYREQUEST_SETAGENTSESSIONSTATE equ 0000000Eh
LINEPROXYREQUEST_GETAGENTSESSIONINFO equ 0000000Fh
LINEPROXYREQUEST_GETQUEUELIST    equ 00000010h
LINEPROXYREQUEST_SETQUEUEMEASUREMENTPERIOD equ 00000011h
LINEPROXYREQUEST_GETQUEUEINFO    equ 00000012h
LINEPROXYREQUEST_GETGROUPLIST    equ 00000013h
LINEPROXYREQUEST_SETAGENTSTATEEX equ 00000014h
LINEREMOVEFROMCONF_NONE          equ 00000001h
LINEREMOVEFROMCONF_LAST          equ 00000002h
LINEREMOVEFROMCONF_ANY           equ 00000003h
LINEREQUESTMODE_MAKECALL         equ 00000001h
LINEREQUESTMODE_MEDIACALL        equ 00000002h
LINEREQUESTMODE_DROP             equ 00000004h
LAST_LINEREQUESTMODE             equ LINEREQUESTMODE_MEDIACAL
LINEROAMMODE_UNKNOWN             equ 00000001h
LINEROAMMODE_UNAVAIL             equ 00000002h
LINEROAMMODE_HOME                equ 00000004h
LINEROAMMODE_ROAMA               equ 00000008h
LINEROAMMODE_ROAMB               equ 00000010h
LINESPECIALINFO_NOCIRCUIT        equ 00000001h
LINESPECIALINFO_CUSTIRREG        equ 00000002h
LINESPECIALINFO_REORDER          equ 00000004h
LINESPECIALINFO_UNKNOWN          equ 00000008h
LINESPECIALINFO_UNAVAIL          equ 00000010h
LINETERMDEV_PHONE                equ 00000001h
LINETERMDEV_HEADSET              equ 00000002h
LINETERMDEV_SPEAKER              equ 00000004h
LINETERMMODE_BUTTONS             equ 00000001h
LINETERMMODE_LAMPS               equ 00000002h
LINETERMMODE_DISPLAY             equ 00000004h
LINETERMMODE_RINGER              equ 00000008h
LINETERMMODE_HOOKSWITCH          equ 00000010h
LINETERMMODE_MEDIATOLINE         equ 00000020h
LINETERMMODE_MEDIAFROMLINE       equ 00000040h
LINETERMMODE_MEDIABIDIRECT       equ 00000080h
LINETERMSHARING_PRIVATE          equ 00000001h
LINETERMSHARING_SHAREDEXCL       equ 00000002h
LINETERMSHARING_SHAREDCONF       equ 00000004h
LINETOLLLISTOPTION_ADD           equ 00000001h
LINETOLLLISTOPTION_REMOVE        equ 00000002h
LINETONEMODE_CUSTOM              equ 00000001h
LINETONEMODE_RINGBACK            equ 00000002h
LINETONEMODE_BUSY                equ 00000004h
LINETONEMODE_BEEP                equ 00000008h
LINETONEMODE_BILLING             equ 00000010h
LINETRANSFERMODE_TRANSFER        equ 00000001h
LINETRANSFERMODE_CONFERENCE      equ 00000002h
LINETRANSLATEOPTION_CARDOVERRIDE equ 00000001h
LINETRANSLATEOPTION_CANCELCALLWAITING equ 00000002h
LINETRANSLATEOPTION_FORCELOCAL   equ 00000004h
LINETRANSLATEOPTION_FORCELD      equ 00000008h
LINETRANSLATERESULT_CANONICAL    equ 00000001h
LINETRANSLATERESULT_INTERNATIONAL equ 00000002h
LINETRANSLATERESULT_LONGDISTANCE equ 00000004h
LINETRANSLATERESULT_LOCAL        equ 00000008h
LINETRANSLATERESULT_INTOLLLIST   equ 00000010h
LINETRANSLATERESULT_NOTINTOLLLIST equ 00000020h
LINETRANSLATERESULT_DIALBILLING  equ 00000040h
LINETRANSLATERESULT_DIALQUIET    equ 00000080h
LINETRANSLATERESULT_DIALDIALTONE equ 00000100h
LINETRANSLATERESULT_DIALPROMPT   equ 00000200h
LINETRANSLATERESULT_VOICEDETECT  equ 00000400h
LINETRANSLATERESULT_NOTRANSLATION equ 00000800h
PHONEBUTTONFUNCTION_UNKNOWN      equ 00000000h
PHONEBUTTONFUNCTION_CONFERENCE   equ 00000001h
PHONEBUTTONFUNCTION_TRANSFER     equ 00000002h
PHONEBUTTONFUNCTION_DROP         equ 00000003h
PHONEBUTTONFUNCTION_HOLD         equ 00000004h
PHONEBUTTONFUNCTION_RECALL       equ 00000005h
PHONEBUTTONFUNCTION_DISCONNECT   equ 00000006h
PHONEBUTTONFUNCTION_CONNECT      equ 00000007h
PHONEBUTTONFUNCTION_MSGWAITON    equ 00000008h
PHONEBUTTONFUNCTION_MSGWAITOFF   equ 00000009h
PHONEBUTTONFUNCTION_SELECTRING   equ 0000000Ah
PHONEBUTTONFUNCTION_ABBREVDIAL   equ 0000000Bh
PHONEBUTTONFUNCTION_FORWARD      equ 0000000Ch
PHONEBUTTONFUNCTION_PICKUP       equ 0000000Dh
PHONEBUTTONFUNCTION_RINGAGAIN    equ 0000000Eh
PHONEBUTTONFUNCTION_PARK         equ 0000000Fh
PHONEBUTTONFUNCTION_REJECT       equ 00000010h
PHONEBUTTONFUNCTION_REDIRECT     equ 00000011h
PHONEBUTTONFUNCTION_MUTE         equ 00000012h
PHONEBUTTONFUNCTION_VOLUMEUP     equ 00000013h
PHONEBUTTONFUNCTION_VOLUMEDOWN   equ 00000014h
PHONEBUTTONFUNCTION_SPEAKERON    equ 00000015h
PHONEBUTTONFUNCTION_SPEAKEROFF   equ 00000016h
PHONEBUTTONFUNCTION_FLASH        equ 00000017h
PHONEBUTTONFUNCTION_DATAON       equ 00000018h
PHONEBUTTONFUNCTION_DATAOFF      equ 00000019h
PHONEBUTTONFUNCTION_DONOTDISTURB equ 0000001Ah
PHONEBUTTONFUNCTION_INTERCOM     equ 0000001Bh
PHONEBUTTONFUNCTION_BRIDGEDAPP   equ 0000001Ch
PHONEBUTTONFUNCTION_BUSY         equ 0000001Dh
PHONEBUTTONFUNCTION_CALLAPP      equ 0000001Eh
PHONEBUTTONFUNCTION_DATETIME     equ 0000001Fh
PHONEBUTTONFUNCTION_DIRECTORY    equ 00000020h
PHONEBUTTONFUNCTION_COVER        equ 00000021h
PHONEBUTTONFUNCTION_CALLID       equ 00000022h
PHONEBUTTONFUNCTION_LASTNUM      equ 00000023h
PHONEBUTTONFUNCTION_NIGHTSRV     equ 00000024h
PHONEBUTTONFUNCTION_SENDCALLS    equ 00000025h
PHONEBUTTONFUNCTION_MSGINDICATOR equ 00000026h
PHONEBUTTONFUNCTION_REPDIAL      equ 00000027h
PHONEBUTTONFUNCTION_SETREPDIAL   equ 00000028h
PHONEBUTTONFUNCTION_SYSTEMSPEED  equ 00000029h
PHONEBUTTONFUNCTION_STATIONSPEED equ 0000002Ah
PHONEBUTTONFUNCTION_CAMPON       equ 0000002Bh
PHONEBUTTONFUNCTION_SAVEREPEAT   equ 0000002Ch
PHONEBUTTONFUNCTION_QUEUECALL    equ 0000002Dh
PHONEBUTTONFUNCTION_NONE         equ 0000002Eh
PHONEBUTTONFUNCTION_SEND         equ 0000002Fh
PHONEBUTTONMODE_DUMMY            equ 00000001h
PHONEBUTTONMODE_CALL             equ 00000002h
PHONEBUTTONMODE_FEATURE          equ 00000004h
PHONEBUTTONMODE_KEYPAD           equ 00000008h
PHONEBUTTONMODE_LOCAL            equ 00000010h
PHONEBUTTONMODE_DISPLAY          equ 00000020h
PHONEBUTTONSTATE_UP              equ 00000001h
PHONEBUTTONSTATE_DOWN            equ 00000002h
PHONEBUTTONSTATE_UNKNOWN         equ 00000004h
PHONEBUTTONSTATE_UNAVAIL         equ 00000008h
PHONEERR_ALLOCATED               equ 90000001h
PHONEERR_BADDEVICEID             equ 90000002h
PHONEERR_INCOMPATIBLEAPIVERSION  equ 90000003h
PHONEERR_INCOMPATIBLEEXTVERSION  equ 90000004h
PHONEERR_INIFILECORRUPT          equ 90000005h
PHONEERR_INUSE                   equ 90000006h
PHONEERR_INVALAPPHANDLE          equ 90000007h
PHONEERR_INVALAPPNAME            equ 90000008h
PHONEERR_INVALBUTTONLAMPID       equ 90000009h
PHONEERR_INVALBUTTONMODE         equ 9000000Ah
PHONEERR_INVALBUTTONSTATE        equ 9000000Bh
PHONEERR_INVALDATAID             equ 9000000Ch
PHONEERR_INVALDEVICECLASS        equ 9000000Dh
PHONEERR_INVALEXTVERSION         equ 9000000Eh
PHONEERR_INVALHOOKSWITCHDEV      equ 9000000Fh
PHONEERR_INVALHOOKSWITCHMODE     equ 90000010h
PHONEERR_INVALLAMPMODE           equ 90000011h
PHONEERR_INVALPARAM              equ 90000012h
PHONEERR_INVALPHONEHANDLE        equ 90000013h
PHONEERR_INVALPHONESTATE         equ 90000014h
PHONEERR_INVALPOINTER            equ 90000015h
PHONEERR_INVALPRIVILEGE          equ 90000016h
PHONEERR_INVALRINGMODE           equ 90000017h
PHONEERR_NODEVICE                equ 90000018h
PHONEERR_NODRIVER                equ 90000019h
PHONEERR_NOMEM                   equ 9000001Ah
PHONEERR_NOTOWNER                equ 9000001Bh
PHONEERR_OPERATIONFAILED         equ 9000001Ch
PHONEERR_OPERATIONUNAVAIL        equ 9000001Dh
PHONEERR_RESOURCEUNAVAIL         equ 9000001Fh
PHONEERR_REQUESTOVERRUN          equ 90000020h
PHONEERR_STRUCTURETOOSMALL       equ 90000021h
PHONEERR_UNINITIALIZED           equ 90000022h
PHONEERR_REINIT                  equ 90000023h
PHONEERR_DISCONNECTED            equ 90000024h
PHONEERR_SERVICE_NOT_RUNNING     equ 90000025h
PHONEFEATURE_GETBUTTONINFO       equ 00000001h
PHONEFEATURE_GETDATA             equ 00000002h
PHONEFEATURE_GETDISPLAY          equ 00000004h
PHONEFEATURE_GETGAINHANDSET      equ 00000008h
PHONEFEATURE_GETGAINSPEAKER      equ 00000010h
PHONEFEATURE_GETGAINHEADSET      equ 00000020h
PHONEFEATURE_GETHOOKSWITCHHANDSET equ 00000040h
PHONEFEATURE_GETHOOKSWITCHSPEAKER equ 00000080h
PHONEFEATURE_GETHOOKSWITCHHEADSET equ 00000100h
PHONEFEATURE_GETLAMP             equ 00000200h
PHONEFEATURE_GETRING             equ 00000400h
PHONEFEATURE_GETVOLUMEHANDSET    equ 00000800h
PHONEFEATURE_GETVOLUMESPEAKER    equ 00001000h
PHONEFEATURE_GETVOLUMEHEADSET    equ 00002000h
PHONEFEATURE_SETBUTTONINFO       equ 00004000h
PHONEFEATURE_SETDATA             equ 00008000h
PHONEFEATURE_SETDISPLAY          equ 00010000h
PHONEFEATURE_SETGAINHANDSET      equ 00020000h
PHONEFEATURE_SETGAINSPEAKER      equ 00040000h
PHONEFEATURE_SETGAINHEADSET      equ 00080000h
PHONEFEATURE_SETHOOKSWITCHHANDSET equ 00100000h
PHONEFEATURE_SETHOOKSWITCHSPEAKER equ 00200000h
PHONEFEATURE_SETHOOKSWITCHHEADSET equ 00400000h
PHONEFEATURE_SETLAMP             equ 00800000h
PHONEFEATURE_SETRING             equ 01000000h
PHONEFEATURE_SETVOLUMEHANDSET    equ 02000000h
PHONEFEATURE_SETVOLUMESPEAKER    equ 04000000h
PHONEFEATURE_SETVOLUMEHEADSET    equ 08000000h
PHONEFEATURE_GENERICPHONE        equ 10000000h
PHONEHOOKSWITCHDEV_HANDSET       equ 00000001h
PHONEHOOKSWITCHDEV_SPEAKER       equ 00000002h
PHONEHOOKSWITCHDEV_HEADSET       equ 00000004h
PHONEHOOKSWITCHMODE_ONHOOK       equ 00000001h
PHONEHOOKSWITCHMODE_MIC          equ 00000002h
PHONEHOOKSWITCHMODE_SPEAKER      equ 00000004h
PHONEHOOKSWITCHMODE_MICSPEAKER   equ 00000008h
PHONEHOOKSWITCHMODE_UNKNOWN      equ 00000010h
PHONEINITIALIZEEXOPTION_USEHIDDENWINDOW equ 00000001h
PHONEINITIALIZEEXOPTION_USEEVENT equ 00000002h
PHONEINITIALIZEEXOPTION_USECOMPLETIONPORT equ 00000003h
PHONELAMPMODE_DUMMY              equ 00000001h
PHONELAMPMODE_OFF                equ 00000002h
PHONELAMPMODE_STEADY             equ 00000004h
PHONELAMPMODE_WINK               equ 00000008h
PHONELAMPMODE_FLASH              equ 00000010h
PHONELAMPMODE_FLUTTER            equ 00000020h
PHONELAMPMODE_BROKENFLUTTER      equ 00000040h
PHONELAMPMODE_UNKNOWN            equ 00000080h
PHONEPRIVILEGE_MONITOR           equ 00000001h
PHONEPRIVILEGE_OWNER             equ 00000002h
PHONESTATE_OTHER                 equ 00000001h
PHONESTATE_CONNECTED             equ 00000002h
PHONESTATE_DISCONNECTED          equ 00000004h
PHONESTATE_OWNER                 equ 00000008h
PHONESTATE_MONITORS              equ 00000010h
PHONESTATE_DISPLAY               equ 00000020h
PHONESTATE_LAMP                  equ 00000040h
PHONESTATE_RINGMODE              equ 00000080h
PHONESTATE_RINGVOLUME            equ 00000100h
PHONESTATE_HANDSETHOOKSWITCH     equ 00000200h
PHONESTATE_HANDSETVOLUME         equ 00000400h
PHONESTATE_HANDSETGAIN           equ 00000800h
PHONESTATE_SPEAKERHOOKSWITCH     equ 00001000h
PHONESTATE_SPEAKERVOLUME         equ 00002000h
PHONESTATE_SPEAKERGAIN           equ 00004000h
PHONESTATE_HEADSETHOOKSWITCH     equ 00008000h
PHONESTATE_HEADSETVOLUME         equ 00010000h
PHONESTATE_HEADSETGAIN           equ 00020000h
PHONESTATE_SUSPEND               equ 00040000h
PHONESTATE_RESUME                equ 00080000h
PHONESTATE_DEVSPECIFIC           equ 00100000h
PHONESTATE_REINIT                equ 00200000h
PHONESTATE_CAPSCHANGE            equ 00400000h
PHONESTATE_REMOVED               equ 00800000h
PHONESTATUSFLAGS_CONNECTED       equ 00000001h
PHONESTATUSFLAGS_SUSPENDED       equ 00000002h
STRINGFORMAT_ASCII               equ 00000001h
STRINGFORMAT_DBCS                equ 00000002h
STRINGFORMAT_UNICODE             equ 00000003h
STRINGFORMAT_BINARY              equ 00000004h
TAPI_REPLY                       equ WM_USER + 99
TAPIERR_CONNECTED                equ 0
TAPIERR_DROPPED                  equ -1
TAPIERR_NOREQUESTRECIPIENT       equ -2
TAPIERR_REQUESTQUEUEFULL         equ -3
TAPIERR_INVALDESTADDRESS         equ -4
TAPIERR_INVALWINDOWHANDLE        equ -5
TAPIERR_INVALDEVICECLASS         equ -6
TAPIERR_INVALDEVICEID            equ -7
TAPIERR_DEVICECLASSUNAVAIL       equ -8
TAPIERR_DEVICEIDUNAVAIL          equ -9
TAPIERR_DEVICEINUSE              equ -10
TAPIERR_DESTBUSY                 equ -11
TAPIERR_DESTNOANSWER             equ -12
TAPIERR_DESTUNAVAIL              equ -13
TAPIERR_UNKNOWNWINHANDLE         equ -14
TAPIERR_UNKNOWNREQUESTID         equ -15
TAPIERR_REQUESTFAILED            equ -16
TAPIERR_REQUESTCANCELLED         equ -17
TAPIERR_INVALPOINTER             equ -18
TAPIERR_NOTADMIN                 equ -19
TAPIERR_MMCWRITELOCKED           equ -20
TAPIERR_PROVIDERALREADYINSTALLED equ -21
TAPIMAXDESTADDRESSSIZE           equ 80
TAPIMAXAPPNAMESIZE               equ 40
TAPIMAXCALLEDPARTYSIZE           equ 40
TAPIMAXCOMMENTSIZE               equ 80
TAPIMAXDEVICECLASSSIZE           equ 40
TAPIMAXDEVICEIDSIZE              equ 40

;;;; end tapi


;;;; ntstatus

STATUS_SUCCESS                   equ 00000000h
FACILITY_USB_ERROR_CODE          equ 10h
FACILITY_TERMINAL_SERVER         equ Ah
FACILITY_SXS_ERROR_CODE          equ 15h
FACILITY_RPC_STUBS               equ 3h
FACILITY_RPC_RUNTIME             equ 2h
FACILITY_IO_ERROR_CODE           equ 4h
FACILITY_HID_ERROR_CODE          equ 11h
FACILITY_FIREWIRE_ERROR_CODE     equ 12h
FACILITY_DEBUGGER                equ 1h
FACILITY_CLUSTER_ERROR_CODE      equ 13h
FACILITY_ACPI_ERROR_CODE         equ 14h
STATUS_SEVERITY_WARNING          equ 2h
STATUS_SEVERITY_SUCCESS          equ 0h
STATUS_SEVERITY_INFORMATIONAL    equ 1h
STATUS_SEVERITY_ERROR            equ 3h
STATUS_WAIT_1                    equ 00000001h
STATUS_WAIT_2                    equ 00000002h
STATUS_WAIT_3                    equ 00000003h
STATUS_WAIT_63                   equ 0000003Fh
STATUS_ABANDONED                 equ 00000080h
STATUS_ABANDONED_WAIT_63         equ 000000BFh
STATUS_KERNEL_APC                equ 00000100h
STATUS_ALERTED                   equ 00000101h
STATUS_REPARSE                   equ 00000104h
STATUS_MORE_ENTRIES              equ 00000105h
STATUS_NOT_ALL_ASSIGNED          equ 00000106h
STATUS_SOME_NOT_MAPPED           equ 00000107h
STATUS_OPLOCK_BREAK_IN_PROGRESS  equ 00000108h
STATUS_VOLUME_MOUNTED            equ 00000109h
STATUS_RXACT_COMMITTED           equ 0000010Ah
STATUS_NOTIFY_CLEANUP            equ 0000010Bh
STATUS_NOTIFY_ENUM_DIR           equ 0000010Ch
STATUS_NO_QUOTAS_FOR_ACCOUNT     equ 0000010Dh
STATUS_PRIMARY_TRANSPORT_CONNECT_FAILED equ 0000010Eh
STATUS_PAGE_FAULT_TRANSITION     equ 00000110h
STATUS_PAGE_FAULT_DEMAND_ZERO    equ 00000111h
STATUS_PAGE_FAULT_COPY_ON_WRITE  equ 00000112h
STATUS_PAGE_FAULT_GUARD_PAGE     equ 00000113h
STATUS_PAGE_FAULT_PAGING_FILE    equ 00000114h
STATUS_CACHE_PAGE_LOCKED         equ 00000115h
STATUS_CRASH_DUMP                equ 00000116h
STATUS_BUFFER_ALL_ZEROS          equ 00000117h
STATUS_REPARSE_OBJECT            equ 00000118h
STATUS_RESOURCE_REQUIREMENTS_CHANGED equ 00000119h
STATUS_TRANSLATION_COMPLETE      equ 00000120h
STATUS_DS_MEMBERSHIP_EVALUATED_LOCALLY equ 00000121h
STATUS_NOTHING_TO_TERMINATE      equ 00000122h
STATUS_PROCESS_NOT_IN_JOB        equ 00000123h
STATUS_PROCESS_IN_JOB            equ 00000124h
STATUS_VOLSNAP_HIBERNATE_READY   equ 00000125h
STATUS_FSFILTER_OP_COMPLETED_SUCCESSFULLY equ 00000126h
STATUS_OBJECT_NAME_EXISTS        equ 40000000h
STATUS_THREAD_WAS_SUSPENDED      equ 40000001h
STATUS_WORKING_SET_LIMIT_RANGE   equ 40000002h
STATUS_IMAGE_NOT_AT_BASE         equ 40000003h
STATUS_RXACT_STATE_CREATED       equ 40000004h
STATUS_LOCAL_USER_SESSION_KEY    equ 40000006h
STATUS_BAD_CURRENT_DIRECTORY     equ 40000007h
STATUS_SERIAL_MORE_WRITES        equ 40000008h
STATUS_REGISTRY_RECOVERED        equ 40000009h
STATUS_FT_READ_RECOVERY_FROM_BACKUP equ 4000000Ah
STATUS_FT_WRITE_RECOVERY         equ 4000000Bh
STATUS_SERIAL_COUNTER_TIMEOUT    equ 4000000Ch
STATUS_NULL_LM_PASSWORD          equ 4000000Dh
STATUS_IMAGE_MACHINE_TYPE_MISMATCH equ 4000000Eh
STATUS_RECEIVE_PARTIAL           equ 4000000Fh
STATUS_RECEIVE_EXPEDITED         equ 40000010h
STATUS_RECEIVE_PARTIAL_EXPEDITED equ 40000011h
STATUS_EVENT_DONE                equ 40000012h
STATUS_EVENT_PENDING             equ 40000013h
STATUS_CHECKING_FILE_SYSTEM      equ 40000014h
STATUS_FATAL_APP_EXIT            equ 40000015h
STATUS_PREDEFINED_HANDLE         equ 40000016h
STATUS_WAS_UNLOCKED              equ 40000017h
STATUS_SERVICE_NOTIFICATION      equ 40000018h
STATUS_WAS_LOCKED                equ 40000019h
STATUS_LOG_HARD_ERROR            equ 4000001Ah
STATUS_ALREADY_WIN32             equ 4000001Bh
STATUS_WX86_UNSIMULATE           equ 4000001Ch
STATUS_WX86_CONTINUE             equ 4000001Dh
STATUS_WX86_SINGLE_STEP          equ 4000001Eh
STATUS_WX86_BREAKPOINT           equ 4000001Fh
STATUS_WX86_EXCEPTION_CONTINUE   equ 40000020h
STATUS_WX86_EXCEPTION_LASTCHANCE equ 40000021h
STATUS_WX86_EXCEPTION_CHAIN      equ 40000022h
STATUS_IMAGE_MACHINE_TYPE_MISMATCH_EXE equ 40000023h
STATUS_NO_YIELD_PERFORMED        equ 40000024h
STATUS_TIMER_RESUME_IGNORED      equ 40000025h
STATUS_ARBITRATION_UNHANDLED     equ 40000026h
STATUS_CARDBUS_NOT_SUPPORTED     equ 40000027h
STATUS_WX86_CREATEWX86TIB        equ 40000028h
STATUS_MP_PROCESSOR_MISMATCH     equ 40000029h
STATUS_HIBERNATED                equ 4000002Ah
STATUS_RESUME_HIBERNATION        equ 4000002Bh
STATUS_FIRMWARE_UPDATED          equ 4000002Ch
STATUS_DRIVERS_LEAKING_LOCKED_PAGES equ 4000002Dh
DBG_REPLY_LATER                  equ 40010001h
DBG_UNABLE_TO_PROVIDE_HANDLE     equ 40010002h
DBG_PRINTEXCEPTION_C             equ 40010006h
DBG_RIPEXCEPTION                 equ 40010007h
STATUS_BUFFER_OVERFLOW           equ 80000005h
STATUS_NO_MORE_FILES             equ 80000006h
STATUS_WAKE_SYSTEM_DEBUGGER      equ 80000007h
STATUS_HANDLES_CLOSED            equ 8000000Ah
STATUS_NO_INHERITANCE            equ 8000000Bh
STATUS_GUID_SUBSTITUTION_MADE    equ 8000000Ch
STATUS_PARTIAL_COPY              equ 8000000Dh
STATUS_DEVICE_PAPER_EMPTY        equ 8000000Eh
STATUS_DEVICE_POWERED_OFF        equ 8000000Fh
STATUS_DEVICE_OFF_LINE           equ 80000010h
STATUS_DEVICE_BUSY               equ 80000011h
STATUS_NO_MORE_EAS               equ 80000012h
STATUS_INVALID_EA_NAME           equ 80000013h
STATUS_EA_LIST_INCONSISTENT      equ 80000014h
STATUS_INVALID_EA_FLAG           equ 80000015h
STATUS_VERIFY_REQUIRED           equ 80000016h
STATUS_EXTRANEOUS_INFORMATION    equ 80000017h
STATUS_RXACT_COMMIT_NECESSARY    equ 80000018h
STATUS_NO_MORE_ENTRIES           equ 8000001Ah
STATUS_FILEMARK_DETECTED         equ 8000001Bh
STATUS_MEDIA_CHANGED             equ 8000001Ch
STATUS_BUS_RESET                 equ 8000001Dh
STATUS_END_OF_MEDIA              equ 8000001Eh
STATUS_BEGINNING_OF_MEDIA        equ 8000001Fh
STATUS_MEDIA_CHECK               equ 80000020h
STATUS_SETMARK_DETECTED          equ 80000021h
STATUS_NO_DATA_DETECTED          equ 80000022h
STATUS_REDIRECTOR_HAS_OPEN_HANDLES equ 80000023h
STATUS_SERVER_HAS_OPEN_HANDLES   equ 80000024h
STATUS_ALREADY_DISCONNECTED      equ 80000025h
STATUS_LONGJUMP                  equ 80000026h
STATUS_CLEANER_CARTRIDGE_INSTALLED equ 80000027h
STATUS_PLUGPLAY_QUERY_VETOED     equ 80000028h
STATUS_UNWIND_CONSOLIDATE        equ 80000029h
STATUS_REGISTRY_HIVE_RECOVERED   equ 8000002Ah
STATUS_DLL_MIGHT_BE_INSECURE     equ 8000002Bh
STATUS_DLL_MIGHT_BE_INCOMPATIBLE equ 8000002Ch
STATUS_CLUSTER_NODE_ALREADY_UP   equ 80130001h
STATUS_CLUSTER_NODE_ALREADY_DOWN equ 80130002h
STATUS_CLUSTER_NETWORK_ALREADY_ONLINE equ 80130003h
STATUS_CLUSTER_NETWORK_ALREADY_OFFLINE equ 80130004h
STATUS_CLUSTER_NODE_ALREADY_MEMBER equ 80130005h
STATUS_UNSUCCESSFUL              equ 0C0000001h
STATUS_NOT_IMPLEMENTED           equ 0C0000002h
STATUS_INVALID_INFO_CLASS        equ 0C0000003h
STATUS_INFO_LENGTH_MISMATCH      equ 0C0000004h
STATUS_PAGEFILE_QUOTA            equ 0C0000007h
STATUS_BAD_INITIAL_STACK         equ 0C0000009h
STATUS_BAD_INITIAL_PC            equ 0C000000Ah
STATUS_INVALID_CID               equ 0C000000Bh
STATUS_TIMER_NOT_CANCELED        equ 0C000000Ch
STATUS_INVALID_PARAMETER         equ 0C000000Dh
STATUS_NO_SUCH_DEVICE            equ 0C000000Eh
STATUS_NO_SUCH_FILE              equ 0C000000Fh
STATUS_INVALID_DEVICE_REQUEST    equ 0C0000010h
STATUS_END_OF_FILE               equ 0C0000011h
STATUS_WRONG_VOLUME              equ 0C0000012h
STATUS_NO_MEDIA_IN_DEVICE        equ 0C0000013h
STATUS_UNRECOGNIZED_MEDIA        equ 0C0000014h
STATUS_NONEXISTENT_SECTOR        equ 0C0000015h
STATUS_MORE_PROCESSING_REQUIRED  equ 0C0000016h
STATUS_CONFLICTING_ADDRESSES     equ 0C0000018h
STATUS_NOT_MAPPED_VIEW           equ 0C0000019h
STATUS_UNABLE_TO_FREE_VM         equ 0C000001Ah
STATUS_UNABLE_TO_DELETE_SECTION  equ 0C000001Bh
STATUS_INVALID_SYSTEM_SERVICE    equ 0C000001Ch
STATUS_INVALID_LOCK_SEQUENCE     equ 0C000001Eh
STATUS_INVALID_VIEW_SIZE         equ 0C000001Fh
STATUS_INVALID_FILE_FOR_SECTION  equ 0C0000020h
STATUS_ALREADY_COMMITTED         equ 0C0000021h
STATUS_ACCESS_DENIED             equ 0C0000022h
STATUS_BUFFER_TOO_SMALL          equ 0C0000023h
STATUS_OBJECT_TYPE_MISMATCH      equ 0C0000024h
STATUS_UNWIND                    equ 0C0000027h
STATUS_BAD_STACK                 equ 0C0000028h
STATUS_INVALID_UNWIND_TARGET     equ 0C0000029h
STATUS_NOT_LOCKED                equ 0C000002Ah
STATUS_PARITY_ERROR              equ 0C000002Bh
STATUS_UNABLE_TO_DECOMMIT_VM     equ 0C000002Ch
STATUS_NOT_COMMITTED             equ 0C000002Dh
STATUS_INVALID_PORT_ATTRIBUTES   equ 0C000002Eh
STATUS_PORT_MESSAGE_TOO_LONG     equ 0C000002Fh
STATUS_INVALID_PARAMETER_MIX     equ 0C0000030h
STATUS_INVALID_QUOTA_LOWER       equ 0C0000031h
STATUS_DISK_CORRUPT_ERROR        equ 0C0000032h
STATUS_OBJECT_NAME_INVALID       equ 0C0000033h
STATUS_OBJECT_NAME_NOT_FOUND     equ 0C0000034h
STATUS_OBJECT_NAME_COLLISION     equ 0C0000035h
STATUS_PORT_DISCONNECTED         equ 0C0000037h
STATUS_DEVICE_ALREADY_ATTACHED   equ 0C0000038h
STATUS_OBJECT_PATH_INVALID       equ 0C0000039h
STATUS_OBJECT_PATH_NOT_FOUND     equ 0C000003Ah
STATUS_OBJECT_PATH_SYNTAX_BAD    equ 0C000003Bh
STATUS_DATA_OVERRUN              equ 0C000003Ch
STATUS_DATA_LATE_ERROR           equ 0C000003Dh
STATUS_DATA_ERROR                equ 0C000003Eh
STATUS_CRC_ERROR                 equ 0C000003Fh
STATUS_SECTION_TOO_BIG           equ 0C0000040h
STATUS_PORT_CONNECTION_REFUSED   equ 0C0000041h
STATUS_INVALID_PORT_HANDLE       equ 0C0000042h
STATUS_SHARING_VIOLATION         equ 0C0000043h
STATUS_QUOTA_EXCEEDED            equ 0C0000044h
STATUS_INVALID_PAGE_PROTECTION   equ 0C0000045h
STATUS_MUTANT_NOT_OWNED          equ 0C0000046h
STATUS_SEMAPHORE_LIMIT_EXCEEDED  equ 0C0000047h
STATUS_PORT_ALREADY_SET          equ 0C0000048h
STATUS_SECTION_NOT_IMAGE         equ 0C0000049h
STATUS_SUSPEND_COUNT_EXCEEDED    equ 0C000004Ah
STATUS_THREAD_IS_TERMINATING     equ 0C000004Bh
STATUS_BAD_WORKING_SET_LIMIT     equ 0C000004Ch
STATUS_INCOMPATIBLE_FILE_MAP     equ 0C000004Dh
STATUS_SECTION_PROTECTION        equ 0C000004Eh
STATUS_EAS_NOT_SUPPORTED         equ 0C000004Fh
STATUS_EA_TOO_LARGE              equ 0C0000050h
STATUS_NONEXISTENT_EA_ENTRY      equ 0C0000051h
STATUS_NO_EAS_ON_FILE            equ 0C0000052h
STATUS_EA_CORRUPT_ERROR          equ 0C0000053h
STATUS_FILE_LOCK_CONFLICT        equ 0C0000054h
STATUS_LOCK_NOT_GRANTED          equ 0C0000055h
STATUS_DELETE_PENDING            equ 0C0000056h
STATUS_CTL_FILE_NOT_SUPPORTED    equ 0C0000057h
STATUS_UNKNOWN_REVISION          equ 0C0000058h
STATUS_REVISION_MISMATCH         equ 0C0000059h
STATUS_INVALID_OWNER             equ 0C000005Ah
STATUS_INVALID_PRIMARY_GROUP     equ 0C000005Bh
STATUS_NO_IMPERSONATION_TOKEN    equ 0C000005Ch
STATUS_CANT_DISABLE_MANDATORY    equ 0C000005Dh
STATUS_NO_LOGON_SERVERS          equ 0C000005Eh
STATUS_NO_SUCH_LOGON_SESSION     equ 0C000005Fh
STATUS_NO_SUCH_PRIVILEGE         equ 0C0000060h
STATUS_PRIVILEGE_NOT_HELD        equ 0C0000061h
STATUS_INVALID_ACCOUNT_NAME      equ 0C0000062h
STATUS_USER_EXISTS               equ 0C0000063h
STATUS_NO_SUCH_USER              equ 0C0000064h
STATUS_GROUP_EXISTS              equ 0C0000065h
STATUS_NO_SUCH_GROUP             equ 0C0000066h
STATUS_MEMBER_IN_GROUP           equ 0C0000067h
STATUS_MEMBER_NOT_IN_GROUP       equ 0C0000068h
STATUS_LAST_ADMIN                equ 0C0000069h
STATUS_WRONG_PASSWORD            equ 0C000006Ah
STATUS_ILL_FORMED_PASSWORD       equ 0C000006Bh
STATUS_PASSWORD_RESTRICTION      equ 0C000006Ch
STATUS_LOGON_FAILURE             equ 0C000006Dh
STATUS_ACCOUNT_RESTRICTION       equ 0C000006Eh
STATUS_INVALID_LOGON_HOURS       equ 0C000006Fh
STATUS_INVALID_WORKSTATION       equ 0C0000070h
STATUS_PASSWORD_EXPIRED          equ 0C0000071h
STATUS_ACCOUNT_DISABLED          equ 0C0000072h
STATUS_NONE_MAPPED               equ 0C0000073h
STATUS_TOO_MANY_LUIDS_REQUESTED  equ 0C0000074h
STATUS_LUIDS_EXHAUSTED           equ 0C0000075h
STATUS_INVALID_SUB_AUTHORITY     equ 0C0000076h
STATUS_INVALID_ACL               equ 0C0000077h
STATUS_INVALID_SID               equ 0C0000078h
STATUS_INVALID_SECURITY_DESCR    equ 0C0000079h
STATUS_PROCEDURE_NOT_FOUND       equ 0C000007Ah
STATUS_INVALID_IMAGE_FORMAT      equ 0C000007Bh
STATUS_NO_TOKEN                  equ 0C000007Ch
STATUS_BAD_INHERITANCE_ACL       equ 0C000007Dh
STATUS_RANGE_NOT_LOCKED          equ 0C000007Eh
STATUS_DISK_FULL                 equ 0C000007Fh
STATUS_SERVER_DISABLED           equ 0C0000080h
STATUS_SERVER_NOT_DISABLED       equ 0C0000081h
STATUS_TOO_MANY_GUIDS_REQUESTED  equ 0C0000082h
STATUS_GUIDS_EXHAUSTED           equ 0C0000083h
STATUS_INVALID_ID_AUTHORITY      equ 0C0000084h
STATUS_AGENTS_EXHAUSTED          equ 0C0000085h
STATUS_INVALID_VOLUME_LABEL      equ 0C0000086h
STATUS_SECTION_NOT_EXTENDED      equ 0C0000087h
STATUS_NOT_MAPPED_DATA           equ 0C0000088h
STATUS_RESOURCE_DATA_NOT_FOUND   equ 0C0000089h
STATUS_RESOURCE_TYPE_NOT_FOUND   equ 0C000008Ah
STATUS_RESOURCE_NAME_NOT_FOUND   equ 0C000008Bh
STATUS_TOO_MANY_PAGING_FILES     equ 0C0000097h
STATUS_FILE_INVALID              equ 0C0000098h
STATUS_ALLOTTED_SPACE_EXCEEDED   equ 0C0000099h
STATUS_INSUFFICIENT_RESOURCES    equ 0C000009Ah
STATUS_DFS_EXIT_PATH_FOUND       equ 0C000009Bh
STATUS_DEVICE_DATA_ERROR         equ 0C000009Ch
STATUS_DEVICE_NOT_CONNECTED      equ 0C000009Dh
STATUS_DEVICE_POWER_FAILURE      equ 0C000009Eh
STATUS_FREE_VM_NOT_AT_BASE       equ 0C000009Fh
STATUS_MEMORY_NOT_ALLOCATED      equ 0C00000A0h
STATUS_WORKING_SET_QUOTA         equ 0C00000A1h
STATUS_MEDIA_WRITE_PROTECTED     equ 0C00000A2h
STATUS_DEVICE_NOT_READY          equ 0C00000A3h
STATUS_INVALID_GROUP_ATTRIBUTES  equ 0C00000A4h
STATUS_BAD_IMPERSONATION_LEVEL   equ 0C00000A5h
STATUS_CANT_OPEN_ANONYMOUS       equ 0C00000A6h
STATUS_BAD_VALIDATION_CLASS      equ 0C00000A7h
STATUS_BAD_TOKEN_TYPE            equ 0C00000A8h
STATUS_BAD_MASTER_BOOT_RECORD    equ 0C00000A9h
STATUS_INSTRUCTION_MISALIGNMENT  equ 0C00000AAh
STATUS_INSTANCE_NOT_AVAILABLE    equ 0C00000ABh
STATUS_PIPE_NOT_AVAILABLE        equ 0C00000ACh
STATUS_INVALID_PIPE_STATE        equ 0C00000ADh
STATUS_PIPE_BUSY                 equ 0C00000AEh
STATUS_ILLEGAL_FUNCTION          equ 0C00000AFh
STATUS_PIPE_DISCONNECTED         equ 0C00000B0h
STATUS_PIPE_CLOSING              equ 0C00000B1h
STATUS_PIPE_CONNECTED            equ 0C00000B2h
STATUS_PIPE_LISTENING            equ 0C00000B3h
STATUS_INVALID_READ_MODE         equ 0C00000B4h
STATUS_IO_TIMEOUT                equ 0C00000B5h
STATUS_FILE_FORCED_CLOSED        equ 0C00000B6h
STATUS_PROFILING_NOT_STARTED     equ 0C00000B7h
STATUS_PROFILING_NOT_STOPPED     equ 0C00000B8h
STATUS_COULD_NOT_INTERPRET       equ 0C00000B9h
STATUS_FILE_IS_A_DIRECTORY       equ 0C00000BAh
STATUS_NOT_SUPPORTED             equ 0C00000BBh
STATUS_REMOTE_NOT_LISTENING      equ 0C00000BCh
STATUS_DUPLICATE_NAME            equ 0C00000BDh
STATUS_BAD_NETWORK_PATH          equ 0C00000BEh
STATUS_NETWORK_BUSY              equ 0C00000BFh
STATUS_DEVICE_DOES_NOT_EXIST     equ 0C00000C0h
STATUS_TOO_MANY_COMMANDS         equ 0C00000C1h
STATUS_ADAPTER_HARDWARE_ERROR    equ 0C00000C2h
STATUS_INVALID_NETWORK_RESPONSE  equ 0C00000C3h
STATUS_UNEXPECTED_NETWORK_ERROR  equ 0C00000C4h
STATUS_BAD_REMOTE_ADAPTER        equ 0C00000C5h
STATUS_PRINT_QUEUE_FULL          equ 0C00000C6h
STATUS_NO_SPOOL_SPACE            equ 0C00000C7h
STATUS_PRINT_CANCELLED           equ 0C00000C8h
STATUS_NETWORK_NAME_DELETED      equ 0C00000C9h
STATUS_NETWORK_ACCESS_DENIED     equ 0C00000CAh
STATUS_BAD_DEVICE_TYPE           equ 0C00000CBh
STATUS_BAD_NETWORK_NAME          equ 0C00000CCh
STATUS_TOO_MANY_NAMES            equ 0C00000CDh
STATUS_TOO_MANY_SESSIONS         equ 0C00000CEh
STATUS_SHARING_PAUSED            equ 0C00000CFh
STATUS_REQUEST_NOT_ACCEPTED      equ 0C00000D0h
STATUS_REDIRECTOR_PAUSED         equ 0C00000D1h
STATUS_NET_WRITE_FAULT           equ 0C00000D2h
STATUS_PROFILING_AT_LIMIT        equ 0C00000D3h
STATUS_NOT_SAME_DEVICE           equ 0C00000D4h
STATUS_FILE_RENAMED              equ 0C00000D5h
STATUS_VIRTUAL_CIRCUIT_CLOSED    equ 0C00000D6h
STATUS_NO_SECURITY_ON_OBJECT     equ 0C00000D7h
STATUS_CANT_WAIT                 equ 0C00000D8h
STATUS_PIPE_EMPTY                equ 0C00000D9h
STATUS_CANT_ACCESS_DOMAIN_INFO   equ 0C00000DAh
STATUS_CANT_TERMINATE_SELF       equ 0C00000DBh
STATUS_INVALID_SERVER_STATE      equ 0C00000DCh
STATUS_INVALID_DOMAIN_STATE      equ 0C00000DDh
STATUS_INVALID_DOMAIN_ROLE       equ 0C00000DEh
STATUS_NO_SUCH_DOMAIN            equ 0C00000DFh
STATUS_DOMAIN_EXISTS             equ 0C00000E0h
STATUS_DOMAIN_LIMIT_EXCEEDED     equ 0C00000E1h
STATUS_OPLOCK_NOT_GRANTED        equ 0C00000E2h
STATUS_INVALID_OPLOCK_PROTOCOL   equ 0C00000E3h
STATUS_INTERNAL_DB_CORRUPTION    equ 0C00000E4h
STATUS_INTERNAL_ERROR            equ 0C00000E5h
STATUS_GENERIC_NOT_MAPPED        equ 0C00000E6h
STATUS_BAD_DESCRIPTOR_FORMAT     equ 0C00000E7h
STATUS_INVALID_USER_BUFFER       equ 0C00000E8h
STATUS_UNEXPECTED_IO_ERROR       equ 0C00000E9h
STATUS_UNEXPECTED_MM_CREATE_ERR  equ 0C00000EAh
STATUS_UNEXPECTED_MM_MAP_ERROR   equ 0C00000EBh
STATUS_UNEXPECTED_MM_EXTEND_ERR  equ 0C00000ECh
STATUS_NOT_LOGON_PROCESS         equ 0C00000EDh
STATUS_LOGON_SESSION_EXISTS      equ 0C00000EEh
STATUS_INVALID_PARAMETER_1       equ 0C00000EFh
STATUS_INVALID_PARAMETER_2       equ 0C00000F0h
STATUS_INVALID_PARAMETER_3       equ 0C00000F1h
STATUS_INVALID_PARAMETER_4       equ 0C00000F2h
STATUS_INVALID_PARAMETER_5       equ 0C00000F3h
STATUS_INVALID_PARAMETER_6       equ 0C00000F4h
STATUS_INVALID_PARAMETER_7       equ 0C00000F5h
STATUS_INVALID_PARAMETER_8       equ 0C00000F6h
STATUS_INVALID_PARAMETER_9       equ 0C00000F7h
STATUS_INVALID_PARAMETER_10      equ 0C00000F8h
STATUS_INVALID_PARAMETER_11      equ 0C00000F9h
STATUS_INVALID_PARAMETER_12      equ 0C00000FAh
STATUS_REDIRECTOR_NOT_STARTED    equ 0C00000FBh
STATUS_REDIRECTOR_STARTED        equ 0C00000FCh
STATUS_NO_SUCH_PACKAGE           equ 0C00000FEh
STATUS_BAD_FUNCTION_TABLE        equ 0C00000FFh
STATUS_VARIABLE_NOT_FOUND        equ 0C0000100h
STATUS_DIRECTORY_NOT_EMPTY       equ 0C0000101h
STATUS_FILE_CORRUPT_ERROR        equ 0C0000102h
STATUS_NOT_A_DIRECTORY           equ 0C0000103h
STATUS_BAD_LOGON_SESSION_STATE   equ 0C0000104h
STATUS_LOGON_SESSION_COLLISION   equ 0C0000105h
STATUS_NAME_TOO_LONG             equ 0C0000106h
STATUS_FILES_OPEN                equ 0C0000107h
STATUS_CONNECTION_IN_USE         equ 0C0000108h
STATUS_MESSAGE_NOT_FOUND         equ 0C0000109h
STATUS_PROCESS_IS_TERMINATING    equ 0C000010Ah
STATUS_INVALID_LOGON_TYPE        equ 0C000010Bh
STATUS_NO_GUID_TRANSLATION       equ 0C000010Ch
STATUS_CANNOT_IMPERSONATE        equ 0C000010Dh
STATUS_IMAGE_ALREADY_LOADED      equ 0C000010Eh
STATUS_ABIOS_NOT_PRESENT         equ 0C000010Fh
STATUS_ABIOS_LID_NOT_EXIST       equ 0C0000110h
STATUS_ABIOS_LID_ALREADY_OWNED   equ 0C0000111h
STATUS_ABIOS_NOT_LID_OWNER       equ 0C0000112h
STATUS_ABIOS_INVALID_COMMAND     equ 0C0000113h
STATUS_ABIOS_INVALID_LID         equ 0C0000114h
STATUS_ABIOS_SELECTOR_NOT_AVAILABLE equ 0C0000115h
STATUS_ABIOS_INVALID_SELECTOR    equ 0C0000116h
STATUS_NO_LDT                    equ 0C0000117h
STATUS_INVALID_LDT_SIZE          equ 0C0000118h
STATUS_INVALID_LDT_OFFSET        equ 0C0000119h
STATUS_INVALID_LDT_DESCRIPTOR    equ 0C000011Ah
STATUS_INVALID_IMAGE_NE_FORMAT   equ 0C000011Bh
STATUS_RXACT_INVALID_STATE       equ 0C000011Ch
STATUS_RXACT_COMMIT_FAILURE      equ 0C000011Dh
STATUS_MAPPED_FILE_SIZE_ZERO     equ 0C000011Eh
STATUS_TOO_MANY_OPENED_FILES     equ 0C000011Fh
STATUS_CANCELLED                 equ 0C0000120h
STATUS_CANNOT_DELETE             equ 0C0000121h
STATUS_INVALID_COMPUTER_NAME     equ 0C0000122h
STATUS_FILE_DELETED              equ 0C0000123h
STATUS_SPECIAL_ACCOUNT           equ 0C0000124h
STATUS_SPECIAL_GROUP             equ 0C0000125h
STATUS_SPECIAL_USER              equ 0C0000126h
STATUS_MEMBERS_PRIMARY_GROUP     equ 0C0000127h
STATUS_FILE_CLOSED               equ 0C0000128h
STATUS_TOO_MANY_THREADS          equ 0C0000129h
STATUS_THREAD_NOT_IN_PROCESS     equ 0C000012Ah
STATUS_TOKEN_ALREADY_IN_USE      equ 0C000012Bh
STATUS_PAGEFILE_QUOTA_EXCEEDED   equ 0C000012Ch
STATUS_COMMITMENT_LIMIT          equ 0C000012Dh
STATUS_INVALID_IMAGE_LE_FORMAT   equ 0C000012Eh
STATUS_INVALID_IMAGE_NOT_MZ      equ 0C000012Fh
STATUS_INVALID_IMAGE_PROTECT     equ 0C0000130h
STATUS_INVALID_IMAGE_WIN_16      equ 0C0000131h
STATUS_LOGON_SERVER_CONFLICT     equ 0C0000132h
STATUS_TIME_DIFFERENCE_AT_DC     equ 0C0000133h
STATUS_SYNCHRONIZATION_REQUIRED  equ 0C0000134h
STATUS_DLL_NOT_FOUND             equ 0C0000135h
STATUS_OPEN_FAILED               equ 0C0000136h
STATUS_IO_PRIVILEGE_FAILED       equ 0C0000137h
STATUS_ORDINAL_NOT_FOUND         equ 0C0000138h
STATUS_ENTRYPOINT_NOT_FOUND      equ 0C0000139h
STATUS_LOCAL_DISCONNECT          equ 0C000013Bh
STATUS_REMOTE_DISCONNECT         equ 0C000013Ch
STATUS_REMOTE_RESOURCES          equ 0C000013Dh
STATUS_LINK_FAILED               equ 0C000013Eh
STATUS_LINK_TIMEOUT              equ 0C000013Fh
STATUS_INVALID_CONNECTION        equ 0C0000140h
STATUS_INVALID_ADDRESS           equ 0C0000141h
STATUS_DLL_INIT_FAILED           equ 0C0000142h
STATUS_MISSING_SYSTEMFILE        equ 0C0000143h
STATUS_UNHANDLED_EXCEPTION       equ 0C0000144h
STATUS_APP_INIT_FAILURE          equ 0C0000145h
STATUS_PAGEFILE_CREATE_FAILED    equ 0C0000146h
STATUS_NO_PAGEFILE               equ 0C0000147h
STATUS_INVALID_LEVEL             equ 0C0000148h
STATUS_WRONG_PASSWORD_CORE       equ 0C0000149h
STATUS_ILLEGAL_FLOAT_CONTEXT     equ 0C000014Ah
STATUS_PIPE_BROKEN               equ 0C000014Bh
STATUS_REGISTRY_CORRUPT          equ 0C000014Ch
STATUS_REGISTRY_IO_FAILED        equ 0C000014Dh
STATUS_NO_EVENT_PAIR             equ 0C000014Eh
STATUS_UNRECOGNIZED_VOLUME       equ 0C000014Fh
STATUS_SERIAL_NO_DEVICE_INITED   equ 0C0000150h
STATUS_NO_SUCH_ALIAS             equ 0C0000151h
STATUS_MEMBER_NOT_IN_ALIAS       equ 0C0000152h
STATUS_MEMBER_IN_ALIAS           equ 0C0000153h
STATUS_ALIAS_EXISTS              equ 0C0000154h
STATUS_LOGON_NOT_GRANTED         equ 0C0000155h
STATUS_TOO_MANY_SECRETS          equ 0C0000156h
STATUS_SECRET_TOO_LONG           equ 0C0000157h
STATUS_INTERNAL_DB_ERROR         equ 0C0000158h
STATUS_FULLSCREEN_MODE           equ 0C0000159h
STATUS_TOO_MANY_CONTEXT_IDS      equ 0C000015Ah
STATUS_LOGON_TYPE_NOT_GRANTED    equ 0C000015Bh
STATUS_NOT_REGISTRY_FILE         equ 0C000015Ch
STATUS_NT_CROSS_ENCRYPTION_REQUIRED equ 0C000015Dh
STATUS_DOMAIN_CTRLR_CONFIG_ERROR equ 0C000015Eh
STATUS_FT_MISSING_MEMBER         equ 0C000015Fh
STATUS_ILL_FORMED_SERVICE_ENTRY  equ 0C0000160h
STATUS_ILLEGAL_CHARACTER         equ 0C0000161h
STATUS_UNMAPPABLE_CHARACTER      equ 0C0000162h
STATUS_UNDEFINED_CHARACTER       equ 0C0000163h
STATUS_FLOPPY_VOLUME             equ 0C0000164h
STATUS_FLOPPY_ID_MARK_NOT_FOUND  equ 0C0000165h
STATUS_FLOPPY_WRONG_CYLINDER     equ 0C0000166h
STATUS_FLOPPY_UNKNOWN_ERROR      equ 0C0000167h
STATUS_FLOPPY_BAD_REGISTERS      equ 0C0000168h
STATUS_DISK_RECALIBRATE_FAILED   equ 0C0000169h
STATUS_DISK_OPERATION_FAILED     equ 0C000016Ah
STATUS_DISK_RESET_FAILED         equ 0C000016Bh
STATUS_SHARED_IRQ_BUSY           equ 0C000016Ch
STATUS_FT_ORPHANING              equ 0C000016Dh
STATUS_BIOS_FAILED_TO_CONNECT_INTERRUPT equ 0C000016Eh
STATUS_PARTITION_FAILURE         equ 0C0000172h
STATUS_INVALID_BLOCK_LENGTH      equ 0C0000173h
STATUS_DEVICE_NOT_PARTITIONED    equ 0C0000174h
STATUS_UNABLE_TO_LOCK_MEDIA      equ 0C0000175h
STATUS_UNABLE_TO_UNLOAD_MEDIA    equ 0C0000176h
STATUS_EOM_OVERFLOW              equ 0C0000177h
STATUS_NO_MEDIA                  equ 0C0000178h
STATUS_NO_SUCH_MEMBER            equ 0C000017Ah
STATUS_INVALID_MEMBER            equ 0C000017Bh
STATUS_KEY_DELETED               equ 0C000017Ch
STATUS_NO_LOG_SPACE              equ 0C000017Dh
STATUS_TOO_MANY_SIDS             equ 0C000017Eh
STATUS_LM_CROSS_ENCRYPTION_REQUIRED equ 0C000017Fh
STATUS_KEY_HAS_CHILDREN          equ 0C0000180h
STATUS_CHILD_MUST_BE_VOLATILE    equ 0C0000181h
STATUS_DEVICE_CONFIGURATION_ERROR equ 0C0000182h
STATUS_DRIVER_INTERNAL_ERROR     equ 0C0000183h
STATUS_INVALID_DEVICE_STATE      equ 0C0000184h
STATUS_IO_DEVICE_ERROR           equ 0C0000185h
STATUS_DEVICE_PROTOCOL_ERROR     equ 0C0000186h
STATUS_BACKUP_CONTROLLER         equ 0C0000187h
STATUS_LOG_FILE_FULL             equ 0C0000188h
STATUS_TOO_LATE                  equ 0C0000189h
STATUS_NO_TRUST_LSA_SECRET       equ 0C000018Ah
STATUS_NO_TRUST_SAM_ACCOUNT      equ 0C000018Bh
STATUS_TRUSTED_DOMAIN_FAILURE    equ 0C000018Ch
STATUS_TRUSTED_RELATIONSHIP_FAILURE equ 0C000018Dh
STATUS_EVENTLOG_FILE_CORRUPT     equ 0C000018Eh
STATUS_EVENTLOG_CANT_START       equ 0C000018Fh
STATUS_TRUST_FAILURE             equ 0C0000190h
STATUS_MUTANT_LIMIT_EXCEEDED     equ 0C0000191h
STATUS_NETLOGON_NOT_STARTED      equ 0C0000192h
STATUS_ACCOUNT_EXPIRED           equ 0C0000193h
STATUS_POSSIBLE_DEADLOCK         equ 0C0000194h
STATUS_NETWORK_CREDENTIAL_CONFLICT equ 0C0000195h
STATUS_REMOTE_SESSION_LIMIT      equ 0C0000196h
STATUS_EVENTLOG_FILE_CHANGED     equ 0C0000197h
STATUS_NOLOGON_INTERDOMAIN_TRUST_ACCOUNT equ 0C0000198h
STATUS_NOLOGON_WORKSTATION_TRUST_ACCOUNT equ 0C0000199h
STATUS_NOLOGON_SERVER_TRUST_ACCOUNT equ 0C000019Ah
STATUS_DOMAIN_TRUST_INCONSISTENT equ 0C000019Bh
STATUS_FS_DRIVER_REQUIRED        equ 0C000019Ch
STATUS_NO_USER_SESSION_KEY       equ 0C0000202h
STATUS_USER_SESSION_DELETED      equ 0C0000203h
STATUS_RESOURCE_LANG_NOT_FOUND   equ 0C0000204h
STATUS_INSUFF_SERVER_RESOURCES   equ 0C0000205h
STATUS_INVALID_BUFFER_SIZE       equ 0C0000206h
STATUS_INVALID_ADDRESS_COMPONENT equ 0C0000207h
STATUS_INVALID_ADDRESS_WILDCARD  equ 0C0000208h
STATUS_TOO_MANY_ADDRESSES        equ 0C0000209h
STATUS_ADDRESS_ALREADY_EXISTS    equ 0C000020Ah
STATUS_ADDRESS_CLOSED            equ 0C000020Bh
STATUS_CONNECTION_DISCONNECTED   equ 0C000020Ch
STATUS_CONNECTION_RESET          equ 0C000020Dh
STATUS_TOO_MANY_NODES            equ 0C000020Eh
STATUS_TRANSACTION_ABORTED       equ 0C000020Fh
STATUS_TRANSACTION_TIMED_OUT     equ 0C0000210h
STATUS_TRANSACTION_NO_RELEASE    equ 0C0000211h
STATUS_TRANSACTION_NO_MATCH      equ 0C0000212h
STATUS_TRANSACTION_RESPONDED     equ 0C0000213h
STATUS_TRANSACTION_INVALID_ID    equ 0C0000214h
STATUS_TRANSACTION_INVALID_TYPE  equ 0C0000215h
STATUS_NOT_SERVER_SESSION        equ 0C0000216h
STATUS_NOT_CLIENT_SESSION        equ 0C0000217h
STATUS_CANNOT_LOAD_REGISTRY_FILE equ 0C0000218h
STATUS_DEBUG_ATTACH_FAILED       equ 0C0000219h
STATUS_SYSTEM_PROCESS_TERMINATED equ 0C000021Ah
STATUS_DATA_NOT_ACCEPTED         equ 0C000021Bh
STATUS_NO_BROWSER_SERVERS_FOUND  equ 0C000021Ch
STATUS_VDM_HARD_ERROR            equ 0C000021Dh
STATUS_DRIVER_CANCEL_TIMEOUT     equ 0C000021Eh
STATUS_REPLY_MESSAGE_MISMATCH    equ 0C000021Fh
STATUS_MAPPED_ALIGNMENT          equ 0C0000220h
STATUS_IMAGE_CHECKSUM_MISMATCH   equ 0C0000221h
STATUS_LOST_WRITEBEHIND_DATA     equ 0C0000222h
STATUS_CLIENT_SERVER_PARAMETERS_INVALID equ 0C0000223h
STATUS_PASSWORD_MUST_CHANGE      equ 0C0000224h
STATUS_NOT_FOUND                 equ 0C0000225h
STATUS_NOT_TINY_STREAM           equ 0C0000226h
STATUS_RECOVERY_FAILURE          equ 0C0000227h
STATUS_STACK_OVERFLOW_READ       equ 0C0000228h
STATUS_FAIL_CHECK                equ 0C0000229h
STATUS_DUPLICATE_OBJECTID        equ 0C000022Ah
STATUS_OBJECTID_EXISTS           equ 0C000022Bh
STATUS_CONVERT_TO_LARGE          equ 0C000022Ch
STATUS_RETRY                     equ 0C000022Dh
STATUS_FOUND_OUT_OF_SCOPE        equ 0C000022Eh
STATUS_ALLOCATE_BUCKET           equ 0C000022Fh
STATUS_PROPSET_NOT_FOUND         equ 0C0000230h
STATUS_MARSHALL_OVERFLOW         equ 0C0000231h
STATUS_INVALID_VARIANT           equ 0C0000232h
STATUS_DOMAIN_CONTROLLER_NOT_FOUND equ 0C0000233h
STATUS_ACCOUNT_LOCKED_OUT        equ 0C0000234h
STATUS_HANDLE_NOT_CLOSABLE       equ 0C0000235h
STATUS_CONNECTION_REFUSED        equ 0C0000236h
STATUS_GRACEFUL_DISCONNECT       equ 0C0000237h
STATUS_ADDRESS_ALREADY_ASSOCIATED equ 0C0000238h
STATUS_ADDRESS_NOT_ASSOCIATED    equ 0C0000239h
STATUS_CONNECTION_INVALID        equ 0C000023Ah
STATUS_CONNECTION_ACTIVE         equ 0C000023Bh
STATUS_NETWORK_UNREACHABLE       equ 0C000023Ch
STATUS_HOST_UNREACHABLE          equ 0C000023Dh
STATUS_PROTOCOL_UNREACHABLE      equ 0C000023Eh
STATUS_PORT_UNREACHABLE          equ 0C000023Fh
STATUS_REQUEST_ABORTED           equ 0C0000240h
STATUS_CONNECTION_ABORTED        equ 0C0000241h
STATUS_BAD_COMPRESSION_BUFFER    equ 0C0000242h
STATUS_USER_MAPPED_FILE          equ 0C0000243h
STATUS_AUDIT_FAILED              equ 0C0000244h
STATUS_TIMER_RESOLUTION_NOT_SET  equ 0C0000245h
STATUS_CONNECTION_COUNT_LIMIT    equ 0C0000246h
STATUS_LOGIN_TIME_RESTRICTION    equ 0C0000247h
STATUS_LOGIN_WKSTA_RESTRICTION   equ 0C0000248h
STATUS_IMAGE_MP_UP_MISMATCH      equ 0C0000249h
STATUS_INSUFFICIENT_LOGON_INFO   equ 0C0000250h
STATUS_BAD_DLL_ENTRYPOINT        equ 0C0000251h
STATUS_BAD_SERVICE_ENTRYPOINT    equ 0C0000252h
STATUS_LPC_REPLY_LOST            equ 0C0000253h
STATUS_IP_ADDRESS_CONFLICT1      equ 0C0000254h
STATUS_IP_ADDRESS_CONFLICT2      equ 0C0000255h
STATUS_REGISTRY_QUOTA_LIMIT      equ 0C0000256h
STATUS_PATH_NOT_COVERED          equ 0C0000257h
STATUS_NO_CALLBACK_ACTIVE        equ 0C0000258h
STATUS_LICENSE_QUOTA_EXCEEDED    equ 0C0000259h
STATUS_PWD_TOO_SHORT             equ 0C000025Ah
STATUS_PWD_TOO_RECENT            equ 0C000025Bh
STATUS_PWD_HISTORY_CONFLICT      equ 0C000025Ch
STATUS_PLUGPLAY_NO_DEVICE        equ 0C000025Eh
STATUS_UNSUPPORTED_COMPRESSION   equ 0C000025Fh
STATUS_INVALID_HW_PROFILE        equ 0C0000260h
STATUS_INVALID_PLUGPLAY_DEVICE_PATH equ 0C0000261h
STATUS_DRIVER_ORDINAL_NOT_FOUND  equ 0C0000262h
STATUS_DRIVER_ENTRYPOINT_NOT_FOUND equ 0C0000263h
STATUS_RESOURCE_NOT_OWNED        equ 0C0000264h
STATUS_TOO_MANY_LINKS            equ 0C0000265h
STATUS_QUOTA_LIST_INCONSISTENT   equ 0C0000266h
STATUS_FILE_IS_OFFLINE           equ 0C0000267h
STATUS_EVALUATION_EXPIRATION     equ 0C0000268h
STATUS_ILLEGAL_DLL_RELOCATION    equ 0C0000269h
STATUS_LICENSE_VIOLATION         equ 0C000026Ah
STATUS_DLL_INIT_FAILED_LOGOFF    equ 0C000026Bh
STATUS_DRIVER_UNABLE_TO_LOAD     equ 0C000026Ch
STATUS_DFS_UNAVAILABLE           equ 0C000026Dh
STATUS_VOLUME_DISMOUNTED         equ 0C000026Eh
STATUS_WX86_INTERNAL_ERROR       equ 0C000026Fh
STATUS_WX86_FLOAT_STACK_CHECK    equ 0C0000270h
STATUS_VALIDATE_CONTINUE         equ 0C0000271h
STATUS_NO_MATCH                  equ 0C0000272h
STATUS_NO_MORE_MATCHES           equ 0C0000273h
STATUS_NOT_A_REPARSE_POINT       equ 0C0000275h
STATUS_IO_REPARSE_TAG_INVALID    equ 0C0000276h
STATUS_IO_REPARSE_TAG_MISMATCH   equ 0C0000277h
STATUS_IO_REPARSE_DATA_INVALID   equ 0C0000278h
STATUS_IO_REPARSE_TAG_NOT_HANDLED equ 0C0000279h
STATUS_REPARSE_POINT_NOT_RESOLVED equ 0C0000280h
STATUS_DIRECTORY_IS_A_REPARSE_POINT equ 0C0000281h
STATUS_RANGE_LIST_CONFLICT       equ 0C0000282h
STATUS_SOURCE_ELEMENT_EMPTY      equ 0C0000283h
STATUS_DESTINATION_ELEMENT_FULL  equ 0C0000284h
STATUS_ILLEGAL_ELEMENT_ADDRESS   equ 0C0000285h
STATUS_MAGAZINE_NOT_PRESENT      equ 0C0000286h
STATUS_REINITIALIZATION_NEEDED   equ 0C0000287h
STATUS_DEVICE_REQUIRES_CLEANING  equ 80000288h
STATUS_DEVICE_DOOR_OPEN          equ 80000289h
STATUS_ENCRYPTION_FAILED         equ 0C000028Ah
STATUS_DECRYPTION_FAILED         equ 0C000028Bh
STATUS_RANGE_NOT_FOUND           equ 0C000028Ch
STATUS_NO_RECOVERY_POLICY        equ 0C000028Dh
STATUS_NO_EFS                    equ 0C000028Eh
STATUS_WRONG_EFS                 equ 0C000028Fh
STATUS_NO_USER_KEYS              equ 0C0000290h
STATUS_FILE_NOT_ENCRYPTED        equ 0C0000291h
STATUS_NOT_EXPORT_FORMAT         equ 0C0000292h
STATUS_FILE_ENCRYPTED            equ 0C0000293h
STATUS_WAKE_SYSTEM               equ 40000294h
STATUS_WMI_GUID_NOT_FOUND        equ 0C0000295h
STATUS_WMI_INSTANCE_NOT_FOUND    equ 0C0000296h
STATUS_WMI_ITEMID_NOT_FOUND      equ 0C0000297h
STATUS_WMI_TRY_AGAIN             equ 0C0000298h
STATUS_SHARED_POLICY             equ 0C0000299h
STATUS_POLICY_OBJECT_NOT_FOUND   equ 0C000029Ah
STATUS_POLICY_ONLY_IN_DS         equ 0C000029Bh
STATUS_VOLUME_NOT_UPGRADED       equ 0C000029Ch
STATUS_REMOTE_STORAGE_NOT_ACTIVE equ 0C000029Dh
STATUS_REMOTE_STORAGE_MEDIA_ERROR equ 0C000029Eh
STATUS_NO_TRACKING_SERVICE       equ 0C000029Fh
STATUS_SERVER_SID_MISMATCH       equ 0C00002A0h
STATUS_DS_NO_ATTRIBUTE_OR_VALUE  equ 0C00002A1h
STATUS_DS_INVALID_ATTRIBUTE_SYNTAX equ 0C00002A2h
STATUS_DS_ATTRIBUTE_TYPE_UNDEFINED equ 0C00002A3h
STATUS_DS_ATTRIBUTE_OR_VALUE_EXISTS equ 0C00002A4h
STATUS_DS_BUSY                   equ 0C00002A5h
STATUS_DS_UNAVAILABLE            equ 0C00002A6h
STATUS_DS_NO_RIDS_ALLOCATED      equ 0C00002A7h
STATUS_DS_NO_MORE_RIDS           equ 0C00002A8h
STATUS_DS_INCORRECT_ROLE_OWNER   equ 0C00002A9h
STATUS_DS_RIDMGR_INIT_ERROR      equ 0C00002AAh
STATUS_DS_OBJ_CLASS_VIOLATION    equ 0C00002ABh
STATUS_DS_CANT_ON_NON_LEAF       equ 0C00002ACh
STATUS_DS_CANT_ON_RDN            equ 0C00002ADh
STATUS_DS_CANT_MOD_OBJ_CLASS     equ 0C00002AEh
STATUS_DS_CROSS_DOM_MOVE_FAILED  equ 0C00002AFh
STATUS_DS_GC_NOT_AVAILABLE       equ 0C00002B0h
STATUS_DIRECTORY_SERVICE_REQUIRED equ 0C00002B1h
STATUS_REPARSE_ATTRIBUTE_CONFLICT equ 0C00002B2h
STATUS_CANT_ENABLE_DENY_ONLY     equ 0C00002B3h
STATUS_DEVICE_REMOVED            equ 0C00002B6h
STATUS_JOURNAL_DELETE_IN_PROGRESS equ 0C00002B7h
STATUS_JOURNAL_NOT_ACTIVE        equ 0C00002B8h
STATUS_NOINTERFACE               equ 0C00002B9h
STATUS_DS_ADMIN_LIMIT_EXCEEDED   equ 0C00002C1h
STATUS_DRIVER_FAILED_SLEEP       equ 0C00002C2h
STATUS_MUTUAL_AUTHENTICATION_FAILED equ 0C00002C3h
STATUS_CORRUPT_SYSTEM_FILE       equ 0C00002C4h
STATUS_DATATYPE_MISALIGNMENT_ERROR equ 0C00002C5h
STATUS_WMI_READ_ONLY             equ 0C00002C6h
STATUS_WMI_SET_FAILURE           equ 0C00002C7h
STATUS_COMMITMENT_MINIMUM        equ 0C00002C8h
STATUS_TRANSPORT_FULL            equ 0C00002CAh
STATUS_DS_SAM_INIT_FAILURE       equ 0C00002CBh
STATUS_ONLY_IF_CONNECTED         equ 0C00002CCh
STATUS_DS_SENSITIVE_GROUP_VIOLATION equ 0C00002CDh
STATUS_PNP_RESTART_ENUMERATION   equ 0C00002CEh
STATUS_JOURNAL_ENTRY_DELETED     equ 0C00002CFh
STATUS_DS_CANT_MOD_PRIMARYGROUPID equ 0C00002D0h
STATUS_SYSTEM_IMAGE_BAD_SIGNATURE equ 0C00002D1h
STATUS_PNP_REBOOT_REQUIRED       equ 0C00002D2h
STATUS_POWER_STATE_INVALID       equ 0C00002D3h
STATUS_DS_INVALID_GROUP_TYPE     equ 0C00002D4h
STATUS_DS_NO_NEST_GLOBALGROUP_IN_MIXEDDOMAIN equ 0C00002D5h
STATUS_DS_NO_NEST_LOCALGROUP_IN_MIXEDDOMAIN equ 0C00002D6h
STATUS_DS_GLOBAL_CANT_HAVE_LOCAL_MEMBER equ 0C00002D7h
STATUS_DS_GLOBAL_CANT_HAVE_UNIVERSAL_MEMBER equ 0C00002D8h
STATUS_DS_UNIVERSAL_CANT_HAVE_LOCAL_MEMBER equ 0C00002D9h
STATUS_DS_GLOBAL_CANT_HAVE_CROSSDOMAIN_MEMBER equ 0C00002DAh
STATUS_DS_LOCAL_CANT_HAVE_CROSSDOMAIN_LOCAL_MEMBER equ 0C00002DBh
STATUS_DS_HAVE_PRIMARY_MEMBERS   equ 0C00002DCh
STATUS_WMI_NOT_SUPPORTED         equ 0C00002DDh
STATUS_INSUFFICIENT_POWER        equ 0C00002DEh
STATUS_SAM_NEED_BOOTKEY_PASSWORD equ 0C00002DFh
STATUS_SAM_NEED_BOOTKEY_FLOPPY   equ 0C00002E0h
STATUS_DS_CANT_START             equ 0C00002E1h
STATUS_DS_INIT_FAILURE           equ 0C00002E2h
STATUS_SAM_INIT_FAILURE          equ 0C00002E3h
STATUS_DS_GC_REQUIRED            equ 0C00002E4h
STATUS_DS_LOCAL_MEMBER_OF_LOCAL_ONLY equ 0C00002E5h
STATUS_DS_NO_FPO_IN_UNIVERSAL_GROUPS equ 0C00002E6h
STATUS_DS_MACHINE_ACCOUNT_QUOTA_EXCEEDED equ 0C00002E7h
STATUS_MULTIPLE_FAULT_VIOLATION  equ 0C00002E8h
STATUS_CURRENT_DOMAIN_NOT_ALLOWED equ 0C00002E9h
STATUS_CANNOT_MAKE               equ 0C00002EAh
STATUS_SYSTEM_SHUTDOWN           equ 0C00002EBh
STATUS_DS_INIT_FAILURE_CONSOLE   equ 0C00002ECh
STATUS_DS_SAM_INIT_FAILURE_CONSOLE equ 0C00002EDh
STATUS_UNFINISHED_CONTEXT_DELETED equ 0C00002EEh
STATUS_NO_TGT_REPLY              equ 0C00002EFh
STATUS_OBJECTID_NOT_FOUND        equ 0C00002F0h
STATUS_NO_IP_ADDRESSES           equ 0C00002F1h
STATUS_WRONG_CREDENTIAL_HANDLE   equ 0C00002F2h
STATUS_CRYPTO_SYSTEM_INVALID     equ 0C00002F3h
STATUS_MAX_REFERRALS_EXCEEDED    equ 0C00002F4h
STATUS_MUST_BE_KDC               equ 0C00002F5h
STATUS_STRONG_CRYPTO_NOT_SUPPORTED equ 0C00002F6h
STATUS_TOO_MANY_PRINCIPALS       equ 0C00002F7h
STATUS_NO_PA_DATA                equ 0C00002F8h
STATUS_PKINIT_NAME_MISMATCH      equ 0C00002F9h
STATUS_SMARTCARD_LOGON_REQUIRED  equ 0C00002FAh
STATUS_KDC_INVALID_REQUEST       equ 0C00002FBh
STATUS_KDC_UNABLE_TO_REFER       equ 0C00002FCh
STATUS_KDC_UNKNOWN_ETYPE         equ 0C00002FDh
STATUS_SHUTDOWN_IN_PROGRESS      equ 0C00002FEh
STATUS_SERVER_SHUTDOWN_IN_PROGRESS equ 0C00002FFh
STATUS_NOT_SUPPORTED_ON_SBS      equ 0C0000300h
STATUS_WMI_GUID_DISCONNECTED     equ 0C0000301h
STATUS_WMI_ALREADY_DISABLED      equ 0C0000302h
STATUS_WMI_ALREADY_ENABLED       equ 0C0000303h
STATUS_MFT_TOO_FRAGMENTED        equ 0C0000304h
STATUS_COPY_PROTECTION_FAILURE   equ 0C0000305h
STATUS_CSS_AUTHENTICATION_FAILURE equ 0C0000306h
STATUS_CSS_KEY_NOT_PRESENT       equ 0C0000307h
STATUS_CSS_KEY_NOT_ESTABLISHED   equ 0C0000308h
STATUS_CSS_SCRAMBLED_SECTOR      equ 0C0000309h
STATUS_CSS_REGION_MISMATCH       equ 0C000030Ah
STATUS_CSS_RESETS_EXHAUSTED      equ 0C000030Bh
STATUS_PKINIT_FAILURE            equ 0C0000320h
STATUS_SMARTCARD_SUBSYSTEM_FAILURE equ 0C0000321h
STATUS_NO_KERB_KEY               equ 0C0000322h
STATUS_HOST_DOWN                 equ 0C0000350h
STATUS_UNSUPPORTED_PREAUTH       equ 0C0000351h
STATUS_EFS_ALG_BLOB_TOO_BIG      equ 0C0000352h
STATUS_PORT_NOT_SET              equ 0C0000353h
STATUS_DEBUGGER_INACTIVE         equ 0C0000354h
STATUS_DS_VERSION_CHECK_FAILURE  equ 0C0000355h
STATUS_AUDITING_DISABLED         equ 0C0000356h
STATUS_PRENT4_MACHINE_ACCOUNT    equ 0C0000357h
STATUS_DS_AG_CANT_HAVE_UNIVERSAL_MEMBER equ 0C0000358h
STATUS_INVALID_IMAGE_WIN_32      equ 0C0000359h
STATUS_INVALID_IMAGE_WIN_64      equ 0C000035Ah
STATUS_BAD_BINDINGS              equ 0C000035Bh
STATUS_NETWORK_SESSION_EXPIRED   equ 0C000035Ch
STATUS_APPHELP_BLOCK             equ 0C000035Dh
STATUS_ALL_SIDS_FILTERED         equ 0C000035Eh
STATUS_NOT_SAFE_MODE_DRIVER      equ 0C000035Fh
STATUS_ACCESS_DISABLED_BY_POLICY_DEFAULT equ 0C0000361h
STATUS_ACCESS_DISABLED_BY_POLICY_PATH equ 0C0000362h
STATUS_ACCESS_DISABLED_BY_POLICY_PUBLISHER equ 0C0000363h
STATUS_ACCESS_DISABLED_BY_POLICY_OTHER equ 0C0000364h
STATUS_FAILED_DRIVER_ENTRY       equ 0C0000365h
STATUS_DEVICE_ENUMERATION_ERROR  equ 0C0000366h
STATUS_WAIT_FOR_OPLOCK           equ 00000367h
STATUS_MOUNT_POINT_NOT_RESOLVED  equ 0C0000368h
STATUS_INVALID_DEVICE_OBJECT_PARAMETER equ 0C0000369h
STATUS_MCA_OCCURED               equ 0C000036Ah
STATUS_DRIVER_BLOCKED_CRITICAL   equ 0C000036Bh
STATUS_DRIVER_BLOCKED            equ 0C000036Ch
STATUS_DRIVER_DATABASE_ERROR     equ 0C000036Dh
STATUS_SYSTEM_HIVE_TOO_LARGE     equ 0C000036Eh
STATUS_INVALID_IMPORT_OF_NON_DLL equ 0C000036Fh
STATUS_DS_SHUTTING_DOWN          equ 40000370h
STATUS_SMARTCARD_WRONG_PIN       equ 0C0000380h
STATUS_SMARTCARD_CARD_BLOCKED    equ 0C0000381h
STATUS_SMARTCARD_CARD_NOT_AUTHENTICATED equ 0C0000382h
STATUS_SMARTCARD_NO_CARD         equ 0C0000383h
STATUS_SMARTCARD_NO_KEY_CONTAINER equ 0C0000384h
STATUS_SMARTCARD_NO_CERTIFICATE  equ 0C0000385h
STATUS_SMARTCARD_NO_KEYSET       equ 0C0000386h
STATUS_SMARTCARD_IO_ERROR        equ 0C0000387h
STATUS_DOWNGRADE_DETECTED        equ 0C0000388h
STATUS_SMARTCARD_CERT_REVOKED    equ 0C0000389h
STATUS_ISSUING_CA_UNTRUSTED      equ 0C000038Ah
STATUS_REVOCATION_OFFLINE_C      equ 0C000038Bh
STATUS_PKINIT_CLIENT_FAILURE     equ 0C000038Ch
STATUS_SMARTCARD_CERT_EXPIRED    equ 0C000038Dh
STATUS_DRIVER_FAILED_PRIOR_UNLOAD equ 0C000038Eh
STATUS_SMARTCARD_SILENT_CONTEXT  equ 0C000038Fh
STATUS_PER_USER_TRUST_QUOTA_EXCEEDED equ 0C0000401h
STATUS_ALL_USER_TRUST_QUOTA_EXCEEDED equ 0C0000402h
STATUS_USER_DELETE_TRUST_QUOTA_EXCEEDED equ 0C0000403h
STATUS_DS_NAME_NOT_UNIQUE        equ 0C0000404h
STATUS_DS_DUPLICATE_ID_FOUND     equ 0C0000405h
STATUS_DS_GROUP_CONVERSION_ERROR equ 0C0000406h
STATUS_VOLSNAP_PREPARE_HIBERNATE equ 0C0000407h
STATUS_USER2USER_REQUIRED        equ 0C0000408h
STATUS_STACK_BUFFER_OVERRUN      equ 0C0000409h
STATUS_NO_S4U_PROT_SUPPORT       equ 0C000040Ah
STATUS_CROSSREALM_DELEGATION_FAILURE equ 0C000040Bh
STATUS_REVOCATION_OFFLINE_KDC    equ 0C000040Ch
STATUS_ISSUING_CA_UNTRUSTED_KDC  equ 0C000040Dh
STATUS_KDC_CERT_EXPIRED          equ 0C000040Eh
STATUS_KDC_CERT_REVOKED          equ 0C000040Fh
STATUS_PARAMETER_QUOTA_EXCEEDED  equ 0C0000410h
STATUS_HIBERNATION_FAILURE       equ 0C0000411h
STATUS_DELAY_LOAD_FAILED         equ 0C0000412h
STATUS_AUTHENTICATION_FIREWALL_FAILED equ 0C0000413h
STATUS_VDM_DISALLOWED            equ 0C0000414h
STATUS_HUNG_DISPLAY_DRIVER_THREAD equ 0C0000415h
STATUS_WOW_ASSERTION             equ 0C0009898h
DBG_NO_STATE_CHANGE              equ 0C0010001h
DBG_APP_NOT_IDLE                 equ 0C0010002h
RPC_NT_INVALID_STRING_BINDING    equ 0C0020001h
RPC_NT_WRONG_KIND_OF_BINDING     equ 0C0020002h
RPC_NT_INVALID_BINDING           equ 0C0020003h
RPC_NT_PROTSEQ_NOT_SUPPORTED     equ 0C0020004h
RPC_NT_INVALID_RPC_PROTSEQ       equ 0C0020005h
RPC_NT_INVALID_STRING_UUID       equ 0C0020006h
RPC_NT_INVALID_ENDPOINT_FORMAT   equ 0C0020007h
RPC_NT_INVALID_NET_ADDR          equ 0C0020008h
RPC_NT_NO_ENDPOINT_FOUND         equ 0C0020009h
RPC_NT_INVALID_TIMEOUT           equ 0C002000Ah
RPC_NT_OBJECT_NOT_FOUND          equ 0C002000Bh
RPC_NT_ALREADY_REGISTERED        equ 0C002000Ch
RPC_NT_TYPE_ALREADY_REGISTERED   equ 0C002000Dh
RPC_NT_ALREADY_LISTENING         equ 0C002000Eh
RPC_NT_NO_PROTSEQS_REGISTERED    equ 0C002000Fh
RPC_NT_NOT_LISTENING             equ 0C0020010h
RPC_NT_UNKNOWN_MGR_TYPE          equ 0C0020011h
RPC_NT_UNKNOWN_IF                equ 0C0020012h
RPC_NT_NO_BINDINGS               equ 0C0020013h
RPC_NT_NO_PROTSEQS               equ 0C0020014h
RPC_NT_CANT_CREATE_ENDPOINT      equ 0C0020015h
RPC_NT_OUT_OF_RESOURCES          equ 0C0020016h
RPC_NT_SERVER_UNAVAILABLE        equ 0C0020017h
RPC_NT_SERVER_TOO_BUSY           equ 0C0020018h
RPC_NT_INVALID_NETWORK_OPTIONS   equ 0C0020019h
RPC_NT_NO_CALL_ACTIVE            equ 0C002001Ah
RPC_NT_CALL_FAILED               equ 0C002001Bh
RPC_NT_CALL_FAILED_DNE           equ 0C002001Ch
RPC_NT_PROTOCOL_ERROR            equ 0C002001Dh
RPC_NT_UNSUPPORTED_TRANS_SYN     equ 0C002001Fh
RPC_NT_UNSUPPORTED_TYPE          equ 0C0020021h
RPC_NT_INVALID_TAG               equ 0C0020022h
RPC_NT_INVALID_BOUND             equ 0C0020023h
RPC_NT_NO_ENTRY_NAME             equ 0C0020024h
RPC_NT_INVALID_NAME_SYNTAX       equ 0C0020025h
RPC_NT_UNSUPPORTED_NAME_SYNTAX   equ 0C0020026h
RPC_NT_UUID_NO_ADDRESS           equ 0C0020028h
RPC_NT_DUPLICATE_ENDPOINT        equ 0C0020029h
RPC_NT_UNKNOWN_AUTHN_TYPE        equ 0C002002Ah
RPC_NT_MAX_CALLS_TOO_SMALL       equ 0C002002Bh
RPC_NT_STRING_TOO_LONG           equ 0C002002Ch
RPC_NT_PROTSEQ_NOT_FOUND         equ 0C002002Dh
RPC_NT_PROCNUM_OUT_OF_RANGE      equ 0C002002Eh
RPC_NT_BINDING_HAS_NO_AUTH       equ 0C002002Fh
RPC_NT_UNKNOWN_AUTHN_SERVICE     equ 0C0020030h
RPC_NT_UNKNOWN_AUTHN_LEVEL       equ 0C0020031h
RPC_NT_INVALID_AUTH_IDENTITY     equ 0C0020032h
RPC_NT_UNKNOWN_AUTHZ_SERVICE     equ 0C0020033h
EPT_NT_INVALID_ENTRY             equ 0C0020034h
EPT_NT_CANT_PERFORM_OP           equ 0C0020035h
EPT_NT_NOT_REGISTERED            equ 0C0020036h
RPC_NT_NOTHING_TO_EXPORT         equ 0C0020037h
RPC_NT_INCOMPLETE_NAME           equ 0C0020038h
RPC_NT_INVALID_VERS_OPTION       equ 0C0020039h
RPC_NT_NO_MORE_MEMBERS           equ 0C002003Ah
RPC_NT_NOT_ALL_OBJS_UNEXPORTED   equ 0C002003Bh
RPC_NT_INTERFACE_NOT_FOUND       equ 0C002003Ch
RPC_NT_ENTRY_ALREADY_EXISTS      equ 0C002003Dh
RPC_NT_ENTRY_NOT_FOUND           equ 0C002003Eh
RPC_NT_NAME_SERVICE_UNAVAILABLE  equ 0C002003Fh
RPC_NT_INVALID_NAF_ID            equ 0C0020040h
RPC_NT_CANNOT_SUPPORT            equ 0C0020041h
RPC_NT_NO_CONTEXT_AVAILABLE      equ 0C0020042h
RPC_NT_INTERNAL_ERROR            equ 0C0020043h
RPC_NT_ZERO_DIVIDE               equ 0C0020044h
RPC_NT_ADDRESS_ERROR             equ 0C0020045h
RPC_NT_FP_DIV_ZERO               equ 0C0020046h
RPC_NT_FP_UNDERFLOW              equ 0C0020047h
RPC_NT_FP_OVERFLOW               equ 0C0020048h
RPC_NT_NO_MORE_ENTRIES           equ 0C0030001h
RPC_NT_SS_CHAR_TRANS_OPEN_FAIL   equ 0C0030002h
RPC_NT_SS_CHAR_TRANS_SHORT_FILE  equ 0C0030003h
RPC_NT_SS_IN_NULL_CONTEXT        equ 0C0030004h
RPC_NT_SS_CONTEXT_MISMATCH       equ 0C0030005h
RPC_NT_SS_CONTEXT_DAMAGED        equ 0C0030006h
RPC_NT_SS_HANDLES_MISMATCH       equ 0C0030007h
RPC_NT_SS_CANNOT_GET_CALL_HANDLE equ 0C0030008h
RPC_NT_NULL_REF_POINTER          equ 0C0030009h
RPC_NT_ENUM_VALUE_OUT_OF_RANGE   equ 0C003000Ah
RPC_NT_BYTE_COUNT_TOO_SMALL      equ 0C003000Bh
RPC_NT_BAD_STUB_DATA             equ 0C003000Ch
RPC_NT_CALL_IN_PROGRESS          equ 0C0020049h
RPC_NT_NO_MORE_BINDINGS          equ 0C002004Ah
RPC_NT_GROUP_MEMBER_NOT_FOUND    equ 0C002004Bh
EPT_NT_CANT_CREATE               equ 0C002004Ch
RPC_NT_INVALID_OBJECT            equ 0C002004Dh
RPC_NT_NO_INTERFACES             equ 0C002004Fh
RPC_NT_CALL_CANCELLED            equ 0C0020050h
RPC_NT_BINDING_INCOMPLETE        equ 0C0020051h
RPC_NT_COMM_FAILURE              equ 0C0020052h
RPC_NT_UNSUPPORTED_AUTHN_LEVEL   equ 0C0020053h
RPC_NT_NO_PRINC_NAME             equ 0C0020054h
RPC_NT_NOT_RPC_ERROR             equ 0C0020055h
RPC_NT_UUID_LOCAL_ONLY           equ 40020056h
RPC_NT_SEC_PKG_ERROR             equ 0C0020057h
RPC_NT_NOT_CANCELLED             equ 0C0020058h
RPC_NT_INVALID_ES_ACTION         equ 0C0030059h
RPC_NT_WRONG_ES_VERSION          equ 0C003005Ah
RPC_NT_WRONG_STUB_VERSION        equ 0C003005Bh
RPC_NT_INVALID_PIPE_OBJECT       equ 0C003005Ch
RPC_NT_INVALID_PIPE_OPERATION    equ 0C003005Dh
RPC_NT_WRONG_PIPE_VERSION        equ 0C003005Eh
RPC_NT_PIPE_CLOSED               equ 0C003005Fh
RPC_NT_PIPE_DISCIPLINE_ERROR     equ 0C0030060h
RPC_NT_PIPE_EMPTY                equ 0C0030061h
RPC_NT_INVALID_ASYNC_HANDLE      equ 0C0020062h
RPC_NT_INVALID_ASYNC_CALL        equ 0C0020063h
RPC_NT_SEND_INCOMPLETE           equ 400200AFh
STATUS_ACPI_INVALID_OPCODE       equ 0C0140001h
STATUS_ACPI_STACK_OVERFLOW       equ 0C0140002h
STATUS_ACPI_ASSERT_FAILED        equ 0C0140003h
STATUS_ACPI_INVALID_INDEX        equ 0C0140004h
STATUS_ACPI_INVALID_ARGUMENT     equ 0C0140005h
STATUS_ACPI_FATAL                equ 0C0140006h
STATUS_ACPI_INVALID_SUPERNAME    equ 0C0140007h
STATUS_ACPI_INVALID_ARGTYPE      equ 0C0140008h
STATUS_ACPI_INVALID_OBJTYPE      equ 0C0140009h
STATUS_ACPI_INVALID_TARGETTYPE   equ 0C014000Ah
STATUS_ACPI_INCORRECT_ARGUMENT_COUNT equ 0C014000Bh
STATUS_ACPI_ADDRESS_NOT_MAPPED   equ 0C014000Ch
STATUS_ACPI_INVALID_EVENTTYPE    equ 0C014000Dh
STATUS_ACPI_HANDLER_COLLISION    equ 0C014000Eh
STATUS_ACPI_INVALID_DATA         equ 0C014000Fh
STATUS_ACPI_INVALID_REGION       equ 0C0140010h
STATUS_ACPI_INVALID_ACCESS_SIZE  equ 0C0140011h
STATUS_ACPI_ACQUIRE_GLOBAL_LOCK  equ 0C0140012h
STATUS_ACPI_ALREADY_INITIALIZED  equ 0C0140013h
STATUS_ACPI_NOT_INITIALIZED      equ 0C0140014h
STATUS_ACPI_INVALID_MUTEX_LEVEL  equ 0C0140015h
STATUS_ACPI_MUTEX_NOT_OWNED      equ 0C0140016h
STATUS_ACPI_MUTEX_NOT_OWNER      equ 0C0140017h
STATUS_ACPI_RS_ACCESS            equ 0C0140018h
STATUS_ACPI_INVALID_TABLE        equ 0C0140019h
STATUS_ACPI_REG_HANDLER_FAILED   equ 0C0140020h
STATUS_ACPI_POWER_REQUEST_FAILED equ 0C0140021h
STATUS_CTX_WINSTATION_NAME_INVALID equ 0C00A0001h
STATUS_CTX_INVALID_PD            equ 0C00A0002h
STATUS_CTX_PD_NOT_FOUND          equ 0C00A0003h
STATUS_CTX_CDM_CONNECT           equ 400A0004h
STATUS_CTX_CDM_DISCONNECT        equ 400A0005h
STATUS_CTX_CLOSE_PENDING         equ 0C00A0006h
STATUS_CTX_NO_OUTBUF             equ 0C00A0007h
STATUS_CTX_MODEM_INF_NOT_FOUND   equ 0C00A0008h
STATUS_CTX_INVALID_MODEMNAME     equ 0C00A0009h
STATUS_CTX_RESPONSE_ERROR        equ 0C00A000Ah
STATUS_CTX_MODEM_RESPONSE_TIMEOUT equ 0C00A000Bh
STATUS_CTX_MODEM_RESPONSE_NO_CARRIER equ 0C00A000Ch
STATUS_CTX_MODEM_RESPONSE_NO_DIALTONE equ 0C00A000Dh
STATUS_CTX_MODEM_RESPONSE_BUSY   equ 0C00A000Eh
STATUS_CTX_MODEM_RESPONSE_VOICE  equ 0C00A000Fh
STATUS_CTX_TD_ERROR              equ 0C00A0010h
STATUS_CTX_LICENSE_CLIENT_INVALID equ 0C00A0012h
STATUS_CTX_LICENSE_NOT_AVAILABLE equ 0C00A0013h
STATUS_CTX_LICENSE_EXPIRED       equ 0C00A0014h
STATUS_CTX_WINSTATION_NOT_FOUND  equ 0C00A0015h
STATUS_CTX_WINSTATION_NAME_COLLISION equ 0C00A0016h
STATUS_CTX_WINSTATION_BUSY       equ 0C00A0017h
STATUS_CTX_BAD_VIDEO_MODE        equ 0C00A0018h
STATUS_CTX_GRAPHICS_INVALID      equ 0C00A0022h
STATUS_CTX_NOT_CONSOLE           equ 0C00A0024h
STATUS_CTX_CLIENT_QUERY_TIMEOUT  equ 0C00A0026h
STATUS_CTX_CONSOLE_DISCONNECT    equ 0C00A0027h
STATUS_CTX_CONSOLE_CONNECT       equ 0C00A0028h
STATUS_CTX_SHADOW_DENIED         equ 0C00A002Ah
STATUS_CTX_WINSTATION_ACCESS_DENIED equ 0C00A002Bh
STATUS_CTX_INVALID_WD            equ 0C00A002Eh
STATUS_CTX_WD_NOT_FOUND          equ 0C00A002Fh
STATUS_CTX_SHADOW_INVALID        equ 0C00A0030h
STATUS_CTX_SHADOW_DISABLED       equ 0C00A0031h
STATUS_RDP_PROTOCOL_ERROR        equ 0C00A0032h
STATUS_CTX_CLIENT_LICENSE_NOT_SET equ 0C00A0033h
STATUS_CTX_CLIENT_LICENSE_IN_USE equ 0C00A0034h
STATUS_CTX_SHADOW_ENDED_BY_MODE_CHANGE equ 0C00A0035h
STATUS_CTX_SHADOW_NOT_RUNNING    equ 0C00A0036h
STATUS_PNP_BAD_MPS_TABLE         equ 0C0040035h
STATUS_PNP_TRANSLATION_FAILED    equ 0C0040036h
STATUS_PNP_IRQ_TRANSLATION_FAILED equ 0C0040037h
STATUS_PNP_INVALID_ID            equ 0C0040038h
STATUS_SXS_SECTION_NOT_FOUND     equ 0C0150001h
STATUS_SXS_CANT_GEN_ACTCTX       equ 0C0150002h
STATUS_SXS_INVALID_ACTCTXDATA_FORMAT equ 0C0150003h
STATUS_SXS_ASSEMBLY_NOT_FOUND    equ 0C0150004h
STATUS_SXS_MANIFEST_FORMAT_ERROR equ 0C0150005h
STATUS_SXS_MANIFEST_PARSE_ERROR  equ 0C0150006h
STATUS_SXS_ACTIVATION_CONTEXT_DISABLED equ 0C0150007h
STATUS_SXS_KEY_NOT_FOUND         equ 0C0150008h
STATUS_SXS_VERSION_CONFLICT      equ 0C0150009h
STATUS_SXS_WRONG_SECTION_TYPE    equ 0C015000Ah
STATUS_SXS_THREAD_QUERIES_DISABLED equ 0C015000Bh
STATUS_SXS_ASSEMBLY_MISSING      equ 0C015000Ch
STATUS_SXS_RELEASE_ACTIVATION_CONTEXT equ 4015000Dh
STATUS_SXS_PROCESS_DEFAULT_ALREADY_SET equ 0C015000Eh
STATUS_SXS_MULTIPLE_DEACTIVATION equ 0C0150011h
STATUS_SXS_SYSTEM_DEFAULT_ACTIVATION_CONTEXT_EMPTY equ 0C0150012h
STATUS_SXS_PROCESS_TERMINATION_REQUESTED equ 0C0150013h
STATUS_SXS_CORRUPT_ACTIVATION_STACK equ 0C0150014h
STATUS_SXS_CORRUPTION            equ 0C0150015h
STATUS_CLUSTER_INVALID_NODE      equ 0C0130001h
STATUS_CLUSTER_NODE_EXISTS       equ 0C0130002h
STATUS_CLUSTER_JOIN_IN_PROGRESS  equ 0C0130003h
STATUS_CLUSTER_NODE_NOT_FOUND    equ 0C0130004h
STATUS_CLUSTER_LOCAL_NODE_NOT_FOUND equ 0C0130005h
STATUS_CLUSTER_NETWORK_EXISTS    equ 0C0130006h
STATUS_CLUSTER_NETWORK_NOT_FOUND equ 0C0130007h
STATUS_CLUSTER_NETINTERFACE_EXISTS equ 0C0130008h
STATUS_CLUSTER_NETINTERFACE_NOT_FOUND equ 0C0130009h
STATUS_CLUSTER_INVALID_REQUEST   equ 0C013000Ah
STATUS_CLUSTER_INVALID_NETWORK_PROVIDER equ 0C013000Bh
STATUS_CLUSTER_NODE_DOWN         equ 0C013000Ch
STATUS_CLUSTER_NODE_UNREACHABLE  equ 0C013000Dh
STATUS_CLUSTER_NODE_NOT_MEMBER   equ 0C013000Eh
STATUS_CLUSTER_JOIN_NOT_IN_PROGRESS equ 0C013000Fh
STATUS_CLUSTER_INVALID_NETWORK   equ 0C0130010h
STATUS_CLUSTER_NO_NET_ADAPTERS   equ 0C0130011h
STATUS_CLUSTER_NODE_UP           equ 0C0130012h
STATUS_CLUSTER_NODE_PAUSED       equ 0C0130013h
STATUS_CLUSTER_NODE_NOT_PAUSED   equ 0C0130014h
STATUS_CLUSTER_NO_SECURITY_CONTEXT equ 0C0130015h
STATUS_CLUSTER_NETWORK_NOT_INTERNAL equ 0C0130016h
STATUS_CLUSTER_POISONED          equ 0C0130017h
FACILITY_VIDEO                   equ 1Bh
FACILITY_TRANSACTION             equ 19h
FACILITY_NTSSPI                  equ 9h
FACILITY_NTWIN32                 equ 7h
FACILITY_NDIS_ERROR_CODE         equ 23h
FACILTIY_MUI_ERROR_CODE          equ 0Bh
FACILITY_MONITOR                 equ 1Dh
FACILITY_MAXIMUM_VALUE           equ 37h
FACILITY_IPSEC                   equ 36h
FACILITY_HYPERVISOR              equ 35h
FACILITY_GRAPHICS_KERNEL         equ 1Eh
FACILITY_FWP_ERROR_CODE          equ 22h
FACILITY_FVE_ERROR_CODE          equ 21h
FACILITY_FILTER_MANAGER          equ 1Ch
FACILITY_DRIVER_FRAMEWORK        equ 20h
FACILITY_COMMONLOG               equ 1Ah
STATUS_INTERRUPT_VECTOR_ALREADY_CONNECTED equ 00000127h
STATUS_INTERRUPT_STILL_CONNECTED equ 00000128h
STATUS_PROCESS_CLONED            equ 00000129h
STATUS_FILE_LOCKED_WITH_ONLY_READERS equ 0000012Ah
STATUS_FILE_LOCKED_WITH_WRITERS  equ 0000012Bh
STATUS_RESOURCEMANAGER_READ_ONLY equ 00000202h
STATUS_FLT_IO_COMPLETE           equ 001C0001h
STATUS_MESSAGE_RETRIEVED         equ 4000002Eh
STATUS_SYSTEM_POWERSTATE_TRANSITION equ 4000002Fh
STATUS_ALPC_CHECK_COMPLETION_LIST equ 40000030h
STATUS_SYSTEM_POWERSTATE_COMPLEX_TRANSITION equ 40000031h
STATUS_ACCESS_AUDIT_BY_POLICY    equ 40000032h
STATUS_ABANDON_HIBERFILE         equ 40000033h
STATUS_BIZRULES_NOT_ENABLED      equ 40000034h
STATUS_FLT_BUFFER_TOO_SMALL      equ 801C0001h
STATUS_STOPPED_ON_SYMLINK        equ 8000002Dh
STATUS_FVE_PARTIAL_METADATA      equ 80210001h
STATUS_IMAGE_ALREADY_LOADED_AS_DLL equ 0C000019Dh
STATUS_NETWORK_OPEN_RESTRICTION  equ 0C0000201h
STATUS_NO_SECRETS                equ 0C0000371h
STATUS_ACCESS_DISABLED_NO_SAFER_UI_BY_POLICY equ 0C0000372h
STATUS_FAILED_STACK_SWITCH       equ 0C0000373h
STATUS_HEAP_CORRUPTION           equ 0C0000374h
STATUS_INSUFFICIENT_RESOURCE_FOR_SPECIFIED_SHARED_SECTION_SIZE equ 0C0000416h
STATUS_INVALID_CRUNTIME_PARAMETER equ 0C0000417h
STATUS_NTLM_BLOCKED              equ 0C0000418h
STATUS_ASSERTION_FAILURE         equ 0C0000420h
STATUS_VERIFIER_STOP             equ 0C0000421h
STATUS_CALLBACK_POP_STACK        equ 0C0000423h
STATUS_INCOMPATIBLE_DRIVER_BLOCKED equ 0C0000424h
STATUS_HIVE_UNLOADED             equ 0C0000425h
STATUS_COMPRESSION_DISABLED      equ 0C0000426h
STATUS_FILE_SYSTEM_LIMITATION    equ 0C0000427h
STATUS_INVALID_IMAGE_HASH        equ 0C0000428h
STATUS_NOT_CAPABLE               equ 0C0000429h
STATUS_REQUEST_OUT_OF_SEQUENCE   equ 0C000042Ah
STATUS_IMPLEMENTATION_LIMIT      equ 0C000042Bh
STATUS_ELEVATION_REQUIRED        equ 0C000042Ch
STATUS_BEYOND_VDL                equ 0C0000432h
STATUS_ENCOUNTERED_WRITE_IN_PROGRESS equ 0C0000433h
STATUS_PTE_CHANGED               equ 0C0000434h
STATUS_PURGE_FAILED              equ 0C0000435h
STATUS_CRED_REQUIRES_CONFIRMATION equ 0C0000440h
STATUS_CS_ENCRYPTION_INVALID_SERVER_RESPONSE equ 0C0000441h
STATUS_CS_ENCRYPTION_UNSUPPORTED_SERVER equ 0C0000442h
STATUS_CS_ENCRYPTION_EXISTING_ENCRYPTED_FILE equ 0C0000443h
STATUS_CS_ENCRYPTION_NEW_ENCRYPTED_FILE equ 0C0000444h
STATUS_CS_ENCRYPTION_FILE_NOT_CSE equ 0C0000445h
STATUS_INVALID_LABEL             equ 0C0000446h
STATUS_DRIVER_PROCESS_TERMINATED equ 0C0000450h
STATUS_AMBIGUOUS_SYSTEM_DEVICE   equ 0C0000451h
STATUS_SYSTEM_DEVICE_NOT_FOUND   equ 0C0000452h
STATUS_RESTART_BOOT_APPLICATION  equ 0C0000453h
STATUS_INVALID_TASK_NAME         equ 0C0000500h
STATUS_INVALID_TASK_INDEX        equ 0C0000501h
STATUS_THREAD_ALREADY_IN_TASK    equ 0C0000502h
STATUS_CALLBACK_BYPASS           equ 0C0000503h
STATUS_PORT_CLOSED               equ 0C0000700h
STATUS_MESSAGE_LOST              equ 0C0000701h
STATUS_INVALID_MESSAGE           equ 0C0000702h
STATUS_REQUEST_CANCELED          equ 0C0000703h
STATUS_RECURSIVE_DISPATCH        equ 0C0000704h
STATUS_LPC_RECEIVE_BUFFER_EXPECTED equ 0C0000705h
STATUS_LPC_INVALID_CONNECTION_USAGE equ 0C0000706h
STATUS_LPC_REQUESTS_NOT_ALLOWED  equ 0C0000707h
STATUS_RESOURCE_IN_USE           equ 0C0000708h
STATUS_HARDWARE_MEMORY_ERROR     equ 0C0000709h
STATUS_THREADPOOL_HANDLE_EXCEPTION equ 0C000070Ah
STATUS_THREADPOOL_SET_EVENT_ON_COMPLETION_FAILED equ 0C000070Bh
STATUS_THREADPOOL_RELEASE_SEMAPHORE_ON_COMPLETION_FAILED equ 0C000070Ch
STATUS_THREADPOOL_RELEASE_MUTEX_ON_COMPLETION_FAILED equ 0C000070Dh
STATUS_THREADPOOL_FREE_LIBRARY_ON_COMPLETION_FAILED equ 0C000070Eh
STATUS_THREADPOOL_RELEASED_DURING_OPERATION equ 0C000070Fh
STATUS_CALLBACK_RETURNED_WHILE_IMPERSONATING equ 0C0000710h
STATUS_APC_RETURNED_WHILE_IMPERSONATING equ 0C0000711h
STATUS_PROCESS_IS_PROTECTED      equ 0C0000712h
STATUS_MCA_EXCEPTION             equ 0C0000713h
STATUS_CERTIFICATE_MAPPING_NOT_UNIQUE equ 0C0000714h
STATUS_SYMLINK_CLASS_DISABLED    equ 0C0000715h
STATUS_INVALID_IDN_NORMALIZATION equ 0C0000716h
STATUS_NO_UNICODE_TRANSLATION    equ 0C0000717h
STATUS_ALREADY_REGISTERED        equ 0C0000718h
STATUS_CONTEXT_MISMATCH          equ 0C0000719h
STATUS_PORT_ALREADY_HAS_COMPLETION_LIST equ 0C000071Ah
STATUS_CALLBACK_RETURNED_THREAD_PRIORITY equ 0C000071Bh
STATUS_INVALID_THREAD            equ 0C000071Ch
STATUS_CALLBACK_RETURNED_TRANSACTION equ 0C000071Dh
STATUS_CALLBACK_RETURNED_LDR_LOCK equ 0C000071Eh
STATUS_CALLBACK_RETURNED_LANG    equ 0C000071Fh
STATUS_CALLBACK_RETURNED_PRI_BACK equ 0C0000720h
STATUS_CALLBACK_RETURNED_THREAD_AFFINITY equ 0C0000721h
STATUS_DISK_REPAIR_DISABLED      equ 0C0000800h
STATUS_DS_DOMAIN_RENAME_IN_PROGRESS equ 0C0000801h
STATUS_DISK_QUOTA_EXCEEDED       equ 0C0000802h
STATUS_DATA_LOST_REPAIR          equ 80000803h
STATUS_CONTENT_BLOCKED           equ 0C0000804h
STATUS_BAD_CLUSTERS              equ 0C0000805h
STATUS_VOLUME_DIRTY              equ 0C0000806h
STATUS_FILE_CHECKED_OUT          equ 0C0000901h
STATUS_CHECKOUT_REQUIRED         equ 0C0000902h
STATUS_BAD_FILE_TYPE             equ 0C0000903h
STATUS_FILE_TOO_LARGE            equ 0C0000904h
STATUS_FORMS_AUTH_REQUIRED       equ 0C0000905h
STATUS_VIRUS_INFECTED            equ 0C0000906h
STATUS_VIRUS_DELETED             equ 0C0000907h
STATUS_BAD_MCFG_TABLE            equ 0C0000908h
STATUS_INVALID_SIGNATURE         equ 0C000A000h
STATUS_HMAC_NOT_SUPPORTED        equ 0C000A001h
STATUS_IPSEC_QUEUE_OVERFLOW      equ 0C000A010h
STATUS_ND_QUEUE_OVERFLOW         equ 0C000A011h
STATUS_HOPLIMIT_EXCEEDED         equ 0C000A012h
STATUS_PROTOCOL_NOT_SUPPORTED    equ 0C000A013h
STATUS_LOST_WRITEBEHIND_DATA_NETWORK_DISCONNECTED equ 0C000A080h
STATUS_LOST_WRITEBEHIND_DATA_NETWORK_SERVER_ERROR equ 0C000A081h
STATUS_LOST_WRITEBEHIND_DATA_LOCAL_DISK_ERROR equ 0C000A082h
STATUS_XML_PARSE_ERROR           equ 0C000A083h
STATUS_XMLDSIG_ERROR             equ 0C000A084h
STATUS_WRONG_COMPARTMENT         equ 0C000A085h
STATUS_AUTHIP_FAILURE            equ 0C000A086h
RPC_NT_PROXY_ACCESS_DENIED       equ 0C0020064h
STATUS_CTX_LOGON_DISABLED        equ 0C00A0037h
STATUS_CTX_SECURITY_LAYER_ERROR  equ 0C00A0038h
STATUS_TS_INCOMPATIBLE_SESSIONS  equ 0C00A0039h
STATUS_IO_REISSUE_AS_CACHED      equ 0C0040039h
STATUS_MUI_FILE_NOT_FOUND        equ 0C00B0001h
STATUS_MUI_INVALID_FILE          equ 0C00B0002h
STATUS_MUI_INVALID_RC_CONFIG     equ 0C00B0003h
STATUS_MUI_INVALID_LOCALE_NAME   equ 0C00B0004h
STATUS_MUI_INVALID_ULTIMATEFALLBACK_NAME equ 0C00B0005h
STATUS_MUI_FILE_NOT_LOADED       equ 0C00B0006h
STATUS_RESOURCE_ENUM_USER_STOP   equ 0C00B0007h
STATUS_FLT_NO_HANDLER_DEFINED    equ 0C01C0001h
STATUS_FLT_CONTEXT_ALREADY_DEFINED equ 0C01C0002h
STATUS_FLT_INVALID_ASYNCHRONOUS_REQUEST equ 0C01C0003h
STATUS_FLT_DISALLOW_FAST_IO      equ 0C01C0004h
STATUS_FLT_INVALID_NAME_REQUEST  equ 0C01C0005h
STATUS_FLT_NOT_SAFE_TO_POST_OPERATION equ 0C01C0006h
STATUS_FLT_NOT_INITIALIZED       equ 0C01C0007h
STATUS_FLT_FILTER_NOT_READY      equ 0C01C0008h
STATUS_FLT_POST_OPERATION_CLEANUP equ 0C01C0009h
STATUS_FLT_INTERNAL_ERROR        equ 0C01C000Ah
STATUS_FLT_DELETING_OBJECT       equ 0C01C000Bh
STATUS_FLT_MUST_BE_NONPAGED_POOL equ 0C01C000Ch
STATUS_FLT_DUPLICATE_ENTRY       equ 0C01C000Dh
STATUS_FLT_CBDQ_DISABLED         equ 0C01C000Eh
STATUS_FLT_DO_NOT_ATTACH         equ 0C01C000Fh
STATUS_FLT_DO_NOT_DETACH         equ 0C01C0010h
STATUS_FLT_INSTANCE_ALTITUDE_COLLISION equ 0C01C0011h
STATUS_FLT_INSTANCE_NAME_COLLISION equ 0C01C0012h
STATUS_FLT_FILTER_NOT_FOUND      equ 0C01C0013h
STATUS_FLT_VOLUME_NOT_FOUND      equ 0C01C0014h
STATUS_FLT_INSTANCE_NOT_FOUND    equ 0C01C0015h
STATUS_FLT_CONTEXT_ALLOCATION_NOT_FOUND equ 0C01C0016h
STATUS_FLT_INVALID_CONTEXT_REGISTRATION equ 0C01C0017h
STATUS_FLT_NAME_CACHE_MISS       equ 0C01C0018h
STATUS_FLT_NO_DEVICE_OBJECT      equ 0C01C0019h
STATUS_FLT_VOLUME_ALREADY_MOUNTED equ 0C01C001Ah
STATUS_FLT_ALREADY_ENLISTED      equ 0C01C001Bh
STATUS_FLT_CONTEXT_ALREADY_LINKED equ 0C01C001Ch
STATUS_FLT_NO_WAITER_FOR_REPLY   equ 0C01C0020h
STATUS_SXS_INVALID_IDENTITY_ATTRIBUTE_VALUE equ 0C0150016h
STATUS_SXS_INVALID_IDENTITY_ATTRIBUTE_NAME equ 0C0150017h
STATUS_SXS_IDENTITY_DUPLICATE_ATTRIBUTE equ 0C0150018h
STATUS_SXS_IDENTITY_PARSE_ERROR  equ 0C0150019h
STATUS_SXS_COMPONENT_STORE_CORRUPT equ 0C015001Ah
STATUS_SXS_FILE_HASH_MISMATCH    equ 0C015001Bh
STATUS_SXS_MANIFEST_IDENTITY_SAME_BUT_CONTENTS_DIFFERENT equ 0C015001Ch
STATUS_SXS_IDENTITIES_DIFFERENT  equ 0C015001Dh
STATUS_SXS_ASSEMBLY_IS_NOT_A_DEPLOYMENT equ 0C015001Eh
STATUS_SXS_FILE_NOT_PART_OF_ASSEMBLY equ 0C015001Fh
STATUS_ADVANCED_INSTALLER_FAILED equ 0C0150020h
STATUS_XML_ENCODING_MISMATCH     equ 0C0150021h
STATUS_SXS_MANIFEST_TOO_BIG      equ 0C0150022h
STATUS_SXS_SETTING_NOT_REGISTERED equ 0C0150023h
STATUS_SXS_TRANSACTION_CLOSURE_INCOMPLETE equ 0C0150024h
STATUS_SMI_PRIMITIVE_INSTALLER_FAILED equ 0C0150025h
STATUS_GENERIC_COMMAND_FAILED    equ 0C0150026h
STATUS_SXS_FILE_HASH_MISSING     equ 0C0150027h
STATUS_TRANSACTIONAL_CONFLICT    equ 0C0190001h
STATUS_INVALID_TRANSACTION       equ 0C0190002h
STATUS_TRANSACTION_NOT_ACTIVE    equ 0C0190003h
STATUS_TM_INITIALIZATION_FAILED  equ 0C0190004h
STATUS_RM_NOT_ACTIVE             equ 0C0190005h
STATUS_RM_METADATA_CORRUPT       equ 0C0190006h
STATUS_TRANSACTION_NOT_JOINED    equ 0C0190007h
STATUS_DIRECTORY_NOT_RM          equ 0C0190008h
STATUS_COULD_NOT_RESIZE_LOG      equ 80190009h
STATUS_TRANSACTIONS_UNSUPPORTED_REMOTE equ 0C019000Ah
STATUS_LOG_RESIZE_INVALID_SIZE   equ 0C019000Bh
STATUS_REMOTE_FILE_VERSION_MISMATCH equ 0C019000Ch
STATUS_CRM_PROTOCOL_ALREADY_EXISTS equ 0C019000Fh
STATUS_TRANSACTION_PROPAGATION_FAILED equ 0C0190010h
STATUS_CRM_PROTOCOL_NOT_FOUND    equ 0C0190011h
STATUS_TRANSACTION_SUPERIOR_EXISTS equ 0C0190012h
STATUS_TRANSACTION_REQUEST_NOT_VALID equ 0C0190013h
STATUS_TRANSACTION_NOT_REQUESTED equ 0C0190014h
STATUS_TRANSACTION_ALREADY_ABORTED equ 0C0190015h
STATUS_TRANSACTION_ALREADY_COMMITTED equ 0C0190016h
STATUS_TRANSACTION_INVALID_MARSHALL_BUFFER equ 0C0190017h
STATUS_CURRENT_TRANSACTION_NOT_VALID equ 0C0190018h
STATUS_LOG_GROWTH_FAILED         equ 0C0190019h
STATUS_OBJECT_NO_LONGER_EXISTS   equ 0C0190021h
STATUS_STREAM_MINIVERSION_NOT_FOUND equ 0C0190022h
STATUS_STREAM_MINIVERSION_NOT_VALID equ 0C0190023h
STATUS_MINIVERSION_INACCESSIBLE_FROM_SPECIFIED_TRANSACTION equ 0C0190024h
STATUS_CANT_OPEN_MINIVERSION_WITH_MODIFY_INTENT equ 0C0190025h
STATUS_CANT_CREATE_MORE_STREAM_MINIVERSIONS equ 0C0190026h
STATUS_HANDLE_NO_LONGER_VALID    equ 0C0190028h
STATUS_NO_TXF_METADATA           equ 80190029h
STATUS_LOG_CORRUPTION_DETECTED   equ 0C0190030h
STATUS_CANT_RECOVER_WITH_HANDLE_OPEN equ 80190031h
STATUS_RM_DISCONNECTED           equ 0C0190032h
STATUS_ENLISTMENT_NOT_SUPERIOR   equ 0C0190033h
STATUS_RECOVERY_NOT_NEEDED       equ 40190034h
STATUS_RM_ALREADY_STARTED        equ 40190035h
STATUS_FILE_IDENTITY_NOT_PERSISTENT equ 0C0190036h
STATUS_CANT_BREAK_TRANSACTIONAL_DEPENDENCY equ 0C0190037h
STATUS_CANT_CROSS_RM_BOUNDARY    equ 0C0190038h
STATUS_TXF_DIR_NOT_EMPTY         equ 0C0190039h
STATUS_INDOUBT_TRANSACTIONS_EXIST equ 0C019003Ah
STATUS_TM_VOLATILE               equ 0C019003Bh
STATUS_ROLLBACK_TIMER_EXPIRED    equ 0C019003Ch
STATUS_TXF_ATTRIBUTE_CORRUPT     equ 0C019003Dh
STATUS_EFS_NOT_ALLOWED_IN_TRANSACTION equ 0C019003Eh
STATUS_TRANSACTIONAL_OPEN_NOT_ALLOWED equ 0C019003Fh
STATUS_TRANSACTED_MAPPING_UNSUPPORTED_REMOTE equ 0C0190040h
STATUS_TXF_METADATA_ALREADY_PRESENT equ 80190041h
STATUS_TRANSACTION_SCOPE_CALLBACKS_NOT_SET equ 80190042h
STATUS_TRANSACTION_REQUIRED_PROMOTION equ 0C0190043h
STATUS_CANNOT_EXECUTE_FILE_IN_TRANSACTION equ 0C0190044h
STATUS_TRANSACTIONS_NOT_FROZEN   equ 0C0190045h
STATUS_TRANSACTION_FREEZE_IN_PROGRESS equ 0C0190046h
STATUS_NOT_SNAPSHOT_VOLUME       equ 0C0190047h
STATUS_NO_SAVEPOINT_WITH_OPEN_FILES equ 0C0190048h
STATUS_SPARSE_NOT_ALLOWED_IN_TRANSACTION equ 0C0190049h
STATUS_TM_IDENTITY_MISMATCH      equ 0C019004Ah
STATUS_FLOATED_SECTION           equ 0C019004Bh
STATUS_CANNOT_ACCEPT_TRANSACTED_WORK equ 0C019004Ch
STATUS_CANNOT_ABORT_TRANSACTIONS equ 0C019004Dh
STATUS_TRANSACTION_NOT_FOUND     equ 0C019004Eh
STATUS_RESOURCEMANAGER_NOT_FOUND equ 0C019004Fh
STATUS_ENLISTMENT_NOT_FOUND      equ 0C0190050h
STATUS_TRANSACTIONMANAGER_NOT_FOUND equ 0C0190051h
STATUS_TRANSACTIONMANAGER_NOT_ONLINE equ 0C0190052h
STATUS_TRANSACTIONMANAGER_RECOVERY_NAME_COLLISION equ 0C0190053h
STATUS_TRANSACTION_NOT_ROOT      equ 0C0190054h
STATUS_TRANSACTION_OBJECT_EXPIRED equ 0C0190055h
STATUS_COMPRESSION_NOT_ALLOWED_IN_TRANSACTION equ 0C0190056h
STATUS_TRANSACTION_RESPONSE_NOT_ENLISTED equ 0C0190057h
STATUS_TRANSACTION_RECORD_TOO_LONG equ 0C0190058h
STATUS_NO_LINK_TRACKING_IN_TRANSACTION equ 0C0190059h
STATUS_OPERATION_NOT_SUPPORTED_IN_TRANSACTION equ 0C019005Ah
STATUS_TRANSACTION_INTEGRITY_VIOLATED equ 0C019005Bh
STATUS_LOG_SECTOR_INVALID        equ 0C01A0001h
STATUS_LOG_SECTOR_PARITY_INVALID equ 0C01A0002h
STATUS_LOG_SECTOR_REMAPPED       equ 0C01A0003h
STATUS_LOG_BLOCK_INCOMPLETE      equ 0C01A0004h
STATUS_LOG_INVALID_RANGE         equ 0C01A0005h
STATUS_LOG_BLOCKS_EXHAUSTED      equ 0C01A0006h
STATUS_LOG_READ_CONTEXT_INVALID  equ 0C01A0007h
STATUS_LOG_RESTART_INVALID       equ 0C01A0008h
STATUS_LOG_BLOCK_VERSION         equ 0C01A0009h
STATUS_LOG_BLOCK_INVALID         equ 0C01A000Ah
STATUS_LOG_READ_MODE_INVALID     equ 0C01A000Bh
STATUS_LOG_NO_RESTART            equ 401A000Ch
STATUS_LOG_METADATA_CORRUPT      equ 0C01A000Dh
STATUS_LOG_METADATA_INVALID      equ 0C01A000Eh
STATUS_LOG_METADATA_INCONSISTENT equ 0C01A000Fh
STATUS_LOG_RESERVATION_INVALID   equ 0C01A0010h
STATUS_LOG_CANT_DELETE           equ 0C01A0011h
STATUS_LOG_CONTAINER_LIMIT_EXCEEDED equ 0C01A0012h
STATUS_LOG_START_OF_LOG          equ 0C01A0013h
STATUS_LOG_POLICY_ALREADY_INSTALLED equ 0C01A0014h
STATUS_LOG_POLICY_NOT_INSTALLED  equ 0C01A0015h
STATUS_LOG_POLICY_INVALID        equ 0C01A0016h
STATUS_LOG_POLICY_CONFLICT       equ 0C01A0017h
STATUS_LOG_PINNED_ARCHIVE_TAIL   equ 0C01A0018h
STATUS_LOG_RECORD_NONEXISTENT    equ 0C01A0019h
STATUS_LOG_RECORDS_RESERVED_INVALID equ 0C01A001Ah
STATUS_LOG_SPACE_RESERVED_INVALID equ 0C01A001Bh
STATUS_LOG_TAIL_INVALID          equ 0C01A001Ch
STATUS_LOG_FULL                  equ 0C01A001Dh
STATUS_LOG_MULTIPLEXED           equ 0C01A001Eh
STATUS_LOG_DEDICATED             equ 0C01A001Fh
STATUS_LOG_ARCHIVE_NOT_IN_PROGRESS equ 0C01A0020h
STATUS_LOG_ARCHIVE_IN_PROGRESS   equ 0C01A0021h
STATUS_LOG_EPHEMERAL             equ 0C01A0022h
STATUS_LOG_NOT_ENOUGH_CONTAINERS equ 0C01A0023h
STATUS_LOG_CLIENT_ALREADY_REGISTERED equ 0C01A0024h
STATUS_LOG_CLIENT_NOT_REGISTERED equ 0C01A0025h
STATUS_LOG_FULL_HANDLER_IN_PROGRESS equ 0C01A0026h
STATUS_LOG_CONTAINER_READ_FAILED equ 0C01A0027h
STATUS_LOG_CONTAINER_WRITE_FAILED equ 0C01A0028h
STATUS_LOG_CONTAINER_OPEN_FAILED equ 0C01A0029h
STATUS_LOG_CONTAINER_STATE_INVALID equ 0C01A002Ah
STATUS_LOG_STATE_INVALID         equ 0C01A002Bh
STATUS_LOG_PINNED                equ 0C01A002Ch
STATUS_LOG_METADATA_FLUSH_FAILED equ 0C01A002Dh
STATUS_LOG_INCONSISTENT_SECURITY equ 0C01A002Eh
STATUS_LOG_APPENDED_FLUSH_FAILED equ 0C01A002Fh
STATUS_LOG_PINNED_RESERVATION    equ 0C01A0030h
STATUS_VIDEO_HUNG_DISPLAY_DRIVER_THREAD equ 0C01B00EAh
STATUS_VIDEO_HUNG_DISPLAY_DRIVER_THREAD_RECOVERED equ 801B00EBh
STATUS_VIDEO_DRIVER_DEBUG_REPORT_REQUEST equ 401B00ECh
STATUS_MONITOR_NO_DESCRIPTOR     equ 0C01D0001h
STATUS_MONITOR_UNKNOWN_DESCRIPTOR_FORMAT equ 0C01D0002h
STATUS_MONITOR_INVALID_DESCRIPTOR_CHECKSUM equ 0C01D0003h
STATUS_MONITOR_INVALID_STANDARD_TIMING_BLOCK equ 0C01D0004h
STATUS_MONITOR_WMI_DATABLOCK_REGISTRATION_FAILED equ 0C01D0005h
STATUS_MONITOR_INVALID_SERIAL_NUMBER_MONDSC_BLOCK equ 0C01D0006h
STATUS_MONITOR_INVALID_USER_FRIENDLY_MONDSC_BLOCK equ 0C01D0007h
STATUS_MONITOR_NO_MORE_DESCRIPTOR_DATA equ 0C01D0008h
STATUS_MONITOR_INVALID_DETAILED_TIMING_BLOCK equ 0C01D0009h
STATUS_GRAPHICS_NOT_EXCLUSIVE_MODE_OWNER equ 0C01E0000h
STATUS_GRAPHICS_INSUFFICIENT_DMA_BUFFER equ 0C01E0001h
STATUS_GRAPHICS_INVALID_DISPLAY_ADAPTER equ 0C01E0002h
STATUS_GRAPHICS_ADAPTER_WAS_RESET equ 0C01E0003h
STATUS_GRAPHICS_INVALID_DRIVER_MODEL equ 0C01E0004h
STATUS_GRAPHICS_PRESENT_MODE_CHANGED equ 0C01E0005h
STATUS_GRAPHICS_PRESENT_OCCLUDED equ 0C01E0006h
STATUS_GRAPHICS_PRESENT_DENIED   equ 0C01E0007h
STATUS_GRAPHICS_CANNOTCOLORCONVERT equ 0C01E0008h
STATUS_GRAPHICS_DRIVER_MISMATCH  equ 0C01E0009h
STATUS_GRAPHICS_PARTIAL_DATA_POPULATED equ 401E000Ah
STATUS_GRAPHICS_NO_VIDEO_MEMORY  equ 0C01E0100h
STATUS_GRAPHICS_CANT_LOCK_MEMORY equ 0C01E0101h
STATUS_GRAPHICS_ALLOCATION_BUSY  equ 0C01E0102h
STATUS_GRAPHICS_TOO_MANY_REFERENCES equ 0C01E0103h
STATUS_GRAPHICS_TRY_AGAIN_LATER  equ 0C01E0104h
STATUS_GRAPHICS_TRY_AGAIN_NOW    equ 0C01E0105h
STATUS_GRAPHICS_ALLOCATION_INVALID equ 0C01E0106h
STATUS_GRAPHICS_UNSWIZZLING_APERTURE_UNAVAILABLE equ 0C01E0107h
STATUS_GRAPHICS_UNSWIZZLING_APERTURE_UNSUPPORTED equ 0C01E0108h
STATUS_GRAPHICS_CANT_EVICT_PINNED_ALLOCATION equ 0C01E0109h
STATUS_GRAPHICS_INVALID_ALLOCATION_USAGE equ 0C01E0110h
STATUS_GRAPHICS_CANT_RENDER_LOCKED_ALLOCATION equ 0C01E0111h
STATUS_GRAPHICS_ALLOCATION_CLOSED equ 0C01E0112h
STATUS_GRAPHICS_INVALID_ALLOCATION_INSTANCE equ 0C01E0113h
STATUS_GRAPHICS_INVALID_ALLOCATION_HANDLE equ 0C01E0114h
STATUS_GRAPHICS_WRONG_ALLOCATION_DEVICE equ 0C01E0115h
STATUS_GRAPHICS_ALLOCATION_CONTENT_LOST equ 0C01E0116h
STATUS_GRAPHICS_GPU_EXCEPTION_ON_DEVICE equ 0C01E0200h
STATUS_GRAPHICS_INVALID_VIDPN_TOPOLOGY equ 0C01E0300h
STATUS_GRAPHICS_VIDPN_TOPOLOGY_NOT_SUPPORTED equ 0C01E0301h
STATUS_GRAPHICS_VIDPN_TOPOLOGY_CURRENTLY_NOT_SUPPORTED equ 0C01E0302h
STATUS_GRAPHICS_INVALID_VIDPN    equ 0C01E0303h
STATUS_GRAPHICS_INVALID_VIDEO_PRESENT_SOURCE equ 0C01E0304h
STATUS_GRAPHICS_INVALID_VIDEO_PRESENT_TARGET equ 0C01E0305h
STATUS_GRAPHICS_VIDPN_MODALITY_NOT_SUPPORTED equ 0C01E0306h
STATUS_GRAPHICS_MODE_NOT_PINNED  equ 401E0307h
STATUS_GRAPHICS_INVALID_VIDPN_SOURCEMODESET equ 0C01E0308h
STATUS_GRAPHICS_INVALID_VIDPN_TARGETMODESET equ 0C01E0309h
STATUS_GRAPHICS_INVALID_FREQUENCY equ 0C01E030Ah
STATUS_GRAPHICS_INVALID_ACTIVE_REGION equ 0C01E030Bh
STATUS_GRAPHICS_INVALID_TOTAL_REGION equ 0C01E030Ch
STATUS_GRAPHICS_INVALID_VIDEO_PRESENT_SOURCE_MODE equ 0C01E0310h
STATUS_GRAPHICS_INVALID_VIDEO_PRESENT_TARGET_MODE equ 0C01E0311h
STATUS_GRAPHICS_PINNED_MODE_MUST_REMAIN_IN_SET equ 0C01E0312h
STATUS_GRAPHICS_PATH_ALREADY_IN_TOPOLOGY equ 0C01E0313h
STATUS_GRAPHICS_MODE_ALREADY_IN_MODESET equ 0C01E0314h
STATUS_GRAPHICS_INVALID_VIDEOPRESENTSOURCESET equ 0C01E0315h
STATUS_GRAPHICS_INVALID_VIDEOPRESENTTARGETSET equ 0C01E0316h
STATUS_GRAPHICS_SOURCE_ALREADY_IN_SET equ 0C01E0317h
STATUS_GRAPHICS_TARGET_ALREADY_IN_SET equ 0C01E0318h
STATUS_GRAPHICS_INVALID_VIDPN_PRESENT_PATH equ 0C01E0319h
STATUS_GRAPHICS_NO_RECOMMENDED_VIDPN_TOPOLOGY equ 0C01E031Ah
STATUS_GRAPHICS_INVALID_MONITOR_FREQUENCYRANGESET equ 0C01E031Bh
STATUS_GRAPHICS_INVALID_MONITOR_FREQUENCYRANGE equ 0C01E031Ch
STATUS_GRAPHICS_FREQUENCYRANGE_NOT_IN_SET equ 0C01E031Dh
STATUS_GRAPHICS_NO_PREFERRED_MODE equ 401E031Eh
STATUS_GRAPHICS_FREQUENCYRANGE_ALREADY_IN_SET equ 0C01E031Fh
STATUS_GRAPHICS_STALE_MODESET    equ 0C01E0320h
STATUS_GRAPHICS_INVALID_MONITOR_SOURCEMODESET equ 0C01E0321h
STATUS_GRAPHICS_INVALID_MONITOR_SOURCE_MODE equ 0C01E0322h
STATUS_GRAPHICS_NO_RECOMMENDED_FUNCTIONAL_VIDPN equ 0C01E0323h
STATUS_GRAPHICS_MODE_ID_MUST_BE_UNIQUE equ 0C01E0324h
STATUS_GRAPHICS_EMPTY_ADAPTER_MONITOR_MODE_SUPPORT_INTERSECTION equ 0C01E0325h
STATUS_GRAPHICS_VIDEO_PRESENT_TARGETS_LESS_THAN_SOURCES equ 0C01E0326h
STATUS_GRAPHICS_PATH_NOT_IN_TOPOLOGY equ 0C01E0327h
STATUS_GRAPHICS_ADAPTER_MUST_HAVE_AT_LEAST_ONE_SOURCE equ 0C01E0328h
STATUS_GRAPHICS_ADAPTER_MUST_HAVE_AT_LEAST_ONE_TARGET equ 0C01E0329h
STATUS_GRAPHICS_INVALID_MONITORDESCRIPTORSET equ 0C01E032Ah
STATUS_GRAPHICS_INVALID_MONITORDESCRIPTOR equ 0C01E032Bh
STATUS_GRAPHICS_MONITORDESCRIPTOR_NOT_IN_SET equ 0C01E032Ch
STATUS_GRAPHICS_MONITORDESCRIPTOR_ALREADY_IN_SET equ 0C01E032Dh
STATUS_GRAPHICS_MONITORDESCRIPTOR_ID_MUST_BE_UNIQUE equ 0C01E032Eh
STATUS_GRAPHICS_INVALID_VIDPN_TARGET_SUBSET_TYPE equ 0C01E032Fh
STATUS_GRAPHICS_RESOURCES_NOT_RELATED equ 0C01E0330h
STATUS_GRAPHICS_SOURCE_ID_MUST_BE_UNIQUE equ 0C01E0331h
STATUS_GRAPHICS_TARGET_ID_MUST_BE_UNIQUE equ 0C01E0332h
STATUS_GRAPHICS_NO_AVAILABLE_VIDPN_TARGET equ 0C01E0333h
STATUS_GRAPHICS_MONITOR_COULD_NOT_BE_ASSOCIATED_WITH_ADAPTER equ 0C01E0334h
STATUS_GRAPHICS_NO_VIDPNMGR      equ 0C01E0335h
STATUS_GRAPHICS_NO_ACTIVE_VIDPN  equ 0C01E0336h
STATUS_GRAPHICS_STALE_VIDPN_TOPOLOGY equ 0C01E0337h
STATUS_GRAPHICS_MONITOR_NOT_CONNECTED equ 0C01E0338h
STATUS_GRAPHICS_SOURCE_NOT_IN_TOPOLOGY equ 0C01E0339h
STATUS_GRAPHICS_INVALID_PRIMARYSURFACE_SIZE equ 0C01E033Ah
STATUS_GRAPHICS_INVALID_VISIBLEREGION_SIZE equ 0C01E033Bh
STATUS_GRAPHICS_INVALID_STRIDE   equ 0C01E033Ch
STATUS_GRAPHICS_INVALID_PIXELFORMAT equ 0C01E033Dh
STATUS_GRAPHICS_INVALID_COLORBASIS equ 0C01E033Eh
STATUS_GRAPHICS_INVALID_PIXELVALUEACCESSMODE equ 0C01E033Fh
STATUS_GRAPHICS_TARGET_NOT_IN_TOPOLOGY equ 0C01E0340h
STATUS_GRAPHICS_NO_DISPLAY_MODE_MANAGEMENT_SUPPORT equ 0C01E0341h
STATUS_GRAPHICS_VIDPN_SOURCE_IN_USE equ 0C01E0342h
STATUS_GRAPHICS_CANT_ACCESS_ACTIVE_VIDPN equ 0C01E0343h
STATUS_GRAPHICS_INVALID_PATH_IMPORTANCE_ORDINAL equ 0C01E0344h
STATUS_GRAPHICS_INVALID_PATH_CONTENT_GEOMETRY_TRANSFORMATION equ 0C01E0345h
STATUS_GRAPHICS_PATH_CONTENT_GEOMETRY_TRANSFORMATION_NOT_SUPPORTED equ 0C01E0346h
STATUS_GRAPHICS_INVALID_GAMMA_RAMP equ 0C01E0347h
STATUS_GRAPHICS_GAMMA_RAMP_NOT_SUPPORTED equ 0C01E0348h
STATUS_GRAPHICS_MULTISAMPLING_NOT_SUPPORTED equ 0C01E0349h
STATUS_GRAPHICS_MODE_NOT_IN_MODESET equ 0C01E034Ah
STATUS_GRAPHICS_DATASET_IS_EMPTY equ 401E034Bh
STATUS_GRAPHICS_NO_MORE_ELEMENTS_IN_DATASET equ 401E034Ch
STATUS_GRAPHICS_INVALID_VIDPN_TOPOLOGY_RECOMMENDATION_REASON equ 0C01E034Dh
STATUS_GRAPHICS_INVALID_PATH_CONTENT_TYPE equ 0C01E034Eh
STATUS_GRAPHICS_INVALID_COPYPROTECTION_TYPE equ 0C01E034Fh
STATUS_GRAPHICS_UNASSIGNED_MODESET_ALREADY_EXISTS equ 0C01E0350h
STATUS_GRAPHICS_PATH_CONTENT_GEOMETRY_TRANSFORMATION_NOT_PINNED equ 401E0351h
STATUS_GRAPHICS_INVALID_SCANLINE_ORDERING equ 0C01E0352h
STATUS_GRAPHICS_TOPOLOGY_CHANGES_NOT_ALLOWED equ 0C01E0353h
STATUS_GRAPHICS_NO_AVAILABLE_IMPORTANCE_ORDINALS equ 0C01E0354h
STATUS_GRAPHICS_INCOMPATIBLE_PRIVATE_FORMAT equ 0C01E0355h
STATUS_GRAPHICS_INVALID_MODE_PRUNING_ALGORITHM equ 0C01E0356h
STATUS_GRAPHICS_INVALID_MONITOR_CAPABILITY_ORIGIN equ 0C01E0357h
STATUS_GRAPHICS_INVALID_MONITOR_FREQUENCYRANGE_CONSTRAINT equ 0C01E0358h
STATUS_GRAPHICS_MAX_NUM_PATHS_REACHED equ 0C01E0359h
STATUS_GRAPHICS_CANCEL_VIDPN_TOPOLOGY_AUGMENTATION equ 0C01E035Ah
STATUS_GRAPHICS_INVALID_CLIENT_TYPE equ 0C01E035Bh
STATUS_GRAPHICS_CLIENTVIDPN_NOT_SET equ 0C01E035Ch
STATUS_GRAPHICS_SPECIFIED_CHILD_ALREADY_CONNECTED equ 0C01E0400h
STATUS_GRAPHICS_CHILD_DESCRIPTOR_NOT_SUPPORTED equ 0C01E0401h
STATUS_GRAPHICS_UNKNOWN_CHILD_STATUS equ 401E042Fh
STATUS_GRAPHICS_NOT_A_LINKED_ADAPTER equ 0C01E0430h
STATUS_GRAPHICS_LEADLINK_NOT_ENUMERATED equ 0C01E0431h
STATUS_GRAPHICS_CHAINLINKS_NOT_ENUMERATED equ 0C01E0432h
STATUS_GRAPHICS_ADAPTER_CHAIN_NOT_READY equ 0C01E0433h
STATUS_GRAPHICS_CHAINLINKS_NOT_STARTED equ 0C01E0434h
STATUS_GRAPHICS_CHAINLINKS_NOT_POWERED_ON equ 0C01E0435h
STATUS_GRAPHICS_INCONSISTENT_DEVICE_LINK_STATE equ 0C01E0436h
STATUS_GRAPHICS_LEADLINK_START_DEFERRED equ 401E0437h
STATUS_GRAPHICS_NOT_POST_DEVICE_DRIVER equ 0C01E0438h
STATUS_GRAPHICS_POLLING_TOO_FREQUENTLY equ 401E0439h
STATUS_GRAPHICS_START_DEFERRED   equ 401E043Ah
STATUS_GRAPHICS_ADAPTER_ACCESS_NOT_EXCLUDED equ 0C01E043Bh
STATUS_GRAPHICS_OPM_NOT_SUPPORTED equ 0C01E0500h
STATUS_GRAPHICS_COPP_NOT_SUPPORTED equ 0C01E0501h
STATUS_GRAPHICS_UAB_NOT_SUPPORTED equ 0C01E0502h
STATUS_GRAPHICS_OPM_INVALID_ENCRYPTED_PARAMETERS equ 0C01E0503h
STATUS_GRAPHICS_OPM_NO_PROTECTED_OUTPUTS_EXIST equ 0C01E0505h
STATUS_GRAPHICS_OPM_INTERNAL_ERROR equ 0C01E050Bh
STATUS_GRAPHICS_OPM_INVALID_HANDLE equ 0C01E050Ch
STATUS_GRAPHICS_PVP_INVALID_CERTIFICATE_LENGTH equ 0C01E050Eh
STATUS_GRAPHICS_OPM_SPANNING_MODE_ENABLED equ 0C01E050Fh
STATUS_GRAPHICS_OPM_THEATER_MODE_ENABLED equ 0C01E0510h
STATUS_GRAPHICS_PVP_HFS_FAILED   equ 0C01E0511h
STATUS_GRAPHICS_OPM_INVALID_SRM  equ 0C01E0512h
STATUS_GRAPHICS_OPM_OUTPUT_DOES_NOT_SUPPORT_HDCP equ 0C01E0513h
STATUS_GRAPHICS_OPM_OUTPUT_DOES_NOT_SUPPORT_ACP equ 0C01E0514h
STATUS_GRAPHICS_OPM_OUTPUT_DOES_NOT_SUPPORT_CGMSA equ 0C01E0515h
STATUS_GRAPHICS_OPM_HDCP_SRM_NEVER_SET equ 0C01E0516h
STATUS_GRAPHICS_OPM_RESOLUTION_TOO_HIGH equ 0C01E0517h
STATUS_GRAPHICS_OPM_ALL_HDCP_HARDWARE_ALREADY_IN_USE equ 0C01E0518h
STATUS_GRAPHICS_OPM_PROTECTED_OUTPUT_NO_LONGER_EXISTS equ 0C01E051Ah
STATUS_GRAPHICS_OPM_PROTECTED_OUTPUT_DOES_NOT_HAVE_COPP_SEMANTICS equ 0C01E051Ch
STATUS_GRAPHICS_OPM_INVALID_INFORMATION_REQUEST equ 0C01E051Dh
STATUS_GRAPHICS_OPM_DRIVER_INTERNAL_ERROR equ 0C01E051Eh
STATUS_GRAPHICS_OPM_PROTECTED_OUTPUT_DOES_NOT_HAVE_OPM_SEMANTICS equ 0C01E051Fh
STATUS_GRAPHICS_OPM_SIGNALING_NOT_SUPPORTED equ 0C01E0520h
STATUS_GRAPHICS_OPM_INVALID_CONFIGURATION_REQUEST equ 0C01E0521h
STATUS_GRAPHICS_I2C_NOT_SUPPORTED equ 0C01E0580h
STATUS_GRAPHICS_I2C_DEVICE_DOES_NOT_EXIST equ 0C01E0581h
STATUS_GRAPHICS_I2C_ERROR_TRANSMITTING_DATA equ 0C01E0582h
STATUS_GRAPHICS_I2C_ERROR_RECEIVING_DATA equ 0C01E0583h
STATUS_GRAPHICS_DDCCI_VCP_NOT_SUPPORTED equ 0C01E0584h
STATUS_GRAPHICS_DDCCI_INVALID_DATA equ 0C01E0585h
STATUS_GRAPHICS_DDCCI_MONITOR_RETURNED_INVALID_TIMING_STATUS_BYTE equ 0C01E0586h
STATUS_GRAPHICS_DDCCI_INVALID_CAPABILITIES_STRING equ 0C01E0587h
STATUS_GRAPHICS_MCA_INTERNAL_ERROR equ 0C01E0588h
STATUS_GRAPHICS_DDCCI_INVALID_MESSAGE_COMMAND equ 0C01E0589h
STATUS_GRAPHICS_DDCCI_INVALID_MESSAGE_LENGTH equ 0C01E058Ah
STATUS_GRAPHICS_DDCCI_INVALID_MESSAGE_CHECKSUM equ 0C01E058Bh
STATUS_GRAPHICS_INVALID_PHYSICAL_MONITOR_HANDLE equ 0C01E058Ch
STATUS_GRAPHICS_MONITOR_NO_LONGER_EXISTS equ 0C01E058Dh
STATUS_GRAPHICS_ONLY_CONSOLE_SESSION_SUPPORTED equ 0C01E05E0h
STATUS_GRAPHICS_NO_DISPLAY_DEVICE_CORRESPONDS_TO_NAME equ 0C01E05E1h
STATUS_GRAPHICS_DISPLAY_DEVICE_NOT_ATTACHED_TO_DESKTOP equ 0C01E05E2h
STATUS_GRAPHICS_MIRRORING_DEVICES_NOT_SUPPORTED equ 0C01E05E3h
STATUS_GRAPHICS_INVALID_POINTER  equ 0C01E05E4h
STATUS_GRAPHICS_NO_MONITORS_CORRESPOND_TO_DISPLAY_DEVICE equ 0C01E05E5h
STATUS_GRAPHICS_PARAMETER_ARRAY_TOO_SMALL equ 0C01E05E6h
STATUS_GRAPHICS_INTERNAL_ERROR   equ 0C01E05E7h
STATUS_GRAPHICS_SESSION_TYPE_CHANGE_IN_PROGRESS equ 0C01E05E8h
STATUS_FVE_LOCKED_VOLUME         equ 0C0210000h
STATUS_FVE_NOT_ENCRYPTED         equ 0C0210001h
STATUS_FVE_BAD_INFORMATION       equ 0C0210002h
STATUS_FVE_TOO_SMALL             equ 0C0210003h
STATUS_FVE_FAILED_WRONG_FS       equ 0C0210004h
STATUS_FVE_FAILED_BAD_FS         equ 0C0210005h
STATUS_FVE_FS_NOT_EXTENDED       equ 0C0210006h
STATUS_FVE_FS_MOUNTED            equ 0C0210007h
STATUS_FVE_NO_LICENSE            equ 0C0210008h
STATUS_FVE_ACTION_NOT_ALLOWED    equ 0C0210009h
STATUS_FVE_BAD_DATA              equ 0C021000Ah
STATUS_FVE_VOLUME_NOT_BOUND      equ 0C021000Bh
STATUS_FVE_NOT_DATA_VOLUME       equ 0C021000Ch
STATUS_FVE_CONV_READ_ERROR       equ 0C021000Dh
STATUS_FVE_CONV_WRITE_ERROR      equ 0C021000Eh
STATUS_FVE_OVERLAPPED_UPDATE     equ 0C021000Fh
STATUS_FVE_FAILED_SECTOR_SIZE    equ 0C0210010h
STATUS_FVE_FAILED_AUTHENTICATION equ 0C0210011h
STATUS_FVE_NOT_OS_VOLUME         equ 0C0210012h
STATUS_FVE_KEYFILE_NOT_FOUND     equ 0C0210013h
STATUS_FVE_KEYFILE_INVALID       equ 0C0210014h
STATUS_FVE_KEYFILE_NO_VMK        equ 0C0210015h
STATUS_FVE_TPM_DISABLED          equ 0C0210016h
STATUS_FVE_TPM_SRK_AUTH_NOT_ZERO equ 0C0210017h
STATUS_FVE_TPM_INVALID_PCR       equ 0C0210018h
STATUS_FVE_TPM_NO_VMK            equ 0C0210019h
STATUS_FVE_PIN_INVALID           equ 0C021001Ah
STATUS_FVE_AUTH_INVALID_APPLICATION equ 0C021001Bh
STATUS_FVE_AUTH_INVALID_CONFIG   equ 0C021001Ch
STATUS_FVE_DEBUGGER_ENABLED      equ 0C021001Dh
STATUS_FVE_DRY_RUN_FAILED        equ 0C021001Eh
STATUS_FVE_BAD_METADATA_POINTER  equ 0C021001Fh
STATUS_FVE_OLD_METADATA_COPY     equ 0C0210020h
STATUS_FVE_REBOOT_REQUIRED       equ 0C0210021h
STATUS_FVE_RAW_ACCESS            equ 0C0210022h
STATUS_FVE_RAW_BLOCKED           equ 0C0210023h
STATUS_FWP_CALLOUT_NOT_FOUND     equ 0C0220001h
STATUS_FWP_CONDITION_NOT_FOUND   equ 0C0220002h
STATUS_FWP_FILTER_NOT_FOUND      equ 0C0220003h
STATUS_FWP_LAYER_NOT_FOUND       equ 0C0220004h
STATUS_FWP_PROVIDER_NOT_FOUND    equ 0C0220005h
STATUS_FWP_PROVIDER_CONTEXT_NOT_FOUND equ 0C0220006h
STATUS_FWP_SUBLAYER_NOT_FOUND    equ 0C0220007h
STATUS_FWP_NOT_FOUND             equ 0C0220008h
STATUS_FWP_ALREADY_EXISTS        equ 0C0220009h
STATUS_FWP_IN_USE                equ 0C022000Ah
STATUS_FWP_DYNAMIC_SESSION_IN_PROGRESS equ 0C022000Bh
STATUS_FWP_WRONG_SESSION         equ 0C022000Ch
STATUS_FWP_NO_TXN_IN_PROGRESS    equ 0C022000Dh
STATUS_FWP_TXN_IN_PROGRESS       equ 0C022000Eh
STATUS_FWP_TXN_ABORTED           equ 0C022000Fh
STATUS_FWP_SESSION_ABORTED       equ 0C0220010h
STATUS_FWP_INCOMPATIBLE_TXN      equ 0C0220011h
STATUS_FWP_TIMEOUT               equ 0C0220012h
STATUS_FWP_NET_EVENTS_DISABLED   equ 0C0220013h
STATUS_FWP_INCOMPATIBLE_LAYER    equ 0C0220014h
STATUS_FWP_KM_CLIENTS_ONLY       equ 0C0220015h
STATUS_FWP_LIFETIME_MISMATCH     equ 0C0220016h
STATUS_FWP_BUILTIN_OBJECT        equ 0C0220017h
STATUS_FWP_TOO_MANY_CALLOUTS     equ 0C0220018h
STATUS_FWP_NOTIFICATION_DROPPED  equ 0C0220019h
STATUS_FWP_TRAFFIC_MISMATCH      equ 0C022001Ah
STATUS_FWP_INCOMPATIBLE_SA_STATE equ 0C022001Bh
STATUS_FWP_NULL_POINTER          equ 0C022001Ch
STATUS_FWP_INVALID_ENUMERATOR    equ 0C022001Dh
STATUS_FWP_INVALID_FLAGS         equ 0C022001Eh
STATUS_FWP_INVALID_NET_MASK      equ 0C022001Fh
STATUS_FWP_INVALID_RANGE         equ 0C0220020h
STATUS_FWP_INVALID_INTERVAL      equ 0C0220021h
STATUS_FWP_ZERO_LENGTH_ARRAY     equ 0C0220022h
STATUS_FWP_NULL_DISPLAY_NAME     equ 0C0220023h
STATUS_FWP_INVALID_ACTION_TYPE   equ 0C0220024h
STATUS_FWP_INVALID_WEIGHT        equ 0C0220025h
STATUS_FWP_MATCH_TYPE_MISMATCH   equ 0C0220026h
STATUS_FWP_TYPE_MISMATCH         equ 0C0220027h
STATUS_FWP_OUT_OF_BOUNDS         equ 0C0220028h
STATUS_FWP_RESERVED              equ 0C0220029h
STATUS_FWP_DUPLICATE_CONDITION   equ 0C022002Ah
STATUS_FWP_DUPLICATE_KEYMOD      equ 0C022002Bh
STATUS_FWP_ACTION_INCOMPATIBLE_WITH_LAYER equ 0C022002Ch
STATUS_FWP_ACTION_INCOMPATIBLE_WITH_SUBLAYER equ 0C022002Dh
STATUS_FWP_CONTEXT_INCOMPATIBLE_WITH_LAYER equ 0C022002Eh
STATUS_FWP_CONTEXT_INCOMPATIBLE_WITH_CALLOUT equ 0C022002Fh
STATUS_FWP_INCOMPATIBLE_AUTH_METHOD equ 0C0220030h
STATUS_FWP_INCOMPATIBLE_DH_GROUP equ 0C0220031h
STATUS_FWP_EM_NOT_SUPPORTED      equ 0C0220032h
STATUS_FWP_NEVER_MATCH           equ 0C0220033h
STATUS_FWP_PROVIDER_CONTEXT_MISMATCH equ 0C0220034h
STATUS_FWP_INVALID_PARAMETER     equ 0C0220035h
STATUS_FWP_TOO_MANY_SUBLAYERS    equ 0C0220036h
STATUS_FWP_CALLOUT_NOTIFICATION_FAILED equ 0C0220037h
STATUS_FWP_INVALID_AUTH_TRANSFORM equ 0C0220038h
STATUS_FWP_INVALID_CIPHER_TRANSFORM equ 0C0220039h
STATUS_FWP_TCPIP_NOT_READY       equ 0C0220100h
STATUS_FWP_INJECT_HANDLE_CLOSING equ 0C0220101h
STATUS_FWP_INJECT_HANDLE_STALE   equ 0C0220102h
STATUS_FWP_CANNOT_PEND           equ 0C0220103h
STATUS_NDIS_CLOSING              equ 0C0230002h
STATUS_NDIS_BAD_VERSION          equ 0C0230004h
STATUS_NDIS_BAD_CHARACTERISTICS  equ 0C0230005h
STATUS_NDIS_ADAPTER_NOT_FOUND    equ 0C0230006h
STATUS_NDIS_OPEN_FAILED          equ 0C0230007h
STATUS_NDIS_DEVICE_FAILED        equ 0C0230008h
STATUS_NDIS_MULTICAST_FULL       equ 0C0230009h
STATUS_NDIS_MULTICAST_EXISTS     equ 0C023000Ah
STATUS_NDIS_MULTICAST_NOT_FOUND  equ 0C023000Bh
STATUS_NDIS_REQUEST_ABORTED      equ 0C023000Ch
STATUS_NDIS_RESET_IN_PROGRESS    equ 0C023000Dh
STATUS_NDIS_NOT_SUPPORTED        equ 0C02300BBh
STATUS_NDIS_INVALID_PACKET       equ 0C023000Fh
STATUS_NDIS_ADAPTER_NOT_READY    equ 0C0230011h
STATUS_NDIS_INVALID_LENGTH       equ 0C0230014h
STATUS_NDIS_INVALID_DATA         equ 0C0230015h
STATUS_NDIS_BUFFER_TOO_SHORT     equ 0C0230016h
STATUS_NDIS_INVALID_OID          equ 0C0230017h
STATUS_NDIS_ADAPTER_REMOVED      equ 0C0230018h
STATUS_NDIS_UNSUPPORTED_MEDIA    equ 0C0230019h
STATUS_NDIS_GROUP_ADDRESS_IN_USE equ 0C023001Ah
STATUS_NDIS_FILE_NOT_FOUND       equ 0C023001Bh
STATUS_NDIS_ERROR_READING_FILE   equ 0C023001Ch
STATUS_NDIS_ALREADY_MAPPED       equ 0C023001Dh
STATUS_NDIS_RESOURCE_CONFLICT    equ 0C023001Eh
STATUS_NDIS_MEDIA_DISCONNECTED   equ 0C023001Fh
STATUS_NDIS_INVALID_ADDRESS      equ 0C0230022h
STATUS_NDIS_INVALID_DEVICE_REQUEST equ 0C0230010h
STATUS_NDIS_PAUSED               equ 0C023002Ah
STATUS_NDIS_INTERFACE_NOT_FOUND  equ 0C023002Bh
STATUS_NDIS_UNSUPPORTED_REVISION equ 0C023002Ch
STATUS_NDIS_INVALID_PORT         equ 0C023002Dh
STATUS_NDIS_INVALID_PORT_STATE   equ 0C023002Eh
STATUS_NDIS_LOW_POWER_STATE      equ 0C023002Fh
STATUS_NDIS_DOT11_AUTO_CONFIG_ENABLED equ 0C0232000h
STATUS_NDIS_DOT11_MEDIA_IN_USE   equ 0C0232001h
STATUS_NDIS_DOT11_POWER_STATE_INVALID equ 0C0232002h
STATUS_NDIS_INDICATION_REQUIRED  equ 40230001h
STATUS_IPSEC_BAD_SPI             equ 0C0360001h
STATUS_IPSEC_SA_LIFETIME_EXPIRED equ 0C0360002h
STATUS_IPSEC_WRONG_SA            equ 0C0360003h
STATUS_IPSEC_REPLAY_CHECK_FAILED equ 0C0360004h
STATUS_IPSEC_INVALID_PACKET      equ 0C0360005h
STATUS_IPSEC_INTEGRITY_CHECK_FAILED equ 0C0360006h
STATUS_IPSEC_CLEAR_TEXT_DROP     equ 0C0360007h

;;;; end ntstatus


;;;; lmserver

;-------------------------------------[ lmcons.h ]--------------------------------------
CNLEN equ 15
LM20_CNLEN equ 15
DNLEN equ CNLEN
LM20_DNLEN equ LM20_CNLEN
UNCLEN equ CNLEN+2
LM20_UNCLEN equ LM20_CNLEN+2
NNLEN equ 80
LM20_NNLEN equ 12
RMLEN equ UNCLEN+1+NNLEN
LM20_RMLEN equ LM20_UNCLEN+1+LM20_NNLEN
SNLEN equ 80
LM20_SNLEN equ 15
STXTLEN equ 256
LM20_STXTLEN equ 63
PATHLEN equ 256
LM20_PATHLEN equ 256
DEVLEN equ 80
LM20_DEVLEN equ 8
EVLEN equ 16
UNLEN equ 256
LM20_UNLEN equ 20
GNLEN equ UNLEN
LM20_GNLEN equ LM20_UNLEN
PWLEN equ 256
LM20_PWLEN equ 14
SHPWLEN equ 8
CLTYPE_LEN equ 12
MAXCOMMENTSZ equ 256
LM20_MAXCOMMENTSZ equ 48
QNLEN equ NNLEN
LM20_QNLEN equ LM20_NNLEN
ALERTSZ equ 128
MAXDEVENTRIES equ sizeof int 4*8
NETBIOS_NAME_LEN equ 16
MAX_PREFERRED_LENGTH equ -1
CRYPT_KEY_LEN equ 7
CRYPT_TXT_LEN equ 8
ENCRYPTED_PWLEN equ 16
SESSION_PWLEN equ 24
SESSION_CRYPT_KLEN equ 21
PARMNUM_ALL equ 0
PARM_ERROR_UNKNOWN equ -1
PARM_ERROR_NONE equ 0
PARMNUM_BASE_INFOLEVEL equ 1000
MESSAGE_FILENAME equ "NETMSG"
OS2MSG_FILENAME equ "BASE"
HELP_MSG_FILENAME equ "NETH"
BACKUP_MSG_FILENAME equ "BAK.MSG"
PLATFORM_ID_DOS equ 300
PLATFORM_ID_OS2 equ 400
PLATFORM_ID_NT equ 500
PLATFORM_ID_OSF equ 600
PLATFORM_ID_VMS equ 700
MIN_LANMAN_MESSAGE_ID equ NERR_BASE
MAX_LANMAN_MESSAGE_ID equ 5799

;==================================== LMServer.h =================================
SV_PLATFORM_ID_OS2 equ 400
SV_PLATFORM_ID_NT equ 500
MAJOR_VERSION_MASK equ 0Fh
SV_TYPE_WORKSTATION equ 1
SV_TYPE_SERVER equ 2
SV_TYPE_SQLSERVER equ 4
SV_TYPE_DOMAIN_CTRL equ 8
SV_TYPE_DOMAIN_BAKCTRL equ 10h
SV_TYPE_TIME_SOURCE equ 20h
SV_TYPE_AFP equ 40h
SV_TYPE_NOVELL equ 80h
SV_TYPE_DOMAIN_MEMBER equ 100h
SV_TYPE_PRINTQ_SERVER equ 200h
SV_TYPE_DIALIN_SERVER equ 400h
SV_TYPE_XENIX_SERVER equ 800h
SV_TYPE_SERVER_UNIX equ SV_TYPE_XENIX_SERVER
SV_TYPE_NT equ 1000h
SV_TYPE_WFW equ 2000h
SV_TYPE_SERVER_MFPN equ 4000h
SV_TYPE_SERVER_NT equ 8000h
SV_TYPE_POTENTIAL_BROWSER equ 10000h
SV_TYPE_BACKUP_BROWSER equ 20000h
SV_TYPE_MASTER_BROWSER equ 40000h
SV_TYPE_DOMAIN_MASTER equ 80000h
SV_TYPE_SERVER_OSF equ 100000h
SV_TYPE_SERVER_VMS equ 200000h
SV_TYPE_WINDOWS equ 400000h
SV_TYPE_DFS equ 800000h
SV_TYPE_CLUSTER_NT equ 1000000h
SV_TYPE_DCE equ 10000000h
SV_TYPE_ALTERNATE_XPORT equ 20000000h
SV_TYPE_LOCAL_LIST_ONLY equ 40000000h
SV_TYPE_DOMAIN_ENUM equ 80000000h
SV_TYPE_ALL equ 0FFFFFFFFh
SV_NODISC equ (-1)
SV_USERSECURITY equ 1
SV_SHARESECURITY equ 0
SV_HIDDEN equ 1
SV_VISIBLE equ 0
SV_PLATFORM_ID_PARMNUM equ 101
SV_NAME_PARMNUM equ 102
SV_VERSION_MAJOR_PARMNUM equ 103
SV_VERSION_MINOR_PARMNUM equ 104
SV_TYPE_PARMNUM equ 105
SV_COMMENT_PARMNUM equ 5
SV_USERS_PARMNUM equ 107
SV_DISC_PARMNUM equ 10
SV_HIDDEN_PARMNUM equ 16
SV_ANNOUNCE_PARMNUM equ 17
SV_ANNDELTA_PARMNUM equ 18
SV_USERPATH_PARMNUM equ 112
SV_ULIST_MTIME_PARMNUM equ 401
SV_GLIST_MTIME_PARMNUM equ 402
SV_ALIST_MTIME_PARMNUM equ 403
SV_ALERTS_PARMNUM equ 11
SV_SECURITY_PARMNUM equ 405
SV_NUMADMIN_PARMNUM equ 406
SV_LANMASK_PARMNUM equ 407
SV_GUESTACC_PARMNUM equ 408
SV_CHDEVQ_PARMNUM equ 410
SV_CHDEVJOBS_PARMNUM equ 411
SV_CONNECTIONS_PARMNUM equ 412
SV_SHARES_PARMNUM equ 413
SV_OPENFILES_PARMNUM equ 414
SV_SESSREQS_PARMNUM equ 417
SV_ACTIVELOCKS_PARMNUM equ 419
SV_NUMREQBUF_PARMNUM equ 420
SV_NUMBIGBUF_PARMNUM equ 422
SV_NUMFILETASKS_PARMNUM equ 423
SV_ALERTSCHED_PARMNUM equ 37
SV_ERRORALERT_PARMNUM equ 38
SV_LOGONALERT_PARMNUM equ 39
SV_ACCESSALERT_PARMNUM equ 40
SV_DISKALERT_PARMNUM equ 41
SV_NETIOALERT_PARMNUM equ 42
SV_MAXAUDITSZ_PARMNUM equ 43
SV_SRVHEURISTICS_PARMNUM equ 431
SV_SESSOPENS_PARMNUM equ 501
SV_SESSVCS_PARMNUM equ 502
SV_OPENSEARCH_PARMNUM equ 503
SV_SIZREQBUF_PARMNUM equ 504
SV_INITWORKITEMS_PARMNUM equ 505
SV_MAXWORKITEMS_PARMNUM equ 506
SV_RAWWORKITEMS_PARMNUM equ 507
SV_IRPSTACKSIZE_PARMNUM equ 508
SV_MAXRAWBUFLEN_PARMNUM equ 509
SV_SESSUSERS_PARMNUM equ 510
SV_SESSCONNS_PARMNUM equ 511
SV_MAXNONPAGEDMEMORYUSAGE_PARMNUM equ 512
SV_MAXPAGEDMEMORYUSAGE_PARMNUM equ 513
SV_ENABLESOFTCOMPAT_PARMNUM equ 514
SV_ENABLEFORCEDLOGOFF_PARMNUM equ 515
SV_TIMESOURCE_PARMNUM equ 516
SV_ACCEPTDOWNLEVELAPIS_PARMNUM equ 517
SV_LMANNOUNCE_PARMNUM equ 518
SV_DOMAIN_PARMNUM equ 519
SV_MAXCOPYREADLEN_PARMNUM equ 520
SV_MAXCOPYWRITELEN_PARMNUM equ 521
SV_MINKEEPSEARCH_PARMNUM equ 522
SV_MAXKEEPSEARCH_PARMNUM equ 523
SV_MINKEEPCOMPLSEARCH_PARMNUM equ 524
SV_MAXKEEPCOMPLSEARCH_PARMNUM equ 525
SV_THREADCOUNTADD_PARMNUM equ 526
SV_NUMBLOCKTHREADS_PARMNUM equ 527
SV_SCAVTIMEOUT_PARMNUM equ 528
SV_MINRCVQUEUE_PARMNUM equ 529
SV_MINFREEWORKITEMS_PARMNUM equ 530
SV_XACTMEMSIZE_PARMNUM equ 531
SV_THREADPRIORITY_PARMNUM equ 532
SV_MAXMPXCT_PARMNUM equ 533
SV_OPLOCKBREAKWAIT_PARMNUM equ 534
SV_OPLOCKBREAKRESPONSEWAIT_PARMNUM equ 535
SV_ENABLEOPLOCKS_PARMNUM equ 536
SV_ENABLEOPLOCKFORCECLOSE_PARMNUM equ 537
SV_ENABLEFCBOPENS_PARMNUM equ 538
SV_ENABLERAW_PARMNUM equ 539
SV_ENABLESHAREDNETDRIVES_PARMNUM equ 540
SV_MINFREECONNECTIONS_PARMNUM equ 541
SV_MAXFREECONNECTIONS_PARMNUM equ 542
SV_INITSESSTABLE_PARMNUM equ 543
SV_INITCONNTABLE_PARMNUM equ 544
SV_INITFILETABLE_PARMNUM equ 545
SV_INITSEARCHTABLE_PARMNUM equ 546
SV_ALERTSCHEDULE_PARMNUM equ 547
SV_ERRORTHRESHOLD_PARMNUM equ 548
SV_NETWORKERRORTHRESHOLD_PARMNUM equ 549
SV_DISKSPACETHRESHOLD_PARMNUM equ 550
SV_MAXLINKDELAY_PARMNUM equ 552
SV_MINLINKTHROUGHPUT_PARMNUM equ 553
SV_LINKINFOVALIDTIME_PARMNUM equ 554
SV_SCAVQOSINFOUPDATETIME_PARMNUM equ 555
SV_MAXWORKITEMIDLETIME_PARMNUM equ 556
SV_MAXRAWWORKITEMS_PARMNUM equ 557
SV_PRODUCTTYPE_PARMNUM equ 560
SV_SERVERSIZE_PARMNUM equ 561
SV_CONNECTIONLESSAUTODISC_PARMNUM equ 562
SV_SHARINGVIOLATIONRETRIES_PARMNUM equ 563
SV_SHARINGVIOLATIONDELAY_PARMNUM equ 564
SV_MAXGLOBALOPENSEARCH_PARMNUM equ 565
SV_REMOVEDUPLICATESEARCHES_PARMNUM equ 566
SV_LOCKVIOLATIONRETRIES_PARMNUM equ 567
SV_LOCKVIOLATIONOFFSET_PARMNUM equ 568
SV_LOCKVIOLATIONDELAY_PARMNUM equ 569
SV_MDLREADSWITCHOVER_PARMNUM equ 570
SV_CACHEDOPENLIMIT_PARMNUM equ 571
SV_CRITICALTHREADS_PARMNUM equ 572
SV_RESTRICTNULLSESSACCESS_PARMNUM equ 573
SV_ENABLEWFW311DIRECTIPX_PARMNUM equ 574
SV_OTHERQUEUEAFFINITY_PARMNUM equ 575
SV_QUEUESAMPLESECS_PARMNUM equ 576
SV_BALANCECOUNT_PARMNUM equ 577
SV_PREFERREDAFFINITY_PARMNUM equ 578
SV_MAXFREERFCBS_PARMNUM equ 579
SV_MAXFREEMFCBS_PARMNUM equ 580
SV_MAXFREELFCBS_PARMNUM equ 581
SV_MAXFREEPAGEDPOOLCHUNKS_PARMNUM equ 582
SV_MINPAGEDPOOLCHUNKSIZE_PARMNUM equ 583
SV_MAXPAGEDPOOLCHUNKSIZE_PARMNUM equ 584
SV_SENDSFROMPREFERREDPROCESSOR_PARMNUM equ 585
SV_MAXTHREADSPERQUEUE_PARMNUM equ 586
SV_CACHEDDIRECTORYLIMIT_PARMNUM equ 587
SV_MAXCOPYLENGTH_PARMNUM equ 588
SV_ENABLEBULKTRANSFER_PARMNUM equ 589
SV_ENABLECOMPRESSION_PARMNUM equ 590
SV_AUTOSHAREWKS_PARMNUM equ 591
SV_AUTOSHARESERVER_PARMNUM equ 592
SV_ENABLESECURITYSIGNATURE_PARMNUM equ 593
SV_REQUIRESECURITYSIGNATURE_PARMNUM equ 594
SV_MINCLIENTBUFFERSIZE_PARMNUM equ 595
SV_CONNECTIONNOSESSIONSTIMEOUT_PARMNUM equ 596
SV_COMMENT_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_COMMENT_PARMNUM)
SV_USERS_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_USERS_PARMNUM)
SV_DISC_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_DISC_PARMNUM)
SV_HIDDEN_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_HIDDEN_PARMNUM)
SV_ANNOUNCE_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_ANNOUNCE_PARMNUM)
SV_ANNDELTA_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_ANNDELTA_PARMNUM)
SV_SESSOPENS_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_SESSOPENS_PARMNUM)
SV_SESSVCS_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_SESSVCS_PARMNUM)
SV_OPENSEARCH_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_OPENSEARCH_PARMNUM)
SV_MAXWORKITEMS_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_MAXWORKITEMS_PARMNUM)
SV_MAXRAWBUFLEN_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_MAXRAWBUFLEN_PARMNUM)
SV_SESSUSERS_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_SESSUSERS_PARMNUM)
SV_SESSCONNS_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_SESSCONNS_PARMNUM)
SV_MAXNONPAGEDMEMORYUSAGE_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_MAXNONPAGEDMEMORYUSAGE_PARMNUM)
SV_MAXPAGEDMEMORYUSAGE_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_MAXPAGEDMEMORYUSAGE_PARMNUM)
SV_ENABLESOFTCOMPAT_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_ENABLESOFTCOMPAT_PARMNUM)
SV_ENABLEFORCEDLOGOFF_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_ENABLEFORCEDLOGOFF_PARMNUM)
SV_TIMESOURCE_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_TIMESOURCE_PARMNUM)
SV_LMANNOUNCE_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_LMANNOUNCE_PARMNUM)
SV_MAXCOPYREADLEN_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_MAXCOPYREADLEN_PARMNUM)
SV_MAXCOPYWRITELEN_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_MAXCOPYWRITELEN_PARMNUM)
SV_MINKEEPSEARCH_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_MINKEEPSEARCH_PARMNUM)
SV_MAXKEEPSEARCH_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_MAXKEEPSEARCH_PARMNUM)
SV_MINKEEPCOMPLSEARCH_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_MINKEEPCOMPLSEARCH_PARMNUM)
SV_MAXKEEPCOMPLSEARCH_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_MAXKEEPCOMPLSEARCH_PARMNUM)
SV_SCAVTIMEOUT_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_SCAVTIMEOUT_PARMNUM)
SV_MINRCVQUEUE_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_MINRCVQUEUE_PARMNUM)
SV_MINFREEWORKITEMS_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_MINFREEWORKITEMS_PARMNUM)
SV_MAXMPXCT_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_MAXMPXCT_PARMNUM)
SV_OPLOCKBREAKWAIT_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_OPLOCKBREAKWAIT_PARMNUM)
SV_OPLOCKBREAKRESPONSEWAIT_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_OPLOCKBREAKRESPONSEWAIT_PARMNUM)
SV_ENABLEOPLOCKS_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_ENABLEOPLOCKS_PARMNUM)
SV_ENABLEOPLOCKFORCECLOSE_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_ENABLEOPLOCKFORCECLOSE_PARMNUM)
SV_ENABLEFCBOPENS_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_ENABLEFCBOPENS_PARMNUM)
SV_ENABLERAW_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_ENABLERAW_PARMNUM)
SV_ENABLESHAREDNETDRIVES_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_ENABLESHAREDNETDRIVES_PARMNUM)
SV_MINFREECONNECTIONS_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_MINFREECONNECTIONS_PARMNUM)
SV_MAXFREECONNECTIONS_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_MAXFREECONNECTIONS_PARMNUM)
SV_INITSESSTABLE_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_INITSESSTABLE_PARMNUM)
SV_INITCONNTABLE_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_INITCONNTABLE_PARMNUM)
SV_INITFILETABLE_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_INITFILETABLE_PARMNUM)
SV_INITSEARCHTABLE_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_INITSEARCHTABLE_PARMNUM)
SV_ALERTSCHEDULE_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_ALERTSCHEDULE_PARMNUM)
SV_ERRORTHRESHOLD_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_ERRORTHRESHOLD_PARMNUM)
SV_NETWORKERRORTHRESHOLD_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_NETWORKERRORTHRESHOLD_PARMNUM)
SV_DISKSPACETHRESHOLD_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_DISKSPACETHRESHOLD_PARMNUM)
SV_MAXLINKDELAY_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_MAXLINKDELAY_PARMNUM)
SV_MINLINKTHROUGHPUT_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_MINLINKTHROUGHPUT_PARMNUM)
SV_LINKINFOVALIDTIME_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_LINKINFOVALIDTIME_PARMNUM)
SV_SCAVQOSINFOUPDATETIME_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_SCAVQOSINFOUPDATETIME_PARMNUM)
SV_MAXWORKITEMIDLETIME_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_MAXWORKITEMIDLETIME_PARMNUM)
SV_MAXRAWWORKITEMS_INFOLOEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_MAXRAWWORKITEMS_PARMNUM)
SV_PRODUCTTYPE_INFOLOEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_PRODUCTTYPE_PARMNUM)
SV_SERVERSIZE_INFOLOEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_SERVERSIZE_PARMNUM)
SV_CONNECTIONLESSAUTODISC_INFOLOEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_CONNECTIONLESSAUTODISC_PARMNUM)
SV_SHARINGVIOLATIONRETRIES_INFOLOEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_SHARINGVIOLATIONRETRIES_PARMNUM)
SV_SHARINGVIOLATIONDELAY_INFOLOEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_SHARINGVIOLATIONDELAY_PARMNUM)
SV_MAXGLOBALOPENSEARCH_INFOLOEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_MAXGLOBALOPENSEARCH_PARMNUM)
SV_REMOVEDUPLICATESEARCHES_INFOLOEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_REMOVEDUPLICATESEARCHES_PARMNUM)
SV_LOCKVIOLATIONRETRIES_INFOLOEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_LOCKVIOLATIONRETRIES_PARMNUM)
SV_LOCKVIOLATIONOFFSET_INFOLOEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_LOCKVIOLATIONOFFSET_PARMNUM)
SV_LOCKVIOLATIONDELAY_INFOLOEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_LOCKVIOLATIONDELAY_PARMNUM)
SV_MDLREADSWITCHOVER_INFOLOEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_MDLREADSWITCHOVER_PARMNUM)
SV_CACHEDOPENLIMIT_INFOLOEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_CACHEDOPENLIMIT_PARMNUM)
SV_CRITICALTHREADS_INFOLOEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_CRITICALTHREADS_PARMNUM)
SV_RESTRICTNULLSESSACCESS_INFOLOEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_RESTRICTNULLSESSACCESS_PARMNUM)
SV_ENABLEWFW311DIRECTIPX_INFOLOEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_ENABLEWFW311DIRECTIPX_PARMNUM)
SV_OTHERQUEUEAFFINITY_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_OTHERQUEUEAFFINITY_PARMNUM)
SV_QUEUESAMPLESECS_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_QUEUESAMPLESECS_PARMNUM)
SV_BALANCECOUNT_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_BALANCECOUNT_PARMNUM)
SV_PREFERREDAFFINITY_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_PREFERREDAFFINITY_PARMNUM)
SV_MAXFREERFCBS_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_MAXFREERFCBS_PARMNUM)
SV_MAXFREEMFCBS_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_MAXFREEMFCBS_PARMNUM)
SV_MAXFREELFCBS_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_MAXFREELFCBS_PARMNUM)
SV_MAXFREEPAGEDPOOLCHUNKS_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_MAXFREEPAGEDPOOLCHUNKS_PARMNUM)
SV_MINPAGEDPOOLCHUNKSIZE_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_MINPAGEDPOOLCHUNKSIZE_PARMNUM)
SV_MAXPAGEDPOOLCHUNKSIZE_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_MAXPAGEDPOOLCHUNKSIZE_PARMNUM)
SV_SENDSFROMPREFERREDPROCESSOR_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_SENDSFROMPREFERREDPROCESSOR_PARMNUM)
SV_MAXTHREADSPERQUEUE_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_MAXTHREADSPERQUEUE_PARMNUM)
SV_CACHEDDIRECTORYLIMIT_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_CACHEDDIRECTORYLIMIT_PARMNUM)
SV_MAXCOPYLENGTH_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_MAXCOPYLENGTH_PARMNUM)
SV_ENABLEBULKTRANSFER_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_ENABLEBULKTRANSFER_PARMNUM)
SV_ENABLECOMPRESSION_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_ENABLECOMPRESSION_PARMNUM)
SV_AUTOSHAREWKS_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_AUTOSHAREWKS_PARMNUM)
SV_AUTOSHARESERVER_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_AUTOSHARESERVER_PARMNUM)
SV_ENABLESECURITYSIGNATURE_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_ENABLESECURITYSIGNATURE_PARMNUM)
SV_REQUIRESECURITYSIGNATURE_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_REQUIRESECURITYSIGNATURE_PARMNUM)
SV_MINCLIENTBUFFERSIZE_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_MINCLIENTBUFFERSIZE_PARMNUM)
SV_CONNECTIONNOSESSIONSTIMEOUT_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SV_CONNECTIONNOSESSIONSTIMEOUT_PARMNUM)
SVI1_NUM_ELEMENTS equ 5
SVI2_NUM_ELEMENTS equ 40
SVI3_NUM_ELEMENTS equ 44
SV_MAX_CMD_LEN equ PATHLEN
SW_AUTOPROF_LOAD_MASK equ 1
SW_AUTOPROF_SAVE_MASK equ 2
SV_MAX_SRV_HEUR_LEN equ 32      
SV_USERS_PER_LICENSE equ 5
SVTI2_REMAP_PIPE_NAMES equ 2

SERVER_INFO_100 STRUCT
    sv100_platform_id dd ?
    sv100_name dd ?
SERVER_INFO_100 ENDS

SERVER_INFO_101 STRUCT
    sv101_platform_id dd ?
    sv101_name dd ?
    sv101_version_major dd ?
    sv101_version_minor dd ?
    sv101_type dd ?
    sv101_comment dd ?
SERVER_INFO_101 ENDS

SERVER_INFO_102 STRUCT
     sv102_platform_id dd ?
     sv102_name dd ?
     sv102_version_major dd ?
     sv102_version_minor dd ?
     sv102_type dd ?
     sv102_comment dd ?
     sv102_users dd ?
     sv102_disc dd ?
     sv102_hidden dd ?
     sv102_announce dd ?
     sv102_anndelta dd ?
     sv102_licenses dd ?
     sv102_userpath dd ?
SERVER_INFO_102 ENDS

SERVER_INFO_402  STRUCT
     sv402_ulist_mtime dd ?
     sv402_glist_mtime dd ?
     sv402_alist_mtime dd ?
     sv402_alerts dd ?
     sv402_security dd ?
     sv402_numadmin dd ?
     sv402_lanmask dd ?
     sv402_guestacct dd ?
     sv402_chdevs dd ?
     sv402_chdevq dd ?
     sv402_chdevjobs dd ?
     sv402_connections dd ?
     sv402_shares dd ?
     sv402_openfiles dd ?
     sv402_sessopens dd ?
     sv402_sessvcs dd ?
     sv402_sessreqs dd ?
     sv402_opensearch dd ?
     sv402_activelocks dd ?
     sv402_numreqbuf dd ?
     sv402_sizreqbuf dd ?
     sv402_numbigbuf dd ?
     sv402_numfiletasks dd ?
     sv402_alertsched dd ?
     sv402_erroralert dd ?
     sv402_logonalert dd ?
     sv402_accessalert dd ?
     sv402_diskalert dd ?
     sv402_netioalert dd ?
     sv402_maxauditsz dd ?
     sv402_srvheuristics dd ?
SERVER_INFO_402 ENDS

SERVER_INFO_403 STRUCT
     sv403_ulist_mtime dd ?
     sv403_glist_mtime dd ?
     sv403_alist_mtime dd ?
     sv403_alerts dd ?
     sv403_security dd ?
     sv403_numadmin dd ?
     sv403_lanmask dd ?
     sv403_guestacct dd ?
     sv403_chdevs dd ?
     sv403_chdevq dd ?
     sv403_chdevjobs dd ?
     sv403_connections dd ?
     sv403_shares dd ?
     sv403_openfiles dd ?
     sv403_sessopens dd ?
     sv403_sessvcs dd ?
     sv403_sessreqs dd ?
     sv403_opensearch dd ?
     sv403_activelocks dd ?
     sv403_numreqbuf dd ?
     sv403_sizreqbuf dd ?
     sv403_numbigbuf dd ?
     sv403_numfiletasks dd ?
     sv403_alertsched dd ?
     sv403_erroralert dd ?
     sv403_logonalert dd ?
     sv403_accessalert dd ?
     sv403_diskalert dd ?
     sv403_netioalert dd ?
     sv403_maxauditsz dd ?
     sv403_srvheuristics dd ?
     sv403_auditedevents dd ?
     sv403_autoprofile dd ?
     sv403_autopath dd ?
SERVER_INFO_403 ENDS

SERVER_INFO_502 STRUCT
    sv502_sessopens dd ?
    sv502_sessvcs dd ?
    sv502_opensearch dd ?
    sv502_sizreqbuf dd ?
    sv502_initworkitems dd ?
    sv502_maxworkitems dd ?
    sv502_rawworkitems dd ?
    sv502_irpstacksize dd ?
    sv502_maxrawbuflen dd ?
    sv502_sessusers dd ?
    sv502_sessconns dd ?
    sv502_maxpagedmemoryusage dd ?
    sv502_maxnonpagedmemoryusage dd ?
    sv502_enablesoftcompat dd ?
    sv502_enableforcedlogoff dd ?
    sv502_timesource dd ?
    sv502_acceptdownlevelapis dd ?
    sv502_lmannounce dd ?
SERVER_INFO_502 ENDS

SERVER_INFO_503 STRUCT
    sv503_sessopens dd ?
    sv503_sessvcs dd ?
    sv503_opensearch dd ?
    sv503_sizreqbuf dd ?
    sv503_initworkitems dd ?
    sv503_maxworkitems dd ?
    sv503_rawworkitems dd ?
    sv503_irpstacksize dd ?
    sv503_maxrawbuflen dd ?
    sv503_sessusers dd ?
    sv503_sessconns dd ?
    sv503_maxpagedmemoryusage dd ?
    sv503_maxnonpagedmemoryusage dd ?
    sv503_enablesoftcompat dd ?
    sv503_enableforcedlogoff dd ?
    sv503_timesource dd ?
    sv503_acceptdownlevelapis dd ?
    sv503_lmannounce dd ?
    sv503_domain dd ?
    sv503_maxcopyreadlen dd ?
    sv503_maxcopywritelen dd ?
    sv503_minkeepsearch dd ?
    sv503_maxkeepsearch dd ?
    sv503_minkeepcomplsearch dd ?
    sv503_maxkeepcomplsearch dd ?
    sv503_threadcountadd dd ?
    sv503_numblockthreads dd ?
    sv503_scavtimeout dd ?
    sv503_minrcvqueue dd ?
    sv503_minfreeworkitems dd ?
    sv503_xactmemsize dd ?
    sv503_threadpriority dd ?
    sv503_maxmpxct dd ?
    sv503_oplockbreakwait dd ?
    sv503_oplockbreakresponsewait dd ?
    sv503_enableoplocks dd ?
    sv503_enableoplockforceclose dd ?
    sv503_enablefcbopens dd ?
    sv503_enableraw dd ?
    sv503_enablesharednetdrives dd ?
    sv503_minfreeconnections dd ?
    sv503_maxfreeconnections dd ?
SERVER_INFO_503 ENDS

SERVER_INFO_599 STRUCT
    sv599_sessopens dd ?
    sv599_sessvcs dd ?
    sv599_opensearch dd ?
    sv599_sizreqbuf dd ?
    sv599_initworkitems dd ?
    sv599_maxworkitems dd ?
    sv599_rawworkitems dd ?
    sv599_irpstacksize dd ?
    sv599_maxrawbuflen dd ?
    sv599_sessusers dd ?
    sv599_sessconns dd ?
    sv599_maxpagedmemoryusage dd ?
    sv599_maxnonpagedmemoryusage dd ?
    sv599_enablesoftcompat dd ?
    sv599_enableforcedlogoff dd ?
    sv599_timesource dd ?
    sv599_acceptdownlevelapis dd ?
    sv599_lmannounce dd ?
    sv599_domain dd ?
    sv599_maxcopyreadlen dd ?
    sv599_maxcopywritelen dd ?
    sv599_minkeepsearch dd ?
    sv599_maxkeepsearch dd ?
    sv599_minkeepcomplsearch dd ?
    sv599_maxkeepcomplsearch dd ?
    sv599_threadcountadd dd ?
    sv599_numblockthreads dd ?
    sv599_scavtimeout dd ?
    sv599_minrcvqueue dd ?
    sv599_minfreeworkitems dd ?
    sv599_xactmemsize dd ?
    sv599_threadpriority dd ?
    sv599_maxmpxct dd ?
    sv599_oplockbreakwait dd ?
    sv599_oplockbreakresponsewait dd ?
    sv599_enableoplocks dd ?
    sv599_enableoplockforceclose dd ?
    sv599_enablefcbopens dd ?
    sv599_enableraw dd ?
    sv599_enablesharednetdrives dd ?
    sv599_minfreeconnections dd ?
    sv599_maxfreeconnections dd ?
    sv599_initsesstable dd ?
    sv599_initconntable dd ?
    sv599_initfiletable dd ?
    sv599_initsearchtable dd ?
    sv599_alertschedule dd ?
    sv599_errorthreshold dd ?
    sv599_networkerrorthreshold dd ?
    sv599_diskspacethreshold dd ?
    sv599_reserved dd ?
    sv599_maxlinkdelay dd ?
    sv599_minlinkthroughput dd ?
    sv599_linkinfovalidtime dd ?
    sv599_scavqosinfoupdatetime dd ?
    sv599_maxworkitemidletime dd ?
SERVER_INFO_599 ENDS

SERVER_INFO_598 STRUCT
    sv598_maxrawworkitems dd ?
    sv598_maxthreadsperqueue dd ?
    sv598_producttype dd ?
    sv598_serversize dd ?
    sv598_connectionlessautodisc dd ?
    sv598_sharingviolationretries dd ?
    sv598_sharingviolationdelay dd ?
    sv598_maxglobalopensearch dd ?
    sv598_removeduplicatesearches dd ?
    sv598_lockviolationoffset dd ?
    sv598_lockviolationdelay dd ?
    sv598_mdlreadswitchover dd ?
    sv598_cachedopenlimit dd ?
    sv598_otherqueueaffinity dd ?
    sv598_restrictnullsessaccess dd ?
    sv598_enablewfw311directipx dd ?
    sv598_queuesamplesecs dd ?
    sv598_balancecount dd ?
    sv598_preferredaffinity dd ?
    sv598_maxfreerfcbs dd ?
    sv598_maxfreemfcbs dd ?
    sv598_maxfreelfcbs dd ?
    sv598_maxfreepagedpoolchunks dd ?
    sv598_minpagedpoolchunksize dd ?
    sv598_maxpagedpoolchunksize dd ?
    sv598_sendsfrompreferredprocessor dd ?
    sv598_cacheddirectorylimit dd ?
    sv598_maxcopylength dd ?
    sv598_enablebulktransfer dd ?
    sv598_enablecompression dd ?
    sv598_autosharewks dd ?
    sv598_autoshareserver dd ?
    sv598_enablesecuritysignature dd ?
    sv598_requiresecuritysignature dd ?
    sv598_minclientbuffersize dd ?
    sv598_serverguid GUID <>
    sv598_ConnectionNoSessionsTimeout dd ?
SERVER_INFO_598 ENDS

SERVER_INFO_1005 STRUCT
    sv1005_comment dd ?
SERVER_INFO_1005 ENDS

SERVER_INFO_1107 STRUCT
    sv1107_users dd ?
SERVER_INFO_1107 ENDS

SERVER_INFO_1010 STRUCT
    sv1010_disc dd ?
SERVER_INFO_1010 ENDS

SERVER_INFO_1016 STRUCT
    sv1016_hidden dd ?
SERVER_INFO_1016 ENDS

SERVER_INFO_1017 STRUCT
    sv1017_announce dd ?
SERVER_INFO_1017 ENDS

SERVER_INFO_1018 STRUCT
    sv1018_anndelta dd ?
SERVER_INFO_1018 ENDS

SERVER_INFO_1501 STRUCT
    sv1501_sessopens dd ?
SERVER_INFO_1501 ENDS

SERVER_INFO_1502 STRUCT
    sv1502_sessvcs dd ?
SERVER_INFO_1502 ENDS

SERVER_INFO_1503 STRUCT
    sv1503_opensearch dd ?
SERVER_INFO_1503 ENDS

SERVER_INFO_1506 STRUCT
    sv1506_maxworkitems dd ?
SERVER_INFO_1506 ENDS

SERVER_INFO_1509 STRUCT
    sv1509_maxrawbuflen dd ?
SERVER_INFO_1509 ENDS

SERVER_INFO_1510 STRUCT
    sv1510_sessusers dd ?
SERVER_INFO_1510 ENDS

SERVER_INFO_1511 STRUCT
    sv1511_sessconns dd ?
SERVER_INFO_1511 ENDS

SERVER_INFO_1512 STRUCT
    sv1512_maxnonpagedmemoryusage dd ?
SERVER_INFO_1512 ENDS

SERVER_INFO_1513 STRUCT
    sv1513_maxpagedmemoryusage dd ?
SERVER_INFO_1513 ENDS

SERVER_INFO_1514 STRUCT
    sv1514_enablesoftcompat dd ?
SERVER_INFO_1514 ENDS

SERVER_INFO_1515 STRUCT
    sv1515_enableforcedlogoff dd ?
SERVER_INFO_1515 ENDS

SERVER_INFO_1516 STRUCT
    sv1516_timesource dd ?
SERVER_INFO_1516 ENDS

SERVER_INFO_1518 STRUCT
    sv1518_lmannounce dd ?
SERVER_INFO_1518 ENDS

SERVER_INFO_1520 STRUCT
    sv1520_maxcopyreadlen dd ?
SERVER_INFO_1520 ENDS

SERVER_INFO_1521 STRUCT
    sv1521_maxcopywritelen dd ?
SERVER_INFO_1521 ENDS

SERVER_INFO_1522 STRUCT
    sv1522_minkeepsearch dd ?
SERVER_INFO_1522 ENDS

SERVER_INFO_1523 STRUCT
    sv1523_maxkeepsearch dd ?
SERVER_INFO_1523 ENDS

SERVER_INFO_1524 STRUCT
    sv1524_minkeepcomplsearch dd ?
SERVER_INFO_1524 ENDS

SERVER_INFO_1525 STRUCT
    sv1525_maxkeepcomplsearch dd ?
SERVER_INFO_1525 ENDS

SERVER_INFO_1528 STRUCT
    sv1528_scavtimeout dd ?
SERVER_INFO_1528 ENDS

SERVER_INFO_1529 STRUCT
    sv1529_minrcvqueue dd ?
SERVER_INFO_1529 ENDS

SERVER_INFO_1530 STRUCT
    sv1530_minfreeworkitems dd ?
SERVER_INFO_1530 ENDS

SERVER_INFO_1533 STRUCT
    sv1533_maxmpxct dd ?
SERVER_INFO_1533 ENDS

SERVER_INFO_1534 STRUCT
    sv1534_oplockbreakwait dd ?
SERVER_INFO_1534 ENDS

SERVER_INFO_1535 STRUCT
    sv1535_oplockbreakresponsewait dd ?
SERVER_INFO_1535 ENDS

SERVER_INFO_1536 STRUCT
    sv1536_enableoplocks dd ?
SERVER_INFO_1536 ENDS

SERVER_INFO_1537 STRUCT
    sv1537_enableoplockforceclose dd ?
SERVER_INFO_1537 ENDS

SERVER_INFO_1538 STRUCT
    sv1538_enablefcbopens dd ?
SERVER_INFO_1538 ENDS

SERVER_INFO_1539 STRUCT
    sv1539_enableraw dd ?
SERVER_INFO_1539 ENDS

SERVER_INFO_1540 STRUCT
    sv1540_enablesharednetdrives dd ?
SERVER_INFO_1540 ENDS

SERVER_INFO_1541 STRUCT
    sv1541_minfreeconnections dd ?
SERVER_INFO_1541 ENDS

SERVER_INFO_1542 STRUCT
    sv1542_maxfreeconnections dd ?
SERVER_INFO_1542 ENDS

SERVER_INFO_1543 STRUCT
    sv1543_initsesstable dd ?
SERVER_INFO_1543 ENDS

SERVER_INFO_1544 STRUCT
    sv1544_initconntable dd ?
SERVER_INFO_1544 ENDS

SERVER_INFO_1545 STRUCT
    sv1545_initfiletable dd ?
SERVER_INFO_1545 ENDS

SERVER_INFO_1546 STRUCT
    sv1546_initsearchtable dd ?
SERVER_INFO_1546 ENDS

SERVER_INFO_1547 STRUCT
    sv1547_alertschedule dd ?
SERVER_INFO_1547 ENDS

SERVER_INFO_1548 STRUCT
    sv1548_errorthreshold dd ?
SERVER_INFO_1548 ENDS

SERVER_INFO_1549 STRUCT
    sv1549_networkerrorthreshold dd ?
SERVER_INFO_1549 ENDS

SERVER_INFO_1550 STRUCT
    sv1550_diskspacethreshold dd ?
SERVER_INFO_1550 ENDS

SERVER_INFO_1552 STRUCT
    sv1552_maxlinkdelay dd ?
SERVER_INFO_1552 ENDS

SERVER_INFO_1553 STRUCT
    sv1553_minlinkthroughput dd ?
SERVER_INFO_1553 ENDS

SERVER_INFO_1554 STRUCT
    sv1554_linkinfovalidtime dd ?
SERVER_INFO_1554 ENDS

SERVER_INFO_1555 STRUCT
    sv1555_scavqosinfoupdatetime dd ?
SERVER_INFO_1555 ENDS

SERVER_INFO_1556 STRUCT
    sv1556_maxworkitemidletime dd ?
SERVER_INFO_1556 ENDS

SERVER_INFO_1557 STRUCT
    sv1557_maxrawworkitems dd ?
SERVER_INFO_1557 ENDS

SERVER_INFO_1560 STRUCT
    sv1560_producttype dd ?
SERVER_INFO_1560 ENDS

SERVER_INFO_1561 STRUCT
    sv1561_serversize dd ?
SERVER_INFO_1561 ENDS

SERVER_INFO_1562 STRUCT
    sv1562_connectionlessautodisc dd ?
SERVER_INFO_1562 ENDS

SERVER_INFO_1563 STRUCT
    sv1563_sharingviolationretries dd ?
SERVER_INFO_1563 ENDS

SERVER_INFO_1564 STRUCT
    sv1564_sharingviolationdelay dd ?
SERVER_INFO_1564 ENDS

SERVER_INFO_1565 STRUCT
    sv1565_maxglobalopensearch dd ?
SERVER_INFO_1565 ENDS

SERVER_INFO_1566 STRUCT
    sv1566_removeduplicatesearches dd ?
SERVER_INFO_1566 ENDS

SERVER_INFO_1567 STRUCT
    sv1567_lockviolationretries dd ?
SERVER_INFO_1567 ENDS

SERVER_INFO_1568 STRUCT
    sv1568_lockviolationoffset dd ?
SERVER_INFO_1568 ENDS

SERVER_INFO_1569 STRUCT
    sv1569_lockviolationdelay dd ?
SERVER_INFO_1569 ENDS

SERVER_INFO_1570 STRUCT
    sv1570_mdlreadswitchover dd ?
SERVER_INFO_1570 ENDS

SERVER_INFO_1571 STRUCT
    sv1571_cachedopenlimit dd ?
SERVER_INFO_1571 ENDS

SERVER_INFO_1572 STRUCT
    sv1572_criticalthreads dd ?
SERVER_INFO_1572 ENDS

SERVER_INFO_1573 STRUCT
    sv1573_restrictnullsessaccess dd ?
SERVER_INFO_1573 ENDS

SERVER_INFO_1574 STRUCT
    sv1574_enablewfw311directipx dd ?
SERVER_INFO_1574 ENDS

SERVER_INFO_1575 STRUCT
    sv1575_otherqueueaffinity dd ?
SERVER_INFO_1575 ENDS

SERVER_INFO_1576 STRUCT
    sv1576_queuesamplesecs dd ?
SERVER_INFO_1576 ENDS

SERVER_INFO_1577 STRUCT
    sv1577_balancecount dd ?
SERVER_INFO_1577 ENDS

SERVER_INFO_1578 STRUCT
    sv1578_preferredaffinity dd ?
SERVER_INFO_1578 ENDS

SERVER_INFO_1579 STRUCT
    sv1579_maxfreerfcbs dd ?
SERVER_INFO_1579 ENDS

SERVER_INFO_1580 STRUCT
    sv1580_maxfreemfcbs dd ?
SERVER_INFO_1580 ENDS

SERVER_INFO_1581 STRUCT
    sv1581_maxfreemlcbs dd ?
SERVER_INFO_1581 ENDS

SERVER_INFO_1582 STRUCT
    sv1582_maxfreepagedpoolchunks dd ?
SERVER_INFO_1582 ENDS

SERVER_INFO_1583 STRUCT
    sv1583_minpagedpoolchunksize dd ?
SERVER_INFO_1583 ENDS

SERVER_INFO_1584 STRUCT
    sv1584_maxpagedpoolchunksize dd ?
SERVER_INFO_1584 ENDS

SERVER_INFO_1585 STRUCT
    sv1585_sendsfrompreferredprocessor dd ?
SERVER_INFO_1585 ENDS

SERVER_INFO_1586 STRUCT
    sv1586_maxthreadsperqueue dd ?
SERVER_INFO_1586 ENDS

SERVER_INFO_1587 STRUCT
    sv1587_cacheddirectorylimit dd ?
SERVER_INFO_1587 ENDS

SERVER_INFO_1588 STRUCT
    sv1588_maxcopylength dd ?
SERVER_INFO_1588 ENDS

SERVER_INFO_1589 STRUCT
    sv1589_enablebulktransfer dd ?
SERVER_INFO_1589 ENDS

SERVER_INFO_1590 STRUCT
    sv1590_enablecompression dd ?
SERVER_INFO_1590 ENDS

SERVER_INFO_1591 STRUCT
    sv1591_autosharewks dd ?
SERVER_INFO_1591 ENDS

SERVER_INFO_1592 STRUCT
    sv1592_autosharewks dd ?
SERVER_INFO_1592 ENDS

SERVER_INFO_1593 STRUCT
    sv1593_enablesecuritysignature dd ?
SERVER_INFO_1593 ENDS

SERVER_INFO_1594 STRUCT
    sv1594_requiresecuritysignature dd ?
SERVER_INFO_1594 ENDS

SERVER_INFO_1595 STRUCT
    sv1595_minclientbuffersize dd ?
SERVER_INFO_1595 ENDS

SERVER_INFO_1596 STRUCT
    sv1596_ConnectionNoSessionsTimeout dd ?
SERVER_INFO_1596 ENDS

SERVER_TRANSPORT_INFO_0 STRUCT
    svti0_numberofvcs dd ?
    svti0_transportname dd ?
    svti0_transportaddress dd ?
    svti0_transportaddresslength dd ?
    svti0_networkaddress dd ?
SERVER_TRANSPORT_INFO_0 ENDS

SERVER_TRANSPORT_INFO_1 STRUCT
    svti1_numberofvcs dd ?
    svti1_transportname dd ?
    svti1_transportaddress dd ?
    svti1_transportaddresslength dd ?
    svti1_networkaddress dd ?
    svti1_domain dd ?
SERVER_TRANSPORT_INFO_1 ENDS

SERVER_TRANSPORT_INFO_2 STRUCT
    svti2_numberofvcs dd ?
    svti2_transportname dd ?
    svti2_transportaddress dd ?
    svti2_transportaddresslength dd ?
    svti2_networkaddress dd ?
    svti2_domain dd ?
    svti2_flags dd ?
SERVER_TRANSPORT_INFO_2 ENDS

;------------------------------------------------------ LMERR.H --------------------------------------------
NERR_Success equ 0
NERR_BASE equ 2100
NERR_NetNotStarted equ (NERR_BASE+2)
NERR_UnknownServer equ (NERR_BASE+3)
NERR_ShareMem equ (NERR_BASE+4)
NERR_NoNetworkResource equ (NERR_BASE+5)
NERR_RemoteOnly equ (NERR_BASE+6)
NERR_DevNotRedirected equ (NERR_BASE+7)
NERR_ServerNotStarted equ (NERR_BASE+14)
NERR_ItemNotFound equ (NERR_BASE+15)
NERR_UnknownDevDir equ (NERR_BASE+16)
NERR_RedirectedPath equ (NERR_BASE+17)
NERR_DuplicateShare equ (NERR_BASE+18)
NERR_NoRoom equ (NERR_BASE+19)
NERR_TooManyItems equ (NERR_BASE+21)
NERR_InvalidMaxUsers equ (NERR_BASE+22)
NERR_BufTooSmall equ (NERR_BASE+23)
NERR_RemoteErr equ (NERR_BASE+27)
NERR_LanmanIniError equ (NERR_BASE+31)
NERR_NetworkError equ (NERR_BASE+36)
NERR_WkstaInconsistentState equ (NERR_BASE+37)
NERR_WkstaNotStarted equ (NERR_BASE+38)
NERR_BrowserNotStarted equ (NERR_BASE+39)
NERR_InternalError equ (NERR_BASE+40)
NERR_BadTransactConfig equ (NERR_BASE+41)
NERR_InvalidAPI equ (NERR_BASE+42)
NERR_BadEventName equ (NERR_BASE+43)
NERR_DupNameReboot equ (NERR_BASE+44)
NERR_CfgCompNotFound equ (NERR_BASE+46)
NERR_CfgParamNotFound equ (NERR_BASE+47)
NERR_LineTooLong equ (NERR_BASE+49)
NERR_QNotFound equ (NERR_BASE+50)
NERR_JobNotFound equ (NERR_BASE+51)
NERR_DestNotFound equ (NERR_BASE+52)
NERR_DestExists equ (NERR_BASE+53)
NERR_QExists equ (NERR_BASE+54)
NERR_QNoRoom equ (NERR_BASE+55)
NERR_JobNoRoom equ (NERR_BASE+56)
NERR_DestNoRoom equ (NERR_BASE+57)
NERR_DestIdle equ (NERR_BASE+58)
NERR_DestInvalidOp equ (NERR_BASE+59)
NERR_ProcNoRespond equ (NERR_BASE+60)
NERR_SpoolerNotLoaded equ (NERR_BASE+61)
NERR_DestInvalidState equ (NERR_BASE+62)
NERR_QInvalidState equ (NERR_BASE+63)
NERR_JobInvalidState equ (NERR_BASE+64)
NERR_SpoolNoMemory equ (NERR_BASE+65)
NERR_DriverNotFound equ (NERR_BASE+66)
NERR_DataTypeInvalid equ (NERR_BASE+67)
NERR_ProcNotFound equ (NERR_BASE+68)
NERR_ServiceTableLocked equ (NERR_BASE+80)
NERR_ServiceTableFull equ (NERR_BASE+81)
NERR_ServiceInstalled equ (NERR_BASE+82)
NERR_ServiceEntryLocked equ (NERR_BASE+83)
NERR_ServiceNotInstalled equ (NERR_BASE+84)
NERR_BadServiceName equ (NERR_BASE+85)
NERR_ServiceCtlTimeout equ (NERR_BASE+86)
NERR_ServiceCtlBusy equ (NERR_BASE+87)
NERR_BadServiceProgName equ (NERR_BASE+88)
NERR_ServiceNotCtrl equ (NERR_BASE+89)
NERR_ServiceKillProc equ (NERR_BASE+90)
NERR_ServiceCtlNotValid equ (NERR_BASE+91)
NERR_NotInDispatchTbl equ (NERR_BASE+92) 
NERR_BadControlRecv equ (NERR_BASE+93)
NERR_ServiceNotStarting equ (NERR_BASE+94)
NERR_AlreadyLoggedOn equ (NERR_BASE+100)
NERR_NotLoggedOn equ (NERR_BASE+101)
NERR_BadUsername equ (NERR_BASE+102)
NERR_BadPassword equ (NERR_BASE+103)
NERR_UnableToAddName_W equ (NERR_BASE+104)
NERR_UnableToAddName_F equ (NERR_BASE+105)
NERR_UnableToDelName_W equ (NERR_BASE+106)
NERR_UnableToDelName_F equ (NERR_BASE+107)
NERR_LogonsPaused equ (NERR_BASE+109)
NERR_LogonServerConflict equ (NERR_BASE+110)
NERR_LogonNoUserPath equ (NERR_BASE+111)
NERR_LogonScriptError equ (NERR_BASE+112)
NERR_StandaloneLogon equ (NERR_BASE+114)
NERR_LogonServerNotFound equ (NERR_BASE+115)
NERR_LogonDomainExists equ (NERR_BASE+116)
NERR_NonValidatedLogon equ (NERR_BASE+117)
NERR_ACFNotFound equ (NERR_BASE+119)
NERR_GroupNotFound equ (NERR_BASE+120)
NERR_UserNotFound equ (NERR_BASE+121)
NERR_ResourceNotFound equ (NERR_BASE+122)
NERR_GroupExists equ (NERR_BASE+123)
NERR_UserExists equ (NERR_BASE+124)
NERR_ResourceExists equ (NERR_BASE+125)
NERR_NotPrimary equ (NERR_BASE+126)
NERR_ACFNotLoaded equ (NERR_BASE+127)
NERR_ACFNoRoom equ (NERR_BASE+128)
NERR_ACFFileIOFail equ (NERR_BASE+129)
NERR_ACFTooManyLists equ (NERR_BASE+130)
NERR_UserLogon equ (NERR_BASE+131)
NERR_ACFNoParent equ (NERR_BASE+132)
NERR_CanNotGrowSegment equ (NERR_BASE+133)
NERR_SpeGroupOp equ (NERR_BASE+134)
NERR_NotInCache equ (NERR_BASE+135)
NERR_UserInGroup equ (NERR_BASE+136)
NERR_UserNotInGroup equ (NERR_BASE+137) 
NERR_AccountUndefined equ (NERR_BASE+138) 
NERR_AccountExpired equ (NERR_BASE+139) 
NERR_InvalidWorkstation equ (NERR_BASE+140) 
NERR_InvalidLogonHours equ (NERR_BASE+141) 
NERR_PasswordExpired equ (NERR_BASE+142) 
NERR_PasswordCantChange equ (NERR_BASE+143) 
NERR_PasswordHistConflict equ (NERR_BASE+144) 
NERR_PasswordTooShort equ (NERR_BASE+145) 
NERR_PasswordTooRecent equ (NERR_BASE+146) 
NERR_InvalidDatabase equ (NERR_BASE+147) 
NERR_DatabaseUpToDate equ (NERR_BASE+148) 
NERR_SyncRequired equ (NERR_BASE+149) 
NERR_UseNotFound equ (NERR_BASE+150) 
NERR_BadAsgType equ (NERR_BASE+151) 
NERR_DeviceIsShared equ (NERR_BASE+152) 
NERR_NoComputerName equ (NERR_BASE+170) 
NERR_MsgAlreadyStarted equ (NERR_BASE+171) 
NERR_MsgInitFailed equ (NERR_BASE+172) 
NERR_NameNotFound equ (NERR_BASE+173) 
NERR_AlreadyForwarded equ (NERR_BASE+174) 
NERR_AddForwarded equ (NERR_BASE+175) 
NERR_AlreadyExists equ (NERR_BASE+176) 
NERR_TooManyNames equ (NERR_BASE+177) 
NERR_DelComputerName equ (NERR_BASE+178) 
NERR_LocalForward equ (NERR_BASE+179) 
NERR_GrpMsgProcessor equ (NERR_BASE+180) 
NERR_PausedRemote equ (NERR_BASE+181) 
NERR_BadReceive equ (NERR_BASE+182) 
NERR_NameInUse equ (NERR_BASE+183) 
NERR_MsgNotStarted equ (NERR_BASE+184) 
NERR_NotLocalName equ (NERR_BASE+185) 
NERR_NoForwardName equ (NERR_BASE+186) 
NERR_RemoteFull equ (NERR_BASE+187) 
NERR_NameNotForwarded equ (NERR_BASE+188) 
NERR_TruncatedBroadcast equ (NERR_BASE+189) 
NERR_InvalidDevice equ (NERR_BASE+194) 
NERR_WriteFault equ (NERR_BASE+195) 
NERR_DuplicateName equ (NERR_BASE+197) 
NERR_DeleteLater equ (NERR_BASE+198) 
NERR_IncompleteDel equ (NERR_BASE+199) 
NERR_MultipleNets equ (NERR_BASE+200) 
NERR_NetNameNotFound equ (NERR_BASE+210) 
NERR_DeviceNotShared equ (NERR_BASE+211) 
NERR_ClientNameNotFound equ (NERR_BASE+212) 
NERR_FileIdNotFound equ (NERR_BASE+214) 
NERR_ExecFailure equ (NERR_BASE+215) 
NERR_TmpFile equ (NERR_BASE+216) 
NERR_TooMuchData equ (NERR_BASE+217) 
NERR_DeviceShareConflict equ (NERR_BASE+218) 
NERR_BrowserTableIncomplete equ (NERR_BASE+219)  
NERR_NotLocalDomain equ (NERR_BASE+220) 
NERR_IsDfsShare equ (NERR_BASE+221) 
NERR_DevInvalidOpCode equ (NERR_BASE+231) 
NERR_DevNotFound equ (NERR_BASE+232) 
NERR_DevNotOpen equ (NERR_BASE+233) 
NERR_BadQueueDevString equ (NERR_BASE+234) 
NERR_BadQueuePriority equ (NERR_BASE+235) 
NERR_NoCommDevs equ (NERR_BASE+237) 
NERR_QueueNotFound equ (NERR_BASE+238) 
NERR_BadDevString equ (NERR_BASE+240) 
NERR_BadDev equ (NERR_BASE+241) 
NERR_InUseBySpooler equ (NERR_BASE+242) 
NERR_CommDevInUse equ (NERR_BASE+243) 
NERR_InvalidComputer equ (NERR_BASE+251) 
NERR_MaxLenExceeded equ (NERR_BASE+254) 
NERR_BadComponent equ (NERR_BASE+256) 
NERR_CantType equ (NERR_BASE+257) 
NERR_TooManyEntries equ (NERR_BASE+262) 
NERR_ProfileFileTooBig equ (NERR_BASE+270) 
NERR_ProfileOffset equ (NERR_BASE+271) 
NERR_ProfileCleanup equ (NERR_BASE+272) 
NERR_ProfileUnknownCmd equ (NERR_BASE+273) 
NERR_ProfileLoadErr equ (NERR_BASE+274) 
NERR_ProfileSaveErr equ (NERR_BASE+275) 
NERR_LogOverflow equ (NERR_BASE+277)      
NERR_LogFileChanged equ (NERR_BASE+278)      
NERR_LogFileCorrupt equ (NERR_BASE+279)      
NERR_SourceIsDir equ (NERR_BASE+280) 
NERR_BadSource equ (NERR_BASE+281) 
NERR_BadDest equ (NERR_BASE+282) 
NERR_DifferentServers equ (NERR_BASE+283) 
NERR_RunSrvPaused equ (NERR_BASE+285) 
NERR_ErrCommRunSrv equ (NERR_BASE+289) 
NERR_ErrorExecingGhost equ (NERR_BASE+291) 
NERR_ShareNotFound equ (NERR_BASE+292) 
NERR_InvalidLana equ (NERR_BASE+300) 
NERR_OpenFiles equ (NERR_BASE+301) 
NERR_ActiveConns equ (NERR_BASE+302) 
NERR_BadPasswordCore equ (NERR_BASE+303) 
NERR_DevInUse equ (NERR_BASE+304) 
NERR_LocalDrive equ (NERR_BASE+305) 
NERR_AlertExists equ (NERR_BASE+330) 
NERR_TooManyAlerts equ (NERR_BASE+331) 
NERR_NoSuchAlert equ (NERR_BASE+332) 
NERR_BadRecipient equ (NERR_BASE+333) 
NERR_AcctLimitExceeded equ (NERR_BASE+334)
NERR_InvalidLogSeek equ (NERR_BASE+340) 
NERR_BadUasConfig equ (NERR_BASE+350) 
NERR_InvalidUASOp equ (NERR_BASE+351) 
NERR_LastAdmin equ (NERR_BASE+352) 
NERR_DCNotFound equ (NERR_BASE+353) 
NERR_LogonTrackingError equ (NERR_BASE+354) 
NERR_NetlogonNotStarted equ (NERR_BASE+355) 
NERR_CanNotGrowUASFile equ (NERR_BASE+356) 
NERR_TimeDiffAtDC equ (NERR_BASE+357) 
NERR_PasswordMismatch equ (NERR_BASE+358) 
NERR_NoSuchServer equ (NERR_BASE+360) 
NERR_NoSuchSession equ (NERR_BASE+361) 
NERR_NoSuchConnection equ (NERR_BASE+362) 
NERR_TooManyServers equ (NERR_BASE+363) 
NERR_TooManySessions equ (NERR_BASE+364) 
NERR_TooManyConnections equ (NERR_BASE+365) 
NERR_TooManyFiles equ (NERR_BASE+366) 
NERR_NoAlternateServers equ (NERR_BASE+367) 
NERR_TryDownLevel equ (NERR_BASE+370) 
NERR_UPSDriverNotStarted equ (NERR_BASE+380) 
NERR_UPSInvalidConfig equ (NERR_BASE+381) 
NERR_UPSInvalidCommPort equ (NERR_BASE+382) 
NERR_UPSSignalAsserted equ (NERR_BASE+383) 
NERR_UPSShutdownFailed equ (NERR_BASE+384) 
NERR_BadDosRetCode equ (NERR_BASE+400) 
NERR_ProgNeedsExtraMem equ (NERR_BASE+401) 
NERR_BadDosFunction equ (NERR_BASE+402) 
NERR_RemoteBootFailed equ (NERR_BASE+403) 
NERR_BadFileCheckSum equ (NERR_BASE+404) 
NERR_NoRplBootSystem equ (NERR_BASE+405) 
NERR_RplLoadrNetBiosErr equ (NERR_BASE+406) 
NERR_RplLoadrDiskErr equ (NERR_BASE+407) 
NERR_ImageParamErr equ (NERR_BASE+408) 
NERR_TooManyImageParams equ (NERR_BASE+409) 
NERR_NonDosFloppyUsed equ (NERR_BASE+410) 
NERR_RplBootRestart equ (NERR_BASE+411) 
NERR_RplSrvrCallFailed equ (NERR_BASE+412) 
NERR_CantConnectRplSrvr equ (NERR_BASE+413) 
NERR_CantOpenImageFile equ (NERR_BASE+414) 
NERR_CallingRplSrvr equ (NERR_BASE+415) 
NERR_StartingRplBoot equ (NERR_BASE+416) 
NERR_RplBootServiceTerm equ (NERR_BASE+417) 
NERR_RplBootStartFailed equ (NERR_BASE+418) 
NERR_RPL_CONNECTED equ (NERR_BASE+419) 
NERR_BrowserConfiguredToNotRun equ (NERR_BASE+450) 
NERR_RplNoAdaptersStarted equ (NERR_BASE+510) 
NERR_RplBadRegistry equ (NERR_BASE+511) 
NERR_RplBadDatabase equ (NERR_BASE+512) 
NERR_RplRplfilesShare equ (NERR_BASE+513) 
NERR_RplNotRplServer equ (NERR_BASE+514) 
NERR_RplCannotEnum equ (NERR_BASE+515) 
NERR_RplWkstaInfoCorrupted equ (NERR_BASE+516) 
NERR_RplWkstaNotFound equ (NERR_BASE+517) 
NERR_RplWkstaNameUnavailable equ (NERR_BASE+518) 
NERR_RplProfileInfoCorrupted equ (NERR_BASE+519) 
NERR_RplProfileNotFound equ (NERR_BASE+520) 
NERR_RplProfileNameUnavailable equ (NERR_BASE+521) 
NERR_RplProfileNotEmpty equ (NERR_BASE+522) 
NERR_RplConfigInfoCorrupted equ (NERR_BASE+523) 
NERR_RplConfigNotFound equ (NERR_BASE+524) 
NERR_RplAdapterInfoCorrupted equ (NERR_BASE+525) 
NERR_RplInternal equ (NERR_BASE+526) 
NERR_RplVendorInfoCorrupted equ (NERR_BASE+527) 
NERR_RplBootInfoCorrupted equ (NERR_BASE+528) 
NERR_RplWkstaNeedsUserAcct equ (NERR_BASE+529) 
NERR_RplNeedsRPLUSERAcct equ (NERR_BASE+530) 
NERR_RplBootNotFound equ (NERR_BASE+531) 
NERR_RplIncompatibleProfile equ (NERR_BASE+532) 
NERR_RplAdapterNameUnavailable equ (NERR_BASE+533) 
NERR_RplConfigNotEmpty equ (NERR_BASE+534) 
NERR_RplBootInUse equ (NERR_BASE+535) 
NERR_RplBackupDatabase equ (NERR_BASE+536) 
NERR_RplAdapterNotFound equ (NERR_BASE+537) 
NERR_RplVendorNotFound equ (NERR_BASE+538) 
NERR_RplVendorNameUnavailable equ (NERR_BASE+539) 
NERR_RplBootNameUnavailable equ (NERR_BASE+540) 
NERR_RplConfigNameUnavailable equ (NERR_BASE+541) 
NERR_DfsInternalCorruption equ (NERR_BASE+560) 
NERR_DfsVolumeDataCorrupt equ (NERR_BASE+561) 
NERR_DfsNoSuchVolume equ (NERR_BASE+562) 
NERR_DfsVolumeAlreadyExists equ (NERR_BASE+563) 
NERR_DfsAlreadyShared equ (NERR_BASE+564) 
NERR_DfsNoSuchShare equ (NERR_BASE+565) 
NERR_DfsNotALeafVolume equ (NERR_BASE+566) 
NERR_DfsLeafVolume equ (NERR_BASE+567) 
NERR_DfsVolumeHasMultipleServers equ (NERR_BASE+568) 
NERR_DfsCantCreateJunctionPoint equ (NERR_BASE+569) 
NERR_DfsServerNotDfsAware equ (NERR_BASE+570) 
NERR_DfsBadRenamePath equ (NERR_BASE+571) 
NERR_DfsVolumeIsOffline equ (NERR_BASE+572) 
NERR_DfsNoSuchServer equ (NERR_BASE+573) 
NERR_DfsCyclicalName equ (NERR_BASE+574) 
NERR_DfsNotSupportedInServerDfs equ (NERR_BASE+575) 
NERR_DfsInternalError equ (NERR_BASE+590) 
NERR_SetupAlreadyJoined equ (NERR_BASE+591) 
NERR_SetupNotJoined equ (NERR_BASE+592) 
NERR_SetupDomainController equ (NERR_BASE+593) 
MAX_NERR equ (NERR_BASE+899) 

;===================================[ LMWKSTA.H ]==============================
WKSTA_PLATFORM_ID_PARMNUM equ 100
WKSTA_COMPUTERNAME_PARMNUM equ 1
WKSTA_LANGROUP_PARMNUM equ 2
WKSTA_VER_MAJOR_PARMNUM equ 4
WKSTA_VER_MINOR_PARMNUM equ 5
WKSTA_LOGGED_ON_USERS_PARMNUM equ 6
WKSTA_LANROOT_PARMNUM equ 7
WKSTA_LOGON_DOMAIN_PARMNUM equ 8
WKSTA_LOGON_SERVER_PARMNUM equ 9
WKSTA_CHARWAIT_PARMNUM equ 10  
WKSTA_CHARTIME_PARMNUM equ 11  
WKSTA_CHARCOUNT_PARMNUM equ 12  
WKSTA_KEEPCONN_PARMNUM equ 13
WKSTA_KEEPSEARCH_PARMNUM equ 14
WKSTA_MAXCMDS_PARMNUM equ 15
WKSTA_NUMWORKBUF_PARMNUM equ 16
WKSTA_MAXWRKCACHE_PARMNUM equ 17
WKSTA_SESSTIMEOUT_PARMNUM equ 18
WKSTA_SIZERROR_PARMNUM equ 19
WKSTA_NUMALERTS_PARMNUM equ 20
WKSTA_NUMSERVICES_PARMNUM equ 21
WKSTA_NUMCHARBUF_PARMNUM equ 22
WKSTA_SIZCHARBUF_PARMNUM equ 23
WKSTA_ERRLOGSZ_PARMNUM equ 27  
WKSTA_PRINTBUFTIME_PARMNUM equ 28  
WKSTA_SIZWORKBUF_PARMNUM equ 29
WKSTA_MAILSLOTS_PARMNUM equ 30
WKSTA_NUMDGRAMBUF_PARMNUM equ 31
WKSTA_WRKHEURISTICS_PARMNUM equ 32  
WKSTA_MAXTHREADS_PARMNUM equ 33
WKSTA_LOCKQUOTA_PARMNUM equ 41
WKSTA_LOCKINCREMENT_PARMNUM equ 42
WKSTA_LOCKMAXIMUM_PARMNUM equ 43
WKSTA_PIPEINCREMENT_PARMNUM equ 44
WKSTA_PIPEMAXIMUM_PARMNUM equ 45
WKSTA_DORMANTFILELIMIT_PARMNUM equ 46
WKSTA_CACHEFILETIMEOUT_PARMNUM equ 47
WKSTA_USEOPPORTUNISTICLOCKING_PARMNUM equ 48
WKSTA_USEUNLOCKBEHIND_PARMNUM equ 49
WKSTA_USECLOSEBEHIND_PARMNUM equ 50
WKSTA_BUFFERNAMEDPIPES_PARMNUM equ 51
WKSTA_USELOCKANDREADANDUNLOCK_PARMNUM equ 52
WKSTA_UTILIZENTCACHING_PARMNUM equ 53
WKSTA_USERAWREAD_PARMNUM equ 54
WKSTA_USERAWWRITE_PARMNUM equ 55
WKSTA_USEWRITERAWWITHDATA_PARMNUM equ 56
WKSTA_USEENCRYPTION_PARMNUM equ 57
WKSTA_BUFFILESWITHDENYWRITE_PARMNUM equ 58
WKSTA_BUFFERREADONLYFILES_PARMNUM equ 59
WKSTA_FORCECORECREATEMODE_PARMNUM equ 60
WKSTA_USE512BYTESMAXTRANSFER_PARMNUM equ 61
WKSTA_READAHEADTHRUPUT_PARMNUM equ 62
WKSTA_OTH_DOMAINS_PARMNUM equ 101
TRANSPORT_QUALITYOFSERVICE_PARMNUM equ 201
TRANSPORT_NAME_PARMNUM equ 202

WKSTA_INFO_100 struct
    wki100_platform_id dd ?
    wki100_computername dd ?
    wki100_langroup dd ?
    wki100_ver_major dd ?
    wki100_ver_minor dd ?
WKSTA_INFO_100 ends

WKSTA_INFO_101 struct
    wki101_platform_id dd ?
    wki101_computername dd ?
    wki101_langroup dd ?
    wki101_ver_major dd ?
    wki101_ver_minor dd ?
    wki101_lanroot dd ?
WKSTA_INFO_101 ends

WKSTA_INFO_102  struct
    wki102_platform_id dd ?
    wki102_computername dd ?
    wki102_langroup dd ?
    wki102_ver_major dd ?
    wki102_ver_minor dd ?
    wki102_lanroot dd ?
    wki102_logged_on_users dd ?
WKSTA_INFO_102 ends

WKSTA_INFO_302 struct
    wki302_char_wait dd ?
    wki302_collection_time dd ?
    wki302_maximum_collection_count dd ?
    wki302_keep_conn dd ?
    wki302_keep_search dd ?
    wki302_max_cmds dd ?
    wki302_num_work_buf dd ?
    wki302_siz_work_buf dd ?
    wki302_max_wrk_cache dd ?
    wki302_sess_timeout dd ?
    wki302_siz_error dd ?
    wki302_num_alerts dd ?
    wki302_num_services dd ?
    wki302_errlog_sz dd ?
    wki302_print_buf_time dd ?
    wki302_num_char_buf dd ?
    wki302_siz_char_buf dd ?
    wki302_wrk_heuristics dd ?
    wki302_mailslots dd ?
    wki302_num_dgram_buf dd ?
WKSTA_INFO_302 ends

WKSTA_INFO_402 struct
    wki402_char_wait dd ?
    wki402_collection_time dd ?
    wki402_maximum_collection_count dd ?
    wki402_keep_conn dd ?
    wki402_keep_search dd ?
    wki402_max_cmds dd ?
    wki402_num_work_buf dd ?
    wki402_siz_work_buf dd ?
    wki402_max_wrk_cache dd ?
    wki402_sess_timeout dd ?
    wki402_siz_error dd ?
    wki402_num_alerts dd ?
    wki402_num_services dd ?
    wki402_errlog_sz dd ?
    wki402_print_buf_time dd ?
    wki402_num_char_buf dd ?
    wki402_siz_char_buf dd ?
    wki402_wrk_heuristics dd ?
    wki402_mailslots dd ?
    wki402_num_dgram_buf dd ?
    wki402_max_threads dd ?
WKSTA_INFO_402 ends

WKSTA_INFO_502 struct
    wki502_char_wait dd ?
    wki502_collection_time dd ?
    wki502_maximum_collection_count dd ?
    wki502_keep_conn dd ?
    wki502_max_cmds dd ?
    wki502_sess_timeout dd ?
    wki502_siz_char_buf dd ?
    wki502_max_threads dd ?
    wki502_lock_quota dd ?
    wki502_lock_increment dd ?
    wki502_lock_maximum dd ?
    wki502_pipe_increment dd ?
    wki502_pipe_maximum dd ?
    wki502_cache_file_timeout dd ?
    wki502_dormant_file_limit dd ?
    wki502_read_ahead_throughput dd ?
    wki502_num_mailslot_buffers dd ?
    wki502_num_srv_announce_buffers dd ?
    wki502_max_illegal_datagram_events dd ?
    wki502_illegal_datagram_event_reset_frequency dd ?
    wki502_log_election_packets dd ?
    wki502_use_opportunistic_locking dd ?
    wki502_use_unlock_behind dd ?
    wki502_use_close_behind dd ?
    wki502_buf_named_pipes dd ?
    wki502_use_lock_read_unlock dd ?
    wki502_utilize_nt_caching dd ?
    wki502_use_raw_read dd ?
    wki502_use_raw_write dd ?
    wki502_use_write_raw_data dd ?
    wki502_use_encryption dd ?
    wki502_buf_files_deny_write dd ?
    wki502_buf_read_only_files dd ?
    wki502_force_core_create_mode dd ?
    wki502_use_512_byte_max_transfer dd ?
WKSTA_INFO_502 ends

WKSTA_INFO_1010 struct
     wki1010_char_wait dd ?
WKSTA_INFO_1010 ends

WKSTA_INFO_1011 struct
     wki1011_collection_time dd ?
WKSTA_INFO_1011 ends

WKSTA_INFO_1012 struct
     wki1012_maximum_collection_count dd ?
WKSTA_INFO_1012 ends

WKSTA_INFO_1027 struct
     wki1027_errlog_sz dd ?
WKSTA_INFO_1027 ends

WKSTA_INFO_1028 struct
     wki1028_print_buf_time dd ?
WKSTA_INFO_1028 ends

WKSTA_INFO_1032 struct
     wki1032_wrk_heuristics dd ?
WKSTA_INFO_1032 ends

WKSTA_INFO_1013 struct
     wki1013_keep_conn dd ?
WKSTA_INFO_1013 ends

WKSTA_INFO_1018 struct
     wki1018_sess_timeout dd ?
WKSTA_INFO_1018 ends

WKSTA_INFO_1023 struct
     wki1023_siz_char_buf dd ?
WKSTA_INFO_1023 ends

WKSTA_INFO_1033 struct
     wki1033_max_threads dd ?
WKSTA_INFO_1033 ends

WKSTA_INFO_1041 struct
    wki1041_lock_quota dd ?
WKSTA_INFO_1041 ends

WKSTA_INFO_1042 struct
    wki1042_lock_increment dd ?
WKSTA_INFO_1042 ends

WKSTA_INFO_1043 struct
    wki1043_lock_maximum dd ?
WKSTA_INFO_1043 ends

WKSTA_INFO_1044 struct
    wki1044_pipe_increment dd ?
WKSTA_INFO_1044 ends

WKSTA_INFO_1045 struct
    wki1045_pipe_maximum dd ?
WKSTA_INFO_1045 ends

WKSTA_INFO_1046 struct
    wki1046_dormant_file_limit dd ?
WKSTA_INFO_1046 ends

WKSTA_INFO_1047 struct
    wki1047_cache_file_timeout dd ?
WKSTA_INFO_1047 ends

WKSTA_INFO_1048 struct
    wki1048_use_opportunistic_locking dd ?
WKSTA_INFO_1048 ends

WKSTA_INFO_1049 struct
    wki1049_use_unlock_behind dd ?
WKSTA_INFO_1049 ends

WKSTA_INFO_1050 struct
    wki1050_use_close_behind dd ?
WKSTA_INFO_1050 ends

WKSTA_INFO_1051 struct
    wki1051_buf_named_pipes dd ?
WKSTA_INFO_1051 ends

WKSTA_INFO_1052 struct
    wki1052_use_lock_read_unlock dd ?
WKSTA_INFO_1052 ends

WKSTA_INFO_1053 struct
    wki1053_utilize_nt_caching dd ?
WKSTA_INFO_1053 ends

WKSTA_INFO_1054 struct
    wki1054_use_raw_read dd ?
WKSTA_INFO_1054 ends

WKSTA_INFO_1055 struct
    wki1055_use_raw_write dd ?
WKSTA_INFO_1055 ends

WKSTA_INFO_1056 struct
    wki1056_use_write_raw_data dd ?
WKSTA_INFO_1056 ends

WKSTA_INFO_1057 struct
    wki1057_use_encryption dd ?
WKSTA_INFO_1057 ends

WKSTA_INFO_1058 struct
    wki1058_buf_files_deny_write dd ?
WKSTA_INFO_1058 ends

WKSTA_INFO_1059 struct
    wki1059_buf_read_only_files dd ?
WKSTA_INFO_1059 ends

WKSTA_INFO_1060 struct
    wki1060_force_core_create_mode dd ?
WKSTA_INFO_1060 ends

WKSTA_INFO_1061 struct
    wki1061_use_512_byte_max_transfer dd ?
WKSTA_INFO_1061 ends

WKSTA_INFO_1062 struct
    wki1062_read_ahead_throughput dd ?
WKSTA_INFO_1062 ends

WKSTA_USER_INFO_0 struct
    wkui0_username dd ?
WKSTA_USER_INFO_0 ends

WKSTA_USER_INFO_1 struct
    wkui1_username dd ?
    wkui1_logon_domain dd ?
    wkui1_oth_domains dd ?
    wkui1_logon_server dd ?
WKSTA_USER_INFO_1 ends

WKSTA_USER_INFO_1101 struct
     wkui1101_oth_domains dd ?
WKSTA_USER_INFO_1101 ends

WKSTA_TRANSPORT_INFO_0 struct
    wkti0_quality_of_service dd ?
    wkti0_number_of_vcs dd ?
    wkti0_transport_name dd ?
    wkti0_transport_address dd ?
    wkti0_wan_ish dd ?
WKSTA_TRANSPORT_INFO_0 ends

;=================================[ LMACCESS.H ]=================================
ACCESS_ACCESS_LIST_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + ACCESS_ACCESS_LIST_PARMNUM)
ACCESS_ACCESS_LIST_PARMNUM equ 4
ACCESS_ALL equ (ACCESS_READ+ACCESS_WRITE+ACCESS_CREATE+ACCESS_EXEC+ACCESS_DELETE+ACCESS_ATRIB+ACCESS_PERM)
ACCESS_ATRIB equ 20h
ACCESS_ATTR_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + ACCESS_ATTR_PARMNUM)
ACCESS_ATTR_PARMNUM equ 2
ACCESS_AUDIT equ 1
ACCESS_COUNT_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + ACCESS_COUNT_PARMNUM)
ACCESS_COUNT_PARMNUM equ 3
ACCESS_CREATE equ 4
ACCESS_DELETE equ 10h
ACCESS_EXEC equ 8
ACCESS_FAIL_ACL equ 800h
ACCESS_FAIL_DELETE equ 400h
ACCESS_FAIL_MASK equ 0F00h
ACCESS_FAIL_OPEN equ 100h
ACCESS_FAIL_SHIFT equ 4
ACCESS_FAIL_WRITE equ 200h
ACCESS_GROUP equ 8000h
ACCESS_NONE equ 0
ACCESS_PERM equ 40h
ACCESS_READ equ 1
ACCESS_RESOURCE_NAME_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + ACCESS_RESOURCE_NAME_PARMNUM)
ACCESS_RESOURCE_NAME_PARMNUM equ 1
ACCESS_SUCCESS_ACL equ 80h
ACCESS_SUCCESS_DELETE equ 40h
ACCESS_SUCCESS_MASK equ 0F0h
ACCESS_SUCCESS_OPEN equ 10h
ACCESS_SUCCESS_WRITE equ 20h
ACCESS_WRITE equ 2
AF_OP_ACCOUNTS equ 8
AF_OP_COMM equ 2
AF_OP_PRINT equ 1
AF_OP_SERVER equ 4
AF_SETTABLE_BITS equ (AF_OP_PRINT+AF_OP_COMM+AF_OP_SERVER+AF_OP_ACCOUNTS)
DEF_FORCE_LOGOFF equ 0ffffffffh  
DEF_MAX_BADPW equ 0
DEF_MAX_PWAGE equ TIMEQ_FOREVER               
DEF_MAX_PWHIST equ 8
DEF_MIN_PWAGE equ 0         
DEF_MIN_PWLEN equ 6
DEF_PWUNIQUENESS equ 5
FILTER_INTERDOMAIN_TRUST_ACCOUNT equ (8)
FILTER_NORMAL_ACCOUNT equ (2)
FILTER_SERVER_TRUST_ACCOUNT equ (20h)
FILTER_TEMP_DUPLICATE_ACCOUNT equ (1)
FILTER_WORKSTATION_TRUST_ACCOUNT equ (10h)
GROUPIDMASK equ 8000h      
GROUP_ALL_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + GROUP_ALL_PARMNUM)
GROUP_ALL_PARMNUM equ 0
GROUP_ATTRIBUTES_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + GROUP_ATTRIBUTES_PARMNUM)
GROUP_ATTRIBUTES_PARMNUM equ 3
GROUP_COMMENT_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + GROUP_COMMENT_PARMNUM)
GROUP_COMMENT_PARMNUM equ 2
GROUP_NAME_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + GROUP_NAME_PARMNUM)
GROUP_NAME_PARMNUM equ 1
GROUP_POSIX_ID_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + GROUP_POSIX_ID_PARMNUM)
GROUP_SPECIALGRP_ADMINS equ L"ADMINS"
GROUP_SPECIALGRP_GUESTS equ L"GUESTS"
GROUP_SPECIALGRP_LOCAL equ L"LOCAL"
GROUP_SPECIALGRP_USERS equ L"USERS"
LG_INCLUDE_INDIRECT equ (1)
LOCALGROUP_COMMENT_PARMNUM equ 2
LOCALGROUP_NAME_PARMNUM equ 1
MAXPERMENTRIES equ 64
MAX_PASSWD_LEN equ PWLEN
MODALS_DOMAIN_ID_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + MODALS_DOMAIN_ID_PARMNUM)
MODALS_DOMAIN_ID_PARMNUM equ 9
MODALS_DOMAIN_NAME_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + MODALS_DOMAIN_NAME_PARMNUM)
MODALS_DOMAIN_NAME_PARMNUM equ 8
MODALS_FORCE_LOGOFF_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + MODALS_FORCE_LOGOFF_PARMNUM)
MODALS_FORCE_LOGOFF_PARMNUM equ 4
MODALS_LOCKOUT_DURATION_PARMNUM equ 10
MODALS_LOCKOUT_OBSERVATION_WINDOW_PARMNUM equ 11
MODALS_LOCKOUT_THRESHOLD_PARMNUM equ 12
MODALS_MAX_PASSWD_AGE_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + MODALS_MAX_PASSWD_AGE_PARMNUM)
MODALS_MAX_PASSWD_AGE_PARMNUM equ 2
MODALS_MIN_PASSWD_AGE_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + MODALS_MIN_PASSWD_AGE_PARMNUM)
MODALS_MIN_PASSWD_AGE_PARMNUM equ 3
MODALS_MIN_PASSWD_LEN_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + MODALS_MIN_PASSWD_LEN_PARMNUM)
MODALS_MIN_PASSWD_LEN_PARMNUM equ 1
MODALS_PASSWD_HIST_LEN_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + MODALS_PASSWD_HIST_LEN_PARMNUM)
MODALS_PASSWD_HIST_LEN_PARMNUM equ 5
MODALS_PRIMARY_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + MODALS_PRIMARY_PARMNUM)
MODALS_PRIMARY_PARMNUM equ 7
MODALS_ROLE_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + MODALS_ROLE_PARMNUM)
MODALS_ROLE_PARMNUM equ 6
NETLOGON_CONTROL_BACKUP_CHANGE_LOG equ 0FFFCh
NETLOGON_CONTROL_BREAKPOINT equ 0FFFFh
NETLOGON_CONTROL_CHANGE_PASSWORD equ 9
NETLOGON_CONTROL_FIND_USER equ 8    
NETLOGON_CONTROL_FORCE_DNS_REG   equ 11
NETLOGON_CONTROL_PDC_REPLICATE equ 4    
NETLOGON_CONTROL_QUERY equ 1    
NETLOGON_CONTROL_QUERY_DNS_REG   equ 12
NETLOGON_CONTROL_REDISCOVER equ 5    
NETLOGON_CONTROL_REPLICATE equ 2    
NETLOGON_CONTROL_SET_DBFLAG equ 0FFFEh
NETLOGON_CONTROL_SYNCHRONIZE equ 3    
NETLOGON_CONTROL_TC_QUERY equ 6    
NETLOGON_CONTROL_TC_VERIFY       equ 10
NETLOGON_CONTROL_TRANSPORT_NOTIFY equ 7 
NETLOGON_CONTROL_TRUNCATE_LOG equ 0FFFDh
NETLOGON_CONTROL_UNLOAD_NETLOGON_DLL equ 0FFFBh
NETLOGON_DNS_UPDATE_FAILURE      equ 40h
NETLOGON_FULL_SYNC_REPLICATION equ 4  
NETLOGON_HAS_IP                  equ 10h
NETLOGON_HAS_TIMESERV            equ 20h
NETLOGON_REDO_NEEDED equ 8 
NETLOGON_REPLICATION_IN_PROGRESS equ 2  
NETLOGON_REPLICATION_NEEDED equ 1  
NETLOGON_VERIFY_STATUS_RETURNED  equ 80h
NET_VALIDATE_BAD_PASSWORD_COUNT  equ 00000008h
NET_VALIDATE_BAD_PASSWORD_TIME   equ 00000002h
NET_VALIDATE_LOCKOUT_TIME        equ 00000004h
NET_VALIDATE_PASSWORD_HISTORY    equ 00000020h
NET_VALIDATE_PASSWORD_HISTORY_LENGTH equ 00000010h
NET_VALIDATE_PASSWORD_LAST_SET   equ 00000001h
NON_VALIDATED_LOGON equ 3
NULL_USERSETINFO_PASSWD equ "              "
ONE_DAY equ 01*24*3600  
PASSWORD_EXPIRED equ 2
TIMEQ_FOREVER equ -1
UAS_ROLE_BACKUP equ 2
UAS_ROLE_MEMBER equ 1
UAS_ROLE_PRIMARY equ 3
UAS_ROLE_STANDALONE equ 0
UF_ACCOUNTDISABLE equ 2
UF_ACCOUNT_TYPE_MASK equ (UF_TEMP_DUPLICATE_ACCOUNT+UF_NORMAL_ACCOUNT+UF_INTERDOMAIN_TRUST_ACCOUNT+UF_WORKSTATION_TRUST_ACCOUNT+UF_SERVER_TRUST_ACCOUNT)
UF_DONT_EXPIRE_PASSWD equ 10000h
UF_DONT_REQUIRE_PREAUTH          equ 400000h
UF_ENCRYPTED_TEXT_PASSWORD_ALLOWED equ 0080h
UF_HOMEDIR_REQUIRED equ 8
UF_INTERDOMAIN_TRUST_ACCOUNT equ 800h
UF_LOCKOUT equ 10h
UF_MACHINE_ACCOUNT_MASK equ ( UF_INTERDOMAIN_TRUST_ACCOUNT+UF_WORKSTATION_TRUST_ACCOUNT+UF_SERVER_TRUST_ACCOUNT )
UF_MNS_LOGON_ACCOUNT equ 20000h
UF_NORMAL_ACCOUNT equ 200h
UF_NOT_DELEGATED                 equ 100000h
UF_NO_AUTH_DATA_REQUIRED         equ 2000000h
UF_PARTIAL_SECRETS_ACCOUNT       equ 4000000h
UF_PASSWD_CANT_CHANGE equ 40h
UF_PASSWD_NOTREQD equ 20h
UF_PASSWORD_EXPIRED              equ 800000h
UF_SCRIPT equ 1
UF_SERVER_TRUST_ACCOUNT equ 2000h
UF_SETTABLE_BITS equ (UF_SCRIPT+UF_ACCOUNTDISABLE+UF_LOCKOUT+UF_HOMEDIR_REQUIRED+UF_PASSWD_NOTREQD+UF_PASSWD_CANT_CHANGE+UF_ACCOUNT_TYPE_MASK+UF_DONT_EXPIRE_PASSWD+UF_MNS_LOGON_ACCOUNT)
UF_SMARTCARD_REQUIRED            equ 40000h
UF_TEMP_DUPLICATE_ACCOUNT equ 100h
UF_TRUSTED_FOR_DELEGATION        equ 80000h
UF_TRUSTED_TO_AUTHENTICATE_FOR_DELEGATION equ 1000000h
UF_USE_AES_KEYS                  equ 8000000h
UF_USE_DES_KEY_ONLY              equ 200000h
UF_WORKSTATION_TRUST_ACCOUNT equ 1000h
UNITS_PER_DAY equ 24
UNITS_PER_WEEK equ UNITS_PER_DAY * 7
USER_ACCT_EXPIRES_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + USER_ACCT_EXPIRES_PARMNUM)
USER_ACCT_EXPIRES_PARMNUM equ 17
USER_AUTH_FLAGS_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + USER_AUTH_FLAGS_PARMNUM)
USER_AUTH_FLAGS_PARMNUM equ 10
USER_CODE_PAGE_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + USER_CODE_PAGE_PARMNUM)
USER_CODE_PAGE_PARMNUM equ 25
USER_COMMENT_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + USER_COMMENT_PARMNUM)
USER_COMMENT_PARMNUM equ 7
USER_COUNTRY_CODE_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + USER_COUNTRY_CODE_PARMNUM)
USER_COUNTRY_CODE_PARMNUM equ 24
USER_FLAGS_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + USER_FLAGS_PARMNUM)
USER_FLAGS_PARMNUM equ 8
USER_FULL_NAME_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + USER_FULL_NAME_PARMNUM)
USER_FULL_NAME_PARMNUM equ 11
USER_HOME_DIR_DRIVE_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + USER_HOME_DIR_DRIVE_PARMNUM)
USER_HOME_DIR_DRIVE_PARMNUM equ 53
USER_HOME_DIR_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + USER_HOME_DIR_PARMNUM)
USER_HOME_DIR_PARMNUM equ 6
USER_LAST_LOGOFF_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + USER_LAST_LOGOFF_PARMNUM)
USER_LAST_LOGOFF_PARMNUM equ 16
USER_LAST_LOGON_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + USER_LAST_LOGON_PARMNUM)
USER_LAST_LOGON_PARMNUM equ 15
USER_LOGON_HOURS_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + USER_LOGON_HOURS_PARMNUM)
USER_LOGON_HOURS_PARMNUM equ 20
USER_LOGON_SERVER_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + USER_LOGON_SERVER_PARMNUM)
USER_LOGON_SERVER_PARMNUM equ 23
USER_MAXSTORAGE_UNLIMITED equ -1
USER_MAX_STORAGE_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + USER_MAX_STORAGE_PARMNUM)
USER_MAX_STORAGE_PARMNUM equ 18
USER_NAME_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + USER_NAME_PARMNUM)
USER_NAME_PARMNUM equ 1
USER_NO_LOGOFF equ -1
USER_NUM_LOGONS_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + USER_NUM_LOGONS_PARMNUM)
USER_NUM_LOGONS_PARMNUM equ 22
USER_PAD_PW_COUNT_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + USER_PAD_PW_COUNT_PARMNUM)
USER_PAD_PW_COUNT_PARMNUM equ 21
USER_PARMS_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + USER_PARMS_PARMNUM)
USER_PARMS_PARMNUM equ 13
USER_PASSWORD_AGE_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + USER_PASSWORD_AGE_PARMNUM)
USER_PASSWORD_AGE_PARMNUM equ 4
USER_PASSWORD_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + USER_PASSWORD_PARMNUM)
USER_PASSWORD_PARMNUM equ 3
USER_POSIX_ID_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + USER_POSIX_ID_PARMNUM)
USER_PRIMARY_GROUP_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + USER_PRIMARY_GROUP_PARMNUM)
USER_PRIMARY_GROUP_PARMNUM equ 51
USER_PRIV_ADMIN equ 2
USER_PRIV_GUEST equ 0
USER_PRIV_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + USER_PRIV_PARMNUM)
USER_PRIV_MASK equ 3
USER_PRIV_PARMNUM equ 5
USER_PRIV_USER equ 1
USER_PROFILE equ 52 
USER_PROFILE_PARMNUM equ 52
USER_SCRIPT_PATH_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + USER_SCRIPT_PATH_PARMNUM)
USER_SCRIPT_PATH_PARMNUM equ 9
USER_UNITS_PER_WEEK_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + USER_UNITS_PER_WEEK_PARMNUM)
USER_UNITS_PER_WEEK_PARMNUM equ 19
USER_USR_COMMENT_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + USER_USR_COMMENT_PARMNUM)
USER_USR_COMMENT_PARMNUM equ 12
USER_WORKSTATIONS_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + USER_WORKSTATIONS_PARMNUM)
USER_WORKSTATIONS_PARMNUM equ 14
VALIDATED_LOGON equ 0
VALID_LOGOFF equ 1

USER_INFO_0 struct
    usri0_name dd ?
USER_INFO_0 ends

USER_INFO_1 struct
    usri1_name dd ?
    usri1_password dd ?
    usri1_password_age dd ?
    usri1_priv dd ?
    usri1_home_dir dd ?
    usri1_comment dd ?
    usri1_flags dd ?
    usri1_script_path dd ?
USER_INFO_1 ends

USER_INFO_2 struct
    usri2_name dd ?
    usri2_password dd ?
    usri2_password_age dd ?
    usri2_priv dd ?
    usri2_home_dir dd ?
    usri2_comment dd ?
    usri2_flags dd ?
    usri2_script_path dd ?
    usri2_auth_flags dd ?
    usri2_full_name dd ?
    usri2_usr_comment dd ?
    usri2_parms dd ?
    usri2_workstations dd ?
    usri2_last_logon dd ?
    usri2_last_logoff dd ?
    usri2_acct_expires dd ?
    usri2_max_storage dd ?
    usri2_units_per_week dd ?
    usri2_logon_hours dd ?
    usri2_bad_pw_count dd ?
    usri2_num_logons dd ?
    usri2_logon_server dd ?
    usri2_country_code dd ?
    usri2_code_page dd ?
USER_INFO_2 ends

USER_INFO_3 struct
    usri3_name dd ?
    usri3_password dd ?
    usri3_password_age dd ?
    usri3_priv dd ?
    usri3_home_dir dd ?
    usri3_comment dd ?
    usri3_flags dd ?
    usri3_script_path dd ?
    usri3_auth_flags dd ?
    usri3_full_name dd ?
    usri3_usr_comment dd ?
    usri3_parms dd ?
    usri3_workstations dd ?
    usri3_last_logon dd ?
    usri3_last_logoff dd ?
    usri3_acct_expires dd ?
    usri3_max_storage dd ?
    usri3_units_per_week dd ?
    usri3_logon_hours dd ?
    usri3_bad_pw_count dd ?
    usri3_num_logons dd ?
    usri3_logon_server dd ?
    usri3_country_code dd ?
    usri3_code_page dd ?
    usri3_user_id dd ?
    usri3_primary_group_id dd ?
    usri3_profile dd ?
    usri3_home_dir_drive dd ?
    usri3_password_expired dd ?
USER_INFO_3 ends

USER_INFO_10 struct
    usri10_name dd ?
    usri10_comment dd ?
    usri10_usr_comment dd ?
    usri10_full_name dd ?
USER_INFO_10 ends

USER_INFO_11 struct
    usri11_name dd ?
    usri11_comment dd ?
    usri11_usr_comment dd ?
    usri11_full_name dd ?
    usri11_priv dd ?
    usri11_auth_flags dd ?
    usri11_password_age dd ?
    usri11_home_dir dd ?
    usri11_parms dd ?
    usri11_last_logon dd ?
    usri11_last_logoff dd ?
    usri11_bad_pw_count dd ?
    usri11_num_logons dd ?
    usri11_logon_server dd ?
    usri11_country_code dd ?
    usri11_workstations dd ?
    usri11_max_storage dd ?
    usri11_units_per_week dd ?
    usri11_logon_hours dd ?
    usri11_code_page dd ?
USER_INFO_11 ends

USER_INFO_20 struct
    usri20_name dd ?
    usri20_full_name dd ?
    usri20_comment dd ?
    usri20_flags dd ?
    usri20_user_id dd ?
USER_INFO_20 ends

USER_INFO_21 struct
    usri21_password db ENCRYPTED_PWLEN dup(?)
USER_INFO_21 ends

USER_INFO_22 struct
    usri22_name dd ?
    usri22_password db ENCRYPTED_PWLEN dup(?)
    usri22_password_age dd ?
    usri22_priv dd ?
    usri22_home_dir dd ?
    usri22_comment dd ?
    usri22_flags dd ?
    usri22_script_path dd ?
    usri22_auth_flags dd ?
    usri22_full_name dd ?
    usri22_usr_comment dd ?
    usri22_parms dd ?
    usri22_workstations dd ?
    usri22_last_logon dd ?
    usri22_last_logoff dd ?
    usri22_acct_expires dd ?
    usri22_max_storage dd ?
    usri22_units_per_week dd ?
    usri22_logon_hours dd ?
    usri22_bad_pw_count dd ?
    usri22_num_logons dd ?
    usri22_logon_server dd ?
    usri22_country_code dd ?
    usri22_code_page dd ?
USER_INFO_22 ends

USER_INFO_1003 struct
     usri1003_password dd ?
USER_INFO_1003 ends

USER_INFO_1005 struct
     usri1005_priv dd ?
USER_INFO_1005 ends

USER_INFO_1006 struct
     usri1006_home_dir dd ?
USER_INFO_1006 ends

USER_INFO_1007 struct
     usri1007_comment dd ?
USER_INFO_1007 ends

USER_INFO_1008 struct
     usri1008_flags dd ?
USER_INFO_1008 ends

USER_INFO_1009 struct
     usri1009_script_path dd ?
USER_INFO_1009 ends

USER_INFO_1010 struct
     usri1010_auth_flags dd ?
USER_INFO_1010 ends

USER_INFO_1011 struct
     usri1011_full_name dd ?
USER_INFO_1011 ends

USER_INFO_1012 struct
     usri1012_usr_comment dd ?
USER_INFO_1012 ends

USER_INFO_1013 struct
     usri1013_parms dd ?
USER_INFO_1013 ends

USER_INFO_1014 struct
     usri1014_workstations dd ?
USER_INFO_1014 ends

USER_INFO_1017 struct
     usri1017_acct_expires dd ?
USER_INFO_1017 ends

USER_INFO_1018 struct
     usri1018_max_storage dd ?
USER_INFO_1018 ends

USER_INFO_1020 struct
    usri1020_units_per_week dd ?
    usri1020_logon_hours dd ?
USER_INFO_1020 ends

USER_INFO_1023 struct
     usri1023_logon_server dd ?
USER_INFO_1023 ends

USER_INFO_1024 struct
     usri1024_country_code dd ?
USER_INFO_1024 ends

USER_INFO_1025 struct
     usri1025_code_page dd ?
USER_INFO_1025 ends

USER_INFO_1051 struct
     usri1051_primary_group_id dd ?
USER_INFO_1051 ends

USER_INFO_1052 struct
     usri1052_profile dd ?
USER_INFO_1052 ends

USER_INFO_1053 struct
     usri1053_home_dir_drive dd ?
USER_INFO_1053 ends

USER_MODALS_INFO_0 struct
    usrmod0_min_passwd_len dd ?
    usrmod0_max_passwd_age dd ?
    usrmod0_min_passwd_age dd ?
    usrmod0_force_logoff dd ?
    usrmod0_password_hist_len dd ?
USER_MODALS_INFO_0 ends

USER_MODALS_INFO_1 struct
    usrmod1_role dd ?
    usrmod1_primary dd ?
USER_MODALS_INFO_1 ends

USER_MODALS_INFO_2 struct
    usrmod2_domain_name dd ?
    usrmod2_domain_id dd ?
USER_MODALS_INFO_2 ends

USER_MODALS_INFO_3 struct
    usrmod3_lockout_duration dd ?
    usrmod3_lockout_observation_window dd ?
    usrmod3_lockout_threshold dd ?
USER_MODALS_INFO_3 ends

USER_MODALS_INFO_1001 struct
     usrmod1001_min_passwd_len dd ?
USER_MODALS_INFO_1001 ends

USER_MODALS_INFO_1002 struct
     usrmod1002_max_passwd_age dd ?
USER_MODALS_INFO_1002 ends

USER_MODALS_INFO_1003 struct
     usrmod1003_min_passwd_age dd ?
USER_MODALS_INFO_1003 ends

USER_MODALS_INFO_1004 struct
     usrmod1004_force_logoff dd ?
USER_MODALS_INFO_1004 ends

USER_MODALS_INFO_1005 struct
    usrmod1005_password_hist_len dd ?
USER_MODALS_INFO_1005 ends

USER_MODALS_INFO_1006 struct
     usrmod1006_role dd ?
USER_MODALS_INFO_1006 ends

USER_MODALS_INFO_1007 struct
     usrmod1007_primary dd ?
USER_MODALS_INFO_1007 ends

GROUP_INFO_0 struct
    grpi0_name dd ?
GROUP_INFO_0 ends

GROUP_INFO_1 struct
    grpi1_name dd ?
    grpi1_comment dd ?
GROUP_INFO_1 ends

GROUP_INFO_2 struct
    grpi2_name dd ?
    grpi2_comment dd ?
    grpi2_group_id dd ?
    grpi2_attributes dd ?
GROUP_INFO_2 ends

GROUP_INFO_1002 struct
     grpi1002_comment dd ?
GROUP_INFO_1002 ends

GROUP_INFO_1005 struct
     grpi1005_attributes dd ?
GROUP_INFO_1005 ends

GROUP_USERS_INFO_0 struct
     grui0_name dd ?
GROUP_USERS_INFO_0 ends

GROUP_USERS_INFO_1 struct
     grui1_name dd ?
     grui1_attributes dd ?
GROUP_USERS_INFO_1 ends

LOCALGROUP_INFO_0 struct
    lgrpi0_name dd ?
LOCALGROUP_INFO_0 ends

LOCALGROUP_INFO_1 struct
    lgrpi1_name dd ?
    lgrpi1_comment dd ?
LOCALGROUP_INFO_1 ends

LOCALGROUP_INFO_1002 struct
     lgrpi1002_comment dd ?
LOCALGROUP_INFO_1002 ends

LOCALGROUP_MEMBERS_INFO_0 struct
     lgrmi0_sid dd ?
LOCALGROUP_MEMBERS_INFO_0 ends

LOCALGROUP_MEMBERS_INFO_1 struct
     lgrmi1_sid dd ?
     lgrmi1_sidusage dd ?
     lgrmi1_name dd ?
LOCALGROUP_MEMBERS_INFO_1 ends

LOCALGROUP_MEMBERS_INFO_2 struct
     lgrmi2_sid dd ?
     lgrmi2_sidusage dd ?
     lgrmi2_domainandname dd ?
LOCALGROUP_MEMBERS_INFO_2 ends

LOCALGROUP_MEMBERS_INFO_3 struct
     lgrmi3_domainandname dd ?
LOCALGROUP_MEMBERS_INFO_3 ends

LOCALGROUP_USERS_INFO_0 struct
     lgrui0_name dd ?
LOCALGROUP_USERS_INFO_0 ends

NET_DISPLAY_USER struct
    usri1_name dd ?
    usri1_comment dd ?
    usri1_flags dd ?
    usri1_full_name dd ?
    usri1_user_id dd ?
    usri1_next_index dd ?
NET_DISPLAY_USER ends

NET_DISPLAY_MACHINE struct
    usri2_name dd ?
    usri2_comment dd ?
    usri2_flags dd ?
    usri2_user_id dd ?
    usri2_next_index dd ?
NET_DISPLAY_MACHINE ends

NET_DISPLAY_GROUP struct
    grpi3_name dd ?
    grpi3_comment dd ?
    grpi3_group_id dd ?
    grpi3_attributes dd ?
    grpi3_next_index dd ?
NET_DISPLAY_GROUP ends

ACCESS_INFO_0 struct
    acc0_resource_name dd ?
ACCESS_INFO_0 ends

ACCESS_INFO_1 struct
    acc1_resource_name dd ?
    acc1_attr dd ?
    acc1_count dd ?
ACCESS_INFO_1 ends

ACCESS_INFO_1002 struct
     acc1002_attr dd ?
ACCESS_INFO_1002 ends


ACCESS_LIST struct
    acl_ugname dd ?
    acl_access dd ?
ACCESS_LIST ends

NETLOGON_INFO_1 struct
        netlog1_flags dd ?
        netlog1_pdc_connection_status NET_API_STATUS ?
NETLOGON_INFO_1 ends

NETLOGON_INFO_2 struct
        netlog2_flags dd ?
        netlog2_pdc_connection_status NET_API_STATUS ?
        netlog2_trusted_dc_name dd ?
        netlog2_tc_connection_status NET_API_STATUS ?
NETLOGON_INFO_2 ends

NETLOGON_INFO_3 struct
        netlog3_flags dd ?
        netlog3_logon_attempts dd ?
        netlog3_reserved1 dd ?
        netlog3_reserved2 dd ?
        netlog3_reserved3 dd ?
        netlog3_reserved4 dd ?
        netlog3_reserved5 dd ?
NETLOGON_INFO_3 ends

NETLOGON_INFO_4 struct
        netlog4_trusted_dc_name dd ?
        netlog4_trusted_domain_name dd ?
NETLOGON_INFO_4 ends

;=============================================[ LMALERT.h ]=========================
ALERTER_MAILSLOT equ "\\\\.\\MAILSLOT\\Alerter"
ALERT_PRINT_EVENT equ "PRINTING"
ALERT_MESSAGE_EVENT equ "MESSAGE"
ALERT_ERRORLOG_EVENT equ "ERRORLOG"
ALERT_ADMIN_EVENT equ "ADMIN"
ALERT_USER_EVENT equ "USER"
PRJOB_QSTATUS equ 3         
PRJOB_DEVSTATUS equ 1fch       
PRJOB_COMPLETE equ 4         
PRJOB_INTERV equ 8         
PRJOB_ERROR equ 10h        
PRJOB_DESTOFFLINE equ 20h        
PRJOB_DESTPAUSED equ 40h        
PRJOB_NOTIFY equ 80h        
PRJOB_DESTNOPAPER equ 100h       
PRJOB_DELETED equ 8000h      
PRJOB_QS_QUEUED equ 0
PRJOB_QS_PAUSED equ 1
PRJOB_QS_SPOOLING equ 2
PRJOB_QS_PRINTING equ 3

STD_ALERT struct
    alrt_timestamp dd ?
    alrt_eventname WCHAR  [EVLEN + 1] dup(?)
    alrt_servicename WCHAR [SNLEN + 1] dup(?)
STD_ALERT ends

ADMIN_OTHER_INFO struct
    alrtad_errcode dd ?
    alrtad_numstrings dd ?
ADMIN_OTHER_INFO ends

ERRLOG_OTHER_INFO struct
    alrter_errcode dd ?
    alrter_offset dd ?
ERRLOG_OTHER_INFO ends

PRINT_OTHER_INFO struct
    alrtpr_jobid dd ?
    alrtpr_status dd ?
    alrtpr_submitted dd ?
    alrtpr_size dd ?
PRINT_OTHER_INFO ends

USER_OTHER_INFO struct
    alrtus_errcode dd ?
    alrtus_numstrings dd ?
USER_OTHER_INFO ends

;================================[ LMAT.H'=====================================
JOB_RUN_PERIODICALLY equ 1    
JOB_EXEC_ERROR equ 2    
JOB_RUNS_TODAY equ 4    
JOB_ADD_CURRENT_DATE equ 8    
JOB_NONINTERACTIVE equ 10h    
JOB_INPUT_FLAGS equ (JOB_RUN_PERIODICALLY+JOB_ADD_CURRENT_DATE+JOB_NONINTERACTIVE)
JOB_OUTPUT_FLAGS equ (JOB_RUN_PERIODICALLY+JOB_EXEC_ERROR+JOB_RUNS_TODAY+JOB_NONINTERACTIVE)      
AT_INFO struct
    JobTime dd ?
    DaysOfMonth dd ?
    DaysOfWeek UCHAR ?
    Flags UCHAR ?
    Command dd ?
AT_INFO ends

AT_ENUM struct
    JobId dd ?
    JobTime dd ?
    DaysOfMonth dd ?
    DaysOfWeek UCHAR ?
    Flags UCHAR ?
    Command dd ?
AT_ENUM ends

;=========================================[ LMAUDIT.H ]=========================
LOGFLAGS_FORWARD	equ 0
LOGFLAGS_BACKWARD	equ 1
LOGFLAGS_SEEK	equ 2
ACTION_LOCKOUT equ 00
ACTION_ADMINUNLOCK equ 01
;AE_SRVSTATUS	equ 0
;AE_SESSLOGON	equ 1
;AE_SESSLOGOFF	equ 2
;AE_SESSPWERR	equ 3
;AE_CONNSTART	equ 4
;AE_CONNSTOP	equ 5
;AE_CONNREJ	equ 6
;AE_RESACCESS	equ 7
;AE_RESACCESSREJ	equ 8
;AE_CLOSEFILE	equ 9
;AE_SERVICESTAT	equ 11
;AE_ACLMOD	equ 12
;AE_UASMOD	equ 13
;AE_NETLOGON	equ 14
;AE_NETLOGOFF	equ 15
;AE_NETLOGDENIED equ 16
;AE_ACCLIMITEXCD equ 17
;AE_RESACCESS2	equ 18
;AE_ACLMODFAIL	equ 19
;AE_LOCKOUT equ 20
;AE_GENERIC_TYPE equ 21
AE_SRVSTART	equ 0
AE_SRVPAUSED	equ 1
AE_SRVCONT	equ 2
AE_SRVSTOP	equ 3
AE_GUEST	equ 0		
AE_USER	equ 1
AE_ADMIN	equ 2
AE_NORMAL	equ 0		
AE_USERLIMIT	equ 0
AE_GENERAL	equ 0
AE_ERROR	equ 1
AE_SESSDIS	equ 1
AE_BADPW	equ 1
AE_AUTODIS	equ 2
AE_UNSHARE	equ 2
AE_ADMINPRIVREQD equ 2
AE_ADMINDIS	equ 3
AE_NOACCESSPERM equ 3
AE_ACCRESTRICT	equ 4
AE_NORMAL_CLOSE equ 0
AE_SES_CLOSE equ 1
AE_ADMIN_CLOSE equ 2
AE_LIM_UNKNOWN	equ 0
AE_LIM_LOGONHOURS equ 1
AE_LIM_EXPIRED	equ 2
AE_LIM_INVAL_WKSTA equ 3
AE_LIM_DISABLED equ 4
AE_LIM_DELETED	equ 5
AE_MOD	equ 0
AE_DELETE	equ 1
AE_ADD	equ 2
AE_UAS_USER	equ 0
AE_UAS_GROUP	equ 1
AE_UAS_MODALS	equ 2
SVAUD_SERVICE equ 1
SVAUD_GOODSESSLOGON equ 6
SVAUD_BADSESSLOGON equ 18h
SVAUD_SESSLOGON equ (SVAUD_GOODSESSLOGON + SVAUD_BADSESSLOGON)
SVAUD_GOODNETLOGON equ 60h
SVAUD_BADNETLOGON equ 180h
SVAUD_NETLOGON equ (SVAUD_GOODNETLOGON + SVAUD_BADNETLOGON)
SVAUD_LOGON equ (SVAUD_NETLOGON + SVAUD_SESSLOGON)
SVAUD_GOODUSE equ 600h
SVAUD_BADUSE equ 1800h
SVAUD_USE equ (SVAUD_GOODUSE + SVAUD_BADUSE)
SVAUD_USERLIST equ 2000h
SVAUD_PERMISSIONS equ 4000h
SVAUD_RESOURCE equ 8000h
SVAUD_LOGONLIM	equ 10000h
AA_AUDIT_ALL	equ 1
AA_A_OWNER	equ 4
AA_CLOSE	equ 8
AA_S_OPEN	equ 10h
AA_S_WRITE	equ 20h
AA_S_CREATE	equ 20h
AA_S_DELETE	equ 40h
AA_S_ACL	equ 80h
AA_S_ALL	equ ( AA_S_OPEN + AA_S_WRITE + AA_S_DELETE + AA_S_ACL)
AA_F_OPEN	equ 100h
AA_F_WRITE	equ 200h
AA_F_CREATE	equ 200h
AA_F_DELETE	equ 400h
AA_F_ACL	equ 800h
AA_F_ALL	equ ( AA_F_OPEN + AA_F_WRITE + AA_F_DELETE + AA_F_ACL)
AA_A_OPEN	equ 1000h
AA_A_WRITE	equ 2000h
AA_A_CREATE	equ 2000h
AA_A_DELETE	equ 4000h
AA_A_ACL	equ 8000h
AA_A_ALL	equ ( AA_F_OPEN + AA_F_WRITE + AA_F_DELETE + AA_F_ACL)

HLOG struct
     time dd ?
     last_flags dd ?
     offset1 dd ?
     rec_offset dd ?
HLOG ends

AUDIT_ENTRY struct
     ae_len dd ?
     ae_reserved dd ?
     ae_time dd ?
     ae_type dd ?
     ae_data_offset dd ?
     ae_data_size dd ?
AUDIT_ENTRY ends

AE_SRVSTATUS struct
     ae_sv_status dd ?
AE_SRVSTATUS ends

AE_SESSLOGON struct
     ae_so_compname dd ?
     ae_so_username dd ?
     ae_so_privilege dd ?
AE_SESSLOGON ends

AE_SESSLOGOFF struct
     ae_sf_compname dd ?
     ae_sf_username dd ?
     ae_sf_reason dd ?
AE_SESSLOGOFF ends

AE_SESSPWERR struct
     ae_sp_compname dd ?
     ae_sp_username dd ?
AE_SESSPWERR ends

AE_CONNSTART struct
     ae_ct_compname dd ?
     ae_ct_username dd ?
     ae_ct_netname dd ?
     ae_ct_connid dd ?
AE_CONNSTART ends

AE_CONNSTOP struct
     ae_cp_compname dd ?
     ae_cp_username dd ?
     ae_cp_netname dd ?
     ae_cp_connid dd ?
     ae_cp_reason dd ?
AE_CONNSTOP ends

AE_CONNREJ struct
     ae_cr_compname dd ?
     ae_cr_username dd ?
     ae_cr_netname dd ?
     ae_cr_reason dd ?
AE_CONNREJ ends

AE_RESACCESS struct
     ae_ra_compname dd ?
     ae_ra_username dd ?
     ae_ra_resname dd ?
     ae_ra_operation dd ?
     ae_ra_returncode dd ?
     ae_ra_restype dd ?
     ae_ra_fileid dd ?
AE_RESACCESS ends

AE_RESACCESSREJ struct
     ae_rr_compname dd ?
     ae_rr_username dd ?
     ae_rr_resname dd ?
     ae_rr_operation dd ?
AE_RESACCESSREJ ends

AE_CLOSEFILE struct
     ae_cf_compname dd ?
     ae_cf_username dd ?
     ae_cf_resname dd ?
     ae_cf_fileid dd ?
     ae_cf_duration dd ?
     ae_cf_reason dd ?
AE_CLOSEFILE ends

AE_SERVICESTAT struct
     ae_ss_compname dd ?
     ae_ss_username dd ?
     ae_ss_svcname dd ?
     ae_ss_status dd ?
     ae_ss_code dd ?
     ae_ss_text dd ?
     ae_ss_returnval dd ?
AE_SERVICESTAT ends

AE_ACLMOD struct
     ae_am_compname dd ?
     ae_am_username dd ?
     ae_am_resname dd ?
     ae_am_action dd ?
     ae_am_datalen dd ?
AE_ACLMOD ends

AE_UASMOD struct
     ae_um_compname dd ?
     ae_um_username dd ?
     ae_um_resname dd ?
     ae_um_rectype dd ?
     ae_um_action dd ?
     ae_um_datalen dd ?
AE_UASMOD ends

AE_NETLOGON struct
     ae_no_compname dd ?
     ae_no_username dd ?
     ae_no_privilege dd ?
     ae_no_authflags dd ?
AE_NETLOGON ends

AE_NETLOGOFF struct
     ae_nf_compname dd ?
     ae_nf_username dd ?
     ae_nf_reserved1 dd ?
     ae_nf_reserved2 dd ?
AE_NETLOGOFF ends

AE_ACCLIM struct
     ae_al_compname dd ?
     ae_al_username dd ?
     ae_al_resname dd ?
     ae_al_limit dd ?
AE_ACCLIM ends

AE_LOCKOUT struct
    ae_lk_compname dd ?
    ae_lk_username dd ?
    ae_lk_action dd ?
    ae_lk_bad_pw_count dd ?
AE_LOCKOUT ends

AE_GENERIC struct
     ae_ge_msgfile dd ?
     ae_ge_msgnum dd ?
     ae_ge_params dd ?
     ae_ge_param1 dd ?
     ae_ge_param2 dd ?
     ae_ge_param3 dd ?
     ae_ge_param4 dd ?
     ae_ge_param5 dd ?
     ae_ge_param6 dd ?
     ae_ge_param7 dd ?
     ae_ge_param8 dd ?
     ae_ge_param9 dd ?
AE_GENERIC ends

;==================================[ LMBROWSR.H ]==============================
BROWSER_ROLE_PDC equ 1
BROWSER_ROLE_BDC  equ 2

BROWSER_STATISTICS struct
    StatisticsStartTime LARGE_INTEGER <>
    NumberOfServerAnnouncements LARGE_INTEGER <>
    NumberOfDomainAnnouncements LARGE_INTEGER <>
    NumberOfElectionPackets dd ?
    NumberOfMailslotWrites dd ?
    NumberOfGetBrowserServerListRequests dd ?
    NumberOfServerEnumerations dd ?
    NumberOfDomainEnumerations dd ?
    NumberOfOtherEnumerations dd ?
    NumberOfMissedServerAnnouncements dd ?
    NumberOfMissedMailslotDatagrams dd ?
    NumberOfMissedGetBrowserServerListRequests dd ?
    NumberOfFailedServerAnnounceAllocations dd ?
    NumberOfFailedMailslotAllocations dd ?
    NumberOfFailedMailslotReceives dd ?
    NumberOfFailedMailslotWrites dd ?
    NumberOfFailedMailslotOpens dd ?
    NumberOfDuplicateMasterAnnouncements dd ?
    NumberOfIllegalDatagrams LARGE_INTEGER <>
BROWSER_STATISTICS ends

BROWSER_STATISTICS_100 struct
    StartTime LARGE_INTEGER <>
    NumberOfServerAnnouncements LARGE_INTEGER <>
    NumberOfDomainAnnouncements LARGE_INTEGER <>
    NumberOfElectionPackets dd ?
    NumberOfMailslotWrites dd ?
    NumberOfGetBrowserServerListRequests dd ?
    NumberOfIllegalDatagrams LARGE_INTEGER <>
BROWSER_STATISTICS_100 ends

BROWSER_STATISTICS_101 struct
    StartTime LARGE_INTEGER <>
    NumberOfServerAnnouncements LARGE_INTEGER <>
    NumberOfDomainAnnouncements LARGE_INTEGER <>
    NumberOfElectionPackets dd ?
    NumberOfMailslotWrites dd ?
    NumberOfGetBrowserServerListRequests dd ?
    NumberOfIllegalDatagrams LARGE_INTEGER <>
    NumberOfMissedServerAnnouncements dd ?
    NumberOfMissedMailslotDatagrams dd ?
    NumberOfMissedGetBrowserServerListRequests dd ?
    NumberOfFailedServerAnnounceAllocations dd ?
    NumberOfFailedMailslotAllocations dd ?
    NumberOfFailedMailslotReceives dd ?
    NumberOfFailedMailslotWrites dd ?
    NumberOfFailedMailslotOpens dd ?
    NumberOfDuplicateMasterAnnouncements dd ?
BROWSER_STATISTICS_101 ends


BROWSER_EMULATED_DOMAIN struct
    DomainName dd ?
    EmulatedServerName dd ?
    Role dd ?
BROWSER_EMULATED_DOMAIN ends

;====================================[ LMCHDEV.H ]==============================
CHARDEV_STAT_OPENED equ 2
CHARDEV_STAT_ERROR equ 4
CHARDEV_CLOSE equ 0
CHARDEVQ_DEV_PARMNUM equ 1
CHARDEVQ_PRIORITY_PARMNUM equ 2
CHARDEVQ_DEVS_PARMNUM equ 3
CHARDEVQ_NUMUSERS_PARMNUM equ 4
CHARDEVQ_NUMAHEAD_PARMNUM equ 5
CHARDEVQ_PRIORITY_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + CHARDEVQ_PRIORITY_PARMNUM)
CHARDEVQ_DEVS_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + CHARDEVQ_DEVS_PARMNUM)
CHARDEVQ_MAX_PRIORITY equ 1
CHARDEVQ_MIN_PRIORITY equ 9
CHARDEVQ_DEF_PRIORITY equ 5
CHARDEVQ_NO_REQUESTS equ -1
HANDLE_INFO_LEVEL_1 equ 1
HANDLE_CHARTIME_PARMNUM equ 1
HANDLE_CHARCOUNT_PARMNUM equ 2

CHARDEV_INFO_0 struct
    ch0_dev dd ?
CHARDEV_INFO_0 ends

CHARDEV_INFO_1 struct
    ch1_dev dd ?
    ch1_status dd ?
    ch1_username dd ?
    ch1_time dd ?
CHARDEV_INFO_1 ends

CHARDEVQ_INFO_0 struct
    cq0_dev dd ?
CHARDEVQ_INFO_0 ends

CHARDEVQ_INFO_1 struct
    cq1_dev dd ?
    cq1_priority dd ?
    cq1_devs dd ?
    cq1_numusers dd ?
    cq1_numahead dd ?
CHARDEVQ_INFO_1 ends

CHARDEVQ_INFO_1002 struct
    cq1002_priority dd ?
CHARDEVQ_INFO_1002 ends

CHARDEVQ_INFO_1003 struct
    cq1003_devs dd ?
CHARDEVQ_INFO_1003 ends

HANDLE_INFO_1 struct
    hdli1_chartime dd ?
    hdli1_charcount dd ?
HANDLE_INFO_1 ends

;======================================[ LMCONFIG ]========================
CONFIG_INFO_0 struct
     cfgi0_key dd ?
     cfgi0_data dd ?
CONFIG_INFO_0 ends

;=======================================[ LMDFS.H ]========================

DFS_VOLUME_STATE_OK equ 1
DFS_VOLUME_STATE_INCONSISTENT equ 2
DFS_VOLUME_STATE_OFFLINE equ 3
DFS_VOLUME_STATE_ONLINE equ 4
DFS_STORAGE_STATE_OFFLINE equ 1
DFS_STORAGE_STATE_ONLINE equ 2
DFS_ADD_VOLUME equ 1   
DFS_RESTORE_VOLUME equ 2   
DFS_INFO_1 struct
    EntryPath dd ?
DFS_INFO_1 ends

DFS_INFO_2 struct
    EntryPath dd ?
    Comment dd ?
    State dd ?
    NumberOfStorages dd ?
DFS_INFO_2 ends

DFS_STORAGE_INFO struct
    State dd ?
    ServerName dd ?
    ShareName dd ?
DFS_STORAGE_INFO ends

DFS_INFO_3 struct
    EntryPath dd ?
    Comment dd ?
    State dd ?
    NumberOfStorages dd ?
    Storage dd ?
DFS_INFO_3 ends

DFS_INFO_100 struct
    _comment dd ?
DFS_INFO_100 ends

DFS_INFO_101 struct
    State dd ?
DFS_INFO_101 ends

;================================[ LMERRLOG.H ]===============================
ERRLOG_BASE equ 3100
NELOG_Internal_Error equ (ERRLOG_BASE + 0)
NELOG_Resource_Shortage equ (ERRLOG_BASE + 1)
NELOG_Unable_To_Lock_Segment equ (ERRLOG_BASE + 2)
NELOG_Unable_To_Unlock_Segment equ (ERRLOG_BASE + 3)
NELOG_Uninstall_Service equ (ERRLOG_BASE + 4)
NELOG_Init_Exec_Fail equ (ERRLOG_BASE + 5)
NELOG_Ncb_Error equ (ERRLOG_BASE + 6)
NELOG_Net_Not_Started equ (ERRLOG_BASE + 7)
NELOG_Ioctl_Error equ (ERRLOG_BASE + 8)
NELOG_System_Semaphore equ (ERRLOG_BASE + 9)
NELOG_Init_OpenCreate_Err equ (ERRLOG_BASE + 10)
NELOG_NetBios equ (ERRLOG_BASE + 11)
NELOG_SMB_Illegal equ (ERRLOG_BASE + 12)
NELOG_Service_Fail equ (ERRLOG_BASE + 13)
NELOG_Entries_Lost equ (ERRLOG_BASE + 14)
NELOG_Init_Seg_Overflow equ (ERRLOG_BASE + 20)
NELOG_Srv_No_Mem_Grow equ (ERRLOG_BASE + 21)
NELOG_Access_File_Bad equ (ERRLOG_BASE + 22)
NELOG_Srvnet_Not_Started equ (ERRLOG_BASE + 23)
NELOG_Init_Chardev_Err equ (ERRLOG_BASE + 24)
NELOG_Remote_API equ (ERRLOG_BASE + 25)
NELOG_Ncb_TooManyErr equ (ERRLOG_BASE + 26)
NELOG_Mailslot_err equ (ERRLOG_BASE + 27)
NELOG_ReleaseMem_Alert equ (ERRLOG_BASE + 28)
NELOG_AT_cannot_write equ (ERRLOG_BASE + 29)
NELOG_Cant_Make_Msg_File equ (ERRLOG_BASE + 30)
NELOG_Exec_Netservr_NoMem equ (ERRLOG_BASE + 31)
NELOG_Server_Lock_Failure equ (ERRLOG_BASE + 32)
NELOG_Msg_Shutdown equ (ERRLOG_BASE + 40)
NELOG_Msg_Sem_Shutdown equ (ERRLOG_BASE + 41)
NELOG_Msg_Log_Err equ (ERRLOG_BASE + 50)
NELOG_VIO_POPUP_ERR equ (ERRLOG_BASE + 51)
NELOG_Msg_Unexpected_SMB_Type equ (ERRLOG_BASE + 52)
NELOG_Wksta_Infoseg equ (ERRLOG_BASE + 60)
NELOG_Wksta_Compname equ (ERRLOG_BASE + 61)
NELOG_Wksta_BiosThreadFailure equ (ERRLOG_BASE + 62)
NELOG_Wksta_IniSeg equ (ERRLOG_BASE + 63)
NELOG_Wksta_HostTab_Full equ (ERRLOG_BASE + 64)
NELOG_Wksta_Bad_Mailslot_SMB equ (ERRLOG_BASE + 65)
NELOG_Wksta_UASInit equ (ERRLOG_BASE + 66)
NELOG_Wksta_SSIRelogon equ (ERRLOG_BASE + 67)
NELOG_Build_Name equ (ERRLOG_BASE + 70)
NELOG_Name_Expansion equ (ERRLOG_BASE + 71)
NELOG_Message_Send equ (ERRLOG_BASE + 72)
NELOG_Mail_Slt_Err equ (ERRLOG_BASE + 73)
NELOG_AT_cannot_read equ (ERRLOG_BASE + 74)
NELOG_AT_sched_err equ (ERRLOG_BASE + 75)
NELOG_AT_schedule_file_created equ (ERRLOG_BASE + 76)
NELOG_Srvnet_NB_Open equ (ERRLOG_BASE + 77)
NELOG_AT_Exec_Err equ (ERRLOG_BASE + 78)
NELOG_Lazy_Write_Err equ (ERRLOG_BASE + 80)
NELOG_HotFix equ (ERRLOG_BASE + 81)
NELOG_HardErr_From_Server equ (ERRLOG_BASE + 82)
NELOG_LocalSecFail1 equ (ERRLOG_BASE + 83)
NELOG_LocalSecFail2 equ (ERRLOG_BASE + 84)
NELOG_LocalSecFail3 equ (ERRLOG_BASE + 85)
NELOG_LocalSecGeneralFail equ (ERRLOG_BASE + 86)
NELOG_NetWkSta_Internal_Error equ (ERRLOG_BASE + 90)
NELOG_NetWkSta_No_Resource equ (ERRLOG_BASE + 91)
NELOG_NetWkSta_SMB_Err equ (ERRLOG_BASE + 92)
NELOG_NetWkSta_VC_Err equ (ERRLOG_BASE + 93)
NELOG_NetWkSta_Stuck_VC_Err equ (ERRLOG_BASE + 94)
NELOG_NetWkSta_NCB_Err equ (ERRLOG_BASE + 95)
NELOG_NetWkSta_Write_Behind_Err equ (ERRLOG_BASE + 96)
NELOG_NetWkSta_Reset_Err equ (ERRLOG_BASE + 97)
NELOG_NetWkSta_Too_Many equ (ERRLOG_BASE + 98)
NELOG_Srv_Thread_Failure equ (ERRLOG_BASE + 104)
NELOG_Srv_Close_Failure equ (ERRLOG_BASE + 105)
NELOG_ReplUserCurDir equ (ERRLOG_BASE + 106)
NELOG_ReplCannotMasterDir equ (ERRLOG_BASE + 107)
NELOG_ReplUpdateError equ (ERRLOG_BASE + 108)
NELOG_ReplLostMaster equ (ERRLOG_BASE + 109)
NELOG_NetlogonAuthDCFail equ (ERRLOG_BASE + 110)
NELOG_ReplLogonFailed equ (ERRLOG_BASE + 111)
NELOG_ReplNetErr equ (ERRLOG_BASE + 112)
NELOG_ReplMaxFiles equ (ERRLOG_BASE + 113)
NELOG_ReplMaxTreeDepth equ (ERRLOG_BASE + 114)
NELOG_ReplBadMsg equ (ERRLOG_BASE + 115)
NELOG_ReplSysErr equ (ERRLOG_BASE + 116)
NELOG_ReplUserLoged equ (ERRLOG_BASE + 117)
NELOG_ReplBadImport equ (ERRLOG_BASE + 118)
NELOG_ReplBadExport equ (ERRLOG_BASE + 119)
NELOG_ReplSignalFileErr equ (ERRLOG_BASE + 120)
NELOG_DiskFT equ (ERRLOG_BASE+121)
NELOG_ReplAccessDenied equ (ERRLOG_BASE + 122)
NELOG_NetlogonFailedPrimary equ (ERRLOG_BASE + 123)
NELOG_NetlogonPasswdSetFailed equ (ERRLOG_BASE + 124)
NELOG_NetlogonTrackingError equ (ERRLOG_BASE + 125)
NELOG_NetlogonSyncError equ (ERRLOG_BASE + 126)
NELOG_UPS_PowerOut equ (ERRLOG_BASE + 130)
NELOG_UPS_Shutdown equ (ERRLOG_BASE + 131)
NELOG_UPS_CmdFileError equ (ERRLOG_BASE + 132)
NELOG_UPS_CannotOpenDriver equ (ERRLOG_BASE+133)
NELOG_UPS_PowerBack equ (ERRLOG_BASE + 134)
NELOG_UPS_CmdFileConfig equ (ERRLOG_BASE + 135)
NELOG_UPS_CmdFileExec equ (ERRLOG_BASE + 136)
NELOG_Missing_Parameter equ (ERRLOG_BASE + 150)
NELOG_Invalid_Config_Line equ (ERRLOG_BASE + 151)
NELOG_Invalid_Config_File equ (ERRLOG_BASE + 152)
NELOG_File_Changed equ (ERRLOG_BASE + 153)
NELOG_Files_Dont_Fit equ (ERRLOG_BASE + 154)
NELOG_Wrong_DLL_Version equ (ERRLOG_BASE + 155)
NELOG_Error_in_DLL equ (ERRLOG_BASE + 156)
NELOG_System_Error equ (ERRLOG_BASE + 157)
NELOG_FT_ErrLog_Too_Large equ (ERRLOG_BASE + 158)
NELOG_FT_Update_In_Progress equ (ERRLOG_BASE + 159)
NELOG_OEM_Code equ (ERRLOG_BASE + 199)
ERRLOG2_BASE equ 5700
NELOG_NetlogonSSIInitError equ (ERRLOG2_BASE + 0)
NELOG_NetlogonFailedToUpdateTrustList equ (ERRLOG2_BASE + 1)
NELOG_NetlogonFailedToAddRpcInterface equ (ERRLOG2_BASE + 2)
NELOG_NetlogonFailedToReadMailslot equ (ERRLOG2_BASE + 3)
NELOG_NetlogonFailedToRegisterSC equ (ERRLOG2_BASE + 4)
NELOG_NetlogonChangeLogCorrupt equ (ERRLOG2_BASE + 5)
NELOG_NetlogonFailedToCreateShare equ (ERRLOG2_BASE + 6)
NELOG_NetlogonDownLevelLogonFailed equ (ERRLOG2_BASE + 7)
NELOG_NetlogonDownLevelLogoffFailed equ (ERRLOG2_BASE + 8)
NELOG_NetlogonNTLogonFailed equ (ERRLOG2_BASE + 9)
NELOG_NetlogonNTLogoffFailed equ (ERRLOG2_BASE + 10)
NELOG_NetlogonPartialSyncCallSuccess equ (ERRLOG2_BASE + 11)
NELOG_NetlogonPartialSyncCallFailed equ (ERRLOG2_BASE + 12)
NELOG_NetlogonFullSyncCallSuccess equ (ERRLOG2_BASE + 13)
NELOG_NetlogonFullSyncCallFailed equ (ERRLOG2_BASE + 14)
NELOG_NetlogonPartialSyncSuccess equ (ERRLOG2_BASE + 15)
NELOG_NetlogonPartialSyncFailed equ (ERRLOG2_BASE + 16)
NELOG_NetlogonFullSyncSuccess equ (ERRLOG2_BASE + 17)
NELOG_NetlogonFullSyncFailed equ (ERRLOG2_BASE + 18)
NELOG_NetlogonAuthNoDomainController equ (ERRLOG2_BASE + 19)
NELOG_NetlogonAuthNoTrustLsaSecret equ (ERRLOG2_BASE + 20)
NELOG_NetlogonAuthNoTrustSamAccount equ (ERRLOG2_BASE + 21)
NELOG_NetlogonServerAuthFailed equ (ERRLOG2_BASE + 22)
NELOG_NetlogonServerAuthNoTrustSamAccount equ (ERRLOG2_BASE + 23)
NELOG_FailedToRegisterSC equ (ERRLOG2_BASE + 24)
NELOG_FailedToSetServiceStatus equ (ERRLOG2_BASE + 25)
NELOG_FailedToGetComputerName equ (ERRLOG2_BASE + 26)
NELOG_DriverNotLoaded equ (ERRLOG2_BASE + 27)
NELOG_NoTranportLoaded equ (ERRLOG2_BASE + 28)
NELOG_NetlogonFailedDomainDelta equ (ERRLOG2_BASE + 29)
NELOG_NetlogonFailedGlobalGroupDelta equ (ERRLOG2_BASE + 30)
NELOG_NetlogonFailedLocalGroupDelta equ (ERRLOG2_BASE + 31)
NELOG_NetlogonFailedUserDelta equ (ERRLOG2_BASE + 32)
NELOG_NetlogonFailedPolicyDelta equ (ERRLOG2_BASE + 33)
NELOG_NetlogonFailedTrustedDomainDelta equ (ERRLOG2_BASE + 34)
NELOG_NetlogonFailedAccountDelta equ (ERRLOG2_BASE + 35)
NELOG_NetlogonFailedSecretDelta equ (ERRLOG2_BASE + 36)
NELOG_NetlogonSystemError equ (ERRLOG2_BASE + 37)
NELOG_NetlogonDuplicateMachineAccounts equ (ERRLOG2_BASE + 38)
NELOG_NetlogonTooManyGlobalGroups equ (ERRLOG2_BASE + 39)
NELOG_NetlogonBrowserDriver equ (ERRLOG2_BASE + 40)
NELOG_NetlogonAddNameFailure equ (ERRLOG2_BASE + 41)
NELOG_RplMessages equ (ERRLOG2_BASE + 42)
NELOG_RplXnsBoot equ (ERRLOG2_BASE + 43)
NELOG_RplSystem equ (ERRLOG2_BASE + 44)
NELOG_RplWkstaTimeout equ (ERRLOG2_BASE + 45)
NELOG_RplWkstaFileOpen equ (ERRLOG2_BASE + 46)
NELOG_RplWkstaFileRead equ (ERRLOG2_BASE + 47)
NELOG_RplWkstaMemory equ (ERRLOG2_BASE + 48)
NELOG_RplWkstaFileChecksum equ (ERRLOG2_BASE + 49)
NELOG_RplWkstaFileLineCount equ (ERRLOG2_BASE + 50)
NELOG_RplWkstaBbcFile equ (ERRLOG2_BASE + 51)
NELOG_RplWkstaFileSize equ (ERRLOG2_BASE + 52)
NELOG_RplWkstaInternal equ (ERRLOG2_BASE + 53)
NELOG_RplWkstaWrongVersion equ (ERRLOG2_BASE + 54)
NELOG_RplWkstaNetwork equ (ERRLOG2_BASE + 55)
NELOG_RplAdapterResource equ (ERRLOG2_BASE + 56)
NELOG_RplFileCopy equ (ERRLOG2_BASE + 57)
NELOG_RplFileDelete equ (ERRLOG2_BASE + 58)
NELOG_RplFilePerms equ (ERRLOG2_BASE + 59)
NELOG_RplCheckConfigs equ (ERRLOG2_BASE + 60)
NELOG_RplCreateProfiles equ (ERRLOG2_BASE + 61)
NELOG_RplRegistry equ (ERRLOG2_BASE + 62)
NELOG_RplReplaceRPLDISK equ (ERRLOG2_BASE + 63)
NELOG_RplCheckSecurity equ (ERRLOG2_BASE + 64)
NELOG_RplBackupDatabase equ (ERRLOG2_BASE + 65)
NELOG_RplInitDatabase equ (ERRLOG2_BASE + 66)
NELOG_RplRestoreDatabaseFailure equ (ERRLOG2_BASE + 67)
NELOG_RplRestoreDatabaseSuccess equ (ERRLOG2_BASE + 68)
NELOG_RplInitRestoredDatabase equ (ERRLOG2_BASE + 69)
NELOG_NetlogonSessionTypeWrong equ (ERRLOG2_BASE + 70)
NELOG_RplUpgradeDBTo40 equ (ERRLOG2_BASE + 71)
NELOG_NetlogonLanmanBdcsNotAllowed equ (ERRLOG2_BASE + 72)
NELOG_NetlogonNoDynamicDns equ (ERRLOG2_BASE + 73)
NELOG_NetlogonDynamicDnsRegisterFailure equ (ERRLOG2_BASE + 74)
NELOG_NetlogonDynamicDnsDeregisterFailure equ (ERRLOG2_BASE + 75)
NELOG_NetlogonFailedFileCreate equ (ERRLOG2_BASE + 76)
NELOG_NetlogonGetSubnetToSite equ (ERRLOG2_BASE + 77)
NELOG_NetlogonNoSiteForClient equ (ERRLOG2_BASE + 78)
NELOG_NetlogonBadSiteName equ (ERRLOG2_BASE + 79)
NELOG_NetlogonBadSubnetName equ (ERRLOG2_BASE + 80)

ERROR_LOG struct
     el_len dd ?
     el_reserved dd ?
     el_time dd ?
     el_error dd ?
     el_name dd ?
     el_text dd ?
     el_data dd ?
     el_data_size dd ?
     el_nstrings dd ?
ERROR_LOG ends

;===============================[ LMMSG.H]=============================
MSGNAME_NOT_FORWARDED   equ 0
MSGNAME_FORWARDED_TO   equ 4
MSGNAME_FORWARDED_FROM  equ 10h

MSG_INFO_0 struct
    msgi0_name dd ?
MSG_INFO_0 ends

MSG_INFO_1 struct
    msgi1_name dd ?
    msgi1_forward_flag dd ?
    msgi1_forward dd ?
MSG_INFO_1 ends

;=================================[ LMREMUTL.H]==========================
TIME_OF_DAY_INFO struct
    tod_elapsedt dd ?
    tod_msecs dd ?
    tod_hours dd ?
    tod_mins dd ?
    tod_secs dd ?
    tod_hunds dd ?
    tod_timezone dd ?
    tod_tinterval dd ?
    tod_day dd ?
    tod_month dd ?
    tod_year dd ?
    tod_weekday dd ?
TIME_OF_DAY_INFO ends

SUPPORTS_REMOTE_ADMIN_PROTOCOL equ 2
SUPPORTS_RPC equ 4
SUPPORTS_SAM_PROTOCOL equ 8
SUPPORTS_UNICODE equ 10h
SUPPORTS_LOCAL equ 20h
SUPPORTS_ANY equ 0FFFFFFFFh
NO_PERMISSION_REQUIRED equ 1
ALLOCATE_RESPONSE equ 2
USE_SPECIFIC_TRANSPORT equ 80000000h

;===============================[ LMREPL.H]=======================
REPL_ROLE_EXPORT equ 1
REPL_ROLE_IMPORT equ 2
REPL_ROLE_BOTH equ 3
REPL_INTERVAL_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + 0)
REPL_PULSE_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + 1)
REPL_GUARDTIME_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + 2)
REPL_RANDOM_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + 3)
REPL_INTEGRITY_FILE equ 1
REPL_INTEGRITY_TREE equ 2
REPL_EXTENT_FILE equ 1
REPL_EXTENT_TREE equ 2
REPL_EXPORT_INTEGRITY_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + 0)
REPL_EXPORT_EXTENT_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + 1)
REPL_UNLOCK_NOFORCE equ 0
REPL_UNLOCK_FORCE equ 1
REPL_STATE_OK equ 0
REPL_STATE_NO_MASTER equ 1
REPL_STATE_NO_SYNC equ 2
REPL_STATE_NEVER_REPLICATED equ 3

REPL_INFO_0 struct
    rp0_role dd ?
    rp0_exportpath dd ?
    rp0_exportlist dd ?
    rp0_importpath dd ?
    rp0_importlist dd ?
    rp0_logonusername dd ?
    rp0_interval dd ?
    rp0_pulse dd ?
    rp0_guardtime dd ?
    rp0_random dd ?
REPL_INFO_0 ends

REPL_INFO_1000 struct
    rp1000_interval dd ?
REPL_INFO_1000 ends

REPL_INFO_1001 struct
    rp1001_pulse dd ?
REPL_INFO_1001 ends

REPL_INFO_1002 struct
    rp1002_guardtime dd ?
REPL_INFO_1002 ends

REPL_INFO_1003 struct
    rp1003_random dd ?
REPL_INFO_1003 ends

REPL_EDIR_INFO_0 struct
    rped0_dirname dd ?
REPL_EDIR_INFO_0 ends

REPL_EDIR_INFO_1 struct
    rped1_dirname dd ?
    rped1_integrity dd ?
    rped1_extent dd ?
REPL_EDIR_INFO_1 ends

REPL_EDIR_INFO_2 struct
    rped2_dirname dd ?
    rped2_integrity dd ?
    rped2_extent dd ?
    rped2_lockcount dd ?
    rped2_locktime dd ?
REPL_EDIR_INFO_2 ends

REPL_EDIR_INFO_1000 struct
    rped1000_integrity dd ?
REPL_EDIR_INFO_1000 ends

REPL_EDIR_INFO_1001 struct
    rped1001_extent dd ?
REPL_EDIR_INFO_1001 ends

REPL_IDIR_INFO_0 struct 
    rpid0_dirname dd ?
REPL_IDIR_INFO_0 ends

REPL_IDIR_INFO_1 struct
    rpid1_dirname dd ?
    rpid1_state dd ?
    rpid1_mastername dd ?
    rpid1_last_update_time dd ?
    rpid1_lockcount dd ?
    rpid1_locktime dd ?
REPL_IDIR_INFO_1 ends

;==============================[ LMSHARE.H ]=============================
SHARE_NETNAME_PARMNUM equ 1
SHARE_TYPE_PARMNUM equ 3
SHARE_REMARK_PARMNUM equ 4
SHARE_PERMISSIONS_PARMNUM equ 5
SHARE_MAX_USES_PARMNUM equ 6
SHARE_CURRENT_USES_PARMNUM equ 7
SHARE_PATH_PARMNUM equ 8
SHARE_PASSWD_PARMNUM equ 9
SHARE_FILE_SD_PARMNUM equ 501
SHARE_REMARK_INFOLEVEL equ  (PARMNUM_BASE_INFOLEVEL + SHARE_REMARK_PARMNUM)
SHARE_MAX_USES_INFOLEVEL equ  (PARMNUM_BASE_INFOLEVEL + SHARE_MAX_USES_PARMNUM)
SHARE_FILE_SD_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SHARE_FILE_SD_PARMNUM)
SHI1_NUM_ELEMENTS equ 4
SHI2_NUM_ELEMENTS equ 10
STYPE_DISKTREE equ 0
STYPE_PRINTQ equ 1
STYPE_DEVICE equ 2
STYPE_IPC equ 3
STYPE_SPECIAL equ 80000000h
SHI_USES_UNLIMITED equ (DWORD)-1
SHI1005_FLAGS_DFS equ 1    
SHI1005_FLAGS_DFS_ROOT equ 2    
COW_PERMACHINE equ 4    
COW_PERUSER equ 8    
CSC_CACHEABLE equ 10h    
CSC_NOFLOWOPS equ 20h    
CSC_AUTO_INWARD equ 40h    
CSC_AUTO_OUTWARD equ 80h    
SHI1005_VALID_FLAGS_SET equ (   CSC_CACHEABLE + CSC_NOFLOWOPS + CSC_AUTO_INWARD + CSC_AUTO_OUTWARD+ COW_PERMACHINE  + COW_PERUSER)
SHI1007_VALID_FLAGS_SET equ SHI1005_VALID_FLAGS_SET
SESS_GUEST equ 1  
SESS_NOENCRYPTION equ 2  
SESI1_NUM_ELEMENTS equ 8
SESI2_NUM_ELEMENTS equ 9
PERM_FILE_READ equ 1 
PERM_FILE_WRITE equ 2 
PERM_FILE_CREATE equ 4 

SHARE_INFO_0 struct
    shi0_netname dd ?
SHARE_INFO_0 ends

SHARE_INFO_1 struct
    shi1_netname dd ?
    shi1_type dd ?
    shi1_remark dd ?
SHARE_INFO_1 ends

SHARE_INFO_2 struct
    shi2_netname dd ?
    shi2_type dd ?
    shi2_remark dd ?
    shi2_permissions dd ?
    shi2_max_uses dd ?
    shi2_current_uses dd ?
    shi2_path dd ?
    shi2_passwd dd ?
SHARE_INFO_2 ends

SHARE_INFO_501 struct
    shi501_netname dd ?
    shi501_type dd ?
    shi501_remark dd ?
    shi501_flags dd ?
SHARE_INFO_501 ends

SHARE_INFO_502 struct
    shi502_netname dd ?
    shi502_type dd ?
    shi502_remark dd ?
    shi502_permissions dd ?
    shi502_max_uses dd ?
    shi502_current_uses dd ?
    shi502_path dd ?
    shi502_passwd dd ?
    shi502_reserved dd ?
    shi502_security_descriptor dd ?
SHARE_INFO_502 ends

SHARE_INFO_1004 struct 
    shi1004_remark dd ?
SHARE_INFO_1004 ends

SHARE_INFO_1005 struct
    shi1005_flags dd ?
SHARE_INFO_1005 ends

SHARE_INFO_1006 struct
    shi1006_max_uses dd ?
SHARE_INFO_1006 ends

SHARE_INFO_1007 struct
    shi1007_flags dd ?
    shi1007_AlternateDirectoryName dd ?
SHARE_INFO_1007 ends

SHARE_INFO_1501 struct
    shi1501_reserved dd ?
    shi1501_security_descriptor dd ?
SHARE_INFO_1501 ends

SESSION_INFO_0 struct
    sesi0_cname dd ?
SESSION_INFO_0 ends

SESSION_INFO_1 struct
    sesi1_cname dd ?
    sesi1_username dd ?
    sesi1_num_opens dd ?
    sesi1_time dd ?
    sesi1_idle_time dd ?
    sesi1_user_flags dd ?
SESSION_INFO_1 ends

SESSION_INFO_2 struct
    sesi2_cname dd ?
    sesi2_username dd ?
    sesi2_num_opens dd ?
    sesi2_time dd ?
    sesi2_idle_time dd ?
    sesi2_user_flags dd ?
    sesi2_cltype_name dd ?
SESSION_INFO_2 ends

SESSION_INFO_10 struct
    sesi10_cname dd ?
    sesi10_username dd ?
    sesi10_time dd ?
    sesi10_idle_time dd ?
SESSION_INFO_10 ends

SESSION_INFO_502 struct
    sesi502_cname dd ?
    sesi502_username dd ?
    sesi502_num_opens dd ?
    sesi502_time dd ?
    sesi502_idle_time dd ?
    sesi502_user_flags dd ?
    sesi502_cltype_name dd ?
    sesi502_transport dd ?
SESSION_INFO_502 ends

CONNECTION_INFO_0 struct
    coni0_id dd ?
CONNECTION_INFO_0 ends

CONNECTION_INFO_1 struct
    coni1_id dd ?
    coni1_type dd ?
    coni1_num_opens dd ?
    coni1_num_users dd ?
    coni1_time dd ?
    coni1_username dd ?
    coni1_netname dd ?
CONNECTION_INFO_1 ends

FILE_INFO_2 struct
    fi2_id dd ?
FILE_INFO_2 ends

FILE_INFO_3 struct
    fi3_id dd ?
    fi3_permissions dd ?
    fi3_num_locks dd ?
    fi3_pathname dd ?
    fi3_username dd ?
FILE_INFO_3 ends

;===================================[ LMSNAME.H ]===================
SERVICE_WORKSTATION equ "LanmanWorkstation"
SERVICE_LM20_WORKSTATION equ "WORKSTATION"
WORKSTATION_DISPLAY_NAME equ "Workstation"
SERVICE_SERVER equ "LanmanServer"
SERVICE_LM20_SERVER equ "SERVER"
SERVER_DISPLAY_NAME equ "Server"
SERVICE_BROWSER equ "BROWSER"
SERVICE_LM20_BROWSER equ SERVICE_BROWSER
SERVICE_MESSENGER equ "MESSENGER"
SERVICE_LM20_MESSENGER equ SERVICE_MESSENGER
SERVICE_NETRUN equ "NETRUN"
SERVICE_LM20_NETRUN equ SERVICE_NETRUN
SERVICE_SPOOLER equ "SPOOLER"
SERVICE_LM20_SPOOLER equ SERVICE_SPOOLER
SERVICE_ALERTER equ "ALERTER"
SERVICE_LM20_ALERTER equ SERVICE_ALERTER
SERVICE_NETLOGON equ "NETLOGON"
SERVICE_LM20_NETLOGON equ SERVICE_NETLOGON
SERVICE_NETPOPUP equ "NETPOPUP"
SERVICE_LM20_NETPOPUP equ SERVICE_NETPOPUP
SERVICE_SQLSERVER equ "SQLSERVER"
SERVICE_LM20_SQLSERVER equ SERVICE_SQLSERVER
SERVICE_REPL equ "REPLICATOR"
SERVICE_LM20_REPL equ SERVICE_REPL
SERVICE_RIPL equ "REMOTEBOOT"
SERVICE_LM20_RIPL equ SERVICE_RIPL
SERVICE_TIMESOURCE equ "TIMESOURCE"
SERVICE_LM20_TIMESOURCE equ SERVICE_TIMESOURCE
SERVICE_AFP equ "AFP"
SERVICE_LM20_AFP equ SERVICE_AFP
SERVICE_UPS equ "UPS"
SERVICE_LM20_UPS equ SERVICE_UPS
SERVICE_XACTSRV equ "XACTSRV"
SERVICE_LM20_XACTSRV equ SERVICE_XACTSRV
SERVICE_TCPIP equ "TCPIP"
SERVICE_LM20_TCPIP equ SERVICE_TCPIP
SERVICE_NBT equ "NBT"
SERVICE_LM20_NBT equ SERVICE_NBT
SERVICE_LMHOSTS equ "LMHOSTS"
SERVICE_LM20_LMHOSTS equ SERVICE_LMHOSTS
SERVICE_TELNET equ "Telnet"
SERVICE_LM20_TELNET equ SERVICE_TELNET
SERVICE_SCHEDULE equ "Schedule"
SERVICE_LM20_SCHEDULE equ SERVICE_SCHEDULE
SERVICE_NTLMSSP equ "NtLmSsp"
SERVICE_DHCP equ "DHCP"
SERVICE_LM20_DHCP equ SERVICE_DHCP
SERVICE_NWSAP equ "NwSapAgent"
SERVICE_LM20_NWSAP equ SERVICE_NWSAP
NWSAP_DISPLAY_NAME equ "NW Sap Agent"
SERVICE_NWCS equ "NWCWorkstation"
SERVICE_DNS_CACHE equ "DnsCache"
SERVICE_W32TIME equ "w32time"
SERVCE_LM20_W32TIME equ SERVICE_W32TIME
SERVICE_KDC equ "kdc"
SERVICE_LM20_KDC equ SERVICE_KDC
SERVICE_RPCLOCATOR equ "RPCLOCATOR"
SERVICE_LM20_RPCLOCATOR equ SERVICE_RPCLOCATOR
SERVICE_TRKSVR equ "TrkSvr"
SERVICE_LM20_TRKSVR equ SERVICE_TRKSVR
SERVICE_TRKWKS equ "TrkWks"
SERVICE_LM20_TRKWKS equ SERVICE_TRKWKS

;====================================[ LMSTATS.H ]======================
STATSOPT_CLR equ 1
STATS_NO_VALUE  equ -1
STATS_OVERFLOW  equ  -2

STAT_WORKSTATION_0 struct
     stw0_start dd ?
     stw0_numNCB_r dd ?
     stw0_numNCB_s dd ?
     stw0_numNCB_a dd ?
     stw0_fiNCB_r dd ?
     stw0_fiNCB_s dd ?
     stw0_fiNCB_a dd ?
     stw0_fcNCB_r dd ?
     stw0_fcNCB_s dd ?
     stw0_fcNCB_a dd ?
     stw0_sesstart dd ?
     stw0_sessfailcon dd ?
     stw0_sessbroke dd ?
     stw0_uses dd ?
     stw0_usefail dd ?
     stw0_autorec dd ?
     stw0_bytessent_r_lo dd ?
     stw0_bytessent_r_hi dd ?
     stw0_bytesrcvd_r_lo dd ?
     stw0_bytesrcvd_r_hi dd ?
     stw0_bytessent_s_lo dd ?
     stw0_bytessent_s_hi dd ?
     stw0_bytesrcvd_s_lo dd ?
     stw0_bytesrcvd_s_hi dd ?
     stw0_bytessent_a_lo dd ?
     stw0_bytessent_a_hi dd ?
     stw0_bytesrcvd_a_lo dd ?
     stw0_bytesrcvd_a_hi dd ?
     stw0_reqbufneed dd ?
     stw0_bigbufneed dd ?
STAT_WORKSTATION_0 ends

STAT_SERVER_0 struct
     sts0_start dd ?
     sts0_fopens dd ?
     sts0_devopens dd ?
     sts0_jobsqueued dd ?
     sts0_sopens dd ?
     sts0_stimedout dd ?
     sts0_serrorout dd ?
     sts0_pwerrors dd ?
     sts0_permerrors dd ?
     sts0_syserrors dd ?
     sts0_bytessent_low dd ?
     sts0_bytessent_high dd ?
     sts0_bytesrcvd_low dd ?
     sts0_bytesrcvd_high dd ?
     sts0_avresponse dd ?
     sts0_reqbufneed dd ?
     sts0_bigbufneed dd ?
STAT_SERVER_0 ends

;===========================[ LMSVC.H ]============================
SERVICE_INFO_0 struct
    svci0_name dd ?
SERVICE_INFO_0 ends

SERVICE_INFO_1 struct
    svci1_name dd ?
    svci1_status dd ?
    svci1_code dd ?
    svci1_pid dd ?
SERVICE_INFO_1 ends

SERVICE_INFO_2 struct
    svci2_name dd ?
    svci2_status dd ?
    svci2_code dd ?
    svci2_pid dd ?
    svci2_text dd ?
    svci2_specific_error dd ?
    svci2_display_name dd ?
SERVICE_INFO_2 ends

SERVICE_INSTALL_STATE equ 3
SERVICE_UNINSTALLED equ 0
SERVICE_INSTALL_PENDING equ 1
SERVICE_UNINSTALL_PENDING equ 2
SERVICE_INSTALLED equ 3
SERVICE_PAUSE_STATE equ 0Ch
LM20_SERVICE_ACTIVE equ 0
LM20_SERVICE_CONTINUE_PENDING equ 4
LM20_SERVICE_PAUSE_PENDING equ 8
LM20_SERVICE_PAUSED equ 0Ch
SERVICE_NOT_UNINSTALLABLE equ 0
SERVICE_UNINSTALLABLE equ 10h
SERVICE_NOT_PAUSABLE equ 0
SERVICE_PAUSABLE equ 20h
SERVICE_REDIR_PAUSED equ 700h
SERVICE_REDIR_DISK_PAUSED equ 100h
SERVICE_REDIR_PRINT_PAUSED equ 200h
SERVICE_REDIR_COMM_PAUSED equ 400h
SERVICE_DOS_ENCRYPTION equ L"ENCRYPT"
SERVICE_CTRL_INTERROGATE equ 0
SERVICE_CTRL_PAUSE equ 1
SERVICE_CTRL_CONTINUE equ 2
SERVICE_CTRL_UNINSTALL equ 3
SERVICE_CTRL_REDIR_DISK equ 1
SERVICE_CTRL_REDIR_PRINT equ 2
SERVICE_CTRL_REDIR_COMM equ 4
SERVICE_IP_NO_HINT equ 0
SERVICE_CCP_NO_HINT equ 0
SERVICE_IP_QUERY_HINT equ 10000h
SERVICE_CCP_QUERY_HINT equ 10000h
SERVICE_IP_CHKPT_NUM equ 0FFh
SERVICE_CCP_CHKPT_NUM equ 0FFh
SERVICE_IP_WAIT_TIME equ 0FF00h
SERVICE_CCP_WAIT_TIME equ 0FF00h
SERVICE_IP_WAITTIME_SHIFT equ 8
SERVICE_NTIP_WAITTIME_SHIFT equ 12
UPPER_HINT_MASK equ 0FF00h
LOWER_HINT_MASK equ 0FFh
UPPER_GET_HINT_MASK equ 0FF00000h
LOWER_GET_HINT_MASK equ 0FF00h
SERVICE_NT_MAXTIME equ 0FFFFh
SERVICE_RESRV_MASK equ 1FFFFh
SERVICE_MAXTIME equ 0FFh
SERVICE_BASE equ 3050
SERVICE_UIC_NORMAL equ 0
SERVICE_UIC_BADPARMVAL equ (SERVICE_BASE + 1)
SERVICE_UIC_MISSPARM equ (SERVICE_BASE + 2)
SERVICE_UIC_UNKPARM equ (SERVICE_BASE + 3)
SERVICE_UIC_RESOURCE equ (SERVICE_BASE + 4)
SERVICE_UIC_CONFIG equ (SERVICE_BASE + 5)
SERVICE_UIC_SYSTEM equ (SERVICE_BASE + 6)
SERVICE_UIC_INTERNAL equ (SERVICE_BASE + 7)
SERVICE_UIC_AMBIGPARM equ (SERVICE_BASE + 8)
SERVICE_UIC_DUPPARM equ (SERVICE_BASE + 9)
SERVICE_UIC_KILL equ (SERVICE_BASE + 10)
SERVICE_UIC_EXEC equ (SERVICE_BASE + 11)
SERVICE_UIC_SUBSERV equ (SERVICE_BASE + 12)
SERVICE_UIC_CONFLPARM equ (SERVICE_BASE + 13)
SERVICE_UIC_FILE equ (SERVICE_BASE + 14)
SERVICE_UIC_M_NULL equ 0
SERVICE_UIC_M_MEMORY equ (SERVICE_BASE + 20)
SERVICE_UIC_M_DISK equ (SERVICE_BASE + 21)
SERVICE_UIC_M_THREADS equ (SERVICE_BASE + 22)
SERVICE_UIC_M_PROCESSES equ (SERVICE_BASE + 23)
SERVICE_UIC_M_SECURITY equ (SERVICE_BASE + 24)
SERVICE_UIC_M_LANROOT equ (SERVICE_BASE + 25)
SERVICE_UIC_M_REDIR equ (SERVICE_BASE + 26)
SERVICE_UIC_M_SERVER equ (SERVICE_BASE + 27)
SERVICE_UIC_M_SEC_FILE_ERR equ (SERVICE_BASE + 28)
SERVICE_UIC_M_FILES equ (SERVICE_BASE + 29)
SERVICE_UIC_M_LOGS equ (SERVICE_BASE + 30)
SERVICE_UIC_M_LANGROUP equ (SERVICE_BASE + 31)
SERVICE_UIC_M_MSGNAME equ (SERVICE_BASE + 32)
SERVICE_UIC_M_ANNOUNCE equ (SERVICE_BASE + 33)
SERVICE_UIC_M_UAS equ (SERVICE_BASE + 34)
SERVICE_UIC_M_SERVER_SEC_ERR equ (SERVICE_BASE + 35)
SERVICE_UIC_M_WKSTA equ (SERVICE_BASE + 37)
SERVICE_UIC_M_ERRLOG equ (SERVICE_BASE + 38)
SERVICE_UIC_M_FILE_UW equ (SERVICE_BASE + 39)
SERVICE_UIC_M_ADDPAK equ (SERVICE_BASE + 40)
SERVICE_UIC_M_LAZY equ (SERVICE_BASE + 41)
SERVICE_UIC_M_UAS_MACHINE_ACCT equ (SERVICE_BASE + 42)
SERVICE_UIC_M_UAS_SERVERS_NMEMB equ (SERVICE_BASE + 43)
SERVICE_UIC_M_UAS_SERVERS_NOGRP equ (SERVICE_BASE + 44)
SERVICE_UIC_M_UAS_INVALID_ROLE equ (SERVICE_BASE + 45)
SERVICE_UIC_M_NETLOGON_NO_DC equ (SERVICE_BASE + 46)
SERVICE_UIC_M_NETLOGON_DC_CFLCT equ (SERVICE_BASE + 47)
SERVICE_UIC_M_NETLOGON_AUTH equ (SERVICE_BASE + 48)
SERVICE_UIC_M_UAS_PROLOG equ (SERVICE_BASE + 49)
SERVICE2_BASE equ 5600
SERVICE_UIC_M_NETLOGON_MPATH equ (SERVICE2_BASE + 0)
SERVICE_UIC_M_LSA_MACHINE_ACCT equ (SERVICE2_BASE + 1)
SERVICE_UIC_M_DATABASE_ERROR equ (SERVICE2_BASE + 2)

;==============================[ LMUSE.H ]======================
USE_INFO_0 struct
    ui0_local dd ?
    ui0_remote dd ?
USE_INFO_0 ends

USE_INFO_1 struct
    ui1_local dd ?
    ui1_remote dd ?
    ui1_password dd ?
    ui1_status dd ?
    ui1_asg_type dd ?
    ui1_refcount dd ?
    ui1_usecount dd ?
USE_INFO_1 ends

USE_INFO_2 struct
    ui2_local dd ?
    ui2_remote dd ?
    ui2_password dd ?
    ui2_status dd ?
    ui2_asg_type dd ?
    ui2_refcount dd ?
    ui2_usecount dd ?
    ui2_username dd ?
    ui2_domainname dd ?
USE_INFO_2 ends

USE_INFO_3 struct
    ui3_ui2 USE_INFO_2 <>
    ui3_flags dd ?
USE_INFO_3 ends

USE_LOCAL_PARMNUM equ 1
USE_REMOTE_PARMNUM equ 2
USE_PASSWORD_PARMNUM equ 3
USE_ASGTYPE_PARMNUM equ 4
USE_USERNAME_PARMNUM equ 5
USE_DOMAINNAME_PARMNUM equ 6
USE_OK equ 0
USE_PAUSED equ 1
USE_SESSLOST equ 2
USE_DISCONN equ 2
USE_NETERR equ 3
USE_CONN equ 4
USE_RECONN equ 5
USE_WILDCARD equ -1 
USE_DISKDEV equ 0
USE_SPOOLDEV equ 1
USE_CHARDEV equ 2
USE_IPC equ 3
CREATE_NO_CONNECT equ 1

;============================[ LMUSEFLG.H]=======================
USE_NOFORCE equ 0
USE_FORCE equ 1
USE_LOTS_OF_FORCE equ 2

STRRET STRUCT
 uType      dd ?
 union                        
    pOleStr    dd ?
    pStr       dd ?
    uOffset    dd ?
    union 
	    cStr       db MAX_PATH dup(?)
	    cstr       db MAX_PATH dup(?)
    ends
 ends
STRRET ENDS

;;;; end lmserver


;;;; ras

;---------------------------------------[ RAS.H ]-----------------------------------------

RAS_MaxDeviceType equ 16
RAS_MaxPhoneNumber equ 128
RAS_MaxIpAddress equ 15
RAS_MaxIpxAddress equ 21
RAS_MaxEntryName equ 256
RAS_MaxDeviceName equ 128
RAS_MaxCallbackNumber equ RAS_MaxPhoneNumber
RAS_MaxAreaCode equ 10
RAS_MaxPadType equ 32
RAS_MaxX25Address equ 200
RAS_MaxFacilities equ 200
RAS_MaxUserData equ 200
RASCS_PAUSED equ 1000h
RASCS_DONE equ 2000h
RDEOPT_UsePrefixSuffix equ 1
RDEOPT_PausedStates equ 2
RDEOPT_IgnoreModemSpeaker equ 4
RDEOPT_SetModemSpeaker equ 8
RDEOPT_IgnoreSoftwareCompression equ 10h
RDEOPT_SetSoftwareCompression equ 20h
RDEOPT_DisableConnectedUI equ 40h
RDEOPT_DisableReconnectUI equ 80h
RDEOPT_DisableReconnect equ 100h
RDEOPT_NoUser equ 200h
RDEOPT_PauseOnScript equ 400h
RASDIALEVENT equ "RasDialEvent"
WM_RASDIALEVENT equ 0CCCDh
RASEO_UseCountryAndAreaCodes equ 1
RASEO_SpecificIpAddr equ 2
RASEO_SpecificNameServers equ 4
RASEO_IpHeaderCompression equ 8
RASEO_RemoteDefaultGateway equ 10h
RASEO_DisableLcpExtensions equ 20h
RASEO_TerminalBeforeDial equ 40h
RASEO_TerminalAfterDial equ 80h
RASEO_ModemLights equ 100h
RASEO_SwCompression equ 200h
RASEO_RequireEncryptedPw equ 400h
RASEO_RequireMsEncryptedPw equ 800h
RASEO_RequireDataEncryption equ 1000h
RASEO_NetworkLogon equ 2000h
RASEO_UseLogonCredentials equ 4000h
RASEO_PromoteAlternates equ 8000h
RASEO_SecureLocalFiles equ 10000h
RASNP_NetBEUI equ 1
RASNP_Ipx equ 2
RASNP_Ip equ 4
RASFP_Ppp equ 1
RASFP_Slip equ 2
RASFP_Ras equ 4
RASDT_Modem equ "modem"
RASDT_Isdn equ "isdn"
RASDT_X25 equ "x25"
RASCN_Connection equ 1
RASCN_Disconnection equ 2
RASCN_BandwidthAdded equ 4
RASCN_BandwidthRemoved equ 8
RASEDM_DialAll equ 1
RASEDM_DialAsNeeded equ 2
RASIDS_Disabled equ 0ffffffffh
RASIDS_UseGlobalValue equ 0
RASADFLG_PositionDlg equ 1
RASCM_UserName equ 1
RASCM_Password equ 2
RASCM_Domain equ 4
RASADP_DisableConnectionQuery equ 0
RASADP_LoginSessionDisable equ 1
RASADP_SavedAddressesLimit equ 2
RASADP_FailedConnectionTimeout equ 3
RASADP_ConnectionQueryTimeout equ 4

RASCS_OpenPort equ 0
RASCS_PortOpened equ 1
RASCS_ConnectDevice equ 2
RASCS_DeviceConnected equ 3
RASCS_AllDevicesConnected equ 4
RASCS_Authenticate equ 5
RASCS_AuthNotify equ 6
RASCS_AuthRetry equ 7
RASCS_AuthCallback equ 8
RASCS_AuthChangePassword equ 9
RASCS_AuthProject equ 10
RASCS_AuthLinkSpeed equ 11
RASCS_AuthAck equ 12
RASCS_ReAuthenticate equ 13
RASCS_Authenticated equ 14
RASCS_PrepareForCallback equ 15
RASCS_WaitForModemReset equ 16
RASCS_WaitForCallback equ 17
RASCS_Projected equ 18
RASCS_StartAuthentication equ 19
RASCS_CallbackComplete equ 20
RASCS_LogonNetwork equ 21
RASCS_SubEntryConnected equ 22
RASCS_SubEntryDisconnected equ 23
RASCS_Interactive equ RASCS_PAUSED
RASCS_RetryAuthentication equ RASCS_PAUSED+1
RASCS_CallbackSetByCaller equ RASCS_PAUSED+2
RASCS_PasswordExpired equ RASCS_PAUSED+3
RASCS_Connected equ RASCS_DONE
RASCS_Disconnected equ RASCS_DONE+1
RASP_Amb equ 10000h
RASP_PppNbf equ 803Fh
RASP_PppIpx equ 802Bh
RASP_PppIp equ 8021h
RASP_PppLcp equ 0C021h
RASP_Slip equ 20000h

RASCONNA STRUCT
    dwSize dd ?
    hrasconn dd ?
    szEntryName db RAS_MaxEntryName + 1 dup(?)
    szDeviceType db RAS_MaxDeviceType + 1 dup(?)
    szDeviceName db RAS_MaxDeviceName + 1 dup(?)
RASCONNA ENDS 

RASCONN EQU <RASCONNA>

RASCONNSTATUSA STRUCT
    dwSize dd ?
    rasconnstate dd ?
    dwError dd ?
    szDeviceType db RAS_MaxDeviceType + 1 dup(?)
    szDeviceName db RAS_MaxDeviceName + 1 dup(?)
RASCONNSTATUSA ENDS

RASCONNSTATUS EQU <RASCONNSTATUSA>

RASDIALPARAMSA STRUCT
    dwSize dd ?
    szEntryName db RAS_MaxEntryName + 1 dup(?)
    szPhoneNumber db RAS_MaxPhoneNumber + 1 dup(?)
    szCallbackNumber db RAS_MaxCallbackNumber + 1 dup(?)
    szUserName db UNLEN + 1 dup(?)
    szPassword db PWLEN + 1 dup(?)
    szDomain db DNLEN + 1 dup(?)
RASDIALPARAMSA ENDS

RASDIALPARAMS EQU <RASDIALPARAMSA>

RASDIALEXTENSIONS STRUCT
    dwSize dd ?
    dwfOptions dd ?
    hwndParent dd ?
    reserved dd ?
RASDIALEXTENSIONS ENDS

RASENTRYNAMEA STRUCT
    dwSize dd ?
    szEntryName db RAS_MaxEntryName + 1 dup(?)
RASENTRYNAMEA ENDS

RASENTRYNAME EQU <RASENTRYNAMEA>

RASAMBA STRUCT
    dwSize dd ?
    dwError dd ?
    szNetBiosError db NETBIOS_NAME_LEN + 1 dup(?)
    bLana db ?
RASAMBA ENDS

RASAMB EQU <RASAMBA>

RASPPPNBFA STRUCT
    dwSize dd ?
    dwError dd ?
    dwNetBiosError dd ?
    szNetBiosError db NETBIOS_NAME_LEN + 1 dup(?)
    szWorkstationName db NETBIOS_NAME_LEN + 1 dup(?)
    bLana db ?
RASPPPNBFA ENDS

RASPPPNBF EQU <RASPPPNBFA>

RASPPPIPXA STRUCT
    dwSize dd ?
    dwError dd ?
    szIpxAddress db RAS_MaxIpxAddress + 1 dup (?)
RASPPPIPXA ENDS

RASPPPIPX EQU <RASPPPIPXA>

RASPPPIPA STRUCT
    dwSize dd ?
    dwError dd ?
    szIpAddress db RAS_MaxIpAddress + 1 dup(?)
    szServerIpAddress db RAS_MaxIpAddress + 1 dup(?)
RASPPPIPA ENDS

RASPPPIP EQU <RASPPPIPA>

RASPPPLCP STRUCT
    dwSize dd ?
    fBundled BOOL ?
RASPPPLCP ENDS

RASSLIPA STRUCT
    dwSize dd ?
    dwError dd ?
    szIpAddress db RAS_MaxIpAddress + 1 dup(?)
RASSLIPA ENDS

RASSLIP EQU <RASSLIPA>

RASDEVINFOA STRUCT
    dwSize dd ?
    szDeviceType db RAS_MaxDeviceType + 1 dup(?)
    szDeviceName db RAS_MaxDeviceName + 1 dup(?)
RASDEVINFOA ENDS

RASDEVINFO EQU <RASDEVINFOA>

RASCTRYINFO STRUCT
    dwSize dd ?
    dwCountryID dd ?
    dwNextCountryID dd ?
    dwCountryCode dd ?
    dwCountryNameOffset dd ?
RASCTRYINFO ENDS

RASIPADDR STRUCT
    _a db ?
    _b db ?
    _c db ?
    _d db ?
RASIPADDR ENDS

RASENTRYA STRUCT
    dwSize dd ?
    dwfOptions dd ?
    dwCountryID dd ?
    dwCountryCode dd ?
    szAreaCode db RAS_MaxAreaCode + 1 dup(?)
    szLocalPhoneNumber db RAS_MaxPhoneNumber + 1 dup(?)
    dwAlternateOffset dd ?
    ipaddr RASIPADDR <>
    ipaddrDns RASIPADDR <>
    ipaddrDnsAlt RASIPADDR <>
    ipaddrWins RASIPADDR <>
    ipaddrWinsAlt RASIPADDR <>
    dwFrameSize dd ?
    dwfNetProtocols dd ?
    dwFramingProtocol dd ?
    szScript db MAX_PATH dup(?)
    szAutodialDll db MAX_PATH dup(?)
    szAutodialFunc db MAX_PATH dup(?)
    szDeviceType db RAS_MaxDeviceType + 1 dup(?)
    szDeviceName db RAS_MaxDeviceName + 1 dup(?)
    szX25PadType db RAS_MaxPadType + 1 dup(?)
    szX25Address db RAS_MaxX25Address + 1 dup(?)
    szX25Facilities db RAS_MaxFacilities + 1 dup(?)
    szX25UserData db RAS_MaxUserData + 1 dup(?)
    dwChannels dd ?
    dwReserved1 dd ?
    dwReserved2 dd ?
RASENTRYA ENDS

RASENTRY EQU <RASENTRYA>

;-----------------------------------------[ rasdlg.h ]--------------------------------
RASPBDEVENT_AddEntry equ 1
RASPBDEVENT_EditEntry equ 2
RASPBDEVENT_RemoveEntry equ 3
RASPBDEVENT_DialEntry equ 4
RASPBDEVENT_EditGlobals equ 5
RASPBDEVENT_NoUser equ 6
RASPBDEVENT_NoUserEdit equ 7
RASPBDFLAG_PositionDlg equ 1
RASPBDFLAG_ForceCloseOnDial equ 2
RASPBDFLAG_NoUser equ 10h
RASPBDFLAG_UpdateDefaults equ 80000000h
RASMDPAGE_Status equ 0
RASMDPAGE_Summary equ 1
RASMDPAGE_Preferences equ 2
RASMDFLAG_PositionDlg equ 1
RASMDFLAG_UpdateDefaults equ 80000000h
RASEDFLAG_PositionDlg equ 1
RASEDFLAG_NewEntry equ 2
RASEDFLAG_CloneEntry equ 4
RASEDFLAG_NoRename equ 8
RASDDFLAG_PositionDlg equ 1
RASDDFLAG_LinkFailure equ 80000000h

RASPBDLGA STRUCT
    dwSize dd ?
    hwndOwner dd ?
    dwFlags dd ?
    xDlg dd ?
    yDlg dd ?
    dwCallbackId dd ?
    pCallback dd ?
    dwError dd ?
    reserved dd ?
    reserved2 dd ?
RASPBDLGA ENDS

RASPBDLG EQU <RASPBDLGA>

RASENTRYDLGA STRUCT
    dwSize dd ?
    hwndOwner dd ?
    dwFlags dd ?
    xDlg dd ?
    yDlg dd ?
    szEntry db RAS_MaxEntryName + 1 dup(?)
    dwError dd ?
    reserved dd ?
    reserved2 dd ?
RASENTRYDLGA ENDS

RASENTRYDLG EQU <RASENTRYDLGA>

RASDIALDLG STRUCT
    dwSize dd ?
    hwndOwner dd ?
    dwFlags dd ?
    xDlg dd ?
    yDlg dd ?
    dwSubEntry dd ?
    dwError dd ?
    reserved dd ?
    reserved2 dd ?
RASDIALDLG ENDS

RASMONITORDLG STRUCT
    dwSize dd ?
    hwndOwner dd ?
    dwFlags dd ?
    dwStartPage dd ?
    xDlg dd ?
    yDlg dd ?
    dwError dd ?
    reserved dd ?
    reserved2 dd ?
RASMONITORDLG ENDS

RASNOUSERA STRUCT
    dwSize dd ?
    dwFlags dd ?
    dwTimeoutMs dd ?
    szUserName db UNLEN + 1 dup(?)
    szPassword db PWLEN + 1 dup(?)
    szDomain db DNLEN + 1 dup(?)
RASNOUSERA ENDS

RASNOUSER EQU <RASNOUSERA>

;----------------------------------------[ raserror.h ]--------------------------------
RASBASE equ 600
SUCCESS equ 0
PENDING equ (RASBASE+0)
ERROR_INVALID_PORT_HANDLE equ (RASBASE+1)
ERROR_PORT_ALREADY_OPEN equ (RASBASE+2)
ERROR_BUFFER_TOO_SMALL equ (RASBASE+3)
ERROR_WRONG_INFO_SPECIFIED equ (RASBASE+4)
ERROR_CANNOT_SET_PORT_INFO equ (RASBASE+5)
ERROR_PORT_NOT_CONNECTED equ (RASBASE+6)
ERROR_EVENT_INVALID equ (RASBASE+7)
ERROR_DEVICE_DOES_NOT_EXIST equ (RASBASE+8)
ERROR_DEVICETYPE_DOES_NOT_EXIST equ (RASBASE+9)
ERROR_BUFFER_INVALID equ (RASBASE+10)
ERROR_ROUTE_NOT_AVAILABLE equ (RASBASE+11)
ERROR_ROUTE_NOT_ALLOCATED equ (RASBASE+12)
ERROR_INVALID_COMPRESSION_SPECIFIED equ (RASBASE+13)
ERROR_OUT_OF_BUFFERS equ (RASBASE+14)
ERROR_PORT_NOT_FOUND equ (RASBASE+15)
ERROR_ASYNC_REQUEST_PENDING equ (RASBASE+16)
ERROR_ALREADY_DISCONNECTING equ (RASBASE+17)
ERROR_PORT_NOT_OPEN equ (RASBASE+18)
ERROR_PORT_DISCONNECTED equ (RASBASE+19)
ERROR_NO_ENDPOINTS equ (RASBASE+20)
ERROR_CANNOT_OPEN_PHONEBOOK equ (RASBASE+21)
ERROR_CANNOT_LOAD_PHONEBOOK equ (RASBASE+22)
ERROR_CANNOT_FIND_PHONEBOOK_ENTRY equ (RASBASE+23)
ERROR_CANNOT_WRITE_PHONEBOOK equ (RASBASE+24)
ERROR_CORRUPT_PHONEBOOK equ (RASBASE+25)
ERROR_CANNOT_LOAD_STRING equ (RASBASE+26)
ERROR_KEY_NOT_FOUND equ (RASBASE+27)
ERROR_DISCONNECTION equ (RASBASE+28)
ERROR_REMOTE_DISCONNECTION equ (RASBASE+29)
ERROR_HARDWARE_FAILURE equ (RASBASE+30)
ERROR_USER_DISCONNECTION equ (RASBASE+31)
ERROR_INVALID_SIZE equ (RASBASE+32)
ERROR_PORT_NOT_AVAILABLE equ (RASBASE+33)
ERROR_CANNOT_PROJECT_CLIENT equ (RASBASE+34)
ERROR_UNKNOWN equ (RASBASE+35)
ERROR_WRONG_DEVICE_ATTACHED equ (RASBASE+36)
ERROR_BAD_STRING equ (RASBASE+37)
ERROR_REQUEST_TIMEOUT equ (RASBASE+38)
ERROR_CANNOT_GET_LANA equ (RASBASE+39)
ERROR_NETBIOS_ERROR equ (RASBASE+40)
ERROR_SERVER_OUT_OF_RESOURCES equ (RASBASE+41)
ERROR_NAME_EXISTS_ON_NET equ (RASBASE+42)
ERROR_SERVER_GENERAL_NET_FAILURE equ (RASBASE+43)
WARNING_MSG_ALIAS_NOT_ADDED equ (RASBASE+44)
ERROR_AUTH_INTERNAL equ (RASBASE+45)
ERROR_RESTRICTED_LOGON_HOURS equ (RASBASE+46)
ERROR_ACCT_DISABLED equ (RASBASE+47)
ERROR_PASSWD_EXPIRED equ (RASBASE+48)
ERROR_NO_DIALIN_PERMISSION equ (RASBASE+49)
ERROR_SERVER_NOT_RESPONDING equ (RASBASE+50)
ERROR_FROM_DEVICE equ (RASBASE+51)
ERROR_UNRECOGNIZED_RESPONSE equ (RASBASE+52)
ERROR_MACRO_NOT_FOUND equ (RASBASE+53)
ERROR_MACRO_NOT_DEFINED equ (RASBASE+54)
ERROR_MESSAGE_MACRO_NOT_FOUND equ (RASBASE+55)
ERROR_DEFAULTOFF_MACRO_NOT_FOUND equ (RASBASE+56)
ERROR_FILE_COULD_NOT_BE_OPENED equ (RASBASE+57)
ERROR_DEVICENAME_TOO_LONG equ (RASBASE+58)
ERROR_DEVICENAME_NOT_FOUND equ (RASBASE+59)
ERROR_NO_RESPONSES equ (RASBASE+60)
ERROR_NO_COMMAND_FOUND equ (RASBASE+61)
ERROR_WRONG_KEY_SPECIFIED equ (RASBASE+62)
ERROR_UNKNOWN_DEVICE_TYPE equ (RASBASE+63)
ERROR_ALLOCATING_MEMORY equ (RASBASE+64)
ERROR_PORT_NOT_CONFIGURED equ (RASBASE+65)
ERROR_DEVICE_NOT_READY equ (RASBASE+66)
ERROR_READING_INI_FILE equ (RASBASE+67)
ERROR_NO_CONNECTION equ (RASBASE+68)
ERROR_BAD_USAGE_IN_INI_FILE equ (RASBASE+69)
ERROR_READING_SECTIONNAME equ (RASBASE+70)
ERROR_READING_DEVICETYPE equ (RASBASE+71)
ERROR_READING_DEVICENAME equ (RASBASE+72)
ERROR_READING_USAGE equ (RASBASE+73)
ERROR_READING_MAXCONNECTBPS equ (RASBASE+74)
ERROR_READING_MAXCARRIERBPS equ (RASBASE+75)
ERROR_LINE_BUSY equ (RASBASE+76)
ERROR_VOICE_ANSWER equ (RASBASE+77)
ERROR_NO_ANSWER equ (RASBASE+78)
ERROR_NO_CARRIER equ (RASBASE+79)
ERROR_NO_DIALTONE equ (RASBASE+80)
ERROR_IN_COMMAND equ (RASBASE+81)
ERROR_WRITING_SECTIONNAME equ (RASBASE+82)
ERROR_WRITING_DEVICETYPE equ (RASBASE+83)
ERROR_WRITING_DEVICENAME equ (RASBASE+84)
ERROR_WRITING_MAXCONNECTBPS equ (RASBASE+85)
ERROR_WRITING_MAXCARRIERBPS equ (RASBASE+86)
ERROR_WRITING_USAGE equ (RASBASE+87)
ERROR_WRITING_DEFAULTOFF equ (RASBASE+88)
ERROR_READING_DEFAULTOFF equ (RASBASE+89)
ERROR_EMPTY_INI_FILE equ (RASBASE+90)
ERROR_AUTHENTICATION_FAILURE equ (RASBASE+91)
ERROR_PORT_OR_DEVICE equ (RASBASE+92)
ERROR_NOT_BINARY_MACRO equ (RASBASE+93)
ERROR_DCB_NOT_FOUND equ (RASBASE+94)
ERROR_STATE_MACHINES_NOT_STARTED equ (RASBASE+95)
ERROR_STATE_MACHINES_ALREADY_STARTED equ (RASBASE+96)
ERROR_PARTIAL_RESPONSE_LOOPING equ (RASBASE+97)
ERROR_UNKNOWN_RESPONSE_KEY equ (RASBASE+98)
ERROR_RECV_BUF_FULL equ (RASBASE+99)
ERROR_CMD_TOO_LONG equ (RASBASE+100)
ERROR_UNSUPPORTED_BPS equ (RASBASE+101)
ERROR_UNEXPECTED_RESPONSE equ (RASBASE+102)
ERROR_INTERACTIVE_MODE equ (RASBASE+103)
ERROR_BAD_CALLBACK_NUMBER equ (RASBASE+104)
ERROR_INVALID_AUTH_STATE equ (RASBASE+105)
ERROR_WRITING_INITBPS equ (RASBASE+106)
ERROR_X25_DIAGNOSTIC equ (RASBASE+107)
ERROR_ACCT_EXPIRED equ (RASBASE+108)
ERROR_CHANGING_PASSWORD equ (RASBASE+109)
ERROR_OVERRUN equ (RASBASE+110)
ERROR_RASMAN_CANNOT_INITIALIZE	 equ (RASBASE+111)
ERROR_BIPLEX_PORT_NOT_AVAILABLE equ (RASBASE+112)
ERROR_NO_ACTIVE_ISDN_LINES equ (RASBASE+113)
ERROR_NO_ISDN_CHANNELS_AVAILABLE equ (RASBASE+114)
ERROR_TOO_MANY_LINE_ERRORS equ (RASBASE+115)
ERROR_IP_CONFIGURATION equ (RASBASE+116)
ERROR_NO_IP_ADDRESSES equ (RASBASE+117)
ERROR_PPP_TIMEOUT equ (RASBASE+118)
ERROR_PPP_REMOTE_TERMINATED equ (RASBASE+119)
ERROR_PPP_NO_PROTOCOLS_CONFIGURED equ (RASBASE+120)
ERROR_PPP_NO_RESPONSE equ (RASBASE+121)
ERROR_PPP_INVALID_PACKET equ (RASBASE+122)
ERROR_PHONE_NUMBER_TOO_LONG equ (RASBASE+123)
ERROR_IPXCP_NO_DIALOUT_CONFIGURED equ (RASBASE+124)
ERROR_IPXCP_NO_DIALIN_CONFIGURED equ (RASBASE+125)
ERROR_IPXCP_DIALOUT_ALREADY_ACTIVE equ (RASBASE+126)
ERROR_ACCESSING_TCPCFGDLL equ (RASBASE+127)
ERROR_NO_IP_RAS_ADAPTER equ (RASBASE+128)
ERROR_SLIP_REQUIRES_IP equ (RASBASE+129)
ERROR_PROJECTION_NOT_COMPLETE equ (RASBASE+130)
ERROR_PROTOCOL_NOT_CONFIGURED equ (RASBASE+131)
ERROR_PPP_NOT_CONVERGING equ (RASBASE+132)
ERROR_PPP_CP_REJECTED equ (RASBASE+133)
ERROR_PPP_LCP_TERMINATED equ (RASBASE+134)
ERROR_PPP_REQUIRED_ADDRESS_REJECTED equ (RASBASE+135)
ERROR_PPP_NCP_TERMINATED equ (RASBASE+136)
ERROR_PPP_LOOPBACK_DETECTED equ (RASBASE+137)
ERROR_PPP_NO_ADDRESS_ASSIGNED equ (RASBASE+138)
ERROR_CANNOT_USE_LOGON_CREDENTIALS equ (RASBASE+139)
ERROR_TAPI_CONFIGURATION equ (RASBASE+140)
ERROR_NO_LOCAL_ENCRYPTION equ (RASBASE+141)
ERROR_NO_REMOTE_ENCRYPTION equ (RASBASE+142)
ERROR_REMOTE_REQUIRES_ENCRYPTION equ (RASBASE+143)
ERROR_IPXCP_NET_NUMBER_CONFLICT equ (RASBASE+144)
ERROR_INVALID_SMM equ (RASBASE+145)
ERROR_SMM_UNINITIALIZED equ (RASBASE+146)
ERROR_NO_MAC_FOR_PORT equ (RASBASE+147)
ERROR_SMM_TIMEOUT equ (RASBASE+148)
ERROR_BAD_PHONE_NUMBER equ (RASBASE+149)
ERROR_WRONG_MODULE equ (RASBASE+150)
ERROR_INVALID_CALLBACK_NUMBER equ (RASBASE+151)
ERROR_SCRIPT_SYNTAX equ (RASBASE+152)
RASBASEEND equ (RASBASE+152)

;------------------------------------[ rassapi.h ]-------------------------------------

RASSAPI_MAX_PHONENUMBER_SIZE equ 128
RASSAPI_MAX_MEDIA_NAME	 equ 16
RASSAPI_MAX_PORT_NAME	 equ 16
RASSAPI_MAX_DEVICE_NAME equ 128
RASSAPI_MAX_DEVICETYPE_NAME equ 16
RASSAPI_MAX_PARAM_KEY_SIZE equ 32
RASPRIV_NoCallback equ 1
RASPRIV_AdminSetCallback equ 2
RASPRIV_CallerSetCallback equ 4
RASPRIV_DialinPrivilege equ 8
RASPRIV_CallbackType equ (RASPRIV_AdminSetCallback+RASPRIV_CallerSetCallback+RASPRIV_NoCallback)
RAS_MODEM_OPERATIONAL	 equ 1
RAS_MODEM_NOT_RESPONDING equ 2
RAS_MODEM_HARDWARE_FAILURE equ 3
RAS_MODEM_INCORRECT_RESPONSE equ 4
RAS_MODEM_UNKNOWN equ 	        5
RAS_PORT_NON_OPERATIONAL equ 1
RAS_PORT_DISCONNECTED	 equ 2
RAS_PORT_CALLING_BACK equ 3
RAS_PORT_LISTENING	 equ 4
RAS_PORT_AUTHENTICATING equ 5
RAS_PORT_AUTHENTICATED	 equ 6
RAS_PORT_INITIALIZING	 equ 7
MEDIA_UNKNOWN equ 0
MEDIA_SERIAL equ 1
MEDIA_RAS10_SERIAL equ 2
MEDIA_X25 equ 3
MEDIA_ISDN equ 4
USER_AUTHENTICATED equ 1
MESSENGER_PRESENT equ 2
PPP_CLIENT equ 4
GATEWAY_ACTIVE equ 8
REMOTE_LISTEN equ 10h
PORT_MULTILINKED equ 20h
RAS_IPADDRESSLEN equ 15
RAS_IPXADDRESSLEN equ 22
RAS_ATADDRESSLEN equ 32
RASDOWNLEVEL equ 10
RASADMIN_35 equ 35
RASADMIN_CURRENT equ 40
ParamNumber equ 0
ParamString	equ 1

RAS_PARAMS_VALUE UNION DWORD
	Number dd ?
	struct String
		iLength dd ?
		Data dd ?
	ends
RAS_PARAMS_VALUE ENDS

RAS_PARAMETERS STRUCT
    P_Key	db RASSAPI_MAX_PARAM_KEY_SIZE dup(?)
    P_Type dd ?
    P_Attributes db ?
    P_Value RAS_PARAMS_VALUE <>
RAS_PARAMETERS ENDS

RAS_USER_0 STRUCT
    bfPrivilege db ?
    szPhoneNumber dw RASSAPI_MAX_PHONENUMBER_SIZE + 1 dup(?)
RAS_USER_0 ENDS

RAS_PORT_0 STRUCT
    wszPortName dw RASSAPI_MAX_PORT_NAME dup(?)
    wszDeviceType dw RASSAPI_MAX_DEVICETYPE_NAME dup(?)
    wszDeviceName dw RASSAPI_MAX_DEVICE_NAME dup(?)
    wszMediaName dw RASSAPI_MAX_MEDIA_NAME dup(?)
    reserved dd ?
    Flags dd ?
    wszUserName dw UNLEN + 1 dup(?)
    wszComputer dw NETBIOS_NAME_LEN dup(?)
    dwStartSessionTime dd ?
    wszLogonDomain dw DNLEN + 1 dup(?)
    fAdvancedServer BOOL ?
RAS_PORT_0 ENDS

RAS_PPP_NBFCP_RESULT STRUCT
    dwError dd ?
    dwNetBiosError dd ?
    szName db NETBIOS_NAME_LEN + 1 dup(?)
    wszWksta dw NETBIOS_NAME_LEN + 1 dup(?)
RAS_PPP_NBFCP_RESULT ENDS

RAS_PPP_IPCP_RESULT STRUCT
    dwError dd ?
    wszAddress dw RAS_IPADDRESSLEN + 1 dup(?)
RAS_PPP_IPCP_RESULT ENDS

RAS_PPP_IPXCP_RESULT STRUCT
    dwError dd ?
    wszAddress dw RAS_IPXADDRESSLEN + 1 dup(?)
RAS_PPP_IPXCP_RESULT ENDS

RAS_PPP_ATCP_RESULT STRUCT
    dwError dd ?
    wszAddress dw RAS_ATADDRESSLEN + 1 dup(?)
RAS_PPP_ATCP_RESULT ENDS

RAS_PPP_PROJECTION_RESULT STRUCT
    nbf RAS_PPP_NBFCP_RESULT <>
    ip RAS_PPP_IPCP_RESULT <>
    ipx RAS_PPP_IPXCP_RESULT <>
    at RAS_PPP_ATCP_RESULT <>
RAS_PPP_PROJECTION_RESULT ENDS

RAS_PORT_1 STRUCT
    rasport0 RAS_PORT_0 <>
    LineCondition dd ?
    HardwareCondition dd ?
    LineSpeed dd ?
    NumStatistics dw ?
    NumMediaParms dw ?
    SizeMediaParms dd ?
    ProjResult RAS_PPP_PROJECTION_RESULT <>
RAS_PORT_1 ENDS

RAS_PORT_STATISTICS STRUCT
    dwBytesXmited dd ?
    dwBytesRcved dd ?
    dwFramesXmited dd ?
    dwFramesRcved dd ?
    dwCrcErr dd ?
    dwTimeoutErr dd ?
    dwAlignmentErr dd ?
    dwHardwareOverrunErr dd ?
    dwFramingErr dd ?
    dwBufferOverrunErr dd ?
    dwBytesXmitedUncompressed dd ?
    dwBytesRcvedUncompressed dd ?
    dwBytesXmitedCompressed dd ?
    dwBytesRcvedCompressed dd ?
    dwPortBytesXmited dd ?
    dwPortBytesRcved dd ?
    dwPortFramesXmited dd ?
    dwPortFramesRcved dd ?
    dwPortCrcErr dd ?
    dwPortTimeoutErr dd ?
    dwPortAlignmentErr dd ?
    dwPortHardwareOverrunErr dd ?
    dwPortFramingErr dd ?
    dwPortBufferOverrunErr dd ?
    dwPortBytesXmitedUncompressed dd ?
    dwPortBytesRcvedUncompressed dd ?
    dwPortBytesXmitedCompressed dd ?
    dwPortBytesRcvedCompressed dd ?
RAS_PORT_STATISTICS ENDS

RAS_SERVER_0 STRUCT
    TotalPorts dw ?
    PortsInUse dw ?
    RasVersion dd ?
RAS_SERVER_0 ENDS

;;;; end ras


;;;; oledlg

IDC_OLEUIHELP                    equ 99
IDC_IO_CREATENEW                 equ 2100
IDC_IO_CREATEFROMFILE            equ 2101
IDC_IO_LINKFILE                  equ 2102
IDC_IO_OBJECTTYPELIST            equ 2103
IDC_IO_DISPLAYASICON             equ 2104
IDC_IO_CHANGEICON                equ 2105
IDC_IO_FILE                      equ 2106
IDC_IO_FILEDISPLAY               equ 2107
IDC_IO_RESULTIMAGE               equ 2108
IDC_IO_RESULTTEXT                equ 2109
IDC_IO_ICONDISPLAY               equ 2110
IDC_IO_OBJECTTYPETEXT            equ 2111
IDC_IO_FILETEXT                  equ 2112
IDC_IO_FILETYPE                  equ 2113
IDC_IO_INSERTCONTROL             equ 2114
IDC_IO_ADDCONTROL                equ 2115
IDC_IO_CONTROLTYPELIST           equ 2116
IDC_PS_PASTE                     equ 500
IDC_PS_PASTELINK                 equ 501
IDC_PS_SOURCETEXT                equ 502
IDC_PS_PASTELIST                 equ 503
IDC_PS_PASTELINKLIST             equ 504
IDC_PS_DISPLAYLIST               equ 505
IDC_PS_DISPLAYASICON             equ 506
IDC_PS_ICONDISPLAY               equ 507
IDC_PS_CHANGEICON                equ 508
IDC_PS_RESULTIMAGE               equ 509
IDC_PS_RESULTTEXT                equ 510
IDC_CI_GROUP                     equ 120
IDC_CI_CURRENT                   equ 121
IDC_CI_CURRENTICON               equ 122
IDC_CI_DEFAULT                   equ 123
IDC_CI_DEFAULTICON               equ 124
IDC_CI_FROMFILE                  equ 125
IDC_CI_FROMFILEEDIT              equ 126
IDC_CI_ICONLIST                  equ 127
IDC_CI_LABEL                     equ 128
IDC_CI_LABELEDIT                 equ 129
IDC_CI_BROWSE                    equ 130
IDC_CI_ICONDISPLAY               equ 131
IDC_CV_OBJECTTYPE                equ 150
IDC_CV_DISPLAYASICON             equ 152
IDC_CV_CHANGEICON                equ 153
IDC_CV_ACTIVATELIST              equ 154
IDC_CV_CONVERTTO                 equ 155
IDC_CV_ACTIVATEAS                equ 156
IDC_CV_RESULTTEXT                equ 157
IDC_CV_CONVERTLIST               equ 158
IDC_CV_ICONDISPLAY               equ 165
IDC_EL_CHANGESOURCE              equ 201
IDC_EL_AUTOMATIC                 equ 202
IDC_EL_CANCELLINK                equ 209
IDC_EL_UPDATENOW                 equ 210
IDC_EL_OPENSOURCE                equ 211
IDC_EL_MANUAL                    equ 212
IDC_EL_LINKSOURCE                equ 216
IDC_EL_LINKTYPE                  equ 217
IDC_EL_LINKSLISTBOX              equ 206
IDC_EL_COL1                      equ 220
IDC_EL_COL2                      equ 221
IDC_EL_COL3                      equ 222
IDC_BZ_RETRY                     equ 600
IDC_BZ_ICON                      equ 601
IDC_BZ_MESSAGE1                  equ 602
IDC_BZ_SWITCHTO                  equ 604
IDC_UL_METER                     equ 1029
IDC_UL_STOP                      equ 1030
IDC_UL_PERCENT                   equ 1031
IDC_UL_PROGRESS                  equ 1032
IDC_PU_LINKS                     equ 900
IDC_PU_TEXT                      equ 901
IDC_PU_CONVERT                   equ 902
IDC_PU_ICON                      equ 908
IDC_GP_OBJECTNAME                equ 1009
IDC_GP_OBJECTTYPE                equ 1010
IDC_GP_OBJECTSIZE                equ 1011
IDC_GP_CONVERT                   equ 1013
IDC_GP_OBJECTICON                equ 1014
IDC_GP_OBJECTLOCATION            equ 1022
IDC_VP_PERCENT                   equ 1000
IDC_VP_CHANGEICON                equ 1001
IDC_VP_EDITABLE                  equ 1002
IDC_VP_ASICON                    equ 1003
IDC_VP_RELATIVE                  equ 1005
IDC_VP_SPIN                      equ 1006
IDC_VP_SCALETXT                  equ 1034
IDC_VP_ICONDISPLAY               equ 1021
IDC_VP_RESULTIMAGE               equ 1033
IDC_LP_OPENSOURCE                equ 1006
IDC_LP_UPDATENOW                 equ 1007
IDC_LP_BREAKLINK                 equ 1008
IDC_LP_LINKSOURCE                equ 1012
IDC_LP_CHANGESOURCE              equ 1015
IDC_LP_AUTOMATIC                 equ 1016
IDC_LP_MANUAL                    equ 1017
IDC_LP_DATE                      equ 1018
IDC_LP_TIME                      equ 1019
IDD_INSERTOBJECT                 equ 1000
IDD_CHANGEICON                   equ 1001
IDD_CONVERT                      equ 1002
IDD_PASTESPECIAL                 equ 1003
IDD_EDITLINKS                    equ 1004
IDD_BUSY                         equ 1006
IDD_UPDATELINKS                  equ 1007
IDD_CHANGESOURCE                 equ 1009
IDD_INSERTFILEBROWSE             equ 1010
IDD_CHANGEICONBROWSE             equ 1011
IDD_CONVERTONLY                  equ 1012
IDD_CHANGESOURCE4                equ 1013
IDD_GNRLPROPS                    equ 1100
IDD_VIEWPROPS                    equ 1101
IDD_LINKPROPS                    equ 1102
IDD_CONVERT4                     equ 1103
IDD_CONVERTONLY4                 equ 1104
IDD_EDITLINKS4                   equ 1105
IDD_GNRLPROPS4                   equ 1106
IDD_LINKPROPS4                   equ 1107
IDD_PASTESPECIAL4                equ 1108
IDD_CANNOTUPDATELINK             equ 1008
IDD_LINKSOURCEUNAVAILABLE        equ 1020
IDD_SERVERNOTFOUND               equ 1023
IDD_OUTOFMEMORY                  equ 1024
IDD_SERVERNOTREGW                equ 1021
IDD_LINKTYPECHANGEDW             equ 1022
IDD_SERVERNOTREGA                equ 1025
IDD_LINKTYPECHANGEDA             equ 1026
ID_BROWSE_CHANGEICON             equ 1
ID_BROWSE_INSERTFILE             equ 2
ID_BROWSE_ADDCONTROL             equ 3
ID_BROWSE_CHANGESOURCE           equ 4
OLEUI_FALSE                      equ 0
OLEUI_SUCCESS                    equ 1
OLEUI_OK                         equ 1
OLEUI_CANCEL                     equ 2
OLEUI_ERR_STANDARDMIN            equ 100
OLEUI_ERR_OLEMEMALLOC            equ 100
OLEUI_ERR_STRUCTURENULL          equ 101
OLEUI_ERR_STRUCTUREINVALID       equ 102
OLEUI_ERR_CBSTRUCTINCORRECT      equ 103
OLEUI_ERR_HWNDOWNERINVALID       equ 104
OLEUI_ERR_LPSZCAPTIONINVALID     equ 105
OLEUI_ERR_LPFNHOOKINVALID        equ 106
OLEUI_ERR_HINSTANCEINVALID       equ 107
OLEUI_ERR_LPSZTEMPLATEINVALID    equ 108
OLEUI_ERR_HRESOURCEINVALID       equ 109
OLEUI_ERR_FINDTEMPLATEFAILURE    equ 110
OLEUI_ERR_LOADTEMPLATEFAILURE    equ 111
OLEUI_ERR_DIALOGFAILURE          equ 112
OLEUI_ERR_LOCALMEMALLOC          equ 113
OLEUI_ERR_GLOBALMEMALLOC         equ 114
OLEUI_ERR_LOADSTRING             equ 115
OLEUI_ERR_STANDARDMAX            equ 116
IOF_SHOWHELP                     equ 00000001h
IOF_SELECTCREATENEW              equ 00000002h
IOF_SELECTCREATEFROMFILE         equ 00000004h
IOF_CHECKLINK                    equ 00000008h
IOF_CHECKDISPLAYASICON           equ 00000010h
IOF_CREATENEWOBJECT              equ 00000020h
IOF_CREATEFILEOBJECT             equ 00000040h
IOF_CREATELINKOBJECT             equ 00000080h
IOF_DISABLELINK                  equ 00000100h
IOF_VERIFYSERVERSEXIST           equ 00000200h
IOF_DISABLEDISPLAYASICON         equ 00000400h
IOF_HIDECHANGEICON               equ 00000800h
IOF_SHOWINSERTCONTROL            equ 00001000h
IOF_SELECTCREATECONTROL          equ 00002000h
OLEUI_IOERR_LPSZFILEINVALID      equ (OLEUI_ERR_STANDARDMAX+0)
OLEUI_IOERR_LPSZLABELINVALID     equ (OLEUI_ERR_STANDARDMAX+1)
OLEUI_IOERR_HICONINVALID         equ (OLEUI_ERR_STANDARDMAX+2)
OLEUI_IOERR_LPFORMATETCINVALID   equ (OLEUI_ERR_STANDARDMAX+3)
OLEUI_IOERR_PPVOBJINVALID        equ (OLEUI_ERR_STANDARDMAX+4)
OLEUI_IOERR_LPIOLECLIENTSITEINVALID equ (OLEUI_ERR_STANDARDMAX+5)
OLEUI_IOERR_LPISTORAGEINVALID    equ (OLEUI_ERR_STANDARDMAX+6)
OLEUI_IOERR_SCODEHASERROR        equ (OLEUI_ERR_STANDARDMAX+7)
OLEUI_IOERR_LPCLSIDEXCLUDEINVALID equ (OLEUI_ERR_STANDARDMAX+8)
OLEUI_IOERR_CCHFILEINVALID       equ (OLEUI_ERR_STANDARDMAX+9)
PS_MAXLINKTYPES                  equ 8
PSF_SHOWHELP                     equ 00000001h
PSF_SELECTPASTE                  equ 00000002h
PSF_SELECTPASTELINK              equ 00000004h
PSF_CHECKDISPLAYASICON           equ 00000008h
PSF_DISABLEDISPLAYASICON         equ 00000010h
PSF_HIDECHANGEICON               equ 00000020h
PSF_STAYONCLIPBOARDCHANGE        equ 00000040h
PSF_NOREFRESHDATAOBJECT          equ 00000080h
OLEUI_IOERR_SRCDATAOBJECTINVALID equ (OLEUI_ERR_STANDARDMAX+0)
OLEUI_IOERR_ARRPASTEENTRIESINVALID equ (OLEUI_ERR_STANDARDMAX+1)
OLEUI_IOERR_ARRLINKTYPESINVALID  equ (OLEUI_ERR_STANDARDMAX+2)
OLEUI_PSERR_CLIPBOARDCHANGED     equ (OLEUI_ERR_STANDARDMAX+3)
OLEUI_PSERR_GETCLIPBOARDFAILED   equ (OLEUI_ERR_STANDARDMAX+4)
OLEUI_ELERR_LINKCNTRNULL         equ (OLEUI_ERR_STANDARDMAX+0)
OLEUI_ELERR_LINKCNTRINVALID      equ (OLEUI_ERR_STANDARDMAX+1)
ELF_SHOWHELP                     equ 00000001h
ELF_DISABLEUPDATENOW             equ 00000002h
ELF_DISABLEOPENSOURCE            equ 00000004h
ELF_DISABLECHANGESOURCE          equ 00000008h
ELF_DISABLECANCELLINK            equ 00000010h
CIF_SHOWHELP                     equ 00000001h
CIF_SELECTCURRENT                equ 00000002h
CIF_SELECTDEFAULT                equ 00000004h
CIF_SELECTFROMFILE               equ 00000008h
CIF_USEICONEXE                   equ 00000010h
OLEUI_CIERR_MUSTHAVECLSID        equ (OLEUI_ERR_STANDARDMAX+0)
OLEUI_CIERR_MUSTHAVECURRENTMETAFILE equ (OLEUI_ERR_STANDARDMAX+1)
OLEUI_CIERR_SZICONEXEINVALID     equ (OLEUI_ERR_STANDARDMAX+2)
CF_SHOWHELPBUTTON                equ 00000001h
CF_SETCONVERTDEFAULT             equ 00000002h
CF_SETACTIVATEDEFAULT            equ 00000004h
CF_SELECTCONVERTTO               equ 00000008h
CF_SELECTACTIVATEAS              equ 00000010h
CF_DISABLEDISPLAYASICON          equ 00000020h
CF_DISABLEACTIVATEAS             equ 00000040h
CF_HIDECHANGEICON                equ 00000080h
CF_CONVERTONLY                   equ 00000100h
OLEUI_CTERR_CLASSIDINVALID       equ (OLEUI_ERR_STANDARDMAX+1)
OLEUI_CTERR_DVASPECTINVALID      equ (OLEUI_ERR_STANDARDMAX+2)
OLEUI_CTERR_CBFORMATINVALID      equ (OLEUI_ERR_STANDARDMAX+3)
OLEUI_CTERR_HMETAPICTINVALID     equ (OLEUI_ERR_STANDARDMAX+4)
OLEUI_CTERR_STRINGINVALID        equ (OLEUI_ERR_STANDARDMAX+5)
BZ_DISABLECANCELBUTTON           equ 00000001h
BZ_DISABLESWITCHTOBUTTON         equ 00000002h
BZ_DISABLERETRYBUTTON            equ 00000004h
BZ_NOTRESPONDINGDIALOG           equ 00000008h
OLEUI_BZERR_HTASKINVALID         equ (OLEUI_ERR_STANDARDMAX+0)
OLEUI_BZ_SWITCHTOSELECTED        equ (OLEUI_ERR_STANDARDMAX+1)
OLEUI_BZ_RETRYSELECTED           equ (OLEUI_ERR_STANDARDMAX+2)
OLEUI_BZ_CALLUNBLOCKED           equ (OLEUI_ERR_STANDARDMAX+3)
CSF_SHOWHELP                     equ 00000001h
CSF_VALIDSOURCE                  equ 00000002h
CSF_ONLYGETSOURCE                equ 00000004h
CSF_EXPLORER                     equ 00000008h
OLEUI_CSERR_LINKCNTRNULL         equ (OLEUI_ERR_STANDARDMAX+0)
OLEUI_CSERR_LINKCNTRINVALID      equ (OLEUI_ERR_STANDARDMAX+1)
OLEUI_CSERR_FROMNOTNULL          equ (OLEUI_ERR_STANDARDMAX+2)
OLEUI_CSERR_TONOTNULL            equ (OLEUI_ERR_STANDARDMAX+3)
OLEUI_CSERR_SOURCENULL           equ (OLEUI_ERR_STANDARDMAX+4)
OLEUI_CSERR_SOURCEINVALID        equ (OLEUI_ERR_STANDARDMAX+5)
OLEUI_CSERR_SOURCEPARSERROR      equ (OLEUI_ERR_STANDARDMAX+6)
OLEUI_CSERR_SOURCEPARSEERROR     equ (OLEUI_ERR_STANDARDMAX+6)
VPF_SELECTRELATIVE               equ 00000001h
VPF_DISABLERELATIVE              equ 00000002h
VPF_DISABLESCALE                 equ 00000004h
OPF_OBJECTISLINK                 equ 00000001h
OPF_NOFILLDEFAULT                equ 00000002h
OPF_SHOWHELP                     equ 00000004h
OPF_DISABLECONVERT               equ 00000008h
OLEUI_OPERR_SUBPROPNULL          equ (OLEUI_ERR_STANDARDMAX+0)
OLEUI_OPERR_SUBPROPINVALID       equ (OLEUI_ERR_STANDARDMAX+1)
OLEUI_OPERR_PROPSHEETNULL        equ (OLEUI_ERR_STANDARDMAX+2)
OLEUI_OPERR_PROPSHEETINVALID     equ (OLEUI_ERR_STANDARDMAX+3)
OLEUI_OPERR_SUPPROP              equ (OLEUI_ERR_STANDARDMAX+4)
OLEUI_OPERR_PROPSINVALID         equ (OLEUI_ERR_STANDARDMAX+5)
OLEUI_OPERR_PAGESINCORRECT       equ (OLEUI_ERR_STANDARDMAX+6)
OLEUI_OPERR_INVALIDPAGES         equ (OLEUI_ERR_STANDARDMAX+7)
OLEUI_OPERR_NOTSUPPORTED         equ (OLEUI_ERR_STANDARDMAX+8)
OLEUI_OPERR_DLGPROCNOTNULL       equ (OLEUI_ERR_STANDARDMAX+9)
OLEUI_OPERR_LPARAMNOTZERO        equ (OLEUI_ERR_STANDARDMAX+10)
OLEUI_GPERR_STRINGINVALID        equ (OLEUI_ERR_STANDARDMAX+11)
OLEUI_GPERR_CLASSIDINVALID       equ (OLEUI_ERR_STANDARDMAX+12)
OLEUI_GPERR_LPCLSIDEXCLUDEINVALID equ (OLEUI_ERR_STANDARDMAX+13)
OLEUI_GPERR_CBFORMATINVALID      equ (OLEUI_ERR_STANDARDMAX+14)
OLEUI_VPERR_METAPICTINVALID      equ (OLEUI_ERR_STANDARDMAX+15)
OLEUI_VPERR_DVASPECTINVALID      equ (OLEUI_ERR_STANDARDMAX+16)
OLEUI_LPERR_LINKCNTRNULL         equ (OLEUI_ERR_STANDARDMAX+17)
OLEUI_LPERR_LINKCNTRINVALID      equ (OLEUI_ERR_STANDARDMAX+18)
OLEUI_OPERR_PROPERTYSHEET        equ (OLEUI_ERR_STANDARDMAX+19)
OLEUI_OPERR_OBJINFOINVALID       equ (OLEUI_ERR_STANDARDMAX+20)
OLEUI_OPERR_LINKINFOINVALID      equ (OLEUI_ERR_STANDARDMAX+21)
OLEUI_QUERY_GETCLASSID           equ 0FF00h
OLEUI_QUERY_LINKBROKEN           equ 0FF01h

;;;; end oledlg


;;;; htmlhelp

HH_DISPLAY_TOPIC                 equ 0000h
HH_HELP_FINDER                   equ 0000h
HH_DISPLAY_TOC                   equ 0001h
HH_DISPLAY_INDEX                 equ 0002h
HH_DISPLAY_SEARCH                equ 0003h
HH_SET_WIN_TYPE                  equ 0004h
HH_GET_WIN_TYPE                  equ 0005h
HH_GET_WIN_HANDLE                equ 0006h
HH_ENUM_INFO_TYPE                equ 0007h
HH_SET_INFO_TYPE                 equ 0008h
HH_SYNC                          equ 0009h
HH_RESERVED1                     equ 000Ah
HH_RESERVED2                     equ 000Bh
HH_RESERVED3                     equ 000Ch
HH_KEYWORD_LOOKUP                equ 000Dh
HH_DISPLAY_TEXT_POPUP            equ 000Eh
HH_HELP_CONTEXT                  equ 000Fh
HH_TP_HELP_CONTEXTMENU           equ 0010h
HH_TP_HELP_WM_HELP               equ 0011h
HH_CLOSE_ALL                     equ 0012h
HH_ALINK_LOOKUP                  equ 0013h
HH_GET_LAST_ERROR                equ 0014h
HH_ENUM_CATEGORY                 equ 0015h
HH_ENUM_CATEGORY_IT              equ 0016h
HH_RESET_IT_FILTER               equ 0017h
HH_SET_INCLUSIVE_FILTER          equ 0018h
HH_SET_EXCLUSIVE_FILTER          equ 0019h
HH_INITIALIZE                    equ 001Ch
HH_UNINITIALIZE                  equ 001Dh
HH_SET_QUERYSERVICE              equ 001Eh
HH_PRETRANSLATEMESSAGE           equ 00fdh
HH_SET_GLOBAL_PROPERTY           equ 00fch
IDTB_EXPAND                      equ 200
IDTB_CONTRACT                    equ 201
IDTB_STOP                        equ 202
IDTB_REFRESH                     equ 203
IDTB_BACK                        equ 204
IDTB_HOME                        equ 205
IDTB_SYNC                        equ 206
IDTB_PRINT                       equ 207
IDTB_OPTIONS                     equ 208
IDTB_FORWARD                     equ 209
IDTB_NOTES                       equ 210
IDTB_BROWSE_FWD                  equ 211
IDTB_BROWSE_BACK                 equ 212
IDTB_CONTENTS                    equ 213
IDTB_INDEX                       equ 214
IDTB_SEARCH                      equ 215
IDTB_HISTORY                     equ 216
IDTB_FAVORITES                   equ 217
IDTB_JUMP1                       equ 218
IDTB_JUMP2                       equ 219
IDTB_CUSTOMIZE                   equ 221
IDTB_ZOOM                        equ 222
IDTB_TOC_NEXT                    equ 223
IDTB_TOC_PREV                    equ 224
HH_MAX_TABS                      equ 19
HH_FTS_DEFAULT_PROXIMITY         equ -1
ATOM_HTMLHELP_API_ANSI           equ 14
ATOM_HTMLHELP_API_UNICODE        equ 15

;;;; end htmlhelp


;;;; wincon

NLS_DBCSCHAR                     equ 00010000h
NLS_ALPHANUMERIC                 equ 00000000h
NLS_KATAKANA                     equ 00020000h
NLS_HIRAGANA                     equ 00040000h
NLS_ROMAN                        equ 00400000h
NLS_IME_CONVERSION               equ 00800000h
NLS_IME_DISABLE                  equ 20000000h
MOUSE_WHEELED                    equ 0004h
MOUSE_HWHEELED                   equ 0008h
COMMON_LVB_LEADING_BYTE          equ 0100h
COMMON_LVB_TRAILING_BYTE         equ 0200h
COMMON_LVB_GRID_HORIZONTAL       equ 0400h
COMMON_LVB_GRID_LVERTICAL        equ 0800h
COMMON_LVB_GRID_RVERTICAL        equ 1000h
COMMON_LVB_REVERSE_VIDEO         equ 4000h
COMMON_LVB_UNDERSCORE            equ 8000h
COMMON_LVB_SBCSDBCS              equ 0300h
HISTORY_NO_DUP_FLAG              equ 1h
CONSOLE_NO_SELECTION             equ 0000h
CONSOLE_SELECTION_IN_PROGRESS    equ 0001h
CONSOLE_SELECTION_NOT_EMPTY      equ 0002h
CONSOLE_MOUSE_SELECTION          equ 0004h
CONSOLE_MOUSE_DOWN               equ 0008h
ENABLE_INSERT_MODE               equ 0020h
ENABLE_QUICK_EDIT_MODE           equ 0040h
ENABLE_EXTENDED_FLAGS            equ 0080h
ENABLE_AUTO_POSITION             equ 0100h
CONSOLE_REAL_OUTPUT_HANDLE       equ -2
CONSOLE_REAL_INPUT_HANDLE        equ -3
ATTACH_PARENT_PROCESS            equ -1
CONSOLE_FULLSCREEN               equ 1
CONSOLE_FULLSCREEN_HARDWARE      equ 2
CONSOLE_FULLSCREEN_MODE          equ 1
CONSOLE_WINDOWED_MODE            equ 2

;;;; end wincon


;;;; userenv

PI_NOUI                          equ 00000001h
PI_APPLYPOLICY                   equ 00000002h
PT_TEMPORARY                     equ 00000001h
PT_ROAMING                       equ 00000002h
PT_MANDATORY                     equ 00000004h
RP_FORCE                         equ 1
RP_SYNC                          equ 2
GPC_BLOCK_POLICY                 equ 00000001h
GPO_FLAG_DISABLE                 equ 00000001h
GPO_FLAG_FORCE                   equ 00000002h
GPO_LIST_FLAG_MACHINE            equ 00000001h
GPO_LIST_FLAG_SITEONLY           equ 00000002h
GPO_LIST_FLAG_NO_WMIFILTERS      equ 00000004h
GPO_LIST_FLAG_NO_SECURITYFILTERS equ 00000008h
GPO_INFO_FLAG_MACHINE            equ 00000001h
GPO_INFO_FLAG_BACKGROUND         equ 00000010h
GPO_INFO_FLAG_SLOWLINK           equ 00000020h
GPO_INFO_FLAG_VERBOSE            equ 00000040h
GPO_INFO_FLAG_NOCHANGES          equ 00000080h
GPO_INFO_FLAG_LINKTRANSITION     equ 00000100h
GPO_INFO_FLAG_LOGRSOP_TRANSITION equ 00000200h
GPO_INFO_FLAG_FORCED_REFRESH     equ 00000400h
GPO_INFO_FLAG_SAFEMODE_BOOT      equ 00000800h
GPO_INFO_FLAG_ASYNC_FOREGROUND   equ 00001000h
FLAG_NO_GPO_FILTER               equ 80000000h
FLAG_NO_CSE_INVOKE               equ 40000000h
FLAG_ASSUME_SLOW_LINK            equ 20000000h
FLAG_LOOPBACK_MERGE              equ 10000000h
FLAG_LOOPBACK_REPLACE            equ 08000000h
FLAG_ASSUME_USER_WQLFILTER_TRUE  equ 04000000h
FLAG_ASSUME_COMP_WQLFILTER_TRUE  equ 02000000h
FLAG_PLANNING_MODE               equ 01000000h
FLAG_NO_USER                     equ 00000001h
FLAG_NO_COMPUTER                 equ 00000002h
FLAG_FORCE_CREATENAMESPACE       equ 00000004h
RSOP_USER_ACCESS_DENIED          equ 00000001h
RSOP_COMPUTER_ACCESS_DENIED      equ 00000002h
RSOP_TEMPNAMESPACE_EXISTS        equ 00000004h

;;;; userenv

MCI_VCR_OFFSET                   equ 1280
MCI_LIST                         equ 0878h
MCI_SETAUDIO                     equ 0873h
MCI_SETVIDEO                     equ 0876h
MCI_SIGNAL                       equ 0875h
MCI_MARK                         equ MCI_USER_MESSAGES + 0
MCI_INDEX                        equ MCI_USER_MESSAGES + 1
MCI_SETTUNER                     equ MCI_USER_MESSAGES + 2
MCI_SETVCR                       equ MCI_USER_MESSAGES + 3
MCI_SETTIMECODE                  equ MCI_USER_MESSAGES + 4
MCI_TEST                         equ 00000020h
MCI_VCR_GETDEVCAPS_CAN_DETECT_LENGTH equ 00004001h
MCI_VCR_GETDEVCAPS_SEEK_ACCURACY equ 00004002h
MCI_VCR_GETDEVCAPS_HAS_CLOCK     equ 00004003h
MCI_VCR_GETDEVCAPS_CAN_REVERSE   equ 00004004h
MCI_VCR_GETDEVCAPS_NUMBER_OF_MARKS equ 00004005h
MCI_VCR_GETDEVCAPS_CAN_TEST      equ 00004006h
MCI_VCR_GETDEVCAPS_CAN_PREROLL   equ 00004007h
MCI_VCR_GETDEVCAPS_CAN_PREVIEW   equ 00004008h
MCI_VCR_GETDEVCAPS_CAN_MONITOR_SOURCES equ 00004009h
MCI_VCR_GETDEVCAPS_HAS_TIMECODE  equ 0000400Ah
MCI_VCR_GETDEVCAPS_CAN_FREEZE    equ 0000401Bh
MCI_VCR_GETDEVCAPS_CLOCK_INCREMENT_RATE equ 0000401Ch
MCI_VCR_INFO_VERSION             equ 00010000h
MCI_VCR_PLAY_REVERSE             equ 00010000h
MCI_VCR_PLAY_AT                  equ 00020000h
MCI_VCR_PLAY_SCAN                equ 00040000h
MCI_VCR_RECORD_INITIALIZE        equ 00010000h
MCI_VCR_RECORD_AT                equ 00020000h
MCI_VCR_RECORD_PREVIEW           equ 00040000h
MCI_VCR_CUE_INPUT                equ 00010000h
MCI_VCR_CUE_OUTPUT               equ 00020000h
MCI_VCR_CUE_PREROLL              equ 00040000h
MCI_VCR_CUE_REVERSE              equ 00080000h
MCI_VCR_SEEK_REVERSE             equ 00010000h
MCI_VCR_SEEK_MARK                equ 00020000h
MCI_VCR_SEEK_AT                  equ 00040000h
MCI_VCR_SETTUNER_CHANNEL         equ 00010000h
MCI_VCR_SETTUNER_CHANNEL_UP      equ 00020000h
MCI_VCR_SETTUNER_CHANNEL_DOWN    equ 00040000h
MCI_VCR_SETTUNER_CHANNEL_SEEK_UP equ 00080000h
MCI_VCR_SETTUNER_CHANNEL_SEEK_DOWN equ 00100000h
MCI_VCR_SETTUNER_NUMBER          equ 00200000h
MCI_VCR_SET_TIME_MODE            equ 00010000h
MCI_VCR_SET_POWER                equ 00020000h
MCI_VCR_SET_RECORD_FORMAT        equ 00040000h
MCI_VCR_SET_COUNTER_FORMAT       equ 00080000h
MCI_VCR_SET_INDEX                equ 00100000h
MCI_VCR_SET_ASSEMBLE_RECORD      equ 00200000h
MCI_VCR_SET_TRACKING             equ 00400000h
MCI_VCR_SET_SPEED                equ 00800000h
MCI_VCR_SET_TAPE_LENGTH          equ 01000000h
MCI_VCR_SET_COUNTER_VALUE        equ 02000000h
MCI_VCR_SET_CLOCK                equ 04000000h
MCI_VCR_SET_PAUSE_TIMEOUT        equ 08000000h
MCI_VCR_SET_PREROLL_DURATION     equ 10000000h
MCI_VCR_SET_POSTROLL_DURATION    equ 20000000h
MCI_VCR_SETTIMECODE_RECORD       equ 00010000h
MCI_VCR_STATUS_FRAME_RATE        equ 00004001h
MCI_VCR_STATUS_SPEED             equ 00004002h
MCI_VCR_STATUS_MEDIA_TYPE        equ 00004003h
MCI_VCR_STATUS_RECORD_FORMAT     equ 00004004h
MCI_VCR_STATUS_PLAY_FORMAT       equ 00004005h
MCI_VCR_STATUS_AUDIO_SOURCE      equ 00004006h
MCI_VCR_STATUS_AUDIO_SOURCE_NUMBER equ 00004007h
MCI_VCR_STATUS_VIDEO_SOURCE      equ 00004008h
MCI_VCR_STATUS_VIDEO_SOURCE_NUMBER equ 00004009h
MCI_VCR_STATUS_AUDIO_MONITOR     equ 0000400Ah
MCI_VCR_STATUS_AUDIO_MONITOR_NUMBER equ 0000400Bh
MCI_VCR_STATUS_VIDEO_MONITOR     equ 0000400Ch
MCI_VCR_STATUS_VIDEO_MONITOR_NUMBER equ 0000400Dh
MCI_VCR_STATUS_INDEX_ON          equ 0000400Eh
MCI_VCR_STATUS_INDEX             equ 0000400Fh
MCI_VCR_STATUS_COUNTER_FORMAT    equ 00004010h
MCI_VCR_STATUS_COUNTER_RESOLUTION equ 00004011h
MCI_VCR_STATUS_TIMECODE_TYPE     equ 00004012h
MCI_VCR_STATUS_COUNTER_VALUE     equ 00004013h
MCI_VCR_STATUS_TUNER_CHANNEL     equ 00004014h
MCI_VCR_STATUS_WRITE_PROTECTED   equ 00004015h
MCI_VCR_STATUS_TIMECODE_RECORD   equ 00004016h
MCI_VCR_STATUS_VIDEO_RECORD      equ 00004017h
MCI_VCR_STATUS_AUDIO_RECORD      equ 00004018h
MCI_VCR_STATUS_TIME_TYPE         equ 00004019h
MCI_VCR_STATUS_TIME_MODE         equ 0000401Ah
MCI_VCR_STATUS_POWER_ON          equ 0000401Bh
MCI_VCR_STATUS_CLOCK             equ 0000401Ch
MCI_VCR_STATUS_ASSEMBLE_RECORD   equ 0000401Dh
MCI_VCR_STATUS_TIMECODE_PRESENT  equ 0000401Eh
MCI_VCR_STATUS_NUMBER_OF_VIDEO_TRACKS equ 0000401Fh
MCI_VCR_STATUS_NUMBER_OF_AUDIO_TRACKS equ 00004020h
MCI_VCR_STATUS_CLOCK_ID          equ 00004021h
MCI_VCR_STATUS_PAUSE_TIMEOUT     equ 00004022h
MCI_VCR_STATUS_PREROLL_DURATION  equ 00004023h
MCI_VCR_STATUS_POSTROLL_DURATION equ 00004024h
MCI_VCR_STATUS_VIDEO             equ 00004025h
MCI_VCR_STATUS_AUDIO             equ 00004026h
MCI_VCR_STATUS_NUMBER            equ 00080000h
MCI_VCR_ESCAPE_STRING            equ 00000100h
MCI_VCR_LIST_VIDEO_SOURCE        equ 00010000h
MCI_VCR_LIST_AUDIO_SOURCE        equ 00020000h
MCI_VCR_LIST_COUNT               equ 00040000h
MCI_VCR_LIST_NUMBER              equ 00080000h
MCI_VCR_MARK_WRITE               equ 00010000h
MCI_VCR_MARK_ERASE               equ 00020000h
MCI_VCR_SETAUDIO_RECORD          equ 00010000h
MCI_VCR_SETAUDIO_SOURCE          equ 00020000h
MCI_VCR_SETAUDIO_MONITOR         equ 00040000h
MCI_VCR_SETAUDIO_TO              equ 00200000h
MCI_VCR_SETAUDIO_NUMBER          equ 00400000h
MCI_VCR_SETVIDEO_RECORD          equ 00010000h
MCI_VCR_SETVIDEO_SOURCE          equ 00020000h
MCI_VCR_SETVIDEO_MONITOR         equ 00040000h
MCI_VCR_SETVIDEO_TO              equ 00100000h
MCI_VCR_SETVIDEO_NUMBER          equ 00200000h
MCI_VCR_SIGNAL_AT                equ 00010000h
MCI_VCR_SIGNAL_EVERY             equ 00020000h
MCI_VCR_SIGNAL_USERVAL           equ 00040000h
MCI_VCR_SIGNAL_CANCEL            equ 00080000h
MCI_VCR_SIGNAL_POSITION          equ 00100000h
MCI_VCR_STEP_FRAMES              equ 00010000h
MCI_VCR_STEP_REVERSE             equ 00020000h
MCI_VCR_FREEZE_INPUT             equ 00010000h
MCI_VCR_FREEZE_OUTPUT            equ 00020000h
MCI_VCR_FREEZE_FIELD             equ 00040000h
MCI_VCR_FREEZE_FRAME             equ 00080000h
MCI_VCR_UNFREEZE_INPUT           equ 00010000h
MCI_VCR_UNFREEZE_OUTPUT          equ 00020000h
MCI_VCR_MEDIA_8MM                equ MCI_VCR_OFFSET + 1
MCI_VCR_MEDIA_HI8                equ MCI_VCR_OFFSET + 2
MCI_VCR_MEDIA_VHS                equ MCI_VCR_OFFSET + 3
MCI_VCR_MEDIA_SVHS               equ MCI_VCR_OFFSET + 4
MCI_VCR_MEDIA_BETA               equ MCI_VCR_OFFSET + 5
MCI_VCR_MEDIA_EDBETA             equ MCI_VCR_OFFSET + 6
MCI_VCR_MEDIA_OTHER              equ MCI_VCR_OFFSET + 7
MCI_VCR_FORMAT_SP                equ MCI_VCR_OFFSET + 8
MCI_VCR_FORMAT_LP                equ MCI_VCR_OFFSET + 9
MCI_VCR_FORMAT_EP                equ MCI_VCR_OFFSET + 10
MCI_VCR_FORMAT_OTHER             equ MCI_VCR_OFFSET + 11
MCI_VCR_TIME_TIMECODE            equ MCI_VCR_OFFSET + 12
MCI_VCR_TIME_COUNTER             equ MCI_VCR_OFFSET + 13
MCI_VCR_TIME_DETECT              equ MCI_VCR_OFFSET + 14
MCI_VCR_SRC_TYPE_TUNER           equ MCI_VCR_OFFSET + 15
MCI_VCR_SRC_TYPE_LINE            equ MCI_VCR_OFFSET + 16
MCI_VCR_SRC_TYPE_SVIDEO          equ MCI_VCR_OFFSET + 17
MCI_VCR_SRC_TYPE_RGB             equ MCI_VCR_OFFSET + 18
MCI_VCR_SRC_TYPE_AUX             equ MCI_VCR_OFFSET + 19
MCI_VCR_SRC_TYPE_GENERIC         equ MCI_VCR_OFFSET + 20
MCI_VCR_SRC_TYPE_MUTE            equ MCI_VCR_OFFSET + 21
MCI_VCR_SRC_TYPE_OUTPUT          equ MCI_VCR_OFFSET + 22
MCI_VCR_INDEX_TIMECODE           equ MCI_VCR_OFFSET + 23
MCI_VCR_INDEX_COUNTER            equ MCI_VCR_OFFSET + 24
MCI_VCR_INDEX_DATE               equ MCI_VCR_OFFSET + 25
MCI_VCR_INDEX_TIME               equ MCI_VCR_OFFSET + 26
MCI_VCR_COUNTER_RES_SECONDS      equ MCI_VCR_OFFSET + 27
MCI_VCR_COUNTER_RES_FRAMES       equ MCI_VCR_OFFSET + 28
MCI_VCR_TIMECODE_TYPE_SMPTE      equ MCI_VCR_OFFSET + 29
MCI_VCR_TIMECODE_TYPE_SMPTE_DROP equ MCI_VCR_OFFSET + 30
MCI_VCR_TIMECODE_TYPE_OTHER      equ MCI_VCR_OFFSET + 31
MCI_VCR_TIMECODE_TYPE_NONE       equ MCI_VCR_OFFSET + 32
MCI_VCR_PLUS                     equ MCI_VCR_OFFSET + 33
MCI_VCR_MINUS                    equ MCI_VCR_OFFSET + 34
MCI_VCR_RESET                    equ MCI_VCR_OFFSET + 35
MCIERR_VCR_CANNOT_OPEN_COMM      equ MCIERR_CUSTOM_DRIVER_BASE + 1
MCIERR_VCR_CANNOT_WRITE_COMM     equ MCIERR_CUSTOM_DRIVER_BASE + 2
MCIERR_VCR_READ_TIMEOUT          equ MCIERR_CUSTOM_DRIVER_BASE + 3
MCIERR_VCR_COMMAND_BUFFER_FULL   equ MCIERR_CUSTOM_DRIVER_BASE + 4
MCIERR_VCR_COMMAND_CANCELLED     equ MCIERR_CUSTOM_DRIVER_BASE + 5
MCIERR_VCR_POWER_OFF             equ MCIERR_CUSTOM_DRIVER_BASE + 6
MCIERR_VCR_COMMAND_FAILED        equ MCIERR_CUSTOM_DRIVER_BASE + 7
MCIERR_VCR_SEARCH                equ MCIERR_CUSTOM_DRIVER_BASE + 8
MCIERR_VCR_CONDITION             equ MCIERR_CUSTOM_DRIVER_BASE + 9
MCIERR_VCR_CAMERA_MODE           equ MCIERR_CUSTOM_DRIVER_BASE + 10
MCIERR_VCR_VCR_MODE              equ MCIERR_CUSTOM_DRIVER_BASE + 11
MCIERR_VCR_COUNTER_TYPE          equ MCIERR_CUSTOM_DRIVER_BASE + 12
MCIERR_VCR_TUNER                 equ MCIERR_CUSTOM_DRIVER_BASE + 13
MCIERR_VCR_EMERGENCY_STOP        equ MCIERR_CUSTOM_DRIVER_BASE + 14
MCIERR_VCR_MEDIA_UNMOUNTED       equ MCIERR_CUSTOM_DRIVER_BASE + 15
MCIERR_VCR_REGISTER              equ MCIERR_CUSTOM_DRIVER_BASE + 16
MCIERR_VCR_TRACK_FAILURE         equ MCIERR_CUSTOM_DRIVER_BASE + 17
MCIERR_VCR_CUE_FAILED_FLAGS      equ MCIERR_CUSTOM_DRIVER_BASE + 18
MCIERR_VCR_ISWRITEPROTECTED      equ MCIERR_CUSTOM_DRIVER_BASE + 19


;;;; winerror

FACILITY_WINRM                   equ 51
FACILITY_WINDOWSUPDATE           equ 36
FACILITY_WINDOWS_DEFENDER        equ 80
FACILITY_WINDOWS_CE              equ 24
FACILITY_WINDOWS                 equ 8
FACILITY_URT                     equ 19
FACILITY_UMI                     equ 22
FACILITY_TPM_SOFTWARE            equ 41
FACILITY_TPM_SERVICES            equ 40
FACILITY_SXS                     equ 23
FACILITY_STORAGE                 equ 3
FACILITY_STATE_MANAGEMENT        equ 34
FACILITY_SSPI                    equ 9
FACILITY_SCARD                   equ 16
FACILITY_SHELL                   equ 39
FACILITY_SETUPAPI                equ 15
FACILITY_SECURITY                equ 9
FACILITY_RPC                     equ 1
FACILITY_PLA                     equ 48
FACILITY_WIN32                   equ 7
FACILITY_CONTROL                 equ 10
FACILITY_NULL                    equ 0
FACILITY_NDIS                    equ 52
FACILITY_METADIRECTORY           equ 35
FACILITY_MSMQ                    equ 14
FACILITY_MEDIASERVER             equ 13
FACILITY_INTERNET                equ 12
FACILITY_ITF                     equ 4
FACILITY_USERMODE_HYPERVISOR     equ 53
FACILITY_HTTP                    equ 25
FACILITY_GRAPHICS                equ 38
FACILITY_FWP                     equ 50
FACILITY_FVE                     equ 49
FACILITY_USERMODE_FILTER_MANAGER equ 31
FACILITY_DPLAY                   equ 21
FACILITY_DISPATCH                equ 2
FACILITY_DIRECTORYSERVICE        equ 37
FACILITY_CONFIGURATION           equ 33
FACILITY_COMPLUS                 equ 17
FACILITY_USERMODE_COMMONLOG      equ 26
FACILITY_CMI                     equ 54
FACILITY_CERT                    equ 11
FACILITY_BACKGROUNDCOPY          equ 32
FACILITY_ACS                     equ 20
FACILITY_AAF                     equ 18
SEC_E_OK                         equ 00000000h
ERROR_EXE_MACHINE_TYPE_MISMATCH  equ 216
ERROR_EXE_CANNOT_MODIFY_SIGNED_BINARY equ 217
ERROR_EXE_CANNOT_MODIFY_STRONG_SIGNED_BINARY equ 218
ERROR_FILE_CHECKED_OUT           equ 220
ERROR_CHECKOUT_REQUIRED          equ 221
ERROR_BAD_FILE_TYPE              equ 222
ERROR_FILE_TOO_LARGE             equ 223
ERROR_FORMS_AUTH_REQUIRED        equ 224
ERROR_VIRUS_INFECTED             equ 225
ERROR_VIRUS_DELETED              equ 226
ERROR_PIPE_LOCAL                 equ 229
ERROR_PARTIAL_COPY               equ 299
ERROR_OPLOCK_NOT_GRANTED         equ 300
ERROR_INVALID_OPLOCK_PROTOCOL    equ 301
ERROR_DISK_TOO_FRAGMENTED        equ 302
ERROR_DELETE_PENDING             equ 303
ERROR_SCOPE_NOT_FOUND            equ 318
ERROR_FAIL_NOACTION_REBOOT       equ 350
ERROR_FAIL_SHUTDOWN              equ 351
ERROR_FAIL_RESTART               equ 352
ERROR_MAX_SESSIONS_REACHED       equ 353
ERROR_THREAD_MODE_ALREADY_BACKGROUND equ 400
ERROR_THREAD_MODE_NOT_BACKGROUND equ 401
ERROR_PROCESS_MODE_ALREADY_BACKGROUND equ 402
ERROR_PROCESS_MODE_NOT_BACKGROUND equ 403
ERROR_USER_PROFILE_LOAD          equ 500
ERROR_VERIFIER_STOP              equ 537
ERROR_ABIOS_ERROR                equ 538
ERROR_WX86_WARNING               equ 539
ERROR_WX86_ERROR                 equ 540
ERROR_TIMER_NOT_CANCELED         equ 541
ERROR_UNWIND                     equ 542
ERROR_BAD_STACK                  equ 543
ERROR_INVALID_UNWIND_TARGET      equ 544
ERROR_INVALID_PORT_ATTRIBUTES    equ 545
ERROR_PORT_MESSAGE_TOO_LONG      equ 546
ERROR_INVALID_QUOTA_LOWER        equ 547
ERROR_DEVICE_ALREADY_ATTACHED    equ 548
ERROR_INSTRUCTION_MISALIGNMENT   equ 549
ERROR_PROFILING_NOT_STARTED      equ 550
ERROR_PROFILING_NOT_STOPPED      equ 551
ERROR_COULD_NOT_INTERPRET        equ 552
ERROR_PROFILING_AT_LIMIT         equ 553
ERROR_CANT_WAIT                  equ 554
ERROR_CANT_TERMINATE_SELF        equ 555
ERROR_UNEXPECTED_MM_CREATE_ERR   equ 556
ERROR_UNEXPECTED_MM_MAP_ERROR    equ 557
ERROR_UNEXPECTED_MM_EXTEND_ERR   equ 558
ERROR_BAD_FUNCTION_TABLE         equ 559
ERROR_NO_GUID_TRANSLATION        equ 560
ERROR_INVALID_LDT_SIZE           equ 561
ERROR_INVALID_LDT_OFFSET         equ 563
ERROR_INVALID_LDT_DESCRIPTOR     equ 564
ERROR_TOO_MANY_THREADS           equ 565
ERROR_THREAD_NOT_IN_PROCESS      equ 566
ERROR_PAGEFILE_QUOTA_EXCEEDED    equ 567
ERROR_LOGON_SERVER_CONFLICT      equ 568
ERROR_SYNCHRONIZATION_REQUIRED   equ 569
ERROR_NET_OPEN_FAILED            equ 570
ERROR_IO_PRIVILEGE_FAILED        equ 571
ERROR_CONTROL_C_EXIT             equ 572
ERROR_MISSING_SYSTEMFILE         equ 573
ERROR_UNHANDLED_EXCEPTION        equ 574
ERROR_APP_INIT_FAILURE           equ 575
ERROR_PAGEFILE_CREATE_FAILED     equ 576
ERROR_INVALID_IMAGE_HASH         equ 577
ERROR_NO_PAGEFILE                equ 578
ERROR_ILLEGAL_FLOAT_CONTEXT      equ 579
ERROR_NO_EVENT_PAIR              equ 580
ERROR_DOMAIN_CTRLR_CONFIG_ERROR  equ 581
ERROR_ILLEGAL_CHARACTER          equ 582
ERROR_UNDEFINED_CHARACTER        equ 583
ERROR_FLOPPY_VOLUME              equ 584
ERROR_BIOS_FAILED_TO_CONNECT_INTERRUPT equ 585
ERROR_BACKUP_CONTROLLER          equ 586
ERROR_MUTANT_LIMIT_EXCEEDED      equ 587
ERROR_FS_DRIVER_REQUIRED         equ 588
ERROR_CANNOT_LOAD_REGISTRY_FILE  equ 589
ERROR_DEBUG_ATTACH_FAILED        equ 590
ERROR_SYSTEM_PROCESS_TERMINATED  equ 591
ERROR_DATA_NOT_ACCEPTED          equ 592
ERROR_VDM_HARD_ERROR             equ 593
ERROR_DRIVER_CANCEL_TIMEOUT      equ 594
ERROR_REPLY_MESSAGE_MISMATCH     equ 595
ERROR_LOST_WRITEBEHIND_DATA      equ 596
ERROR_CLIENT_SERVER_PARAMETERS_INVALID equ 597
ERROR_NOT_TINY_STREAM            equ 598
ERROR_STACK_OVERFLOW_READ        equ 599
ERROR_CONVERT_TO_LARGE           equ 600
ERROR_FOUND_OUT_OF_SCOPE         equ 601
ERROR_ALLOCATE_BUCKET            equ 602
ERROR_MARSHALL_OVERFLOW          equ 603
ERROR_INVALID_VARIANT            equ 604
ERROR_BAD_COMPRESSION_BUFFER     equ 605
ERROR_AUDIT_FAILED               equ 606
ERROR_TIMER_RESOLUTION_NOT_SET   equ 607
ERROR_INSUFFICIENT_LOGON_INFO    equ 608
ERROR_BAD_DLL_ENTRYPOINT         equ 609
ERROR_BAD_SERVICE_ENTRYPOINT     equ 610
ERROR_IP_ADDRESS_CONFLICT1       equ 611
ERROR_IP_ADDRESS_CONFLICT2       equ 612
ERROR_REGISTRY_QUOTA_LIMIT       equ 613
ERROR_NO_CALLBACK_ACTIVE         equ 614
ERROR_PWD_TOO_SHORT              equ 615
ERROR_PWD_TOO_RECENT             equ 616
ERROR_PWD_HISTORY_CONFLICT       equ 617
ERROR_UNSUPPORTED_COMPRESSION    equ 618
ERROR_INVALID_HW_PROFILE         equ 619
ERROR_INVALID_PLUGPLAY_DEVICE_PATH equ 620
ERROR_QUOTA_LIST_INCONSISTENT    equ 621
ERROR_EVALUATION_EXPIRATION      equ 622
ERROR_ILLEGAL_DLL_RELOCATION     equ 623
ERROR_DLL_INIT_FAILED_LOGOFF     equ 624
ERROR_VALIDATE_CONTINUE          equ 625
ERROR_NO_MORE_MATCHES            equ 626
ERROR_RANGE_LIST_CONFLICT        equ 627
ERROR_SERVER_SID_MISMATCH        equ 628
ERROR_CANT_ENABLE_DENY_ONLY      equ 629
ERROR_FLOAT_MULTIPLE_FAULTS      equ 630
ERROR_FLOAT_MULTIPLE_TRAPS       equ 631
ERROR_NOINTERFACE                equ 632
ERROR_DRIVER_FAILED_SLEEP        equ 633
ERROR_CORRUPT_SYSTEM_FILE        equ 634
ERROR_COMMITMENT_MINIMUM         equ 635
ERROR_PNP_RESTART_ENUMERATION    equ 636
ERROR_SYSTEM_IMAGE_BAD_SIGNATURE equ 637
ERROR_PNP_REBOOT_REQUIRED        equ 638
ERROR_INSUFFICIENT_POWER         equ 639
ERROR_MULTIPLE_FAULT_VIOLATION   equ 640
ERROR_SYSTEM_SHUTDOWN            equ 641
ERROR_PORT_NOT_SET               equ 642
ERROR_DS_VERSION_CHECK_FAILURE   equ 643
ERROR_RANGE_NOT_FOUND            equ 644
ERROR_NOT_SAFE_MODE_DRIVER       equ 646
ERROR_FAILED_DRIVER_ENTRY        equ 647
ERROR_DEVICE_ENUMERATION_ERROR   equ 648
ERROR_MOUNT_POINT_NOT_RESOLVED   equ 649
ERROR_INVALID_DEVICE_OBJECT_PARAMETER equ 650
ERROR_MCA_OCCURED                equ 651
ERROR_DRIVER_DATABASE_ERROR      equ 652
ERROR_SYSTEM_HIVE_TOO_LARGE      equ 653
ERROR_DRIVER_FAILED_PRIOR_UNLOAD equ 654
ERROR_VOLSNAP_PREPARE_HIBERNATE  equ 655
ERROR_HIBERNATION_FAILURE        equ 656
ERROR_FILE_SYSTEM_LIMITATION     equ 665
ERROR_ASSERTION_FAILURE          equ 668
ERROR_ACPI_ERROR                 equ 669
ERROR_WOW_ASSERTION              equ 670
ERROR_PNP_BAD_MPS_TABLE          equ 671
ERROR_PNP_TRANSLATION_FAILED     equ 672
ERROR_PNP_IRQ_TRANSLATION_FAILED equ 673
ERROR_PNP_INVALID_ID             equ 674
ERROR_WAKE_SYSTEM_DEBUGGER       equ 675
ERROR_HANDLES_CLOSED             equ 676
ERROR_EXTRANEOUS_INFORMATION     equ 677
ERROR_RXACT_COMMIT_NECESSARY     equ 678
ERROR_MEDIA_CHECK                equ 679
ERROR_GUID_SUBSTITUTION_MADE     equ 680
ERROR_STOPPED_ON_SYMLINK         equ 681
ERROR_LONGJUMP                   equ 682
ERROR_PLUGPLAY_QUERY_VETOED      equ 683
ERROR_UNWIND_CONSOLIDATE         equ 684
ERROR_REGISTRY_HIVE_RECOVERED    equ 685
ERROR_DLL_MIGHT_BE_INSECURE      equ 686
ERROR_DLL_MIGHT_BE_INCOMPATIBLE  equ 687
ERROR_DBG_EXCEPTION_NOT_HANDLED  equ 688
ERROR_DBG_REPLY_LATER            equ 689
ERROR_DBG_UNABLE_TO_PROVIDE_HANDLE equ 690
ERROR_DBG_TERMINATE_THREAD       equ 691
ERROR_DBG_TERMINATE_PROCESS      equ 692
ERROR_DBG_CONTROL_C              equ 693
ERROR_DBG_PRINTEXCEPTION_C       equ 694
ERROR_DBG_RIPEXCEPTION           equ 695
ERROR_DBG_CONTROL_BREAK          equ 696
ERROR_DBG_COMMAND_EXCEPTION      equ 697
ERROR_OBJECT_NAME_EXISTS         equ 698
ERROR_THREAD_WAS_SUSPENDED       equ 699
ERROR_IMAGE_NOT_AT_BASE          equ 700
ERROR_RXACT_STATE_CREATED        equ 701
ERROR_SEGMENT_NOTIFICATION       equ 702
ERROR_BAD_CURRENT_DIRECTORY      equ 703
ERROR_FT_READ_RECOVERY_FROM_BACKUP equ 704
ERROR_FT_WRITE_RECOVERY          equ 705
ERROR_IMAGE_MACHINE_TYPE_MISMATCH equ 706
ERROR_RECEIVE_PARTIAL            equ 707
ERROR_RECEIVE_EXPEDITED          equ 708
ERROR_RECEIVE_PARTIAL_EXPEDITED  equ 709
ERROR_EVENT_DONE                 equ 710
ERROR_EVENT_PENDING              equ 711
ERROR_CHECKING_FILE_SYSTEM       equ 712
ERROR_FATAL_APP_EXIT             equ 713
ERROR_PREDEFINED_HANDLE          equ 714
ERROR_WAS_UNLOCKED               equ 715
ERROR_SERVICE_NOTIFICATION       equ 716
ERROR_WAS_LOCKED                 equ 717
ERROR_LOG_HARD_ERROR             equ 718
ERROR_ALREADY_WIN32              equ 719
ERROR_IMAGE_MACHINE_TYPE_MISMATCH_EXE equ 720
ERROR_NO_YIELD_PERFORMED         equ 721
ERROR_TIMER_RESUME_IGNORED       equ 722
ERROR_ARBITRATION_UNHANDLED      equ 723
ERROR_CARDBUS_NOT_SUPPORTED      equ 724
ERROR_MP_PROCESSOR_MISMATCH      equ 725
ERROR_HIBERNATED                 equ 726
ERROR_RESUME_HIBERNATION         equ 727
ERROR_FIRMWARE_UPDATED           equ 728
ERROR_DRIVERS_LEAKING_LOCKED_PAGES equ 729
ERROR_WAKE_SYSTEM                equ 730
ERROR_WAIT_1                     equ 731
ERROR_WAIT_2                     equ 732
ERROR_WAIT_3                     equ 733
ERROR_WAIT_63                    equ 734
ERROR_ABANDONED_WAIT_0           equ 735
ERROR_ABANDONED_WAIT_63          equ 736
ERROR_USER_APC                   equ 737
ERROR_KERNEL_APC                 equ 738
ERROR_ALERTED                    equ 739
ERROR_ELEVATION_REQUIRED         equ 740
ERROR_REPARSE                    equ 741
ERROR_OPLOCK_BREAK_IN_PROGRESS   equ 742
ERROR_VOLUME_MOUNTED             equ 743
ERROR_RXACT_COMMITTED            equ 744
ERROR_NOTIFY_CLEANUP             equ 745
ERROR_PRIMARY_TRANSPORT_CONNECT_FAILED equ 746
ERROR_PAGE_FAULT_TRANSITION      equ 747
ERROR_PAGE_FAULT_DEMAND_ZERO     equ 748
ERROR_PAGE_FAULT_COPY_ON_WRITE   equ 749
ERROR_PAGE_FAULT_GUARD_PAGE      equ 750
ERROR_PAGE_FAULT_PAGING_FILE     equ 751
ERROR_CACHE_PAGE_LOCKED          equ 752
ERROR_CRASH_DUMP                 equ 753
ERROR_BUFFER_ALL_ZEROS           equ 754
ERROR_REPARSE_OBJECT             equ 755
ERROR_RESOURCE_REQUIREMENTS_CHANGED equ 756
ERROR_TRANSLATION_COMPLETE       equ 757
ERROR_NOTHING_TO_TERMINATE       equ 758
ERROR_PROCESS_NOT_IN_JOB         equ 759
ERROR_PROCESS_IN_JOB             equ 760
ERROR_VOLSNAP_HIBERNATE_READY    equ 761
ERROR_FSFILTER_OP_COMPLETED_SUCCESSFULLY equ 762
ERROR_INTERRUPT_VECTOR_ALREADY_CONNECTED equ 763
ERROR_INTERRUPT_STILL_CONNECTED  equ 764
ERROR_WAIT_FOR_OPLOCK            equ 765
ERROR_DBG_EXCEPTION_HANDLED      equ 766
ERROR_DBG_CONTINUE               equ 767
ERROR_CALLBACK_POP_STACK         equ 768
ERROR_COMPRESSION_DISABLED       equ 769
ERROR_CANTFETCHBACKWARDS         equ 770
ERROR_CANTSCROLLBACKWARDS        equ 771
ERROR_ROWSNOTRELEASED            equ 772
ERROR_BAD_ACCESSOR_FLAGS         equ 773
ERROR_ERRORS_ENCOUNTERED         equ 774
ERROR_NOT_CAPABLE                equ 775
ERROR_REQUEST_OUT_OF_SEQUENCE    equ 776
ERROR_VERSION_PARSE_ERROR        equ 777
ERROR_BADSTARTPOSITION           equ 778
ERROR_MEMORY_HARDWARE            equ 779
ERROR_DISK_REPAIR_DISABLED       equ 780
ERROR_INSUFFICIENT_RESOURCE_FOR_SPECIFIED_SHARED_SECTION_SIZE equ 781
ERROR_SYSTEM_POWERSTATE_TRANSITION equ 782
ERROR_SYSTEM_POWERSTATE_COMPLEX_TRANSITION equ 783
ERROR_MCA_EXCEPTION              equ 784
ERROR_ACCESS_AUDIT_BY_POLICY     equ 785
ERROR_ACCESS_DISABLED_NO_SAFER_UI_BY_POLICY equ 786
ERROR_ABANDON_HIBERFILE          equ 787
ERROR_LOST_WRITEBEHIND_DATA_NETWORK_DISCONNECTED equ 788
ERROR_LOST_WRITEBEHIND_DATA_NETWORK_SERVER_ERROR equ 789
ERROR_LOST_WRITEBEHIND_DATA_LOCAL_DISK_ERROR equ 790
ERROR_BAD_MCFG_TABLE             equ 791
ERROR_DIFFERENT_SERVICE_ACCOUNT  equ 1079
ERROR_CANNOT_DETECT_DRIVER_FAILURE equ 1080
ERROR_CANNOT_DETECT_PROCESS_ABORT equ 1081
ERROR_NO_RECOVERY_PROGRAM        equ 1082
ERROR_SERVICE_NOT_IN_EXE         equ 1083
ERROR_NOT_SAFEBOOT_SERVICE       equ 1084
ERROR_SET_POWER_STATE_VETOED     equ 1140
ERROR_SET_POWER_STATE_FAILED     equ 1141
ERROR_TOO_MANY_LINKS             equ 1142
ERROR_APP_WRONG_OS               equ 1151
ERROR_RMODE_APP                  equ 1153
ERROR_INVALID_DLL                equ 1154
ERROR_NO_ASSOCIATION             equ 1155
ERROR_DDE_FAIL                   equ 1156
ERROR_DLL_NOT_FOUND              equ 1157
ERROR_NO_MORE_USER_HANDLES       equ 1158
ERROR_MESSAGE_SYNC_ONLY          equ 1159
ERROR_SOURCE_ELEMENT_EMPTY       equ 1160
ERROR_DESTINATION_ELEMENT_FULL   equ 1161
ERROR_ILLEGAL_ELEMENT_ADDRESS    equ 1162
ERROR_MAGAZINE_NOT_PRESENT       equ 1163
ERROR_DEVICE_REINITIALIZATION_NEEDED equ 1164
ERROR_DEVICE_REQUIRES_CLEANING   equ 1165
ERROR_DEVICE_DOOR_OPEN           equ 1166
ERROR_DEVICE_NOT_CONNECTED       equ 1167
ERROR_NOT_FOUND                  equ 1168
ERROR_NO_MATCH                   equ 1169
ERROR_SET_NOT_FOUND              equ 1170
ERROR_POINT_NOT_FOUND            equ 1171
ERROR_NO_TRACKING_SERVICE        equ 1172
ERROR_NO_VOLUME_ID               equ 1173
ERROR_UNABLE_TO_REMOVE_REPLACED  equ 1175
ERROR_UNABLE_TO_MOVE_REPLACEMENT equ 1176
ERROR_UNABLE_TO_MOVE_REPLACEMENT_2 equ 1177
ERROR_JOURNAL_DELETE_IN_PROGRESS equ 1178
ERROR_JOURNAL_NOT_ACTIVE         equ 1179
ERROR_POTENTIAL_FILE_FOUND       equ 1180
ERROR_JOURNAL_ENTRY_DELETED      equ 1181
ERROR_SHUTDOWN_IS_SCHEDULED      equ 1190
ERROR_SHUTDOWN_USERS_LOGGED_ON   equ 1191
ERROR_CANCELLED                  equ 1223
ERROR_USER_MAPPED_FILE           equ 1224
ERROR_CONNECTION_REFUSED         equ 1225
ERROR_GRACEFUL_DISCONNECT        equ 1226
ERROR_ADDRESS_ALREADY_ASSOCIATED equ 1227
ERROR_ADDRESS_NOT_ASSOCIATED     equ 1228
ERROR_CONNECTION_INVALID         equ 1229
ERROR_CONNECTION_ACTIVE          equ 1230
ERROR_NETWORK_UNREACHABLE        equ 1231
ERROR_HOST_UNREACHABLE           equ 1232
ERROR_PROTOCOL_UNREACHABLE       equ 1233
ERROR_PORT_UNREACHABLE           equ 1234
ERROR_REQUEST_ABORTED            equ 1235
ERROR_CONNECTION_ABORTED         equ 1236
ERROR_RETRY                      equ 1237
ERROR_CONNECTION_COUNT_LIMIT     equ 1238
ERROR_LOGIN_TIME_RESTRICTION     equ 1239
ERROR_LOGIN_WKSTA_RESTRICTION    equ 1240
ERROR_INCORRECT_ADDRESS          equ 1241
ERROR_ALREADY_REGISTERED         equ 1242
ERROR_SERVICE_NOT_FOUND          equ 1243
ERROR_NOT_AUTHENTICATED          equ 1244
ERROR_NOT_LOGGED_ON              equ 1245
ERROR_CONTINUE                   equ 1246
ERROR_ALREADY_INITIALIZED        equ 1247
ERROR_NO_MORE_DEVICES            equ 1248
ERROR_NO_SUCH_SITE               equ 1249
ERROR_DOMAIN_CONTROLLER_EXISTS   equ 1250
ERROR_ONLY_IF_CONNECTED          equ 1251
ERROR_OVERRIDE_NOCHANGES         equ 1252
ERROR_BAD_USER_PROFILE           equ 1253
ERROR_NOT_SUPPORTED_ON_SBS       equ 1254
ERROR_SERVER_SHUTDOWN_IN_PROGRESS equ 1255
ERROR_HOST_DOWN                  equ 1256
ERROR_NON_ACCOUNT_SID            equ 1257
ERROR_NON_DOMAIN_SID             equ 1258
ERROR_APPHELP_BLOCK              equ 1259
ERROR_ACCESS_DISABLED_BY_POLICY  equ 1260
ERROR_REG_NAT_CONSUMPTION        equ 1261
ERROR_CSCSHARE_OFFLINE           equ 1262
ERROR_PKINIT_FAILURE             equ 1263
ERROR_SMARTCARD_SUBSYSTEM_FAILURE equ 1264
ERROR_DOWNGRADE_DETECTED         equ 1265
ERROR_MACHINE_LOCKED             equ 1271
ERROR_CALLBACK_SUPPLIED_INVALID_DATA equ 1273
ERROR_SYNC_FOREGROUND_REFRESH_REQUIRED equ 1274
ERROR_DRIVER_BLOCKED             equ 1275
ERROR_INVALID_IMPORT_OF_NON_DLL  equ 1276
ERROR_ACCESS_DISABLED_WEBBLADE   equ 1277
ERROR_ACCESS_DISABLED_WEBBLADE_TAMPER equ 1278
ERROR_RECOVERY_FAILURE           equ 1279
ERROR_ALREADY_FIBER              equ 1280
ERROR_ALREADY_THREAD             equ 1281
ERROR_STACK_BUFFER_OVERRUN       equ 1282
ERROR_PARAMETER_QUOTA_EXCEEDED   equ 1283
ERROR_DEBUGGER_INACTIVE          equ 1284
ERROR_DELAY_LOAD_FAILED          equ 1285
ERROR_VDM_DISALLOWED             equ 1286
ERROR_UNIDENTIFIED_ERROR         equ 1287
ERROR_INVALID_CRUNTIME_PARAMETER equ 1288
ERROR_BEYOND_VDL                 equ 1289
ERROR_INCOMPATIBLE_SERVICE_SID_TYPE equ 1290
ERROR_DRIVER_PROCESS_TERMINATED  equ 1291
ERROR_IMPLEMENTATION_LIMIT       equ 1292
ERROR_PROCESS_IS_PROTECTED       equ 1293
ERROR_SERVICE_NOTIFY_CLIENT_LAGGING equ 1294
ERROR_DISK_QUOTA_EXCEEDED        equ 1295
ERROR_CONTENT_BLOCKED            equ 1296
ERROR_INCOMPATIBLE_SERVICE_PRIVILEGE equ 1297
ERROR_INVALID_LABEL              equ 1299
ERROR_LICENSE_QUOTA_EXCEEDED     equ 1395
ERROR_WRONG_TARGET_NAME          equ 1396
ERROR_MUTUAL_AUTH_FAILED         equ 1397
ERROR_TIME_SKEW                  equ 1398
ERROR_CURRENT_DOMAIN_NOT_ALLOWED equ 1399
ERROR_GLOBAL_ONLY_HOOK           equ 1429
ERROR_NO_SYSTEM_RESOURCES        equ 1450
ERROR_NONPAGED_SYSTEM_RESOURCES  equ 1451
ERROR_PAGED_SYSTEM_RESOURCES     equ 1452
ERROR_WORKING_SET_QUOTA          equ 1453
ERROR_PAGEFILE_QUOTA             equ 1454
ERROR_COMMITMENT_LIMIT           equ 1455
ERROR_MENU_ITEM_NOT_FOUND        equ 1456
ERROR_INVALID_KEYBOARD_HANDLE    equ 1457
ERROR_HOOK_TYPE_NOT_ALLOWED      equ 1458
ERROR_REQUIRES_INTERACTIVE_WINDOWSTATION equ 1459
ERROR_TIMEOUT                    equ 1460
ERROR_INVALID_MONITOR_HANDLE     equ 1461
ERROR_INCORRECT_SIZE             equ 1462
ERROR_SYMLINK_CLASS_DISABLED     equ 1463
ERROR_SYMLINK_NOT_SUPPORTED      equ 1464
ERROR_XML_PARSE_ERROR            equ 1465
ERROR_XMLDSIG_ERROR              equ 1466
ERROR_RESTART_APPLICATION        equ 1467
ERROR_WRONG_COMPARTMENT          equ 1468
ERROR_AUTHIP_FAILURE             equ 1469
ERROR_INVALID_TASK_NAME          equ 1550
ERROR_INVALID_TASK_INDEX         equ 1551
ERROR_THREAD_ALREADY_IN_TASK     equ 1552
ERROR_INSTALL_SERVICE_FAILURE    equ 1601
ERROR_INSTALL_USEREXIT           equ 1602
ERROR_INSTALL_FAILURE            equ 1603
ERROR_INSTALL_SUSPEND            equ 1604
ERROR_UNKNOWN_PRODUCT            equ 1605
ERROR_UNKNOWN_FEATURE            equ 1606
ERROR_UNKNOWN_COMPONENT          equ 1607
ERROR_UNKNOWN_PROPERTY           equ 1608
ERROR_INVALID_HANDLE_STATE       equ 1609
ERROR_BAD_CONFIGURATION          equ 1610
ERROR_INDEX_ABSENT               equ 1611
ERROR_INSTALL_SOURCE_ABSENT      equ 1612
ERROR_INSTALL_PACKAGE_VERSION    equ 1613
ERROR_PRODUCT_UNINSTALLED        equ 1614
ERROR_BAD_QUERY_SYNTAX           equ 1615
ERROR_INVALID_FIELD              equ 1616
ERROR_DEVICE_REMOVED             equ 1617
ERROR_INSTALL_ALREADY_RUNNING    equ 1618
ERROR_INSTALL_PACKAGE_OPEN_FAILED equ 1619
ERROR_INSTALL_PACKAGE_INVALID    equ 1620
ERROR_INSTALL_UI_FAILURE         equ 1621
ERROR_INSTALL_LOG_FAILURE        equ 1622
ERROR_INSTALL_LANGUAGE_UNSUPPORTED equ 1623
ERROR_INSTALL_TRANSFORM_FAILURE  equ 1624
ERROR_INSTALL_PACKAGE_REJECTED   equ 1625
ERROR_FUNCTION_NOT_CALLED        equ 1626
ERROR_FUNCTION_FAILED            equ 1627
ERROR_INVALID_TABLE              equ 1628
ERROR_DATATYPE_MISMATCH          equ 1629
ERROR_UNSUPPORTED_TYPE           equ 1630
ERROR_CREATE_FAILED              equ 1631
ERROR_INSTALL_TEMP_UNWRITABLE    equ 1632
ERROR_INSTALL_PLATFORM_UNSUPPORTED equ 1633
ERROR_INSTALL_NOTUSED            equ 1634
ERROR_PATCH_PACKAGE_OPEN_FAILED  equ 1635
ERROR_PATCH_PACKAGE_INVALID      equ 1636
ERROR_PATCH_PACKAGE_UNSUPPORTED  equ 1637
ERROR_PRODUCT_VERSION            equ 1638
ERROR_INVALID_COMMAND_LINE       equ 1639
ERROR_INSTALL_REMOTE_DISALLOWED  equ 1640
ERROR_SUCCESS_REBOOT_INITIATED   equ 1641
ERROR_PATCH_TARGET_NOT_FOUND     equ 1642
ERROR_PATCH_PACKAGE_REJECTED     equ 1643
ERROR_INSTALL_TRANSFORM_REJECTED equ 1644
ERROR_INSTALL_REMOTE_PROHIBITED  equ 1645
ERROR_PATCH_REMOVAL_UNSUPPORTED  equ 1646
ERROR_UNKNOWN_PATCH              equ 1647
ERROR_PATCH_NO_SEQUENCE          equ 1648
ERROR_PATCH_REMOVAL_DISALLOWED   equ 1649
ERROR_INVALID_PATCH_XML          equ 1650
ERROR_PATCH_MANAGED_ADVERTISED_PRODUCT equ 1651
ERROR_INSTALL_SERVICE_SAFEBOOT   equ 1652
RPC_S_PROXY_ACCESS_DENIED        equ 1729
RPC_S_NO_INTERFACES              equ 1817
RPC_S_CALL_CANCELLED             equ 1818
RPC_S_BINDING_INCOMPLETE         equ 1819
RPC_S_COMM_FAILURE               equ 1820
RPC_S_UNSUPPORTED_AUTHN_LEVEL    equ 1821
RPC_S_NO_PRINC_NAME              equ 1822
RPC_S_NOT_RPC_ERROR              equ 1823
RPC_S_UUID_LOCAL_ONLY            equ 1824
RPC_S_SEC_PKG_ERROR              equ 1825
RPC_S_NOT_CANCELLED              equ 1826
RPC_X_INVALID_ES_ACTION          equ 1827
RPC_X_WRONG_ES_VERSION           equ 1828
RPC_X_WRONG_STUB_VERSION         equ 1829
RPC_X_INVALID_PIPE_OBJECT        equ 1830
RPC_X_WRONG_PIPE_ORDER           equ 1831
RPC_X_WRONG_PIPE_VERSION         equ 1832
ERROR_PASSWORD_MUST_CHANGE       equ 1907
ERROR_DOMAIN_CONTROLLER_NOT_FOUND equ 1908
ERROR_ACCOUNT_LOCKED_OUT         equ 1909
OR_INVALID_OXID                  equ 1910
OR_INVALID_OID                   equ 1911
OR_INVALID_SET                   equ 1912
RPC_S_SEND_INCOMPLETE            equ 1913
RPC_S_INVALID_ASYNC_HANDLE       equ 1914
RPC_S_INVALID_ASYNC_CALL         equ 1915
RPC_X_PIPE_CLOSED                equ 1916
RPC_X_PIPE_DISCIPLINE_ERROR      equ 1917
RPC_X_PIPE_EMPTY                 equ 1918
ERROR_NO_SITENAME                equ 1919
ERROR_CANT_ACCESS_FILE           equ 1920
ERROR_CANT_RESOLVE_FILENAME      equ 1921
RPC_S_ENTRY_TYPE_MISMATCH        equ 1922
RPC_S_NOT_ALL_OBJS_EXPORTED      equ 1923
RPC_S_INTERFACE_NOT_EXPORTED     equ 1924
RPC_S_PROFILE_NOT_ADDED          equ 1925
RPC_S_PRF_ELT_NOT_ADDED          equ 1926
RPC_S_PRF_ELT_NOT_REMOVED        equ 1927
RPC_S_GRP_ELT_NOT_ADDED          equ 1928
RPC_S_GRP_ELT_NOT_REMOVED        equ 1929
ERROR_KM_DRIVER_BLOCKED          equ 1930
ERROR_CONTEXT_EXPIRED            equ 1931
ERROR_PER_USER_TRUST_QUOTA_EXCEEDED equ 1932
ERROR_ALL_USER_TRUST_QUOTA_EXCEEDED equ 1933
ERROR_USER_DELETE_TRUST_QUOTA_EXCEEDED equ 1934
ERROR_AUTHENTICATION_FIREWALL_FAILED equ 1935
ERROR_REMOTE_PRINT_CONNECTIONS_BLOCKED equ 1936
ERROR_NTLM_BLOCKED               equ 1937
ERROR_INVALID_CMM                equ 2010
ERROR_INVALID_PROFILE            equ 2011
ERROR_TAG_NOT_FOUND              equ 2012
ERROR_TAG_NOT_PRESENT            equ 2013
ERROR_DUPLICATE_TAG              equ 2014
ERROR_PROFILE_NOT_ASSOCIATED_WITH_DEVICE equ 2015
ERROR_PROFILE_NOT_FOUND          equ 2016
ERROR_INVALID_COLORSPACE         equ 2017
ERROR_ICM_NOT_ENABLED            equ 2018
ERROR_DELETING_ICM_XFORM         equ 2019
ERROR_INVALID_TRANSFORM          equ 2020
ERROR_COLORSPACE_MISMATCH        equ 2021
ERROR_INVALID_COLORINDEX         equ 2022
ERROR_PROFILE_DOES_NOT_MATCH_DEVICE equ 2023
ERROR_CONNECTED_OTHER_PASSWORD   equ 2108
ERROR_CONNECTED_OTHER_PASSWORD_DEFAULT equ 2109
ERROR_ACTIVE_CONNECTIONS         equ 2402
ERROR_INVALID_PRINT_MONITOR      equ 3007
ERROR_PRINT_MONITOR_IN_USE       equ 3008
ERROR_PRINTER_HAS_JOBS_QUEUED    equ 3009
ERROR_SUCCESS_REBOOT_REQUIRED    equ 3010
ERROR_SUCCESS_RESTART_REQUIRED   equ 3011
ERROR_PRINTER_NOT_FOUND          equ 3012
ERROR_PRINTER_DRIVER_WARNED      equ 3013
ERROR_PRINTER_DRIVER_BLOCKED     equ 3014
ERROR_PRINTER_DRIVER_PACKAGE_IN_USE equ 3015
ERROR_CORE_DRIVER_PACKAGE_NOT_FOUND equ 3016
ERROR_FAIL_REBOOT_REQUIRED       equ 3017
ERROR_FAIL_REBOOT_INITIATED      equ 3018
ERROR_PRINTER_DRIVER_DOWNLOAD_NEEDED equ 3019
ERROR_PRINT_JOB_RESTART_REQUIRED equ 3020
ERROR_IO_REISSUE_AS_CACHED       equ 3950
ERROR_DHCP_ADDRESS_CONFLICT      equ 4100
ERROR_WMI_GUID_NOT_FOUND         equ 4200
ERROR_WMI_INSTANCE_NOT_FOUND     equ 4201
ERROR_WMI_ITEMID_NOT_FOUND       equ 4202
ERROR_WMI_TRY_AGAIN              equ 4203
ERROR_WMI_DP_NOT_FOUND           equ 4204
ERROR_WMI_UNRESOLVED_INSTANCE_REF equ 4205
ERROR_WMI_ALREADY_ENABLED        equ 4206
ERROR_WMI_GUID_DISCONNECTED      equ 4207
ERROR_WMI_SERVER_UNAVAILABLE     equ 4208
ERROR_WMI_DP_FAILED              equ 4209
ERROR_WMI_INVALID_MOF            equ 4210
ERROR_WMI_INVALID_REGINFO        equ 4211
ERROR_WMI_ALREADY_DISABLED       equ 4212
ERROR_WMI_READ_ONLY              equ 4213
ERROR_WMI_SET_FAILURE            equ 4214
ERROR_INVALID_MEDIA              equ 4300
ERROR_INVALID_LIBRARY            equ 4301
ERROR_INVALID_MEDIA_POOL         equ 4302
ERROR_DRIVE_MEDIA_MISMATCH       equ 4303
ERROR_MEDIA_OFFLINE              equ 4304
ERROR_LIBRARY_OFFLINE            equ 4305
ERROR_EMPTY                      equ 4306
ERROR_NOT_EMPTY                  equ 4307
ERROR_MEDIA_UNAVAILABLE          equ 4308
ERROR_RESOURCE_DISABLED          equ 4309
ERROR_INVALID_CLEANER            equ 4310
ERROR_UNABLE_TO_CLEAN            equ 4311
ERROR_OBJECT_NOT_FOUND           equ 4312
ERROR_DATABASE_FAILURE           equ 4313
ERROR_DATABASE_FULL              equ 4314
ERROR_MEDIA_INCOMPATIBLE         equ 4315
ERROR_RESOURCE_NOT_PRESENT       equ 4316
ERROR_INVALID_OPERATION          equ 4317
ERROR_MEDIA_NOT_AVAILABLE        equ 4318
ERROR_DEVICE_NOT_AVAILABLE       equ 4319
ERROR_REQUEST_REFUSED            equ 4320
ERROR_INVALID_DRIVE_OBJECT       equ 4321
ERROR_LIBRARY_FULL               equ 4322
ERROR_MEDIUM_NOT_ACCESSIBLE      equ 4323
ERROR_UNABLE_TO_LOAD_MEDIUM      equ 4324
ERROR_UNABLE_TO_INVENTORY_DRIVE  equ 4325
ERROR_UNABLE_TO_INVENTORY_SLOT   equ 4326
ERROR_UNABLE_TO_INVENTORY_TRANSPORT equ 4327
ERROR_TRANSPORT_FULL             equ 4328
ERROR_CONTROLLING_IEPORT         equ 4329
ERROR_UNABLE_TO_EJECT_MOUNTED_MEDIA equ 4330
ERROR_CLEANER_SLOT_SET           equ 4331
ERROR_CLEANER_SLOT_NOT_SET       equ 4332
ERROR_CLEANER_CARTRIDGE_SPENT    equ 4333
ERROR_UNEXPECTED_OMID            equ 4334
ERROR_CANT_DELETE_LAST_ITEM      equ 4335
ERROR_MESSAGE_EXCEEDS_MAX_SIZE   equ 4336
ERROR_VOLUME_CONTAINS_SYS_FILES  equ 4337
ERROR_INDIGENOUS_TYPE            equ 4338
ERROR_NO_SUPPORTING_DRIVES       equ 4339
ERROR_CLEANER_CARTRIDGE_INSTALLED equ 4340
ERROR_IEPORT_FULL                equ 4341
ERROR_FILE_OFFLINE               equ 4350
ERROR_REMOTE_STORAGE_NOT_ACTIVE  equ 4351
ERROR_REMOTE_STORAGE_MEDIA_ERROR equ 4352
ERROR_NOT_A_REPARSE_POINT        equ 4390
ERROR_REPARSE_ATTRIBUTE_CONFLICT equ 4391
ERROR_INVALID_REPARSE_DATA       equ 4392
ERROR_REPARSE_TAG_INVALID        equ 4393
ERROR_REPARSE_TAG_MISMATCH       equ 4394
ERROR_VOLUME_NOT_SIS_ENABLED     equ 4500
ERROR_DEPENDENT_RESOURCE_EXISTS  equ 5001
ERROR_DEPENDENCY_NOT_FOUND       equ 5002
ERROR_DEPENDENCY_ALREADY_EXISTS  equ 5003
ERROR_RESOURCE_NOT_ONLINE        equ 5004
ERROR_HOST_NODE_NOT_AVAILABLE    equ 5005
ERROR_RESOURCE_NOT_AVAILABLE     equ 5006
ERROR_RESOURCE_NOT_FOUND         equ 5007
ERROR_SHUTDOWN_CLUSTER           equ 5008
ERROR_CANT_EVICT_ACTIVE_NODE     equ 5009
ERROR_OBJECT_ALREADY_EXISTS      equ 5010
ERROR_OBJECT_IN_LIST             equ 5011
ERROR_GROUP_NOT_AVAILABLE        equ 5012
ERROR_GROUP_NOT_FOUND            equ 5013
ERROR_GROUP_NOT_ONLINE           equ 5014
ERROR_HOST_NODE_NOT_RESOURCE_OWNER equ 5015
ERROR_HOST_NODE_NOT_GROUP_OWNER  equ 5016
ERROR_RESMON_CREATE_FAILED       equ 5017
ERROR_RESMON_ONLINE_FAILED       equ 5018
ERROR_RESOURCE_ONLINE            equ 5019
ERROR_QUORUM_RESOURCE            equ 5020
ERROR_NOT_QUORUM_CAPABLE         equ 5021
ERROR_CLUSTER_SHUTTING_DOWN      equ 5022
ERROR_INVALID_STATE              equ 5023
ERROR_RESOURCE_PROPERTIES_STORED equ 5024
ERROR_NOT_QUORUM_CLASS           equ 5025
ERROR_CORE_RESOURCE              equ 5026
ERROR_QUORUM_RESOURCE_ONLINE_FAILED equ 5027
ERROR_QUORUMLOG_OPEN_FAILED      equ 5028
ERROR_CLUSTERLOG_CORRUPT         equ 5029
ERROR_CLUSTERLOG_RECORD_EXCEEDS_MAXSIZE equ 5030
ERROR_CLUSTERLOG_EXCEEDS_MAXSIZE equ 5031
ERROR_CLUSTERLOG_CHKPOINT_NOT_FOUND equ 5032
ERROR_CLUSTERLOG_NOT_ENOUGH_SPACE equ 5033
ERROR_QUORUM_OWNER_ALIVE         equ 5034
ERROR_NETWORK_NOT_AVAILABLE      equ 5035
ERROR_NODE_NOT_AVAILABLE         equ 5036
ERROR_ALL_NODES_NOT_AVAILABLE    equ 5037
ERROR_RESOURCE_FAILED            equ 5038
ERROR_CLUSTER_INVALID_NODE       equ 5039
ERROR_CLUSTER_NODE_EXISTS        equ 5040
ERROR_CLUSTER_JOIN_IN_PROGRESS   equ 5041
ERROR_CLUSTER_NODE_NOT_FOUND     equ 5042
ERROR_CLUSTER_LOCAL_NODE_NOT_FOUND equ 5043
ERROR_CLUSTER_NETWORK_EXISTS     equ 5044
ERROR_CLUSTER_NETWORK_NOT_FOUND  equ 5045
ERROR_CLUSTER_NETINTERFACE_EXISTS equ 5046
ERROR_CLUSTER_NETINTERFACE_NOT_FOUND equ 5047
ERROR_CLUSTER_INVALID_REQUEST    equ 5048
ERROR_CLUSTER_INVALID_NETWORK_PROVIDER equ 5049
ERROR_CLUSTER_NODE_DOWN          equ 5050
ERROR_CLUSTER_NODE_UNREACHABLE   equ 5051
ERROR_CLUSTER_NODE_NOT_MEMBER    equ 5052
ERROR_CLUSTER_JOIN_NOT_IN_PROGRESS equ 5053
ERROR_CLUSTER_INVALID_NETWORK    equ 5054
ERROR_CLUSTER_NODE_UP            equ 5056
ERROR_CLUSTER_IPADDR_IN_USE      equ 5057
ERROR_CLUSTER_NODE_NOT_PAUSED    equ 5058
ERROR_CLUSTER_NO_SECURITY_CONTEXT equ 5059
ERROR_CLUSTER_NETWORK_NOT_INTERNAL equ 5060
ERROR_CLUSTER_NODE_ALREADY_UP    equ 5061
ERROR_CLUSTER_NODE_ALREADY_DOWN  equ 5062
ERROR_CLUSTER_NETWORK_ALREADY_ONLINE equ 5063
ERROR_CLUSTER_NETWORK_ALREADY_OFFLINE equ 5064
ERROR_CLUSTER_NODE_ALREADY_MEMBER equ 5065
ERROR_CLUSTER_LAST_INTERNAL_NETWORK equ 5066
ERROR_CLUSTER_NETWORK_HAS_DEPENDENTS equ 5067
ERROR_INVALID_OPERATION_ON_QUORUM equ 5068
ERROR_DEPENDENCY_NOT_ALLOWED     equ 5069
ERROR_CLUSTER_NODE_PAUSED        equ 5070
ERROR_NODE_CANT_HOST_RESOURCE    equ 5071
ERROR_CLUSTER_NODE_NOT_READY     equ 5072
ERROR_CLUSTER_NODE_SHUTTING_DOWN equ 5073
ERROR_CLUSTER_JOIN_ABORTED       equ 5074
ERROR_CLUSTER_INCOMPATIBLE_VERSIONS equ 5075
ERROR_CLUSTER_MAXNUM_OF_RESOURCES_EXCEEDED equ 5076
ERROR_CLUSTER_SYSTEM_CONFIG_CHANGED equ 5077
ERROR_CLUSTER_RESOURCE_TYPE_NOT_FOUND equ 5078
ERROR_CLUSTER_RESTYPE_NOT_SUPPORTED equ 5079
ERROR_CLUSTER_RESNAME_NOT_FOUND  equ 5080
ERROR_CLUSTER_NO_RPC_PACKAGES_REGISTERED equ 5081
ERROR_CLUSTER_OWNER_NOT_IN_PREFLIST equ 5082
ERROR_CLUSTER_DATABASE_SEQMISMATCH equ 5083
ERROR_RESMON_INVALID_STATE       equ 5084
ERROR_CLUSTER_GUM_NOT_LOCKER     equ 5085
ERROR_QUORUM_DISK_NOT_FOUND      equ 5086
ERROR_DATABASE_BACKUP_CORRUPT    equ 5087
ERROR_CLUSTER_NODE_ALREADY_HAS_DFS_ROOT equ 5088
ERROR_RESOURCE_PROPERTY_UNCHANGEABLE equ 5089
ERROR_CLUSTER_MEMBERSHIP_INVALID_STATE equ 5890
ERROR_CLUSTER_QUORUMLOG_NOT_FOUND equ 5891
ERROR_CLUSTER_MEMBERSHIP_HALT    equ 5892
ERROR_CLUSTER_INSTANCE_ID_MISMATCH equ 5893
ERROR_CLUSTER_NETWORK_NOT_FOUND_FOR_IP equ 5894
ERROR_CLUSTER_PROPERTY_DATA_TYPE_MISMATCH equ 5895
ERROR_CLUSTER_EVICT_WITHOUT_CLEANUP equ 5896
ERROR_CLUSTER_PARAMETER_MISMATCH equ 5897
ERROR_NODE_CANNOT_BE_CLUSTERED   equ 5898
ERROR_CLUSTER_WRONG_OS_VERSION   equ 5899
ERROR_CLUSTER_CANT_CREATE_DUP_CLUSTER_NAME equ 5900
ERROR_CLUSCFG_ALREADY_COMMITTED  equ 5901
ERROR_CLUSCFG_ROLLBACK_FAILED    equ 5902
ERROR_CLUSCFG_SYSTEM_DISK_DRIVE_LETTER_CONFLICT equ 5903
ERROR_CLUSTER_OLD_VERSION        equ 5904
ERROR_CLUSTER_MISMATCHED_COMPUTER_ACCT_NAME equ 5905
ERROR_CLUSTER_NO_NET_ADAPTERS    equ 5906
ERROR_CLUSTER_POISONED           equ 5907
ERROR_CLUSTER_GROUP_MOVING       equ 5908
ERROR_CLUSTER_RESOURCE_TYPE_BUSY equ 5909
ERROR_RESOURCE_CALL_TIMED_OUT    equ 5910
ERROR_INVALID_CLUSTER_IPV6_ADDRESS equ 5911
ERROR_CLUSTER_INTERNAL_INVALID_FUNCTION equ 5912
ERROR_CLUSTER_PARAMETER_OUT_OF_BOUNDS equ 5913
ERROR_CLUSTER_PARTIAL_SEND       equ 5914
ERROR_CLUSTER_REGISTRY_INVALID_FUNCTION equ 5915
ERROR_CLUSTER_INVALID_STRING_TERMINATION equ 5916
ERROR_CLUSTER_INVALID_STRING_FORMAT equ 5917
ERROR_CLUSTER_DATABASE_TRANSACTION_IN_PROGRESS equ 5918
ERROR_CLUSTER_DATABASE_TRANSACTION_NOT_IN_PROGRESS equ 5919
ERROR_CLUSTER_NULL_DATA          equ 5920
ERROR_CLUSTER_PARTIAL_READ       equ 5921
ERROR_CLUSTER_PARTIAL_WRITE      equ 5922
ERROR_CLUSTER_CANT_DESERIALIZE_DATA equ 5923
ERROR_DEPENDENT_RESOURCE_PROPERTY_CONFLICT equ 5924
ERROR_CLUSTER_NO_QUORUM          equ 5925
ERROR_CLUSTER_INVALID_IPV6_NETWORK equ 5926
ERROR_CLUSTER_INVALID_IPV6_TUNNEL_NETWORK equ 5927
ERROR_QUORUM_NOT_ALLOWED_IN_THIS_GROUP equ 5928
ERROR_DEPENDENCY_TREE_TOO_COMPLEX equ 5929
ERROR_EXCEPTION_IN_RESOURCE_CALL equ 5930
ERROR_CLUSTER_RHS_FAILED_INITIALIZATION equ 5931
ERROR_CLUSTER_NOT_INSTALLED      equ 5932
ERROR_CLUSTER_RESOURCES_MUST_BE_ONLINE_ON_THE_SAME_NODE equ 5933
ERROR_ENCRYPTION_FAILED          equ 6000
ERROR_DECRYPTION_FAILED          equ 6001
ERROR_FILE_ENCRYPTED             equ 6002
ERROR_NO_RECOVERY_POLICY         equ 6003
ERROR_NO_EFS                     equ 6004
ERROR_WRONG_EFS                  equ 6005
ERROR_NO_USER_KEYS               equ 6006
ERROR_FILE_NOT_ENCRYPTED         equ 6007
ERROR_NOT_EXPORT_FORMAT          equ 6008
ERROR_FILE_READ_ONLY             equ 6009
ERROR_DIR_EFS_DISALLOWED         equ 6010
ERROR_EFS_SERVER_NOT_TRUSTED     equ 6011
ERROR_BAD_RECOVERY_POLICY        equ 6012
ERROR_EFS_ALG_BLOB_TOO_BIG       equ 6013
ERROR_VOLUME_NOT_SUPPORT_EFS     equ 6014
ERROR_EFS_DISABLED               equ 6015
ERROR_EFS_VERSION_NOT_SUPPORT    equ 6016
ERROR_CS_ENCRYPTION_INVALID_SERVER_RESPONSE equ 6017
ERROR_CS_ENCRYPTION_UNSUPPORTED_SERVER equ 6018
ERROR_CS_ENCRYPTION_EXISTING_ENCRYPTED_FILE equ 6019
ERROR_CS_ENCRYPTION_NEW_ENCRYPTED_FILE equ 6020
ERROR_CS_ENCRYPTION_FILE_NOT_CSE equ 6021
SCHED_E_SERVICE_NOT_LOCALSYSTEM  equ 6200
ERROR_LOG_SECTOR_INVALID         equ 6600
ERROR_LOG_SECTOR_PARITY_INVALID  equ 6601
ERROR_LOG_SECTOR_REMAPPED        equ 6602
ERROR_LOG_BLOCK_INCOMPLETE       equ 6603
ERROR_LOG_INVALID_RANGE          equ 6604
ERROR_LOG_BLOCKS_EXHAUSTED       equ 6605
ERROR_LOG_READ_CONTEXT_INVALID   equ 6606
ERROR_LOG_RESTART_INVALID        equ 6607
ERROR_LOG_BLOCK_VERSION          equ 6608
ERROR_LOG_BLOCK_INVALID          equ 6609
ERROR_LOG_READ_MODE_INVALID      equ 6610
ERROR_LOG_NO_RESTART             equ 6611
ERROR_LOG_METADATA_CORRUPT       equ 6612
ERROR_LOG_METADATA_INVALID       equ 6613
ERROR_LOG_METADATA_INCONSISTENT  equ 6614
ERROR_LOG_RESERVATION_INVALID    equ 6615
ERROR_LOG_CANT_DELETE            equ 6616
ERROR_LOG_CONTAINER_LIMIT_EXCEEDED equ 6617
ERROR_LOG_START_OF_LOG           equ 6618
ERROR_LOG_POLICY_ALREADY_INSTALLED equ 6619
ERROR_LOG_POLICY_NOT_INSTALLED   equ 6620
ERROR_LOG_POLICY_INVALID         equ 6621
ERROR_LOG_POLICY_CONFLICT        equ 6622
ERROR_LOG_PINNED_ARCHIVE_TAIL    equ 6623
ERROR_LOG_RECORD_NONEXISTENT     equ 6624
ERROR_LOG_RECORDS_RESERVED_INVALID equ 6625
ERROR_LOG_SPACE_RESERVED_INVALID equ 6626
ERROR_LOG_TAIL_INVALID           equ 6627
ERROR_LOG_FULL                   equ 6628
ERROR_COULD_NOT_RESIZE_LOG       equ 6629
ERROR_LOG_MULTIPLEXED            equ 6630
ERROR_LOG_DEDICATED              equ 6631
ERROR_LOG_ARCHIVE_NOT_IN_PROGRESS equ 6632
ERROR_LOG_ARCHIVE_IN_PROGRESS    equ 6633
ERROR_LOG_EPHEMERAL              equ 6634
ERROR_LOG_NOT_ENOUGH_CONTAINERS  equ 6635
ERROR_LOG_CLIENT_ALREADY_REGISTERED equ 6636
ERROR_LOG_CLIENT_NOT_REGISTERED  equ 6637
ERROR_LOG_FULL_HANDLER_IN_PROGRESS equ 6638
ERROR_LOG_CONTAINER_READ_FAILED  equ 6639
ERROR_LOG_CONTAINER_WRITE_FAILED equ 6640
ERROR_LOG_CONTAINER_OPEN_FAILED  equ 6641
ERROR_LOG_CONTAINER_STATE_INVALID equ 6642
ERROR_LOG_STATE_INVALID          equ 6643
ERROR_LOG_PINNED                 equ 6644
ERROR_LOG_METADATA_FLUSH_FAILED  equ 6645
ERROR_LOG_INCONSISTENT_SECURITY  equ 6646
ERROR_LOG_APPENDED_FLUSH_FAILED  equ 6647
ERROR_LOG_PINNED_RESERVATION     equ 6648
ERROR_INVALID_TRANSACTION        equ 6700
ERROR_TRANSACTION_NOT_ACTIVE     equ 6701
ERROR_TRANSACTION_REQUEST_NOT_VALID equ 6702
ERROR_TRANSACTION_NOT_REQUESTED  equ 6703
ERROR_TRANSACTION_ALREADY_ABORTED equ 6704
ERROR_TRANSACTION_ALREADY_COMMITTED equ 6705
ERROR_TM_INITIALIZATION_FAILED   equ 6706
ERROR_RESOURCEMANAGER_READ_ONLY  equ 6707
ERROR_TRANSACTION_NOT_JOINED     equ 6708
ERROR_TRANSACTION_SUPERIOR_EXISTS equ 6709
ERROR_CRM_PROTOCOL_ALREADY_EXISTS equ 6710
ERROR_TRANSACTION_PROPAGATION_FAILED equ 6711
ERROR_CRM_PROTOCOL_NOT_FOUND     equ 6712
ERROR_TRANSACTION_INVALID_MARSHALL_BUFFER equ 6713
ERROR_CURRENT_TRANSACTION_NOT_VALID equ 6714
ERROR_TRANSACTION_NOT_FOUND      equ 6715
ERROR_RESOURCEMANAGER_NOT_FOUND  equ 6716
ERROR_ENLISTMENT_NOT_FOUND       equ 6717
ERROR_TRANSACTIONMANAGER_NOT_FOUND equ 6718
ERROR_TRANSACTIONMANAGER_NOT_ONLINE equ 6719
ERROR_TRANSACTIONMANAGER_RECOVERY_NAME_COLLISION equ 6720
ERROR_TRANSACTION_NOT_ROOT       equ 6721
ERROR_TRANSACTION_OBJECT_EXPIRED equ 6722
ERROR_TRANSACTION_RESPONSE_NOT_ENLISTED equ 6723
ERROR_TRANSACTION_RECORD_TOO_LONG equ 6724
ERROR_IMPLICIT_TRANSACTION_NOT_SUPPORTED equ 6725
ERROR_TRANSACTION_INTEGRITY_VIOLATED equ 6726
ERROR_TRANSACTIONAL_CONFLICT     equ 6800
ERROR_RM_NOT_ACTIVE              equ 6801
ERROR_RM_METADATA_CORRUPT        equ 6802
ERROR_DIRECTORY_NOT_RM           equ 6803
ERROR_TRANSACTIONS_UNSUPPORTED_REMOTE equ 6805
ERROR_LOG_RESIZE_INVALID_SIZE    equ 6806
ERROR_OBJECT_NO_LONGER_EXISTS    equ 6807
ERROR_STREAM_MINIVERSION_NOT_FOUND equ 6808
ERROR_STREAM_MINIVERSION_NOT_VALID equ 6809
ERROR_MINIVERSION_INACCESSIBLE_FROM_SPECIFIED_TRANSACTION equ 6810
ERROR_CANT_OPEN_MINIVERSION_WITH_MODIFY_INTENT equ 6811
ERROR_CANT_CREATE_MORE_STREAM_MINIVERSIONS equ 6812
ERROR_REMOTE_FILE_VERSION_MISMATCH equ 6814
ERROR_HANDLE_NO_LONGER_VALID     equ 6815
ERROR_NO_TXF_METADATA            equ 6816
ERROR_LOG_CORRUPTION_DETECTED    equ 6817
ERROR_CANT_RECOVER_WITH_HANDLE_OPEN equ 6818
ERROR_RM_DISCONNECTED            equ 6819
ERROR_ENLISTMENT_NOT_SUPERIOR    equ 6820
ERROR_RECOVERY_NOT_NEEDED        equ 6821
ERROR_RM_ALREADY_STARTED         equ 6822
ERROR_FILE_IDENTITY_NOT_PERSISTENT equ 6823
ERROR_CANT_BREAK_TRANSACTIONAL_DEPENDENCY equ 6824
ERROR_CANT_CROSS_RM_BOUNDARY     equ 6825
ERROR_TXF_DIR_NOT_EMPTY          equ 6826
ERROR_INDOUBT_TRANSACTIONS_EXIST equ 6827
ERROR_TM_VOLATILE                equ 6828
ERROR_ROLLBACK_TIMER_EXPIRED     equ 6829
ERROR_TXF_ATTRIBUTE_CORRUPT      equ 6830
ERROR_EFS_NOT_ALLOWED_IN_TRANSACTION equ 6831
ERROR_TRANSACTIONAL_OPEN_NOT_ALLOWED equ 6832
ERROR_LOG_GROWTH_FAILED          equ 6833
ERROR_TRANSACTED_MAPPING_UNSUPPORTED_REMOTE equ 6834
ERROR_TXF_METADATA_ALREADY_PRESENT equ 6835
ERROR_TRANSACTION_SCOPE_CALLBACKS_NOT_SET equ 6836
ERROR_TRANSACTION_REQUIRED_PROMOTION equ 6837
ERROR_CANNOT_EXECUTE_FILE_IN_TRANSACTION equ 6838
ERROR_TRANSACTIONS_NOT_FROZEN    equ 6839
ERROR_TRANSACTION_FREEZE_IN_PROGRESS equ 6840
ERROR_NOT_SNAPSHOT_VOLUME        equ 6841
ERROR_NO_SAVEPOINT_WITH_OPEN_FILES equ 6842
ERROR_DATA_LOST_REPAIR           equ 6843
ERROR_SPARSE_NOT_ALLOWED_IN_TRANSACTION equ 6844
ERROR_TM_IDENTITY_MISMATCH       equ 6845
ERROR_FLOATED_SECTION            equ 6846
ERROR_CANNOT_ACCEPT_TRANSACTED_WORK equ 6847
ERROR_CANNOT_ABORT_TRANSACTIONS  equ 6848
ERROR_BAD_CLUSTERS               equ 6849
ERROR_COMPRESSION_NOT_ALLOWED_IN_TRANSACTION equ 6850
ERROR_VOLUME_DIRTY               equ 6851
ERROR_NO_LINK_TRACKING_IN_TRANSACTION equ 6852
ERROR_OPERATION_NOT_SUPPORTED_IN_TRANSACTION equ 6853
ERROR_CTX_WINSTATION_NAME_INVALID equ 7001
ERROR_CTX_INVALID_PD             equ 7002
ERROR_CTX_PD_NOT_FOUND           equ 7003
ERROR_CTX_WD_NOT_FOUND           equ 7004
ERROR_CTX_CANNOT_MAKE_EVENTLOG_ENTRY equ 7005
ERROR_CTX_SERVICE_NAME_COLLISION equ 7006
ERROR_CTX_CLOSE_PENDING          equ 7007
ERROR_CTX_NO_OUTBUF              equ 7008
ERROR_CTX_MODEM_INF_NOT_FOUND    equ 7009
ERROR_CTX_INVALID_MODEMNAME      equ 7010
ERROR_CTX_MODEM_RESPONSE_ERROR   equ 7011
ERROR_CTX_MODEM_RESPONSE_TIMEOUT equ 7012
ERROR_CTX_MODEM_RESPONSE_NO_CARRIER equ 7013
ERROR_CTX_MODEM_RESPONSE_NO_DIALTONE equ 7014
ERROR_CTX_MODEM_RESPONSE_BUSY    equ 7015
ERROR_CTX_MODEM_RESPONSE_VOICE   equ 7016
ERROR_CTX_TD_ERROR               equ 7017
ERROR_CTX_WINSTATION_NOT_FOUND   equ 7022
ERROR_CTX_WINSTATION_ALREADY_EXISTS equ 7023
ERROR_CTX_WINSTATION_BUSY        equ 7024
ERROR_CTX_BAD_VIDEO_MODE         equ 7025
ERROR_CTX_GRAPHICS_INVALID       equ 7035
ERROR_CTX_LOGON_DISABLED         equ 7037
ERROR_CTX_NOT_CONSOLE            equ 7038
ERROR_CTX_CLIENT_QUERY_TIMEOUT   equ 7040
ERROR_CTX_CONSOLE_DISCONNECT     equ 7041
ERROR_CTX_CONSOLE_CONNECT        equ 7042
ERROR_CTX_SHADOW_DENIED          equ 7044
ERROR_CTX_WINSTATION_ACCESS_DENIED equ 7045
ERROR_CTX_INVALID_WD             equ 7049
ERROR_CTX_SHADOW_INVALID         equ 7050
ERROR_CTX_SHADOW_DISABLED        equ 7051
ERROR_CTX_CLIENT_LICENSE_IN_USE  equ 7052
ERROR_CTX_CLIENT_LICENSE_NOT_SET equ 7053
ERROR_CTX_LICENSE_NOT_AVAILABLE  equ 7054
ERROR_CTX_LICENSE_CLIENT_INVALID equ 7055
ERROR_CTX_LICENSE_EXPIRED        equ 7056
ERROR_CTX_SHADOW_NOT_RUNNING     equ 7057
ERROR_CTX_SHADOW_ENDED_BY_MODE_CHANGE equ 7058
ERROR_ACTIVATION_COUNT_EXCEEDED  equ 7059
ERROR_CTX_WINSTATIONS_DISABLED   equ 7060
ERROR_CTX_ENCRYPTION_LEVEL_REQUIRED equ 7061
ERROR_CTX_SESSION_IN_USE         equ 7062
ERROR_CTX_NO_FORCE_LOGOFF        equ 7063
ERROR_CTX_ACCOUNT_RESTRICTION    equ 7064
ERROR_RDP_PROTOCOL_ERROR         equ 7065
ERROR_CTX_CDM_CONNECT            equ 7066
ERROR_CTX_CDM_DISCONNECT         equ 7067
ERROR_CTX_SECURITY_LAYER_ERROR   equ 7068
ERROR_TS_INCOMPATIBLE_SESSIONS   equ 7069
FRS_ERR_INVALID_API_SEQUENCE     equ 8001
FRS_ERR_STARTING_SERVICE         equ 8002
FRS_ERR_STOPPING_SERVICE         equ 8003
FRS_ERR_INTERNAL_API             equ 8004
FRS_ERR_INTERNAL                 equ 8005
FRS_ERR_SERVICE_COMM             equ 8006
FRS_ERR_INSUFFICIENT_PRIV        equ 8007
FRS_ERR_AUTHENTICATION           equ 8008
FRS_ERR_PARENT_INSUFFICIENT_PRIV equ 8009
FRS_ERR_PARENT_AUTHENTICATION    equ 8010
FRS_ERR_CHILD_TO_PARENT_COMM     equ 8011
FRS_ERR_PARENT_TO_CHILD_COMM     equ 8012
FRS_ERR_SYSVOL_POPULATE          equ 8013
FRS_ERR_SYSVOL_POPULATE_TIMEOUT  equ 8014
FRS_ERR_SYSVOL_IS_BUSY           equ 8015
FRS_ERR_SYSVOL_DEMOTE            equ 8016
FRS_ERR_INVALID_SERVICE_PARAMETER equ 8017
DS_S_SUCCESS                     equ NO_ERROR
ERROR_DS_NOT_INSTALLED           equ 8200
ERROR_DS_MEMBERSHIP_EVALUATED_LOCALLY equ 8201
ERROR_DS_NO_ATTRIBUTE_OR_VALUE   equ 8202
ERROR_DS_INVALID_ATTRIBUTE_SYNTAX equ 8203
ERROR_DS_ATTRIBUTE_TYPE_UNDEFINED equ 8204
ERROR_DS_ATTRIBUTE_OR_VALUE_EXISTS equ 8205
ERROR_DS_BUSY                    equ 8206
ERROR_DS_UNAVAILABLE             equ 8207
ERROR_DS_NO_RIDS_ALLOCATED       equ 8208
ERROR_DS_NO_MORE_RIDS            equ 8209
ERROR_DS_INCORRECT_ROLE_OWNER    equ 8210
ERROR_DS_RIDMGR_INIT_ERROR       equ 8211
ERROR_DS_OBJ_CLASS_VIOLATION     equ 8212
ERROR_DS_CANT_ON_NON_LEAF        equ 8213
ERROR_DS_CANT_ON_RDN             equ 8214
ERROR_DS_CANT_MOD_OBJ_CLASS      equ 8215
ERROR_DS_CROSS_DOM_MOVE_ERROR    equ 8216
ERROR_DS_GC_NOT_AVAILABLE        equ 8217
ERROR_SHARED_POLICY              equ 8218
ERROR_POLICY_OBJECT_NOT_FOUND    equ 8219
ERROR_POLICY_ONLY_IN_DS          equ 8220
ERROR_PROMOTION_ACTIVE           equ 8221
ERROR_NO_PROMOTION_ACTIVE        equ 8222
ERROR_DS_OPERATIONS_ERROR        equ 8224
ERROR_DS_PROTOCOL_ERROR          equ 8225
ERROR_DS_TIMELIMIT_EXCEEDED      equ 8226
ERROR_DS_SIZELIMIT_EXCEEDED      equ 8227
ERROR_DS_ADMIN_LIMIT_EXCEEDED    equ 8228
ERROR_DS_COMPARE_FALSE           equ 8229
ERROR_DS_COMPARE_TRUE            equ 8230
ERROR_DS_AUTH_METHOD_NOT_SUPPORTED equ 8231
ERROR_DS_STRONG_AUTH_REQUIRED    equ 8232
ERROR_DS_INAPPROPRIATE_AUTH      equ 8233
ERROR_DS_AUTH_UNKNOWN            equ 8234
ERROR_DS_REFERRAL                equ 8235
ERROR_DS_UNAVAILABLE_CRIT_EXTENSION equ 8236
ERROR_DS_CONFIDENTIALITY_REQUIRED equ 8237
ERROR_DS_INAPPROPRIATE_MATCHING  equ 8238
ERROR_DS_CONSTRAINT_VIOLATION    equ 8239
ERROR_DS_NO_SUCH_OBJECT          equ 8240
ERROR_DS_ALIAS_PROBLEM           equ 8241
ERROR_DS_INVALID_DN_SYNTAX       equ 8242
ERROR_DS_IS_LEAF                 equ 8243
ERROR_DS_ALIAS_DEREF_PROBLEM     equ 8244
ERROR_DS_UNWILLING_TO_PERFORM    equ 8245
ERROR_DS_LOOP_DETECT             equ 8246
ERROR_DS_NAMING_VIOLATION        equ 8247
ERROR_DS_OBJECT_RESULTS_TOO_LARGE equ 8248
ERROR_DS_AFFECTS_MULTIPLE_DSAS   equ 8249
ERROR_DS_SERVER_DOWN             equ 8250
ERROR_DS_LOCAL_ERROR             equ 8251
ERROR_DS_ENCODING_ERROR          equ 8252
ERROR_DS_DECODING_ERROR          equ 8253
ERROR_DS_FILTER_UNKNOWN          equ 8254
ERROR_DS_PARAM_ERROR             equ 8255
ERROR_DS_NOT_SUPPORTED           equ 8256
ERROR_DS_NO_RESULTS_RETURNED     equ 8257
ERROR_DS_CONTROL_NOT_FOUND       equ 8258
ERROR_DS_CLIENT_LOOP             equ 8259
ERROR_DS_REFERRAL_LIMIT_EXCEEDED equ 8260
ERROR_DS_SORT_CONTROL_MISSING    equ 8261
ERROR_DS_OFFSET_RANGE_ERROR      equ 8262
ERROR_DS_ROOT_MUST_BE_NC         equ 8301
ERROR_DS_ADD_REPLICA_INHIBITED   equ 8302
ERROR_DS_ATT_NOT_DEF_IN_SCHEMA   equ 8303
ERROR_DS_MAX_OBJ_SIZE_EXCEEDED   equ 8304
ERROR_DS_OBJ_STRING_NAME_EXISTS  equ 8305
ERROR_DS_NO_RDN_DEFINED_IN_SCHEMA equ 8306
ERROR_DS_RDN_DOESNT_MATCH_SCHEMA equ 8307
ERROR_DS_NO_REQUESTED_ATTS_FOUND equ 8308
ERROR_DS_USER_BUFFER_TO_SMALL    equ 8309
ERROR_DS_ATT_IS_NOT_ON_OBJ       equ 8310
ERROR_DS_ILLEGAL_MOD_OPERATION   equ 8311
ERROR_DS_OBJ_TOO_LARGE           equ 8312
ERROR_DS_BAD_INSTANCE_TYPE       equ 8313
ERROR_DS_MASTERDSA_REQUIRED      equ 8314
ERROR_DS_OBJECT_CLASS_REQUIRED   equ 8315
ERROR_DS_MISSING_REQUIRED_ATT    equ 8316
ERROR_DS_ATT_NOT_DEF_FOR_CLASS   equ 8317
ERROR_DS_ATT_ALREADY_EXISTS      equ 8318
ERROR_DS_CANT_ADD_ATT_VALUES     equ 8320
ERROR_DS_SINGLE_VALUE_CONSTRAINT equ 8321
ERROR_DS_RANGE_CONSTRAINT        equ 8322
ERROR_DS_ATT_VAL_ALREADY_EXISTS  equ 8323
ERROR_DS_CANT_REM_MISSING_ATT    equ 8324
ERROR_DS_CANT_REM_MISSING_ATT_VAL equ 8325
ERROR_DS_ROOT_CANT_BE_SUBREF     equ 8326
ERROR_DS_NO_CHAINING             equ 8327
ERROR_DS_NO_CHAINED_EVAL         equ 8328
ERROR_DS_NO_PARENT_OBJECT        equ 8329
ERROR_DS_PARENT_IS_AN_ALIAS      equ 8330
ERROR_DS_CANT_MIX_MASTER_AND_REPS equ 8331
ERROR_DS_CHILDREN_EXIST          equ 8332
ERROR_DS_OBJ_NOT_FOUND           equ 8333
ERROR_DS_ALIASED_OBJ_MISSING     equ 8334
ERROR_DS_BAD_NAME_SYNTAX         equ 8335
ERROR_DS_ALIAS_POINTS_TO_ALIAS   equ 8336
ERROR_DS_CANT_DEREF_ALIAS        equ 8337
ERROR_DS_OUT_OF_SCOPE            equ 8338
ERROR_DS_OBJECT_BEING_REMOVED    equ 8339
ERROR_DS_CANT_DELETE_DSA_OBJ     equ 8340
ERROR_DS_GENERIC_ERROR           equ 8341
ERROR_DS_DSA_MUST_BE_INT_MASTER  equ 8342
ERROR_DS_CLASS_NOT_DSA           equ 8343
ERROR_DS_INSUFF_ACCESS_RIGHTS    equ 8344
ERROR_DS_ILLEGAL_SUPERIOR        equ 8345
ERROR_DS_ATTRIBUTE_OWNED_BY_SAM  equ 8346
ERROR_DS_NAME_TOO_MANY_PARTS     equ 8347
ERROR_DS_NAME_TOO_LONG           equ 8348
ERROR_DS_NAME_VALUE_TOO_LONG     equ 8349
ERROR_DS_NAME_UNPARSEABLE        equ 8350
ERROR_DS_NAME_TYPE_UNKNOWN       equ 8351
ERROR_DS_NOT_AN_OBJECT           equ 8352
ERROR_DS_SEC_DESC_TOO_SHORT      equ 8353
ERROR_DS_SEC_DESC_INVALID        equ 8354
ERROR_DS_NO_DELETED_NAME         equ 8355
ERROR_DS_SUBREF_MUST_HAVE_PARENT equ 8356
ERROR_DS_NCNAME_MUST_BE_NC       equ 8357
ERROR_DS_CANT_ADD_SYSTEM_ONLY    equ 8358
ERROR_DS_CLASS_MUST_BE_CONCRETE  equ 8359
ERROR_DS_INVALID_DMD             equ 8360
ERROR_DS_OBJ_GUID_EXISTS         equ 8361
ERROR_DS_NOT_ON_BACKLINK         equ 8362
ERROR_DS_NO_CROSSREF_FOR_NC      equ 8363
ERROR_DS_SHUTTING_DOWN           equ 8364
ERROR_DS_UNKNOWN_OPERATION       equ 8365
ERROR_DS_INVALID_ROLE_OWNER      equ 8366
ERROR_DS_COULDNT_CONTACT_FSMO    equ 8367
ERROR_DS_CROSS_NC_DN_RENAME      equ 8368
ERROR_DS_CANT_MOD_SYSTEM_ONLY    equ 8369
ERROR_DS_REPLICATOR_ONLY         equ 8370
ERROR_DS_OBJ_CLASS_NOT_DEFINED   equ 8371
ERROR_DS_OBJ_CLASS_NOT_SUBCLASS  equ 8372
ERROR_DS_NAME_REFERENCE_INVALID  equ 8373
ERROR_DS_CROSS_REF_EXISTS        equ 8374
ERROR_DS_CANT_DEL_MASTER_CROSSREF equ 8375
ERROR_DS_SUBTREE_NOTIFY_NOT_NC_HEAD equ 8376
ERROR_DS_NOTIFY_FILTER_TOO_COMPLEX equ 8377
ERROR_DS_DUP_RDN                 equ 8378
ERROR_DS_DUP_OID                 equ 8379
ERROR_DS_DUP_MAPI_ID             equ 8380
ERROR_DS_DUP_SCHEMA_ID_GUID      equ 8381
ERROR_DS_DUP_LDAP_DISPLAY_NAME   equ 8382
ERROR_DS_SEMANTIC_ATT_TEST       equ 8383
ERROR_DS_SYNTAX_MISMATCH         equ 8384
ERROR_DS_EXISTS_IN_MUST_HAVE     equ 8385
ERROR_DS_EXISTS_IN_MAY_HAVE      equ 8386
ERROR_DS_NONEXISTENT_MAY_HAVE    equ 8387
ERROR_DS_NONEXISTENT_MUST_HAVE   equ 8388
ERROR_DS_AUX_CLS_TEST_FAIL       equ 8389
ERROR_DS_NONEXISTENT_POSS_SUP    equ 8390
ERROR_DS_SUB_CLS_TEST_FAIL       equ 8391
ERROR_DS_BAD_RDN_ATT_ID_SYNTAX   equ 8392
ERROR_DS_EXISTS_IN_AUX_CLS       equ 8393
ERROR_DS_EXISTS_IN_SUB_CLS       equ 8394
ERROR_DS_EXISTS_IN_POSS_SUP      equ 8395
ERROR_DS_RECALCSCHEMA_FAILED     equ 8396
ERROR_DS_TREE_DELETE_NOT_FINISHED equ 8397
ERROR_DS_CANT_DELETE             equ 8398
ERROR_DS_ATT_SCHEMA_REQ_ID       equ 8399
ERROR_DS_BAD_ATT_SCHEMA_SYNTAX   equ 8400
ERROR_DS_CANT_CACHE_ATT          equ 8401
ERROR_DS_CANT_CACHE_CLASS        equ 8402
ERROR_DS_CANT_REMOVE_ATT_CACHE   equ 8403
ERROR_DS_CANT_REMOVE_CLASS_CACHE equ 8404
ERROR_DS_CANT_RETRIEVE_DN        equ 8405
ERROR_DS_MISSING_SUPREF          equ 8406
ERROR_DS_CANT_RETRIEVE_INSTANCE  equ 8407
ERROR_DS_CODE_INCONSISTENCY      equ 8408
ERROR_DS_DATABASE_ERROR          equ 8409
ERROR_DS_GOVERNSID_MISSING       equ 8410
ERROR_DS_MISSING_EXPECTED_ATT    equ 8411
ERROR_DS_NCNAME_MISSING_CR_REF   equ 8412
ERROR_DS_SECURITY_CHECKING_ERROR equ 8413
ERROR_DS_SCHEMA_NOT_LOADED       equ 8414
ERROR_DS_SCHEMA_ALLOC_FAILED     equ 8415
ERROR_DS_ATT_SCHEMA_REQ_SYNTAX   equ 8416
ERROR_DS_GCVERIFY_ERROR          equ 8417
ERROR_DS_DRA_SCHEMA_MISMATCH     equ 8418
ERROR_DS_CANT_FIND_DSA_OBJ       equ 8419
ERROR_DS_CANT_FIND_EXPECTED_NC   equ 8420
ERROR_DS_CANT_FIND_NC_IN_CACHE   equ 8421
ERROR_DS_CANT_RETRIEVE_CHILD     equ 8422
ERROR_DS_SECURITY_ILLEGAL_MODIFY equ 8423
ERROR_DS_CANT_REPLACE_HIDDEN_REC equ 8424
ERROR_DS_BAD_HIERARCHY_FILE      equ 8425
ERROR_DS_BUILD_HIERARCHY_TABLE_FAILED equ 8426
ERROR_DS_CONFIG_PARAM_MISSING    equ 8427
ERROR_DS_COUNTING_AB_INDICES_FAILED equ 8428
ERROR_DS_HIERARCHY_TABLE_MALLOC_FAILED equ 8429
ERROR_DS_INTERNAL_FAILURE        equ 8430
ERROR_DS_UNKNOWN_ERROR           equ 8431
ERROR_DS_ROOT_REQUIRES_CLASS_TOP equ 8432
ERROR_DS_REFUSING_FSMO_ROLES     equ 8433
ERROR_DS_MISSING_FSMO_SETTINGS   equ 8434
ERROR_DS_UNABLE_TO_SURRENDER_ROLES equ 8435
ERROR_DS_DRA_GENERIC             equ 8436
ERROR_DS_DRA_INVALID_PARAMETER   equ 8437
ERROR_DS_DRA_BUSY                equ 8438
ERROR_DS_DRA_BAD_DN              equ 8439
ERROR_DS_DRA_BAD_NC              equ 8440
ERROR_DS_DRA_DN_EXISTS           equ 8441
ERROR_DS_DRA_INTERNAL_ERROR      equ 8442
ERROR_DS_DRA_INCONSISTENT_DIT    equ 8443
ERROR_DS_DRA_CONNECTION_FAILED   equ 8444
ERROR_DS_DRA_BAD_INSTANCE_TYPE   equ 8445
ERROR_DS_DRA_OUT_OF_MEM          equ 8446
ERROR_DS_DRA_MAIL_PROBLEM        equ 8447
ERROR_DS_DRA_REF_ALREADY_EXISTS  equ 8448
ERROR_DS_DRA_REF_NOT_FOUND       equ 8449
ERROR_DS_DRA_OBJ_IS_REP_SOURCE   equ 8450
ERROR_DS_DRA_DB_ERROR            equ 8451
ERROR_DS_DRA_NO_REPLICA          equ 8452
ERROR_DS_DRA_ACCESS_DENIED       equ 8453
ERROR_DS_DRA_NOT_SUPPORTED       equ 8454
ERROR_DS_DRA_RPC_CANCELLED       equ 8455
ERROR_DS_DRA_SOURCE_DISABLED     equ 8456
ERROR_DS_DRA_SINK_DISABLED       equ 8457
ERROR_DS_DRA_NAME_COLLISION      equ 8458
ERROR_DS_DRA_SOURCE_REINSTALLED  equ 8459
ERROR_DS_DRA_MISSING_PARENT      equ 8460
ERROR_DS_DRA_PREEMPTED           equ 8461
ERROR_DS_DRA_ABANDON_SYNC        equ 8462
ERROR_DS_DRA_SHUTDOWN            equ 8463
ERROR_DS_DRA_INCOMPATIBLE_PARTIAL_SET equ 8464
ERROR_DS_DRA_SOURCE_IS_PARTIAL_REPLICA equ 8465
ERROR_DS_DRA_EXTN_CONNECTION_FAILED equ 8466
ERROR_DS_INSTALL_SCHEMA_MISMATCH equ 8467
ERROR_DS_DUP_LINK_ID             equ 8468
ERROR_DS_NAME_ERROR_RESOLVING    equ 8469
ERROR_DS_NAME_ERROR_NOT_FOUND    equ 8470
ERROR_DS_NAME_ERROR_NOT_UNIQUE   equ 8471
ERROR_DS_NAME_ERROR_NO_MAPPING   equ 8472
ERROR_DS_NAME_ERROR_DOMAIN_ONLY  equ 8473
ERROR_DS_NAME_ERROR_NO_SYNTACTICAL_MAPPING equ 8474
ERROR_DS_CONSTRUCTED_ATT_MOD     equ 8475
ERROR_DS_WRONG_OM_OBJ_CLASS      equ 8476
ERROR_DS_DRA_REPL_PENDING        equ 8477
ERROR_DS_DS_REQUIRED             equ 8478
ERROR_DS_INVALID_LDAP_DISPLAY_NAME equ 8479
ERROR_DS_NON_BASE_SEARCH         equ 8480
ERROR_DS_CANT_RETRIEVE_ATTS      equ 8481
ERROR_DS_BACKLINK_WITHOUT_LINK   equ 8482
ERROR_DS_EPOCH_MISMATCH          equ 8483
ERROR_DS_SRC_NAME_MISMATCH       equ 8484
ERROR_DS_SRC_AND_DST_NC_IDENTICAL equ 8485
ERROR_DS_DST_NC_MISMATCH         equ 8486
ERROR_DS_NOT_AUTHORITIVE_FOR_DST_NC equ 8487
ERROR_DS_SRC_GUID_MISMATCH       equ 8488
ERROR_DS_CANT_MOVE_DELETED_OBJECT equ 8489
ERROR_DS_PDC_OPERATION_IN_PROGRESS equ 8490
ERROR_DS_CROSS_DOMAIN_CLEANUP_REQD equ 8491
ERROR_DS_ILLEGAL_XDOM_MOVE_OPERATION equ 8492
ERROR_DS_CANT_WITH_ACCT_GROUP_MEMBERSHPS equ 8493
ERROR_DS_NC_MUST_HAVE_NC_PARENT  equ 8494
ERROR_DS_CR_IMPOSSIBLE_TO_VALIDATE equ 8495
ERROR_DS_DST_DOMAIN_NOT_NATIVE   equ 8496
ERROR_DS_MISSING_INFRASTRUCTURE_CONTAINER equ 8497
ERROR_DS_CANT_MOVE_ACCOUNT_GROUP equ 8498
ERROR_DS_CANT_MOVE_RESOURCE_GROUP equ 8499
ERROR_DS_INVALID_SEARCH_FLAG     equ 8500
ERROR_DS_NO_TREE_DELETE_ABOVE_NC equ 8501
ERROR_DS_COULDNT_LOCK_TREE_FOR_DELETE equ 8502
ERROR_DS_COULDNT_IDENTIFY_OBJECTS_FOR_TREE_DELETE equ 8503
ERROR_DS_SAM_INIT_FAILURE        equ 8504
ERROR_DS_SENSITIVE_GROUP_VIOLATION equ 8505
ERROR_DS_CANT_MOD_PRIMARYGROUPID equ 8506
ERROR_DS_ILLEGAL_BASE_SCHEMA_MOD equ 8507
ERROR_DS_NONSAFE_SCHEMA_CHANGE   equ 8508
ERROR_DS_SCHEMA_UPDATE_DISALLOWED equ 8509
ERROR_DS_CANT_CREATE_UNDER_SCHEMA equ 8510
ERROR_DS_INSTALL_NO_SRC_SCH_VERSION equ 8511
ERROR_DS_INSTALL_NO_SCH_VERSION_IN_INIFILE equ 8512
ERROR_DS_INVALID_GROUP_TYPE      equ 8513
ERROR_DS_NO_NEST_GLOBALGROUP_IN_MIXEDDOMAIN equ 8514
ERROR_DS_NO_NEST_LOCALGROUP_IN_MIXEDDOMAIN equ 8515
ERROR_DS_GLOBAL_CANT_HAVE_LOCAL_MEMBER equ 8516
ERROR_DS_GLOBAL_CANT_HAVE_UNIVERSAL_MEMBER equ 8517
ERROR_DS_UNIVERSAL_CANT_HAVE_LOCAL_MEMBER equ 8518
ERROR_DS_GLOBAL_CANT_HAVE_CROSSDOMAIN_MEMBER equ 8519
ERROR_DS_LOCAL_CANT_HAVE_CROSSDOMAIN_LOCAL_MEMBER equ 8520
ERROR_DS_HAVE_PRIMARY_MEMBERS    equ 8521
ERROR_DS_STRING_SD_CONVERSION_FAILED equ 8522
ERROR_DS_NAMING_MASTER_GC        equ 8523
ERROR_DS_DNS_LOOKUP_FAILURE      equ 8524
ERROR_DS_COULDNT_UPDATE_SPNS     equ 8525
ERROR_DS_CANT_RETRIEVE_SD        equ 8526
ERROR_DS_KEY_NOT_UNIQUE          equ 8527
ERROR_DS_WRONG_LINKED_ATT_SYNTAX equ 8528
ERROR_DS_SAM_NEED_BOOTKEY_PASSWORD equ 8529
ERROR_DS_SAM_NEED_BOOTKEY_FLOPPY equ 8530
ERROR_DS_CANT_START              equ 8531
ERROR_DS_INIT_FAILURE            equ 8532
ERROR_DS_NO_PKT_PRIVACY_ON_CONNECTION equ 8533
ERROR_DS_SOURCE_DOMAIN_IN_FOREST equ 8534
ERROR_DS_DESTINATION_DOMAIN_NOT_IN_FOREST equ 8535
ERROR_DS_DESTINATION_AUDITING_NOT_ENABLED equ 8536
ERROR_DS_CANT_FIND_DC_FOR_SRC_DOMAIN equ 8537
ERROR_DS_SRC_OBJ_NOT_GROUP_OR_USER equ 8538
ERROR_DS_SRC_SID_EXISTS_IN_FOREST equ 8539
ERROR_DS_SRC_AND_DST_OBJECT_CLASS_MISMATCH equ 8540
ERROR_SAM_INIT_FAILURE           equ 8541
ERROR_DS_DRA_SCHEMA_INFO_SHIP    equ 8542
ERROR_DS_DRA_SCHEMA_CONFLICT     equ 8543
ERROR_DS_DRA_EARLIER_SCHEMA_CONFLICT equ 8544
ERROR_DS_DRA_OBJ_NC_MISMATCH     equ 8545
ERROR_DS_NC_STILL_HAS_DSAS       equ 8546
ERROR_DS_GC_REQUIRED             equ 8547
ERROR_DS_LOCAL_MEMBER_OF_LOCAL_ONLY equ 8548
ERROR_DS_NO_FPO_IN_UNIVERSAL_GROUPS equ 8549
ERROR_DS_CANT_ADD_TO_GC          equ 8550
ERROR_DS_NO_CHECKPOINT_WITH_PDC  equ 8551
ERROR_DS_SOURCE_AUDITING_NOT_ENABLED equ 8552
ERROR_DS_CANT_CREATE_IN_NONDOMAIN_NC equ 8553
ERROR_DS_INVALID_NAME_FOR_SPN    equ 8554
ERROR_DS_FILTER_USES_CONTRUCTED_ATTRS equ 8555
ERROR_DS_UNICODEPWD_NOT_IN_QUOTES equ 8556
ERROR_DS_MACHINE_ACCOUNT_QUOTA_EXCEEDED equ 8557
ERROR_DS_MUST_BE_RUN_ON_DST_DC   equ 8558
ERROR_DS_SRC_DC_MUST_BE_SP4_OR_GREATER equ 8559
ERROR_DS_CANT_TREE_DELETE_CRITICAL_OBJ equ 8560
ERROR_DS_INIT_FAILURE_CONSOLE    equ 8561
ERROR_DS_SAM_INIT_FAILURE_CONSOLE equ 8562
ERROR_DS_FOREST_VERSION_TOO_HIGH equ 8563
ERROR_DS_DOMAIN_VERSION_TOO_HIGH equ 8564
ERROR_DS_FOREST_VERSION_TOO_LOW  equ 8565
ERROR_DS_DOMAIN_VERSION_TOO_LOW  equ 8566
ERROR_DS_INCOMPATIBLE_VERSION    equ 8567
ERROR_DS_LOW_DSA_VERSION         equ 8568
ERROR_DS_NO_BEHAVIOR_VERSION_IN_MIXEDDOMAIN equ 8569
ERROR_DS_NOT_SUPPORTED_SORT_ORDER equ 8570
ERROR_DS_NAME_NOT_UNIQUE         equ 8571
ERROR_DS_MACHINE_ACCOUNT_CREATED_PRENT4 equ 8572
ERROR_DS_OUT_OF_VERSION_STORE    equ 8573
ERROR_DS_INCOMPATIBLE_CONTROLS_USED equ 8574
ERROR_DS_NO_REF_DOMAIN           equ 8575
ERROR_DS_RESERVED_LINK_ID        equ 8576
ERROR_DS_LINK_ID_NOT_AVAILABLE   equ 8577
ERROR_DS_AG_CANT_HAVE_UNIVERSAL_MEMBER equ 8578
ERROR_DS_MODIFYDN_DISALLOWED_BY_INSTANCE_TYPE equ 8579
ERROR_DS_NO_OBJECT_MOVE_IN_SCHEMA_NC equ 8580
ERROR_DS_MODIFYDN_DISALLOWED_BY_FLAG equ 8581
ERROR_DS_MODIFYDN_WRONG_GRANDPARENT equ 8582
ERROR_DS_NAME_ERROR_TRUST_REFERRAL equ 8583
ERROR_NOT_SUPPORTED_ON_STANDARD_SERVER equ 8584
ERROR_DS_CANT_ACCESS_REMOTE_PART_OF_AD equ 8585
ERROR_DS_CR_IMPOSSIBLE_TO_VALIDATE_V2 equ 8586
ERROR_DS_THREAD_LIMIT_EXCEEDED   equ 8587
ERROR_DS_NOT_CLOSEST             equ 8588
ERROR_DS_CANT_DERIVE_SPN_WITHOUT_SERVER_REF equ 8589
ERROR_DS_SINGLE_USER_MODE_FAILED equ 8590
ERROR_DS_NTDSCRIPT_SYNTAX_ERROR  equ 8591
ERROR_DS_NTDSCRIPT_PROCESS_ERROR equ 8592
ERROR_DS_DIFFERENT_REPL_EPOCHS   equ 8593
ERROR_DS_DRS_EXTENSIONS_CHANGED  equ 8594
ERROR_DS_REPLICA_SET_CHANGE_NOT_ALLOWED_ON_DISABLED_CR equ 8595
ERROR_DS_NO_MSDS_INTID           equ 8596
ERROR_DS_DUP_MSDS_INTID          equ 8597
ERROR_DS_EXISTS_IN_RDNATTID      equ 8598
ERROR_DS_AUTHORIZATION_FAILED    equ 8599
ERROR_DS_INVALID_SCRIPT          equ 8600
ERROR_DS_REMOTE_CROSSREF_OP_FAILED equ 8601
ERROR_DS_CROSS_REF_BUSY          equ 8602
ERROR_DS_CANT_DERIVE_SPN_FOR_DELETED_DOMAIN equ 8603
ERROR_DS_CANT_DEMOTE_WITH_WRITEABLE_NC equ 8604
ERROR_DS_DUPLICATE_ID_FOUND      equ 8605
ERROR_DS_INSUFFICIENT_ATTR_TO_CREATE_OBJECT equ 8606
ERROR_DS_GROUP_CONVERSION_ERROR  equ 8607
ERROR_DS_CANT_MOVE_APP_BASIC_GROUP equ 8608
ERROR_DS_CANT_MOVE_APP_QUERY_GROUP equ 8609
ERROR_DS_ROLE_NOT_VERIFIED       equ 8610
ERROR_DS_WKO_CONTAINER_CANNOT_BE_SPECIAL equ 8611
ERROR_DS_DOMAIN_RENAME_IN_PROGRESS equ 8612
ERROR_DS_EXISTING_AD_CHILD_NC    equ 8613
ERROR_DS_REPL_LIFETIME_EXCEEDED  equ 8614
ERROR_DS_DISALLOWED_IN_SYSTEM_CONTAINER equ 8615
ERROR_DS_LDAP_SEND_QUEUE_FULL    equ 8616
ERROR_DS_DRA_OUT_SCHEDULE_WINDOW equ 8617
ERROR_DS_POLICY_NOT_KNOWN        equ 8618
ERROR_NO_SITE_SETTINGS_OBJECT    equ 8619
ERROR_NO_SECRETS                 equ 8620
ERROR_NO_WRITABLE_DC_FOUND       equ 8621
ERROR_DS_NO_SERVER_OBJECT        equ 8622
ERROR_DS_NO_NTDSA_OBJECT         equ 8623
ERROR_DS_NON_ASQ_SEARCH          equ 8624
ERROR_DS_AUDIT_FAILURE           equ 8625
ERROR_DS_INVALID_SEARCH_FLAG_SUBTREE equ 8626
ERROR_DS_INVALID_SEARCH_FLAG_TUPLE equ 8627
ERROR_DS_HIERARCHY_TABLE_TOO_DEEP equ 8628
DNS_ERROR_RESPONSE_CODES_BASE    equ 9000
DNS_ERROR_RCODE_NO_ERROR         equ NO_ERROR
DNS_ERROR_MASK                   equ 00002328h
DNS_ERROR_RCODE_FORMAT_ERROR     equ 9001
DNS_ERROR_RCODE_SERVER_FAILURE   equ 9002
DNS_ERROR_RCODE_NAME_ERROR       equ 9003
DNS_ERROR_RCODE_NOT_IMPLEMENTED  equ 9004
DNS_ERROR_RCODE_REFUSED          equ 9005
DNS_ERROR_RCODE_YXDOMAIN         equ 9006
DNS_ERROR_RCODE_YXRRSET          equ 9007
DNS_ERROR_RCODE_NXRRSET          equ 9008
DNS_ERROR_RCODE_NOTAUTH          equ 9009
DNS_ERROR_RCODE_NOTZONE          equ 9010
DNS_ERROR_RCODE_BADSIG           equ 9016
DNS_ERROR_RCODE_BADKEY           equ 9017
DNS_ERROR_RCODE_BADTIME          equ 9018
DNS_ERROR_RCODE_LAST             equ DNS_ERROR_RCODE_BADTIME
DNS_ERROR_PACKET_FMT_BASE        equ 9500
DNS_INFO_NO_RECORDS              equ 9501
DNS_ERROR_BAD_PACKET             equ 9502
DNS_ERROR_NO_PACKET              equ 9503
DNS_ERROR_RCODE                  equ 9504
DNS_ERROR_UNSECURE_PACKET        equ 9505
DNS_STATUS_PACKET_UNSECURE       equ DNS_ERROR_UNSECURE_PACKET
DNS_ERROR_NO_MEMORY              equ ERROR_OUTOFMEMORY
DNS_ERROR_INVALID_NAME           equ ERROR_INVALID_NAME
DNS_ERROR_INVALID_DATA           equ ERROR_INVALID_DATA
DNS_ERROR_GENERAL_API_BASE       equ 9550
DNS_ERROR_INVALID_TYPE           equ 9551
DNS_ERROR_INVALID_IP_ADDRESS     equ 9552
DNS_ERROR_INVALID_PROPERTY       equ 9553
DNS_ERROR_TRY_AGAIN_LATER        equ 9554
DNS_ERROR_NOT_UNIQUE             equ 9555
DNS_ERROR_NON_RFC_NAME           equ 9556
DNS_STATUS_FQDN                  equ 9557
DNS_STATUS_DOTTED_NAME           equ 9558
DNS_STATUS_SINGLE_PART_NAME      equ 9559
DNS_ERROR_INVALID_NAME_CHAR      equ 9560
DNS_ERROR_NUMERIC_NAME           equ 9561
DNS_ERROR_NOT_ALLOWED_ON_ROOT_SERVER equ 9562
DNS_ERROR_NOT_ALLOWED_UNDER_DELEGATION equ 9563
DNS_ERROR_CANNOT_FIND_ROOT_HINTS equ 9564
DNS_ERROR_INCONSISTENT_ROOT_HINTS equ 9565
DNS_ERROR_DWORD_VALUE_TOO_SMALL  equ 9566
DNS_ERROR_DWORD_VALUE_TOO_LARGE  equ 9567
DNS_ERROR_BACKGROUND_LOADING     equ 9568
DNS_ERROR_NOT_ALLOWED_ON_RODC    equ 9569
DNS_ERROR_NOT_ALLOWED_UNDER_DNAME equ 9570
DNS_ERROR_ZONE_BASE              equ 9600
DNS_ERROR_ZONE_DOES_NOT_EXIST    equ 9601
DNS_ERROR_NO_ZONE_INFO           equ 9602
DNS_ERROR_INVALID_ZONE_OPERATION equ 9603
DNS_ERROR_ZONE_CONFIGURATION_ERROR equ 9604
DNS_ERROR_ZONE_HAS_NO_SOA_RECORD equ 9605
DNS_ERROR_ZONE_HAS_NO_NS_RECORDS equ 9606
DNS_ERROR_ZONE_LOCKED            equ 9607
DNS_ERROR_ZONE_CREATION_FAILED   equ 9608
DNS_ERROR_ZONE_ALREADY_EXISTS    equ 9609
DNS_ERROR_AUTOZONE_ALREADY_EXISTS equ 9610
DNS_ERROR_INVALID_ZONE_TYPE      equ 9611
DNS_ERROR_SECONDARY_REQUIRES_MASTER_IP equ 9612
DNS_ERROR_ZONE_NOT_SECONDARY     equ 9613
DNS_ERROR_NEED_SECONDARY_ADDRESSES equ 9614
DNS_ERROR_WINS_INIT_FAILED       equ 9615
DNS_ERROR_NEED_WINS_SERVERS      equ 9616
DNS_ERROR_NBSTAT_INIT_FAILED     equ 9617
DNS_ERROR_SOA_DELETE_INVALID     equ 9618
DNS_ERROR_FORWARDER_ALREADY_EXISTS equ 9619
DNS_ERROR_ZONE_REQUIRES_MASTER_IP equ 9620
DNS_ERROR_ZONE_IS_SHUTDOWN       equ 9621
DNS_ERROR_DATAFILE_BASE          equ 9650
DNS_ERROR_PRIMARY_REQUIRES_DATAFILE equ 9651
DNS_ERROR_INVALID_DATAFILE_NAME  equ 9652
DNS_ERROR_DATAFILE_OPEN_FAILURE  equ 9653
DNS_ERROR_FILE_WRITEBACK_FAILED  equ 9654
DNS_ERROR_DATAFILE_PARSING       equ 9655
DNS_ERROR_DATABASE_BASE          equ 9700
DNS_ERROR_RECORD_DOES_NOT_EXIST  equ 9701
DNS_ERROR_RECORD_FORMAT          equ 9702
DNS_ERROR_NODE_CREATION_FAILED   equ 9703
DNS_ERROR_UNKNOWN_RECORD_TYPE    equ 9704
DNS_ERROR_RECORD_TIMED_OUT       equ 9705
DNS_ERROR_NAME_NOT_IN_ZONE       equ 9706
DNS_ERROR_CNAME_LOOP             equ 9707
DNS_ERROR_NODE_IS_CNAME          equ 9708
DNS_ERROR_CNAME_COLLISION        equ 9709
DNS_ERROR_RECORD_ONLY_AT_ZONE_ROOT equ 9710
DNS_ERROR_RECORD_ALREADY_EXISTS  equ 9711
DNS_ERROR_SECONDARY_DATA         equ 9712
DNS_ERROR_NO_CREATE_CACHE_DATA   equ 9713
DNS_ERROR_NAME_DOES_NOT_EXIST    equ 9714
DNS_WARNING_PTR_CREATE_FAILED    equ 9715
DNS_WARNING_DOMAIN_UNDELETED     equ 9716
DNS_ERROR_DS_UNAVAILABLE         equ 9717
DNS_ERROR_DS_ZONE_ALREADY_EXISTS equ 9718
DNS_ERROR_NO_BOOTFILE_IF_DS_ZONE equ 9719
DNS_ERROR_NODE_IS_DNAME          equ 9720
DNS_ERROR_DNAME_COLLISION        equ 9721
DNS_ERROR_ALIAS_LOOP             equ 9722
DNS_ERROR_OPERATION_BASE         equ 9750
DNS_INFO_AXFR_COMPLETE           equ 9751
DNS_ERROR_AXFR                   equ 9752
DNS_INFO_ADDED_LOCAL_WINS        equ 9753
DNS_ERROR_SECURE_BASE            equ 9800
DNS_STATUS_CONTINUE_NEEDED       equ 9801
DNS_ERROR_SETUP_BASE             equ 9850
DNS_ERROR_NO_TCPIP               equ 9851
DNS_ERROR_NO_DNS_SERVERS         equ 9852
DNS_ERROR_DP_BASE                equ 9900
DNS_ERROR_DP_DOES_NOT_EXIST      equ 9901
DNS_ERROR_DP_ALREADY_EXISTS      equ 9902
DNS_ERROR_DP_NOT_ENLISTED        equ 9903
DNS_ERROR_DP_ALREADY_ENLISTED    equ 9904
DNS_ERROR_DP_NOT_AVAILABLE       equ 9905
DNS_ERROR_DP_FSMO_ERROR          equ 9906
ERROR_IPSEC_QM_POLICY_EXISTS     equ 13000
ERROR_IPSEC_QM_POLICY_NOT_FOUND  equ 13001
ERROR_IPSEC_QM_POLICY_IN_USE     equ 13002
ERROR_IPSEC_MM_POLICY_EXISTS     equ 13003
ERROR_IPSEC_MM_POLICY_NOT_FOUND  equ 13004
ERROR_IPSEC_MM_POLICY_IN_USE     equ 13005
ERROR_IPSEC_MM_FILTER_EXISTS     equ 13006
ERROR_IPSEC_MM_FILTER_NOT_FOUND  equ 13007
ERROR_IPSEC_TRANSPORT_FILTER_EXISTS equ 13008
ERROR_IPSEC_TRANSPORT_FILTER_NOT_FOUND equ 13009
ERROR_IPSEC_MM_AUTH_EXISTS       equ 13010
ERROR_IPSEC_MM_AUTH_NOT_FOUND    equ 13011
ERROR_IPSEC_MM_AUTH_IN_USE       equ 13012
ERROR_IPSEC_DEFAULT_MM_POLICY_NOT_FOUND equ 13013
ERROR_IPSEC_DEFAULT_MM_AUTH_NOT_FOUND equ 13014
ERROR_IPSEC_DEFAULT_QM_POLICY_NOT_FOUND equ 13015
ERROR_IPSEC_TUNNEL_FILTER_EXISTS equ 13016
ERROR_IPSEC_TUNNEL_FILTER_NOT_FOUND equ 13017
ERROR_IPSEC_MM_FILTER_PENDING_DELETION equ 13018
ERROR_IPSEC_TRANSPORT_FILTER_PENDING_DELETION equ 13019
ERROR_IPSEC_TUNNEL_FILTER_PENDING_DELETION equ 13020
ERROR_IPSEC_MM_POLICY_PENDING_DELETION equ 13021
ERROR_IPSEC_MM_AUTH_PENDING_DELETION equ 13022
ERROR_IPSEC_QM_POLICY_PENDING_DELETION equ 13023
WARNING_IPSEC_MM_POLICY_PRUNED   equ 13024
WARNING_IPSEC_QM_POLICY_PRUNED   equ 13025
ERROR_IPSEC_IKE_NEG_STATUS_BEGIN equ 13800
ERROR_IPSEC_IKE_AUTH_FAIL        equ 13801
ERROR_IPSEC_IKE_ATTRIB_FAIL      equ 13802
ERROR_IPSEC_IKE_NEGOTIATION_PENDING equ 13803
ERROR_IPSEC_IKE_GENERAL_PROCESSING_ERROR equ 13804
ERROR_IPSEC_IKE_TIMED_OUT        equ 13805
ERROR_IPSEC_IKE_NO_CERT          equ 13806
ERROR_IPSEC_IKE_SA_DELETED       equ 13807
ERROR_IPSEC_IKE_SA_REAPED        equ 13808
ERROR_IPSEC_IKE_MM_ACQUIRE_DROP  equ 13809
ERROR_IPSEC_IKE_QM_ACQUIRE_DROP  equ 13810
ERROR_IPSEC_IKE_QUEUE_DROP_MM    equ 13811
ERROR_IPSEC_IKE_QUEUE_DROP_NO_MM equ 13812
ERROR_IPSEC_IKE_DROP_NO_RESPONSE equ 13813
ERROR_IPSEC_IKE_MM_DELAY_DROP    equ 13814
ERROR_IPSEC_IKE_QM_DELAY_DROP    equ 13815
ERROR_IPSEC_IKE_ERROR            equ 13816
ERROR_IPSEC_IKE_CRL_FAILED       equ 13817
ERROR_IPSEC_IKE_INVALID_KEY_USAGE equ 13818
ERROR_IPSEC_IKE_INVALID_CERT_TYPE equ 13819
ERROR_IPSEC_IKE_NO_PRIVATE_KEY   equ 13820
ERROR_IPSEC_IKE_DH_FAIL          equ 13822
ERROR_IPSEC_IKE_INVALID_HEADER   equ 13824
ERROR_IPSEC_IKE_NO_POLICY        equ 13825
ERROR_IPSEC_IKE_INVALID_SIGNATURE equ 13826
ERROR_IPSEC_IKE_KERBEROS_ERROR   equ 13827
ERROR_IPSEC_IKE_NO_PUBLIC_KEY    equ 13828
ERROR_IPSEC_IKE_PROCESS_ERR      equ 13829
ERROR_IPSEC_IKE_PROCESS_ERR_SA   equ 13830
ERROR_IPSEC_IKE_PROCESS_ERR_PROP equ 13831
ERROR_IPSEC_IKE_PROCESS_ERR_TRANS equ 13832
ERROR_IPSEC_IKE_PROCESS_ERR_KE   equ 13833
ERROR_IPSEC_IKE_PROCESS_ERR_ID   equ 13834
ERROR_IPSEC_IKE_PROCESS_ERR_CERT equ 13835
ERROR_IPSEC_IKE_PROCESS_ERR_CERT_REQ equ 13836
ERROR_IPSEC_IKE_PROCESS_ERR_HASH equ 13837
ERROR_IPSEC_IKE_PROCESS_ERR_SIG  equ 13838
ERROR_IPSEC_IKE_PROCESS_ERR_NONCE equ 13839
ERROR_IPSEC_IKE_PROCESS_ERR_NOTIFY equ 13840
ERROR_IPSEC_IKE_PROCESS_ERR_DELETE equ 13841
ERROR_IPSEC_IKE_PROCESS_ERR_VENDOR equ 13842
ERROR_IPSEC_IKE_INVALID_PAYLOAD  equ 13843
ERROR_IPSEC_IKE_LOAD_SOFT_SA     equ 13844
ERROR_IPSEC_IKE_SOFT_SA_TORN_DOWN equ 13845
ERROR_IPSEC_IKE_INVALID_COOKIE   equ 13846
ERROR_IPSEC_IKE_NO_PEER_CERT     equ 13847
ERROR_IPSEC_IKE_PEER_CRL_FAILED  equ 13848
ERROR_IPSEC_IKE_POLICY_CHANGE    equ 13849
ERROR_IPSEC_IKE_NO_MM_POLICY     equ 13850
ERROR_IPSEC_IKE_NOTCBPRIV        equ 13851
ERROR_IPSEC_IKE_SECLOADFAIL      equ 13852
ERROR_IPSEC_IKE_FAILSSPINIT      equ 13853
ERROR_IPSEC_IKE_FAILQUERYSSP     equ 13854
ERROR_IPSEC_IKE_SRVACQFAIL       equ 13855
ERROR_IPSEC_IKE_SRVQUERYCRED     equ 13856
ERROR_IPSEC_IKE_GETSPIFAIL       equ 13857
ERROR_IPSEC_IKE_INVALID_FILTER   equ 13858
ERROR_IPSEC_IKE_OUT_OF_MEMORY    equ 13859
ERROR_IPSEC_IKE_ADD_UPDATE_KEY_FAILED equ 13860
ERROR_IPSEC_IKE_INVALID_POLICY   equ 13861
ERROR_IPSEC_IKE_UNKNOWN_DOI      equ 13862
ERROR_IPSEC_IKE_INVALID_SITUATION equ 13863
ERROR_IPSEC_IKE_DH_FAILURE       equ 13864
ERROR_IPSEC_IKE_INVALID_GROUP    equ 13865
ERROR_IPSEC_IKE_ENCRYPT          equ 13866
ERROR_IPSEC_IKE_DECRYPT          equ 13867
ERROR_IPSEC_IKE_POLICY_MATCH     equ 13868
ERROR_IPSEC_IKE_UNSUPPORTED_ID   equ 13869
ERROR_IPSEC_IKE_INVALID_HASH     equ 13870
ERROR_IPSEC_IKE_INVALID_HASH_ALG equ 13871
ERROR_IPSEC_IKE_INVALID_HASH_SIZE equ 13872
ERROR_IPSEC_IKE_INVALID_ENCRYPT_ALG equ 13873
ERROR_IPSEC_IKE_INVALID_AUTH_ALG equ 13874
ERROR_IPSEC_IKE_INVALID_SIG      equ 13875
ERROR_IPSEC_IKE_LOAD_FAILED      equ 13876
ERROR_IPSEC_IKE_RPC_DELETE       equ 13877
ERROR_IPSEC_IKE_BENIGN_REINIT    equ 13878
ERROR_IPSEC_IKE_INVALID_RESPONDER_LIFETIME_NOTIFY equ 13879
ERROR_IPSEC_IKE_INVALID_CERT_KEYLEN equ 13881
ERROR_IPSEC_IKE_MM_LIMIT         equ 13882
ERROR_IPSEC_IKE_NEGOTIATION_DISABLED equ 13883
ERROR_IPSEC_IKE_QM_LIMIT         equ 13884
ERROR_IPSEC_IKE_MM_EXPIRED       equ 13885
ERROR_IPSEC_IKE_PEER_MM_ASSUMED_INVALID equ 13886
ERROR_IPSEC_IKE_CERT_CHAIN_POLICY_MISMATCH equ 13887
ERROR_IPSEC_IKE_UNEXPECTED_MESSAGE_ID equ 13888
ERROR_IPSEC_IKE_INVALID_AUTH_PAYLOAD equ 13889
ERROR_IPSEC_IKE_DOS_COOKIE_SENT  equ 13890
ERROR_IPSEC_IKE_SHUTTING_DOWN    equ 13891
ERROR_IPSEC_IKE_CGA_AUTH_FAILED  equ 13892
ERROR_IPSEC_IKE_PROCESS_ERR_NATOA equ 13893
ERROR_IPSEC_IKE_INVALID_MM_FOR_QM equ 13894
ERROR_IPSEC_IKE_QM_EXPIRED       equ 13895
ERROR_IPSEC_IKE_TOO_MANY_FILTERS equ 13896
ERROR_IPSEC_IKE_NEG_STATUS_END   equ 13897
ERROR_IPSEC_BAD_SPI              equ 13910
ERROR_IPSEC_SA_LIFETIME_EXPIRED  equ 13911
ERROR_IPSEC_WRONG_SA             equ 13912
ERROR_IPSEC_REPLAY_CHECK_FAILED  equ 13913
ERROR_IPSEC_INVALID_PACKET       equ 13914
ERROR_IPSEC_INTEGRITY_CHECK_FAILED equ 13915
ERROR_IPSEC_CLEAR_TEXT_DROP      equ 13916
ERROR_SXS_SECTION_NOT_FOUND      equ 14000
ERROR_SXS_CANT_GEN_ACTCTX        equ 14001
ERROR_SXS_INVALID_ACTCTXDATA_FORMAT equ 14002
ERROR_SXS_ASSEMBLY_NOT_FOUND     equ 14003
ERROR_SXS_MANIFEST_FORMAT_ERROR  equ 14004
ERROR_SXS_MANIFEST_PARSE_ERROR   equ 14005
ERROR_SXS_ACTIVATION_CONTEXT_DISABLED equ 14006
ERROR_SXS_KEY_NOT_FOUND          equ 14007
ERROR_SXS_VERSION_CONFLICT       equ 14008
ERROR_SXS_WRONG_SECTION_TYPE     equ 14009
ERROR_SXS_THREAD_QUERIES_DISABLED equ 14010
ERROR_SXS_PROCESS_DEFAULT_ALREADY_SET equ 14011
ERROR_SXS_UNKNOWN_ENCODING_GROUP equ 14012
ERROR_SXS_UNKNOWN_ENCODING       equ 14013
ERROR_SXS_INVALID_XML_NAMESPACE_URI equ 14014
ERROR_SXS_ROOT_MANIFEST_DEPENDENCY_NOT_INSTALLED equ 14015
ERROR_SXS_LEAF_MANIFEST_DEPENDENCY_NOT_INSTALLED equ 14016
ERROR_SXS_INVALID_ASSEMBLY_IDENTITY_ATTRIBUTE equ 14017
ERROR_SXS_MANIFEST_MISSING_REQUIRED_DEFAULT_NAMESPACE equ 14018
ERROR_SXS_MANIFEST_INVALID_REQUIRED_DEFAULT_NAMESPACE equ 14019
ERROR_SXS_PRIVATE_MANIFEST_CROSS_PATH_WITH_REPARSE_POINT equ 14020
ERROR_SXS_DUPLICATE_DLL_NAME     equ 14021
ERROR_SXS_DUPLICATE_WINDOWCLASS_NAME equ 14022
ERROR_SXS_DUPLICATE_CLSID        equ 14023
ERROR_SXS_DUPLICATE_IID          equ 14024
ERROR_SXS_DUPLICATE_TLBID        equ 14025
ERROR_SXS_DUPLICATE_PROGID       equ 14026
ERROR_SXS_DUPLICATE_ASSEMBLY_NAME equ 14027
ERROR_SXS_FILE_HASH_MISMATCH     equ 14028
ERROR_SXS_POLICY_PARSE_ERROR     equ 14029
ERROR_SXS_XML_E_MISSINGQUOTE     equ 14030
ERROR_SXS_XML_E_COMMENTSYNTAX    equ 14031
ERROR_SXS_XML_E_BADSTARTNAMECHAR equ 14032
ERROR_SXS_XML_E_BADNAMECHAR      equ 14033
ERROR_SXS_XML_E_BADCHARINSTRING  equ 14034
ERROR_SXS_XML_E_XMLDECLSYNTAX    equ 14035
ERROR_SXS_XML_E_BADCHARDATA      equ 14036
ERROR_SXS_XML_E_MISSINGWHITESPACE equ 14037
ERROR_SXS_XML_E_EXPECTINGTAGEND  equ 14038
ERROR_SXS_XML_E_MISSINGSEMICOLON equ 14039
ERROR_SXS_XML_E_UNBALANCEDPAREN  equ 14040
ERROR_SXS_XML_E_INTERNALERROR    equ 14041
ERROR_SXS_XML_E_UNEXPECTED_WHITESPACE equ 14042
ERROR_SXS_XML_E_INCOMPLETE_ENCODING equ 14043
ERROR_SXS_XML_E_MISSING_PAREN    equ 14044
ERROR_SXS_XML_E_EXPECTINGCLOSEQUOTE equ 14045
ERROR_SXS_XML_E_MULTIPLE_COLONS  equ 14046
ERROR_SXS_XML_E_INVALID_DECIMAL  equ 14047
ERROR_SXS_XML_E_INVALID_HEXIDECIMAL equ 14048
ERROR_SXS_XML_E_INVALID_UNICODE  equ 14049
ERROR_SXS_XML_E_WHITESPACEORQUESTIONMARK equ 14050
ERROR_SXS_XML_E_UNEXPECTEDENDTAG equ 14051
ERROR_SXS_XML_E_UNCLOSEDTAG      equ 14052
ERROR_SXS_XML_E_DUPLICATEATTRIBUTE equ 14053
ERROR_SXS_XML_E_MULTIPLEROOTS    equ 14054
ERROR_SXS_XML_E_INVALIDATROOTLEVEL equ 14055
ERROR_SXS_XML_E_BADXMLDECL       equ 14056
ERROR_SXS_XML_E_MISSINGROOT      equ 14057
ERROR_SXS_XML_E_UNEXPECTEDEOF    equ 14058
ERROR_SXS_XML_E_BADPEREFINSUBSET equ 14059
ERROR_SXS_XML_E_UNCLOSEDSTARTTAG equ 14060
ERROR_SXS_XML_E_UNCLOSEDENDTAG   equ 14061
ERROR_SXS_XML_E_UNCLOSEDSTRING   equ 14062
ERROR_SXS_XML_E_UNCLOSEDCOMMENT  equ 14063
ERROR_SXS_XML_E_UNCLOSEDDECL     equ 14064
ERROR_SXS_XML_E_UNCLOSEDCDATA    equ 14065
ERROR_SXS_XML_E_RESERVEDNAMESPACE equ 14066
ERROR_SXS_XML_E_INVALIDENCODING  equ 14067
ERROR_SXS_XML_E_INVALIDSWITCH    equ 14068
ERROR_SXS_XML_E_BADXMLCASE       equ 14069
ERROR_SXS_XML_E_INVALID_STANDALONE equ 14070
ERROR_SXS_XML_E_UNEXPECTED_STANDALONE equ 14071
ERROR_SXS_XML_E_INVALID_VERSION  equ 14072
ERROR_SXS_XML_E_MISSINGEQUALS    equ 14073
ERROR_SXS_PROTECTION_RECOVERY_FAILED equ 14074
ERROR_SXS_PROTECTION_PUBLIC_KEY_TOO_SHORT equ 14075
ERROR_SXS_PROTECTION_CATALOG_NOT_VALID equ 14076
ERROR_SXS_UNTRANSLATABLE_HRESULT equ 14077
ERROR_SXS_PROTECTION_CATALOG_FILE_MISSING equ 14078
ERROR_SXS_MISSING_ASSEMBLY_IDENTITY_ATTRIBUTE equ 14079
ERROR_SXS_INVALID_ASSEMBLY_IDENTITY_ATTRIBUTE_NAME equ 14080
ERROR_SXS_ASSEMBLY_MISSING       equ 14081
ERROR_SXS_CORRUPT_ACTIVATION_STACK equ 14082
ERROR_SXS_CORRUPTION             equ 14083
ERROR_SXS_EARLY_DEACTIVATION     equ 14084
ERROR_SXS_INVALID_DEACTIVATION   equ 14085
ERROR_SXS_MULTIPLE_DEACTIVATION  equ 14086
ERROR_SXS_PROCESS_TERMINATION_REQUESTED equ 14087
ERROR_SXS_RELEASE_ACTIVATION_CONTEXT equ 14088
ERROR_SXS_SYSTEM_DEFAULT_ACTIVATION_CONTEXT_EMPTY equ 14089
ERROR_SXS_INVALID_IDENTITY_ATTRIBUTE_VALUE equ 14090
ERROR_SXS_INVALID_IDENTITY_ATTRIBUTE_NAME equ 14091
ERROR_SXS_IDENTITY_DUPLICATE_ATTRIBUTE equ 14092
ERROR_SXS_IDENTITY_PARSE_ERROR   equ 14093
ERROR_MALFORMED_SUBSTITUTION_STRING equ 14094
ERROR_SXS_INCORRECT_PUBLIC_KEY_TOKEN equ 14095
ERROR_UNMAPPED_SUBSTITUTION_STRING equ 14096
ERROR_SXS_ASSEMBLY_NOT_LOCKED    equ 14097
ERROR_SXS_COMPONENT_STORE_CORRUPT equ 14098
ERROR_ADVANCED_INSTALLER_FAILED  equ 14099
ERROR_XML_ENCODING_MISMATCH      equ 14100
ERROR_SXS_MANIFEST_IDENTITY_SAME_BUT_CONTENTS_DIFFERENT equ 14101
ERROR_SXS_IDENTITIES_DIFFERENT   equ 14102
ERROR_SXS_ASSEMBLY_IS_NOT_A_DEPLOYMENT equ 14103
ERROR_SXS_FILE_NOT_PART_OF_ASSEMBLY equ 14104
ERROR_SXS_MANIFEST_TOO_BIG       equ 14105
ERROR_SXS_SETTING_NOT_REGISTERED equ 14106
ERROR_SXS_TRANSACTION_CLOSURE_INCOMPLETE equ 14107
ERROR_SMI_PRIMITIVE_INSTALLER_FAILED equ 14108
ERROR_GENERIC_COMMAND_FAILED     equ 14109
ERROR_SXS_FILE_HASH_MISSING      equ 14110
ERROR_EVT_INVALID_CHANNEL_PATH   equ 15000
ERROR_EVT_INVALID_QUERY          equ 15001
ERROR_EVT_PUBLISHER_METADATA_NOT_FOUND equ 15002
ERROR_EVT_EVENT_TEMPLATE_NOT_FOUND equ 15003
ERROR_EVT_INVALID_PUBLISHER_NAME equ 15004
ERROR_EVT_INVALID_EVENT_DATA     equ 15005
ERROR_EVT_CHANNEL_NOT_FOUND      equ 15007
ERROR_EVT_MALFORMED_XML_TEXT     equ 15008
ERROR_EVT_SUBSCRIPTION_TO_DIRECT_CHANNEL equ 15009
ERROR_EVT_CONFIGURATION_ERROR    equ 15010
ERROR_EVT_QUERY_RESULT_STALE     equ 15011
ERROR_EVT_QUERY_RESULT_INVALID_POSITION equ 15012
ERROR_EVT_NON_VALIDATING_MSXML   equ 15013
ERROR_EVT_FILTER_ALREADYSCOPED   equ 15014
ERROR_EVT_FILTER_NOTELTSET       equ 15015
ERROR_EVT_FILTER_INVARG          equ 15016
ERROR_EVT_FILTER_INVTEST         equ 15017
ERROR_EVT_FILTER_INVTYPE         equ 15018
ERROR_EVT_FILTER_PARSEERR        equ 15019
ERROR_EVT_FILTER_UNSUPPORTEDOP   equ 15020
ERROR_EVT_FILTER_UNEXPECTEDTOKEN equ 15021
ERROR_EVT_INVALID_OPERATION_OVER_ENABLED_DIRECT_CHANNEL equ 15022
ERROR_EVT_INVALID_CHANNEL_PROPERTY_VALUE equ 15023
ERROR_EVT_INVALID_PUBLISHER_PROPERTY_VALUE equ 15024
ERROR_EVT_CHANNEL_CANNOT_ACTIVATE equ 15025
ERROR_EVT_FILTER_TOO_COMPLEX     equ 15026
ERROR_EVT_MESSAGE_NOT_FOUND      equ 15027
ERROR_EVT_MESSAGE_ID_NOT_FOUND   equ 15028
ERROR_EVT_UNRESOLVED_VALUE_INSERT equ 15029
ERROR_EVT_UNRESOLVED_PARAMETER_INSERT equ 15030
ERROR_EVT_MAX_INSERTS_REACHED    equ 15031
ERROR_EVT_EVENT_DEFINITION_NOT_FOUND equ 15032
ERROR_EVT_MESSAGE_LOCALE_NOT_FOUND equ 15033
ERROR_EVT_VERSION_TOO_OLD        equ 15034
ERROR_EVT_VERSION_TOO_NEW        equ 15035
ERROR_EVT_CANNOT_OPEN_CHANNEL_OF_QUERY equ 15036
ERROR_EVT_PUBLISHER_DISABLED     equ 15037
ERROR_EVT_FILTER_OUT_OF_RANGE    equ 15038
ERROR_EC_SUBSCRIPTION_CANNOT_ACTIVATE equ 15080
ERROR_EC_LOG_DISABLED            equ 15081
ERROR_EC_CIRCULAR_FORWARDING     equ 15082
ERROR_EC_CREDSTORE_FULL          equ 15083
ERROR_EC_CRED_NOT_FOUND          equ 15084
ERROR_EC_NO_ACTIVE_CHANNEL       equ 15085
ERROR_MUI_FILE_NOT_FOUND         equ 15100
ERROR_MUI_INVALID_FILE           equ 15101
ERROR_MUI_INVALID_RC_CONFIG      equ 15102
ERROR_MUI_INVALID_LOCALE_NAME    equ 15103
ERROR_MUI_INVALID_ULTIMATEFALLBACK_NAME equ 15104
ERROR_MUI_FILE_NOT_LOADED        equ 15105
ERROR_RESOURCE_ENUM_USER_STOP    equ 15106
ERROR_MUI_INTLSETTINGS_UILANG_NOT_INSTALLED equ 15107
ERROR_MUI_INTLSETTINGS_INVALID_LOCALE_NAME equ 15108
ERROR_MCA_INVALID_CAPABILITIES_STRING equ 15200
ERROR_MCA_INVALID_VCP_VERSION    equ 15201
ERROR_MCA_MONITOR_VIOLATES_MCCS_SPECIFICATION equ 15202
ERROR_MCA_MCCS_VERSION_MISMATCH  equ 15203
ERROR_MCA_UNSUPPORTED_MCCS_VERSION equ 15204
ERROR_MCA_INTERNAL_ERROR         equ 15205
ERROR_MCA_INVALID_TECHNOLOGY_TYPE_RETURNED equ 15206
ERROR_MCA_UNSUPPORTED_COLOR_TEMPERATURE equ 15207
ERROR_AMBIGUOUS_SYSTEM_DEVICE    equ 15250
ERROR_SYSTEM_DEVICE_NOT_FOUND    equ 15299
E_PENDING                        equ 8000000Ah
CO_E_CANT_REMOTE                 equ 80004013h
CO_E_BAD_SERVER_NAME             equ 80004014h
CO_E_WRONG_SERVER_IDENTITY       equ 80004015h
CO_E_OLE1DDE_DISABLED            equ 80004016h
CO_E_RUNAS_SYNTAX                equ 80004017h
CO_E_CREATEPROCESS_FAILURE       equ 80004018h
CO_E_RUNAS_LOGON_FAILURE         equ 8000401Ah
CO_E_LAUNCH_PERMSSION_DENIED     equ 8000401Bh
CO_E_START_SERVICE_FAILURE       equ 8000401Ch
CO_E_SERVER_START_TIMEOUT        equ 8000401Eh
CO_E_CLSREG_INCONSISTENT         equ 8000401Fh
CO_E_IIDREG_INCONSISTENT         equ 80004020h
CO_E_NOT_SUPPORTED               equ 80004021h
CO_E_RELOAD_DLL                  equ 80004022h
CO_E_MSI_ERROR                   equ 80004023h
CO_E_SERVER_PAUSED               equ 80004025h
CO_E_SERVER_NOT_PAUSED           equ 80004026h
CO_E_CLASS_DISABLED              equ 80004027h
CO_E_CLRNOTAVAILABLE             equ 80004028h
CO_E_ASYNC_WORK_REJECTED         equ 80004029h
CO_E_SERVER_INIT_TIMEOUT         equ 8000402Ah
CO_E_NO_SECCTX_IN_ACTIVATE       equ 8000402Bh
CO_E_TRACKER_CONFIG              equ 80004030h
CO_E_THREADPOOL_CONFIG           equ 80004031h
CO_E_SXS_CONFIG                  equ 80004032h
CO_E_MALFORMED_SPN               equ 80004033h
CLASS_E_NOTLICENSED              equ 80040112h
REGDB_E_BADTHREADINGMODEL        equ 80040156h
CAT_E_FIRST                      equ 80040160h
CAT_E_LAST                       equ 80040161h
CAT_E_CATIDNOEXIST               equ 80040160h
CAT_E_NODESCRIPTION              equ 80040161h
CS_E_FIRST                       equ 80040164h
CS_E_LAST                        equ 8004016Fh
CS_E_PACKAGE_NOTFOUND            equ 80040164h
CS_E_NOT_DELETABLE               equ 80040165h
CS_E_CLASS_NOTFOUND              equ 80040166h
CS_E_INVALID_VERSION             equ 80040167h
CS_E_NO_CLASSSTORE               equ 80040168h
CS_E_OBJECT_NOTFOUND             equ 80040169h
CS_E_OBJECT_ALREADY_EXISTS       equ 8004016Ah
CS_E_INVALID_PATH                equ 8004016Bh
CS_E_NETWORK_ERROR               equ 8004016Ch
CS_E_ADMIN_LIMIT_EXCEEDED        equ 8004016Dh
CS_E_SCHEMA_MISMATCH             equ 8004016Eh
CS_E_INTERNAL_ERROR              equ 8004016Fh
EVENT_E_FIRST                    equ 80040200h
EVENT_E_LAST                     equ 8004021Fh
EVENT_S_FIRST                    equ 00040200h
EVENT_S_LAST                     equ 0004021Fh
EVENT_S_SOME_SUBSCRIBERS_FAILED  equ 00040200h
EVENT_E_ALL_SUBSCRIBERS_FAILED   equ 80040201h
EVENT_S_NOSUBSCRIBERS            equ 00040202h
EVENT_E_QUERYSYNTAX              equ 80040203h
EVENT_E_QUERYFIELD               equ 80040204h
EVENT_E_INTERNALEXCEPTION        equ 80040205h
EVENT_E_INTERNALERROR            equ 80040206h
EVENT_E_INVALID_PER_USER_SID     equ 80040207h
EVENT_E_USER_EXCEPTION           equ 80040208h
EVENT_E_TOO_MANY_METHODS         equ 80040209h
EVENT_E_MISSING_EVENTCLASS       equ 8004020Ah
EVENT_E_NOT_ALL_REMOVED          equ 8004020Bh
EVENT_E_COMPLUS_NOT_INSTALLED    equ 8004020Ch
XACT_E_FIRST                     equ 8004D000h
XACT_E_LAST                      equ 8004D029h
XACT_S_FIRST                     equ 0004D000h
XACT_S_LAST                      equ 0004D010h
XACT_E_ALREADYOTHERSINGLEPHASE   equ 8004D000h
XACT_E_CANTRETAIN                equ 8004D001h
XACT_E_COMMITFAILED              equ 8004D002h
XACT_E_COMMITPREVENTED           equ 8004D003h
XACT_E_HEURISTICABORT            equ 8004D004h
XACT_E_HEURISTICCOMMIT           equ 8004D005h
XACT_E_HEURISTICDAMAGE           equ 8004D006h
XACT_E_HEURISTICDANGER           equ 8004D007h
XACT_E_ISOLATIONLEVEL            equ 8004D008h
XACT_E_NOASYNC                   equ 8004D009h
XACT_E_NOENLIST                  equ 8004D00Ah
XACT_E_NOISORETAIN               equ 8004D00Bh
XACT_E_NORESOURCE                equ 8004D00Ch
XACT_E_NOTCURRENT                equ 8004D00Dh
XACT_E_NOTRANSACTION             equ 8004D00Eh
XACT_E_NOTSUPPORTED              equ 8004D00Fh
XACT_E_UNKNOWNRMGRID             equ 8004D010h
XACT_E_WRONGSTATE                equ 8004D011h
XACT_E_WRONGUOW                  equ 8004D012h
XACT_E_XTIONEXISTS               equ 8004D013h
XACT_E_NOIMPORTOBJECT            equ 8004D014h
XACT_E_INVALIDCOOKIE             equ 8004D015h
XACT_E_INDOUBT                   equ 8004D016h
XACT_E_NOTIMEOUT                 equ 8004D017h
XACT_E_ALREADYINPROGRESS         equ 8004D018h
XACT_E_ABORTED                   equ 8004D019h
XACT_E_LOGFULL                   equ 8004D01Ah
XACT_E_TMNOTAVAILABLE            equ 8004D01Bh
XACT_E_CONNECTION_DOWN           equ 8004D01Ch
XACT_E_CONNECTION_DENIED         equ 8004D01Dh
XACT_E_REENLISTTIMEOUT           equ 8004D01Eh
XACT_E_TIP_CONNECT_FAILED        equ 8004D01Fh
XACT_E_TIP_PROTOCOL_ERROR        equ 8004D020h
XACT_E_TIP_PULL_FAILED           equ 8004D021h
XACT_E_DEST_TMNOTAVAILABLE       equ 8004D022h
XACT_E_TIP_DISABLED              equ 8004D023h
XACT_E_NETWORK_TX_DISABLED       equ 8004D024h
XACT_E_XA_TX_DISABLED            equ 8004D026h
XACT_E_UNABLE_TO_LOAD_DTC_PROXY  equ 8004D028h
XACT_E_ABORTING                  equ 8004D029h
XACT_E_CLERKNOTFOUND             equ 8004D080h
XACT_E_CLERKEXISTS               equ 8004D081h
XACT_E_RECOVERYINPROGRESS        equ 8004D082h
XACT_E_TRANSACTIONCLOSED         equ 8004D083h
XACT_E_INVALIDLSN                equ 8004D084h
XACT_E_REPLAYREQUEST             equ 8004D085h
XACT_S_ASYNC                     equ 0004D000h
XACT_S_DEFECT                    equ 0004D001h
XACT_S_READONLY                  equ 0004D002h
XACT_S_SOMENORETAIN              equ 0004D003h
XACT_S_OKINFORM                  equ 0004D004h
XACT_S_MADECHANGESCONTENT        equ 0004D005h
XACT_S_MADECHANGESINFORM         equ 0004D006h
XACT_S_ALLNORETAIN               equ 0004D007h
XACT_S_ABORTING                  equ 0004D008h
XACT_S_SINGLEPHASE               equ 0004D009h
XACT_S_LOCALLY_OK                equ 0004D00Ah
XACT_S_LASTRESOURCEMANAGER       equ 0004D010h
CONTEXT_E_FIRST                  equ 8004E000h
CONTEXT_E_LAST                   equ 8004E02Fh
CONTEXT_S_FIRST                  equ 0004E000h
CONTEXT_S_LAST                   equ 0004E02Fh
CONTEXT_E_ABORTED                equ 8004E002h
CONTEXT_E_ABORTING               equ 8004E003h
CONTEXT_E_NOCONTEXT              equ 8004E004h
CONTEXT_E_WOULD_DEADLOCK         equ 8004E005h
CONTEXT_E_SYNCH_TIMEOUT          equ 8004E006h
CONTEXT_E_OLDREF                 equ 8004E007h
CONTEXT_E_ROLENOTFOUND           equ 8004E00Ch
CONTEXT_E_TMNOTAVAILABLE         equ 8004E00Fh
CO_E_ACTIVATIONFAILED            equ 8004E021h
CO_E_ACTIVATIONFAILED_TIMEOUT    equ 8004E024h
CO_E_INITIALIZATIONFAILED        equ 8004E025h
CONTEXT_E_NOJIT                  equ 8004E026h
CONTEXT_E_NOTRANSACTION          equ 8004E027h
CO_E_THREADINGMODEL_CHANGED      equ 8004E028h
CO_E_NOIISINTRINSICS             equ 8004E029h
CO_E_NOCOOKIES                   equ 8004E02Ah
CO_E_DBERROR                     equ 8004E02Bh
CO_E_NOTPOOLED                   equ 8004E02Ch
CO_E_NOTCONSTRUCTED              equ 8004E02Dh
CO_E_NOSYNCHRONIZATION           equ 8004E02Eh
CO_E_ISOLEVELMISMATCH            equ 8004E02Fh
SCHED_S_TASK_READY               equ 00041300h
SCHED_S_TASK_RUNNING             equ 00041301h
SCHED_S_TASK_DISABLED            equ 00041302h
SCHED_S_TASK_HAS_NOT_RUN         equ 00041303h
SCHED_S_TASK_NO_MORE_RUNS        equ 00041304h
SCHED_S_TASK_NOT_SCHEDULED       equ 00041305h
SCHED_S_TASK_TERMINATED          equ 00041306h
SCHED_S_TASK_NO_VALID_TRIGGERS   equ 00041307h
SCHED_S_EVENT_TRIGGER            equ 00041308h
SCHED_E_TRIGGER_NOT_FOUND        equ 80041309h
SCHED_E_TASK_NOT_READY           equ 8004130Ah
SCHED_E_TASK_NOT_RUNNING         equ 8004130Bh
SCHED_E_SERVICE_NOT_INSTALLED    equ 8004130Ch
SCHED_E_CANNOT_OPEN_TASK         equ 8004130Dh
SCHED_E_INVALID_TASK             equ 8004130Eh
SCHED_E_ACCOUNT_NAME_NOT_FOUND   equ 80041310h
SCHED_E_ACCOUNT_DBASE_CORRUPT    equ 80041311h
SCHED_E_NO_SECURITY_SERVICES     equ 80041312h
SCHED_E_UNKNOWN_OBJECT_VERSION   equ 80041313h
SCHED_E_SERVICE_NOT_RUNNING      equ 80041315h
SCHED_E_UNEXPECTEDNODE           equ 80041316h
SCHED_E_NAMESPACE                equ 80041317h
SCHED_E_INVALIDVALUE             equ 80041318h
SCHED_E_MISSINGNODE              equ 80041319h
SCHED_E_MALFORMEDXML             equ 8004131Ah
SCHED_S_SOME_TRIGGERS_FAILED     equ 0004131Bh
SCHED_S_BATCH_LOGON_PROBLEM      equ 0004131Ch
SCHED_E_TOO_MANY_NODES           equ 8004131Dh
SCHED_E_PAST_END_BOUNDARY        equ 8004131Eh
SCHED_E_ALREADY_RUNNING          equ 8004131Fh
SCHED_E_USER_NOT_LOGGED_ON       equ 80041320h
SCHED_E_INVALID_TASK_HASH        equ 80041321h
SCHED_E_SERVICE_NOT_AVAILABLE    equ 80041322h
SCHED_E_SERVICE_TOO_BUSY         equ 80041323h
SCHED_E_TASK_ATTEMPTED           equ 80041324h
SCHED_S_TASK_QUEUED              equ 00041325h
SCHED_E_TASK_DISABLED            equ 80041326h
SCHED_E_TASK_NOT_V1_COMPAT       equ 80041327h
SCHED_E_START_ON_DEMAND          equ 80041328h
CO_S_NOTALLINTERFACES            equ 00080012h
CO_S_MACHINENAMENOTFOUND         equ 00080013h
CO_E_MISSING_DISPLAYNAME         equ 80080015h
CO_E_RUNAS_VALUE_MUST_BE_AAA     equ 80080016h
CO_E_ELEVATION_DISABLED          equ 80080017h
DISP_E_DIVBYZERO                 equ 80020012h
DISP_E_BUFFERTOOSMALL            equ 80020013h
TYPE_E_FIELDNOTFOUND             equ 80028017h
STG_E_PROPSETMISMATCHED          equ 800300F0h
STG_E_DOCFILECORRUPT             equ 80030109h
STG_E_BADBASEADDRESS             equ 80030110h
STG_E_DOCFILETOOLARGE            equ 80030111h
STG_E_NOTSIMPLEFORMAT            equ 80030112h
STG_E_INCOMPLETE                 equ 80030201h
STG_E_TERMINATED                 equ 80030202h
STG_S_BLOCK                      equ 00030201h
STG_S_RETRYNOW                   equ 00030202h
STG_S_MONITORING                 equ 00030203h
STG_S_MULTIPLEOPENS              equ 00030204h
STG_S_CONSOLIDATIONFAILED        equ 00030205h
STG_S_CANNOTCONSOLIDATE          equ 00030206h
STG_E_CSS_KEY_NOT_PRESENT        equ 80030307h
STG_E_CSS_KEY_NOT_ESTABLISHED    equ 80030308h
STG_E_CSS_SCRAMBLED_SECTOR       equ 80030309h
STG_E_CSS_REGION_MISMATCH        equ 8003030Ah
STG_E_RESETS_EXHAUSTED           equ 8003030Bh
RPC_E_VERSION_MISMATCH           equ 80010110h
RPC_E_INVALID_HEADER             equ 80010111h
RPC_E_INVALID_EXTENSION          equ 80010112h
RPC_E_INVALID_IPID               equ 80010113h
RPC_E_INVALID_OBJECT             equ 80010114h
RPC_S_CALLPENDING                equ 80010115h
RPC_S_WAITONTIMER                equ 80010116h
RPC_E_CALL_COMPLETE              equ 80010117h
RPC_E_UNSECURE_CALL              equ 80010118h
RPC_E_TOO_LATE                   equ 80010119h
RPC_E_NO_GOOD_SECURITY_PACKAGES  equ 8001011Ah
RPC_E_ACCESS_DENIED              equ 8001011Bh
RPC_E_REMOTE_DISABLED            equ 8001011Ch
RPC_E_INVALID_OBJREF             equ 8001011Dh
RPC_E_NO_CONTEXT                 equ 8001011Eh
RPC_E_TIMEOUT                    equ 8001011Fh
RPC_E_NO_SYNC                    equ 80010120h
RPC_E_FULLSIC_REQUIRED           equ 80010121h
RPC_E_INVALID_STD_NAME           equ 80010122h
CO_E_FAILEDTOIMPERSONATE         equ 80010123h
CO_E_FAILEDTOGETSECCTX           equ 80010124h
CO_E_FAILEDTOOPENTHREADTOKEN     equ 80010125h
CO_E_FAILEDTOGETTOKENINFO        equ 80010126h
CO_E_TRUSTEEDOESNTMATCHCLIENT    equ 80010127h
CO_E_FAILEDTOQUERYCLIENTBLANKET  equ 80010128h
CO_E_FAILEDTOSETDACL             equ 80010129h
CO_E_ACCESSCHECKFAILED           equ 8001012Ah
CO_E_NETACCESSAPIFAILED          equ 8001012Bh
CO_E_WRONGTRUSTEENAMESYNTAX      equ 8001012Ch
CO_E_INVALIDSID                  equ 8001012Dh
CO_E_CONVERSIONFAILED            equ 8001012Eh
CO_E_NOMATCHINGSIDFOUND          equ 8001012Fh
CO_E_LOOKUPACCSIDFAILED          equ 80010130h
CO_E_NOMATCHINGNAMEFOUND         equ 80010131h
CO_E_LOOKUPACCNAMEFAILED         equ 80010132h
CO_E_SETSERLHNDLFAILED           equ 80010133h
CO_E_FAILEDTOGETWINDIR           equ 80010134h
CO_E_PATHTOOLONG                 equ 80010135h
CO_E_FAILEDTOGENUUID             equ 80010136h
CO_E_FAILEDTOCREATEFILE          equ 80010137h
CO_E_FAILEDTOCLOSEHANDLE         equ 80010138h
CO_E_EXCEEDSYSACLLIMIT           equ 80010139h
CO_E_ACESINWRONGORDER            equ 8001013Ah
CO_E_INCOMPATIBLESTREAMVERSION   equ 8001013Bh
CO_E_FAILEDTOOPENPROCESSTOKEN    equ 8001013Ch
CO_E_DECODEFAILED                equ 8001013Dh
CO_E_ACNOTINITIALIZED            equ 8001013Fh
CO_E_CANCEL_DISABLED             equ 80010140h
ERROR_AUDITING_DISABLED          equ 0C0090001h
ERROR_ALL_SIDS_FILTERED          equ 0C0090002h
ERROR_BIZRULES_NOT_ENABLED       equ 0C0090003h
NTE_BAD_UID                      equ 80090001h
NTE_BAD_HASH                     equ 80090002h
NTE_BAD_KEY                      equ 80090003h
NTE_BAD_LEN                      equ 80090004h
NTE_BAD_DATA                     equ 80090005h
NTE_BAD_SIGNATURE                equ 80090006h
NTE_BAD_VER                      equ 80090007h
NTE_BAD_ALGID                    equ 80090008h
NTE_BAD_FLAGS                    equ 80090009h
NTE_BAD_TYPE                     equ 8009000Ah
NTE_BAD_KEY_STATE                equ 8009000Bh
NTE_BAD_HASH_STATE               equ 8009000Ch
NTE_NO_KEY                       equ 8009000Dh
NTE_NO_MEMORY                    equ 8009000Eh
NTE_EXISTS                       equ 8009000Fh
NTE_PERM                         equ 80090010h
NTE_NOT_FOUND                    equ 80090011h
NTE_DOUBLE_ENCRYPT               equ 80090012h
NTE_BAD_PROVIDER                 equ 80090013h
NTE_BAD_PROV_TYPE                equ 80090014h
NTE_BAD_PUBLIC_KEY               equ 80090015h
NTE_BAD_KEYSET                   equ 80090016h
NTE_PROV_TYPE_NOT_DEF            equ 80090017h
NTE_PROV_TYPE_ENTRY_BAD          equ 80090018h
NTE_KEYSET_NOT_DEF               equ 80090019h
NTE_KEYSET_ENTRY_BAD             equ 8009001Ah
NTE_PROV_TYPE_NO_MATCH           equ 8009001Bh
NTE_SIGNATURE_FILE_BAD           equ 8009001Ch
NTE_PROVIDER_DLL_FAIL            equ 8009001Dh
NTE_PROV_DLL_NOT_FOUND           equ 8009001Eh
NTE_BAD_KEYSET_PARAM             equ 8009001Fh
NTE_FAIL                         equ 80090020h
NTE_SYS_ERR                      equ 80090021h
NTE_SILENT_CONTEXT               equ 80090022h
NTE_TOKEN_KEYSET_STORAGE_FULL    equ 80090023h
NTE_TEMPORARY_PROFILE            equ 80090024h
NTE_FIXEDPARAMETER               equ 80090025h
NTE_INVALID_HANDLE               equ 80090026h
NTE_INVALID_PARAMETER            equ 80090027h
NTE_BUFFER_TOO_SMALL             equ 80090028h
NTE_NOT_SUPPORTED                equ 80090029h
NTE_NO_MORE_ITEMS                equ 8009002Ah
NTE_BUFFERS_OVERLAP              equ 8009002Bh
NTE_DECRYPTION_FAILURE           equ 8009002Ch
NTE_INTERNAL_ERROR               equ 8009002Dh
NTE_UI_REQUIRED                  equ 8009002Eh
NTE_HMAC_NOT_SUPPORTED           equ 8009002Fh
SEC_E_INSUFFICIENT_MEMORY        equ 80090300h
SEC_E_INVALID_HANDLE             equ 80090301h
SEC_E_UNSUPPORTED_FUNCTION       equ 80090302h
SEC_E_TARGET_UNKNOWN             equ 80090303h
SEC_E_INTERNAL_ERROR             equ 80090304h
SEC_E_SECPKG_NOT_FOUND           equ 80090305h
SEC_E_NOT_OWNER                  equ 80090306h
SEC_E_CANNOT_INSTALL             equ 80090307h
SEC_E_INVALID_TOKEN              equ 80090308h
SEC_E_CANNOT_PACK                equ 80090309h
SEC_E_QOP_NOT_SUPPORTED          equ 8009030Ah
SEC_E_NO_IMPERSONATION           equ 8009030Bh
SEC_E_LOGON_DENIED               equ 8009030Ch
SEC_E_UNKNOWN_CREDENTIALS        equ 8009030Dh
SEC_E_NO_CREDENTIALS             equ 8009030Eh
SEC_E_MESSAGE_ALTERED            equ 8009030Fh
SEC_E_OUT_OF_SEQUENCE            equ 80090310h
SEC_I_CONTINUE_NEEDED            equ 00090312h
SEC_I_COMPLETE_NEEDED            equ 00090313h
SEC_I_COMPLETE_AND_CONTINUE      equ 00090314h
SEC_I_LOCAL_LOGON                equ 00090315h
SEC_E_BAD_PKGID                  equ 80090316h
SEC_E_CONTEXT_EXPIRED            equ 80090317h
SEC_I_CONTEXT_EXPIRED            equ 00090317h
SEC_E_INCOMPLETE_MESSAGE         equ 80090318h
SEC_E_INCOMPLETE_CREDENTIALS     equ 80090320h
SEC_E_BUFFER_TOO_SMALL           equ 80090321h
SEC_I_INCOMPLETE_CREDENTIALS     equ 00090320h
SEC_I_RENEGOTIATE                equ 00090321h
SEC_E_WRONG_PRINCIPAL            equ 80090322h
SEC_I_NO_LSA_CONTEXT             equ 00090323h
SEC_E_TIME_SKEW                  equ 80090324h
SEC_E_UNTRUSTED_ROOT             equ 80090325h
SEC_E_ILLEGAL_MESSAGE            equ 80090326h
SEC_E_CERT_UNKNOWN               equ 80090327h
SEC_E_CERT_EXPIRED               equ 80090328h
SEC_E_ENCRYPT_FAILURE            equ 80090329h
SEC_E_DECRYPT_FAILURE            equ 80090330h
SEC_E_ALGORITHM_MISMATCH         equ 80090331h
SEC_E_SECURITY_QOS_FAILED        equ 80090332h
SEC_E_NO_TGT_REPLY               equ 80090334h
SEC_E_NO_IP_ADDRESSES            equ 80090335h
SEC_E_WRONG_CREDENTIAL_HANDLE    equ 80090336h
SEC_E_CRYPTO_SYSTEM_INVALID      equ 80090337h
SEC_E_MAX_REFERRALS_EXCEEDED     equ 80090338h
SEC_E_MUST_BE_KDC                equ 80090339h
SEC_E_TOO_MANY_PRINCIPALS        equ 8009033Bh
SEC_E_NO_PA_DATA                 equ 8009033Ch
SEC_E_PKINIT_NAME_MISMATCH       equ 8009033Dh
SEC_E_SMARTCARD_LOGON_REQUIRED   equ 8009033Eh
SEC_E_SHUTDOWN_IN_PROGRESS       equ 8009033Fh
SEC_E_KDC_INVALID_REQUEST        equ 80090340h
SEC_E_KDC_UNABLE_TO_REFER        equ 80090341h
SEC_E_KDC_UNKNOWN_ETYPE          equ 80090342h
SEC_E_UNSUPPORTED_PREAUTH        equ 80090343h
SEC_E_DELEGATION_REQUIRED        equ 80090345h
SEC_E_BAD_BINDINGS               equ 80090346h
SEC_E_MULTIPLE_ACCOUNTS          equ 80090347h
SEC_E_NO_KERB_KEY                equ 80090348h
SEC_E_CERT_WRONG_USAGE           equ 80090349h
SEC_E_DOWNGRADE_DETECTED         equ 80090350h
SEC_E_SMARTCARD_CERT_REVOKED     equ 80090351h
SEC_E_ISSUING_CA_UNTRUSTED       equ 80090352h
SEC_E_REVOCATION_OFFLINE_C       equ 80090353h
SEC_E_PKINIT_CLIENT_FAILURE      equ 80090354h
SEC_E_SMARTCARD_CERT_EXPIRED     equ 80090355h
SEC_E_NO_S4U_PROT_SUPPORT        equ 80090356h
SEC_E_REVOCATION_OFFLINE_KDC     equ 80090358h
SEC_E_ISSUING_CA_UNTRUSTED_KDC   equ 80090359h
SEC_E_KDC_CERT_EXPIRED           equ 8009035Ah
SEC_E_KDC_CERT_REVOKED           equ 8009035Bh
SEC_I_SIGNATURE_NEEDED           equ 0009035Ch
SEC_E_INVALID_PARAMETER          equ 8009035Dh
SEC_E_DELEGATION_POLICY          equ 8009035Eh
SEC_E_POLICY_NLTM_ONLY           equ 8009035Fh
SEC_I_NO_RENEGOTIATION           equ 00090360h
SEC_E_NO_SPM                     equ SEC_E_INTERNAL_ERROR
SEC_E_NOT_SUPPORTED              equ SEC_E_UNSUPPORTED_FUNCTION
CRYPT_E_MSG_ERROR                equ 80091001h
CRYPT_E_UNKNOWN_ALGO             equ 80091002h
CRYPT_E_OID_FORMAT               equ 80091003h
CRYPT_E_INVALID_MSG_TYPE         equ 80091004h
CRYPT_E_UNEXPECTED_ENCODING      equ 80091005h
CRYPT_E_AUTH_ATTR_MISSING        equ 80091006h
CRYPT_E_HASH_VALUE               equ 80091007h
CRYPT_E_INVALID_INDEX            equ 80091008h
CRYPT_E_ALREADY_DECRYPTED        equ 80091009h
CRYPT_E_NOT_DECRYPTED            equ 8009100Ah
CRYPT_E_RECIPIENT_NOT_FOUND      equ 8009100Bh
CRYPT_E_CONTROL_TYPE             equ 8009100Ch
CRYPT_E_ISSUER_SERIALNUMBER      equ 8009100Dh
CRYPT_E_SIGNER_NOT_FOUND         equ 8009100Eh
CRYPT_E_ATTRIBUTES_MISSING       equ 8009100Fh
CRYPT_E_STREAM_MSG_NOT_READY     equ 80091010h
CRYPT_I_NEW_PROTECTION_REQUIRED  equ 00091012h
CRYPT_E_BAD_LEN                  equ 80092001h
CRYPT_E_BAD_ENCODE               equ 80092002h
CRYPT_E_FILE_ERROR               equ 80092003h
CRYPT_E_NOT_FOUND                equ 80092004h
CRYPT_E_EXISTS                   equ 80092005h
CRYPT_E_NO_PROVIDER              equ 80092006h
CRYPT_E_SELF_SIGNED              equ 80092007h
CRYPT_E_DELETED_PREV             equ 80092008h
CRYPT_E_NO_MATCH                 equ 80092009h
CRYPT_E_UNEXPECTED_MSG_TYPE      equ 8009200Ah
CRYPT_E_NO_KEY_PROPERTY          equ 8009200Bh
CRYPT_E_NO_DECRYPT_CERT          equ 8009200Ch
CRYPT_E_BAD_MSG                  equ 8009200Dh
CRYPT_E_NO_SIGNER                equ 8009200Eh
CRYPT_E_PENDING_CLOSE            equ 8009200Fh
CRYPT_E_REVOKED                  equ 80092010h
CRYPT_E_NO_REVOCATION_DLL        equ 80092011h
CRYPT_E_NO_REVOCATION_CHECK      equ 80092012h
CRYPT_E_REVOCATION_OFFLINE       equ 80092013h
CRYPT_E_INVALID_NUMERIC_STRING   equ 80092020h
CRYPT_E_INVALID_IA5_STRING       equ 80092022h
CRYPT_E_INVALID_X500_STRING      equ 80092023h
CRYPT_E_NOT_CHAR_STRING          equ 80092024h
CRYPT_E_FILERESIZED              equ 80092025h
CRYPT_E_SECURITY_SETTINGS        equ 80092026h
CRYPT_E_NO_VERIFY_USAGE_DLL      equ 80092027h
CRYPT_E_NO_VERIFY_USAGE_CHECK    equ 80092028h
CRYPT_E_VERIFY_USAGE_OFFLINE     equ 80092029h
CRYPT_E_NOT_IN_CTL               equ 8009202Ah
CRYPT_E_NO_TRUSTED_SIGNER        equ 8009202Bh
CRYPT_E_MISSING_PUBKEY_PARA      equ 8009202Ch
CRYPT_E_OSS_ERROR                equ 80093000h
OSS_MORE_BUF                     equ 80093001h
OSS_NEGATIVE_UINTEGER            equ 80093002h
OSS_PDU_RANGE                    equ 80093003h
OSS_MORE_INPUT                   equ 80093004h
OSS_DATA_ERROR                   equ 80093005h
OSS_BAD_ARG                      equ 80093006h
OSS_BAD_VERSION                  equ 80093007h
OSS_OUT_MEMORY                   equ 80093008h
OSS_PDU_MISMATCH                 equ 80093009h
OSS_LIMITED                      equ 8009300Ah
OSS_BAD_PTR                      equ 8009300Bh
OSS_BAD_TIME                     equ 8009300Ch
OSS_INDEFINITE_NOT_SUPPORTED     equ 8009300Dh
OSS_MEM_ERROR                    equ 8009300Eh
OSS_BAD_TABLE                    equ 8009300Fh
OSS_TOO_LONG                     equ 80093010h
OSS_CONSTRAINT_VIOLATED          equ 80093011h
OSS_FATAL_ERROR                  equ 80093012h
OSS_ACCESS_SERIALIZATION_ERROR   equ 80093013h
OSS_NULL_TBL                     equ 80093014h
OSS_NULL_FCN                     equ 80093015h
OSS_BAD_ENCRULES                 equ 80093016h
OSS_UNAVAIL_ENCRULES             equ 80093017h
OSS_CANT_OPEN_TRACE_WINDOW       equ 80093018h
OSS_UNIMPLEMENTED                equ 80093019h
OSS_OID_DLL_NOT_LINKED           equ 8009301Ah
OSS_CANT_OPEN_TRACE_FILE         equ 8009301Bh
OSS_TRACE_FILE_ALREADY_OPEN      equ 8009301Ch
OSS_TABLE_MISMATCH               equ 8009301Dh
OSS_TYPE_NOT_SUPPORTED           equ 8009301Eh
OSS_REAL_DLL_NOT_LINKED          equ 8009301Fh
OSS_REAL_CODE_NOT_LINKED         equ 80093020h
OSS_OUT_OF_RANGE                 equ 80093021h
OSS_COPIER_DLL_NOT_LINKED        equ 80093022h
OSS_CONSTRAINT_DLL_NOT_LINKED    equ 80093023h
OSS_COMPARATOR_DLL_NOT_LINKED    equ 80093024h
OSS_COMPARATOR_CODE_NOT_LINKED   equ 80093025h
OSS_MEM_MGR_DLL_NOT_LINKED       equ 80093026h
OSS_PDV_DLL_NOT_LINKED           equ 80093027h
OSS_PDV_CODE_NOT_LINKED          equ 80093028h
OSS_API_DLL_NOT_LINKED           equ 80093029h
OSS_BERDER_DLL_NOT_LINKED        equ 8009302Ah
OSS_PER_DLL_NOT_LINKED           equ 8009302Bh
OSS_OPEN_TYPE_ERROR              equ 8009302Ch
OSS_MUTEX_NOT_CREATED            equ 8009302Dh
OSS_CANT_CLOSE_TRACE_FILE        equ 8009302Eh
CRYPT_E_ASN1_ERROR               equ 80093100h
CRYPT_E_ASN1_INTERNAL            equ 80093101h
CRYPT_E_ASN1_EOD                 equ 80093102h
CRYPT_E_ASN1_CORRUPT             equ 80093103h
CRYPT_E_ASN1_LARGE               equ 80093104h
CRYPT_E_ASN1_CONSTRAINT          equ 80093105h
CRYPT_E_ASN1_MEMORY              equ 80093106h
CRYPT_E_ASN1_OVERFLOW            equ 80093107h
CRYPT_E_ASN1_BADPDU              equ 80093108h
CRYPT_E_ASN1_BADARGS             equ 80093109h
CRYPT_E_ASN1_BADREAL             equ 8009310Ah
CRYPT_E_ASN1_BADTAG              equ 8009310Bh
CRYPT_E_ASN1_CHOICE              equ 8009310Ch
CRYPT_E_ASN1_RULE                equ 8009310Dh
CRYPT_E_ASN1_UTF8                equ 8009310Eh
CRYPT_E_ASN1_PDU_TYPE            equ 80093133h
CRYPT_E_ASN1_NYI                 equ 80093134h
CRYPT_E_ASN1_EXTENDED            equ 80093201h
CRYPT_E_ASN1_NOEOD               equ 80093202h
CERTSRV_E_BAD_REQUESTSUBJECT     equ 80094001h
CERTSRV_E_NO_REQUEST             equ 80094002h
CERTSRV_E_BAD_REQUESTSTATUS      equ 80094003h
CERTSRV_E_PROPERTY_EMPTY         equ 80094004h
CERTSRV_E_SERVER_SUSPENDED       equ 80094006h
CERTSRV_E_ENCODING_LENGTH        equ 80094007h
CERTSRV_E_ROLECONFLICT           equ 80094008h
CERTSRV_E_RESTRICTEDOFFICER      equ 80094009h
CERTSRV_E_NO_VALID_KRA           equ 8009400Bh
CERTSRV_E_NO_CAADMIN_DEFINED     equ 8009400Dh
CERTSRV_E_NO_DB_SESSIONS         equ 8009400Fh
CERTSRV_E_ALIGNMENT_FAULT        equ 80094010h
CERTSRV_E_ENROLL_DENIED          equ 80094011h
CERTSRV_E_TEMPLATE_DENIED        equ 80094012h
CERTSRV_E_UNSUPPORTED_CERT_TYPE  equ 80094800h
CERTSRV_E_NO_CERT_TYPE           equ 80094801h
CERTSRV_E_TEMPLATE_CONFLICT      equ 80094802h
CERTSRV_E_ARCHIVED_KEY_REQUIRED  equ 80094804h
CERTSRV_E_SMIME_REQUIRED         equ 80094805h
CERTSRV_E_BAD_RENEWAL_SUBJECT    equ 80094806h
CERTSRV_E_BAD_TEMPLATE_VERSION   equ 80094807h
CERTSRV_E_SIGNATURE_COUNT        equ 8009480Ah
CERTSRV_E_SIGNATURE_REJECTED     equ 8009480Bh
CERTSRV_E_SUBJECT_UPN_REQUIRED   equ 8009480Dh
CERTSRV_E_SUBJECT_DNS_REQUIRED   equ 8009480Fh
CERTSRV_E_KEY_LENGTH             equ 80094811h
CERTSRV_E_UNKNOWN_CERT_TYPE      equ 80094813h
CERTSRV_E_CERT_TYPE_OVERLAP      equ 80094814h
CERTSRV_E_TOO_MANY_SIGNATURES    equ 80094815h
XENROLL_E_KEY_NOT_EXPORTABLE     equ 80095000h
XENROLL_E_CANNOT_ADD_ROOT_CERT   equ 80095001h
TRUST_E_SYSTEM_ERROR             equ 80096001h
TRUST_E_NO_SIGNER_CERT           equ 80096002h
TRUST_E_COUNTER_SIGNER           equ 80096003h
TRUST_E_CERT_SIGNATURE           equ 80096004h
TRUST_E_TIME_STAMP               equ 80096005h
TRUST_E_BAD_DIGEST               equ 80096010h
TRUST_E_BASIC_CONSTRAINTS        equ 80096019h
TRUST_E_FINANCIAL_CRITERIA       equ 8009601Eh
MSSIPOTF_E_OUTOFMEMRANGE         equ 80097001h
MSSIPOTF_E_CANTGETOBJECT         equ 80097002h
MSSIPOTF_E_NOHEADTABLE           equ 80097003h
MSSIPOTF_E_BAD_MAGICNUMBER       equ 80097004h
MSSIPOTF_E_BAD_OFFSET_TABLE      equ 80097005h
MSSIPOTF_E_TABLE_TAGORDER        equ 80097006h
MSSIPOTF_E_TABLE_LONGWORD        equ 80097007h
MSSIPOTF_E_TABLES_OVERLAP        equ 80097009h
MSSIPOTF_E_TABLE_PADBYTES        equ 8009700Ah
MSSIPOTF_E_FILETOOSMALL          equ 8009700Bh
MSSIPOTF_E_TABLE_CHECKSUM        equ 8009700Ch
MSSIPOTF_E_FILE_CHECKSUM         equ 8009700Dh
MSSIPOTF_E_FAILED_POLICY         equ 80097010h
MSSIPOTF_E_FAILED_HINTS_CHECK    equ 80097011h
MSSIPOTF_E_NOT_OPENTYPE          equ 80097012h
MSSIPOTF_E_FILE                  equ 80097013h
MSSIPOTF_E_CRYPT                 equ 80097014h
MSSIPOTF_E_BADVERSION            equ 80097015h
MSSIPOTF_E_DSIG_STRUCTURE        equ 80097016h
MSSIPOTF_E_PCONST_CHECK          equ 80097017h
MSSIPOTF_E_STRUCTURE             equ 80097018h
NTE_OP_OK                        equ 0
TRUST_E_PROVIDER_UNKNOWN         equ 800B0001h
TRUST_E_ACTION_UNKNOWN           equ 800B0002h
TRUST_E_SUBJECT_FORM_UNKNOWN     equ 800B0003h
TRUST_E_SUBJECT_NOT_TRUSTED      equ 800B0004h
DIGSIG_E_ENCODE                  equ 800B0005h
DIGSIG_E_DECODE                  equ 800B0006h
DIGSIG_E_EXTENSIBILITY           equ 800B0007h
DIGSIG_E_CRYPTO                  equ 800B0008h
PERSIST_E_SIZEDEFINITE           equ 800B0009h
PERSIST_E_SIZEINDEFINITE         equ 800B000Ah
PERSIST_E_NOTSELFSIZING          equ 800B000Bh
TRUST_E_NOSIGNATURE              equ 800B0100h
CERT_E_EXPIRED                   equ 800B0101h
CERT_E_VALIDITYPERIODNESTING     equ 800B0102h
CERT_E_ROLE                      equ 800B0103h
CERT_E_PATHLENCONST              equ 800B0104h
CERT_E_CRITICAL                  equ 800B0105h
CERT_E_PURPOSE                   equ 800B0106h
CERT_E_ISSUERCHAINING            equ 800B0107h
CERT_E_MALFORMED                 equ 800B0108h
CERT_E_UNTRUSTEDROOT             equ 800B0109h
CERT_E_CHAINING                  equ 800B010Ah
TRUST_E_FAIL                     equ 800B010Bh
CERT_E_REVOKED                   equ 800B010Ch
CERT_E_UNTRUSTEDTESTROOT         equ 800B010Dh
CERT_E_REVOCATION_FAILURE        equ 800B010Eh
CERT_E_CN_NO_MATCH               equ 800B010Fh
CERT_E_WRONG_USAGE               equ 800B0110h
TRUST_E_EXPLICIT_DISTRUST        equ 800B0111h
CERT_E_UNTRUSTEDCA               equ 800B0112h
CERT_E_INVALID_POLICY            equ 800B0113h
CERT_E_INVALID_NAME              equ 800B0114h
SPAPI_E_EXPECTED_SECTION_NAME    equ 800F0000h
SPAPI_E_BAD_SECTION_NAME_LINE    equ 800F0001h
SPAPI_E_SECTION_NAME_TOO_LONG    equ 800F0002h
SPAPI_E_GENERAL_SYNTAX           equ 800F0003h
SPAPI_E_WRONG_INF_STYLE          equ 800F0100h
SPAPI_E_SECTION_NOT_FOUND        equ 800F0101h
SPAPI_E_LINE_NOT_FOUND           equ 800F0102h
SPAPI_E_NO_BACKUP                equ 800F0103h
SPAPI_E_NO_ASSOCIATED_CLASS      equ 800F0200h
SPAPI_E_CLASS_MISMATCH           equ 800F0201h
SPAPI_E_DUPLICATE_FOUND          equ 800F0202h
SPAPI_E_NO_DRIVER_SELECTED       equ 800F0203h
SPAPI_E_KEY_DOES_NOT_EXIST       equ 800F0204h
SPAPI_E_INVALID_DEVINST_NAME     equ 800F0205h
SPAPI_E_INVALID_CLASS            equ 800F0206h
SPAPI_E_DEVINST_ALREADY_EXISTS   equ 800F0207h
SPAPI_E_DEVINFO_NOT_REGISTERED   equ 800F0208h
SPAPI_E_INVALID_REG_PROPERTY     equ 800F0209h
SPAPI_E_NO_INF                   equ 800F020Ah
SPAPI_E_NO_SUCH_DEVINST          equ 800F020Bh
SPAPI_E_CANT_LOAD_CLASS_ICON     equ 800F020Ch
SPAPI_E_INVALID_CLASS_INSTALLER  equ 800F020Dh
SPAPI_E_DI_DO_DEFAULT            equ 800F020Eh
SPAPI_E_DI_NOFILECOPY            equ 800F020Fh
SPAPI_E_INVALID_HWPROFILE        equ 800F0210h
SPAPI_E_NO_DEVICE_SELECTED       equ 800F0211h
SPAPI_E_DEVINFO_LIST_LOCKED      equ 800F0212h
SPAPI_E_DEVINFO_DATA_LOCKED      equ 800F0213h
SPAPI_E_DI_BAD_PATH              equ 800F0214h
SPAPI_E_NO_CLASSINSTALL_PARAMS   equ 800F0215h
SPAPI_E_FILEQUEUE_LOCKED         equ 800F0216h
SPAPI_E_BAD_SERVICE_INSTALLSECT  equ 800F0217h
SPAPI_E_NO_CLASS_DRIVER_LIST     equ 800F0218h
SPAPI_E_NO_ASSOCIATED_SERVICE    equ 800F0219h
SPAPI_E_DEVICE_INTERFACE_ACTIVE  equ 800F021Bh
SPAPI_E_NO_SUCH_INTERFACE_CLASS  equ 800F021Eh
SPAPI_E_INVALID_MACHINENAME      equ 800F0220h
SPAPI_E_REMOTE_COMM_FAILURE      equ 800F0221h
SPAPI_E_MACHINE_UNAVAILABLE      equ 800F0222h
SPAPI_E_NO_CONFIGMGR_SERVICES    equ 800F0223h
SPAPI_E_INVALID_COINSTALLER      equ 800F0227h
SPAPI_E_NO_COMPAT_DRIVERS        equ 800F0228h
SPAPI_E_NO_DEVICE_ICON           equ 800F0229h
SPAPI_E_INVALID_INF_LOGCONFIG    equ 800F022Ah
SPAPI_E_DI_DONT_INSTALL          equ 800F022Bh
SPAPI_E_INVALID_FILTER_DRIVER    equ 800F022Ch
SPAPI_E_NON_WINDOWS_NT_DRIVER    equ 800F022Dh
SPAPI_E_NON_WINDOWS_DRIVER       equ 800F022Eh
SPAPI_E_NO_CATALOG_FOR_OEM_INF   equ 800F022Fh
SPAPI_E_NOT_DISABLEABLE          equ 800F0231h
SPAPI_E_CANT_REMOVE_DEVINST      equ 800F0232h
SPAPI_E_INVALID_TARGET           equ 800F0233h
SPAPI_E_DRIVER_NONNATIVE         equ 800F0234h
SPAPI_E_IN_WOW64                 equ 800F0235h
SPAPI_E_INCORRECTLY_COPIED_INF   equ 800F0237h
SPAPI_E_SCE_DISABLED             equ 800F0238h
SPAPI_E_UNKNOWN_EXCEPTION        equ 800F0239h
SPAPI_E_PNP_REGISTRY_ERROR       equ 800F023Ah
SPAPI_E_INF_IN_USE_BY_DEVICES    equ 800F023Dh
SPAPI_E_DI_FUNCTION_OBSOLETE     equ 800F023Eh
SPAPI_E_NO_AUTHENTICODE_CATALOG  equ 800F023Fh
SPAPI_E_AUTHENTICODE_DISALLOWED  equ 800F0240h
SPAPI_E_DRIVER_STORE_ADD_FAILED  equ 800F0247h
SPAPI_E_DEVICE_INSTALL_BLOCKED   equ 800F0248h
SPAPI_E_DRIVER_INSTALL_BLOCKED   equ 800F0249h
SPAPI_E_WRONG_INF_TYPE           equ 800F024Ah
SPAPI_E_ERROR_NOT_INSTALLED      equ 800F1000h
SCARD_S_SUCCESS                  equ NO_ERROR
SCARD_F_INTERNAL_ERROR           equ 80100001h
SCARD_E_CANCELLED                equ 80100002h
SCARD_E_INVALID_HANDLE           equ 80100003h
SCARD_E_INVALID_PARAMETER        equ 80100004h
SCARD_E_INVALID_TARGET           equ 80100005h
SCARD_E_NO_MEMORY                equ 80100006h
SCARD_F_WAITED_TOO_LONG          equ 80100007h
SCARD_E_INSUFFICIENT_BUFFER      equ 80100008h
SCARD_E_UNKNOWN_READER           equ 80100009h
SCARD_E_TIMEOUT                  equ 8010000Ah
SCARD_E_SHARING_VIOLATION        equ 8010000Bh
SCARD_E_NO_SMARTCARD             equ 8010000Ch
SCARD_E_UNKNOWN_CARD             equ 8010000Dh
SCARD_E_CANT_DISPOSE             equ 8010000Eh
SCARD_E_PROTO_MISMATCH           equ 8010000Fh
SCARD_E_NOT_READY                equ 80100010h
SCARD_E_INVALID_VALUE            equ 80100011h
SCARD_E_SYSTEM_CANCELLED         equ 80100012h
SCARD_F_COMM_ERROR               equ 80100013h
SCARD_F_UNKNOWN_ERROR            equ 80100014h
SCARD_E_INVALID_ATR              equ 80100015h
SCARD_E_NOT_TRANSACTED           equ 80100016h
SCARD_E_READER_UNAVAILABLE       equ 80100017h
SCARD_P_SHUTDOWN                 equ 80100018h
SCARD_E_PCI_TOO_SMALL            equ 80100019h
SCARD_E_READER_UNSUPPORTED       equ 8010001Ah
SCARD_E_DUPLICATE_READER         equ 8010001Bh
SCARD_E_CARD_UNSUPPORTED         equ 8010001Ch
SCARD_E_NO_SERVICE               equ 8010001Dh
SCARD_E_SERVICE_STOPPED          equ 8010001Eh
SCARD_E_UNEXPECTED               equ 8010001Fh
SCARD_E_ICC_INSTALLATION         equ 80100020h
SCARD_E_ICC_CREATEORDER          equ 80100021h
SCARD_E_UNSUPPORTED_FEATURE      equ 80100022h
SCARD_E_DIR_NOT_FOUND            equ 80100023h
SCARD_E_FILE_NOT_FOUND           equ 80100024h
SCARD_E_NO_DIR                   equ 80100025h
SCARD_E_NO_FILE                  equ 80100026h
SCARD_E_NO_ACCESS                equ 80100027h
SCARD_E_WRITE_TOO_MANY           equ 80100028h
SCARD_E_BAD_SEEK                 equ 80100029h
SCARD_E_INVALID_CHV              equ 8010002Ah
SCARD_E_UNKNOWN_RES_MNG          equ 8010002Bh
SCARD_E_NO_SUCH_CERTIFICATE      equ 8010002Ch
SCARD_E_CERTIFICATE_UNAVAILABLE  equ 8010002Dh
SCARD_E_NO_READERS_AVAILABLE     equ 8010002Eh
SCARD_E_COMM_DATA_LOST           equ 8010002Fh
SCARD_E_NO_KEY_CONTAINER         equ 80100030h
SCARD_E_SERVER_TOO_BUSY          equ 80100031h
SCARD_W_UNSUPPORTED_CARD         equ 80100065h
SCARD_W_UNRESPONSIVE_CARD        equ 80100066h
SCARD_W_UNPOWERED_CARD           equ 80100067h
SCARD_W_RESET_CARD               equ 80100068h
SCARD_W_REMOVED_CARD             equ 80100069h
SCARD_W_SECURITY_VIOLATION       equ 8010006Ah
SCARD_W_WRONG_CHV                equ 8010006Bh
SCARD_W_CHV_BLOCKED              equ 8010006Ch
SCARD_W_EOF                      equ 8010006Dh
SCARD_W_CANCELLED_BY_USER        equ 8010006Eh
SCARD_W_CARD_NOT_AUTHENTICATED   equ 8010006Fh
SCARD_W_CACHE_ITEM_NOT_FOUND     equ 80100070h
SCARD_W_CACHE_ITEM_STALE         equ 80100071h
SCARD_W_CACHE_ITEM_TOO_BIG       equ 80100072h
COMADMIN_E_OBJECTERRORS          equ 80110401h
COMADMIN_E_OBJECTINVALID         equ 80110402h
COMADMIN_E_KEYMISSING            equ 80110403h
COMADMIN_E_ALREADYINSTALLED      equ 80110404h
COMADMIN_E_APP_FILE_WRITEFAIL    equ 80110407h
COMADMIN_E_APP_FILE_READFAIL     equ 80110408h
COMADMIN_E_APP_FILE_VERSION      equ 80110409h
COMADMIN_E_BADPATH               equ 8011040Ah
COMADMIN_E_APPLICATIONEXISTS     equ 8011040Bh
COMADMIN_E_ROLEEXISTS            equ 8011040Ch
COMADMIN_E_CANTCOPYFILE          equ 8011040Dh
COMADMIN_E_NOUSER                equ 8011040Fh
COMADMIN_E_INVALIDUSERIDS        equ 80110410h
COMADMIN_E_NOREGISTRYCLSID       equ 80110411h
COMADMIN_E_BADREGISTRYPROGID     equ 80110412h
COMADMIN_E_AUTHENTICATIONLEVEL   equ 80110413h
COMADMIN_E_USERPASSWDNOTVALID    equ 80110414h
COMADMIN_E_CLSIDORIIDMISMATCH    equ 80110418h
COMADMIN_E_REMOTEINTERFACE       equ 80110419h
COMADMIN_E_DLLREGISTERSERVER     equ 8011041Ah
COMADMIN_E_NOSERVERSHARE         equ 8011041Bh
COMADMIN_E_DLLLOADFAILED         equ 8011041Dh
COMADMIN_E_BADREGISTRYLIBID      equ 8011041Eh
COMADMIN_E_APPDIRNOTFOUND        equ 8011041Fh
COMADMIN_E_REGISTRARFAILED       equ 80110423h
COMADMIN_E_COMPFILE_LOADDLLFAIL  equ 80110425h
COMADMIN_E_COMPFILE_GETCLASSOBJ  equ 80110426h
COMADMIN_E_COMPFILE_BADTLB       equ 80110428h
COMADMIN_E_NOTCHANGEABLE         equ 8011042Ah
COMADMIN_E_NOTDELETEABLE         equ 8011042Bh
COMADMIN_E_SESSION               equ 8011042Ch
COMADMIN_E_COMP_MOVE_LOCKED      equ 8011042Dh
COMADMIN_E_COMP_MOVE_BAD_DEST    equ 8011042Eh
COMADMIN_E_REGISTERTLB           equ 80110430h
COMADMIN_E_SYSTEMAPP             equ 80110433h
COMADMIN_E_COMPFILE_NOREGISTRAR  equ 80110434h
COMADMIN_E_COREQCOMPINSTALLED    equ 80110435h
COMADMIN_E_SERVICENOTINSTALLED   equ 80110436h
COMADMIN_E_PROPERTYSAVEFAILED    equ 80110437h
COMADMIN_E_OBJECTEXISTS          equ 80110438h
COMADMIN_E_COMPONENTEXISTS       equ 80110439h
COMADMIN_E_REGFILE_CORRUPT       equ 8011043Bh
COMADMIN_E_PROPERTY_OVERFLOW     equ 8011043Ch
COMADMIN_E_NOTINREGISTRY         equ 8011043Eh
COMADMIN_E_OBJECTNOTPOOLABLE     equ 8011043Fh
COMADMIN_E_APPLID_MATCHES_CLSID  equ 80110446h
COMADMIN_E_ROLE_DOES_NOT_EXIST   equ 80110447h
COMADMIN_E_CAN_NOT_START_APP     equ 8011044Bh
COMADMIN_E_BASE_PARTITION_ONLY   equ 80110450h
COMADMIN_E_START_APP_DISABLED    equ 80110451h
COMADMIN_E_CAT_PARTITION_IN_USE  equ 80110459h
COMADMIN_E_REGDB_NOTINITIALIZED  equ 80110472h
COMADMIN_E_REGDB_NOTOPEN         equ 80110473h
COMADMIN_E_REGDB_SYSTEMERR       equ 80110474h
COMADMIN_E_REGDB_ALREADYRUNNING  equ 80110475h
COMADMIN_E_MIG_SCHEMANOTFOUND    equ 80110481h
COMADMIN_E_CAT_BITNESSMISMATCH   equ 80110482h
COMADMIN_E_CAT_WRONGAPPBITNESS   equ 80110484h
COMADMIN_E_CAT_SERVERFAULT       equ 80110486h
COMQC_E_APPLICATION_NOT_QUEUED   equ 80110600h
COMQC_E_NO_QUEUEABLE_INTERFACES  equ 80110601h
COMQC_E_NO_IPERSISTSTREAM        equ 80110603h
COMQC_E_BAD_MESSAGE              equ 80110604h
COMQC_E_UNAUTHENTICATED          equ 80110605h
COMQC_E_UNTRUSTED_ENQUEUER       equ 80110606h
MSDTC_E_DUPLICATE_RESOURCE       equ 80110701h
COMADMIN_E_APP_NOT_RUNNING       equ 8011080Ah
COMADMIN_E_INVALID_PARTITION     equ 8011080Bh
COMADMIN_E_USER_IN_SET           equ 8011080Eh
COMADMIN_E_PROGIDINUSEBYCLSID    equ 80110815h
COMADMIN_E_PARTITION_MSI_ONLY    equ 80110819h
COMADMIN_E_COMP_MOVE_SOURCE      equ 8011081Ch
COMADMIN_E_COMP_MOVE_DEST        equ 8011081Dh
COMADMIN_E_COMP_MOVE_PRIVATE     equ 8011081Eh
COMADMIN_E_PRIVATE_ACCESSDENIED  equ 80110821h
COMADMIN_E_SAFERINVALID          equ 80110822h
COMADMIN_E_PARTITIONS_DISABLED   equ 80110824h
ERROR_FLT_IO_COMPLETE            equ 001F0001h
ERROR_FLT_NO_HANDLER_DEFINED     equ 801F0001h
ERROR_FLT_DISALLOW_FAST_IO       equ 801F0004h
ERROR_FLT_INVALID_NAME_REQUEST   equ 801F0005h
ERROR_FLT_NOT_INITIALIZED        equ 801F0007h
ERROR_FLT_FILTER_NOT_READY       equ 801F0008h
ERROR_FLT_INTERNAL_ERROR         equ 801F000Ah
ERROR_FLT_DELETING_OBJECT        equ 801F000Bh
ERROR_FLT_MUST_BE_NONPAGED_POOL  equ 801F000Ch
ERROR_FLT_DUPLICATE_ENTRY        equ 801F000Dh
ERROR_FLT_CBDQ_DISABLED          equ 801F000Eh
ERROR_FLT_DO_NOT_ATTACH          equ 801F000Fh
ERROR_FLT_DO_NOT_DETACH          equ 801F0010h
ERROR_FLT_FILTER_NOT_FOUND       equ 801F0013h
ERROR_FLT_VOLUME_NOT_FOUND       equ 801F0014h
ERROR_FLT_INSTANCE_NOT_FOUND     equ 801F0015h
ERROR_FLT_NAME_CACHE_MISS        equ 801F0018h
ERROR_FLT_NO_DEVICE_OBJECT       equ 801F0019h
ERROR_FLT_ALREADY_ENLISTED       equ 801F001Bh
ERROR_FLT_NO_WAITER_FOR_REPLY    equ 801F0020h
DWM_E_COMPOSITIONDISABLED        equ 80263001h
DWM_E_REMOTING_NOT_SUPPORTED     equ 80263002h
DWM_E_NOT_QUEUING_PRESENTS       equ 80263004h
ERROR_MONITOR_NO_DESCRIPTOR      equ 80261001h
ERROR_GRAPHICS_PRESENT_OCCLUDED  equ 0C0262006h
ERROR_GRAPHICS_PRESENT_DENIED    equ 0C0262007h
ERROR_GRAPHICS_DRIVER_MISMATCH   equ 0C0262009h
ERROR_GRAPHICS_NO_VIDEO_MEMORY   equ 0C0262100h
ERROR_GRAPHICS_CANT_LOCK_MEMORY  equ 0C0262101h
ERROR_GRAPHICS_ALLOCATION_BUSY   equ 0C0262102h
ERROR_GRAPHICS_TRY_AGAIN_LATER   equ 0C0262104h
ERROR_GRAPHICS_TRY_AGAIN_NOW     equ 0C0262105h
ERROR_GRAPHICS_INVALID_VIDPN     equ 0C0262303h
ERROR_GRAPHICS_MODE_NOT_PINNED   equ 00262307h
ERROR_GRAPHICS_STALE_MODESET     equ 0C0262320h
ERROR_GRAPHICS_NO_VIDPNMGR       equ 0C0262335h
ERROR_GRAPHICS_NO_ACTIVE_VIDPN   equ 0C0262336h
ERROR_GRAPHICS_INVALID_STRIDE    equ 0C026233Ch
ERROR_GRAPHICS_DATASET_IS_EMPTY  equ 0026234Bh
ERROR_GRAPHICS_START_DEFERRED    equ 4026243Ah
ERROR_GRAPHICS_PVP_HFS_FAILED    equ 0C0262511h
ERROR_GRAPHICS_OPM_INVALID_SRM   equ 0C0262512h
ERROR_GRAPHICS_INVALID_POINTER   equ 0C02625E4h
ERROR_GRAPHICS_INTERNAL_ERROR    equ 0C02625E7h
TPM_E_ERROR_MASK                 equ 80280000h
TPM_E_AUTHFAIL                   equ 80280001h
TPM_E_BADINDEX                   equ 80280002h
TPM_E_BAD_PARAMETER              equ 80280003h
TPM_E_AUDITFAILURE               equ 80280004h
TPM_E_CLEAR_DISABLED             equ 80280005h
TPM_E_DEACTIVATED                equ 80280006h
TPM_E_DISABLED                   equ 80280007h
TPM_E_DISABLED_CMD               equ 80280008h
TPM_E_FAIL                       equ 80280009h
TPM_E_BAD_ORDINAL                equ 8028000Ah
TPM_E_INSTALL_DISABLED           equ 8028000Bh
TPM_E_INVALID_KEYHANDLE          equ 8028000Ch
TPM_E_KEYNOTFOUND                equ 8028000Dh
TPM_E_INAPPROPRIATE_ENC          equ 8028000Eh
TPM_E_MIGRATEFAIL                equ 8028000Fh
TPM_E_INVALID_PCR_INFO           equ 80280010h
TPM_E_NOSPACE                    equ 80280011h
TPM_E_NOSRK                      equ 80280012h
TPM_E_NOTSEALED_BLOB             equ 80280013h
TPM_E_OWNER_SET                  equ 80280014h
TPM_E_RESOURCES                  equ 80280015h
TPM_E_SHORTRANDOM                equ 80280016h
TPM_E_SIZE                       equ 80280017h
TPM_E_WRONGPCRVAL                equ 80280018h
TPM_E_BAD_PARAM_SIZE             equ 80280019h
TPM_E_SHA_THREAD                 equ 8028001Ah
TPM_E_SHA_ERROR                  equ 8028001Bh
TPM_E_FAILEDSELFTEST             equ 8028001Ch
TPM_E_AUTH2FAIL                  equ 8028001Dh
TPM_E_BADTAG                     equ 8028001Eh
TPM_E_IOERROR                    equ 8028001Fh
TPM_E_ENCRYPT_ERROR              equ 80280020h
TPM_E_DECRYPT_ERROR              equ 80280021h
TPM_E_INVALID_AUTHHANDLE         equ 80280022h
TPM_E_NO_ENDORSEMENT             equ 80280023h
TPM_E_INVALID_KEYUSAGE           equ 80280024h
TPM_E_WRONG_ENTITYTYPE           equ 80280025h
TPM_E_INVALID_POSTINIT           equ 80280026h
TPM_E_INAPPROPRIATE_SIG          equ 80280027h
TPM_E_BAD_KEY_PROPERTY           equ 80280028h
TPM_E_BAD_MIGRATION              equ 80280029h
TPM_E_BAD_SCHEME                 equ 8028002Ah
TPM_E_BAD_DATASIZE               equ 8028002Bh
TPM_E_BAD_MODE                   equ 8028002Ch
TPM_E_BAD_PRESENCE               equ 8028002Dh
TPM_E_BAD_VERSION                equ 8028002Eh
TPM_E_NO_WRAP_TRANSPORT          equ 8028002Fh
TPM_E_AUDITFAIL_UNSUCCESSFUL     equ 80280030h
TPM_E_AUDITFAIL_SUCCESSFUL       equ 80280031h
TPM_E_NOTRESETABLE               equ 80280032h
TPM_E_NOTLOCAL                   equ 80280033h
TPM_E_BAD_TYPE                   equ 80280034h
TPM_E_INVALID_RESOURCE           equ 80280035h
TPM_E_NOTFIPS                    equ 80280036h
TPM_E_INVALID_FAMILY             equ 80280037h
TPM_E_NO_NV_PERMISSION           equ 80280038h
TPM_E_REQUIRES_SIGN              equ 80280039h
TPM_E_KEY_NOTSUPPORTED           equ 8028003Ah
TPM_E_AUTH_CONFLICT              equ 8028003Bh
TPM_E_AREA_LOCKED                equ 8028003Ch
TPM_E_BAD_LOCALITY               equ 8028003Dh
TPM_E_READ_ONLY                  equ 8028003Eh
TPM_E_PER_NOWRITE                equ 8028003Fh
TPM_E_FAMILYCOUNT                equ 80280040h
TPM_E_WRITE_LOCKED               equ 80280041h
TPM_E_BAD_ATTRIBUTES             equ 80280042h
TPM_E_INVALID_STRUCTURE          equ 80280043h
TPM_E_KEY_OWNER_CONTROL          equ 80280044h
TPM_E_BAD_COUNTER                equ 80280045h
TPM_E_NOT_FULLWRITE              equ 80280046h
TPM_E_CONTEXT_GAP                equ 80280047h
TPM_E_MAXNVWRITES                equ 80280048h
TPM_E_NOOPERATOR                 equ 80280049h
TPM_E_RESOURCEMISSING            equ 8028004Ah
TPM_E_DELEGATE_LOCK              equ 8028004Bh
TPM_E_DELEGATE_FAMILY            equ 8028004Ch
TPM_E_DELEGATE_ADMIN             equ 8028004Dh
TPM_E_TRANSPORT_NOTEXCLUSIVE     equ 8028004Eh
TPM_E_OWNER_CONTROL              equ 8028004Fh
TPM_E_DAA_RESOURCES              equ 80280050h
TPM_E_DAA_INPUT_DATA0            equ 80280051h
TPM_E_DAA_INPUT_DATA1            equ 80280052h
TPM_E_DAA_ISSUER_SETTINGS        equ 80280053h
TPM_E_DAA_TPM_SETTINGS           equ 80280054h
TPM_E_DAA_STAGE                  equ 80280055h
TPM_E_DAA_ISSUER_VALIDITY        equ 80280056h
TPM_E_DAA_WRONG_W                equ 80280057h
TPM_E_BAD_HANDLE                 equ 80280058h
TPM_E_BAD_DELEGATE               equ 80280059h
TPM_E_BADCONTEXT                 equ 8028005Ah
TPM_E_TOOMANYCONTEXTS            equ 8028005Bh
TPM_E_MA_TICKET_SIGNATURE        equ 8028005Ch
TPM_E_MA_DESTINATION             equ 8028005Dh
TPM_E_MA_SOURCE                  equ 8028005Eh
TPM_E_MA_AUTHORITY               equ 8028005Fh
TPM_E_PERMANENTEK                equ 80280061h
TPM_E_BAD_SIGNATURE              equ 80280062h
TPM_E_NOCONTEXTSPACE             equ 80280063h
TPM_E_COMMAND_BLOCKED            equ 80280400h
TPM_E_INVALID_HANDLE             equ 80280401h
TPM_E_DUPLICATE_VHANDLE          equ 80280402h
TPM_E_EMBEDDED_COMMAND_BLOCKED   equ 80280403h
TPM_E_RETRY                      equ 80280800h
TPM_E_NEEDS_SELFTEST             equ 80280801h
TPM_E_DOING_SELFTEST             equ 80280802h
TPM_E_DEFEND_LOCK_RUNNING        equ 80280803h
TBS_E_INTERNAL_ERROR             equ 80284001h
TBS_E_BAD_PARAMETER              equ 80284002h
TBS_E_INVALID_OUTPUT_POINTER     equ 80284003h
TBS_E_INVALID_CONTEXT            equ 80284004h
TBS_E_INSUFFICIENT_BUFFER        equ 80284005h
TBS_E_IOERROR                    equ 80284006h
TBS_E_INVALID_CONTEXT_PARAM      equ 80284007h
TBS_E_SERVICE_NOT_RUNNING        equ 80284008h
TBS_E_TOO_MANY_TBS_CONTEXTS      equ 80284009h
TBS_E_TOO_MANY_RESOURCES         equ 8028400Ah
TBS_E_SERVICE_START_PENDING      equ 8028400Bh
TBS_E_PPI_NOT_SUPPORTED          equ 8028400Ch
TBS_E_COMMAND_CANCELED           equ 8028400Dh
TBS_E_BUFFER_TOO_LARGE           equ 8028400Eh
TBS_E_TPM_NOT_FOUND              equ 8028400Fh
TBS_E_SERVICE_DISABLED           equ 80284010h
TPMAPI_E_INVALID_STATE           equ 80290100h
TPMAPI_E_NOT_ENOUGH_DATA         equ 80290101h
TPMAPI_E_TOO_MUCH_DATA           equ 80290102h
TPMAPI_E_INVALID_OUTPUT_POINTER  equ 80290103h
TPMAPI_E_INVALID_PARAMETER       equ 80290104h
TPMAPI_E_OUT_OF_MEMORY           equ 80290105h
TPMAPI_E_BUFFER_TOO_SMALL        equ 80290106h
TPMAPI_E_INTERNAL_ERROR          equ 80290107h
TPMAPI_E_ACCESS_DENIED           equ 80290108h
TPMAPI_E_AUTHORIZATION_FAILED    equ 80290109h
TPMAPI_E_INVALID_CONTEXT_HANDLE  equ 8029010Ah
TPMAPI_E_TPM_COMMAND_ERROR       equ 8029010Ch
TPMAPI_E_MESSAGE_TOO_LARGE       equ 8029010Dh
TPMAPI_E_INVALID_ENCODING        equ 8029010Eh
TPMAPI_E_INVALID_KEY_SIZE        equ 8029010Fh
TPMAPI_E_ENCRYPTION_FAILED       equ 80290110h
TPMAPI_E_INVALID_KEY_PARAMS      equ 80290111h
TPMAPI_E_INVALID_PCR_INDEX       equ 80290113h
TPMAPI_E_INVALID_DELEGATE_BLOB   equ 80290114h
TPMAPI_E_INVALID_CONTEXT_PARAMS  equ 80290115h
TPMAPI_E_INVALID_KEY_BLOB        equ 80290116h
TPMAPI_E_INVALID_PCR_DATA        equ 80290117h
TPMAPI_E_INVALID_OWNER_AUTH      equ 80290118h
TPMAPI_E_FIPS_RNG_CHECK_FAILED   equ 80290119h
TBSIMP_E_BUFFER_TOO_SMALL        equ 80290200h
TBSIMP_E_CLEANUP_FAILED          equ 80290201h
TBSIMP_E_INVALID_CONTEXT_HANDLE  equ 80290202h
TBSIMP_E_INVALID_CONTEXT_PARAM   equ 80290203h
TBSIMP_E_TPM_ERROR               equ 80290204h
TBSIMP_E_HASH_BAD_KEY            equ 80290205h
TBSIMP_E_DUPLICATE_VHANDLE       equ 80290206h
TBSIMP_E_INVALID_OUTPUT_POINTER  equ 80290207h
TBSIMP_E_INVALID_PARAMETER       equ 80290208h
TBSIMP_E_RPC_INIT_FAILED         equ 80290209h
TBSIMP_E_SCHEDULER_NOT_RUNNING   equ 8029020Ah
TBSIMP_E_COMMAND_CANCELED        equ 8029020Bh
TBSIMP_E_OUT_OF_MEMORY           equ 8029020Ch
TBSIMP_E_LIST_NO_MORE_ITEMS      equ 8029020Dh
TBSIMP_E_LIST_NOT_FOUND          equ 8029020Eh
TBSIMP_E_NOT_ENOUGH_SPACE        equ 8029020Fh
TBSIMP_E_COMMAND_FAILED          equ 80290211h
TBSIMP_E_UNKNOWN_ORDINAL         equ 80290212h
TBSIMP_E_RESOURCE_EXPIRED        equ 80290213h
TBSIMP_E_INVALID_RESOURCE        equ 80290214h
TBSIMP_E_NOTHING_TO_UNLOAD       equ 80290215h
TBSIMP_E_HASH_TABLE_FULL         equ 80290216h
TBSIMP_E_TOO_MANY_TBS_CONTEXTS   equ 80290217h
TBSIMP_E_TOO_MANY_RESOURCES      equ 80290218h
TBSIMP_E_PPI_NOT_SUPPORTED       equ 80290219h
TBSIMP_E_TPM_INCOMPATIBLE        equ 8029021Ah
TPM_E_PPI_ACPI_FAILURE           equ 80290300h
TPM_E_PPI_USER_ABORT             equ 80290301h
TPM_E_PPI_BIOS_FAILURE           equ 80290302h
TPM_E_PPI_NOT_SUPPORTED          equ 80290303h
PLA_E_DCS_NOT_FOUND              equ 80300002h
PLA_E_DCS_IN_USE                 equ 803000AAh
PLA_E_TOO_MANY_FOLDERS           equ 80300045h
PLA_E_NO_MIN_DISK                equ 80300070h
PLA_E_DCS_ALREADY_EXISTS         equ 803000B7h
PLA_S_PROPERTY_IGNORED           equ 00300100h
PLA_E_PROPERTY_CONFLICT          equ 80300101h
PLA_E_DCS_SINGLETON_REQUIRED     equ 80300102h
PLA_E_CREDENTIALS_REQUIRED       equ 80300103h
PLA_E_DCS_NOT_RUNNING            equ 80300104h
PLA_E_CONFLICT_INCL_EXCL_API     equ 80300105h
PLA_E_NETWORK_EXE_NOT_VALID      equ 80300106h
PLA_E_EXE_ALREADY_CONFIGURED     equ 80300107h
PLA_E_EXE_PATH_NOT_VALID         equ 80300108h
PLA_E_DC_ALREADY_EXISTS          equ 80300109h
PLA_E_DCS_START_WAIT_TIMEOUT     equ 8030010Ah
PLA_E_DC_START_WAIT_TIMEOUT      equ 8030010Bh
PLA_E_REPORT_WAIT_TIMEOUT        equ 8030010Ch
PLA_E_NO_DUPLICATES              equ 8030010Dh
PLA_E_EXE_FULL_PATH_REQUIRED     equ 8030010Eh
PLA_E_INVALID_SESSION_NAME       equ 8030010Fh
PLA_E_PLA_CHANNEL_NOT_ENABLED    equ 80300110h
PLA_E_RULES_MANAGER_FAILED       equ 80300112h
PLA_E_CABAPI_FAILURE             equ 80300113h
FVE_E_LOCKED_VOLUME              equ 80310000h
FVE_E_NOT_ENCRYPTED              equ 80310001h
FVE_E_NO_TPM_BIOS                equ 80310002h
FVE_E_NO_MBR_METRIC              equ 80310003h
FVE_E_NO_BOOTSECTOR_METRIC       equ 80310004h
FVE_E_NO_BOOTMGR_METRIC          equ 80310005h
FVE_E_WRONG_BOOTMGR              equ 80310006h
FVE_E_SECURE_KEY_REQUIRED        equ 80310007h
FVE_E_NOT_ACTIVATED              equ 80310008h
FVE_E_ACTION_NOT_ALLOWED         equ 80310009h
FVE_E_AD_SCHEMA_NOT_INSTALLED    equ 8031000Ah
FVE_E_AD_INVALID_DATATYPE        equ 8031000Bh
FVE_E_AD_INVALID_DATASIZE        equ 8031000Ch
FVE_E_AD_NO_VALUES               equ 8031000Dh
FVE_E_AD_ATTR_NOT_SET            equ 8031000Eh
FVE_E_AD_GUID_NOT_FOUND          equ 8031000Fh
FVE_E_BAD_INFORMATION            equ 80310010h
FVE_E_TOO_SMALL                  equ 80310011h
FVE_E_SYSTEM_VOLUME              equ 80310012h
FVE_E_FAILED_WRONG_FS            equ 80310013h
FVE_E_FAILED_BAD_FS              equ 80310014h
FVE_E_NOT_SUPPORTED              equ 80310015h
FVE_E_BAD_DATA                   equ 80310016h
FVE_E_VOLUME_NOT_BOUND           equ 80310017h
FVE_E_TPM_NOT_OWNED              equ 80310018h
FVE_E_NOT_DATA_VOLUME            equ 80310019h
FVE_E_AD_INSUFFICIENT_BUFFER     equ 8031001Ah
FVE_E_CONV_READ                  equ 8031001Bh
FVE_E_CONV_WRITE                 equ 8031001Ch
FVE_E_KEY_REQUIRED               equ 8031001Dh
FVE_E_CLUSTERING_NOT_SUPPORTED   equ 8031001Eh
FVE_E_VOLUME_BOUND_ALREADY       equ 8031001Fh
FVE_E_OS_NOT_PROTECTED           equ 80310020h
FVE_E_PROTECTION_DISABLED        equ 80310021h
FVE_E_RECOVERY_KEY_REQUIRED      equ 80310022h
FVE_E_FOREIGN_VOLUME             equ 80310023h
FVE_E_OVERLAPPED_UPDATE          equ 80310024h
FVE_E_TPM_SRK_AUTH_NOT_ZERO      equ 80310025h
FVE_E_FAILED_SECTOR_SIZE         equ 80310026h
FVE_E_FAILED_AUTHENTICATION      equ 80310027h
FVE_E_NOT_OS_VOLUME              equ 80310028h
FVE_E_AUTOUNLOCK_ENABLED         equ 80310029h
FVE_E_WRONG_BOOTSECTOR           equ 8031002Ah
FVE_E_WRONG_SYSTEM_FS            equ 8031002Bh
FVE_E_POLICY_PASSWORD_REQUIRED   equ 8031002Ch
FVE_E_CANNOT_SET_FVEK_ENCRYPTED  equ 8031002Dh
FVE_E_CANNOT_ENCRYPT_NO_KEY      equ 8031002Eh
FVE_E_BOOTABLE_CDDVD             equ 80310030h
FVE_E_PROTECTOR_EXISTS           equ 80310031h
FVE_E_RELATIVE_PATH              equ 80310032h
FVE_E_PROTECTOR_NOT_FOUND        equ 80310033h
FVE_E_INVALID_KEY_FORMAT         equ 80310034h
FVE_E_INVALID_PASSWORD_FORMAT    equ 80310035h
FVE_E_FIPS_RNG_CHECK_FAILED      equ 80310036h
FVE_E_NOT_DECRYPTED              equ 80310039h
FVE_E_INVALID_PROTECTOR_TYPE     equ 8031003Ah
FVE_E_NO_PROTECTORS_TO_TEST      equ 8031003Bh
FVE_E_KEYFILE_NOT_FOUND          equ 8031003Ch
FVE_E_KEYFILE_INVALID            equ 8031003Dh
FVE_E_KEYFILE_NO_VMK             equ 8031003Eh
FVE_E_TPM_DISABLED               equ 8031003Fh
FVE_E_NOT_ALLOWED_IN_SAFE_MODE   equ 80310040h
FVE_E_TPM_INVALID_PCR            equ 80310041h
FVE_E_TPM_NO_VMK                 equ 80310042h
FVE_E_PIN_INVALID                equ 80310043h
FVE_E_AUTH_INVALID_APPLICATION   equ 80310044h
FVE_E_AUTH_INVALID_CONFIG        equ 80310045h
FVE_E_FS_NOT_EXTENDED            equ 80310047h
FVE_E_NO_LICENSE                 equ 80310049h
FVE_E_NOT_ON_STACK               equ 8031004Ah
FVE_E_FS_MOUNTED                 equ 8031004Bh
FVE_E_TOKEN_NOT_IMPERSONATED     equ 8031004Ch
FVE_E_DRY_RUN_FAILED             equ 8031004Dh
FVE_E_REBOOT_REQUIRED            equ 8031004Eh
FVE_E_DEBUGGER_ENABLED           equ 8031004Fh
FVE_E_RAW_ACCESS                 equ 80310050h
FVE_E_RAW_BLOCKED                equ 80310051h
FVE_E_NOT_ALLOWED_IN_VERSION     equ 80310053h
FWP_E_CALLOUT_NOT_FOUND          equ 80320001h
FWP_E_CONDITION_NOT_FOUND        equ 80320002h
FWP_E_FILTER_NOT_FOUND           equ 80320003h
FWP_E_LAYER_NOT_FOUND            equ 80320004h
FWP_E_PROVIDER_NOT_FOUND         equ 80320005h
FWP_E_SUBLAYER_NOT_FOUND         equ 80320007h
FWP_E_NOT_FOUND                  equ 80320008h
FWP_E_ALREADY_EXISTS             equ 80320009h
FWP_E_IN_USE                     equ 8032000Ah
FWP_E_WRONG_SESSION              equ 8032000Ch
FWP_E_NO_TXN_IN_PROGRESS         equ 8032000Dh
FWP_E_TXN_IN_PROGRESS            equ 8032000Eh
FWP_E_TXN_ABORTED                equ 8032000Fh
FWP_E_SESSION_ABORTED            equ 80320010h
FWP_E_INCOMPATIBLE_TXN           equ 80320011h
FWP_E_TIMEOUT                    equ 80320012h
FWP_E_NET_EVENTS_DISABLED        equ 80320013h
FWP_E_INCOMPATIBLE_LAYER         equ 80320014h
FWP_E_KM_CLIENTS_ONLY            equ 80320015h
FWP_E_LIFETIME_MISMATCH          equ 80320016h
FWP_E_BUILTIN_OBJECT             equ 80320017h
FWP_E_TOO_MANY_CALLOUTS          equ 80320018h
FWP_E_NOTIFICATION_DROPPED       equ 80320019h
FWP_E_TRAFFIC_MISMATCH           equ 8032001Ah
FWP_E_INCOMPATIBLE_SA_STATE      equ 8032001Bh
FWP_E_NULL_POINTER               equ 8032001Ch
FWP_E_INVALID_ENUMERATOR         equ 8032001Dh
FWP_E_INVALID_FLAGS              equ 8032001Eh
FWP_E_INVALID_NET_MASK           equ 8032001Fh
FWP_E_INVALID_RANGE              equ 80320020h
FWP_E_INVALID_INTERVAL           equ 80320021h
FWP_E_ZERO_LENGTH_ARRAY          equ 80320022h
FWP_E_NULL_DISPLAY_NAME          equ 80320023h
FWP_E_INVALID_ACTION_TYPE        equ 80320024h
FWP_E_INVALID_WEIGHT             equ 80320025h
FWP_E_MATCH_TYPE_MISMATCH        equ 80320026h
FWP_E_TYPE_MISMATCH              equ 80320027h
FWP_E_OUT_OF_BOUNDS              equ 80320028h
FWP_E_RESERVED                   equ 80320029h
FWP_E_DUPLICATE_CONDITION        equ 8032002Ah
FWP_E_DUPLICATE_KEYMOD           equ 8032002Bh
FWP_E_INCOMPATIBLE_AUTH_METHOD   equ 80320030h
FWP_E_INCOMPATIBLE_DH_GROUP      equ 80320031h
FWP_E_EM_NOT_SUPPORTED           equ 80320032h
FWP_E_NEVER_MATCH                equ 80320033h
FWP_E_PROVIDER_CONTEXT_MISMATCH  equ 80320034h
FWP_E_INVALID_PARAMETER          equ 80320035h
FWP_E_TOO_MANY_SUBLAYERS         equ 80320036h
FWP_E_INVALID_AUTH_TRANSFORM     equ 80320038h
FWP_E_INVALID_CIPHER_TRANSFORM   equ 80320039h
ERROR_NDIS_INTERFACE_CLOSING     equ 80340002h
ERROR_NDIS_BAD_VERSION           equ 80340004h
ERROR_NDIS_BAD_CHARACTERISTICS   equ 80340005h
ERROR_NDIS_ADAPTER_NOT_FOUND     equ 80340006h
ERROR_NDIS_OPEN_FAILED           equ 80340007h
ERROR_NDIS_DEVICE_FAILED         equ 80340008h
ERROR_NDIS_MULTICAST_FULL        equ 80340009h
ERROR_NDIS_MULTICAST_EXISTS      equ 8034000Ah
ERROR_NDIS_MULTICAST_NOT_FOUND   equ 8034000Bh
ERROR_NDIS_REQUEST_ABORTED       equ 8034000Ch
ERROR_NDIS_RESET_IN_PROGRESS     equ 8034000Dh
ERROR_NDIS_NOT_SUPPORTED         equ 803400BBh
ERROR_NDIS_INVALID_PACKET        equ 8034000Fh
ERROR_NDIS_ADAPTER_NOT_READY     equ 80340011h
ERROR_NDIS_INVALID_LENGTH        equ 80340014h
ERROR_NDIS_INVALID_DATA          equ 80340015h
ERROR_NDIS_BUFFER_TOO_SHORT      equ 80340016h
ERROR_NDIS_INVALID_OID           equ 80340017h
ERROR_NDIS_ADAPTER_REMOVED       equ 80340018h
ERROR_NDIS_UNSUPPORTED_MEDIA     equ 80340019h
ERROR_NDIS_GROUP_ADDRESS_IN_USE  equ 8034001Ah
ERROR_NDIS_FILE_NOT_FOUND        equ 8034001Bh
ERROR_NDIS_ERROR_READING_FILE    equ 8034001Ch
ERROR_NDIS_ALREADY_MAPPED        equ 8034001Dh
ERROR_NDIS_RESOURCE_CONFLICT     equ 8034001Eh
ERROR_NDIS_MEDIA_DISCONNECTED    equ 8034001Fh
ERROR_NDIS_INVALID_ADDRESS       equ 80340022h
ERROR_NDIS_INVALID_DEVICE_REQUEST equ 80340010h
ERROR_NDIS_PAUSED                equ 8034002Ah
ERROR_NDIS_INTERFACE_NOT_FOUND   equ 8034002Bh
ERROR_NDIS_UNSUPPORTED_REVISION  equ 8034002Ch
ERROR_NDIS_INVALID_PORT          equ 8034002Dh
ERROR_NDIS_INVALID_PORT_STATE    equ 8034002Eh
ERROR_NDIS_LOW_POWER_STATE       equ 8034002Fh
ERROR_NDIS_DOT11_AUTO_CONFIG_ENABLED equ 80342000h
ERROR_NDIS_DOT11_MEDIA_IN_USE    equ 80342001h
ERROR_NDIS_DOT11_POWER_STATE_INVALID equ 80342002h
ERROR_NDIS_INDICATION_REQUIRED   equ 00340001h

;;;; end winerror


;;;; wincrypt

ALG_CLASS_ANY                    equ 0
ALG_TYPE_ANY                     equ 0
ALG_SID_ANY                      equ 0
ALG_SID_RSA_ANY                  equ 0
ALG_SID_RSA_PKCS                 equ 1
ALG_SID_RSA_MSATWORK             equ 2
ALG_SID_RSA_ENTRUST              equ 3
ALG_SID_RSA_PGP                  equ 4
ALG_SID_DSS_ANY                  equ 0
ALG_SID_DSS_PKCS                 equ 1
ALG_SID_DSS_DMS                  equ 2
ALG_SID_DES                      equ 1
ALG_SID_3DES                     equ 3
ALG_SID_DESX                     equ 4
ALG_SID_IDEA                     equ 5
ALG_SID_CAST                     equ 6
ALG_SID_SAFERSK64                equ 7
ALG_SID_SAFERSK128               equ 8
ALG_SID_3DES_112                 equ 9
ALG_SID_CYLINK_MEK               equ 12
ALG_SID_RC5                      equ 13
ALG_SID_AES_128                  equ 14
ALG_SID_AES_192                  equ 15
ALG_SID_AES_256                  equ 16
ALG_SID_AES                      equ 17
ALG_SID_SKIPJACK                 equ 10
ALG_SID_TEK                      equ 11
CRYPT_MODE_CBCI                  equ 6
CRYPT_MODE_CFBP                  equ 7
CRYPT_MODE_OFBP                  equ 8
CRYPT_MODE_CBCOFM                equ 9
CRYPT_MODE_CBCOFMI               equ 10
ALG_SID_RC2                      equ 2
ALG_SID_RC4                      equ 1
ALG_SID_SEAL                     equ 2
ALG_SID_DH_SANDF                 equ 1
ALG_SID_DH_EPHEM                 equ 2
ALG_SID_AGREED_KEY_ANY           equ 3
ALG_SID_KEA                      equ 4
ALG_SID_MD2                      equ 1
ALG_SID_MD4                      equ 2
ALG_SID_MD5                      equ 3
ALG_SID_SHA                      equ 4
ALG_SID_SHA1                     equ 4
ALG_SID_MAC                      equ 5
ALG_SID_RIPEMD                   equ 6
ALG_SID_RIPEMD160                equ 7
ALG_SID_SSL3SHAMD5               equ 8
ALG_SID_HMAC                     equ 9
ALG_SID_TLS1PRF                  equ 10
ALG_SID_HASH_REPLACE_OWF         equ 11
ALG_SID_SHA_256                  equ 12
ALG_SID_SHA_384                  equ 13
ALG_SID_SHA_512                  equ 14
ALG_SID_SSL3_MASTER              equ 1
ALG_SID_SCHANNEL_MASTER_HASH     equ 2
ALG_SID_SCHANNEL_MAC_KEY         equ 3
ALG_SID_PCT1_MASTER              equ 4
ALG_SID_SSL2_MASTER              equ 5
ALG_SID_TLS1_MASTER              equ 6
ALG_SID_SCHANNEL_ENC_KEY         equ 7
ALG_SID_EXAMPLE                  equ 80
CALG_MD2                         equ (ALG_CLASS_HASH or ALG_TYPE_ANY or ALG_SID_MD2)
CALG_MD4                         equ (ALG_CLASS_HASH or ALG_TYPE_ANY or ALG_SID_MD4)
CALG_MD5                         equ (ALG_CLASS_HASH or ALG_TYPE_ANY or ALG_SID_MD5)
CALG_SHA                         equ (ALG_CLASS_HASH or ALG_TYPE_ANY or ALG_SID_SHA)
CALG_SHA1                        equ (ALG_CLASS_HASH or ALG_TYPE_ANY or ALG_SID_SHA1)
CALG_MAC                         equ (ALG_CLASS_HASH or ALG_TYPE_ANY or ALG_SID_MAC)
CALG_RSA_SIGN                    equ (ALG_CLASS_SIGNATURE or ALG_TYPE_RSA or ALG_SID_RSA_ANY)
CALG_DSS_SIGN                    equ (ALG_CLASS_SIGNATURE or ALG_TYPE_DSS or ALG_SID_DSS_ANY)
CALG_NO_SIGN                     equ (ALG_CLASS_SIGNATURE or ALG_TYPE_ANY or ALG_SID_ANY)
CALG_RSA_KEYX                    equ (ALG_CLASS_KEY_EXCHANGEorALG_TYPE_RSAorALG_SID_RSA_ANY)
CALG_DES                         equ (ALG_CLASS_DATA_ENCRYPTorALG_TYPE_BLOCKorALG_SID_DES)
CALG_3DES_112                    equ (ALG_CLASS_DATA_ENCRYPTorALG_TYPE_BLOCKorALG_SID_3DES_112)
CALG_3DES                        equ (ALG_CLASS_DATA_ENCRYPTorALG_TYPE_BLOCKorALG_SID_3DES)
CALG_DESX                        equ (ALG_CLASS_DATA_ENCRYPTorALG_TYPE_BLOCKorALG_SID_DESX)
CALG_RC2                         equ (ALG_CLASS_DATA_ENCRYPTorALG_TYPE_BLOCKorALG_SID_RC2)
CALG_RC4                         equ (ALG_CLASS_DATA_ENCRYPTorALG_TYPE_STREAMorALG_SID_RC4)
CALG_SEAL                        equ (ALG_CLASS_DATA_ENCRYPTorALG_TYPE_STREAMorALG_SID_SEAL)
CALG_DH_SF                       equ (ALG_CLASS_KEY_EXCHANGEorALG_TYPE_DHorALG_SID_DH_SANDF)
CALG_DH_EPHEM                    equ (ALG_CLASS_KEY_EXCHANGEorALG_TYPE_DHorALG_SID_DH_EPHEM)
CALG_AGREEDKEY_ANY               equ (ALG_CLASS_KEY_EXCHANGEorALG_TYPE_DHorALG_SID_AGREED_KEY_ANY)
CALG_KEA_KEYX                    equ (ALG_CLASS_KEY_EXCHANGEorALG_TYPE_DHorALG_SID_KEA)
CALG_HUGHES_MD5                  equ (ALG_CLASS_KEY_EXCHANGEorALG_TYPE_ANYorALG_SID_MD5)
CALG_SKIPJACK                    equ (ALG_CLASS_DATA_ENCRYPTorALG_TYPE_BLOCKorALG_SID_SKIPJACK)
CALG_TEK                         equ (ALG_CLASS_DATA_ENCRYPTorALG_TYPE_BLOCKorALG_SID_TEK)
CALG_CYLINK_MEK                  equ (ALG_CLASS_DATA_ENCRYPTorALG_TYPE_BLOCKorALG_SID_CYLINK_MEK)
CALG_SSL3_SHAMD5                 equ (ALG_CLASS_HASH or ALG_TYPE_ANY or ALG_SID_SSL3SHAMD5)
CALG_SSL3_MASTER                 equ (ALG_CLASS_MSG_ENCRYPTorALG_TYPE_SECURECHANNELorALG_SID_SSL3_MASTER)
CALG_SCHANNEL_MASTER_HASH        equ (ALG_CLASS_MSG_ENCRYPTorALG_TYPE_SECURECHANNELorALG_SID_SCHANNEL_MASTER_HASH)
CALG_SCHANNEL_MAC_KEY            equ (ALG_CLASS_MSG_ENCRYPTorALG_TYPE_SECURECHANNELorALG_SID_SCHANNEL_MAC_KEY)
CALG_SCHANNEL_ENC_KEY            equ (ALG_CLASS_MSG_ENCRYPTorALG_TYPE_SECURECHANNELorALG_SID_SCHANNEL_ENC_KEY)
CALG_PCT1_MASTER                 equ (ALG_CLASS_MSG_ENCRYPTorALG_TYPE_SECURECHANNELorALG_SID_PCT1_MASTER)
CALG_SSL2_MASTER                 equ (ALG_CLASS_MSG_ENCRYPTorALG_TYPE_SECURECHANNELorALG_SID_SSL2_MASTER)
CALG_TLS1_MASTER                 equ (ALG_CLASS_MSG_ENCRYPTorALG_TYPE_SECURECHANNELorALG_SID_TLS1_MASTER)
CALG_RC5                         equ (ALG_CLASS_DATA_ENCRYPTorALG_TYPE_BLOCKorALG_SID_RC5)
CALG_HMAC                        equ (ALG_CLASS_HASH or ALG_TYPE_ANY or ALG_SID_HMAC)
CALG_TLS1PRF                     equ (ALG_CLASS_HASH or ALG_TYPE_ANY or ALG_SID_TLS1PRF)
CALG_HASH_REPLACE_OWF            equ (ALG_CLASS_HASH or ALG_TYPE_ANY or ALG_SID_HASH_REPLACE_OWF)
CALG_AES_128                     equ (ALG_CLASS_DATA_ENCRYPTorALG_TYPE_BLOCKorALG_SID_AES_128)
CALG_AES_192                     equ (ALG_CLASS_DATA_ENCRYPTorALG_TYPE_BLOCKorALG_SID_AES_192)
CALG_AES_256                     equ (ALG_CLASS_DATA_ENCRYPTorALG_TYPE_BLOCKorALG_SID_AES_256)
CALG_AES                         equ (ALG_CLASS_DATA_ENCRYPTorALG_TYPE_BLOCKorALG_SID_AES)
CALG_SHA_256                     equ (ALG_CLASS_HASH or ALG_TYPE_ANY or ALG_SID_SHA_256)
CALG_SHA_384                     equ (ALG_CLASS_HASH or ALG_TYPE_ANY or ALG_SID_SHA_384)
CALG_SHA_512                     equ (ALG_CLASS_HASH or ALG_TYPE_ANY or ALG_SID_SHA_512)
CRYPT_VERIFYCONTEXT              equ 0F0000000h
CRYPT_NEWKEYSET                  equ 00000008h
CRYPT_DELETEKEYSET               equ 00000010h
CRYPT_MACHINE_KEYSET             equ 00000020h
CRYPT_SILENT                     equ 00000040h
CRYPT_EXPORTABLE                 equ 00000001h
CRYPT_USER_PROTECTED             equ 00000002h
CRYPT_CREATE_SALT                equ 00000004h
CRYPT_UPDATE_KEY                 equ 00000008h
CRYPT_NO_SALT                    equ 00000010h
CRYPT_PREGEN                     equ 00000040h
CRYPT_RECIPIENT                  equ 00000010h
CRYPT_INITIATOR                  equ 00000040h
CRYPT_ONLINE                     equ 00000080h
CRYPT_SF                         equ 00000100h
CRYPT_CREATE_IV                  equ 00000200h
CRYPT_KEK                        equ 00000400h
CRYPT_DATA_KEY                   equ 00000800h
CRYPT_VOLATILE                   equ 00001000h
CRYPT_SGCKEY                     equ 00002000h
CRYPT_ARCHIVABLE                 equ 00004000h
RSA1024BIT_KEY                   equ 04000000h
CRYPT_SERVER                     equ 00000400h
KEY_LENGTH_MASK                  equ 0FFFF0000h
CRYPT_Y_ONLY                     equ 00000001h
CRYPT_SSL2_FALLBACK              equ 00000002h
CRYPT_DESTROYKEY                 equ 00000004h
CRYPT_OAEP                       equ 00000040h
CRYPT_BLOB_VER3                  equ 00000080h
CRYPT_IPSEC_HMAC_KEY             equ 00000100h
CRYPT_DECRYPT_RSA_NO_PADDING_CHECK equ 00000020h
CRYPT_SECRETDIGEST               equ 00000001h
CRYPT_OWF_REPL_LM_HASH           equ 00000001h
CRYPT_LITTLE_ENDIAN              equ 00000001h
CRYPT_NOHASHOID                  equ 00000001h
CRYPT_TYPE2_FORMAT               equ 00000002h
CRYPT_X931_FORMAT                equ 00000004h
CRYPT_MACHINE_DEFAULT            equ 00000001h
CRYPT_USER_DEFAULT               equ 00000002h
CRYPT_DELETE_DEFAULT             equ 00000004h
SIMPLEBLOB                       equ 1h
PUBLICKEYBLOB                    equ 6h
PRIVATEKEYBLOB                   equ 7h
PLAINTEXTKEYBLOB                 equ 8h
OPAQUEKEYBLOB                    equ 9h
PUBLICKEYBLOBEX                  equ Ah
SYMMETRICWRAPKEYBLOB             equ Bh
AT_KEYEXCHANGE                   equ 1
AT_SIGNATURE                     equ 2
CRYPT_USERDATA                   equ 1
KP_IV                            equ 1
KP_SALT                          equ 2
KP_PADDING                       equ 3
KP_MODE                          equ 4
KP_MODE_BITS                     equ 5
KP_PERMISSIONS                   equ 6
KP_ALGID                         equ 7
KP_BLOCKLEN                      equ 8
KP_KEYLEN                        equ 9
KP_SALT_EX                       equ 10
KP_P                             equ 11
KP_G                             equ 12
KP_Q                             equ 13
KP_X                             equ 14
KP_Y                             equ 15
KP_RA                            equ 16
KP_RB                            equ 17
KP_INFO                          equ 18
KP_EFFECTIVE_KEYLEN              equ 19
KP_SCHANNEL_ALG                  equ 20
KP_CLIENT_RANDOM                 equ 21
KP_SERVER_RANDOM                 equ 22
KP_RP                            equ 23
KP_PRECOMP_MD5                   equ 24
KP_PRECOMP_SHA                   equ 25
KP_CERTIFICATE                   equ 26
KP_CLEAR_KEY                     equ 27
KP_PUB_EX_LEN                    equ 28
KP_PUB_EX_VAL                    equ 29
KP_KEYVAL                        equ 30
KP_ADMIN_PIN                     equ 31
KP_KEYEXCHANGE_PIN               equ 32
KP_SIGNATURE_PIN                 equ 33
KP_PREHASH                       equ 34
KP_ROUNDS                        equ 35
KP_OAEP_PARAMS                   equ 36
KP_CMS_KEY_INFO                  equ 37
KP_CMS_DH_KEY_INFO               equ 38
KP_PUB_PARAMS                    equ 39
KP_VERIFY_PARAMS                 equ 40
KP_HIGHEST_VERSION               equ 41
KP_GET_USE_COUNT                 equ 42
PKCS5_PADDING                    equ 1
RANDOM_PADDING                   equ 2
ZERO_PADDING                     equ 3
CRYPT_MODE_CBC                   equ 1
CRYPT_MODE_ECB                   equ 2
CRYPT_MODE_OFB                   equ 3
CRYPT_MODE_CFB                   equ 4
CRYPT_MODE_CTS                   equ 5
CRYPT_ENCRYPT                    equ 0001h
CRYPT_DECRYPT                    equ 0002h
CRYPT_EXPORT                     equ 0004h
CRYPT_READ                       equ 0008h
CRYPT_WRITE                      equ 0010h
CRYPT_MAC                        equ 0020h
CRYPT_EXPORT_KEY                 equ 0040h
CRYPT_IMPORT_KEY                 equ 0080h
CRYPT_ARCHIVE                    equ 0100h
HP_ALGID                         equ 0001h
HP_HASHVAL                       equ 0002h
HP_HASHSIZE                      equ 0004h
HP_HMAC_INFO                     equ 0005h
HP_TLS1PRF_LABEL                 equ 0006h
HP_TLS1PRF_SEED                  equ 0007h
CRYPT_FAILED                     equ FALSE
CRYPT_SUCCEED                    equ TRUE
PP_ENUMALGS                      equ 1
PP_ENUMCONTAINERS                equ 2
PP_IMPTYPE                       equ 3
PP_NAME                          equ 4
PP_VERSION                       equ 5
PP_CONTAINER                     equ 6
PP_CHANGE_PASSWORD               equ 7
PP_KEYSET_SEC_DESCR              equ 8
PP_CERTCHAIN                     equ 9
PP_KEY_TYPE_SUBTYPE              equ 10
PP_PROVTYPE                      equ 16
PP_KEYSTORAGE                    equ 17
PP_APPLI_CERT                    equ 18
PP_SYM_KEYSIZE                   equ 19
PP_SESSION_KEYSIZE               equ 20
PP_UI_PROMPT                     equ 21
PP_ENUMALGS_EX                   equ 22
PP_ENUMMANDROOTS                 equ 25
PP_ENUMELECTROOTS                equ 26
PP_KEYSET_TYPE                   equ 27
PP_ADMIN_PIN                     equ 31
PP_KEYEXCHANGE_PIN               equ 32
PP_SIGNATURE_PIN                 equ 33
PP_SIG_KEYSIZE_INC               equ 34
PP_KEYX_KEYSIZE_INC              equ 35
PP_UNIQUE_CONTAINER              equ 36
PP_SGC_INFO                      equ 37
PP_USE_HARDWARE_RNG              equ 38
PP_KEYSPEC                       equ 39
PP_ENUMEX_SIGNING_PROT           equ 40
PP_CRYPT_COUNT_KEY_USE           equ 41
CRYPT_FIRST                      equ 1
CRYPT_NEXT                       equ 2
CRYPT_SGC_ENUM                   equ 4
CRYPT_IMPL_HARDWARE              equ 1
CRYPT_IMPL_SOFTWARE              equ 2
CRYPT_IMPL_MIXED                 equ 3
CRYPT_IMPL_UNKNOWN               equ 4
CRYPT_IMPL_REMOVABLE             equ 8
CRYPT_SEC_DESCR                  equ 00000001h
CRYPT_PSTORE                     equ 00000002h
CRYPT_UI_PROMPT                  equ 00000004h
CRYPT_FLAG_PCT1                  equ 0001h
CRYPT_FLAG_SSL2                  equ 0002h
CRYPT_FLAG_SSL3                  equ 0004h
CRYPT_FLAG_TLS1                  equ 0008h
CRYPT_FLAG_IPSEC                 equ 0010h
CRYPT_FLAG_SIGNING               equ 0020h
CRYPT_SGC                        equ 0001h
CRYPT_FASTSGC                    equ 0002h
PP_CLIENT_HWND                   equ 1
PP_CONTEXT_INFO                  equ 11
PP_KEYEXCHANGE_KEYSIZE           equ 12
PP_SIGNATURE_KEYSIZE             equ 13
PP_KEYEXCHANGE_ALG               equ 14
PP_SIGNATURE_ALG                 equ 15
PP_DELETEKEY                     equ 24
PROV_RSA_FULL                    equ 1
PROV_RSA_SIG                     equ 2
PROV_DSS                         equ 3
PROV_FORTEZZA                    equ 4
PROV_MS_EXCHANGE                 equ 5
PROV_SSL                         equ 6
PROV_RSA_SCHANNEL                equ 12
PROV_DSS_DH                      equ 13
PROV_EC_ECDSA_SIG                equ 14
PROV_EC_ECNRA_SIG                equ 15
PROV_EC_ECDSA_FULL               equ 16
PROV_EC_ECNRA_FULL               equ 17
PROV_DH_SCHANNEL                 equ 18
PROV_SPYRUS_LYNKS                equ 20
PROV_RNG                         equ 21
PROV_INTEL_SEC                   equ 22
PROV_REPLACE_OWF                 equ 23
PROV_RSA_AES                     equ 24
MAXUIDLEN                        equ 64
dwFORCE_KEY_PROTECTION_DISABLED  equ 0h
dwFORCE_KEY_PROTECTION_USER_SELECT equ 1h
dwFORCE_KEY_PROTECTION_HIGH      equ 2h
CUR_BLOB_VERSION                 equ 2
SCHANNEL_MAC_KEY                 equ 00000000h
SCHANNEL_ENC_KEY                 equ 00000001h
INTERNATIONAL_USAGE              equ 00000001h
CERT_RDN_ANY_TYPE                equ 0
CERT_RDN_ENCODED_BLOB            equ 1
CERT_RDN_OCTET_STRING            equ 2
CERT_RDN_NUMERIC_STRING          equ 3
CERT_RDN_PRINTABLE_STRING        equ 4
CERT_RDN_TELETEX_STRING          equ 5
CERT_RDN_T61_STRING              equ 5
CERT_RDN_VIDEOTEX_STRING         equ 6
CERT_RDN_IA5_STRING              equ 7
CERT_RDN_GRAPHIC_STRING          equ 8
CERT_RDN_VISIBLE_STRING          equ 9
CERT_RDN_ISO646_STRING           equ 9
CERT_RDN_GENERAL_STRING          equ 10
CERT_RDN_UNIVERSAL_STRING        equ 11
CERT_RDN_INT4_STRING             equ 11
CERT_RDN_BMP_STRING              equ 12
CERT_RDN_UNICODE_STRING          equ 12
CERT_RDN_UTF8_STRING             equ 13
CERT_RDN_TYPE_MASK               equ 000000FFh
CERT_RDN_FLAGS_MASK              equ 0FF000000h
CERT_RDN_ENABLE_T61_UNICODE_FLAG equ 80000000h
CERT_RDN_ENABLE_UTF8_UNICODE_FLAG equ 20000000h
CERT_RDN_DISABLE_CHECK_TYPE_FLAG equ 40000000h
CERT_RDN_DISABLE_IE4_UTF8_FLAG   equ 01000000h
CERT_RSA_PUBLIC_KEY_OBJID        equ szOID_RSA_RSA
CERT_DEFAULT_OID_PUBLIC_KEY_SIGN equ szOID_RSA_RSA
CERT_DEFAULT_OID_PUBLIC_KEY_XCHG equ szOID_RSA_RSA
CERT_V1                          equ 0
CERT_V2                          equ 1
CERT_V3                          equ 2
CERT_INFO_VERSION_FLAG           equ 1
CERT_INFO_SERIAL_NUMBER_FLAG     equ 2
CERT_INFO_SIGNATURE_ALGORITHM_FLAG equ 3
CERT_INFO_ISSUER_FLAG            equ 4
CERT_INFO_NOT_BEFORE_FLAG        equ 5
CERT_INFO_NOT_AFTER_FLAG         equ 6
CERT_INFO_SUBJECT_FLAG           equ 7
CERT_INFO_SUBJECT_PUBLIC_KEY_INFO_FLAG equ 8
CERT_INFO_ISSUER_UNIQUE_ID_FLAG  equ 9
CERT_INFO_SUBJECT_UNIQUE_ID_FLAG equ 10
CERT_INFO_EXTENSION_FLAG         equ 11
CRL_V1                           equ 0
CRL_V2                           equ 1
CERT_REQUEST_V1                  equ 0
CERT_KEYGEN_REQUEST_V1           equ 0
CTL_V1                           equ 0
CERT_ENCODING_TYPE_MASK          equ 0000FFFFh
CMSG_ENCODING_TYPE_MASK          equ 0FFFF0000h
CRYPT_ASN_ENCODING               equ 00000001h
CRYPT_NDR_ENCODING               equ 00000002h
X509_ASN_ENCODING                equ 00000001h
X509_NDR_ENCODING                equ 00000002h
PKCS_7_ASN_ENCODING              equ 00010000h
PKCS_7_NDR_ENCODING              equ 00020000h
CRYPT_FORMAT_STR_MULTI_LINE      equ 0001h
CRYPT_FORMAT_STR_NO_HEX          equ 0010h
CRYPT_FORMAT_SIMPLE              equ 0001h
CRYPT_FORMAT_X509                equ 0002h
CRYPT_FORMAT_OID                 equ 0004h
CRYPT_FORMAT_RDN_SEMICOLON       equ 0100h
CRYPT_FORMAT_RDN_CRLF            equ 0200h
CRYPT_FORMAT_RDN_UNQUOTE         equ 0400h
CRYPT_FORMAT_RDN_REVERSE         equ 0800h
CRYPT_FORMAT_COMMA               equ 1000h
CRYPT_FORMAT_SEMICOLON           equ CRYPT_FORMAT_RDN_SEMICOLON
CRYPT_FORMAT_CRLF                equ CRYPT_FORMAT_RDN_CRLF
CRYPT_ENCODE_NO_SIGNATURE_BYTE_REVERSAL_FLAG equ 8h
CRYPT_ENCODE_ALLOC_FLAG          equ 8000h
CRYPT_SORTED_CTL_ENCODE_HASHED_SUBJECT_IDENTIFIER_FLAG equ 10000h
CRYPT_DECODE_NOCOPY_FLAG         equ 1h
CRYPT_DECODE_TO_BE_SIGNED_FLAG   equ 2h
CRYPT_DECODE_SHARE_OID_STRING_FLAG equ 4h
CRYPT_DECODE_NO_SIGNATURE_BYTE_REVERSAL_FLAG equ 8h
CRYPT_DECODE_ALLOC_FLAG          equ 8000h
CRYPT_ENCODE_DECODE_NONE         equ 0
X509_CERT                        equ 1
X509_CERT_TO_BE_SIGNED           equ 2
X509_CERT_CRL_TO_BE_SIGNED       equ 3
X509_CERT_REQUEST_TO_BE_SIGNED   equ 4
X509_EXTENSIONS                  equ 5
X509_NAME_VALUE                  equ 6
X509_NAME                        equ 7
X509_PUBLIC_KEY_INFO             equ 8
X509_AUTHORITY_KEY_ID            equ 9
X509_KEY_ATTRIBUTES              equ 10
X509_KEY_USAGE_RESTRICTION       equ 11
X509_ALTERNATE_NAME              equ 12
X509_BASIC_CONSTRAINTS           equ 13
X509_KEY_USAGE                   equ 14
X509_BASIC_CONSTRAINTS2          equ 15
X509_CERT_POLICIES               equ 16
PKCS_UTC_TIME                    equ 17
PKCS_TIME_REQUEST                equ 18
RSA_CSP_PUBLICKEYBLOB            equ 19
X509_UNICODE_NAME                equ 20
X509_KEYGEN_REQUEST_TO_BE_SIGNED equ 21
PKCS_ATTRIBUTE                   equ 22
PKCS_CONTENT_INFO_SEQUENCE_OF_ANY equ 23
X509_UNICODE_NAME_VALUE          equ 24
X509_ANY_STRING                  equ X509_NAME_VALUE
X509_UNICODE_ANY_STRING          equ X509_UNICODE_NAME_VALUE
X509_OCTET_STRING                equ 25
X509_BITS                        equ 26
X509_INTEGER                     equ 27
X509_MULTI_BYTE_INTEGER          equ 28
X509_ENUMERATED                  equ 29
X509_CHOICE_OF_TIME              equ 30
X509_AUTHORITY_KEY_ID2           equ 31
X509_AUTHORITY_INFO_ACCESS       equ 32
X509_CRL_REASON_CODE             equ X509_ENUMERATED
PKCS_CONTENT_INFO                equ 33
X509_SEQUENCE_OF_ANY             equ 34
X509_CRL_DIST_POINTS             equ 35
X509_ENHANCED_KEY_USAGE          equ 36
PKCS_CTL                         equ 37
X509_MULTI_BYTE_UINT             equ 38
X509_DSS_PUBLICKEY               equ X509_MULTI_BYTE_UINT
X509_DSS_PARAMETERS              equ 39
X509_DSS_SIGNATURE               equ 40
PKCS_RC2_CBC_PARAMETERS          equ 41
PKCS_SMIME_CAPABILITIES          equ 42
PKCS_RSA_PRIVATE_KEY             equ 43
PKCS_PRIVATE_KEY_INFO            equ 44
PKCS_ENCRYPTED_PRIVATE_KEY_INFO  equ 45
X509_PKIX_POLICY_QUALIFIER_USERNOTICE equ 46
X509_DH_PUBLICKEY                equ X509_MULTI_BYTE_UINT
X509_DH_PARAMETERS               equ 47
PKCS_ATTRIBUTES                  equ 48
PKCS_SORTED_CTL                  equ 49
X942_DH_PARAMETERS               equ 50
X509_BITS_WITHOUT_TRAILING_ZEROES equ 51
X942_OTHER_INFO                  equ 52
X509_CERT_PAIR                   equ 53
X509_ISSUING_DIST_POINT          equ 54
X509_NAME_CONSTRAINTS            equ 55
X509_POLICY_MAPPINGS             equ 56
X509_POLICY_CONSTRAINTS          equ 57
X509_CROSS_CERT_DIST_POINTS      equ 58
CMC_DATA                         equ 59
CMC_RESPONSE                     equ 60
CMC_STATUS                       equ 61
CMC_ADD_EXTENSIONS               equ 62
CMC_ADD_ATTRIBUTES               equ 63
X509_CERTIFICATE_TEMPLATE        equ 64
PKCS7_SIGNER_INFO                equ 500
CMS_SIGNER_INFO                  equ 501
CERT_UNICODE_RDN_ERR_INDEX_MASK  equ 3FFh
CERT_UNICODE_RDN_ERR_INDEX_SHIFT equ 22
CERT_UNICODE_ATTR_ERR_INDEX_MASK equ 003Fh
CERT_UNICODE_ATTR_ERR_INDEX_SHIFT equ 16
CERT_UNICODE_VALUE_ERR_INDEX_MASK equ 0000FFFFh
CERT_UNICODE_VALUE_ERR_INDEX_SHIFT equ 0
CERT_DIGITAL_SIGNATURE_KEY_USAGE equ 80h
CERT_NON_REPUDIATION_KEY_USAGE   equ 40h
CERT_KEY_ENCIPHERMENT_KEY_USAGE  equ 20h
CERT_DATA_ENCIPHERMENT_KEY_USAGE equ 10h
CERT_KEY_AGREEMENT_KEY_USAGE     equ 08h
CERT_KEY_CERT_SIGN_KEY_USAGE     equ 04h
CERT_OFFLINE_CRL_SIGN_KEY_USAGE  equ 02h
CERT_CRL_SIGN_KEY_USAGE          equ 02h
CERT_ENCIPHER_ONLY_KEY_USAGE     equ 01h
CERT_DECIPHER_ONLY_KEY_USAGE     equ 80h
CERT_ALT_NAME_OTHER_NAME         equ 1
CERT_ALT_NAME_RFC822_NAME        equ 2
CERT_ALT_NAME_DNS_NAME           equ 3
CERT_ALT_NAME_X400_ADDRESS       equ 4
CERT_ALT_NAME_DIRECTORY_NAME     equ 5
CERT_ALT_NAME_EDI_PARTY_NAME     equ 6
CERT_ALT_NAME_URL                equ 7
CERT_ALT_NAME_IP_ADDRESS         equ 8
CERT_ALT_NAME_REGISTERED_ID      equ 9
CERT_ALT_NAME_ENTRY_ERR_INDEX_MASK equ 0FFh
CERT_ALT_NAME_ENTRY_ERR_INDEX_SHIFT equ 16
CERT_ALT_NAME_VALUE_ERR_INDEX_MASK equ 0000FFFFh
CERT_ALT_NAME_VALUE_ERR_INDEX_SHIFT equ 0
CERT_CA_SUBJECT_FLAG             equ 80h
CERT_END_ENTITY_SUBJECT_FLAG     equ 40h
CRL_REASON_UNSPECIFIED           equ 0
CRL_REASON_KEY_COMPROMISE        equ 1
CRL_REASON_CA_COMPROMISE         equ 2
CRL_REASON_AFFILIATION_CHANGED   equ 3
CRL_REASON_SUPERSEDED            equ 4
CRL_REASON_CESSATION_OF_OPERATION equ 5
CRL_REASON_CERTIFICATE_HOLD      equ 6
CRL_REASON_REMOVE_FROM_CRL       equ 8
CRL_DIST_POINT_NO_NAME           equ 0
CRL_DIST_POINT_FULL_NAME         equ 1
CRL_DIST_POINT_ISSUER_RDN_NAME   equ 2
CRL_REASON_UNUSED_FLAG           equ 80h
CRL_REASON_KEY_COMPROMISE_FLAG   equ 40h
CRL_REASON_CA_COMPROMISE_FLAG    equ 20h
CRL_REASON_AFFILIATION_CHANGED_FLAG equ 10h
CRL_REASON_SUPERSEDED_FLAG       equ 08h
CRL_REASON_CESSATION_OF_OPERATION_FLAG equ 04h
CRL_REASON_CERTIFICATE_HOLD_FLAG equ 02h
CRL_DIST_POINT_ERR_INDEX_MASK    equ 7Fh
CRL_DIST_POINT_ERR_INDEX_SHIFT   equ 24
CRL_DIST_POINT_ERR_CRL_ISSUER_BIT equ 80000000h
CROSS_CERT_DIST_POINT_ERR_INDEX_MASK equ 0FFh
CROSS_CERT_DIST_POINT_ERR_INDEX_SHIFT equ 24
CERT_EXCLUDED_SUBTREE_BIT        equ 80000000h
SORTED_CTL_EXT_FLAGS_OFFSET      equ 0
SORTED_CTL_EXT_COUNT_OFFSET      equ 4
SORTED_CTL_EXT_MAX_COLLISION_OFFSET equ 8
SORTED_CTL_EXT_HASH_BUCKET_OFFSET equ 12
SORTED_CTL_EXT_HASHED_SUBJECT_IDENTIFIER_FLAG equ 1h
CERT_DSS_R_LEN                   equ 20
CERT_DSS_S_LEN                   equ 20
CERT_DSS_SIGNATURE_LEN           equ (CERT_DSS_R_LEN + CERT_DSS_S_LEN)
CRYPT_X942_COUNTER_BYTE_LENGTH   equ 4
CRYPT_X942_KEY_LENGTH_BYTE_LENGTH equ 4
CRYPT_X942_PUB_INFO_BYTE_LENGTH  equ (512/8)
CRYPT_RC2_40BIT_VERSION          equ 160
CRYPT_RC2_56BIT_VERSION          equ 52
CRYPT_RC2_64BIT_VERSION          equ 120
CRYPT_RC2_128BIT_VERSION         equ 58
NETSCAPE_SSL_CLIENT_AUTH_CERT_TYPE equ 80h
NETSCAPE_SSL_SERVER_AUTH_CERT_TYPE equ 40h
NETSCAPE_SMIME_CERT_TYPE         equ 20h
NETSCAPE_SIGN_CERT_TYPE          equ 10h
NETSCAPE_SSL_CA_CERT_TYPE        equ 04h
NETSCAPE_SMIME_CA_CERT_TYPE      equ 02h
NETSCAPE_SIGN_CA_CERT_TYPE       equ 01h
CMC_TAGGED_CERT_REQUEST_CHOICE   equ 1
CMC_OTHER_INFO_NO_CHOICE         equ 0
CMC_OTHER_INFO_FAIL_CHOICE       equ 1
CMC_OTHER_INFO_PEND_CHOICE       equ 2
CMC_STATUS_SUCCESS               equ 0
CMC_STATUS_FAILED                equ 2
CMC_STATUS_PENDING               equ 3
CMC_STATUS_NO_SUPPORT            equ 4
CMC_STATUS_CONFIRM_REQUIRED      equ 5
CMC_FAIL_BAD_ALG                 equ 0
CMC_FAIL_BAD_MESSAGE_CHECK       equ 1
CMC_FAIL_BAD_REQUEST             equ 2
CMC_FAIL_BAD_TIME                equ 3
CMC_FAIL_BAD_CERT_ID             equ 4
CMC_FAIL_UNSUPORTED_EXT          equ 5
CMC_FAIL_MUST_ARCHIVE_KEYS       equ 6
CMC_FAIL_BAD_IDENTITY            equ 7
CMC_FAIL_POP_REQUIRED            equ 8
CMC_FAIL_POP_FAILED              equ 9
CMC_FAIL_NO_KEY_REUSE            equ 10
CMC_FAIL_INTERNAL_CA_ERROR       equ 11
CMC_FAIL_TRY_LATER               equ 12
CRYPT_INSTALL_OID_FUNC_BEFORE_FLAG equ 1
CRYPT_GET_INSTALLED_OID_FUNC_FLAG equ 1h
CRYPT_REGISTER_FIRST_INDEX       equ 0
CRYPT_REGISTER_LAST_INDEX        equ 0FFFFFFFFh
CRYPT_MATCH_ANY_ENCODING_TYPE    equ 0FFFFFFFFh
CRYPT_HASH_ALG_OID_GROUP_ID      equ 1
CRYPT_ENCRYPT_ALG_OID_GROUP_ID   equ 2
CRYPT_PUBKEY_ALG_OID_GROUP_ID    equ 3
CRYPT_SIGN_ALG_OID_GROUP_ID      equ 4
CRYPT_RDN_ATTR_OID_GROUP_ID      equ 5
CRYPT_EXT_OR_ATTR_OID_GROUP_ID   equ 6
CRYPT_ENHKEY_USAGE_OID_GROUP_ID  equ 7
CRYPT_POLICY_OID_GROUP_ID        equ 8
CRYPT_TEMPLATE_OID_GROUP_ID      equ 9
CRYPT_LAST_OID_GROUP_ID          equ 9
CRYPT_FIRST_ALG_OID_GROUP_ID     equ CRYPT_HASH_ALG_OID_GROUP_ID
CRYPT_LAST_ALG_OID_GROUP_ID      equ CRYPT_SIGN_ALG_OID_GROUP_ID
CRYPT_OID_INHIBIT_SIGNATURE_FORMAT_FLAG equ 1h
CRYPT_OID_USE_PUBKEY_PARA_FOR_PKCS7_FLAG equ 2h
CRYPT_OID_NO_NULL_ALGORITHM_PARA_FLAG equ 4h
CRYPT_OID_INFO_OID_KEY           equ 1
CRYPT_OID_INFO_NAME_KEY          equ 2
CRYPT_OID_INFO_ALGID_KEY         equ 3
CRYPT_OID_INFO_SIGN_KEY          equ 4
CRYPT_INSTALL_OID_INFO_BEFORE_FLAG equ 1
CRYPT_LOCALIZED_NAME_ENCODING_TYPE equ 0
CMSG_DATA                        equ 1
CMSG_SIGNED                      equ 2
CMSG_ENVELOPED                   equ 3
CMSG_SIGNED_AND_ENVELOPED        equ 4
CMSG_HASHED                      equ 5
CMSG_ENCRYPTED                   equ 6
CERT_ID_ISSUER_SERIAL_NUMBER     equ 1
CERT_ID_KEY_IDENTIFIER           equ 2
CERT_ID_SHA1_HASH                equ 3
CMSG_KEY_AGREE_EPHEMERAL_KEY_CHOICE equ 1
CMSG_KEY_AGREE_STATIC_KEY_CHOICE equ 2
CMSG_MAIL_LIST_HANDLE_KEY_CHOICE equ 1
CMSG_KEY_TRANS_RECIPIENT         equ 1
CMSG_KEY_AGREE_RECIPIENT         equ 2
CMSG_MAIL_LIST_RECIPIENT         equ 3
CMSG_SP3_COMPATIBLE_ENCRYPT_FLAG equ 80000000h
CMSG_RC4_NO_SALT_FLAG            equ 40000000h
CMSG_INDEFINITE_LENGTH           equ 0FFFFFFFFh
CMSG_BARE_CONTENT_FLAG           equ 00000001h
CMSG_LENGTH_ONLY_FLAG            equ 00000002h
CMSG_DETACHED_FLAG               equ 00000004h
CMSG_AUTHENTICATED_ATTRIBUTES_FLAG equ 00000008h
CMSG_CONTENTS_OCTETS_FLAG        equ 00000010h
CMSG_MAX_LENGTH_FLAG             equ 00000020h
CMSG_CMS_ENCAPSULATED_CONTENT_FLAG equ 00000040h
CMSG_CRYPT_RELEASE_CONTEXT_FLAG  equ 00008000h
CMSG_TYPE_PARAM                  equ 1
CMSG_CONTENT_PARAM               equ 2
CMSG_BARE_CONTENT_PARAM          equ 3
CMSG_INNER_CONTENT_TYPE_PARAM    equ 4
CMSG_SIGNER_COUNT_PARAM          equ 5
CMSG_SIGNER_INFO_PARAM           equ 6
CMSG_SIGNER_CERT_INFO_PARAM      equ 7
CMSG_SIGNER_HASH_ALGORITHM_PARAM equ 8
CMSG_SIGNER_AUTH_ATTR_PARAM      equ 9
CMSG_SIGNER_UNAUTH_ATTR_PARAM    equ 10
CMSG_CERT_COUNT_PARAM            equ 11
CMSG_CERT_PARAM                  equ 12
CMSG_CRL_COUNT_PARAM             equ 13
CMSG_CRL_PARAM                   equ 14
CMSG_ENVELOPE_ALGORITHM_PARAM    equ 15
CMSG_RECIPIENT_COUNT_PARAM       equ 17
CMSG_RECIPIENT_INDEX_PARAM       equ 18
CMSG_RECIPIENT_INFO_PARAM        equ 19
CMSG_HASH_ALGORITHM_PARAM        equ 20
CMSG_HASH_DATA_PARAM             equ 21
CMSG_COMPUTED_HASH_PARAM         equ 22
CMSG_ENCRYPT_PARAM               equ 26
CMSG_ENCRYPTED_DIGEST            equ 27
CMSG_ENCODED_SIGNER              equ 28
CMSG_ENCODED_MESSAGE             equ 29
CMSG_VERSION_PARAM               equ 30
CMSG_ATTR_CERT_COUNT_PARAM       equ 31
CMSG_ATTR_CERT_PARAM             equ 32
CMSG_CMS_RECIPIENT_COUNT_PARAM   equ 33
CMSG_CMS_RECIPIENT_INDEX_PARAM   equ 34
CMSG_CMS_RECIPIENT_ENCRYPTED_KEY_INDEX_PARAM equ 35
CMSG_CMS_RECIPIENT_INFO_PARAM    equ 36
CMSG_UNPROTECTED_ATTR_PARAM      equ 37
CMSG_SIGNER_CERT_ID_PARAM        equ 38
CMSG_CMS_SIGNER_INFO_PARAM       equ 39
CMSG_SIGNED_DATA_V1              equ 1
CMSG_SIGNED_DATA_V3              equ 3
CMSG_SIGNED_DATA_PKCS_1_5_VERSION equ CMSG_SIGNED_DATA_V1
CMSG_SIGNED_DATA_CMS_VERSION     equ CMSG_SIGNED_DATA_V3
CMSG_SIGNER_INFO_V1              equ 1
CMSG_SIGNER_INFO_V3              equ 3
CMSG_SIGNER_INFO_PKCS_1_5_VERSION equ CMSG_SIGNER_INFO_V1
CMSG_SIGNER_INFO_CMS_VERSION     equ CMSG_SIGNER_INFO_V3
CMSG_HASHED_DATA_V0              equ 0
CMSG_HASHED_DATA_V2              equ 2
CMSG_HASHED_DATA_PKCS_1_5_VERSION equ CMSG_HASHED_DATA_V0
CMSG_HASHED_DATA_CMS_VERSION     equ CMSG_HASHED_DATA_V2
CMSG_ENVELOPED_DATA_V0           equ 0
CMSG_ENVELOPED_DATA_V2           equ 2
CMSG_ENVELOPED_DATA_PKCS_1_5_VERSION equ CMSG_ENVELOPED_DATA_V0
CMSG_ENVELOPED_DATA_CMS_VERSION  equ CMSG_ENVELOPED_DATA_V2
CMSG_KEY_AGREE_ORIGINATOR_CERT   equ 1
CMSG_KEY_AGREE_ORIGINATOR_PUBLIC_KEY equ 2
CMSG_ENVELOPED_RECIPIENT_V0      equ 0
CMSG_ENVELOPED_RECIPIENT_V2      equ 2
CMSG_ENVELOPED_RECIPIENT_V3      equ 3
CMSG_ENVELOPED_RECIPIENT_V4      equ 4
CMSG_KEY_TRANS_PKCS_1_5_VERSION  equ CMSG_ENVELOPED_RECIPIENT_V0
CMSG_KEY_TRANS_CMS_VERSION       equ CMSG_ENVELOPED_RECIPIENT_V2
CMSG_KEY_AGREE_VERSION           equ CMSG_ENVELOPED_RECIPIENT_V3
CMSG_MAIL_LIST_VERSION           equ CMSG_ENVELOPED_RECIPIENT_V4
CMSG_CTRL_VERIFY_SIGNATURE       equ 1
CMSG_CTRL_DECRYPT                equ 2
CMSG_CTRL_VERIFY_HASH            equ 5
CMSG_CTRL_ADD_SIGNER             equ 6
CMSG_CTRL_DEL_SIGNER             equ 7
CMSG_CTRL_ADD_SIGNER_UNAUTH_ATTR equ 8
CMSG_CTRL_DEL_SIGNER_UNAUTH_ATTR equ 9
CMSG_CTRL_ADD_CERT               equ 10
CMSG_CTRL_DEL_CERT               equ 11
CMSG_CTRL_ADD_CRL                equ 12
CMSG_CTRL_DEL_CRL                equ 13
CMSG_CTRL_ADD_ATTR_CERT          equ 14
CMSG_CTRL_DEL_ATTR_CERT          equ 15
CMSG_CTRL_KEY_TRANS_DECRYPT      equ 16
CMSG_CTRL_KEY_AGREE_DECRYPT      equ 17
CMSG_CTRL_MAIL_LIST_DECRYPT      equ 18
CMSG_CTRL_VERIFY_SIGNATURE_EX    equ 19
CMSG_CTRL_ADD_CMS_SIGNER_INFO    equ 20
CMSG_VERIFY_SIGNER_PUBKEY        equ 1
CMSG_VERIFY_SIGNER_CERT          equ 2
CMSG_VERIFY_SIGNER_CHAIN         equ 3
CMSG_VERIFY_SIGNER_NULL          equ 4
CMSG_DEFAULT_INSTALLABLE_FUNC_OID equ 1
CMSG_CONTENT_ENCRYPT_PAD_ENCODED_LEN_FLAG equ 00000001h
CMSG_CONTENT_ENCRYPT_FREE_PARA_FLAG equ 00000001h
CMSG_CONTENT_ENCRYPT_RELEASE_CONTEXT_FLAG equ 00008000h
CMSG_KEY_TRANS_ENCRYPT_FREE_PARA_FLAG equ 00000001h
CMSG_KEY_AGREE_ENCRYPT_FREE_PARA_FLAG equ 00000001h
CMSG_KEY_AGREE_ENCRYPT_FREE_MATERIAL_FLAG equ 00000002h
CMSG_KEY_AGREE_ENCRYPT_FREE_PUBKEY_ALG_FLAG equ 00000004h
CMSG_KEY_AGREE_ENCRYPT_FREE_PUBKEY_PARA_FLAG equ 00000008h
CMSG_KEY_AGREE_ENCRYPT_FREE_PUBKEY_BITS_FLAG equ 00000010h
CMSG_MAIL_LIST_ENCRYPT_FREE_PARA_FLAG equ 00000001h
CERT_KEY_PROV_HANDLE_PROP_ID     equ 1
CERT_KEY_PROV_INFO_PROP_ID       equ 2
CERT_SHA1_HASH_PROP_ID           equ 3
CERT_MD5_HASH_PROP_ID            equ 4
CERT_HASH_PROP_ID                equ CERT_SHA1_HASH_PROP_ID
CERT_KEY_CONTEXT_PROP_ID         equ 5
CERT_KEY_SPEC_PROP_ID            equ 6
CERT_IE30_RESERVED_PROP_ID       equ 7
CERT_PUBKEY_HASH_RESERVED_PROP_ID equ 8
CERT_ENHKEY_USAGE_PROP_ID        equ 9
CERT_CTL_USAGE_PROP_ID           equ CERT_ENHKEY_USAGE_PROP_ID
CERT_NEXT_UPDATE_LOCATION_PROP_ID equ 10
CERT_FRIENDLY_NAME_PROP_ID       equ 11
CERT_PVK_FILE_PROP_ID            equ 12
CERT_DESCRIPTION_PROP_ID         equ 13
CERT_ACCESS_STATE_PROP_ID        equ 14
CERT_SIGNATURE_HASH_PROP_ID      equ 15
CERT_SMART_CARD_DATA_PROP_ID     equ 16
CERT_EFS_PROP_ID                 equ 17
CERT_FORTEZZA_DATA_PROP_ID       equ 18
CERT_ARCHIVED_PROP_ID            equ 19
CERT_KEY_IDENTIFIER_PROP_ID      equ 20
CERT_AUTO_ENROLL_PROP_ID         equ 21
CERT_PUBKEY_ALG_PARA_PROP_ID     equ 22
CERT_CROSS_CERT_DIST_POINTS_PROP_ID equ 23
CERT_ISSUER_PUBLIC_KEY_MD5_HASH_PROP_ID equ 24
CERT_SUBJECT_PUBLIC_KEY_MD5_HASH_PROP_ID equ 25
CERT_ENROLLMENT_PROP_ID          equ 26
CERT_DATE_STAMP_PROP_ID          equ 27
CERT_ISSUER_SERIAL_NUMBER_MD5_HASH_PROP_ID equ 28
CERT_SUBJECT_NAME_MD5_HASH_PROP_ID equ 29
CERT_EXTENDED_ERROR_INFO_PROP_ID equ 30
CERT_RENEWAL_PROP_ID             equ 64
CERT_ARCHIVED_KEY_HASH_PROP_ID   equ 65
CERT_AUTO_ENROLL_RETRY_PROP_ID   equ 66
CERT_AIA_URL_RETRIEVED_PROP_ID   equ 67
CERT_FIRST_RESERVED_PROP_ID      equ 68
CERT_LAST_RESERVED_PROP_ID       equ 00007FFFh
CERT_FIRST_USER_PROP_ID          equ 00008000h
CERT_LAST_USER_PROP_ID           equ 0000FFFFh
CERT_ACCESS_STATE_WRITE_PERSIST_FLAG equ 1h
CERT_ACCESS_STATE_SYSTEM_STORE_FLAG equ 2h
CERT_ACCESS_STATE_LM_SYSTEM_STORE_FLAG equ 4h
CERT_SET_KEY_PROV_HANDLE_PROP_ID equ 00000001h
CERT_SET_KEY_CONTEXT_PROP_ID     equ 00000001h
CERT_STORE_PROV_MSG              equ 1
CERT_STORE_PROV_MEMORY           equ 2
CERT_STORE_PROV_FILE             equ 3
CERT_STORE_PROV_REG              equ 4
CERT_STORE_PROV_PKCS7            equ 5
CERT_STORE_PROV_SERIALIZED       equ 6
CERT_STORE_PROV_FILENAME_A       equ 7
CERT_STORE_PROV_FILENAME_W       equ 8
CERT_STORE_PROV_FILENAME         equ CERT_STORE_PROV_FILENAME_W
CERT_STORE_PROV_SYSTEM_A         equ 9
CERT_STORE_PROV_SYSTEM_W         equ 10
CERT_STORE_PROV_SYSTEM           equ CERT_STORE_PROV_SYSTEM_W
CERT_STORE_PROV_COLLECTION       equ 11
CERT_STORE_PROV_SYSTEM_REGISTRY_A equ 12
CERT_STORE_PROV_SYSTEM_REGISTRY_W equ 13
CERT_STORE_PROV_SYSTEM_REGISTRY  equ CERT_STORE_PROV_SYSTEM_REGISTRY_W
CERT_STORE_PROV_PHYSICAL_W       equ 14
CERT_STORE_PROV_PHYSICAL         equ CERT_STORE_PROV_PHYSICAL_W
CERT_STORE_PROV_SMART_CARD_W     equ 15
CERT_STORE_PROV_SMART_CARD       equ CERT_STORE_PROV_SMART_CARD_W
CERT_STORE_PROV_LDAP_W           equ 16
CERT_STORE_PROV_LDAP             equ CERT_STORE_PROV_LDAP_W
CERT_STORE_SIGNATURE_FLAG        equ 00000001h
CERT_STORE_TIME_VALIDITY_FLAG    equ 00000002h
CERT_STORE_REVOCATION_FLAG       equ 00000004h
CERT_STORE_NO_CRL_FLAG           equ 00010000h
CERT_STORE_NO_ISSUER_FLAG        equ 00020000h
CERT_STORE_BASE_CRL_FLAG         equ 00000100h
CERT_STORE_DELTA_CRL_FLAG        equ 00000200h
CERT_STORE_NO_CRYPT_RELEASE_FLAG equ 00000001h
CERT_STORE_SET_LOCALIZED_NAME_FLAG equ 00000002h
CERT_STORE_DEFER_CLOSE_UNTIL_LAST_FREE_FLAG equ 00000004h
CERT_STORE_DELETE_FLAG           equ 00000010h
CERT_STORE_UNSAFE_PHYSICAL_FLAG  equ 00000020h
CERT_STORE_SHARE_STORE_FLAG      equ 00000040h
CERT_STORE_SHARE_CONTEXT_FLAG    equ 00000080h
CERT_STORE_MANIFOLD_FLAG         equ 00000100h
CERT_STORE_ENUM_ARCHIVED_FLAG    equ 00000200h
CERT_STORE_UPDATE_KEYID_FLAG     equ 00000400h
CERT_STORE_BACKUP_RESTORE_FLAG   equ 00000800h
CERT_STORE_READONLY_FLAG         equ 00008000h
CERT_STORE_OPEN_EXISTING_FLAG    equ 00004000h
CERT_STORE_CREATE_NEW_FLAG       equ 00002000h
CERT_STORE_MAXIMUM_ALLOWED_FLAG  equ 00001000h
CERT_SYSTEM_STORE_MASK           equ 0FFFF0000h
CERT_SYSTEM_STORE_RELOCATE_FLAG  equ 80000000h
CERT_SYSTEM_STORE_UNPROTECTED_FLAG equ 40000000h
CERT_SYSTEM_STORE_LOCATION_MASK  equ 00FF0000h
CERT_SYSTEM_STORE_LOCATION_SHIFT equ 16
CERT_SYSTEM_STORE_CURRENT_USER_ID equ 1
CERT_SYSTEM_STORE_LOCAL_MACHINE_ID equ 2
CERT_SYSTEM_STORE_CURRENT_SERVICE_ID equ 4
CERT_SYSTEM_STORE_SERVICES_ID    equ 5
CERT_SYSTEM_STORE_USERS_ID       equ 6
CERT_SYSTEM_STORE_CURRENT_USER_GROUP_POLICY_ID equ 7
CERT_SYSTEM_STORE_LOCAL_MACHINE_GROUP_POLICY_ID equ 8
CERT_SYSTEM_STORE_LOCAL_MACHINE_ENTERPRISE_ID equ 9
CERT_PROT_ROOT_DISABLE_CURRENT_USER_FLAG equ 1h
CERT_PROT_ROOT_INHIBIT_ADD_AT_INIT_FLAG equ 2h
CERT_PROT_ROOT_INHIBIT_PURGE_LM_FLAG equ 4h
CERT_PROT_ROOT_DISABLE_LM_AUTH_FLAG equ 8h
CERT_PROT_ROOT_ONLY_LM_GPT_FLAG  equ 8h
CERT_PROT_ROOT_DISABLE_NT_AUTH_REQUIRED_FLAG equ 10h
CERT_PROT_ROOT_DISABLE_NOT_DEFINED_NAME_CONSTRAINT_FLAG equ 20h
CERT_TRUST_PUB_ALLOW_TRUST_MASK  equ 00000003h
CERT_TRUST_PUB_ALLOW_END_USER_TRUST equ 00000000h
CERT_TRUST_PUB_ALLOW_MACHINE_ADMIN_TRUST equ 00000001h
CERT_TRUST_PUB_ALLOW_ENTERPRISE_ADMIN_TRUST equ 00000002h
CERT_TRUST_PUB_CHECK_PUBLISHER_REV_FLAG equ 00000100h
CERT_TRUST_PUB_CHECK_TIMESTAMP_REV_FLAG equ 00000200h
CERT_AUTH_ROOT_AUTO_UPDATE_DISABLE_UNTRUSTED_ROOT_LOGGING_FLAG equ 1h
CERT_AUTH_ROOT_AUTO_UPDATE_DISABLE_PARTIAL_CHAIN_LOGGING_FLAG equ 2h
CERT_REGISTRY_STORE_REMOTE_FLAG  equ 10000h
CERT_REGISTRY_STORE_SERIALIZED_FLAG equ 20000h
CERT_REGISTRY_STORE_CLIENT_GPT_FLAG equ 80000000h
CERT_REGISTRY_STORE_LM_GPT_FLAG  equ 01000000h
CERT_REGISTRY_STORE_ROAMING_FLAG equ 40000h
CERT_REGISTRY_STORE_MY_IE_DIRTY_FLAG equ 80000h
CERT_FILE_STORE_COMMIT_ENABLE_FLAG equ 10000h
CERT_LDAP_STORE_SIGN_FLAG        equ 10000h
CERT_LDAP_STORE_AREC_EXCLUSIVE_FLAG equ 20000h
CERT_LDAP_STORE_OPENED_FLAG      equ 40000h
CERT_LDAP_STORE_UNBIND_FLAG      equ 80000h
CERT_STORE_PROV_EXTERNAL_FLAG    equ 1h
CERT_STORE_PROV_DELETED_FLAG     equ 2h
CERT_STORE_PROV_NO_PERSIST_FLAG  equ 4h
CERT_STORE_PROV_SYSTEM_STORE_FLAG equ 8h
CERT_STORE_PROV_LM_SYSTEM_STORE_FLAG equ 10h
CERT_STORE_PROV_CLOSE_FUNC       equ 0
CERT_STORE_PROV_READ_CERT_FUNC   equ 1
CERT_STORE_PROV_WRITE_CERT_FUNC  equ 2
CERT_STORE_PROV_DELETE_CERT_FUNC equ 3
CERT_STORE_PROV_SET_CERT_PROPERTY_FUNC equ 4
CERT_STORE_PROV_READ_CRL_FUNC    equ 5
CERT_STORE_PROV_WRITE_CRL_FUNC   equ 6
CERT_STORE_PROV_DELETE_CRL_FUNC  equ 7
CERT_STORE_PROV_SET_CRL_PROPERTY_FUNC equ 8
CERT_STORE_PROV_READ_CTL_FUNC    equ 9
CERT_STORE_PROV_WRITE_CTL_FUNC   equ 10
CERT_STORE_PROV_DELETE_CTL_FUNC  equ 11
CERT_STORE_PROV_SET_CTL_PROPERTY_FUNC equ 12
CERT_STORE_PROV_CONTROL_FUNC     equ 13
CERT_STORE_PROV_FIND_CERT_FUNC   equ 14
CERT_STORE_PROV_FREE_FIND_CERT_FUNC equ 15
CERT_STORE_PROV_GET_CERT_PROPERTY_FUNC equ 16
CERT_STORE_PROV_FIND_CRL_FUNC    equ 17
CERT_STORE_PROV_FREE_FIND_CRL_FUNC equ 18
CERT_STORE_PROV_GET_CRL_PROPERTY_FUNC equ 19
CERT_STORE_PROV_FIND_CTL_FUNC    equ 20
CERT_STORE_PROV_FREE_FIND_CTL_FUNC equ 21
CERT_STORE_PROV_GET_CTL_PROPERTY_FUNC equ 22
CERT_STORE_PROV_WRITE_ADD_FLAG   equ 1h
CERT_STORE_SAVE_AS_STORE         equ 1
CERT_STORE_SAVE_AS_PKCS7         equ 2
CERT_STORE_SAVE_TO_FILE          equ 1
CERT_STORE_SAVE_TO_MEMORY        equ 2
CERT_STORE_SAVE_TO_FILENAME_A    equ 3
CERT_STORE_SAVE_TO_FILENAME_W    equ 4
CERT_STORE_SAVE_TO_FILENAME      equ CERT_STORE_SAVE_TO_FILENAME_W
CERT_CLOSE_STORE_FORCE_FLAG      equ 00000001h
CERT_CLOSE_STORE_CHECK_FLAG      equ 00000002h
CERT_COMPARE_MASK                equ 0FFFFh
CERT_COMPARE_SHIFT               equ 16
CERT_COMPARE_ANY                 equ 0
CERT_COMPARE_SHA1_HASH           equ 1
CERT_COMPARE_NAME                equ 2
CERT_COMPARE_ATTR                equ 3
CERT_COMPARE_MD5_HASH            equ 4
CERT_COMPARE_PROPERTY            equ 5
CERT_COMPARE_PUBLIC_KEY          equ 6
CERT_COMPARE_HASH                equ CERT_COMPARE_SHA1_HASH
CERT_COMPARE_NAME_STR_A          equ 7
CERT_COMPARE_NAME_STR_W          equ 8
CERT_COMPARE_KEY_SPEC            equ 9
CERT_COMPARE_ENHKEY_USAGE        equ 10
CERT_COMPARE_CTL_USAGE           equ CERT_COMPARE_ENHKEY_USAGE
CERT_COMPARE_SUBJECT_CERT        equ 11
CERT_COMPARE_ISSUER_OF           equ 12
CERT_COMPARE_EXISTING            equ 13
CERT_COMPARE_SIGNATURE_HASH      equ 14
CERT_COMPARE_KEY_IDENTIFIER      equ 15
CERT_COMPARE_CERT_ID             equ 16
CERT_COMPARE_CROSS_CERT_DIST_POINTS equ 17
CERT_COMPARE_PUBKEY_MD5_HASH     equ 18
CERT_FIND_OPTIONAL_ENHKEY_USAGE_FLAG equ 1h
CERT_FIND_EXT_ONLY_ENHKEY_USAGE_FLAG equ 2h
CERT_FIND_PROP_ONLY_ENHKEY_USAGE_FLAG equ 4h
CERT_FIND_NO_ENHKEY_USAGE_FLAG   equ 8h
CERT_FIND_OR_ENHKEY_USAGE_FLAG   equ 10h
CERT_FIND_VALID_ENHKEY_USAGE_FLAG equ 20h
CERT_FIND_OPTIONAL_CTL_USAGE_FLAG equ CERT_FIND_OPTIONAL_ENHKEY_USAGE_FLAG
CERT_FIND_NO_CTL_USAGE_FLAG      equ CERT_FIND_NO_ENHKEY_USAGE_FLAG
CERT_FIND_OR_CTL_USAGE_FLAG      equ CERT_FIND_OR_ENHKEY_USAGE_FLAG
CERT_FIND_VALID_CTL_USAGE_FLAG   equ CERT_FIND_VALID_ENHKEY_USAGE_FLAG
CERT_SET_PROPERTY_IGNORE_PERSIST_ERROR_FLAG equ 80000000h
CERT_SET_PROPERTY_INHIBIT_PERSIST_FLAG equ 40000000h
CTL_ENTRY_FROM_PROP_CHAIN_FLAG   equ 1h
CRL_FIND_ANY                     equ 0
CRL_FIND_ISSUED_BY               equ 1
CRL_FIND_EXISTING                equ 2
CRL_FIND_ISSUED_FOR              equ 3
CRL_FIND_ISSUED_BY_AKI_FLAG      equ 1h
CRL_FIND_ISSUED_BY_SIGNATURE_FLAG equ 2h
CRL_FIND_ISSUED_BY_DELTA_FLAG    equ 4h
CRL_FIND_ISSUED_BY_BASE_FLAG     equ 8h
CERT_STORE_ADD_NEW               equ 1
CERT_STORE_ADD_USE_EXISTING      equ 2
CERT_STORE_ADD_REPLACE_EXISTING  equ 3
CERT_STORE_ADD_ALWAYS            equ 4
CERT_STORE_ADD_REPLACE_EXISTING_INHERIT_PROPERTIES equ 5
CERT_STORE_ADD_NEWER             equ 6
CERT_STORE_ADD_NEWER_INHERIT_PROPERTIES equ 7
CERT_STORE_CERTIFICATE_CONTEXT   equ 1
CERT_STORE_CRL_CONTEXT           equ 2
CERT_STORE_CTL_CONTEXT           equ 3
CERT_STORE_ALL_CONTEXT_FLAG      equ 0
CTL_ANY_SUBJECT_TYPE             equ 1
CTL_CERT_SUBJECT_TYPE            equ 2
CTL_FIND_ANY                     equ 0
CTL_FIND_SHA1_HASH               equ 1
CTL_FIND_MD5_HASH                equ 2
CTL_FIND_USAGE                   equ 3
CTL_FIND_SUBJECT                 equ 4
CTL_FIND_EXISTING                equ 5
CTL_FIND_NO_LIST_ID_CBDATA       equ 0FFFFFFFFh
CTL_FIND_NO_SIGNER_PTR           equ -1
CTL_FIND_SAME_USAGE_FLAG         equ 1h
CERT_STORE_CTRL_RESYNC           equ 1
CERT_STORE_CTRL_NOTIFY_CHANGE    equ 2
CERT_STORE_CTRL_COMMIT           equ 3
CERT_STORE_CTRL_AUTO_RESYNC      equ 4
CERT_STORE_CTRL_CANCEL_NOTIFY    equ 5
CERT_STORE_CTRL_INHIBIT_DUPLICATE_HANDLE_FLAG equ 1h
CERT_STORE_CTRL_COMMIT_FORCE_FLAG equ 1h
CERT_STORE_CTRL_COMMIT_CLEAR_FLAG equ 2h
CERT_STORE_LOCALIZED_NAME_PROP_ID equ 1000h
CERT_CREATE_CONTEXT_NOCOPY_FLAG  equ 1h
CERT_CREATE_CONTEXT_SORTED_FLAG  equ 2h
CERT_CREATE_CONTEXT_NO_HCRYPTMSG_FLAG equ 4h
CERT_CREATE_CONTEXT_NO_ENTRY_FLAG equ 8h
CERT_PHYSICAL_STORE_ADD_ENABLE_FLAG equ 1h
CERT_PHYSICAL_STORE_OPEN_DISABLE_FLAG equ 2h
CERT_PHYSICAL_STORE_REMOTE_OPEN_DISABLE_FLAG equ 4h
CERT_PHYSICAL_STORE_INSERT_COMPUTER_NAME_ENABLE_FLAG equ 8h
CERT_PHYSICAL_STORE_PREDEFINED_ENUM_FLAG equ 1h
CMSG_TRUSTED_SIGNER_FLAG         equ 1h
CMSG_SIGNER_ONLY_FLAG            equ 2h
CMSG_USE_SIGNER_INDEX_FLAG       equ 4h
CMSG_CMS_ENCAPSULATED_CTL_FLAG   equ 00008000h
CMSG_ENCODE_SORTED_CTL_FLAG      equ 1h
CMSG_ENCODE_HASHED_SUBJECT_IDENTIFIER_FLAG equ 2h
CERT_VERIFY_INHIBIT_CTL_UPDATE_FLAG equ 1h
CERT_VERIFY_TRUSTED_SIGNERS_FLAG equ 2h
CERT_VERIFY_NO_TIME_CHECK_FLAG   equ 4h
CERT_VERIFY_ALLOW_MORE_USAGE_FLAG equ 8h
CERT_VERIFY_UPDATED_CTL_FLAG     equ 1h
CERT_CONTEXT_REVOCATION_TYPE     equ 1
CERT_VERIFY_REV_CHAIN_FLAG       equ 00000001h
CERT_VERIFY_CACHE_ONLY_BASED_REVOCATION equ 00000002h
CERT_VERIFY_REV_ACCUMULATIVE_TIMEOUT_FLAG equ 00000004h
CERT_UNICODE_IS_RDN_ATTRS_FLAG   equ 1h
CERT_CASE_INSENSITIVE_IS_RDN_ATTRS_FLAG equ 2h
CRYPT_VERIFY_CERT_SIGN_SUBJECT_BLOB equ 1
CRYPT_VERIFY_CERT_SIGN_SUBJECT_CERT equ 2
CRYPT_VERIFY_CERT_SIGN_SUBJECT_CRL equ 3
CRYPT_VERIFY_CERT_SIGN_ISSUER_PUBKEY equ 1
CRYPT_VERIFY_CERT_SIGN_ISSUER_CERT equ 2
CRYPT_VERIFY_CERT_SIGN_ISSUER_CHAIN equ 3
CRYPT_VERIFY_CERT_SIGN_ISSUER_NULL equ 4
CRYPT_DEFAULT_CONTEXT_AUTO_RELEASE_FLAG equ 00000001h
CRYPT_DEFAULT_CONTEXT_PROCESS_FLAG equ 00000002h
CRYPT_DEFAULT_CONTEXT_CERT_SIGN_OID equ 1
CRYPT_DEFAULT_CONTEXT_MULTI_CERT_SIGN_OID equ 2
CRYPT_ACQUIRE_CACHE_FLAG         equ 00000001h
CRYPT_ACQUIRE_USE_PROV_INFO_FLAG equ 00000002h
CRYPT_ACQUIRE_COMPARE_KEY_FLAG   equ 00000004h
CRYPT_ACQUIRE_SILENT_FLAG        equ 00000040h
CRYPT_FIND_USER_KEYSET_FLAG      equ 00000001h
CRYPT_FIND_MACHINE_KEYSET_FLAG   equ 00000002h
CRYPT_FIND_SILENT_KEYSET_FLAG    equ 00000040h
CRYPT_DELETE_KEYSET              equ 0001h
CERT_SIMPLE_NAME_STR             equ 1
CERT_OID_NAME_STR                equ 2
CERT_X500_NAME_STR               equ 3
CERT_NAME_STR_SEMICOLON_FLAG     equ 40000000h
CERT_NAME_STR_NO_PLUS_FLAG       equ 20000000h
CERT_NAME_STR_NO_QUOTING_FLAG    equ 10000000h
CERT_NAME_STR_CRLF_FLAG          equ 08000000h
CERT_NAME_STR_COMMA_FLAG         equ 04000000h
CERT_NAME_STR_REVERSE_FLAG       equ 02000000h
CERT_NAME_STR_DISABLE_IE4_UTF8_FLAG equ 00010000h
CERT_NAME_STR_ENABLE_T61_UNICODE_FLAG equ 00020000h
CERT_NAME_STR_ENABLE_UTF8_UNICODE_FLAG equ 00040000h
CERT_NAME_EMAIL_TYPE             equ 1
CERT_NAME_RDN_TYPE               equ 2
CERT_NAME_ATTR_TYPE              equ 3
CERT_NAME_SIMPLE_DISPLAY_TYPE    equ 4
CERT_NAME_FRIENDLY_DISPLAY_TYPE  equ 5
CERT_NAME_DNS_TYPE               equ 6
CERT_NAME_URL_TYPE               equ 7
CERT_NAME_UPN_TYPE               equ 8
CERT_NAME_ISSUER_FLAG            equ 1h
CERT_NAME_DISABLE_IE4_UTF8_FLAG  equ 00010000h
CRYPT_MESSAGE_BARE_CONTENT_OUT_FLAG equ 00000001h
CRYPT_MESSAGE_ENCAPSULATED_CONTENT_OUT_FLAG equ 00000002h
CRYPT_MESSAGE_KEYID_SIGNER_FLAG  equ 00000004h
CRYPT_MESSAGE_SILENT_KEYSET_FLAG equ 00000040h
CRYPT_MESSAGE_KEYID_RECIPIENT_FLAG equ 4h
CERT_QUERY_OBJECT_FILE           equ 00000001h
CERT_QUERY_OBJECT_BLOB           equ 00000002h
CERT_QUERY_CONTENT_CERT          equ 1
CERT_QUERY_CONTENT_CTL           equ 2
CERT_QUERY_CONTENT_CRL           equ 3
CERT_QUERY_CONTENT_SERIALIZED_STORE equ 4
CERT_QUERY_CONTENT_SERIALIZED_CERT equ 5
CERT_QUERY_CONTENT_SERIALIZED_CTL equ 6
CERT_QUERY_CONTENT_SERIALIZED_CRL equ 7
CERT_QUERY_CONTENT_PKCS7_SIGNED  equ 8
CERT_QUERY_CONTENT_PKCS7_UNSIGNED equ 9
CERT_QUERY_CONTENT_PKCS7_SIGNED_EMBED equ 10
CERT_QUERY_CONTENT_PKCS10        equ 11
CERT_QUERY_CONTENT_PFX           equ 12
CERT_QUERY_CONTENT_CERT_PAIR     equ 13
CERT_QUERY_FORMAT_BINARY         equ 1
CERT_QUERY_FORMAT_BASE64_ENCODED equ 2
CERT_QUERY_FORMAT_ASN_ASCII_HEX_ENCODED equ 3
CREDENTIAL_OID_PASSWORD_CREDENTIALS_A equ 1
CREDENTIAL_OID_PASSWORD_CREDENTIALS_W equ 2
CONTEXT_OID_CERTIFICATE          equ 1
CONTEXT_OID_CRL                  equ 2
CONTEXT_OID_CTL                  equ 3
CONTEXT_OID_PKCS7                equ 4
CONTEXT_OID_CAPI2_ANY            equ 5
CRYPT_RETRIEVE_MULTIPLE_OBJECTS  equ 00000001h
CRYPT_CACHE_ONLY_RETRIEVAL       equ 00000002h
CRYPT_WIRE_ONLY_RETRIEVAL        equ 00000004h
CRYPT_DONT_CACHE_RESULT          equ 00000008h
CRYPT_ASYNC_RETRIEVAL            equ 00000010h
CRYPT_STICKY_CACHE_RETRIEVAL     equ 00001000h
CRYPT_LDAP_SCOPE_BASE_ONLY_RETRIEVAL equ 00002000h
CRYPT_OFFLINE_CHECK_RETRIEVAL    equ 00004000h
CRYPT_LDAP_INSERT_ENTRY_ATTRIBUTE equ 00008000h
CRYPT_LDAP_SIGN_RETRIEVAL        equ 00010000h
CRYPT_NO_AUTH_RETRIEVAL          equ 00020000h
CRYPT_LDAP_AREC_EXCLUSIVE_RETRIEVAL equ 00040000h
CRYPT_AIA_RETRIEVAL              equ 00080000h
CRYPT_VERIFY_CONTEXT_SIGNATURE   equ 00000020h
CRYPT_VERIFY_DATA_HASH           equ 00000040h
CRYPT_KEEP_TIME_VALID            equ 00000080h
CRYPT_DONT_VERIFY_SIGNATURE      equ 00000100h
CRYPT_DONT_CHECK_TIME_VALIDITY   equ 00000200h
CRYPT_CHECK_FRESHNESS_TIME_VALIDITY equ 00000400h
CRYPT_ACCUMULATIVE_TIMEOUT       equ 00000800h
CRYPT_PARAM_ASYNC_RETRIEVAL_COMPLETION equ 1
CRYPT_PARAM_CANCEL_ASYNC_RETRIEVAL equ 2
CRYPT_GET_URL_FROM_PROPERTY      equ 00000001h
CRYPT_GET_URL_FROM_EXTENSION     equ 00000002h
CRYPT_GET_URL_FROM_UNAUTH_ATTRIBUTE equ 00000004h
CRYPT_GET_URL_FROM_AUTH_ATTRIBUTE equ 00000008h
URL_OID_CERTIFICATE_ISSUER       equ 1
URL_OID_CERTIFICATE_CRL_DIST_POINT equ 2
URL_OID_CTL_ISSUER               equ 3
URL_OID_CTL_NEXT_UPDATE          equ 4
URL_OID_CRL_ISSUER               equ 5
URL_OID_CERTIFICATE_FRESHEST_CRL equ 6
URL_OID_CRL_FRESHEST_CRL         equ 7
URL_OID_CROSS_CERT_DIST_POINT    equ 8
TIME_VALID_OID_GET_CTL           equ 1
TIME_VALID_OID_GET_CRL           equ 2
TIME_VALID_OID_GET_CRL_FROM_CERT equ 3
TIME_VALID_OID_GET_FRESHEST_CRL_FROM_CERT equ 4
TIME_VALID_OID_GET_FRESHEST_CRL_FROM_CRL equ 5
TIME_VALID_OID_FLUSH_CTL         equ 1
TIME_VALID_OID_FLUSH_CRL         equ 2
TIME_VALID_OID_FLUSH_CRL_FROM_CERT equ 3
TIME_VALID_OID_FLUSH_FRESHEST_CRL_FROM_CERT equ 4
TIME_VALID_OID_FLUSH_FRESHEST_CRL_FROM_CRL equ 5
CRYPTPROTECT_PROMPT_ON_UNPROTECT equ 1h
CRYPTPROTECT_PROMPT_ON_PROTECT   equ 2h
CRYPTPROTECT_PROMPT_RESERVED     equ 04h
CRYPTPROTECT_PROMPT_STRONG       equ 08h
CRYPTPROTECT_PROMPT_REQUIRE_STRONG equ 10h
CRYPTPROTECT_UI_FORBIDDEN        equ 1h
CRYPTPROTECT_LOCAL_MACHINE       equ 4h
CRYPTPROTECT_CRED_SYNC           equ 8h
CRYPTPROTECT_AUDIT               equ 10h
CRYPTPROTECT_NO_RECOVERY         equ 20h
CRYPTPROTECT_VERIFY_PROTECTION   equ 40h
CRYPTPROTECT_CRED_REGENERATE     equ 80h
CRYPTPROTECT_FIRST_RESERVED_FLAGVAL equ 0FFFFFFFh
CRYPTPROTECT_LAST_RESERVED_FLAGVAL equ 0FFFFFFFFh
CRYPTPROTECTMEMORY_BLOCK_SIZE    equ 16
CRYPTPROTECTMEMORY_SAME_PROCESS  equ 00h
CRYPTPROTECTMEMORY_CROSS_PROCESS equ 01h
CRYPTPROTECTMEMORY_SAME_LOGON    equ 02h
CERT_CREATE_SELFSIGN_NO_SIGN     equ 1
CERT_CREATE_SELFSIGN_NO_KEY_INFO equ 2
CRYPT_KEYID_MACHINE_FLAG         equ 00000020h
CRYPT_KEYID_ALLOC_FLAG           equ 00008000h
CRYPT_KEYID_DELETE_FLAG          equ 00000010h
CRYPT_KEYID_SET_NEW_FLAG         equ 00002000h
CERT_CHAIN_MAX_AIA_URL_COUNT_IN_CERT_DEFAULT equ 5
CERT_CHAIN_MAX_AIA_URL_RETRIEVAL_COUNT_PER_CHAIN_DEFAULT equ 10
CERT_CHAIN_MAX_AIA_URL_RETRIEVAL_BYTE_COUNT_DEFAULT equ 100000
CERT_CHAIN_MAX_AIA_URL_RETRIEVAL_CERT_COUNT_DEFAULT equ 10
HCCE_CURRENT_USER                equ 0
HCCE_LOCAL_MACHINE               equ 1
CERT_CHAIN_CACHE_END_CERT        equ 00000001h
CERT_CHAIN_THREAD_STORE_SYNC     equ 00000002h
CERT_CHAIN_CACHE_ONLY_URL_RETRIEVAL equ 00000004h
CERT_CHAIN_USE_LOCAL_MACHINE_STORE equ 00000008h
CERT_CHAIN_ENABLE_CACHE_AUTO_UPDATE equ 00000010h
CERT_CHAIN_ENABLE_SHARE_STORE    equ 00000020h
CERT_TRUST_NO_ERROR              equ 00000000h
CERT_TRUST_IS_NOT_TIME_VALID     equ 00000001h
CERT_TRUST_IS_NOT_TIME_NESTED    equ 00000002h
CERT_TRUST_IS_REVOKED            equ 00000004h
CERT_TRUST_IS_NOT_SIGNATURE_VALID equ 00000008h
CERT_TRUST_IS_NOT_VALID_FOR_USAGE equ 00000010h
CERT_TRUST_IS_UNTRUSTED_ROOT     equ 00000020h
CERT_TRUST_REVOCATION_STATUS_UNKNOWN equ 00000040h
CERT_TRUST_IS_CYCLIC             equ 00000080h
CERT_TRUST_INVALID_EXTENSION     equ 00000100h
CERT_TRUST_INVALID_POLICY_CONSTRAINTS equ 00000200h
CERT_TRUST_INVALID_BASIC_CONSTRAINTS equ 00000400h
CERT_TRUST_INVALID_NAME_CONSTRAINTS equ 00000800h
CERT_TRUST_HAS_NOT_SUPPORTED_NAME_CONSTRAINT equ 00001000h
CERT_TRUST_HAS_NOT_DEFINED_NAME_CONSTRAINT equ 00002000h
CERT_TRUST_HAS_NOT_PERMITTED_NAME_CONSTRAINT equ 00004000h
CERT_TRUST_HAS_EXCLUDED_NAME_CONSTRAINT equ 00008000h
CERT_TRUST_IS_OFFLINE_REVOCATION equ 01000000h
CERT_TRUST_NO_ISSUANCE_CHAIN_POLICY equ 02000000h
CERT_TRUST_IS_PARTIAL_CHAIN      equ 00010000h
CERT_TRUST_CTL_IS_NOT_TIME_VALID equ 00020000h
CERT_TRUST_CTL_IS_NOT_SIGNATURE_VALID equ 00040000h
CERT_TRUST_CTL_IS_NOT_VALID_FOR_USAGE equ 00080000h
CERT_TRUST_HAS_EXACT_MATCH_ISSUER equ 00000001h
CERT_TRUST_HAS_KEY_MATCH_ISSUER  equ 00000002h
CERT_TRUST_HAS_NAME_MATCH_ISSUER equ 00000004h
CERT_TRUST_IS_SELF_SIGNED        equ 00000008h
CERT_TRUST_HAS_PREFERRED_ISSUER  equ 00000100h
CERT_TRUST_HAS_ISSUANCE_CHAIN_POLICY equ 00000200h
CERT_TRUST_HAS_VALID_NAME_CONSTRAINTS equ 00000400h
CERT_TRUST_IS_COMPLEX_CHAIN      equ 00010000h
USAGE_MATCH_TYPE_AND             equ 00000000h
USAGE_MATCH_TYPE_OR              equ 00000001h
CERT_CHAIN_REVOCATION_CHECK_END_CERT equ 10000000h
CERT_CHAIN_REVOCATION_CHECK_CHAIN equ 20000000h
CERT_CHAIN_REVOCATION_CHECK_CHAIN_EXCLUDE_ROOT equ 40000000h
CERT_CHAIN_REVOCATION_CHECK_CACHE_ONLY equ 80000000h
CERT_CHAIN_REVOCATION_ACCUMULATIVE_TIMEOUT equ 08000000h
CERT_CHAIN_DISABLE_PASS1_QUALITY_FILTERING equ 00000040h
CERT_CHAIN_RETURN_LOWER_QUALITY_CONTEXTS equ 00000080h
CERT_CHAIN_DISABLE_AUTH_ROOT_AUTO_UPDATE equ 00000100h
CERT_CHAIN_TIMESTAMP_TIME        equ 00000200h
REVOCATION_OID_CRL_REVOCATION    equ 1
CERT_CHAIN_FIND_BY_ISSUER        equ 1
CERT_CHAIN_FIND_BY_ISSUER_COMPARE_KEY_FLAG equ 0001h
CERT_CHAIN_FIND_BY_ISSUER_COMPLEX_CHAIN_FLAG equ 0002h
CERT_CHAIN_FIND_BY_ISSUER_CACHE_ONLY_URL_FLAG equ 0004h
CERT_CHAIN_FIND_BY_ISSUER_LOCAL_MACHINE_FLAG equ 0008h
CERT_CHAIN_FIND_BY_ISSUER_NO_KEY_FLAG equ 4000h
CERT_CHAIN_FIND_BY_ISSUER_CACHE_ONLY_FLAG equ 8000h
CERT_CHAIN_POLICY_IGNORE_NOT_TIME_VALID_FLAG equ 00000001h
CERT_CHAIN_POLICY_IGNORE_CTL_NOT_TIME_VALID_FLAG equ 00000002h
CERT_CHAIN_POLICY_IGNORE_NOT_TIME_NESTED_FLAG equ 00000004h
CERT_CHAIN_POLICY_IGNORE_INVALID_BASIC_CONSTRAINTS_FLAG equ 00000008h
CERT_CHAIN_POLICY_ALLOW_UNKNOWN_CA_FLAG equ 00000010h
CERT_CHAIN_POLICY_IGNORE_WRONG_USAGE_FLAG equ 00000020h
CERT_CHAIN_POLICY_IGNORE_INVALID_NAME_FLAG equ 00000040h
CERT_CHAIN_POLICY_IGNORE_INVALID_POLICY_FLAG equ 00000080h
CERT_CHAIN_POLICY_IGNORE_END_REV_UNKNOWN_FLAG equ 00000100h
CERT_CHAIN_POLICY_IGNORE_CTL_SIGNER_REV_UNKNOWN_FLAG equ 00000200h
CERT_CHAIN_POLICY_IGNORE_CA_REV_UNKNOWN_FLAG equ 00000400h
CERT_CHAIN_POLICY_IGNORE_ROOT_REV_UNKNOWN_FLAG equ 00000800h
CERT_CHAIN_POLICY_ALLOW_TESTROOT_FLAG equ 00008000h
CERT_CHAIN_POLICY_TRUST_TESTROOT_FLAG equ 00004000h
CERT_CHAIN_POLICY_BASE           equ 1
CERT_CHAIN_POLICY_AUTHENTICODE   equ 2
CERT_CHAIN_POLICY_AUTHENTICODE_TS equ 3
CERT_CHAIN_POLICY_SSL            equ 4
CERT_CHAIN_POLICY_BASIC_CONSTRAINTS equ 5
CERT_CHAIN_POLICY_NT_AUTH        equ 6
CERT_CHAIN_POLICY_MICROSOFT_ROOT equ 7
BASIC_CONSTRAINTS_CERT_CHAIN_POLICY_CA_FLAG equ 80000000h
BASIC_CONSTRAINTS_CERT_CHAIN_POLICY_END_ENTITY_FLAG equ 40000000h
MICROSOFT_ROOT_CERT_CHAIN_POLICY_ENABLE_TEST_ROOT_FLAG equ 00010000h
CRYPT_STRING_BASE64HEADER        equ 00000000h
CRYPT_STRING_BASE64              equ 00000001h
CRYPT_STRING_BINARY              equ 00000002h
CRYPT_STRING_BASE64REQUESTHEADER equ 00000003h
CRYPT_STRING_HEX                 equ 00000004h
CRYPT_STRING_HEXASCII            equ 00000005h
CRYPT_STRING_BASE64_ANY          equ 00000006h
CRYPT_STRING_ANY                 equ 00000007h
CRYPT_STRING_HEX_ANY             equ 00000008h
CRYPT_STRING_BASE64X509CRLHEADER equ 00000009h
CRYPT_STRING_HEXADDR             equ 0000000ah
CRYPT_STRING_HEXASCIIADDR        equ 0000000bh
CRYPT_STRING_NOCR                equ 80000000h
CRYPT_USER_KEYSET                equ 00001000h
PKCS12_IMPORT_RESERVED_MASK      equ 0ffff0000h
REPORT_NO_PRIVATE_KEY            equ 0001h
REPORT_NOT_ABLE_TO_EXPORT_PRIVATE_KEY equ 0002h
EXPORT_PRIVATE_KEYS              equ 0004h
PKCS12_EXPORT_RESERVED_MASK      equ 0ffff0000h

;;;; end wincrypt

;;;; odbcsql

;------------------------------------------------------------------------------------------
;                                   ODBCInst.h
;------------------------------------------------------------------------------------------
ODBC_ADD_DSN equ 1               
ODBC_CONFIG_DSN equ 2               
ODBC_REMOVE_DSN equ 3               
ODBC_ADD_SYS_DSN equ 4				  
ODBC_CONFIG_SYS_DSN	equ 5		  
ODBC_REMOVE_SYS_DSN	equ 6		  
ODBC_REMOVE_DEFAULT_DSN	equ 7		
ODBC_INSTALL_INQUIRY	equ 1		
ODBC_INSTALL_COMPLETE	equ 2
ODBC_INSTALL_DRIVER	equ 1
ODBC_REMOVE_DRIVER	equ 2
ODBC_CONFIG_DRIVER	equ 3
ODBC_CONFIG_DRIVER_MAX equ 100
ODBC_BOTH_DSN	equ 0
ODBC_USER_DSN	equ 1
ODBC_SYSTEM_DSN	equ 2
ODBC_ERROR_GENERAL_ERR equ 1
ODBC_ERROR_INVALID_BUFF_LEN equ 2
ODBC_ERROR_INVALID_HWND equ 3
ODBC_ERROR_INVALID_STR equ 4
ODBC_ERROR_INVALID_REQUEST_TYPE equ 5
ODBC_ERROR_COMPONENT_NOT_FOUND equ 6
ODBC_ERROR_INVALID_NAME equ 7
ODBC_ERROR_INVALID_KEYWORD_VALUE equ 8
ODBC_ERROR_INVALID_DSN equ 9
ODBC_ERROR_INVALID_INF equ 10
ODBC_ERROR_REQUEST_FAILED equ 11
ODBC_ERROR_INVALID_PATH equ 12
ODBC_ERROR_LOAD_LIB_FAILED equ 13
ODBC_ERROR_INVALID_PARAM_SEQUENCE equ 14
ODBC_ERROR_INVALID_LOG_FILE equ 15
ODBC_ERROR_USER_CANCELED equ 16
ODBC_ERROR_USAGE_UPDATE_FAILED equ 17
ODBC_ERROR_CREATE_DSN_FAILED equ 18
ODBC_ERROR_WRITING_SYSINFO_FAILED equ 19
ODBC_ERROR_REMOVE_DSN_FAILED equ 20
ODBC_ERROR_OUT_OF_MEM equ 21
ODBC_ERROR_OUTPUT_STRING_TRUNCATED equ 22
;---------------------------------------------------------------------------------------------------------------
;							sql.h
;---------------------------------------------------------------------------------------------------------------
SQL_NULL_DATA equ (-1)
SQL_DATA_AT_EXEC equ (-2)
SQL_SUCCESS equ 0
SQL_SUCCESS_WITH_INFO equ 1
SQL_NO_DATA equ 100
SQL_ERROR equ (-1)
SQL_INVALID_HANDLE equ (-2)
SQL_STILL_EXECUTING equ 2
SQL_NEED_DATA equ 99
SQL_NTS equ (-3)
SQL_NTSL equ (-3)
SQL_MAX_MESSAGE_LENGTH equ 512
SQL_DATE_LEN equ 10
SQL_TIME_LEN equ 8
SQL_TIMESTAMP_LEN equ 19
SQL_HANDLE_ENV equ 1
SQL_HANDLE_DBC equ 2
SQL_HANDLE_STMT equ 3
SQL_HANDLE_DESC equ 4
SQL_ATTR_OUTPUT_NTS equ 10001
SQL_ATTR_AUTO_IPD equ 10001
SQL_ATTR_METADATA_ID equ 10014
SQL_ATTR_APP_ROW_DESC equ 10010
SQL_ATTR_APP_PARAM_DESC equ 10011
SQL_ATTR_IMP_ROW_DESC equ 10012
SQL_ATTR_IMP_PARAM_DESC equ 10013
SQL_ATTR_CURSOR_SCROLLABLE equ (-1)
SQL_ATTR_CURSOR_SENSITIVITY equ (-2)
SQL_NONSCROLLABLE	equ 0
SQL_SCROLLABLE	equ 1
SQL_DESC_COUNT equ 1001
SQL_DESC_TYPE equ 1002
SQL_DESC_LENGTH equ 1003
SQL_DESC_OCTET_LENGTH_PTR equ 1004
SQL_DESC_PRECISION equ 1005
SQL_DESC_SCALE equ 1006
SQL_DESC_DATETIME_INTERVAL_CODE equ 1007
SQL_DESC_NULLABLE equ 1008
SQL_DESC_INDICATOR_PTR equ 1009
SQL_DESC_DATA_PTR equ 1010
SQL_DESC_NAME equ 1011
SQL_DESC_UNNAMED equ 1012
SQL_DESC_OCTET_LENGTH equ 1013
SQL_DESC_ALLOC_TYPE equ 1099
SQL_DIAG_RETURNCODE equ 1
SQL_DIAG_NUMBER equ 2
SQL_DIAG_ROW_COUNT equ 3
SQL_DIAG_SQLSTATE equ 4
SQL_DIAG_NATIVE equ 5
SQL_DIAG_MESSAGE_TEXT equ 6
SQL_DIAG_DYNAMIC_FUNCTION equ 7
SQL_DIAG_CLASS_ORIGIN equ 8
SQL_DIAG_SUBCLASS_ORIGIN equ 9
SQL_DIAG_CONNECTION_NAME equ 10
SQL_DIAG_SERVER_NAME equ 11
SQL_DIAG_DYNAMIC_FUNCTION_CODE equ 12
SQL_DIAG_ALTER_DOMAIN	equ 3
SQL_DIAG_ALTER_TABLE equ 4
SQL_DIAG_CALL	equ 7
SQL_DIAG_CREATE_ASSERTION	equ 6
SQL_DIAG_CREATE_CHARACTER_SET	equ 8
SQL_DIAG_CREATE_COLLATION	equ 10
SQL_DIAG_CREATE_DOMAIN	equ 23
SQL_DIAG_CREATE_INDEX equ (-1)
SQL_DIAG_CREATE_SCHEMA	equ 64
SQL_DIAG_CREATE_TABLE equ 77
SQL_DIAG_CREATE_TRANSLATION	equ 79
SQL_DIAG_CREATE_VIEW equ 84
SQL_DIAG_DELETE_WHERE equ 19
SQL_DIAG_DROP_ASSERTION	equ 24
SQL_DIAG_DROP_CHARACTER_SET	equ 25
SQL_DIAG_DROP_COLLATION	equ 26
SQL_DIAG_DROP_DOMAIN	equ 27
SQL_DIAG_DROP_INDEX equ (-2)
SQL_DIAG_DROP_SCHEMA	equ 31
SQL_DIAG_DROP_TABLE equ 32
SQL_DIAG_DROP_TRANSLATION equ 33
SQL_DIAG_DROP_VIEW equ 36
SQL_DIAG_DYNAMIC_DELETE_CURSOR equ 38
SQL_DIAG_DYNAMIC_UPDATE_CURSOR equ 81
SQL_DIAG_GRANT equ 48
SQL_DIAG_INSERT equ 50
SQL_DIAG_REVOKE equ 59
SQL_DIAG_SELECT_CURSOR equ 85
SQL_DIAG_UNKNOWN_STATEMENT equ 0
SQL_DIAG_UPDATE_WHERE equ 82
SQL_UNKNOWN_TYPE	equ 0
SQL_CHAR equ 1
SQL_NUMERIC equ 2
SQL_DECIMAL equ 3
SQL_INTEGER equ 4
SQL_SMALLINT equ 5
SQL_FLOAT equ 6
SQL_REAL equ 7
SQL_DOUBLE equ 8
SQL_DATETIME equ 9
SQL_VARCHAR equ 12
SQL_TYPE_DATE equ 91
SQL_TYPE_TIME equ 92
SQL_TYPE_TIMESTAMP equ 93
SQL_UNSPECIFIED equ 0
SQL_INSENSITIVE equ 1
SQL_SENSITIVE equ 2
SQL_ALL_TYPES equ 0
SQL_DEFAULT equ 99
SQL_ARD_TYPE equ (-99)
SQL_CODE_DATE equ 1
SQL_CODE_TIME equ 2
SQL_CODE_TIMESTAMP equ 3
SQL_FALSE equ 0
SQL_TRUE equ 1
SQL_NO_NULLS equ 0
SQL_NULLABLE equ 1
SQL_NULLABLE_UNKNOWN equ 2
SQL_PRED_NONE equ 0
SQL_PRED_CHAR equ 1
SQL_PRED_BASIC equ 2
SQL_NAMED equ 0
SQL_UNNAMED equ 1
SQL_DESC_ALLOC_AUTO equ 1
SQL_DESC_ALLOC_USER equ 2
SQL_CLOSE equ 0
SQL_DROP equ 1
SQL_UNBIND equ 2
SQL_RESET_PARAMS equ 3
SQL_FETCH_NEXT equ 1
SQL_FETCH_FIRST equ 2
SQL_FETCH_LAST equ 3
SQL_FETCH_PRIOR equ 4
SQL_FETCH_ABSOLUTE equ 5
SQL_FETCH_RELATIVE equ 6
SQL_COMMIT equ 0
SQL_ROLLBACK equ 1
SQL_NULL_HENV equ 0
SQL_NULL_HDBC equ 0
SQL_NULL_HSTMT equ 0
SQL_NULL_HDESC equ 0
SQL_NULL_HANDLE equ 0
SQL_SCOPE_CURROW equ 0
SQL_SCOPE_TRANSACTION equ 1
SQL_SCOPE_SESSION equ 2
SQL_PC_UNKNOWN equ 0
SQL_PC_NON_PSEUDO equ 1
SQL_PC_PSEUDO equ 2
SQL_ROW_IDENTIFIER equ 1
SQL_INDEX_UNIQUE equ 0
SQL_INDEX_ALL equ 1
SQL_INDEX_CLUSTERED equ 1
SQL_INDEX_HASHED equ 2
SQL_INDEX_OTHER equ 3
SQL_API_SQLALLOCCONNECT equ 1
SQL_API_SQLALLOCENV equ 2
SQL_API_SQLALLOCHANDLE equ 1001
SQL_API_SQLALLOCSTMT equ 3
SQL_API_SQLBINDCOL equ 4
SQL_API_SQLBINDPARAM equ 1002
SQL_API_SQLCANCEL equ 5
SQL_API_SQLCLOSECURSOR equ 1003
SQL_API_SQLCOLATTRIBUTE equ 6
SQL_API_SQLCOLUMNS equ 40
SQL_API_SQLCONNECT equ 7
SQL_API_SQLCOPYDESC equ 1004
SQL_API_SQLDATASOURCES equ 57
SQL_API_SQLDESCRIBECOL equ 8
SQL_API_SQLDISCONNECT equ 9
SQL_API_SQLENDTRAN equ 1005
SQL_API_SQLERROR equ 10
SQL_API_SQLEXECDIRECT equ 11
SQL_API_SQLEXECUTE equ 12
SQL_API_SQLFETCH equ 13
SQL_API_SQLFETCHSCROLL equ 1021
SQL_API_SQLFREECONNECT equ 14
SQL_API_SQLFREEENV equ 15
SQL_API_SQLFREEHANDLE equ 1006
SQL_API_SQLFREESTMT equ 16
SQL_API_SQLGETCONNECTATTR equ 1007
SQL_API_SQLGETCONNECTOPTION equ 42
SQL_API_SQLGETCURSORNAME equ 17
SQL_API_SQLGETDATA equ 43
SQL_API_SQLGETDESCFIELD equ 1008
SQL_API_SQLGETDESCREC equ 1009
SQL_API_SQLGETDIAGFIELD equ 1010
SQL_API_SQLGETDIAGREC equ 1011
SQL_API_SQLGETENVATTR equ 1012
SQL_API_SQLGETFUNCTIONS equ 44
SQL_API_SQLGETINFO equ 45
SQL_API_SQLGETSTMTATTR equ 1014
SQL_API_SQLGETSTMTOPTION equ 46
SQL_API_SQLGETTYPEINFO equ 47
SQL_API_SQLNUMRESULTCOLS equ 18
SQL_API_SQLPARAMDATA equ 48
SQL_API_SQLPREPARE equ 19
SQL_API_SQLPUTDATA equ 49
SQL_API_SQLROWCOUNT equ 20
SQL_API_SQLSETCONNECTATTR equ 1016
SQL_API_SQLSETCONNECTOPTION equ 50
SQL_API_SQLSETCURSORNAME equ 21
SQL_API_SQLSETDESCFIELD equ 1017
SQL_API_SQLSETDESCREC equ 1018
SQL_API_SQLSETENVATTR equ 1019
SQL_API_SQLSETPARAM equ 22
SQL_API_SQLSETSTMTATTR equ 1020
SQL_API_SQLSETSTMTOPTION equ 51
SQL_API_SQLSPECIALCOLUMNS equ 52
SQL_API_SQLSTATISTICS equ 53
SQL_API_SQLTABLES equ 54
SQL_API_SQLTRANSACT equ 23
SQL_MAX_DRIVER_CONNECTIONS equ 0
SQL_MAXIMUM_DRIVER_CONNECTIONS	equ SQL_MAX_DRIVER_CONNECTIONS
SQL_MAX_CONCURRENT_ACTIVITIES equ 1
SQL_MAXIMUM_CONCURRENT_ACTIVITIES	equ SQL_MAX_CONCURRENT_ACTIVITIES
SQL_DATA_SOURCE_NAME equ 2
SQL_FETCH_DIRECTION equ 8
SQL_SERVER_NAME equ 13
SQL_SEARCH_PATTERN_ESCAPE equ 14
SQL_DBMS_NAME equ 17
SQL_DBMS_VER equ 18
SQL_ACCESSIBLE_TABLES equ 19
SQL_ACCESSIBLE_PROCEDURES equ 20
SQL_CURSOR_COMMIT_BEHAVIOR equ 23
SQL_DATA_SOURCE_READ_ONLY equ 25
SQL_DEFAULT_TXN_ISOLATION equ 26
SQL_IDENTIFIER_CASE equ 28
SQL_IDENTIFIER_QUOTE_CHAR equ 29
SQL_MAX_COLUMN_NAME_LEN equ 30
SQL_MAXIMUM_COLUMN_NAME_LENGTH	equ SQL_MAX_COLUMN_NAME_LEN
SQL_MAX_CURSOR_NAME_LEN equ 31
SQL_MAXIMUM_CURSOR_NAME_LENGTH	equ SQL_MAX_CURSOR_NAME_LEN
SQL_MAX_SCHEMA_NAME_LEN equ 32
SQL_MAXIMUM_SCHEMA_NAME_LENGTH	equ SQL_MAX_SCHEMA_NAME_LEN
SQL_MAX_CATALOG_NAME_LEN equ 34
SQL_MAXIMUM_CATALOG_NAME_LENGTH	equ SQL_MAX_CATALOG_NAME_LEN
SQL_MAX_TABLE_NAME_LEN equ 35
SQL_SCROLL_CONCURRENCY equ 43
SQL_TXN_CAPABLE equ 46
SQL_TRANSACTION_CAPABLE	equ SQL_TXN_CAPABLE
SQL_USER_NAME equ 47
SQL_TXN_ISOLATION_OPTION equ 72
SQL_TRANSACTION_ISOLATION_OPTION	equ SQL_TXN_ISOLATION_OPTION
SQL_INTEGRITY equ 73
SQL_GETDATA_EXTENSIONS equ 81
SQL_NULL_COLLATION equ 85
SQL_ALTER_TABLE equ 86
SQL_ORDER_BY_COLUMNS_IN_SELECT equ 90
SQL_SPECIAL_CHARACTERS equ 94
SQL_MAX_COLUMNS_IN_GROUP_BY equ 97
SQL_MAXIMUM_COLUMNS_IN_GROUP_BY	equ SQL_MAX_COLUMNS_IN_GROUP_BY
SQL_MAX_COLUMNS_IN_INDEX equ 98
SQL_MAXIMUM_COLUMNS_IN_INDEX	equ SQL_MAX_COLUMNS_IN_INDEX
SQL_MAX_COLUMNS_IN_ORDER_BY equ 99
SQL_MAXIMUM_COLUMNS_IN_ORDER_BY	equ SQL_MAX_COLUMNS_IN_ORDER_BY
SQL_MAX_COLUMNS_IN_SELECT equ 100
SQL_MAXIMUM_COLUMNS_IN_SELECT	equ SQL_MAX_COLUMNS_IN_SELECT
SQL_MAX_COLUMNS_IN_TABLE equ 101
SQL_MAX_INDEX_SIZE equ 102
SQL_MAXIMUM_INDEX_SIZE	equ SQL_MAX_INDEX_SIZE
SQL_MAX_ROW_SIZE equ 104
SQL_MAXIMUM_ROW_SIZE	equ SQL_MAX_ROW_SIZE
SQL_MAX_STATEMENT_LEN equ 105
SQL_MAXIMUM_STATEMENT_LENGTH	equ SQL_MAX_STATEMENT_LEN
SQL_MAX_TABLES_IN_SELECT equ 106
SQL_MAXIMUM_TABLES_IN_SELECT	equ SQL_MAX_TABLES_IN_SELECT
SQL_MAX_USER_NAME_LEN equ 107
SQL_MAXIMUM_USER_NAME_LENGTH	equ SQL_MAX_USER_NAME_LEN
SQL_OJ_CAPABILITIES equ 115
SQL_OUTER_JOIN_CAPABILITIES	equ SQL_OJ_CAPABILITIES
SQL_XOPEN_CLI_YEAR equ 10000
SQL_CURSOR_SENSITIVITY equ 10001
SQL_DESCRIBE_PARAMETER equ 10002
SQL_CATALOG_NAME equ 10003
SQL_COLLATION_SEQ equ 10004
SQL_MAX_IDENTIFIER_LEN equ 10005
SQL_MAXIMUM_IDENTIFIER_LENGTH	equ SQL_MAX_IDENTIFIER_LEN
SQL_AT_ADD_COLUMN equ 1
SQL_AT_DROP_COLUMN equ 2
SQL_AT_ADD_CONSTRAINT equ 8
SQL_AT_COLUMN_SINGLE	equ 20h	
SQL_AT_ADD_COLUMN_DEFAULT	equ 40h
SQL_AT_ADD_COLUMN_COLLATION	equ 80h
SQL_AT_SET_COLUMN_DEFAULT	equ 100h
SQL_AT_DROP_COLUMN_DEFAULT	equ 200h
SQL_AT_DROP_COLUMN_CASCADE	equ 400h
SQL_AT_DROP_COLUMN_RESTRICT	equ 800h
SQL_AT_ADD_TABLE_CONSTRAINT	equ 1000h		
SQL_AT_DROP_TABLE_CONSTRAINT_CASCADE	equ 2000h		
SQL_AT_DROP_TABLE_CONSTRAINT_RESTRICT	equ 4000h		
SQL_AT_CONSTRAINT_NAME_DEFINITION	equ 8000h
SQL_AT_CONSTRAINT_INITIALLY_DEFERRED	equ 10000h
SQL_AT_CONSTRAINT_INITIALLY_IMMEDIATE	equ 20000h
SQL_AT_CONSTRAINT_DEFERRABLE	equ 40000h
SQL_AT_CONSTRAINT_NON_DEFERRABLE	equ 80000h
SQL_AM_NONE equ 0
SQL_AM_CONNECTION equ 1
SQL_AM_STATEMENT equ 2
SQL_CB_DELETE equ 0
SQL_CB_CLOSE equ 1
SQL_CB_PRESERVE equ 2
SQL_FD_FETCH_NEXT equ 1
SQL_FD_FETCH_FIRST equ 2
SQL_FD_FETCH_LAST equ 4
SQL_FD_FETCH_PRIOR equ 8
SQL_FD_FETCH_ABSOLUTE equ 10h
SQL_FD_FETCH_RELATIVE equ 20h
SQL_GD_ANY_COLUMN equ 1
SQL_GD_ANY_ORDER equ 2
SQL_IC_UPPER equ 1
SQL_IC_LOWER equ 2
SQL_IC_SENSITIVE equ 3
SQL_IC_MIXED equ 4
SQL_OJ_LEFT equ 1
SQL_OJ_RIGHT equ 2
SQL_OJ_FULL equ 4
SQL_OJ_NESTED equ 8
SQL_OJ_NOT_ORDERED equ 10h
SQL_OJ_INNER equ 20h
SQL_OJ_ALL_COMPARISON_OPS equ 40h
SQL_SCCO_READ_ONLY equ 1
SQL_SCCO_LOCK equ 2
SQL_SCCO_OPT_ROWVER equ 4
SQL_SCCO_OPT_VALUES equ 8
SQL_TC_NONE equ 0
SQL_TC_DML equ 1
SQL_TC_ALL equ 2
SQL_TC_DDL_COMMIT equ 3
SQL_TC_DDL_IGNORE equ 4
SQL_TXN_READ_UNCOMMITTED equ 1
SQL_TRANSACTION_READ_UNCOMMITTED	equ SQL_TXN_READ_UNCOMMITTED
SQL_TXN_READ_COMMITTED equ 2
SQL_TRANSACTION_READ_COMMITTED	equ SQL_TXN_READ_COMMITTED
SQL_TXN_REPEATABLE_READ equ 4
SQL_TRANSACTION_REPEATABLE_READ	equ SQL_TXN_REPEATABLE_READ
SQL_TXN_SERIALIZABLE equ 8
SQL_TRANSACTION_SERIALIZABLE	equ SQL_TXN_SERIALIZABLE
SQL_NC_HIGH equ 0
SQL_NC_LOW equ 1

;------------------------------------------------------------------------------------------------------------
;					sqlext.h
;-------------------------------------------------------------------------------------------------------------

SQL_SPEC_MAJOR equ 3
SQL_SPEC_MINOR	equ 51
SQL_SPEC_STRING equ "03.51"
SQL_SQLSTATE_SIZE	equ 5
SQL_MAX_DSN_LENGTH	equ 32
SQL_MAX_OPTION_STRING_LENGTH equ 256
SQL_NO_DATA_FOUND	equ SQL_NO_DATA
SQL_HANDLE_SENV	equ 5
SQL_ATTR_ODBC_VERSION	equ 200
SQL_ATTR_CONNECTION_POOLING	equ 201
SQL_ATTR_CP_MATCH	equ 202
SQL_CP_OFF	equ 0
SQL_CP_ONE_PER_DRIVER	equ 1
SQL_CP_ONE_PER_HENV	equ 2
SQL_CP_DEFAULT	equ SQL_CP_OFF
SQL_CP_STRICT_MATCH	equ 0
SQL_CP_RELAXED_MATCH	equ 1
SQL_CP_MATCH_DEFAULT	equ SQL_CP_STRICT_MATCH		
SQL_OV_ODBC2	equ 2
SQL_OV_ODBC3 equ 3
SQL_ACCESS_MODE equ 101
SQL_AUTOCOMMIT equ 102
SQL_LOGIN_TIMEOUT equ 103
SQL_OPT_TRACE equ 104
SQL_OPT_TRACEFILE equ 105
SQL_TRANSLATE_DLL equ 106
SQL_TRANSLATE_OPTION equ 107
SQL_TXN_ISOLATION equ 108
SQL_CURRENT_QUALIFIER equ 109
SQL_ODBC_CURSORS equ 110
SQL_QUIET_MODE equ 111
SQL_PACKET_SIZE equ 112
SQL_ATTR_ACCESS_MODE	equ SQL_ACCESS_MODE
SQL_ATTR_AUTOCOMMIT	equ SQL_AUTOCOMMIT
SQL_ATTR_CONNECTION_TIMEOUT	equ 113
SQL_ATTR_CURRENT_CATALOG	equ SQL_CURRENT_QUALIFIER
SQL_ATTR_DISCONNECT_BEHAVIOR	equ 114
SQL_ATTR_ENLIST_IN_DTC	equ 1207
SQL_ATTR_ENLIST_IN_XA	equ 1208
SQL_ATTR_LOGIN_TIMEOUT	equ SQL_LOGIN_TIMEOUT
SQL_ATTR_ODBC_CURSORS	equ SQL_ODBC_CURSORS
SQL_ATTR_PACKET_SIZE	equ SQL_PACKET_SIZE
SQL_ATTR_QUIET_MODE	equ SQL_QUIET_MODE
SQL_ATTR_TRACE	equ SQL_OPT_TRACE
SQL_ATTR_TRACEFILE	equ SQL_OPT_TRACEFILE
SQL_ATTR_TRANSLATE_LIB	equ SQL_TRANSLATE_DLL
SQL_ATTR_TRANSLATE_OPTION	equ SQL_TRANSLATE_OPTION
SQL_ATTR_TXN_ISOLATION	equ SQL_TXN_ISOLATION
SQL_ATTR_CONNECTION_DEAD	equ 1209
SQL_ATTR_ANSI_APP	equ 115
SQL_MODE_READ_WRITE equ 0
SQL_MODE_READ_ONLY equ 1
SQL_MODE_DEFAULT equ SQL_MODE_READ_WRITE
SQL_AUTOCOMMIT_OFF equ 0
SQL_AUTOCOMMIT_ON equ 1
SQL_AUTOCOMMIT_DEFAULT equ SQL_AUTOCOMMIT_ON
SQL_LOGIN_TIMEOUT_DEFAULT equ 15
SQL_OPT_TRACE_OFF equ 0
SQL_OPT_TRACE_ON equ 1
SQL_OPT_TRACE_DEFAULT equ SQL_OPT_TRACE_OFF
SQL_OPT_TRACE_FILE_DEFAULT equ "\\SQL.LOG"
SQL_CUR_USE_IF_NEEDED equ 0
SQL_CUR_USE_ODBC equ 1
SQL_CUR_USE_DRIVER equ 2
SQL_CUR_DEFAULT equ SQL_CUR_USE_DRIVER
SQL_DB_RETURN_TO_POOL	equ 0
SQL_DB_DISCONNECT	equ 1
SQL_DB_DEFAULT	equ SQL_DB_RETURN_TO_POOL
SQL_DTC_DONE	equ 0
SQL_CD_TRUE	equ 1
SQL_CD_FALSE	equ 0
SQL_AA_TRUE	equ 1
SQL_AA_FALSE	equ 0
SQL_QUERY_TIMEOUT	equ 0
SQL_MAX_ROWS	equ 1
SQL_NOSCAN	equ 2
SQL_MAX_LENGTH	equ 3
SQL_ASYNC_ENABLE	equ 4
SQL_BIND_TYPE	equ 5
SQL_CURSOR_TYPE	equ 6
SQL_CONCURRENCY	equ 7
SQL_KEYSET_SIZE	equ 8
SQL_ROWSET_SIZE	equ 9
SQL_SIMULATE_CURSOR	equ 10
SQL_RETRIEVE_DATA	equ 11
SQL_USE_BOOKMARKS	equ 12
SQL_GET_BOOKMARK	equ 13
SQL_ROW_NUMBER	equ 14
SQL_ATTR_ASYNC_ENABLE	equ 4
SQL_ATTR_CONCURRENCY	equ SQL_CONCURRENCY
SQL_ATTR_CURSOR_TYPE	equ SQL_CURSOR_TYPE
SQL_ATTR_ENABLE_AUTO_IPD	equ 15
SQL_ATTR_FETCH_BOOKMARK_PTR	equ 16
SQL_ATTR_KEYSET_SIZE	equ SQL_KEYSET_SIZE
SQL_ATTR_MAX_LENGTH	equ SQL_MAX_LENGTH
SQL_ATTR_MAX_ROWS	equ SQL_MAX_ROWS
SQL_ATTR_NOSCAN	equ SQL_NOSCAN
SQL_ATTR_PARAM_BIND_OFFSET_PTR	equ 17
SQL_ATTR_PARAM_BIND_TYPE	equ 18
SQL_ATTR_PARAM_OPERATION_PTR	equ 19
SQL_ATTR_PARAM_STATUS_PTR	equ 20
SQL_ATTR_PARAMS_PROCESSED_PTR	equ 21
SQL_ATTR_PARAMSET_SIZE	equ 22
SQL_ATTR_QUERY_TIMEOUT	equ SQL_QUERY_TIMEOUT
SQL_ATTR_RETRIEVE_DATA	equ SQL_RETRIEVE_DATA
SQL_ATTR_ROW_BIND_OFFSET_PTR	equ 23
SQL_ATTR_ROW_BIND_TYPE	equ SQL_BIND_TYPE
SQL_ATTR_ROW_NUMBER	equ SQL_ROW_NUMBER
SQL_ATTR_ROW_OPERATION_PTR	equ 24
SQL_ATTR_ROW_STATUS_PTR	equ 25
SQL_ATTR_ROWS_FETCHED_PTR	equ 26
SQL_ATTR_ROW_ARRAY_SIZE	equ 27	
SQL_ATTR_SIMULATE_CURSOR	equ SQL_SIMULATE_CURSOR
SQL_ATTR_USE_BOOKMARKS	equ SQL_USE_BOOKMARKS	
SQL_COL_PRED_CHAR	equ SQL_LIKE_ONLY
SQL_COL_PRED_BASIC	equ SQL_ALL_EXCEPT_LIKE
SQL_IS_POINTER	equ (-4)
SQL_IS_UINTEGER	equ (-5)
SQL_IS_INTEGER	equ (-6)
SQL_IS_USMALLINT	equ (-7)
SQL_IS_SMALLINT	equ (-8)
SQL_PARAM_BIND_BY_COLUMN	equ 0
SQL_PARAM_BIND_TYPE_DEFAULT	equ SQL_PARAM_BIND_BY_COLUMN
SQL_QUERY_TIMEOUT_DEFAULT equ 0
SQL_MAX_ROWS_DEFAULT equ 0
SQL_NOSCAN_OFF equ 0
SQL_NOSCAN_ON equ 1
SQL_NOSCAN_DEFAULT equ SQL_NOSCAN_OFF
SQL_MAX_LENGTH_DEFAULT equ 0
SQL_ASYNC_ENABLE_OFF	equ 0
SQL_ASYNC_ENABLE_ON	equ 1
SQL_ASYNC_ENABLE_DEFAULT equ SQL_ASYNC_ENABLE_OFF
SQL_BIND_BY_COLUMN equ 0
SQL_BIND_TYPE_DEFAULT equ SQL_BIND_BY_COLUMN
SQL_CONCUR_READ_ONLY equ 1
SQL_CONCUR_LOCK equ 2
SQL_CONCUR_ROWVER equ 3
SQL_CONCUR_VALUES equ 4
SQL_CONCUR_DEFAULT equ SQL_CONCUR_READ_ONLY
SQL_CURSOR_FORWARD_ONLY equ 0
SQL_CURSOR_KEYSET_DRIVEN equ 1
SQL_CURSOR_DYNAMIC equ 2
SQL_CURSOR_STATIC equ 3
SQL_CURSOR_TYPE_DEFAULT equ SQL_CURSOR_FORWARD_ONLY
SQL_ROWSET_SIZE_DEFAULT equ 1
SQL_KEYSET_SIZE_DEFAULT equ 0
SQL_SC_NON_UNIQUE equ 0
SQL_SC_TRY_UNIQUE equ 1
SQL_SC_UNIQUE equ 2
SQL_RD_OFF equ 0
SQL_RD_ON equ 1
SQL_RD_DEFAULT equ SQL_RD_ON
SQL_UB_OFF equ 0
SQL_UB_ON	equ 01
SQL_UB_DEFAULT equ SQL_UB_OFF
SQL_UB_FIXED	equ SQL_UB_ON
SQL_UB_VARIABLE	equ 2
SQL_DESC_ARRAY_SIZE	equ 20
SQL_DESC_ARRAY_STATUS_PTR	equ 21
SQL_DESC_AUTO_UNIQUE_VALUE	equ SQL_COLUMN_AUTO_INCREMENT
SQL_DESC_BASE_COLUMN_NAME	equ 22
SQL_DESC_BASE_TABLE_NAME	equ 23
SQL_DESC_BIND_OFFSET_PTR	equ 24
SQL_DESC_BIND_TYPE	equ 25
SQL_DESC_CASE_SENSITIVE	equ SQL_COLUMN_CASE_SENSITIVE
SQL_DESC_CATALOG_NAME	equ SQL_COLUMN_QUALIFIER_NAME
SQL_DESC_CONCISE_TYPE	equ SQL_COLUMN_TYPE
SQL_DESC_DATETIME_INTERVAL_PRECISION	equ 26
SQL_DESC_DISPLAY_SIZE	equ SQL_COLUMN_DISPLAY_SIZE
SQL_DESC_FIXED_PREC_SCALE	equ SQL_COLUMN_MONEY
SQL_DESC_LABEL	equ SQL_COLUMN_LABEL
SQL_DESC_LITERAL_PREFIX	equ 27
SQL_DESC_LITERAL_SUFFIX	equ 28
SQL_DESC_LOCAL_TYPE_NAME	equ 29
SQL_DESC_MAXIMUM_SCALE	equ 30
SQL_DESC_MINIMUM_SCALE	equ 31
SQL_DESC_NUM_PREC_RADIX	equ 32
SQL_DESC_PARAMETER_TYPE	equ 33
SQL_DESC_ROWS_PROCESSED_PTR	equ 34
SQL_DESC_ROWVER	equ 35
SQL_DESC_SCHEMA_NAME	equ SQL_COLUMN_OWNER_NAME
SQL_DESC_SEARCHABLE	equ SQL_COLUMN_SEARCHABLE
SQL_DESC_TYPE_NAME	equ SQL_COLUMN_TYPE_NAME
SQL_DESC_TABLE_NAME	equ SQL_COLUMN_TABLE_NAME
SQL_DESC_UNSIGNED	equ SQL_COLUMN_UNSIGNED
SQL_DESC_UPDATABLE	equ SQL_COLUMN_UPDATABLE
SQL_DIAG_CURSOR_ROW_COUNT	equ (-1249)
SQL_DIAG_ROW_NUMBER	equ (-1248)
SQL_DIAG_COLUMN_NUMBER	equ (-1247)	
SQL_DATE equ 9
SQL_INTERVAL	equ 10
SQL_TIME equ 10
SQL_TIMESTAMP equ 11
SQL_LONGVARCHAR equ (-1)
SQL_BINARY equ (-2)
SQL_VARBINARY equ (-3)
SQL_LONGVARBINARY equ (-4)
SQL_BIGINT equ (-5)
SQL_TINYINT equ (-6)
SQL_BIT equ (-7)
SQL_GUID	equ (-11)
SQL_CODE_YEAR	equ 1
SQL_CODE_MONTH	equ 2
SQL_CODE_DAY	equ 3
SQL_CODE_HOUR	equ 4
SQL_CODE_MINUTE	equ 5
SQL_CODE_SECOND	equ 6
SQL_CODE_YEAR_TO_MONTH	equ 7
SQL_CODE_DAY_TO_HOUR	equ 8
SQL_CODE_DAY_TO_MINUTE	equ 9
SQL_CODE_DAY_TO_SECOND	equ 10
SQL_CODE_HOUR_TO_MINUTE	equ 11
SQL_CODE_HOUR_TO_SECOND	equ 12
SQL_CODE_MINUTE_TO_SECOND	equ 13
SQL_INTERVAL_YEAR	equ (100 + SQL_CODE_YEAR)
SQL_INTERVAL_MONTH	equ (100 + SQL_CODE_MONTH)
SQL_INTERVAL_DAY	equ (100 + SQL_CODE_DAY) 
SQL_INTERVAL_HOUR	equ (100 + SQL_CODE_HOUR) 
SQL_INTERVAL_MINUTE	equ (100 + SQL_CODE_MINUTE) 
SQL_INTERVAL_SECOND equ (100 + SQL_CODE_SECOND) 
SQL_INTERVAL_YEAR_TO_MONTH	equ (100 + SQL_CODE_YEAR_TO_MONTH)
SQL_INTERVAL_DAY_TO_HOUR	equ (100 + SQL_CODE_DAY_TO_HOUR) 
SQL_INTERVAL_DAY_TO_MINUTE	equ (100 + SQL_CODE_DAY_TO_MINUTE) 
SQL_INTERVAL_DAY_TO_SECOND	equ (100 + SQL_CODE_DAY_TO_SECOND) 
SQL_INTERVAL_HOUR_TO_MINUTE	equ (100 + SQL_CODE_HOUR_TO_MINUTE)
SQL_INTERVAL_HOUR_TO_SECOND	equ (100 + SQL_CODE_HOUR_TO_SECOND) 
SQL_INTERVAL_MINUTE_TO_SECOND	equ (100 + SQL_CODE_MINUTE_TO_SECOND) 
SQL_WCHAR       equ   (-8)
SQL_UNICODE	equ SQL_WCHAR
SQL_WVARCHAR    equ   (-9) 
SQL_UNICODE_VARCHAR	equ SQL_WVARCHAR
SQL_WLONGVARCHAR equ   (-10)
SQL_UNICODE_LONGVARCHAR	equ SQL_WLONGVARCHAR
SQL_UNICODE_CHAR	equ SQL_WCHAR
SQL_C_CHAR equ SQL_CHAR
SQL_C_LONG equ SQL_INTEGER
SQL_C_SHORT equ SQL_SMALLINT
SQL_C_FLOAT equ SQL_REAL
SQL_C_DOUBLE equ SQL_DOUBLE
SQL_C_NUMERIC	equ SQL_NUMERIC
SQL_C_DEFAULT equ 99
SQL_SIGNED_OFFSET equ (-20)
SQL_UNSIGNED_OFFSET equ (-22)
SQL_C_DATE equ SQL_DATE
SQL_C_TIME equ SQL_TIME
SQL_C_TIMESTAMP equ SQL_TIMESTAMP
SQL_C_TYPE_DATE	equ SQL_TYPE_DATE
SQL_C_TYPE_TIME	equ SQL_TYPE_TIME
SQL_C_TYPE_TIMESTAMP	equ SQL_TYPE_TIMESTAMP
SQL_C_INTERVAL_YEAR	equ SQL_INTERVAL_YEAR
SQL_C_INTERVAL_MONTH	equ SQL_INTERVAL_MONTH
SQL_C_INTERVAL_DAY	equ SQL_INTERVAL_DAY
SQL_C_INTERVAL_HOUR	equ SQL_INTERVAL_HOUR
SQL_C_INTERVAL_MINUTE	equ SQL_INTERVAL_MINUTE
SQL_C_INTERVAL_SECOND	equ SQL_INTERVAL_SECOND
SQL_C_INTERVAL_YEAR_TO_MONTH	equ SQL_INTERVAL_YEAR_TO_MONTH
SQL_C_INTERVAL_DAY_TO_HOUR	equ SQL_INTERVAL_DAY_TO_HOUR
SQL_C_INTERVAL_DAY_TO_MINUTE	equ SQL_INTERVAL_DAY_TO_MINUTE
SQL_C_INTERVAL_DAY_TO_SECOND	equ SQL_INTERVAL_DAY_TO_SECOND
SQL_C_INTERVAL_HOUR_TO_MINUTE	equ SQL_INTERVAL_HOUR_TO_MINUTE
SQL_C_INTERVAL_HOUR_TO_SECOND	equ SQL_INTERVAL_HOUR_TO_SECOND
SQL_C_INTERVAL_MINUTE_TO_SECOND	equ SQL_INTERVAL_MINUTE_TO_SECOND
SQL_C_BINARY equ SQL_BINARY
SQL_C_BIT equ SQL_BIT
SQL_C_SBIGINT	equ (SQL_BIGINT+SQL_SIGNED_OFFSET)
SQL_C_UBIGINT	equ (SQL_BIGINT+SQL_UNSIGNED_OFFSET)
SQL_C_TINYINT equ SQL_TINYINT
SQL_C_SLONG equ (SQL_C_LONG+SQL_SIGNED_OFFSET)
SQL_C_SSHORT equ (SQL_C_SHORT+SQL_SIGNED_OFFSET)
SQL_C_STINYINT equ (SQL_TINYINT+SQL_SIGNED_OFFSET)
SQL_C_ULONG equ (SQL_C_LONG+SQL_UNSIGNED_OFFSET)
SQL_C_USHORT equ (SQL_C_SHORT+SQL_UNSIGNED_OFFSET)
SQL_C_UTINYINT equ (SQL_TINYINT+SQL_UNSIGNED_OFFSET)
SQL_C_BOOKMARK equ SQL_C_ULONG
SQL_C_GUID	equ SQL_GUID
SQL_TYPE_NULL equ 0
SQL_C_VARBOOKMARK	equ SQL_C_BINARY
SQL_NO_ROW_NUMBER	equ (-1)
SQL_NO_COLUMN_NUMBER	equ (-1)
SQL_ROW_NUMBER_UNKNOWN	equ (-2)
SQL_COLUMN_NUMBER_UNKNOWN	equ (-2)
SQL_DEFAULT_PARAM equ (-5)
SQL_IGNORE equ (-6)
SQL_COLUMN_IGNORE	equ SQL_IGNORE
SQL_LEN_DATA_AT_EXEC_OFFSET equ (-100)
SQL_LEN_BINARY_ATTR_OFFSET	equ (-100)
SQL_PARAM_TYPE_DEFAULT equ SQL_PARAM_INPUT_OUTPUT
SQL_SETPARAM_VALUE_MAX equ (-1)
SQL_COLUMN_COUNT equ 0
SQL_COLUMN_NAME equ 1
SQL_COLUMN_TYPE equ 2
SQL_COLUMN_LENGTH equ 3
SQL_COLUMN_PRECISION equ 4
SQL_COLUMN_SCALE equ 5
SQL_COLUMN_DISPLAY_SIZE equ 6
SQL_COLUMN_NULLABLE equ 7
SQL_COLUMN_UNSIGNED equ 8
SQL_COLUMN_MONEY equ 9
SQL_COLUMN_UPDATABLE equ 10
SQL_COLUMN_AUTO_INCREMENT equ 11
SQL_COLUMN_CASE_SENSITIVE equ 12
SQL_COLUMN_SEARCHABLE equ 13
SQL_COLUMN_TYPE_NAME equ 14
SQL_COLUMN_TABLE_NAME equ 15
SQL_COLUMN_OWNER_NAME equ 16
SQL_COLUMN_QUALIFIER_NAME equ 17
SQL_COLUMN_LABEL equ 18
SQL_COLATT_OPT_MAX equ SQL_COLUMN_LABEL
SQL_COLATT_OPT_MIN equ SQL_COLUMN_COUNT
SQL_ATTR_READONLY equ 0
SQL_ATTR_WRITE equ 1
SQL_ATTR_READWRITE_UNKNOWN equ 2
SQL_UNSEARCHABLE equ 0
SQL_LIKE_ONLY equ 1
SQL_ALL_EXCEPT_LIKE equ 2
SQL_SEARCHABLE equ 3
SQL_PRED_SEARCHABLE	equ SQL_SEARCHABLE
SQL_NO_TOTAL equ (-4)
SQL_API_SQLALLOCHANDLESTD	equ 73
SQL_API_SQLBULKOPERATIONS	equ 24
SQL_API_SQLBINDPARAMETER equ 72
SQL_API_SQLBROWSECONNECT equ 55    
SQL_API_SQLCOLATTRIBUTES equ 6 
SQL_API_SQLCOLUMNPRIVILEGES equ 56
SQL_API_SQLDESCRIBEPARAM equ 58
SQL_API_SQLDRIVERCONNECT	equ 41 
SQL_API_SQLDRIVERS equ 71
SQL_API_SQLEXTENDEDFETCH equ 59
SQL_API_SQLFOREIGNKEYS equ 60
SQL_API_SQLMORERESULTS equ 61
SQL_API_SQLNATIVESQL equ 62
SQL_API_SQLNUMPARAMS equ 63
SQL_API_SQLPARAMOPTIONS equ 64
SQL_API_SQLPRIMARYKEYS equ 65
SQL_API_SQLPROCEDURECOLUMNS equ 66
SQL_API_SQLPROCEDURES equ 67
SQL_API_SQLSETPOS equ 68
SQL_API_SQLSETSCROLLOPTIONS equ 69
SQL_API_SQLTABLEPRIVILEGES equ 70
SQL_API_ALL_FUNCTIONS equ 0
SQL_API_LOADBYORDINAL equ 199
SQL_API_ODBC3_ALL_FUNCTIONS	equ 999
SQL_API_ODBC3_ALL_FUNCTIONS_SIZE	equ 250
SQL_INFO_FIRST equ 0		
SQL_ACTIVE_CONNECTIONS equ 0
SQL_ACTIVE_STATEMENTS equ 1
SQL_DRIVER_HDBC equ 3
SQL_DRIVER_HENV equ 4
SQL_DRIVER_HSTMT equ 5
SQL_DRIVER_NAME equ 6
SQL_DRIVER_VER equ 7
SQL_ODBC_API_CONFORMANCE equ 9
SQL_ODBC_VER equ 10
SQL_ROW_UPDATES equ 11
SQL_ODBC_SAG_CLI_CONFORMANCE equ 12
SQL_ODBC_SQL_CONFORMANCE equ 15
SQL_PROCEDURES equ 21
SQL_CONCAT_NULL_BEHAVIOR equ 22
SQL_CURSOR_ROLLBACK_BEHAVIOR equ 24
SQL_EXPRESSIONS_IN_ORDERBY equ 27
SQL_MAX_OWNER_NAME_LEN equ 32
SQL_MAX_PROCEDURE_NAME_LEN equ 33
SQL_MAX_QUALIFIER_NAME_LEN equ 34
SQL_MULT_RESULT_SETS equ 36
SQL_MULTIPLE_ACTIVE_TXN equ 37
SQL_OUTER_JOINS equ 38
SQL_OWNER_TERM equ 39
SQL_PROCEDURE_TERM equ 40
SQL_QUALIFIER_NAME_SEPARATOR equ 41
SQL_QUALIFIER_TERM equ 42
SQL_SCROLL_OPTIONS equ 44
SQL_TABLE_TERM equ 45
SQL_CONVERT_FUNCTIONS equ 48
SQL_NUMERIC_FUNCTIONS equ 49
SQL_STRING_FUNCTIONS equ 50
SQL_SYSTEM_FUNCTIONS equ 51
SQL_TIMEDATE_FUNCTIONS equ 52
SQL_CONVERT_BIGINT equ 53
SQL_CONVERT_BINARY equ 54
SQL_CONVERT_BIT equ 55
SQL_CONVERT_CHAR equ 56
SQL_CONVERT_DATE equ 57
SQL_CONVERT_DECIMAL equ 58
SQL_CONVERT_DOUBLE equ 59
SQL_CONVERT_FLOAT equ 60
SQL_CONVERT_INTEGER equ 61
SQL_CONVERT_LONGVARCHAR equ 62
SQL_CONVERT_NUMERIC equ 63
SQL_CONVERT_REAL equ 64
SQL_CONVERT_SMALLINT equ 65
SQL_CONVERT_TIME equ 66
SQL_CONVERT_TIMESTAMP equ 67
SQL_CONVERT_TINYINT equ 68
SQL_CONVERT_VARBINARY equ 69
SQL_CONVERT_VARCHAR equ 70
SQL_CONVERT_LONGVARBINARY equ 71
SQL_ODBC_SQL_OPT_IEF equ 73
SQL_CORRELATION_NAME equ 74
SQL_NON_NULLABLE_COLUMNS equ 75
SQL_DRIVER_HLIB equ 76
SQL_DRIVER_ODBC_VER equ 77
SQL_LOCK_TYPES equ 78
SQL_POS_OPERATIONS equ 79
SQL_POSITIONED_STATEMENTS equ 80
SQL_BOOKMARK_PERSISTENCE equ 82
SQL_STATIC_SENSITIVITY equ 83
SQL_FILE_USAGE equ 84
SQL_COLUMN_ALIAS equ 87
SQL_GROUP_BY equ 88
SQL_KEYWORDS equ 89
SQL_OWNER_USAGE equ 91
SQL_QUALIFIER_USAGE equ 92
SQL_QUOTED_IDENTIFIER_CASE equ 93
SQL_SUBQUERIES equ 95
SQL_UNION equ 96
SQL_MAX_ROW_SIZE_INCLUDES_LONG equ 103
SQL_MAX_CHAR_LITERAL_LEN equ 108
SQL_TIMEDATE_ADD_INTERVALS equ 109
SQL_TIMEDATE_DIFF_INTERVALS equ 110
SQL_NEED_LONG_DATA_LEN equ 111
SQL_MAX_BINARY_LITERAL_LEN equ 112
SQL_LIKE_ESCAPE_CLAUSE equ 113
SQL_QUALIFIER_LOCATION equ 114
SQL_ACTIVE_ENVIRONMENTS	equ 116
SQL_ALTER_DOMAIN	equ 117
SQL_SQL_CONFORMANCE	equ 118
SQL_DATETIME_LITERALS	equ 119
SQL_ASYNC_MODE	equ 10021
SQL_BATCH_ROW_COUNT	equ 120
SQL_BATCH_SUPPORT	equ 121
SQL_CATALOG_LOCATION	equ SQL_QUALIFIER_LOCATION
SQL_CATALOG_NAME_SEPARATOR	equ SQL_QUALIFIER_NAME_SEPARATOR
SQL_CATALOG_TERM	equ SQL_QUALIFIER_TERM
SQL_CATALOG_USAGE	equ SQL_QUALIFIER_USAGE
SQL_CONVERT_WCHAR	equ 122
SQL_CONVERT_INTERVAL_DAY_TIME	equ 123
SQL_CONVERT_INTERVAL_YEAR_MONTH	equ 124
SQL_CONVERT_WLONGVARCHAR	equ 125
SQL_CONVERT_WVARCHAR	equ 126
SQL_CREATE_ASSERTION	equ 127
SQL_CREATE_CHARACTER_SET	equ 128
SQL_CREATE_COLLATION	equ 129
SQL_CREATE_DOMAIN	equ 130
SQL_CREATE_SCHEMA	equ 131
SQL_CREATE_TABLE	equ 132
SQL_CREATE_TRANSLATION	equ 133
SQL_CREATE_VIEW	equ 134
SQL_DRIVER_HDESC	equ 135
SQL_DROP_ASSERTION	equ 136
SQL_DROP_CHARACTER_SET	equ 137
SQL_DROP_COLLATION	equ 138
SQL_DROP_DOMAIN	equ 139
SQL_DROP_SCHEMA	equ 140
SQL_DROP_TABLE	equ 141
SQL_DROP_TRANSLATION	equ 142
SQL_DROP_VIEW	equ 143
SQL_DYNAMIC_CURSOR_ATTRIBUTES1	equ 144
SQL_DYNAMIC_CURSOR_ATTRIBUTES2	equ 145
SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES1	equ 146		
SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES2	equ 147
SQL_INDEX_KEYWORDS	equ 148
SQL_INFO_SCHEMA_VIEWS	equ 149
SQL_KEYSET_CURSOR_ATTRIBUTES1	equ 150
SQL_KEYSET_CURSOR_ATTRIBUTES2	equ 151
SQL_MAX_ASYNC_CONCURRENT_STATEMENTS	equ 10022
SQL_ODBC_INTERFACE_CONFORMANCE	equ 152
SQL_PARAM_ARRAY_ROW_COUNTS equ 153
SQL_PARAM_ARRAY_SELECTS equ 154
SQL_SCHEMA_TERM	equ SQL_OWNER_TERM
SQL_SCHEMA_USAGE	equ SQL_OWNER_USAGE
SQL_SQL92_DATETIME_FUNCTIONS	equ 155
SQL_SQL92_FOREIGN_KEY_DELETE_RULE	equ 156		
SQL_SQL92_FOREIGN_KEY_UPDATE_RULE	equ 157		
SQL_SQL92_GRANT	equ 158
SQL_SQL92_NUMERIC_VALUE_FUNCTIONS	equ 159
SQL_SQL92_PREDICATES	equ 160
SQL_SQL92_RELATIONAL_JOIN_OPERATORS	equ 161
SQL_SQL92_REVOKE	equ 162
SQL_SQL92_ROW_VALUE_CONSTRUCTOR	equ 163
SQL_SQL92_STRING_FUNCTIONS	equ 164
SQL_SQL92_VALUE_EXPRESSIONS	equ 165
SQL_STANDARD_CLI_CONFORMANCE	equ 166
SQL_STATIC_CURSOR_ATTRIBUTES1	equ 167	
SQL_STATIC_CURSOR_ATTRIBUTES2	equ 168
SQL_AGGREGATE_FUNCTIONS	equ 169
SQL_DDL_INDEX	equ 170
SQL_DM_VER	equ 171
SQL_INSERT_STATEMENT	equ 172
SQL_UNION_STATEMENT	equ SQL_UNION
SQL_DTC_TRANSITION_COST	equ 1750
SQL_AT_ADD_COLUMN_SINGLE	equ 20h	
SQL_CVT_CHAR equ 1
SQL_CVT_NUMERIC equ 2
SQL_CVT_DECIMAL equ 4
SQL_CVT_INTEGER equ 8
SQL_CVT_SMALLINT equ 10h
SQL_CVT_FLOAT equ 20h
SQL_CVT_REAL equ 40h
SQL_CVT_DOUBLE equ 80h
SQL_CVT_VARCHAR equ 100h
SQL_CVT_LONGVARCHAR equ 200h
SQL_CVT_BINARY equ 400h
SQL_CVT_VARBINARY equ 800h
SQL_CVT_BIT equ 1000h
SQL_CVT_TINYINT equ 2000h
SQL_CVT_BIGINT equ 4000h
SQL_CVT_DATE equ 8000h
SQL_CVT_TIME equ 10000h
SQL_CVT_TIMESTAMP equ 20000h
SQL_CVT_LONGVARBINARY equ 40000h
SQL_CVT_INTERVAL_YEAR_MONTH	equ 80000h
SQL_CVT_INTERVAL_DAY_TIME	equ 100000h
SQL_CVT_WCHAR	equ 200000h
SQL_CVT_WLONGVARCHAR	equ 400000h
SQL_CVT_WVARCHAR	equ 800000h
SQL_FN_CVT_CONVERT equ 1
SQL_FN_CVT_CAST	equ 2
SQL_FN_STR_CONCAT equ 1
SQL_FN_STR_INSERT equ 2
SQL_FN_STR_LEFT equ 4
SQL_FN_STR_LTRIM equ 8
SQL_FN_STR_LENGTH equ 10h
SQL_FN_STR_LOCATE equ 20h
SQL_FN_STR_LCASE equ 40h
SQL_FN_STR_REPEAT equ 80h
SQL_FN_STR_REPLACE equ 100h
SQL_FN_STR_RIGHT equ 200h
SQL_FN_STR_RTRIM equ 400h
SQL_FN_STR_SUBSTRING equ 800h
SQL_FN_STR_UCASE equ 1000h
SQL_FN_STR_ASCII equ 2000h
SQL_FN_STR_CHAR equ 4000h
SQL_FN_STR_DIFFERENCE equ 8000h
SQL_FN_STR_LOCATE_2 equ 10000h
SQL_FN_STR_SOUNDEX equ 20000h
SQL_FN_STR_SPACE equ 40000h
SQL_FN_STR_BIT_LENGTH	equ 80000h
SQL_FN_STR_CHAR_LENGTH	equ 100000h
SQL_FN_STR_CHARACTER_LENGTH	equ 200000h
SQL_FN_STR_OCTET_LENGTH	equ 400000h
SQL_FN_STR_POSITION	equ 800000h
SQL_SSF_CONVERT	equ 1	
SQL_SSF_LOWER	equ 2
SQL_SSF_UPPER	equ 4
SQL_SSF_SUBSTRING	equ 8
SQL_SSF_TRANSLATE	equ 10h
SQL_SSF_TRIM_BOTH	equ 20h
SQL_SSF_TRIM_LEADING	equ 40h
SQL_SSF_TRIM_TRAILING	equ 80h
SQL_FN_NUM_ABS equ 1
SQL_FN_NUM_ACOS equ 2
SQL_FN_NUM_ASIN equ 4
SQL_FN_NUM_ATAN equ 8
SQL_FN_NUM_ATAN2 equ 10h
SQL_FN_NUM_CEILING equ 20h
SQL_FN_NUM_COS equ 40h
SQL_FN_NUM_COT equ 80h
SQL_FN_NUM_EXP equ 100h
SQL_FN_NUM_FLOOR equ 200h
SQL_FN_NUM_LOG equ 400h
SQL_FN_NUM_MOD equ 800h
SQL_FN_NUM_SIGN equ 1000h
SQL_FN_NUM_SIN equ 2000h
SQL_FN_NUM_SQRT equ 4000h
SQL_FN_NUM_TAN equ 8000h
SQL_FN_NUM_PI equ 10000h
SQL_FN_NUM_RAND equ 20000h
SQL_FN_NUM_DEGREES equ 40000h
SQL_FN_NUM_LOG10 equ 80000h
SQL_FN_NUM_POWER equ 100000h
SQL_FN_NUM_RADIANS equ 200000h
SQL_FN_NUM_ROUND equ 400000h
SQL_FN_NUM_TRUNCATE equ 800000h
SQL_SNVF_BIT_LENGTH	equ 1
SQL_SNVF_CHAR_LENGTH	equ 2
SQL_SNVF_CHARACTER_LENGTH	equ 4
SQL_SNVF_EXTRACT	equ 8
SQL_SNVF_OCTET_LENGTH	equ 10h
SQL_SNVF_POSITION	equ 20h
SQL_FN_TD_NOW equ 1
SQL_FN_TD_CURDATE equ 2
SQL_FN_TD_DAYOFMONTH equ 4
SQL_FN_TD_DAYOFWEEK equ 8
SQL_FN_TD_DAYOFYEAR equ 10h
SQL_FN_TD_MONTH equ 20h
SQL_FN_TD_QUARTER equ 40h
SQL_FN_TD_WEEK equ 80h
SQL_FN_TD_YEAR equ 100h
SQL_FN_TD_CURTIME equ 200h
SQL_FN_TD_HOUR equ 400h
SQL_FN_TD_MINUTE equ 800h
SQL_FN_TD_SECOND equ 1000h
SQL_FN_TD_TIMESTAMPADD equ 2000h
SQL_FN_TD_TIMESTAMPDIFF equ 4000h
SQL_FN_TD_DAYNAME equ 8000h
SQL_FN_TD_MONTHNAME equ 10000h
SQL_FN_TD_CURRENT_DATE	equ 20000h
SQL_FN_TD_CURRENT_TIME	equ 40000h
SQL_FN_TD_CURRENT_TIMESTAMP	equ 80000h
SQL_FN_TD_EXTRACT	equ 100000h
SQL_SDF_CURRENT_DATE	equ 1
SQL_SDF_CURRENT_TIME	equ 2
SQL_SDF_CURRENT_TIMESTAMP	equ 4
SQL_FN_SYS_USERNAME equ 1
SQL_FN_SYS_DBNAME equ 2
SQL_FN_SYS_IFNULL equ 4
SQL_FN_TSI_FRAC_SECOND equ 1
SQL_FN_TSI_SECOND equ 2
SQL_FN_TSI_MINUTE equ 4
SQL_FN_TSI_HOUR equ 8
SQL_FN_TSI_DAY equ 10h
SQL_FN_TSI_WEEK equ 20h
SQL_FN_TSI_MONTH equ 40h
SQL_FN_TSI_QUARTER equ 80h
SQL_FN_TSI_YEAR equ 100h
SQL_CA1_NEXT	equ 1
SQL_CA1_ABSOLUTE	equ 2
SQL_CA1_RELATIVE	equ 4
SQL_CA1_BOOKMARK	equ 8
SQL_CA1_LOCK_NO_CHANGE	equ 40h
SQL_CA1_LOCK_EXCLUSIVE	equ 80h
SQL_CA1_LOCK_UNLOCK	equ 100h
SQL_CA1_POS_POSITION	equ 200h
SQL_CA1_POS_UPDATE	equ 400h
SQL_CA1_POS_DELETE	equ 800h
SQL_CA1_POS_REFRESH	equ 1000h
SQL_CA1_POSITIONED_UPDATE	equ 2000h
SQL_CA1_POSITIONED_DELETE	equ 4000h
SQL_CA1_SELECT_FOR_UPDATE	equ 8000h
SQL_CA1_BULK_ADD	equ 10000h
SQL_CA1_BULK_UPDATE_BY_BOOKMARK	equ 20000h
SQL_CA1_BULK_DELETE_BY_BOOKMARK	equ 40000h
SQL_CA1_BULK_FETCH_BY_BOOKMARK	equ 80000h
SQL_CA2_READ_ONLY_CONCURRENCY	equ 1
SQL_CA2_LOCK_CONCURRENCY	equ 2
SQL_CA2_OPT_ROWVER_CONCURRENCY	equ 4
SQL_CA2_OPT_VALUES_CONCURRENCY	equ 8
SQL_CA2_SENSITIVITY_ADDITIONS	equ 10h
SQL_CA2_SENSITIVITY_DELETIONS	equ 20h
SQL_CA2_SENSITIVITY_UPDATES	equ 40h
SQL_CA2_MAX_ROWS_SELECT	equ 80h
SQL_CA2_MAX_ROWS_INSERT	equ 100h
SQL_CA2_MAX_ROWS_DELETE	equ 200h
SQL_CA2_MAX_ROWS_UPDATE	equ 400h
SQL_CA2_MAX_ROWS_CATALOG	equ 800h
SQL_CA2_MAX_ROWS_AFFECTS_ALL	equ (SQL_CA2_MAX_ROWS_SELECT+SQL_CA2_MAX_ROWS_INSERT+SQL_CA2_MAX_ROWS_DELETE+SQL_CA2_MAX_ROWS_UPDATE+SQL_CA2_MAX_ROWS_CATALOG)
SQL_CA2_CRC_EXACT	equ 1000h
SQL_CA2_CRC_APPROXIMATE	equ 2000h
SQL_CA2_SIMULATE_NON_UNIQUE	equ 4000h
SQL_CA2_SIMULATE_TRY_UNIQUE	equ 8000h
SQL_CA2_SIMULATE_UNIQUE	equ 10000h
SQL_OAC_NONE equ 0
SQL_OAC_LEVEL1 equ 1
SQL_OAC_LEVEL2 equ 2
SQL_OSCC_NOT_COMPLIANT equ 0
SQL_OSCC_COMPLIANT equ 1
SQL_OSC_MINIMUM equ 0
SQL_OSC_CORE equ 1
SQL_OSC_EXTENDED equ 2
SQL_CB_NULL equ 0
SQL_CB_NON_NULL equ 1
SQL_SO_FORWARD_ONLY equ 1
SQL_SO_KEYSET_DRIVEN equ 2
SQL_SO_DYNAMIC equ 4
SQL_SO_MIXED equ 8
SQL_SO_STATIC equ 10h
SQL_FD_FETCH_BOOKMARK equ 80h
SQL_CN_NONE equ 0
SQL_CN_DIFFERENT equ 1
SQL_CN_ANY equ 2
SQL_NNC_NULL equ 0
SQL_NNC_NON_NULL equ 1
SQL_NC_START equ 2
SQL_NC_END equ 4
SQL_FILE_NOT_SUPPORTED equ 0
SQL_FILE_TABLE equ 1
SQL_FILE_QUALIFIER equ 2
SQL_FILE_CATALOG	equ SQL_FILE_QUALIFIER
SQL_GD_BLOCK equ 4
SQL_GD_BOUND equ 8
SQL_PS_POSITIONED_DELETE equ 1
SQL_PS_POSITIONED_UPDATE equ 2
SQL_PS_SELECT_FOR_UPDATE equ 4
SQL_GB_NOT_SUPPORTED equ 0
SQL_GB_GROUP_BY_EQUALS_SELECT equ 1
SQL_GB_GROUP_BY_CONTAINS_SELECT equ 2
SQL_GB_NO_RELATION equ 3
SQL_GB_COLLATE	equ 4
SQL_OU_DML_STATEMENTS equ 1
SQL_OU_PROCEDURE_INVOCATION equ 2
SQL_OU_TABLE_DEFINITION equ 4
SQL_OU_INDEX_DEFINITION equ 8
SQL_OU_PRIVILEGE_DEFINITION equ 10h
SQL_SU_DML_STATEMENTS	equ SQL_OU_DML_STATEMENTS 
SQL_SU_PROCEDURE_INVOCATION	equ SQL_OU_PROCEDURE_INVOCATION
SQL_SU_TABLE_DEFINITION	equ SQL_OU_TABLE_DEFINITION
SQL_SU_INDEX_DEFINITION	equ SQL_OU_INDEX_DEFINITION
SQL_SU_PRIVILEGE_DEFINITION	equ SQL_OU_PRIVILEGE_DEFINITION
SQL_QU_DML_STATEMENTS equ 1
SQL_QU_PROCEDURE_INVOCATION equ 2
SQL_QU_TABLE_DEFINITION equ 4
SQL_QU_INDEX_DEFINITION equ 8
SQL_QU_PRIVILEGE_DEFINITION equ 10h
SQL_CU_DML_STATEMENTS	equ SQL_QU_DML_STATEMENTS
SQL_CU_PROCEDURE_INVOCATION	equ SQL_QU_PROCEDURE_INVOCATION 
SQL_CU_TABLE_DEFINITION	equ SQL_QU_TABLE_DEFINITION
SQL_CU_INDEX_DEFINITION	equ SQL_QU_INDEX_DEFINITION 
SQL_CU_PRIVILEGE_DEFINITION	equ SQL_QU_PRIVILEGE_DEFINITION 
SQL_SQ_COMPARISON equ 1
SQL_SQ_EXISTS equ 2
SQL_SQ_IN equ 4
SQL_SQ_QUANTIFIED equ 8
SQL_SQ_CORRELATED_SUBQUERIES equ 10h
SQL_U_UNION equ 1
SQL_U_UNION_ALL equ 2
SQL_BP_CLOSE equ 1
SQL_BP_DELETE equ 2
SQL_BP_DROP equ 4
SQL_BP_TRANSACTION equ 8
SQL_BP_UPDATE equ 10h
SQL_BP_OTHER_HSTMT equ 20h
SQL_BP_SCROLL equ 40h
SQL_SS_ADDITIONS equ 1
SQL_SS_DELETIONS equ 2
SQL_SS_UPDATES equ 4
SQL_CV_CREATE_VIEW	equ 1
SQL_CV_CHECK_OPTION	equ 2
SQL_CV_CASCADED	equ 4
SQL_CV_LOCAL	equ 8
SQL_LCK_NO_CHANGE equ 1
SQL_LCK_EXCLUSIVE equ 2
SQL_LCK_UNLOCK equ 4
SQL_POS_POSITION equ 1
SQL_POS_REFRESH equ 2
SQL_POS_UPDATE equ 4
SQL_POS_DELETE equ 8
SQL_POS_ADD equ 10h
SQL_QL_START equ 1
SQL_QL_END equ 2
SQL_AF_AVG	equ 1
SQL_AF_COUNT	equ 2
SQL_AF_MAX	equ 4
SQL_AF_MIN	equ 8
SQL_AF_SUM	equ 10h
SQL_AF_DISTINCT	equ 20h
SQL_AF_ALL	equ 40h	
SQL_SC_SQL92_ENTRY	equ 1
SQL_SC_FIPS127_2_TRANSITIONAL	equ 2
SQL_SC_SQL92_INTERMEDIATE	equ 4
SQL_SC_SQL92_FULL	equ 8
SQL_DL_SQL92_DATE	equ 1
SQL_DL_SQL92_TIME	equ 2
SQL_DL_SQL92_TIMESTAMP	equ 4
SQL_DL_SQL92_INTERVAL_YEAR	equ 8
SQL_DL_SQL92_INTERVAL_MONTH	equ 10h
SQL_DL_SQL92_INTERVAL_DAY	equ 20h
SQL_DL_SQL92_INTERVAL_HOUR	equ 40h
SQL_DL_SQL92_INTERVAL_MINUTE	equ 80h
SQL_DL_SQL92_INTERVAL_SECOND	equ 100h
SQL_DL_SQL92_INTERVAL_YEAR_TO_MONTH	equ 200h
SQL_DL_SQL92_INTERVAL_DAY_TO_HOUR	equ 400h
SQL_DL_SQL92_INTERVAL_DAY_TO_MINUTE	equ 800h
SQL_DL_SQL92_INTERVAL_DAY_TO_SECOND	equ 1000h
SQL_DL_SQL92_INTERVAL_HOUR_TO_MINUTE	equ 2000h
SQL_DL_SQL92_INTERVAL_HOUR_TO_SECOND	equ 4000h
SQL_DL_SQL92_INTERVAL_MINUTE_TO_SECOND	equ 8000h
SQL_CL_START	equ SQL_QL_START
SQL_CL_END	equ SQL_QL_END
SQL_BRC_PROCEDURES	equ 1
SQL_BRC_EXPLICIT	equ 2
SQL_BRC_ROLLED_UP	equ 4
SQL_BS_SELECT_EXPLICIT	equ 1
SQL_BS_ROW_COUNT_EXPLICIT	equ 2
SQL_BS_SELECT_PROC	equ 4
SQL_BS_ROW_COUNT_PROC	equ 8
SQL_PARC_BATCH	equ 1
SQL_PARC_NO_BATCH	equ 2
SQL_PAS_BATCH	equ 1
SQL_PAS_NO_BATCH	equ 2		
SQL_PAS_NO_SELECT	equ 3
SQL_IK_NONE	equ 0
SQL_IK_ASC	equ 1
SQL_IK_DESC	equ 2
SQL_IK_ALL	equ (SQL_IK_ASC + SQL_IK_DESC)
SQL_ISV_ASSERTIONS	equ 1
SQL_ISV_CHARACTER_SETS	equ 2
SQL_ISV_CHECK_CONSTRAINTS	equ 4
SQL_ISV_COLLATIONS	equ 8
SQL_ISV_COLUMN_DOMAIN_USAGE	equ 10h
SQL_ISV_COLUMN_PRIVILEGES	equ 20h
SQL_ISV_COLUMNS	equ 40h
SQL_ISV_CONSTRAINT_COLUMN_USAGE	equ 80h
SQL_ISV_CONSTRAINT_TABLE_USAGE	equ 100h
SQL_ISV_DOMAIN_CONSTRAINTS	equ 200h
SQL_ISV_DOMAINS	equ 400h
SQL_ISV_KEY_COLUMN_USAGE	equ 800h
SQL_ISV_REFERENTIAL_CONSTRAINTS	equ 1000h
SQL_ISV_SCHEMATA	equ 2000h
SQL_ISV_SQL_LANGUAGES	equ 4000h
SQL_ISV_TABLE_CONSTRAINTS	equ 8000h
SQL_ISV_TABLE_PRIVILEGES	equ 10000h
SQL_ISV_TABLES	equ 20000h
SQL_ISV_TRANSLATIONS	equ 40000h
SQL_ISV_USAGE_PRIVILEGES	equ 80000h
SQL_ISV_VIEW_COLUMN_USAGE	equ 100000h
SQL_ISV_VIEW_TABLE_USAGE	equ 200000h
SQL_ISV_VIEWS	equ 400000h
SQL_AD_CONSTRAINT_NAME_DEFINITION	equ 1	
SQL_AD_ADD_DOMAIN_CONSTRAINT	equ 2
SQL_AD_DROP_DOMAIN_CONSTRAINT	equ 4
SQL_AD_ADD_DOMAIN_DEFAULT equ 8
SQL_AD_DROP_DOMAIN_DEFAULT equ 10h
SQL_AD_ADD_CONSTRAINT_INITIALLY_DEFERRED	equ 20h
SQL_AD_ADD_CONSTRAINT_INITIALLY_IMMEDIATE	equ 40h
SQL_AD_ADD_CONSTRAINT_DEFERRABLE	equ 80h
SQL_AD_ADD_CONSTRAINT_NON_DEFERRABLE	equ 100h
SQL_CS_CREATE_SCHEMA	equ 1
SQL_CS_AUTHORIZATION	equ 2
SQL_CS_DEFAULT_CHARACTER_SET	equ 4
SQL_CTR_CREATE_TRANSLATION	equ 1
SQL_CA_CREATE_ASSERTION	equ 1
SQL_CA_CONSTRAINT_INITIALLY_DEFERRED	equ 10h
SQL_CA_CONSTRAINT_INITIALLY_IMMEDIATE	equ 20h
SQL_CA_CONSTRAINT_DEFERRABLE	equ 40h
SQL_CA_CONSTRAINT_NON_DEFERRABLE	equ 80h
SQL_CCS_CREATE_CHARACTER_SET	equ 1
SQL_CCS_COLLATE_CLAUSE	equ 2
SQL_CCS_LIMITED_COLLATION	equ 4
SQL_CCOL_CREATE_COLLATION	equ 1
SQL_CDO_CREATE_DOMAIN	equ 1
SQL_CDO_DEFAULT	equ 2
SQL_CDO_CONSTRAINT	equ 4
SQL_CDO_COLLATION	equ 8
SQL_CDO_CONSTRAINT_NAME_DEFINITION	equ 10h
SQL_CDO_CONSTRAINT_INITIALLY_DEFERRED	equ 20h
SQL_CDO_CONSTRAINT_INITIALLY_IMMEDIATE	equ 40h
SQL_CDO_CONSTRAINT_DEFERRABLE	equ 80h
SQL_CDO_CONSTRAINT_NON_DEFERRABLE	equ 100h
SQL_CT_CREATE_TABLE	equ 1
SQL_CT_COMMIT_PRESERVE	equ 2
SQL_CT_COMMIT_DELETE	equ 4
SQL_CT_GLOBAL_TEMPORARY	equ 8
SQL_CT_LOCAL_TEMPORARY	equ 10h
SQL_CT_CONSTRAINT_INITIALLY_DEFERRED	equ 20h
SQL_CT_CONSTRAINT_INITIALLY_IMMEDIATE	equ 40h
SQL_CT_CONSTRAINT_DEFERRABLE	equ 80h
SQL_CT_CONSTRAINT_NON_DEFERRABLE	equ 100h
SQL_CT_COLUMN_CONSTRAINT	equ 200h
SQL_CT_COLUMN_DEFAULT	equ 400h
SQL_CT_COLUMN_COLLATION	equ 800h
SQL_CT_TABLE_CONSTRAINT	equ 1000h
SQL_CT_CONSTRAINT_NAME_DEFINITION	equ 2000h
SQL_DI_CREATE_INDEX	equ 1
SQL_DI_DROP_INDEX	equ 2
SQL_DC_DROP_COLLATION	equ 1
SQL_DD_DROP_DOMAIN	equ 1
SQL_DD_RESTRICT	equ 2
SQL_DD_CASCADE	equ 4
SQL_DS_DROP_SCHEMA	equ 1
SQL_DS_RESTRICT	equ 2
SQL_DS_CASCADE	equ 4
SQL_DCS_DROP_CHARACTER_SET	equ 1
SQL_DA_DROP_ASSERTION	equ 1
SQL_DT_DROP_TABLE	equ 1
SQL_DT_RESTRICT	equ 2
SQL_DT_CASCADE	equ 4
SQL_DTR_DROP_TRANSLATION	equ 1
SQL_DV_DROP_VIEW	equ 1
SQL_DV_RESTRICT	equ 2
SQL_DV_CASCADE	equ 4
SQL_IS_INSERT_LITERALS	equ 1
SQL_IS_INSERT_SEARCHED	equ 2
SQL_IS_SELECT_INTO	equ 4
SQL_OIC_CORE	equ 1
SQL_OIC_LEVEL1	equ 2
SQL_OIC_LEVEL2	equ 3
SQL_SFKD_CASCADE	equ 1
SQL_SFKD_NO_ACTION	equ 2
SQL_SFKD_SET_DEFAULT	equ 4
SQL_SFKD_SET_NULL	equ 8
SQL_SFKU_CASCADE	equ 1
SQL_SFKU_NO_ACTION	equ 2
SQL_SFKU_SET_DEFAULT	equ 4
SQL_SFKU_SET_NULL	equ 8
SQL_SG_USAGE_ON_DOMAIN	equ 1
SQL_SG_USAGE_ON_CHARACTER_SET	equ 2
SQL_SG_USAGE_ON_COLLATION	equ 4
SQL_SG_USAGE_ON_TRANSLATION	equ 8
SQL_SG_WITH_GRANT_OPTION	equ 10h
SQL_SG_DELETE_TABLE	equ 20h
SQL_SG_INSERT_TABLE	equ 40h
SQL_SG_INSERT_COLUMN	equ 80h
SQL_SG_REFERENCES_TABLE	equ 100h
SQL_SG_REFERENCES_COLUMN	equ 200h
SQL_SG_SELECT_TABLE	equ 400h
SQL_SG_UPDATE_TABLE	equ 800h
SQL_SG_UPDATE_COLUMN	equ 1000h	
SQL_SP_EXISTS	equ 1
SQL_SP_ISNOTNULL	equ 2
SQL_SP_ISNULL	equ 4
SQL_SP_MATCH_FULL	equ 8
SQL_SP_MATCH_PARTIAL	equ 10h
SQL_SP_MATCH_UNIQUE_FULL	equ 20h
SQL_SP_MATCH_UNIQUE_PARTIAL	equ 40h
SQL_SP_OVERLAPS	equ 80h
SQL_SP_UNIQUE	equ 100h
SQL_SP_LIKE	equ 200h
SQL_SP_IN	equ 400h
SQL_SP_BETWEEN	equ 800h
SQL_SP_COMPARISON	equ 1000h
SQL_SP_QUANTIFIED_COMPARISON	equ 2000h
SQL_SRJO_CORRESPONDING_CLAUSE	equ 1
SQL_SRJO_CROSS_JOIN	equ 2
SQL_SRJO_EXCEPT_JOIN	equ 4
SQL_SRJO_FULL_OUTER_JOIN	equ 8
SQL_SRJO_INNER_JOIN	equ 10h
SQL_SRJO_INTERSECT_JOIN	equ 20h
SQL_SRJO_LEFT_OUTER_JOIN	equ 40h
SQL_SRJO_NATURAL_JOIN	equ 80h
SQL_SRJO_RIGHT_OUTER_JOIN	equ 100h
SQL_SRJO_UNION_JOIN	equ 200h
SQL_SR_USAGE_ON_DOMAIN	equ 1
SQL_SR_USAGE_ON_CHARACTER_SET	equ 2
SQL_SR_USAGE_ON_COLLATION	equ 4
SQL_SR_USAGE_ON_TRANSLATION	equ 8
SQL_SR_GRANT_OPTION_FOR	equ 10h
SQL_SR_CASCADE	equ 20h
SQL_SR_RESTRICT	equ 40h
SQL_SR_DELETE_TABLE	equ 80h
SQL_SR_INSERT_TABLE	equ 100h
SQL_SR_INSERT_COLUMN	equ 200h
SQL_SR_REFERENCES_TABLE	equ 400h
SQL_SR_REFERENCES_COLUMN	equ 800h
SQL_SR_SELECT_TABLE	equ 1000h
SQL_SR_UPDATE_TABLE	equ 2000h
SQL_SR_UPDATE_COLUMN	equ 4000h
SQL_SRVC_VALUE_EXPRESSION	equ 1
SQL_SRVC_NULL	equ 2
SQL_SRVC_DEFAULT	equ 4
SQL_SRVC_ROW_SUBQUERY	equ 8
SQL_SVE_CASE	equ 1
SQL_SVE_CAST	equ 2
SQL_SVE_COALESCE	equ 4
SQL_SVE_NULLIF	equ 8
SQL_SCC_XOPEN_CLI_VERSION1	equ 1
SQL_SCC_ISO92_CLI	equ 2
SQL_US_UNION	equ SQL_U_UNION
SQL_US_UNION_ALL	equ SQL_U_UNION_ALL
SQL_DTC_ENLIST_EXPENSIVE	equ 1
SQL_DTC_UNENLIST_EXPENSIVE	equ 2
SQL_FETCH_FIRST_USER	equ 31
SQL_FETCH_FIRST_SYSTEM	equ 32
SQL_ENTIRE_ROWSET equ 0
SQL_POSITION equ 0
SQL_REFRESH equ 1
SQL_UPDATE equ 2
SQL_DELETE equ 3
SQL_ADD equ 4
SQL_SETPOS_MAX_OPTION_VALUE	equ SQL_ADD
SQL_UPDATE_BY_BOOKMARK	equ 5
SQL_DELETE_BY_BOOKMARK	equ 6
SQL_FETCH_BY_BOOKMARK	equ 7
SQL_LOCK_NO_CHANGE equ 0
SQL_LOCK_EXCLUSIVE equ 1
SQL_LOCK_UNLOCK equ 2
SQL_SETPOS_MAX_LOCK_VALUE	equ SQL_LOCK_UNLOCK
SQL_BEST_ROWID equ 1
SQL_ROWVER equ 2
SQL_PC_NOT_PSEUDO equ 1
SQL_QUICK equ 0
SQL_ENSURE equ 1
SQL_TABLE_STAT equ 0
SQL_ALL_CATALOGS	equ "%"
SQL_ALL_SCHEMAS	equ "%"
SQL_ALL_TABLE_TYPES	equ "%"
SQL_DRIVER_NOPROMPT equ 0
SQL_DRIVER_COMPLETE equ 1
SQL_DRIVER_PROMPT equ 2
SQL_DRIVER_COMPLETE_REQUIRED equ 3
SQL_FETCH_BOOKMARK equ 8
SQL_ROW_SUCCESS equ 0
SQL_ROW_DELETED equ 1
SQL_ROW_UPDATED equ 2
SQL_ROW_NOROW equ 3
SQL_ROW_ADDED equ 4
SQL_ROW_ERROR equ 5
SQL_ROW_SUCCESS_WITH_INFO	equ 6
SQL_ROW_PROCEED	equ 0
SQL_ROW_IGNORE	equ 1
SQL_PARAM_SUCCESS	equ 0
SQL_PARAM_SUCCESS_WITH_INFO	equ 6
SQL_PARAM_ERROR	equ 5
SQL_PARAM_UNUSED	equ 7
SQL_PARAM_DIAG_UNAVAILABLE	equ 1
SQL_PARAM_PROCEED	equ 0
SQL_PARAM_IGNORE	equ 1
SQL_CASCADE equ 0
SQL_RESTRICT equ 1
SQL_SET_NULL equ 2
SQL_NO_ACTION	equ 3
SQL_SET_DEFAULT	equ 4
SQL_INITIALLY_DEFERRED	equ 5
SQL_INITIALLY_IMMEDIATE	equ 6
SQL_NOT_DEFERRABLE	equ 7
SQL_PARAM_TYPE_UNKNOWN equ 0
SQL_PARAM_INPUT equ 1
SQL_PARAM_INPUT_OUTPUT equ 2
SQL_RESULT_COL equ 3
SQL_PARAM_OUTPUT equ 4
SQL_RETURN_VALUE equ 5
SQL_PT_UNKNOWN equ 0
SQL_PT_PROCEDURE equ 1
SQL_PT_FUNCTION equ 2
SQL_YEAR	equ SQL_CODE_YEAR
SQL_MONTH	equ SQL_CODE_MONTH
SQL_DAY	equ SQL_CODE_DAY
SQL_HOUR	equ SQL_CODE_HOUR
SQL_MINUTE	equ SQL_CODE_MINUTE
SQL_SECOND	equ SQL_CODE_SECOND	
SQL_YEAR_TO_MONTH	equ SQL_CODE_YEAR_TO_MONTH
SQL_DAY_TO_HOUR	equ SQL_CODE_DAY_TO_HOUR
SQL_DAY_TO_MINUTE	equ SQL_CODE_DAY_TO_MINUTE
SQL_DAY_TO_SECOND	equ SQL_CODE_DAY_TO_SECOND
SQL_HOUR_TO_MINUTE	equ SQL_CODE_HOUR_TO_MINUTE	
SQL_HOUR_TO_SECOND	equ SQL_CODE_HOUR_TO_SECOND
SQL_MINUTE_TO_SECOND	equ SQL_CODE_MINUTE_TO_SECOND
SQL_DATABASE_NAME equ 16
SQL_FD_FETCH_PREV equ SQL_FD_FETCH_PRIOR
SQL_FETCH_PREV equ SQL_FETCH_PRIOR
SQL_CONCUR_TIMESTAMP equ SQL_CONCUR_ROWVER
SQL_SCCO_OPT_TIMESTAMP equ SQL_SCCO_OPT_ROWVER
SQL_CC_DELETE equ SQL_CB_DELETE
SQL_CR_DELETE equ SQL_CB_DELETE
SQL_CC_CLOSE equ SQL_CB_CLOSE
SQL_CR_CLOSE equ SQL_CB_CLOSE
SQL_CC_PRESERVE equ SQL_CB_PRESERVE
SQL_CR_PRESERVE equ SQL_CB_PRESERVE
SQL_SCROLL_FORWARD_ONLY equ 0
SQL_SCROLL_KEYSET_DRIVEN equ (-1)
SQL_SCROLL_DYNAMIC equ (-2)
SQL_SCROLL_STATIC equ (-3)
TRACE_VERSION	equ 1000
TRACE_ON	equ 1
TRACE_VS_EVENT_ON	equ 2	
ODBC_VS_FLAG_UNICODE_ARG	equ 1
ODBC_VS_FLAG_UNICODE_COR	equ 2
ODBC_VS_FLAG_RETCODE	equ 4
ODBC_VS_FLAG_STOP equ 8
;-----------------------------------------------------------------------------------------
;				sqltypes.h
;-----------------------------------------------------------------------------------------
DATE_STRUCT struct
         year dw ?
        month dw ?
        day dw ?
DATE_STRUCT ends
SQL_DATE_STRUCT equ DATE_STRUCT
TIME_STRUCT struct
        hour dw ?
        minute dw ?
        second dw ?
TIME_STRUCT ends
SQL_TIME_STRUCT equ TIME_STRUCT
TIMESTAMP_STRUCT struct
        year dw ?
        month dw ?
        day dw ?
        hour dw ?
        minute dw ?
        second dw ?
        fraction dd ?
TIMESTAMP_STRUCT ends
SQL_TIMESTAMP_STRUCT equ TIMESTAMP_STRUCT
SQL_IS_YEAR						equ 1
SQL_IS_MONTH					equ 2
SQL_IS_DAY						equ 3
SQL_IS_HOUR						equ 4
SQL_IS_MINUTE					equ 5
SQL_IS_SECOND					equ 6
SQL_IS_YEAR_TO_MONTH			equ 7
SQL_IS_DAY_TO_HOUR				equ 8
SQL_IS_DAY_TO_MINUTE			equ 9
SQL_IS_DAY_TO_SECOND			equ 10
SQL_IS_HOUR_TO_MINUTE			equ 11
SQL_IS_HOUR_TO_SECOND			equ 12
SQL_IS_MINUTE_TO_SECOND		equ 13
SQL_YEAR_MONTH_STRUCT struct
		year dword ?
		month dword ?
SQL_YEAR_MONTH_STRUCT ends
SQL_DAY_SECOND_STRUCT struct
		day dword ?
		hour dword ?
		minute dword ?
		second dword ?
		fraction dword ?
SQL_DAY_SECOND_STRUCT ends

SQL_INTERVAL_STRUCT struct
	interval_type dword ?
	interval_sign dword ?
	union intval
		year_month SQL_YEAR_MONTH_STRUCT <>
		day_second SQL_DAY_SECOND_STRUCT <>
	ends
SQL_INTERVAL_STRUCT ends
SQL_MAX_NUMERIC_LEN		equ 16
SQL_NUMERIC_STRUCT struct
	precision db ?
	scale db ?
	sign db ?
	val db SQL_MAX_NUMERIC_LEN dup(?)
SQL_NUMERIC_STRUCT ends
SQLGUID struct
    Data1 dd ?
    Data2 dw ?
    Data3 dw ?
    Data4  db 8 dup(?)
SQLGUID ends

;;;; end odbcsql


;;;; lmpapi

class_NULL                       equ 0
class_SESSION                    equ 1
class_SESSION_GROUP              equ 2
class_RSVP_HOP                   equ 3
class_INTEGRITY                  equ 4
class_TIME_VALUES                equ 5
class_ERROR_SPEC                 equ 6
class_SCOPE                      equ 7
class_STYLE                      equ 8
class_FLOWSPEC                   equ 9
class_IS_FLOWSPEC                equ 9
class_FILTER_SPEC                equ 10
class_SENDER_TEMPLATE            equ 11
class_SENDER_TSPEC               equ 12
class_ADSPEC                     equ 13
class_POLICY_DATA                equ 14
class_CONFIRM                    equ 15
class_MAX                        equ 15
ctype_SESSION_ipv4               equ 1
ctype_SESSION_ipv4GPI            equ 3
SESSFLG_E_Police                 equ 01h
ctype_RSVP_HOP_ipv4              equ 1
Opt_Share_mask                   equ 00000018h
Opt_Distinct                     equ 00000008h
Opt_Shared                       equ 00000010h
Opt_SndSel_mask                  equ 00000007h
Opt_Wildcard                     equ 00000001h
Opt_Explicit                     equ 00000002h
ctype_STYLE                      equ 1
ctype_FILTER_SPEC_ipv4           equ 1
ctype_FILTER_SPEC_ipv4GPI        equ 4
ctype_SENDER_TEMPLATE_ipv4       equ 1
ctype_SENDER_TEMPLATE_ipv4GPI    equ 4
ctype_SCOPE_list_ipv4            equ 1
ctype_ERROR_SPEC_ipv4            equ 1
ERROR_SPECF_InPlace              equ 01h
ERROR_SPECF_NotGuilty            equ 02h
ERR_FORWARD_OK                   equ 8000h
ERR_Usage_globl                  equ 00h
ERR_Usage_local                  equ 10h
ERR_Usage_serv                   equ 11h
ERR_global_mask                  equ 0FFFh
ctype_POLICY_DATA                equ 1
GENERAL_INFO                     equ 1
GUARANTEED_SERV                  equ 2
PREDICTIVE_SERV                  equ 3
CONTROLLED_DELAY_SERV            equ 4
CONTROLLED_LOAD_SERV             equ 5
QUALITATIVE_SERV                 equ 6
INTSERV_VERS_MASK                equ 0F0h
INTSERV_VERSION0                 equ 0
ISSH_BREAK_BIT                   equ 80h
ISPH_FLG_INV                     equ 80h
ctype_SENDER_TSPEC               equ 2
ctype_FLOWSPEC_Intserv0          equ 2
ctype_ADSPEC_INTSERV             equ 2
RSVP_PATH                        equ 1
RSVP_RESV                        equ 2
RSVP_PATH_ERR                    equ 3
RSVP_RESV_ERR                    equ 4
RSVP_PATH_TEAR                   equ 5
RSVP_RESV_TEAR                   equ 6
RSVP_Err_NONE                    equ 0
RSVP_Erv_Nonev                   equ 0
RSVP_Err_ADMISSION               equ 1
RSVP_Erv_Other                   equ 0
RSVP_Erv_DelayBnd                equ 1
RSVP_Erv_Bandwidth               equ 2
RSVP_Erv_MTU                     equ 3
RSVP_Erv_Flow_Rate               equ 8001h
RSVP_Erv_Bucket_szie             equ 8002h
RSVP_Erv_Peak_Rate               equ 8003h
RSVP_Erv_Min_Policied_size       equ 8004h
RSVP_Err_POLICY                  equ 2
POLICY_ERRV_NO_MORE_INFO         equ 1
POLICY_ERRV_UNSUPPORTED_CREDENTIAL_TYPE equ 2
POLICY_ERRV_INSUFFICIENT_PRIVILEGES equ 3
POLICY_ERRV_EXPIRED_CREDENTIALS  equ 4
POLICY_ERRV_IDENTITY_CHANGED     equ 5
POLICY_ERRV_UNKNOWN              equ 0
POLICY_ERRV_GLOBAL_DEF_FLOW_COUNT equ 1
POLICY_ERRV_GLOBAL_GRP_FLOW_COUNT equ 2
POLICY_ERRV_GLOBAL_USER_FLOW_COUNT equ 3
POLICY_ERRV_GLOBAL_UNAUTH_USER_FLOW_COUNT equ 4
POLICY_ERRV_SUBNET_DEF_FLOW_COUNT equ 5
POLICY_ERRV_SUBNET_GRP_FLOW_COUNT equ 6
POLICY_ERRV_SUBNET_USER_FLOW_COUNT equ 7
POLICY_ERRV_SUBNET_UNAUTH_USER_FLOW_COUNT equ 8
POLICY_ERRV_GLOBAL_DEF_FLOW_DURATION equ 9
POLICY_ERRV_GLOBAL_GRP_FLOW_DURATION equ 10
POLICY_ERRV_GLOBAL_USER_FLOW_DURATION equ 11
POLICY_ERRV_GLOBAL_UNAUTH_USER_FLOW_DURATION equ 12
POLICY_ERRV_SUBNET_DEF_FLOW_DURATION equ 13
POLICY_ERRV_SUBNET_GRP_FLOW_DURATION equ 14
POLICY_ERRV_SUBNET_USER_FLOW_DURATION equ 15
POLICY_ERRV_SUBNET_UNAUTH_USER_FLOW_DURATION equ 16
POLICY_ERRV_GLOBAL_DEF_FLOW_RATE equ 17
POLICY_ERRV_GLOBAL_GRP_FLOW_RATE equ 18
POLICY_ERRV_GLOBAL_USER_FLOW_RATE equ 19
POLICY_ERRV_GLOBAL_UNAUTH_USER_FLOW_RATE equ 20
POLICY_ERRV_SUBNET_DEF_FLOW_RATE equ 21
POLICY_ERRV_SUBNET_GRP_FLOW_RATE equ 22
POLICY_ERRV_SUBNET_USER_FLOW_RATE equ 23
POLICY_ERRV_SUBNET_UNAUTH_USER_FLOW_RATE equ 24
POLICY_ERRV_GLOBAL_DEF_PEAK_RATE equ 25
POLICY_ERRV_GLOBAL_GRP_PEAK_RATE equ 26
POLICY_ERRV_GLOBAL_USER_PEAK_RATE equ 27
POLICY_ERRV_GLOBAL_UNAUTH_USER_PEAK_RATE equ 28
POLICY_ERRV_SUBNET_DEF_PEAK_RATE equ 29
POLICY_ERRV_SUBNET_GRP_PEAK_RATE equ 30
POLICY_ERRV_SUBNET_USER_PEAK_RATE equ 31
POLICY_ERRV_SUBNET_UNAUTH_USER_PEAK_RATE equ 32
POLICY_ERRV_GLOBAL_DEF_SUM_FLOW_RATE equ 33
POLICY_ERRV_GLOBAL_GRP_SUM_FLOW_RATE equ 34
POLICY_ERRV_GLOBAL_USER_SUM_FLOW_RATE equ 35
POLICY_ERRV_GLOBAL_UNAUTH_USER_SUM_FLOW_RATE equ 36
POLICY_ERRV_SUBNET_DEF_SUM_FLOW_RATE equ 37
POLICY_ERRV_SUBNET_GRP_SUM_FLOW_RATE equ 38
POLICY_ERRV_SUBNET_USER_SUM_FLOW_RATE equ 39
POLICY_ERRV_SUBNET_UNAUTH_USER_SUM_FLOW_RATE equ 40
POLICY_ERRV_GLOBAL_DEF_SUM_PEAK_RATE equ 41
POLICY_ERRV_GLOBAL_GRP_SUM_PEAK_RATE equ 42
POLICY_ERRV_GLOBAL_USER_SUM_PEAK_RATE equ 43
POLICY_ERRV_GLOBAL_UNAUTH_USER_SUM_PEAK_RATE equ 44
POLICY_ERRV_SUBNET_DEF_SUM_PEAK_RATE equ 45
POLICY_ERRV_SUBNET_GRP_SUM_PEAK_RATE equ 46
POLICY_ERRV_SUBNET_USER_SUM_PEAK_RATE equ 47
POLICY_ERRV_SUBNET_UNAUTH_USER_SUM_PEAK_RATE equ 48
POLICY_ERRV_UNKNOWN_USER         equ 49
POLICY_ERRV_NO_PRIVILEGES        equ 50
POLICY_ERRV_EXPIRED_USER_TOKEN   equ 51
POLICY_ERRV_NO_RESOURCES         equ 52
POLICY_ERRV_PRE_EMPTED           equ 53
POLICY_ERRV_USER_CHANGED         equ 54
POLICY_ERRV_NO_ACCEPTS           equ 55
POLICY_ERRV_NO_MEMORY            equ 56
POLICY_ERRV_CRAZY_FLOWSPEC       equ 57
RSVP_Err_NO_PATH                 equ 3
RSVP_Err_NO_SENDER               equ 4
RSVP_Err_BAD_STYLE               equ 5
RSVP_Err_UNKNOWN_STYLE           equ 6
RSVP_Err_BAD_DSTPORT             equ 7
RSVP_Err_BAD_SNDPORT             equ 8
RSVP_Err_AMBIG_FILTER            equ 9
RSVP_Err_PREEMPTED               equ 12
RSVP_Err_UNKN_OBJ_CLASS          equ 13
RSVP_Err_UNKNOWN_CTYPE           equ 14
RSVP_Err_API_ERROR               equ 20
RSVP_Err_TC_ERROR                equ 21
RSVP_Erv_Conflict_Serv           equ 01
RSVP_Erv_No_Serv                 equ 02
RSVP_Erv_Crazy_Flowspec          equ 03
RSVP_Erv_Crazy_Tspec             equ 04
RSVP_Err_TC_SYS_ERROR            equ 22
RSVP_Err_RSVP_SYS_ERROR          equ 23
RSVP_Erv_MEMORY                  equ 1
RSVP_Erv_API                     equ 2
LPM_PE_USER_IDENTITY             equ 2
LPM_PE_APP_IDENTITY              equ 3
ERROR_NO_MORE_INFO               equ 1
UNSUPPORTED_CREDENTIAL_TYPE      equ 2
INSUFFICIENT_PRIVILEGES          equ 3
EXPIRED_CREDENTIAL               equ 4
IDENTITY_CHANGED                 equ 5
LPM_OK                           equ 0
INV_LPM_HANDLE                   equ 1
LPM_TIME_OUT                     equ 2
INV_REQ_HANDLE                   equ 3
DUP_RESULTS                      equ 4
INV_RESULTS                      equ 5
LPM_PE_ALL_TYPES                 equ 0
LPM_API_VERSION_1                equ 1
PCM_VERSION_1                    equ 1
LPV_RESERVED                     equ 0
LPV_MIN_PRIORITY                 equ 1
LPV_MAX_PRIORITY                 equ 0FF00h
LPV_DROP_MSG                     equ 0FFFDh
LPV_DONT_CARE                    equ 0FFFEh
LPV_REJECT                       equ 0FFFFh
FORCE_IMMEDIATE_REFRESH          equ 1
LPM_RESULT_READY                 equ 0
LPM_RESULT_DEFER                 equ 1
RCVD_PATH_TEAR                   equ 1
RCVD_RESV_TEAR                   equ 2
ADM_CTRL_FAILED                  equ 3
STATE_TIMEOUT                    equ 4
FLOW_DURATION                    equ 5
RESOURCES_ALLOCATED              equ 1
RESOURCES_MODIFIED               equ 2

;;;; end lmpapi


;;;; mmreg

_INC_MMREG                       equ 158
MM_CREATIVE                      equ 2
MM_MEDIAVISION                   equ 3
MM_FUJITSU                       equ 4
MM_PRAGMATRAX                    equ 5
MM_CYRIX                         equ 6
MM_PHILIPS_SPEECH_PROCESSING     equ 7
MM_NETXL                         equ 8
MM_ZYXEL                         equ 9
MM_BECUBED                       equ 10
MM_AARDVARK                      equ 11
MM_BINTEC                        equ 12
MM_HEWLETT_PACKARD               equ 13
MM_ACULAB                        equ 14
MM_FAITH                         equ 15
MM_MITEL                         equ 16
MM_QUANTUM3D                     equ 17
MM_SNI                           equ 18
MM_EMU                           equ 19
MM_ARTISOFT                      equ 20
MM_TURTLE_BEACH                  equ 21
MM_IBM                           equ 22
MM_VOCALTEC                      equ 23
MM_ROLAND                        equ 24
MM_DSP_SOLUTIONS                 equ 25
MM_NEC                           equ 26
MM_ATI                           equ 27
MM_WANGLABS                      equ 28
MM_TANDY                         equ 29
MM_VOYETRA                       equ 30
MM_ANTEX                         equ 31
MM_ICL_PS                        equ 32
MM_INTEL                         equ 33
MM_GRAVIS                        equ 34
MM_VAL                           equ 35
MM_INTERACTIVE                   equ 36
MM_YAMAHA                        equ 37
MM_EVEREX                        equ 38
MM_ECHO                          equ 39
MM_SIERRA                        equ 40
MM_CAT                           equ 41
MM_APPS                          equ 42
MM_DSP_GROUP                     equ 43
MM_MELABS                        equ 44
MM_COMPUTER_FRIENDS              equ 45
MM_ESS                           equ 46
MM_AUDIOFILE                     equ 47
MM_MOTOROLA                      equ 48
MM_CANOPUS                       equ 49
MM_EPSON                         equ 50
MM_TRUEVISION                    equ 51
MM_AZTECH                        equ 52
MM_VIDEOLOGIC                    equ 53
MM_SCALACS                       equ 54
MM_KORG                          equ 55
MM_APT                           equ 56
MM_ICS                           equ 57
MM_ITERATEDSYS                   equ 58
MM_METHEUS                       equ 59
MM_LOGITECH                      equ 60
MM_WINNOV                        equ 61
MM_NCR                           equ 62
MM_EXAN                          equ 63
MM_AST                           equ 64
MM_WILLOWPOND                    equ 65
MM_SONICFOUNDRY                  equ 66
MM_VITEC                         equ 67
MM_MOSCOM                        equ 68
MM_SILICONSOFT                   equ 69
MM_TERRATEC                      equ 70
MM_MEDIASONIC                    equ 71
MM_SANYO                         equ 72
MM_SUPERMAC                      equ 73
MM_AUDIOPT                       equ 74
MM_NOGATECH                      equ 75
MM_SPEECHCOMP                    equ 76
MM_AHEAD                         equ 77
MM_DOLBY                         equ 78
MM_OKI                           equ 79
MM_AURAVISION                    equ 80
MM_OLIVETTI                      equ 81
MM_IOMAGIC                       equ 82
MM_MATSUSHITA                    equ 83
MM_CONTROLRES                    equ 84
MM_XEBEC                         equ 85
MM_NEWMEDIA                      equ 86
MM_NMS                           equ 87
MM_LYRRUS                        equ 88
MM_COMPUSIC                      equ 89
MM_OPTI                          equ 90
MM_ADLACC                        equ 91
MM_COMPAQ                        equ 92
MM_DIALOGIC                      equ 93
MM_INSOFT                        equ 94
MM_MPTUS                         equ 95
MM_WEITEK                        equ 96
MM_LERNOUT_AND_HAUSPIE           equ 97
MM_QCIAR                         equ 98
MM_APPLE                         equ 99
MM_DIGITAL                       equ 100
MM_MOTU                          equ 101
MM_WORKBIT                       equ 102
MM_OSITECH                       equ 103
MM_MIRO                          equ 104
MM_CIRRUSLOGIC                   equ 105
MM_ISOLUTION                     equ 106
MM_HORIZONS                      equ 107
MM_CONCEPTS                      equ 108
MM_VTG                           equ 109
MM_RADIUS                        equ 110
MM_ROCKWELL                      equ 111
MM_XYZ                           equ 112
MM_OPCODE                        equ 113
MM_VOXWARE                       equ 114
MM_NORTHERN_TELECOM              equ 115
MM_APICOM                        equ 116
MM_GRANDE                        equ 117
MM_ADDX                          equ 118
MM_WILDCAT                       equ 119
MM_RHETOREX                      equ 120
MM_BROOKTREE                     equ 121
MM_ENSONIQ                       equ 125
MM_FAST                          equ 126
MM_NVIDIA                        equ 127
MM_OKSORI                        equ 128
MM_DIACOUSTICS                   equ 129
MM_GULBRANSEN                    equ 130
MM_KAY_ELEMETRICS                equ 131
MM_CRYSTAL                       equ 132
MM_SPLASH_STUDIOS                equ 133
MM_QUARTERDECK                   equ 134
MM_TDK                           equ 135
MM_DIGITAL_AUDIO_LABS            equ 136
MM_SEERSYS                       equ 137
MM_PICTURETEL                    equ 138
MM_ATT_MICROELECTRONICS          equ 139
MM_OSPREY                        equ 140
MM_MEDIATRIX                     equ 141
MM_SOUNDESIGNS                   equ 142
MM_ALDIGITAL                     equ 143
MM_SPECTRUM_SIGNAL_PROCESSING    equ 144
MM_ECS                           equ 145
MM_AMD                           equ 146
MM_COREDYNAMICS                  equ 147
MM_CANAM                         equ 148
MM_SOFTSOUND                     equ 149
MM_NORRIS                        equ 150
MM_DDD                           equ 151
MM_EUPHONICS                     equ 152
MM_PRECEPT                       equ 153
MM_CRYSTAL_NET                   equ 154
MM_CHROMATIC                     equ 155
MM_VOICEINFO                     equ 156
MM_VIENNASYS                     equ 157
MM_CONNECTIX                     equ 158
MM_GADGETLABS                    equ 159
MM_FRONTIER                      equ 160
MM_VIONA                         equ 161
MM_CASIO                         equ 162
MM_DIAMONDMM                     equ 163
MM_S3                            equ 164
MM_DVISION                       equ 165
MM_NETSCAPE                      equ 166
MM_SOUNDSPACE                    equ 167
MM_VANKOEVERING                  equ 168
MM_QTEAM                         equ 169
MM_ZEFIRO                        equ 170
MM_STUDER                        equ 171
MM_FRAUNHOFER_IIS                equ 172
MM_QUICKNET                      equ 173
MM_ALARIS                        equ 174
MM_SICRESOURCE                   equ 175
MM_NEOMAGIC                      equ 176
MM_MERGING_TECHNOLOGIES          equ 177
MM_XIRLINK                       equ 178
MM_COLORGRAPH                    equ 179
MM_OTI                           equ 180
MM_AUREAL                        equ 181
MM_VIVO                          equ 182
MM_SHARP                         equ 183
MM_LUCENT                        equ 184
MM_ATT                           equ 185
MM_SUNCOM                        equ 186
MM_SORVIS                        equ 187
MM_INVISION                      equ 188
MM_BERKOM                        equ 189
MM_MARIAN                        equ 190
MM_DPSINC                        equ 191
MM_BCB                           equ 192
MM_MOTIONPIXELS                  equ 193
MM_QDESIGN                       equ 194
MM_NMP                           equ 195
MM_DATAFUSION                    equ 196
MM_DUCK                          equ 197
MM_FTR                           equ 198
MM_BERCOS                        equ 199
MM_ONLIVE                        equ 200
MM_SIEMENS_SBC                   equ 201
MM_TERALOGIC                     equ 202
MM_PHONET                        equ 203
MM_WINBOND                       equ 204
MM_VIRTUALMUSIC                  equ 205
MM_ENET                          equ 206
MM_GUILLEMOT                     equ 207
MM_EMAGIC                        equ 208
MM_MWM                           equ 209
MM_PACIFICRESEARCH               equ 210
MM_SIPROLAB                      equ 211
MM_LYNX                          equ 212
MM_SPECTRUM_PRODUCTIONS          equ 213
MM_DICTAPHONE                    equ 214
MM_QUALCOMM                      equ 215
MM_RZS                           equ 216
MM_AUDIOSCIENCE                  equ 217
MM_PINNACLE                      equ 218
MM_EES                           equ 219
MM_HAFTMANN                      equ 220
MM_LUCID                         equ 221
MM_HEADSPACE                     equ 222
MM_UNISYS                        equ 223
MM_LUMINOSITI                    equ 224
MM_ACTIVEVOICE                   equ 225
MM_DTS                           equ 226
MM_DIGIGRAM                      equ 227
MM_SOFTLAB_NSK                   equ 228
MM_FORTEMEDIA                    equ 229
MM_SONORUS                       equ 230
MM_ARRAY                         equ 231
MM_DATARAN                       equ 232
MM_I_LINK                        equ 233
MM_SELSIUS_SYSTEMS               equ 234
MM_ADMOS                         equ 235
MM_LEXICON                       equ 236
MM_SGI                           equ 237
MM_IPI                           equ 238
MM_ICE                           equ 239
MM_VQST                          equ 240
MM_ETEK                          equ 241
MM_CS                            equ 242
MM_ALESIS                        equ 243
MM_INTERNET                      equ 244
MM_SONY                          equ 245
MM_HYPERACTIVE                   equ 246
MM_UHER_INFORMATIC               equ 247
MM_SYDEC_NV                      equ 248
MM_FLEXION                       equ 249
MM_VIA                           equ 250
MM_MICRONAS                      equ 251
MM_ANALOGDEVICES                 equ 252
MM_HP                            equ 253
MM_MATROX_DIV                    equ 254
MM_QUICKAUDIO                    equ 255
MM_YOUCOM                        equ 256
MM_RICHMOND                      equ 257
MM_IODD                          equ 258
MM_ICCC                          equ 259
MM_3COM                          equ 260
MM_MALDEN                        equ 261
MM_3DFX                          equ 262
MM_MINDMAKER                     equ 263
MM_TELEKOL                       equ 264
MM_ST_MICROELECTRONICS           equ 265
MM_ALGOVISION                    equ 266
MM_UNMAPPED                      equ 0FFFFh
MM_PID_UNMAPPED                  equ MM_UNMAPPED
MM_PCSPEAKER_WAVEOUT             equ 13
MM_MSFT_WSS_WAVEIN               equ 14
MM_MSFT_WSS_WAVEOUT              equ 15
MM_MSFT_WSS_FMSYNTH_STEREO       equ 16
MM_MSFT_WSS_MIXER                equ 17
MM_MSFT_WSS_OEM_WAVEIN           equ 18
MM_MSFT_WSS_OEM_WAVEOUT          equ 19
MM_MSFT_WSS_OEM_FMSYNTH_STEREO   equ 20
MM_MSFT_WSS_AUX                  equ 21
MM_MSFT_WSS_OEM_AUX              equ 22
MM_MSFT_GENERIC_WAVEIN           equ 23
MM_MSFT_GENERIC_WAVEOUT          equ 24
MM_MSFT_GENERIC_MIDIIN           equ 25
MM_MSFT_GENERIC_MIDIOUT          equ 26
MM_MSFT_GENERIC_MIDISYNTH        equ 27
MM_MSFT_GENERIC_AUX_LINE         equ 28
MM_MSFT_GENERIC_AUX_MIC          equ 29
MM_MSFT_GENERIC_AUX_CD           equ 30
MM_MSFT_WSS_OEM_MIXER            equ 31
MM_MSFT_MSACM                    equ 32
MM_MSFT_ACM_MSADPCM              equ 33
MM_MSFT_ACM_IMAADPCM             equ 34
MM_MSFT_ACM_MSFILTER             equ 35
MM_MSFT_ACM_GSM610               equ 36
MM_MSFT_ACM_G711                 equ 37
MM_MSFT_ACM_PCM                  equ 38
MM_WSS_SB16_WAVEIN               equ 39
MM_WSS_SB16_WAVEOUT              equ 40
MM_WSS_SB16_MIDIIN               equ 41
MM_WSS_SB16_MIDIOUT              equ 42
MM_WSS_SB16_SYNTH                equ 43
MM_WSS_SB16_AUX_LINE             equ 44
MM_WSS_SB16_AUX_CD               equ 45
MM_WSS_SB16_MIXER                equ 46
MM_WSS_SBPRO_WAVEIN              equ 47
MM_WSS_SBPRO_WAVEOUT             equ 48
MM_WSS_SBPRO_MIDIIN              equ 49
MM_WSS_SBPRO_MIDIOUT             equ 50
MM_WSS_SBPRO_SYNTH               equ 51
MM_WSS_SBPRO_AUX_LINE            equ 52
MM_WSS_SBPRO_AUX_CD              equ 53
MM_WSS_SBPRO_MIXER               equ 54
MM_MSFT_WSS_NT_WAVEIN            equ 55
MM_MSFT_WSS_NT_WAVEOUT           equ 56
MM_MSFT_WSS_NT_FMSYNTH_STEREO    equ 57
MM_MSFT_WSS_NT_MIXER             equ 58
MM_MSFT_WSS_NT_AUX               equ 59
MM_MSFT_SB16_WAVEIN              equ 60
MM_MSFT_SB16_WAVEOUT             equ 61
MM_MSFT_SB16_MIDIIN              equ 62
MM_MSFT_SB16_MIDIOUT             equ 63
MM_MSFT_SB16_SYNTH               equ 64
MM_MSFT_SB16_AUX_LINE            equ 65
MM_MSFT_SB16_AUX_CD              equ 66
MM_MSFT_SB16_MIXER               equ 67
MM_MSFT_SBPRO_WAVEIN             equ 68
MM_MSFT_SBPRO_WAVEOUT            equ 69
MM_MSFT_SBPRO_MIDIIN             equ 70
MM_MSFT_SBPRO_MIDIOUT            equ 71
MM_MSFT_SBPRO_SYNTH              equ 72
MM_MSFT_SBPRO_AUX_LINE           equ 73
MM_MSFT_SBPRO_AUX_CD             equ 74
MM_MSFT_SBPRO_MIXER              equ 75
MM_MSFT_MSOPL_SYNTH              equ 76
MM_MSFT_VMDMS_LINE_WAVEIN        equ 80
MM_MSFT_VMDMS_LINE_WAVEOUT       equ 81
MM_MSFT_VMDMS_HANDSET_WAVEIN     equ 82
MM_MSFT_VMDMS_HANDSET_WAVEOUT    equ 83
MM_MSFT_VMDMW_LINE_WAVEIN        equ 84
MM_MSFT_VMDMW_LINE_WAVEOUT       equ 85
MM_MSFT_VMDMW_HANDSET_WAVEIN     equ 86
MM_MSFT_VMDMW_HANDSET_WAVEOUT    equ 87
MM_MSFT_VMDMW_MIXER              equ 88
MM_MSFT_VMDM_GAME_WAVEOUT        equ 89
MM_MSFT_VMDM_GAME_WAVEIN         equ 90
MM_MSFT_ACM_MSNAUDIO             equ 91
MM_MSFT_ACM_MSG723               equ 92
MM_MSFT_ACM_MSRT24               equ 93
MM_MSFT_WDMAUDIO_WAVEOUT         equ 100
MM_MSFT_WDMAUDIO_WAVEIN          equ 101
MM_MSFT_WDMAUDIO_MIDIOUT         equ 102
MM_MSFT_WDMAUDIO_MIDIIN          equ 103
MM_MSFT_WDMAUDIO_MIXER           equ 104
MM_MSFT_WDMAUDIO_AUX             equ 105
MM_CREATIVE_SB15_WAVEIN          equ 1
MM_CREATIVE_SB20_WAVEIN          equ 2
MM_CREATIVE_SBPRO_WAVEIN         equ 3
MM_CREATIVE_SBP16_WAVEIN         equ 4
MM_CREATIVE_PHNBLST_WAVEIN       equ 5
MM_CREATIVE_SB15_WAVEOUT         equ 101
MM_CREATIVE_SB20_WAVEOUT         equ 102
MM_CREATIVE_SBPRO_WAVEOUT        equ 103
MM_CREATIVE_SBP16_WAVEOUT        equ 104
MM_CREATIVE_PHNBLST_WAVEOUT      equ 105
MM_CREATIVE_MIDIOUT              equ 201
MM_CREATIVE_MIDIIN               equ 202
MM_CREATIVE_FMSYNTH_MONO         equ 301
MM_CREATIVE_FMSYNTH_STEREO       equ 302
MM_CREATIVE_MIDI_AWE32           equ 303
MM_CREATIVE_AUX_CD               equ 401
MM_CREATIVE_AUX_LINE             equ 402
MM_CREATIVE_AUX_MIC              equ 403
MM_CREATIVE_AUX_MASTER           equ 404
MM_CREATIVE_AUX_PCSPK            equ 405
MM_CREATIVE_AUX_WAVE             equ 406
MM_CREATIVE_AUX_MIDI             equ 407
MM_CREATIVE_SBPRO_MIXER          equ 408
MM_CREATIVE_SB16_MIXER           equ 409
MM_MEDIAVISION_PROAUDIO          equ 10h
MM_PROAUD_MIDIOUT                equ MM_MEDIAVISION_PROAUDIO+1
MM_PROAUD_MIDIIN                 equ MM_MEDIAVISION_PROAUDIO+2
MM_PROAUD_SYNTH                  equ MM_MEDIAVISION_PROAUDIO+3
MM_PROAUD_WAVEOUT                equ MM_MEDIAVISION_PROAUDIO+4
MM_PROAUD_WAVEIN                 equ MM_MEDIAVISION_PROAUDIO+5
MM_PROAUD_MIXER                  equ MM_MEDIAVISION_PROAUDIO+6
MM_PROAUD_AUX                    equ MM_MEDIAVISION_PROAUDIO+7
MM_MEDIAVISION_THUNDER           equ 20h
MM_THUNDER_SYNTH                 equ MM_MEDIAVISION_THUNDER+3
MM_THUNDER_WAVEOUT               equ MM_MEDIAVISION_THUNDER+4
MM_THUNDER_WAVEIN                equ MM_MEDIAVISION_THUNDER+5
MM_THUNDER_AUX                   equ MM_MEDIAVISION_THUNDER+7
MM_MEDIAVISION_TPORT             equ 40h
MM_TPORT_WAVEOUT                 equ MM_MEDIAVISION_TPORT+1
MM_TPORT_WAVEIN                  equ MM_MEDIAVISION_TPORT+2
MM_TPORT_SYNTH                   equ MM_MEDIAVISION_TPORT+3
MM_MEDIAVISION_PROAUDIO_PLUS     equ 50h
MM_PROAUD_PLUS_MIDIOUT           equ MM_MEDIAVISION_PROAUDIO_PLUS+1
MM_PROAUD_PLUS_MIDIIN            equ MM_MEDIAVISION_PROAUDIO_PLUS+2
MM_PROAUD_PLUS_SYNTH             equ MM_MEDIAVISION_PROAUDIO_PLUS+3
MM_PROAUD_PLUS_WAVEOUT           equ MM_MEDIAVISION_PROAUDIO_PLUS+4
MM_PROAUD_PLUS_WAVEIN            equ MM_MEDIAVISION_PROAUDIO_PLUS+5
MM_PROAUD_PLUS_MIXER             equ MM_MEDIAVISION_PROAUDIO_PLUS+6
MM_PROAUD_PLUS_AUX               equ MM_MEDIAVISION_PROAUDIO_PLUS+7
MM_MEDIAVISION_PROAUDIO_16       equ 60h
MM_PROAUD_16_MIDIOUT             equ MM_MEDIAVISION_PROAUDIO_16+1
MM_PROAUD_16_MIDIIN              equ MM_MEDIAVISION_PROAUDIO_16+2
MM_PROAUD_16_SYNTH               equ MM_MEDIAVISION_PROAUDIO_16+3
MM_PROAUD_16_WAVEOUT             equ MM_MEDIAVISION_PROAUDIO_16+4
MM_PROAUD_16_WAVEIN              equ MM_MEDIAVISION_PROAUDIO_16+5
MM_PROAUD_16_MIXER               equ MM_MEDIAVISION_PROAUDIO_16+6
MM_PROAUD_16_AUX                 equ MM_MEDIAVISION_PROAUDIO_16+7
MM_MEDIAVISION_PROSTUDIO_16      equ 60h
MM_STUDIO_16_MIDIOUT             equ MM_MEDIAVISION_PROSTUDIO_16+1
MM_STUDIO_16_MIDIIN              equ MM_MEDIAVISION_PROSTUDIO_16+2
MM_STUDIO_16_SYNTH               equ MM_MEDIAVISION_PROSTUDIO_16+3
MM_STUDIO_16_WAVEOUT             equ MM_MEDIAVISION_PROSTUDIO_16+4
MM_STUDIO_16_WAVEIN              equ MM_MEDIAVISION_PROSTUDIO_16+5
MM_STUDIO_16_MIXER               equ MM_MEDIAVISION_PROSTUDIO_16+6
MM_STUDIO_16_AUX                 equ MM_MEDIAVISION_PROSTUDIO_16+7
MM_MEDIAVISION_CDPC              equ 70h
MM_CDPC_MIDIOUT                  equ MM_MEDIAVISION_CDPC+1
MM_CDPC_MIDIIN                   equ MM_MEDIAVISION_CDPC+2
MM_CDPC_SYNTH                    equ MM_MEDIAVISION_CDPC+3
MM_CDPC_WAVEOUT                  equ MM_MEDIAVISION_CDPC+4
MM_CDPC_WAVEIN                   equ MM_MEDIAVISION_CDPC+5
MM_CDPC_MIXER                    equ MM_MEDIAVISION_CDPC+6
MM_CDPC_AUX                      equ MM_MEDIAVISION_CDPC+7
MM_MEDIAVISION_OPUS1208          equ 80h
MM_OPUS401_MIDIOUT               equ MM_MEDIAVISION_OPUS1208+1
MM_OPUS401_MIDIIN                equ MM_MEDIAVISION_OPUS1208+2
MM_OPUS1208_SYNTH                equ MM_MEDIAVISION_OPUS1208+3
MM_OPUS1208_WAVEOUT              equ MM_MEDIAVISION_OPUS1208+4
MM_OPUS1208_WAVEIN               equ MM_MEDIAVISION_OPUS1208+5
MM_OPUS1208_MIXER                equ MM_MEDIAVISION_OPUS1208+6
MM_OPUS1208_AUX                  equ MM_MEDIAVISION_OPUS1208+7
MM_MEDIAVISION_OPUS1216          equ 90h
MM_OPUS1216_MIDIOUT              equ MM_MEDIAVISION_OPUS1216+1
MM_OPUS1216_MIDIIN               equ MM_MEDIAVISION_OPUS1216+2
MM_OPUS1216_SYNTH                equ MM_MEDIAVISION_OPUS1216+3
MM_OPUS1216_WAVEOUT              equ MM_MEDIAVISION_OPUS1216+4
MM_OPUS1216_WAVEIN               equ MM_MEDIAVISION_OPUS1216+5
MM_OPUS1216_MIXER                equ MM_MEDIAVISION_OPUS1216+6
MM_OPUS1216_AUX                  equ MM_MEDIAVISION_OPUS1216+7
MM_CYRIX_XASYNTH                 equ 1
MM_CYRIX_XAMIDIIN                equ 2
MM_CYRIX_XAMIDIOUT               equ 3
MM_CYRIX_XAWAVEIN                equ 4
MM_CYRIX_XAWAVEOUT               equ 5
MM_CYRIX_XAAUX                   equ 6
MM_CYRIX_XAMIXER                 equ 7
MM_PHILIPS_ACM_LPCBB             equ 1
MM_NETXL_XLVIDEO                 equ 1
MM_ZYXEL_ACM_ADPCM               equ 1
MM_AARDVARK_STUDIO12_WAVEOUT     equ 1
MM_AARDVARK_STUDIO12_WAVEIN      equ 2
MM_AARDVARK_STUDIO88_WAVEOUT     equ 3
MM_AARDVARK_STUDIO88_WAVEIN      equ 4
MM_BINTEC_TAPI_WAVE              equ 1
MM_HEWLETT_PACKARD_CU_CODEC      equ 1
MM_MITEL_TALKTO_LINE_WAVEOUT     equ 100
MM_MITEL_TALKTO_LINE_WAVEIN      equ 101
MM_MITEL_TALKTO_HANDSET_WAVEOUT  equ 102
MM_MITEL_TALKTO_HANDSET_WAVEIN   equ 103
MM_MITEL_TALKTO_BRIDGED_WAVEOUT  equ 104
MM_MITEL_TALKTO_BRIDGED_WAVEIN   equ 105
MM_MITEL_MPA_HANDSET_WAVEOUT     equ 200
MM_MITEL_MPA_HANDSET_WAVEIN      equ 201
MM_MITEL_MPA_HANDSFREE_WAVEOUT   equ 202
MM_MITEL_MPA_HANDSFREE_WAVEIN    equ 203
MM_MITEL_MPA_LINE1_WAVEOUT       equ 204
MM_MITEL_MPA_LINE1_WAVEIN        equ 205
MM_MITEL_MPA_LINE2_WAVEOUT       equ 206
MM_MITEL_MPA_LINE2_WAVEIN        equ 207
MM_MITEL_MEDIAPATH_WAVEOUT       equ 300
MM_MITEL_MEDIAPATH_WAVEIN        equ 301
MM_SNI_ACM_G721                  equ 1
MM_EMU_APSSYNTH                  equ 1
MM_EMU_APSMIDIIN                 equ 2
MM_EMU_APSMIDIOUT                equ 3
MM_EMU_APSWAVEIN                 equ 4
MM_EMU_APSWAVEOUT                equ 5
MM_ARTISOFT_SBWAVEIN             equ 1
MM_ARTISOFT_SBWAVEOUT            equ 2
MM_TBS_TROPEZ_WAVEIN             equ 37
MM_TBS_TROPEZ_WAVEOUT            equ 38
MM_TBS_TROPEZ_AUX1               equ 39
MM_TBS_TROPEZ_AUX2               equ 40
MM_TBS_TROPEZ_LINE               equ 41
MM_MMOTION_WAVEAUX               equ 1
MM_MMOTION_WAVEOUT               equ 2
MM_MMOTION_WAVEIN                equ 3
MM_IBM_PCMCIA_WAVEIN             equ 11
MM_IBM_PCMCIA_WAVEOUT            equ 12
MM_IBM_PCMCIA_SYNTH              equ 13
MM_IBM_PCMCIA_MIDIIN             equ 14
MM_IBM_PCMCIA_MIDIOUT            equ 15
MM_IBM_PCMCIA_AUX                equ 16
MM_IBM_THINKPAD200               equ 17
MM_IBM_MWAVE_WAVEIN              equ 18
MM_IBM_MWAVE_WAVEOUT             equ 19
MM_IBM_MWAVE_MIXER               equ 20
MM_IBM_MWAVE_MIDIIN              equ 21
MM_IBM_MWAVE_MIDIOUT             equ 22
MM_IBM_MWAVE_AUX                 equ 23
MM_IBM_WC_MIDIOUT                equ 30
MM_IBM_WC_WAVEOUT                equ 31
MM_IBM_WC_MIXEROUT               equ 33
MM_VOCALTEC_WAVEOUT              equ 1
MM_VOCALTEC_WAVEIN               equ 2
MM_ROLAND_RAP10_MIDIOUT          equ 10
MM_ROLAND_RAP10_MIDIIN           equ 11
MM_ROLAND_RAP10_SYNTH            equ 12
MM_ROLAND_RAP10_WAVEOUT          equ 13
MM_ROLAND_RAP10_WAVEIN           equ 14
MM_ROLAND_MPU401_MIDIOUT         equ 15
MM_ROLAND_MPU401_MIDIIN          equ 16
MM_ROLAND_SMPU_MIDIOUTA          equ 17
MM_ROLAND_SMPU_MIDIOUTB          equ 18
MM_ROLAND_SMPU_MIDIINA           equ 19
MM_ROLAND_SMPU_MIDIINB           equ 20
MM_ROLAND_SC7_MIDIOUT            equ 21
MM_ROLAND_SC7_MIDIIN             equ 22
MM_ROLAND_SERIAL_MIDIOUT         equ 23
MM_ROLAND_SERIAL_MIDIIN          equ 24
MM_ROLAND_SCP_MIDIOUT            equ 38
MM_ROLAND_SCP_MIDIIN             equ 39
MM_ROLAND_SCP_WAVEOUT            equ 40
MM_ROLAND_SCP_WAVEIN             equ 41
MM_ROLAND_SCP_MIXER              equ 42
MM_ROLAND_SCP_AUX                equ 48
MM_DSP_SOLUTIONS_WAVEOUT         equ 1
MM_DSP_SOLUTIONS_WAVEIN          equ 2
MM_DSP_SOLUTIONS_SYNTH           equ 3
MM_DSP_SOLUTIONS_AUX             equ 4
MM_NEC_73_86_SYNTH               equ 5
MM_NEC_73_86_WAVEOUT             equ 6
MM_NEC_73_86_WAVEIN              equ 7
MM_NEC_26_SYNTH                  equ 9
MM_NEC_MPU401_MIDIOUT            equ 10
MM_NEC_MPU401_MIDIIN             equ 11
MM_NEC_JOYSTICK                  equ 12
MM_WANGLABS_WAVEIN1              equ 1
MM_WANGLABS_WAVEOUT1             equ 2
MM_TANDY_VISWAVEIN               equ 1
MM_TANDY_VISWAVEOUT              equ 2
MM_TANDY_VISBIOSSYNTH            equ 3
MM_TANDY_SENS_MMAWAVEIN          equ 4
MM_TANDY_SENS_MMAWAVEOUT         equ 5
MM_TANDY_SENS_MMAMIDIIN          equ 6
MM_TANDY_SENS_MMAMIDIOUT         equ 7
MM_TANDY_SENS_VISWAVEOUT         equ 8
MM_TANDY_PSSJWAVEIN              equ 9
MM_TANDY_PSSJWAVEOUT             equ 10
MM_ANTEX_SX12_WAVEIN             equ 1
MM_ANTEX_SX12_WAVEOUT            equ 2
MM_ANTEX_SX15_WAVEIN             equ 3
MM_ANTEX_SX15_WAVEOUT            equ 4
MM_ANTEX_VP625_WAVEIN            equ 5
MM_ANTEX_VP625_WAVEOUT           equ 6
MM_ANTEX_AUDIOPORT22_WAVEIN      equ 7
MM_ANTEX_AUDIOPORT22_WAVEOUT     equ 8
MM_ANTEX_AUDIOPORT22_FEEDTHRU    equ 9
MM_INTELOPD_WAVEIN               equ 1
MM_INTELOPD_WAVEOUT              equ 101
MM_INTELOPD_AUX                  equ 401
MM_INTEL_NSPMODEMLINEIN          equ 501
MM_INTEL_NSPMODEMLINEOUT         equ 502
MM_VAL_MICROKEY_AP_WAVEIN        equ 1
MM_VAL_MICROKEY_AP_WAVEOUT       equ 2
MM_INTERACTIVE_WAVEIN            equ 45h
MM_INTERACTIVE_WAVEOUT           equ 45h
MM_YAMAHA_GSS_SYNTH              equ 01h
MM_YAMAHA_GSS_WAVEOUT            equ 02h
MM_YAMAHA_GSS_WAVEIN             equ 03h
MM_YAMAHA_GSS_MIDIOUT            equ 04h
MM_YAMAHA_GSS_MIDIIN             equ 05h
MM_YAMAHA_GSS_AUX                equ 06h
MM_YAMAHA_SERIAL_MIDIOUT         equ 07h
MM_YAMAHA_SERIAL_MIDIIN          equ 08h
MM_YAMAHA_OPL3SA_WAVEOUT         equ 10h
MM_YAMAHA_OPL3SA_WAVEIN          equ 11h
MM_YAMAHA_OPL3SA_FMSYNTH         equ 12h
MM_YAMAHA_OPL3SA_YSYNTH          equ 13h
MM_YAMAHA_OPL3SA_MIDIOUT         equ 14h
MM_YAMAHA_OPL3SA_MIDIIN          equ 15h
MM_YAMAHA_OPL3SA_MIXER           equ 17h
MM_YAMAHA_OPL3SA_JOYSTICK        equ 18h
MM_YAMAHA_YMF724LEG_MIDIOUT      equ 19h
MM_YAMAHA_YMF724LEG_MIDIIN       equ 1Ah
MM_YAMAHA_YMF724_WAVEOUT         equ 1Bh
MM_YAMAHA_YMF724_WAVEIN          equ 1Ch
MM_YAMAHA_YMF724_MIDIOUT         equ 1Dh
MM_YAMAHA_YMF724_AUX             equ 1Eh
MM_YAMAHA_YMF724_MIXER           equ 1Fh
MM_YAMAHA_YMF724LEG_FMSYNTH      equ 20h
MM_YAMAHA_YMF724LEG_MIXER        equ 21h
MM_YAMAHA_SXG_MIDIOUT            equ 22h
MM_YAMAHA_SXG_WAVEOUT            equ 23h
MM_YAMAHA_SXG_MIXER              equ 24h
MM_YAMAHA_ACXG_WAVEIN            equ 25h
MM_YAMAHA_ACXG_WAVEOUT           equ 26h
MM_YAMAHA_ACXG_MIDIOUT           equ 27h
MM_YAMAHA_ACXG_MIXER             equ 28h
MM_YAMAHA_ACXG_AUX               equ 29h
MM_EVEREX_CARRIER                equ 1
MM_ECHO_SYNTH                    equ 1
MM_ECHO_WAVEOUT                  equ 2
MM_ECHO_WAVEIN                   equ 3
MM_ECHO_MIDIOUT                  equ 4
MM_ECHO_MIDIIN                   equ 5
MM_ECHO_AUX                      equ 6
MM_SIERRA_ARIA_MIDIOUT           equ 14h
MM_SIERRA_ARIA_MIDIIN            equ 15h
MM_SIERRA_ARIA_SYNTH             equ 16h
MM_SIERRA_ARIA_WAVEOUT           equ 17h
MM_SIERRA_ARIA_WAVEIN            equ 18h
MM_SIERRA_ARIA_AUX               equ 19h
MM_SIERRA_ARIA_AUX2              equ 20h
MM_SIERRA_QUARTET_WAVEIN         equ 50h
MM_SIERRA_QUARTET_WAVEOUT        equ 51h
MM_SIERRA_QUARTET_MIDIIN         equ 52h
MM_SIERRA_QUARTET_MIDIOUT        equ 53h
MM_SIERRA_QUARTET_SYNTH          equ 54h
MM_SIERRA_QUARTET_AUX_CD         equ 55h
MM_SIERRA_QUARTET_AUX_LINE       equ 56h
MM_SIERRA_QUARTET_AUX_MODEM      equ 57h
MM_SIERRA_QUARTET_MIXER          equ 58h
MM_CAT_WAVEOUT                   equ 1
MM_DSP_GROUP_TRUESPEECH          equ 1
MM_MELABS_MIDI2GO                equ 1
MM_ESS_AMWAVEOUT                 equ 01h
MM_ESS_AMWAVEIN                  equ 02h
MM_ESS_AMAUX                     equ 03h
MM_ESS_AMSYNTH                   equ 04h
MM_ESS_AMMIDIOUT                 equ 05h
MM_ESS_AMMIDIIN                  equ 06h
MM_ESS_MIXER                     equ 07h
MM_ESS_AUX_CD                    equ 08h
MM_ESS_MPU401_MIDIOUT            equ 09h
MM_ESS_MPU401_MIDIIN             equ 0Ah
MM_ESS_ES488_WAVEOUT             equ 10h
MM_ESS_ES488_WAVEIN              equ 11h
MM_ESS_ES488_MIXER               equ 12h
MM_ESS_ES688_WAVEOUT             equ 13h
MM_ESS_ES688_WAVEIN              equ 14h
MM_ESS_ES688_MIXER               equ 15h
MM_ESS_ES1488_WAVEOUT            equ 16h
MM_ESS_ES1488_WAVEIN             equ 17h
MM_ESS_ES1488_MIXER              equ 18h
MM_ESS_ES1688_WAVEOUT            equ 19h
MM_ESS_ES1688_WAVEIN             equ 1Ah
MM_ESS_ES1688_MIXER              equ 1Bh
MM_ESS_ES1788_WAVEOUT            equ 1Ch
MM_ESS_ES1788_WAVEIN             equ 1Dh
MM_ESS_ES1788_MIXER              equ 1Eh
MM_ESS_ES1888_WAVEOUT            equ 1Fh
MM_ESS_ES1888_WAVEIN             equ 20h
MM_ESS_ES1888_MIXER              equ 21h
MM_ESS_ES1868_WAVEOUT            equ 22h
MM_ESS_ES1868_WAVEIN             equ 23h
MM_ESS_ES1868_MIXER              equ 24h
MM_ESS_ES1878_WAVEOUT            equ 25h
MM_ESS_ES1878_WAVEIN             equ 26h
MM_ESS_ES1878_MIXER              equ 27h
MM_CANOPUS_ACM_DVREX             equ 1
MM_EPS_FMSND                     equ 1
MM_TRUEVISION_WAVEIN1            equ 1
MM_TRUEVISION_WAVEOUT1           equ 2
MM_AZTECH_MIDIOUT                equ 3
MM_AZTECH_MIDIIN                 equ 4
MM_AZTECH_WAVEIN                 equ 17
MM_AZTECH_WAVEOUT                equ 18
MM_AZTECH_FMSYNTH                equ 20
MM_AZTECH_MIXER                  equ 21
MM_AZTECH_PRO16_WAVEIN           equ 33
MM_AZTECH_PRO16_WAVEOUT          equ 34
MM_AZTECH_PRO16_FMSYNTH          equ 38
MM_AZTECH_DSP16_WAVEIN           equ 65
MM_AZTECH_DSP16_WAVEOUT          equ 66
MM_AZTECH_DSP16_FMSYNTH          equ 68
MM_AZTECH_DSP16_WAVESYNTH        equ 70
MM_AZTECH_NOVA16_WAVEIN          equ 71
MM_AZTECH_NOVA16_WAVEOUT         equ 72
MM_AZTECH_NOVA16_MIXER           equ 73
MM_AZTECH_WASH16_WAVEIN          equ 74
MM_AZTECH_WASH16_WAVEOUT         equ 75
MM_AZTECH_WASH16_MIXER           equ 76
MM_AZTECH_AUX_CD                 equ 401
MM_AZTECH_AUX_LINE               equ 402
MM_AZTECH_AUX_MIC                equ 403
MM_AZTECH_AUX                    equ 404
MM_VIDEOLOGIC_MSWAVEIN           equ 1
MM_VIDEOLOGIC_MSWAVEOUT          equ 2
MM_KORG_PCIF_MIDIOUT             equ 1
MM_KORG_PCIF_MIDIIN              equ 2
MM_KORG_1212IO_MSWAVEIN          equ 3
MM_KORG_1212IO_MSWAVEOUT         equ 4
MM_APT_ACE100CD                  equ 1
MM_ICS_WAVEDECK_WAVEOUT          equ 1
MM_ICS_WAVEDECK_WAVEIN           equ 2
MM_ICS_WAVEDECK_MIXER            equ 3
MM_ICS_WAVEDECK_AUX              equ 4
MM_ICS_WAVEDECK_SYNTH            equ 5
MM_ICS_WAVEDEC_SB_WAVEOUT        equ 6
MM_ICS_WAVEDEC_SB_WAVEIN         equ 7
MM_ICS_WAVEDEC_SB_FM_MIDIOUT     equ 8
MM_ICS_WAVEDEC_SB_MPU401_MIDIOUT equ 9
MM_ICS_WAVEDEC_SB_MPU401_MIDIIN  equ 10
MM_ICS_WAVEDEC_SB_MIXER          equ 11
MM_ICS_WAVEDEC_SB_AUX            equ 12
MM_ICS_2115_LITE_MIDIOUT         equ 13
MM_ICS_2120_LITE_MIDIOUT         equ 14
MM_ITERATEDSYS_FUFCODEC          equ 1
MM_METHEUS_ZIPPER                equ 1
MM_WINNOV_CAVIAR_WAVEIN          equ 1
MM_WINNOV_CAVIAR_WAVEOUT         equ 2
MM_WINNOV_CAVIAR_VIDC            equ 3
MM_WINNOV_CAVIAR_CHAMPAGNE       equ 4
MM_WINNOV_CAVIAR_YUV8            equ 5
MM_NCR_BA_WAVEIN                 equ 1
MM_NCR_BA_WAVEOUT                equ 2
MM_NCR_BA_SYNTH                  equ 3
MM_NCR_BA_AUX                    equ 4
MM_NCR_BA_MIXER                  equ 5
MM_AST_MODEMWAVE_WAVEIN          equ 13
MM_AST_MODEMWAVE_WAVEOUT         equ 14
MM_WILLOWPOND_FMSYNTH_STEREO     equ 20
MM_WILLOWPOND_MPU401             equ 21
MM_WILLOWPOND_SNDPORT_WAVEIN     equ 100
MM_WILLOWPOND_SNDPORT_WAVEOUT    equ 101
MM_WILLOWPOND_SNDPORT_MIXER      equ 102
MM_WILLOWPOND_SNDPORT_AUX        equ 103
MM_WILLOWPOND_PH_WAVEIN          equ 104
MM_WILLOWPOND_PH_WAVEOUT         equ 105
MM_WILLOWPOND_PH_MIXER           equ 106
MM_WILLOWPOND_PH_AUX             equ 107
MM_WILLOPOND_SNDCOMM_WAVEIN      equ 108
MM_WILLOWPOND_SNDCOMM_WAVEOUT    equ 109
MM_WILLOWPOND_SNDCOMM_MIXER      equ 110
MM_WILLOWPOND_SNDCOMM_AUX        equ 111
MM_WILLOWPOND_GENERIC_WAVEIN     equ 112
MM_WILLOWPOND_GENERIC_WAVEOUT    equ 113
MM_WILLOWPOND_GENERIC_MIXER      equ 114
MM_WILLOWPOND_GENERIC_AUX        equ 115
MM_VITEC_VMAKER                  equ 1
MM_VITEC_VMPRO                   equ 2
MM_MOSCOM_VPC2400_IN             equ 1
MM_MOSCOM_VPC2400_OUT            equ 2
MM_SILICONSOFT_SC1_WAVEIN        equ 1
MM_SILICONSOFT_SC1_WAVEOUT       equ 2
MM_SILICONSOFT_SC2_WAVEIN        equ 3
MM_SILICONSOFT_SC2_WAVEOUT       equ 4
MM_SILICONSOFT_SOUNDJR2_WAVEOUT  equ 5
MM_SILICONSOFT_SOUNDJR2PR_WAVEIN equ 6
MM_SILICONSOFT_SOUNDJR2PR_WAVEOUT equ 7
MM_SILICONSOFT_SOUNDJR3_WAVEOUT  equ 8
MM_TTEWS_WAVEIN                  equ 1
MM_TTEWS_WAVEOUT                 equ 2
MM_TTEWS_MIDIIN                  equ 3
MM_TTEWS_MIDIOUT                 equ 4
MM_TTEWS_MIDISYNTH               equ 5
MM_TTEWS_MIDIMONITOR             equ 6
MM_TTEWS_VMIDIIN                 equ 7
MM_TTEWS_VMIDIOUT                equ 8
MM_TTEWS_AUX                     equ 9
MM_TTEWS_MIXER                   equ 10
MM_MEDIASONIC_ACM_G723           equ 1
MM_MEDIASONIC_ICOM               equ 2
MM_ICOM_WAVEIN                   equ 3
MM_ICOM_WAVEOUT                  equ 4
MM_ICOM_MIXER                    equ 5
MM_ICOM_AUX                      equ 6
MM_ICOM_LINE                     equ 7
MM_SANYO_ACM_LD_ADPCM            equ 1
MM_AHEAD_MULTISOUND              equ 1
MM_AHEAD_SOUNDBLASTER            equ 2
MM_AHEAD_PROAUDIO                equ 3
MM_AHEAD_GENERIC                 equ 4
MM_OLIVETTI_WAVEIN               equ 1
MM_OLIVETTI_WAVEOUT              equ 2
MM_OLIVETTI_MIXER                equ 3
MM_OLIVETTI_AUX                  equ 4
MM_OLIVETTI_MIDIIN               equ 5
MM_OLIVETTI_MIDIOUT              equ 6
MM_OLIVETTI_SYNTH                equ 7
MM_OLIVETTI_JOYSTICK             equ 8
MM_OLIVETTI_ACM_GSM              equ 9
MM_OLIVETTI_ACM_ADPCM            equ 10
MM_OLIVETTI_ACM_CELP             equ 11
MM_OLIVETTI_ACM_SBC              equ 12
MM_OLIVETTI_ACM_OPR              equ 13
MM_IOMAGIC_TEMPO_WAVEOUT         equ 1
MM_IOMAGIC_TEMPO_WAVEIN          equ 2
MM_IOMAGIC_TEMPO_SYNTH           equ 3
MM_IOMAGIC_TEMPO_MIDIOUT         equ 4
MM_IOMAGIC_TEMPO_MXDOUT          equ 5
MM_IOMAGIC_TEMPO_AUXOUT          equ 6
MM_MATSUSHITA_WAVEIN             equ 1
MM_MATSUSHITA_WAVEOUT            equ 2
MM_MATSUSHITA_FMSYNTH_STEREO     equ 3
MM_MATSUSHITA_MIXER              equ 4
MM_MATSUSHITA_AUX                equ 5
MM_NEWMEDIA_WAVJAMMER            equ 1
MM_LYRRUS_BRIDGE_GUITAR          equ 1
MM_OPTI_M16_FMSYNTH_STEREO       equ 0001h
MM_OPTI_M16_MIDIIN               equ 0002h
MM_OPTI_M16_MIDIOUT              equ 0003h
MM_OPTI_M16_WAVEIN               equ 0004h
MM_OPTI_M16_WAVEOUT              equ 0005h
MM_OPTI_M16_MIXER                equ 0006h
MM_OPTI_M16_AUX                  equ 0007h
MM_OPTI_P16_FMSYNTH_STEREO       equ 0010h
MM_OPTI_P16_MIDIIN               equ 0011h
MM_OPTI_P16_MIDIOUT              equ 0012h
MM_OPTI_P16_WAVEIN               equ 0013h
MM_OPTI_P16_WAVEOUT              equ 0014h
MM_OPTI_P16_MIXER                equ 0015h
MM_OPTI_P16_AUX                  equ 0016h
MM_OPTI_M32_WAVEIN               equ 0020h
MM_OPTI_M32_WAVEOUT              equ 0021h
MM_OPTI_M32_MIDIIN               equ 0022h
MM_OPTI_M32_MIDIOUT              equ 0023h
MM_OPTI_M32_SYNTH_STEREO         equ 0024h
MM_OPTI_M32_MIXER                equ 0025h
MM_OPTI_M32_AUX                  equ 0026h
MM_COMPAQ_BB_WAVEIN              equ 1
MM_COMPAQ_BB_WAVEOUT             equ 2
MM_COMPAQ_BB_WAVEAUX             equ 3
MM_MPTUS_SPWAVEOUT               equ 1
MM_LERNOUT_ANDHAUSPIE_LHCODECACM equ 1
MM_DIGITAL_AV320_WAVEIN          equ 1
MM_DIGITAL_AV320_WAVEOUT         equ 2
MM_DIGITAL_ACM_G723              equ 3
MM_DIGITAL_ICM_H263              equ 4
MM_DIGITAL_ICM_H261              equ 5
MM_MOTU_MTP_MIDIOUT_ALL          equ 100
MM_MOTU_MTP_MIDIIN_1             equ 101
MM_MOTU_MTP_MIDIOUT_1            equ 101
MM_MOTU_MTP_MIDIIN_2             equ 102
MM_MOTU_MTP_MIDIOUT_2            equ 102
MM_MOTU_MTP_MIDIIN_3             equ 103
MM_MOTU_MTP_MIDIOUT_3            equ 103
MM_MOTU_MTP_MIDIIN_4             equ 104
MM_MOTU_MTP_MIDIOUT_4            equ 104
MM_MOTU_MTP_MIDIIN_5             equ 105
MM_MOTU_MTP_MIDIOUT_5            equ 105
MM_MOTU_MTP_MIDIIN_6             equ 106
MM_MOTU_MTP_MIDIOUT_6            equ 106
MM_MOTU_MTP_MIDIIN_7             equ 107
MM_MOTU_MTP_MIDIOUT_7            equ 107
MM_MOTU_MTP_MIDIIN_8             equ 108
MM_MOTU_MTP_MIDIOUT_8            equ 108
MM_MOTU_MTPII_MIDIOUT_ALL        equ 200
MM_MOTU_MTPII_MIDIIN_SYNC        equ 200
MM_MOTU_MTPII_MIDIIN_1           equ 201
MM_MOTU_MTPII_MIDIOUT_1          equ 201
MM_MOTU_MTPII_MIDIIN_2           equ 202
MM_MOTU_MTPII_MIDIOUT_2          equ 202
MM_MOTU_MTPII_MIDIIN_3           equ 203
MM_MOTU_MTPII_MIDIOUT_3          equ 203
MM_MOTU_MTPII_MIDIIN_4           equ 204
MM_MOTU_MTPII_MIDIOUT_4          equ 204
MM_MOTU_MTPII_MIDIIN_5           equ 205
MM_MOTU_MTPII_MIDIOUT_5          equ 205
MM_MOTU_MTPII_MIDIIN_6           equ 206
MM_MOTU_MTPII_MIDIOUT_6          equ 206
MM_MOTU_MTPII_MIDIIN_7           equ 207
MM_MOTU_MTPII_MIDIOUT_7          equ 207
MM_MOTU_MTPII_MIDIIN_8           equ 208
MM_MOTU_MTPII_MIDIOUT_8          equ 208
MM_MOTU_MTPII_NET_MIDIIN_1       equ 209
MM_MOTU_MTPII_NET_MIDIOUT_1      equ 209
MM_MOTU_MTPII_NET_MIDIIN_2       equ 210
MM_MOTU_MTPII_NET_MIDIOUT_2      equ 210
MM_MOTU_MTPII_NET_MIDIIN_3       equ 211
MM_MOTU_MTPII_NET_MIDIOUT_3      equ 211
MM_MOTU_MTPII_NET_MIDIIN_4       equ 212
MM_MOTU_MTPII_NET_MIDIOUT_4      equ 212
MM_MOTU_MTPII_NET_MIDIIN_5       equ 213
MM_MOTU_MTPII_NET_MIDIOUT_5      equ 213
MM_MOTU_MTPII_NET_MIDIIN_6       equ 214
MM_MOTU_MTPII_NET_MIDIOUT_6      equ 214
MM_MOTU_MTPII_NET_MIDIIN_7       equ 215
MM_MOTU_MTPII_NET_MIDIOUT_7      equ 215
MM_MOTU_MTPII_NET_MIDIIN_8       equ 216
MM_MOTU_MTPII_NET_MIDIOUT_8      equ 216
MM_MOTU_MXP_MIDIIN_MIDIOUT_ALL   equ 300
MM_MOTU_MXP_MIDIIN_SYNC          equ 300
MM_MOTU_MXP_MIDIIN_MIDIIN_1      equ 301
MM_MOTU_MXP_MIDIIN_MIDIOUT_1     equ 301
MM_MOTU_MXP_MIDIIN_MIDIIN_2      equ 302
MM_MOTU_MXP_MIDIIN_MIDIOUT_2     equ 302
MM_MOTU_MXP_MIDIIN_MIDIIN_3      equ 303
MM_MOTU_MXP_MIDIIN_MIDIOUT_3     equ 303
MM_MOTU_MXP_MIDIIN_MIDIIN_4      equ 304
MM_MOTU_MXP_MIDIIN_MIDIOUT_4     equ 304
MM_MOTU_MXP_MIDIIN_MIDIIN_5      equ 305
MM_MOTU_MXP_MIDIIN_MIDIOUT_5     equ 305
MM_MOTU_MXP_MIDIIN_MIDIIN_6      equ 306
MM_MOTU_MXP_MIDIIN_MIDIOUT_6     equ 306
MM_MOTU_MXPMPU_MIDIOUT_ALL       equ 400
MM_MOTU_MXPMPU_MIDIIN_SYNC       equ 400
MM_MOTU_MXPMPU_MIDIIN_1          equ 401
MM_MOTU_MXPMPU_MIDIOUT_1         equ 401
MM_MOTU_MXPMPU_MIDIIN_2          equ 402
MM_MOTU_MXPMPU_MIDIOUT_2         equ 402
MM_MOTU_MXPMPU_MIDIIN_3          equ 403
MM_MOTU_MXPMPU_MIDIOUT_3         equ 403
MM_MOTU_MXPMPU_MIDIIN_4          equ 404
MM_MOTU_MXPMPU_MIDIOUT_4         equ 404
MM_MOTU_MXPMPU_MIDIIN_5          equ 405
MM_MOTU_MXPMPU_MIDIOUT_5         equ 405
MM_MOTU_MXPMPU_MIDIIN_6          equ 406
MM_MOTU_MXPMPU_MIDIOUT_6         equ 406
MM_MOTU_MXN_MIDIOUT_ALL          equ 500
MM_MOTU_MXN_MIDIIN_SYNC          equ 500
MM_MOTU_MXN_MIDIIN_1             equ 501
MM_MOTU_MXN_MIDIOUT_1            equ 501
MM_MOTU_MXN_MIDIIN_2             equ 502
MM_MOTU_MXN_MIDIOUT_2            equ 502
MM_MOTU_MXN_MIDIIN_3             equ 503
MM_MOTU_MXN_MIDIOUT_3            equ 503
MM_MOTU_MXN_MIDIIN_4             equ 504
MM_MOTU_MXN_MIDIOUT_4            equ 504
MM_MOTU_FLYER_MIDI_IN_SYNC       equ 600
MM_MOTU_FLYER_MIDI_IN_A          equ 601
MM_MOTU_FLYER_MIDI_OUT_A         equ 601
MM_MOTU_FLYER_MIDI_IN_B          equ 602
MM_MOTU_FLYER_MIDI_OUT_B         equ 602
MM_MOTU_PKX_MIDI_IN_SYNC         equ 700
MM_MOTU_PKX_MIDI_IN_A            equ 701
MM_MOTU_PKX_MIDI_OUT_A           equ 701
MM_MOTU_PKX_MIDI_IN_B            equ 702
MM_MOTU_PKX_MIDI_OUT_B           equ 702
MM_MOTU_DTX_MIDI_IN_SYNC         equ 800
MM_MOTU_DTX_MIDI_IN_A            equ 801
MM_MOTU_DTX_MIDI_OUT_A           equ 801
MM_MOTU_DTX_MIDI_IN_B            equ 802
MM_MOTU_DTX_MIDI_OUT_B           equ 802
MM_MOTU_MTPAV_MIDIOUT_ALL        equ 900
MM_MOTU_MTPAV_MIDIIN_SYNC        equ 900
MM_MOTU_MTPAV_MIDIIN_1           equ 901
MM_MOTU_MTPAV_MIDIOUT_1          equ 901
MM_MOTU_MTPAV_MIDIIN_2           equ 902
MM_MOTU_MTPAV_MIDIOUT_2          equ 902
MM_MOTU_MTPAV_MIDIIN_3           equ 903
MM_MOTU_MTPAV_MIDIOUT_3          equ 903
MM_MOTU_MTPAV_MIDIIN_4           equ 904
MM_MOTU_MTPAV_MIDIOUT_4          equ 904
MM_MOTU_MTPAV_MIDIIN_5           equ 905
MM_MOTU_MTPAV_MIDIOUT_5          equ 905
MM_MOTU_MTPAV_MIDIIN_6           equ 906
MM_MOTU_MTPAV_MIDIOUT_6          equ 906
MM_MOTU_MTPAV_MIDIIN_7           equ 907
MM_MOTU_MTPAV_MIDIOUT_7          equ 907
MM_MOTU_MTPAV_MIDIIN_8           equ 908
MM_MOTU_MTPAV_MIDIOUT_8          equ 908
MM_MOTU_MTPAV_NET_MIDIIN_1       equ 909
MM_MOTU_MTPAV_NET_MIDIOUT_1      equ 909
MM_MOTU_MTPAV_NET_MIDIIN_2       equ 910
MM_MOTU_MTPAV_NET_MIDIOUT_2      equ 910
MM_MOTU_MTPAV_NET_MIDIIN_3       equ 911
MM_MOTU_MTPAV_NET_MIDIOUT_3      equ 911
MM_MOTU_MTPAV_NET_MIDIIN_4       equ 912
MM_MOTU_MTPAV_NET_MIDIOUT_4      equ 912
MM_MOTU_MTPAV_NET_MIDIIN_5       equ 913
MM_MOTU_MTPAV_NET_MIDIOUT_5      equ 913
MM_MOTU_MTPAV_NET_MIDIIN_6       equ 914
MM_MOTU_MTPAV_NET_MIDIOUT_6      equ 914
MM_MOTU_MTPAV_NET_MIDIIN_7       equ 915
MM_MOTU_MTPAV_NET_MIDIOUT_7      equ 915
MM_MOTU_MTPAV_NET_MIDIIN_8       equ 916
MM_MOTU_MTPAV_NET_MIDIOUT_8      equ 916
MM_MOTU_MTPAV_MIDIIN_ADAT        equ 917
MM_MOTU_MTPAV_MIDIOUT_ADAT       equ 917
MM_MOTU_MXPXT_MIDIIN_SYNC        equ 1000
MM_MOTU_MXPXT_MIDIOUT_ALL        equ 1000
MM_MOTU_MXPXT_MIDIIN_1           equ 1001
MM_MOTU_MXPXT_MIDIOUT_1          equ 1001
MM_MOTU_MXPXT_MIDIOUT_2          equ 1002
MM_MOTU_MXPXT_MIDIIN_2           equ 1002
MM_MOTU_MXPXT_MIDIIN_3           equ 1003
MM_MOTU_MXPXT_MIDIOUT_3          equ 1003
MM_MOTU_MXPXT_MIDIIN_4           equ 1004
MM_MOTU_MXPXT_MIDIOUT_4          equ 1004
MM_MOTU_MXPXT_MIDIIN_5           equ 1005
MM_MOTU_MXPXT_MIDIOUT_5          equ 1005
MM_MOTU_MXPXT_MIDIOUT_6          equ 1006
MM_MOTU_MXPXT_MIDIIN_6           equ 1006
MM_MOTU_MXPXT_MIDIOUT_7          equ 1007
MM_MOTU_MXPXT_MIDIIN_7           equ 1007
MM_MOTU_MXPXT_MIDIOUT_8          equ 1008
MM_MOTU_MXPXT_MIDIIN_8           equ 1008
MM_WORKBIT_MIXER                 equ 1
MM_WORKBIT_WAVEOUT               equ 2
MM_WORKBIT_WAVEIN                equ 3
MM_WORKBIT_MIDIIN                equ 4
MM_WORKBIT_MIDIOUT               equ 5
MM_WORKBIT_FMSYNTH               equ 6
MM_WORKBIT_AUX                   equ 7
MM_WORKBIT_JOYSTICK              equ 8
MM_OSITECH_TRUMPCARD             equ 1
MM_MIRO_MOVIEPRO                 equ 1
MM_MIRO_VIDEOD1                  equ 2
MM_MIRO_VIDEODC1TV               equ 3
MM_MIRO_VIDEOTD                  equ 4
MM_MIRO_DC30_WAVEOUT             equ 5
MM_MIRO_DC30_WAVEIN              equ 6
MM_MIRO_DC30_MIX                 equ 7
MM_ISOLUTION_PASCAL              equ 1
MM_VOICEMIXER                    equ 1
ROCKWELL_WA1_WAVEIN              equ 100
ROCKWELL_WA1_WAVEOUT             equ 101
ROCKWELL_WA1_SYNTH               equ 102
ROCKWELL_WA1_MIXER               equ 103
ROCKWELL_WA1_MPU401_IN           equ 104
ROCKWELL_WA1_MPU401_OUT          equ 105
ROCKWELL_WA2_WAVEIN              equ 200
ROCKWELL_WA2_WAVEOUT             equ 201
ROCKWELL_WA2_SYNTH               equ 202
ROCKWELL_WA2_MIXER               equ 203
ROCKWELL_WA2_MPU401_IN           equ 204
ROCKWELL_WA2_MPU401_OUT          equ 205
MM_VOXWARE_CODEC                 equ 1
MM_NORTEL_MPXAC_WAVEIN           equ 1
MM_NORTEL_MPXAC_WAVEOUT          equ 2
MM_ADDX_PCTV_DIGITALMIX          equ 1
MM_ADDX_PCTV_WAVEIN              equ 2
MM_ADDX_PCTV_WAVEOUT             equ 3
MM_ADDX_PCTV_MIXER               equ 4
MM_ADDX_PCTV_AUX_CD              equ 5
MM_ADDX_PCTV_AUX_LINE            equ 6
MM_WILDCAT_AUTOSCOREMIDIIN       equ 1
MM_RHETOREX_WAVEIN               equ 1
MM_RHETOREX_WAVEOUT              equ 2
MM_BTV_WAVEIN                    equ 1
MM_BTV_WAVEOUT                   equ 2
MM_BTV_MIDIIN                    equ 3
MM_BTV_MIDIOUT                   equ 4
MM_BTV_MIDISYNTH                 equ 5
MM_BTV_AUX_LINE                  equ 6
MM_BTV_AUX_MIC                   equ 7
MM_BTV_AUX_CD                    equ 8
MM_BTV_DIGITALIN                 equ 9
MM_BTV_DIGITALOUT                equ 10
MM_BTV_MIDIWAVESTREAM            equ 11
MM_BTV_MIXER                     equ 12
MM_ENSONIQ_SOUNDSCAPE            equ 10h
MM_SOUNDSCAPE_WAVEOUT            equ MM_ENSONIQ_SOUNDSCAPE+1
MM_SOUNDSCAPE_WAVEOUT_AUX        equ MM_ENSONIQ_SOUNDSCAPE+2
MM_SOUNDSCAPE_WAVEIN             equ MM_ENSONIQ_SOUNDSCAPE+3
MM_SOUNDSCAPE_MIDIOUT            equ MM_ENSONIQ_SOUNDSCAPE+4
MM_SOUNDSCAPE_MIDIIN             equ MM_ENSONIQ_SOUNDSCAPE+5
MM_SOUNDSCAPE_SYNTH              equ MM_ENSONIQ_SOUNDSCAPE+6
MM_SOUNDSCAPE_MIXER              equ MM_ENSONIQ_SOUNDSCAPE+7
MM_SOUNDSCAPE_AUX                equ MM_ENSONIQ_SOUNDSCAPE+8
MM_NVIDIA_WAVEOUT                equ 1
MM_NVIDIA_WAVEIN                 equ 2
MM_NVIDIA_MIDIOUT                equ 3
MM_NVIDIA_MIDIIN                 equ 4
MM_NVIDIA_GAMEPORT               equ 5
MM_NVIDIA_MIXER                  equ 6
MM_NVIDIA_AUX                    equ 7
MM_OKSORI_BASE                   equ 0
MM_OKSORI_OSR8_WAVEOUT           equ MM_OKSORI_BASE+1
MM_OKSORI_OSR8_WAVEIN            equ MM_OKSORI_BASE+2
MM_OKSORI_OSR16_WAVEOUT          equ MM_OKSORI_BASE+3
MM_OKSORI_OSR16_WAVEIN           equ MM_OKSORI_BASE+4
MM_OKSORI_FM_OPL4                equ MM_OKSORI_BASE+5
MM_OKSORI_MIX_MASTER             equ MM_OKSORI_BASE+6
MM_OKSORI_MIX_WAVE               equ MM_OKSORI_BASE+7
MM_OKSORI_MIX_FM                 equ MM_OKSORI_BASE+8
MM_OKSORI_MIX_LINE               equ MM_OKSORI_BASE+9
MM_OKSORI_MIX_CD                 equ MM_OKSORI_BASE+10
MM_OKSORI_MIX_MIC                equ MM_OKSORI_BASE+11
MM_OKSORI_MIX_ECHO               equ MM_OKSORI_BASE+12
MM_OKSORI_MIX_AUX1               equ MM_OKSORI_BASE+13
MM_OKSORI_MIX_LINE1              equ MM_OKSORI_BASE+14
MM_OKSORI_EXT_MIC1               equ MM_OKSORI_BASE+15
MM_OKSORI_EXT_MIC2               equ MM_OKSORI_BASE+16
MM_OKSORI_MIDIOUT                equ MM_OKSORI_BASE+17
MM_OKSORI_MIDIIN                 equ MM_OKSORI_BASE+18
MM_OKSORI_MPEG_CDVISION          equ MM_OKSORI_BASE+19
MM_DIACOUSTICS_DRUM_ACTION       equ 1
MM_KAY_ELEMETRICS_CSL            equ 4300h
MM_KAY_ELEMETRICS_CSL_DAT        equ 4308h
MM_KAY_ELEMETRICS_CSL_4CHANNEL   equ 4309h
MM_CRYSTAL_CS4232_WAVEIN         equ 1
MM_CRYSTAL_CS4232_WAVEOUT        equ 2
MM_CRYSTAL_CS4232_WAVEMIXER      equ 3
MM_CRYSTAL_CS4232_WAVEAUX_AUX1   equ 4
MM_CRYSTAL_CS4232_WAVEAUX_AUX2   equ 5
MM_CRYSTAL_CS4232_WAVEAUX_LINE   equ 6
MM_CRYSTAL_CS4232_WAVEAUX_MONO   equ 7
MM_CRYSTAL_CS4232_WAVEAUX_MASTER equ 8
MM_CRYSTAL_CS4232_MIDIIN         equ 9
MM_CRYSTAL_CS4232_MIDIOUT        equ 10
MM_CRYSTAL_CS4232_INPUTGAIN_AUX1 equ 13
MM_CRYSTAL_CS4232_INPUTGAIN_LOOP equ 14
MM_CRYSTAL_SOUND_FUSION_WAVEIN   equ 21
MM_CRYSTAL_SOUND_FUSION_WAVEOUT  equ 22
MM_CRYSTAL_SOUND_FUSION_MIXER    equ 23
MM_CRYSTAL_SOUND_FUSION_MIDIIN   equ 24
MM_CRYSTAL_SOUND_FUSION_MIDIOUT  equ 25
MM_CRYSTAL_SOUND_FUSION_JOYSTICK equ 26
MM_QUARTERDECK_LHWAVEIN          equ 0
MM_QUARTERDECK_LHWAVEOUT         equ 1
MM_TDK_MW_MIDI_SYNTH             equ 1
MM_TDK_MW_MIDI_IN                equ 2
MM_TDK_MW_MIDI_OUT               equ 3
MM_TDK_MW_WAVE_IN                equ 4
MM_TDK_MW_WAVE_OUT               equ 5
MM_TDK_MW_AUX                    equ 6
MM_TDK_MW_MIXER                  equ 10
MM_TDK_MW_AUX_MASTER             equ 100
MM_TDK_MW_AUX_BASS               equ 101
MM_TDK_MW_AUX_TREBLE             equ 102
MM_TDK_MW_AUX_MIDI_VOL           equ 103
MM_TDK_MW_AUX_WAVE_VOL           equ 104
MM_TDK_MW_AUX_WAVE_RVB           equ 105
MM_TDK_MW_AUX_WAVE_CHR           equ 106
MM_TDK_MW_AUX_VOL                equ 107
MM_TDK_MW_AUX_RVB                equ 108
MM_TDK_MW_AUX_CHR                equ 109
MM_DIGITAL_AUDIO_LABS_TC         equ 01h
MM_DIGITAL_AUDIO_LABS_DOC        equ 02h
MM_DIGITAL_AUDIO_LABS_V8         equ 10h
MM_DIGITAL_AUDIO_LABS_CPRO       equ 11h
MM_DIGITAL_AUDIO_LABS_VP         equ 12h
MM_DIGITAL_AUDIO_LABS_CDLX       equ 13h
MM_DIGITAL_AUDIO_LABS_CTDIF      equ 14h
MM_SEERSYS_SEERSYNTH             equ 1
MM_SEERSYS_SEERWAVE              equ 2
MM_SEERSYS_SEERMIX               equ 3
MM_SEERSYS_WAVESYNTH             equ 4
MM_SEERSYS_WAVESYNTH_WG          equ 5
MM_SEERSYS_REALITY               equ 6
MM_OSPREY_1000WAVEIN             equ 1
MM_OSPREY_1000WAVEOUT            equ 2
MM_SOUNDESIGNS_WAVEIN            equ 1
MM_SOUNDESIGNS_WAVEOUT           equ 2
MM_SSP_SNDFESWAVEIN              equ 1
MM_SSP_SNDFESWAVEOUT             equ 2
MM_SSP_SNDFESMIDIIN              equ 3
MM_SSP_SNDFESMIDIOUT             equ 4
MM_SSP_SNDFESSYNTH               equ 5
MM_SSP_SNDFESMIX                 equ 6
MM_SSP_SNDFESAUX                 equ 7
MM_ECS_AADF_MIDI_IN              equ 10
MM_ECS_AADF_MIDI_OUT             equ 11
MM_ECS_AADF_WAVE2MIDI_IN         equ 12
MM_AMD_INTERWAVE_WAVEIN          equ 1
MM_AMD_INTERWAVE_WAVEOUT         equ 2
MM_AMD_INTERWAVE_SYNTH           equ 3
MM_AMD_INTERWAVE_MIXER1          equ 4
MM_AMD_INTERWAVE_MIXER2          equ 5
MM_AMD_INTERWAVE_JOYSTICK        equ 6
MM_AMD_INTERWAVE_EX_CD           equ 7
MM_AMD_INTERWAVE_MIDIIN          equ 8
MM_AMD_INTERWAVE_MIDIOUT         equ 9
MM_AMD_INTERWAVE_AUX1            equ 10
MM_AMD_INTERWAVE_AUX2            equ 11
MM_AMD_INTERWAVE_AUX_MIC         equ 12
MM_AMD_INTERWAVE_AUX_CD          equ 13
MM_AMD_INTERWAVE_MONO_IN         equ 14
MM_AMD_INTERWAVE_MONO_OUT        equ 15
MM_AMD_INTERWAVE_EX_TELEPHONY    equ 16
MM_AMD_INTERWAVE_WAVEOUT_BASE    equ 17
MM_AMD_INTERWAVE_WAVEOUT_TREBLE  equ 18
MM_AMD_INTERWAVE_STEREO_ENHANCED equ 19
MM_COREDYNAMICS_DYNAMIXHR        equ 1
MM_COREDYNAMICS_DYNASONIX_SYNTH  equ 2
MM_COREDYNAMICS_DYNASONIX_MIDI_IN equ 3
MM_COREDYNAMICS_DYNASONIX_MIDI_OUT equ 4
MM_COREDYNAMICS_DYNASONIX_WAVE_IN equ 5
MM_COREDYNAMICS_DYNASONIX_WAVE_OUT equ 6
MM_COREDYNAMICS_DYNASONIX_AUDIO_IN equ 7
MM_COREDYNAMICS_DYNASONIX_AUDIO_OUT equ 8
MM_COREDYNAMICS_DYNAGRAFX_VGA    equ 9
MM_COREDYNAMICS_DYNAGRAFX_WAVE_IN equ 10
MM_COREDYNAMICS_DYNAGRAFX_WAVE_OUT equ 11
MM_CANAM_CBXWAVEOUT              equ 1
MM_CANAM_CBXWAVEIN               equ 2
MM_SOFTSOUND_CODEC               equ 1
MM_NORRIS_VOICELINK              equ 1
MM_DDD_MIDILINK_MIDIIN           equ 1
MM_DDD_MIDILINK_MIDIOUT          equ 2
MM_EUPHONICS_AUX_CD              equ 1
MM_EUPHONICS_AUX_LINE            equ 2
MM_EUPHONICS_AUX_MASTER          equ 3
MM_EUPHONICS_AUX_MIC             equ 4
MM_EUPHONICS_AUX_MIDI            equ 5
MM_EUPHONICS_AUX_WAVE            equ 6
MM_EUPHONICS_FMSYNTH_MONO        equ 7
MM_EUPHONICS_FMSYNTH_STEREO      equ 8
MM_EUPHONICS_MIDIIN              equ 9
MM_EUPHONICS_MIDIOUT             equ 10
MM_EUPHONICS_MIXER               equ 11
MM_EUPHONICS_WAVEIN              equ 12
MM_EUPHONICS_WAVEOUT             equ 13
MM_EUPHONICS_EUSYNTH             equ 14
CRYSTAL_NET_SFM_CODEC            equ 1
MM_CHROMATIC_M1                  equ 0001h
MM_CHROMATIC_M1_WAVEIN           equ 0002h
MM_CHROMATIC_M1_WAVEOUT          equ 0003h
MM_CHROMATIC_M1_FMSYNTH          equ 0004h
MM_CHROMATIC_M1_MIXER            equ 0005h
MM_CHROMATIC_M1_AUX              equ 0006h
MM_CHROMATIC_M1_AUX_CD           equ 0007h
MM_CHROMATIC_M1_MIDIIN           equ 0008h
MM_CHROMATIC_M1_MIDIOUT          equ 0009h
MM_CHROMATIC_M1_WTSYNTH          equ 0010h
MM_CHROMATIC_M1_MPEGWAVEIN       equ 0011h
MM_CHROMATIC_M1_MPEGWAVEOUT      equ 0012h
MM_CHROMATIC_M2                  equ 0013h
MM_CHROMATIC_M2_WAVEIN           equ 0014h
MM_CHROMATIC_M2_WAVEOUT          equ 0015h
MM_CHROMATIC_M2_FMSYNTH          equ 0016h
MM_CHROMATIC_M2_MIXER            equ 0017h
MM_CHROMATIC_M2_AUX              equ 0018h
MM_CHROMATIC_M2_AUX_CD           equ 0019h
MM_CHROMATIC_M2_MIDIIN           equ 0020h
MM_CHROMATIC_M2_MIDIOUT          equ 0021h
MM_CHROMATIC_M2_WTSYNTH          equ 0022h
MM_CHROMATIC_M2_MPEGWAVEIN       equ 0023h
MM_CHROMATIC_M2_MPEGWAVEOUT      equ 0024h
MM_VIENNASYS_TSP_WAVE_DRIVER     equ 1
MM_CONNECTIX_VIDEC_CODEC         equ 1
MM_GADGETLABS_WAVE44_WAVEIN      equ 1
MM_GADGETLABS_WAVE44_WAVEOUT     equ 2
MM_GADGETLABS_WAVE42_WAVEIN      equ 3
MM_GADGETLABS_WAVE42_WAVEOUT     equ 4
MM_GADGETLABS_WAVE4_MIDIIN       equ 5
MM_GADGETLABS_WAVE4_MIDIOUT      equ 6
MM_FRONTIER_WAVECENTER_MIDIIN    equ 1
MM_FRONTIER_WAVECENTER_MIDIOUT   equ 2
MM_FRONTIER_WAVECENTER_WAVEIN    equ 3
MM_FRONTIER_WAVECENTER_WAVEOUT   equ 4
MM_VIONA_QVINPCI_MIXER           equ 1
MM_VIONA_QVINPCI_WAVEIN          equ 2
MM_VIONAQVINPCI_WAVEOUT          equ 3
MM_VIONA_BUSTER_MIXER            equ 4
MM_VIONA_CINEMASTER_MIXER        equ 5
MM_VIONA_CONCERTO_MIXER          equ 6
MM_CASIO_WP150_MIDIOUT           equ 1
MM_CASIO_WP150_MIDIIN            equ 2
MM_CASIO_LSG_MIDIOUT             equ 3
MM_DIMD_PLATFORM                 equ 0
MM_DIMD_DIRSOUND                 equ 1
MM_DIMD_VIRTMPU                  equ 2
MM_DIMD_VIRTSB                   equ 3
MM_DIMD_VIRTJOY                  equ 4
MM_DIMD_WAVEIN                   equ 5
MM_DIMD_WAVEOUT                  equ 6
MM_DIMD_MIDIIN                   equ 7
MM_DIMD_MIDIOUT                  equ 8
MM_DIMD_AUX_LINE                 equ 9
MM_DIMD_MIXER                    equ 10
MM_DIMD_WSS_WAVEIN               equ 14
MM_DIMD_WSS_WAVEOUT              equ 15
MM_DIMD_WSS_MIXER                equ 17
MM_DIMD_WSS_AUX                  equ 21
MM_DIMD_WSS_SYNTH                equ 76
MM_S3_WAVEOUT                    equ 1
MM_S3_WAVEIN                     equ 2
MM_S3_MIDIOUT                    equ 3
MM_S3_MIDIIN                     equ 4
MM_S3_FMSYNTH                    equ 5
MM_S3_MIXER                      equ 6
MM_S3_AUX                        equ 7
MM_VKC_MPU401_MIDIIN             equ 0100h
MM_VKC_SERIAL_MIDIIN             equ 0101h
MM_VKC_MPU401_MIDIOUT            equ 0200h
MM_VKC_SERIAL_MIDIOUT            equ 0201h
MM_ZEFIRO_ZA2                    equ 2
MM_FHGIIS_MPEGLAYER3             equ 10
MM_QUICKNET_PJWAVEIN             equ 1
MM_QUICKNET_PJWAVEOUT            equ 2
MM_SICRESOURCE_SSO3D             equ 2
MM_SICRESOURCE_SSOW3DI           equ 3
MM_NEOMAGIC_SYNTH                equ 1
MM_NEOMAGIC_WAVEOUT              equ 2
MM_NEOMAGIC_WAVEIN               equ 3
MM_NEOMAGIC_MIDIOUT              equ 4
MM_NEOMAGIC_MIDIIN               equ 5
MM_NEOMAGIC_AUX                  equ 6
MM_NEOMAGIC_MW3DX_WAVEOUT        equ 10
MM_NEOMAGIC_MW3DX_WAVEIN         equ 11
MM_NEOMAGIC_MW3DX_MIDIOUT        equ 12
MM_NEOMAGIC_MW3DX_MIDIIN         equ 13
MM_NEOMAGIC_MW3DX_FMSYNTH        equ 14
MM_NEOMAGIC_MW3DX_GMSYNTH        equ 15
MM_NEOMAGIC_MW3DX_MIXER          equ 16
MM_NEOMAGIC_MW3DX_AUX            equ 17
MM_NEOMAGIC_MWAVE_WAVEOUT        equ 20
MM_NEOMAGIC_MWAVE_WAVEIN         equ 21
MM_NEOMAGIC_MWAVE_MIDIOUT        equ 22
MM_NEOMAGIC_MWAVE_MIDIIN         equ 23
MM_NEOMAGIC_MWAVE_MIXER          equ 24
MM_NEOMAGIC_MWAVE_AUX            equ 25
MM_MERGING_MPEGL3                equ 1
MM_XIRLINK_VISIONLINK            equ 1
MM_OTI_611WAVEIN                 equ 5
MM_OTI_611WAVEOUT                equ 6
MM_OTI_611MIXER                  equ 7
MM_OTI_611MIDIN                  equ 12h
MM_OTI_611MIDIOUT                equ 13h
MM_AUREAL_AU8820                 equ 16
MM_AU8820_SYNTH                  equ 17
MM_AU8820_WAVEOUT                equ 18
MM_AU8820_WAVEIN                 equ 19
MM_AU8820_MIXER                  equ 20
MM_AU8820_AUX                    equ 21
MM_AU8820_MIDIOUT                equ 22
MM_AU8820_MIDIIN                 equ 23
MM_AUREAL_AU8830                 equ 32
MM_AU8830_SYNTH                  equ 33
MM_AU8830_WAVEOUT                equ 34
MM_AU8830_WAVEIN                 equ 35
MM_AU8830_MIXER                  equ 36
MM_AU8830_AUX                    equ 37
MM_AU8830_MIDIOUT                equ 38
MM_AU8830_MIDIIN                 equ 39
MM_VIVO_AUDIO_CODEC              equ 1
MM_SHARP_MDC_MIDI_SYNTH          equ 1
MM_SHARP_MDC_MIDI_IN             equ 2
MM_SHARP_MDC_MIDI_OUT            equ 3
MM_SHARP_MDC_WAVE_IN             equ 4
MM_SHARP_MDC_WAVE_OUT            equ 5
MM_SHARP_MDC_AUX                 equ 6
MM_SHARP_MDC_MIXER               equ 10
MM_SHARP_MDC_AUX_MASTER          equ 100
MM_SHARP_MDC_AUX_BASS            equ 101
MM_SHARP_MDC_AUX_TREBLE          equ 102
MM_SHARP_MDC_AUX_MIDI_VOL        equ 103
MM_SHARP_MDC_AUX_WAVE_VOL        equ 104
MM_SHARP_MDC_AUX_WAVE_RVB        equ 105
MM_SHARP_MDC_AUX_WAVE_CHR        equ 106
MM_SHARP_MDC_AUX_VOL             equ 107
MM_SHARP_MDC_AUX_RVB             equ 108
MM_SHARP_MDC_AUX_CHR             equ 109
MM_LUCENT_ACM_G723               equ 0
MM_ATT_G729A                     equ 1
MM_MARIAN_ARC44WAVEIN            equ 1
MM_MARIAN_ARC44WAVEOUT           equ 2
MM_MARIAN_PRODIF24WAVEIN         equ 3
MM_MARIAN_PRODIF24WAVEOUT        equ 4
MM_MARIAN_ARC88WAVEIN            equ 5
MM_MARIAN_ARC88WAVEOUT           equ 6
MM_BCB_NETBOARD_10               equ 1
MM_BCB_TT75_10                   equ 2
MM_MOTIONPIXELS_MVI2             equ 1
MM_QDESIGN_ACM_MPEG              equ 1
MM_QDESIGN_ACM_QDESIGN_MUSIC     equ 2
MM_NMP_CCP_WAVEIN                equ 1
MM_NMP_CCP_WAVEOUT               equ 2
MM_NMP_ACM_AMR                   equ 10
MM_DF_ACM_G726                   equ 1
MM_DF_ACM_GSM610                 equ 2
MM_BERCOS_WAVEIN                 equ 1
MM_BERCOS_MIXER                  equ 2
MM_BERCOS_WAVEOUT                equ 3
MM_ONLIVE_MPCODEC                equ 1
MM_PHONET_PP_WAVEOUT             equ 1
MM_PHONET_PP_WAVEIN              equ 2
MM_PHONET_PP_MIXER               equ 3
MM_FTR_ENCODER_WAVEIN            equ 1
MM_FTR_ACM                       equ 2
MM_ENET_T2000_LINEIN             equ 1
MM_ENET_T2000_LINEOUT            equ 2
MM_ENET_T2000_HANDSETIN          equ 3
MM_ENET_T2000_HANDSETOUT         equ 4
MM_EMAGIC_UNITOR8                equ 1
MM_SIPROLAB_ACELPNET             equ 1
MM_DICTAPHONE_G726               equ 1
MM_RZS_ACM_TUBGSM                equ 1
MM_EES_PCMIDI14                  equ 1
MM_EES_PCMIDI14_IN               equ 2
MM_EES_PCMIDI14_OUT1             equ 3
MM_EES_PCMIDI14_OUT2             equ 4
MM_EES_PCMIDI14_OUT3             equ 5
MM_EES_PCMIDI14_OUT4             equ 6
MM_HAFTMANN_LPTDAC2              equ 1
MM_LUCID_PCI24WAVEIN             equ 1
MM_LUCID_PCI24WAVEOUT            equ 2
MM_HEADSPACE_HAESYNTH            equ 1
MM_HEADSPACE_HAEWAVEOUT          equ 2
MM_HEADSPACE_HAEWAVEIN           equ 3
MM_HEADSPACE_HAEMIXER            equ 4
MM_UNISYS_ACM_NAP                equ 1
MM_LUMINOSITI_SCWAVEIN           equ 1
MM_LUMINOSITI_SCWAVEOUT          equ 2
MM_LUMINOSITI_SCWAVEMIX          equ 3
MM_ACTIVEVOICE_ACM_VOXADPCM      equ 1
MM_DTS_DS                        equ 1
MM_SOFTLAB_NSK_FRW_WAVEIN        equ 1
MM_SOFTLAB_NSK_FRW_WAVEOUT       equ 2
MM_SOFTLAB_NSK_FRW_MIXER         equ 3
MM_SOFTLAB_NSK_FRW_AUX           equ 4
MM_FORTEMEDIA_WAVEIN             equ 1
MM_FORTEMEDIA_WAVEOUT            equ 2
MM_FORTEMEDIA_FMSYNC             equ 3
MM_FORTEMEDIA_MIXER              equ 4
MM_FORTEMEDIA_AUX                equ 5
MM_SONORUS_STUDIO                equ 1
MM_I_LINK_VOICE_CODER            equ 1
MM_SELSIUS_SYSTEMS_RTPWAVEOUT    equ 1
MM_SELSIUS_SYSTEMS_RTPWAVEIN     equ 2
MM_ADMOS_FM_SYNTH                equ 1
MM_ADMOS_QS3AMIDIOUT             equ 2
MM_ADMOS_QS3AMIDIIN              equ 3
MM_ADMOS_QS3AWAVEOUT             equ 4
MM_ADMOS_QS3AWAVEIN              equ 5
MM_LEXICON_STUDIO_WAVE_OUT       equ 1
MM_LEXICON_STUDIO_WAVE_IN        equ 2
MM_SGI_320_WAVEIN                equ 1
MM_SGI_320_WAVEOUT               equ 2
MM_SGI_320_MIXER                 equ 3
MM_SGI_540_WAVEIN                equ 4
MM_SGI_540_WAVEOUT               equ 5
MM_SGI_540_MIXER                 equ 6
MM_SGI_RAD_ADATMONO1_WAVEIN      equ 7
MM_SGI_RAD_ADATMONO2_WAVEIN      equ 8
MM_SGI_RAD_ADATMONO3_WAVEIN      equ 9
MM_SGI_RAD_ADATMONO4_WAVEIN      equ 10
MM_SGI_RAD_ADATMONO5_WAVEIN      equ 11
MM_SGI_RAD_ADATMONO6_WAVEIN      equ 12
MM_SGI_RAD_ADATMONO7_WAVEIN      equ 13
MM_SGI_RAD_ADATMONO8_WAVEIN      equ 14
MM_SGI_RAD_ADATSTEREO12_WAVEIN   equ 15
MM_SGI_RAD_ADATSTEREO34_WAVEIN   equ 16
MM_SGI_RAD_ADATSTEREO56_WAVEIN   equ 17
MM_SGI_RAD_ADATSTEREO78_WAVEIN   equ 18
MM_SGI_RAD_ADAT8CHAN_WAVEIN      equ 19
MM_SGI_RAD_ADATMONO1_WAVEOUT     equ 20
MM_SGI_RAD_ADATMONO2_WAVEOUT     equ 21
MM_SGI_RAD_ADATMONO3_WAVEOUT     equ 22
MM_SGI_RAD_ADATMONO4_WAVEOUT     equ 23
MM_SGI_RAD_ADATMONO5_WAVEOUT     equ 24
MM_SGI_RAD_ADATMONO6_WAVEOUT     equ 25
MM_SGI_RAD_ADATMONO7_WAVEOUT     equ 26
MM_SGI_RAD_ADATMONO8_WAVEOUT     equ 27
MM_SGI_RAD_ADATSTEREO12_WAVEOUT  equ 28
MM_SGI_RAD_ADATSTEREO32_WAVEOUT  equ 29
MM_SGI_RAD_ADATSTEREO56_WAVEOUT  equ 30
MM_SGI_RAD_ADATSTEREO78_WAVEOUT  equ 31
MM_SGI_RAD_ADAT8CHAN_WAVEOUT     equ 32
MM_SGI_RAD_AESMONO1_WAVEIN       equ 33
MM_SGI_RAD_AESMONO2_WAVEIN       equ 34
MM_SGI_RAD_AESSTEREO_WAVEIN      equ 35
MM_SGI_RAD_AESMONO1_WAVEOUT      equ 36
MM_SGI_RAD_AESMONO2_WAVEOUT      equ 37
MM_SGI_RAD_AESSTEREO_WAVEOUT     equ 38
MM_IPI_ACM_HSX                   equ 1
MM_IPI_ACM_RPELP                 equ 2
MM_IPI_WF_ASSS                   equ 3
MM_IPI_AT_WAVEOUT                equ 4
MM_IPI_AT_WAVEIN                 equ 5
MM_IPI_AT_MIXER                  equ 6
MM_ICE_WAVEOUT                   equ 1
MM_ICE_WAVEIN                    equ 2
MM_ICE_MTWAVEOUT                 equ 3
MM_ICE_MTWAVEIN                  equ 4
MM_ICE_MIDIOUT1                  equ 5
MM_ICE_MIDIIN1                   equ 6
MM_ICE_MIDIOUT2                  equ 7
MM_ICE_MIDIIN2                   equ 8
MM_ICE_SYNTH                     equ 9
MM_ICE_MIXER                     equ 10
MM_ICE_AUX                       equ 11
MM_VQST_VQC1                     equ 1
MM_VQST_VQC2                     equ 2
MM_ETEK_KWIKMIDI_MIDIIN          equ 1
MM_ETEK_KWIKMIDI_MIDIOUT         equ 2
MM_INTERNET_SSW_MIDIOUT          equ 10
MM_INTERNET_SSW_MIDIIN           equ 11
MM_INTERNET_SSW_WAVEOUT          equ 12
MM_INTERNET_SSW_WAVEIN           equ 13
MM_SONY_ACM_SCX                  equ 1
MM_UH_ACM_ADPCM                  equ 1
MM_SYDEC_NV_WAVEIN               equ 1
MM_SYDEC_NV_WAVEOUT              equ 2
MM_FLEXION_X300_WAVEIN           equ 1
MM_FLEXION_X300_WAVEOUT          equ 2
MM_VIA_WAVEOUT                   equ 1
MM_VIA_WAVEIN                    equ 2
MM_VIA_MIXER                     equ 3
MM_VIA_AUX                       equ 4
MM_VIA_MPU401_MIDIOUT            equ 5
MM_VIA_MPU401_MIDIIN             equ 6
MM_VIA_SWFM_SYNTH                equ 7
MM_VIA_WDM_WAVEOUT               equ 8
MM_VIA_WDM_WAVEIN                equ 9
MM_VIA_WDM_MIXER                 equ 10
MM_VIA_WDM_MPU401_MIDIOUT        equ 11
MM_VIA_WDM_MPU401_MIDIIN         equ 12
MM_MICRONAS_SC4                  equ 1
MM_MICRONAS_CLP833               equ 2
MM_HP_WAVEOUT                    equ 1
MM_HP_WAVEIN                     equ 2
MM_QUICKAUDIO_MINIMIDI           equ 1
MM_QUICKAUDIO_MAXIMIDI           equ 2
MM_ICCC_UNA3_WAVEIN              equ 1
MM_ICCC_UNA3_WAVEOUT             equ 2
MM_ICCC_UNA3_AUX                 equ 3
MM_ICCC_UNA3_MIXER               equ 4
MM_3COM_CB_MIXER                 equ 1
MM_3COM_CB_WAVEIN                equ 2
MM_3COM_CB_WAVEOUT               equ 3
MM_MINDMAKER_GC_WAVEIN           equ 1
MM_MINDMAKER_GC_WAVEOUT          equ 2
MM_MINDMAKER_GC_MIXER            equ 3
MM_TELEKOL_WAVEOUT               equ 1
MM_TELEKOL_WAVEIN                equ 2
MM_ALGOVISION_VB80WAVEOUT        equ 1
MM_ALGOVISION_VB80WAVEIN         equ 2
MM_ALGOVISION_VB80MIXER          equ 3
MM_ALGOVISION_VB80AUX            equ 4
MM_ALGOVISION_VB80AUX2           equ 5
WAVE_FORMAT_UNKNOWN              equ 0000h
WAVE_FORMAT_ADPCM                equ 0002h
WAVE_FORMAT_IEEE_FLOAT           equ 0003h
WAVE_FORMAT_VSELP                equ 0004h
WAVE_FORMAT_IBM_CVSD             equ 0005h
WAVE_FORMAT_ALAW                 equ 0006h
WAVE_FORMAT_MULAW                equ 0007h
WAVE_FORMAT_DTS                  equ 0008h
WAVE_FORMAT_DRM                  equ 0009h
WAVE_FORMAT_WMAVOICE9            equ 000Ah
WAVE_FORMAT_WMAVOICE10           equ 000Bh
WAVE_FORMAT_OKI_ADPCM            equ 0010h
WAVE_FORMAT_DVI_ADPCM            equ 0011h
WAVE_FORMAT_IMA_ADPCM            equ WAVE_FORMAT_DVI_ADPCM
WAVE_FORMAT_MEDIASPACE_ADPCM     equ 0012h
WAVE_FORMAT_SIERRA_ADPCM         equ 0013h
WAVE_FORMAT_G723_ADPCM           equ 0014h
WAVE_FORMAT_DIGISTD              equ 0015h
WAVE_FORMAT_DIGIFIX              equ 0016h
WAVE_FORMAT_DIALOGIC_OKI_ADPCM   equ 0017h
WAVE_FORMAT_MEDIAVISION_ADPCM    equ 0018h
WAVE_FORMAT_CU_CODEC             equ 0019h
WAVE_FORMAT_YAMAHA_ADPCM         equ 0020h
WAVE_FORMAT_SONARC               equ 0021h
WAVE_FORMAT_DSPGROUP_TRUESPEECH  equ 0022h
WAVE_FORMAT_ECHOSC1              equ 0023h
WAVE_FORMAT_AUDIOFILE_AF36       equ 0024h
WAVE_FORMAT_APTX                 equ 0025h
WAVE_FORMAT_AUDIOFILE_AF10       equ 0026h
WAVE_FORMAT_PROSODY_1612         equ 0027h
WAVE_FORMAT_LRC                  equ 0028h
WAVE_FORMAT_DOLBY_AC2            equ 0030h
WAVE_FORMAT_GSM610               equ 0031h
WAVE_FORMAT_MSNAUDIO             equ 0032h
WAVE_FORMAT_ANTEX_ADPCME         equ 0033h
WAVE_FORMAT_CONTROL_RES_VQLPC    equ 0034h
WAVE_FORMAT_DIGIREAL             equ 0035h
WAVE_FORMAT_DIGIADPCM            equ 0036h
WAVE_FORMAT_CONTROL_RES_CR10     equ 0037h
WAVE_FORMAT_NMS_VBXADPCM         equ 0038h
WAVE_FORMAT_CS_IMAADPCM          equ 0039h
WAVE_FORMAT_ECHOSC3              equ 003Ah
WAVE_FORMAT_ROCKWELL_ADPCM       equ 003Bh
WAVE_FORMAT_ROCKWELL_DIGITALK    equ 003Ch
WAVE_FORMAT_XEBEC                equ 003Dh
WAVE_FORMAT_G721_ADPCM           equ 0040h
WAVE_FORMAT_G728_CELP            equ 0041h
WAVE_FORMAT_MSG723               equ 0042h
WAVE_FORMAT_MPEG                 equ 0050h
WAVE_FORMAT_RT24                 equ 0052h
WAVE_FORMAT_PAC                  equ 0053h
WAVE_FORMAT_MPEGLAYER3           equ 0055h
WAVE_FORMAT_LUCENT_G723          equ 0059h
WAVE_FORMAT_CIRRUS               equ 0060h
WAVE_FORMAT_ESPCM                equ 0061h
WAVE_FORMAT_VOXWARE              equ 0062h
WAVE_FORMAT_CANOPUS_ATRAC        equ 0063h
WAVE_FORMAT_G726_ADPCM           equ 0064h
WAVE_FORMAT_G722_ADPCM           equ 0065h
WAVE_FORMAT_DSAT_DISPLAY         equ 0067h
WAVE_FORMAT_VOXWARE_BYTE_ALIGNED equ 0069h
WAVE_FORMAT_VOXWARE_AC8          equ 0070h
WAVE_FORMAT_VOXWARE_AC10         equ 0071h
WAVE_FORMAT_VOXWARE_AC16         equ 0072h
WAVE_FORMAT_VOXWARE_AC20         equ 0073h
WAVE_FORMAT_VOXWARE_RT24         equ 0074h
WAVE_FORMAT_VOXWARE_RT29         equ 0075h
WAVE_FORMAT_VOXWARE_RT29HW       equ 0076h
WAVE_FORMAT_VOXWARE_VR12         equ 0077h
WAVE_FORMAT_VOXWARE_VR18         equ 0078h
WAVE_FORMAT_VOXWARE_TQ40         equ 0079h
WAVE_FORMAT_SOFTSOUND            equ 0080h
WAVE_FORMAT_VOXWARE_TQ60         equ 0081h
WAVE_FORMAT_MSRT24               equ 0082h
WAVE_FORMAT_G729A                equ 0083h
WAVE_FORMAT_MVI_MVI2             equ 0084h
WAVE_FORMAT_DF_G726              equ 0085h
WAVE_FORMAT_DF_GSM610            equ 0086h
WAVE_FORMAT_ISIAUDIO             equ 0088h
WAVE_FORMAT_ONLIVE               equ 0089h
WAVE_FORMAT_SBC24                equ 0091h
WAVE_FORMAT_DOLBY_AC3_SPDIF      equ 0092h
WAVE_FORMAT_MEDIASONIC_G723      equ 0093h
WAVE_FORMAT_PROSODY_8KBPS        equ 0094h
WAVE_FORMAT_ZYXEL_ADPCM          equ 0097h
WAVE_FORMAT_PHILIPS_LPCBB        equ 0098h
WAVE_FORMAT_PACKED               equ 0099h
WAVE_FORMAT_MALDEN_PHONYTALK     equ 00A0h
WAVE_FORMAT_RHETOREX_ADPCM       equ 0100h
WAVE_FORMAT_IRAT                 equ 0101h
WAVE_FORMAT_VIVO_G723            equ 0111h
WAVE_FORMAT_VIVO_SIREN           equ 0112h
WAVE_FORMAT_DIGITAL_G723         equ 0123h
WAVE_FORMAT_SANYO_LD_ADPCM       equ 0125h
WAVE_FORMAT_SIPROLAB_ACEPLNET    equ 0130h
WAVE_FORMAT_SIPROLAB_ACELP4800   equ 0131h
WAVE_FORMAT_SIPROLAB_ACELP8V3    equ 0132h
WAVE_FORMAT_SIPROLAB_G729        equ 0133h
WAVE_FORMAT_SIPROLAB_G729A       equ 0134h
WAVE_FORMAT_SIPROLAB_KELVIN      equ 0135h
WAVE_FORMAT_G726ADPCM            equ 0140h
WAVE_FORMAT_QUALCOMM_PUREVOICE   equ 0150h
WAVE_FORMAT_QUALCOMM_HALFRATE    equ 0151h
WAVE_FORMAT_TUBGSM               equ 0155h
WAVE_FORMAT_MSAUDIO1             equ 0160h
WAVE_FORMAT_WMAUDIO2             equ 0161h
WAVE_FORMAT_WMAUDIO3             equ 0162h
WAVE_FORMAT_WMAUDIO_LOSSLESS     equ 0163h
WAVE_FORMAT_WMASPDIF             equ 0164h
WAVE_FORMAT_UNISYS_NAP_ADPCM     equ 0170h
WAVE_FORMAT_UNISYS_NAP_ULAW      equ 0171h
WAVE_FORMAT_UNISYS_NAP_ALAW      equ 0172h
WAVE_FORMAT_UNISYS_NAP_16K       equ 0173h
WAVE_FORMAT_CREATIVE_ADPCM       equ 0200h
WAVE_FORMAT_CREATIVE_FASTSPEECH8 equ 0202h
WAVE_FORMAT_CREATIVE_FASTSPEECH10 equ 0203h
WAVE_FORMAT_UHER_ADPCM           equ 0210h
WAVE_FORMAT_QUARTERDECK          equ 0220h
WAVE_FORMAT_ILINK_VC             equ 0230h
WAVE_FORMAT_RAW_SPORT            equ 0240h
WAVE_FORMAT_ESST_AC3             equ 0241h
WAVE_FORMAT_GENERIC_PASSTHRU     equ 0249h
WAVE_FORMAT_IPI_HSX              equ 0250h
WAVE_FORMAT_IPI_RPELP            equ 0251h
WAVE_FORMAT_CS2                  equ 0260h
WAVE_FORMAT_SONY_SCX             equ 0270h
WAVE_FORMAT_FM_TOWNS_SND         equ 0300h
WAVE_FORMAT_BTV_DIGITAL          equ 0400h
WAVE_FORMAT_QDESIGN_MUSIC        equ 0450h
WAVE_FORMAT_VME_VMPCM            equ 0680h
WAVE_FORMAT_TPC                  equ 0681h
WAVE_FORMAT_OLIGSM               equ 1000h
WAVE_FORMAT_OLIADPCM             equ 1001h
WAVE_FORMAT_OLICELP              equ 1002h
WAVE_FORMAT_OLISBC               equ 1003h
WAVE_FORMAT_OLIOPR               equ 1004h
WAVE_FORMAT_LH_CODEC             equ 1100h
WAVE_FORMAT_NORRIS               equ 1400h
WAVE_FORMAT_SOUNDSPACE_MUSICOMPRESS equ 1500h
WAVE_FORMAT_MPEG_ADTS_AAC        equ 1600h
WAVE_FORMAT_MPEG_RAW_AAC         equ 1601h
WAVE_FORMAT_NOKIA_MPEG_ADTS_AAC  equ 1608h
WAVE_FORMAT_NOKIA_MPEG_RAW_AAC   equ 1609h
WAVE_FORMAT_VODAFONE_MPEG_ADTS_AAC equ 160Ah
WAVE_FORMAT_VODAFONE_MPEG_RAW_AAC equ 160Bh
WAVE_FORMAT_DVM                  equ 2000h
WAVE_FORMAT_EXTENSIBLE           equ 0FFFEh
WAVE_FORMAT_DEVELOPMENT          equ 0FFFFh
SPEAKER_FRONT_LEFT               equ 1h
SPEAKER_FRONT_RIGHT              equ 2h
SPEAKER_FRONT_CENTER             equ 4h
SPEAKER_LOW_FREQUENCY            equ 8h
SPEAKER_BACK_LEFT                equ 10h
SPEAKER_BACK_RIGHT               equ 20h
SPEAKER_FRONT_LEFT_OF_CENTER     equ 40h
SPEAKER_FRONT_RIGHT_OF_CENTER    equ 80h
SPEAKER_BACK_CENTER              equ 100h
SPEAKER_SIDE_LEFT                equ 200h
SPEAKER_SIDE_RIGHT               equ 400h
SPEAKER_TOP_CENTER               equ 800h
SPEAKER_TOP_FRONT_LEFT           equ 1000h
SPEAKER_TOP_FRONT_CENTER         equ 2000h
SPEAKER_TOP_FRONT_RIGHT          equ 4000h
SPEAKER_TOP_BACK_LEFT            equ 8000h
SPEAKER_TOP_BACK_CENTER          equ 10000h
SPEAKER_TOP_BACK_RIGHT           equ 20000h
SPEAKER_RESERVED                 equ 7FFC0000h
SPEAKER_ALL                      equ 80000000h
ACM_MPEG_LAYER1                  equ 0001h
ACM_MPEG_LAYER2                  equ 0002h
ACM_MPEG_LAYER3                  equ 0004h
ACM_MPEG_STEREO                  equ 0001h
ACM_MPEG_JOINTSTEREO             equ 0002h
ACM_MPEG_DUALCHANNEL             equ 0004h
ACM_MPEG_SINGLECHANNEL           equ 0008h
ACM_MPEG_PRIVATEBIT              equ 0001h
ACM_MPEG_COPYRIGHT               equ 0002h
ACM_MPEG_ORIGINALHOME            equ 0004h
ACM_MPEG_PROTECTIONBIT           equ 0008h
ACM_MPEG_ID_MPEG1                equ 0010h
MPEGLAYER3_WFX_EXTRA_BYTES       equ 12
MPEGLAYER3_ID_UNKNOWN            equ 0
MPEGLAYER3_ID_MPEG               equ 1
MPEGLAYER3_ID_CONSTANTFRAMESIZE  equ 2
MPEGLAYER3_FLAG_PADDING_ISO      equ 00000000h
MPEGLAYER3_FLAG_PADDING_ON       equ 00000001h
MPEGLAYER3_FLAG_PADDING_OFF      equ 00000002h
MM_MSFT_ACM_WMAUDIO              equ 39
WMAUDIO_BITS_PER_SAMPLE          equ 16
WMAUDIO_MAX_CHANNELS             equ 2
MM_MSFT_ACM_MSAUDIO1             equ 39
MSAUDIO1_BITS_PER_SAMPLE         equ WMAUDIO_BITS_PER_SAMPLE
MSAUDIO1_MAX_CHANNELS            equ WMAUDIO_MAX_CHANNELS
MM_MSFT_ACM_WMAUDIO2             equ 101
WMAUDIO2_BITS_PER_SAMPLE         equ WMAUDIO_BITS_PER_SAMPLE
WMAUDIO2_MAX_CHANNELS            equ WMAUDIO_MAX_CHANNELS
WAVE_FILTER_UNKNOWN              equ 0000h
WAVE_FILTER_DEVELOPMENT          equ 0FFFFh
WAVE_FILTER_VOLUME               equ 0001h
WAVE_FILTER_ECHO                 equ 0002h
JPEG_PROCESS_BASELINE            equ 0
AVIIF_CONTROLFRAME               equ 00000200h
JIFMK_SOF0                       equ 0FFC0h
JIFMK_SOF1                       equ 0FFC1h
JIFMK_SOF2                       equ 0FFC2h
JIFMK_SOF3                       equ 0FFC3h
JIFMK_SOF5                       equ 0FFC5h
JIFMK_SOF6                       equ 0FFC6h
JIFMK_SOF7                       equ 0FFC7h
JIFMK_JPG                        equ 0FFC8h
JIFMK_SOF9                       equ 0FFC9h
JIFMK_SOF10                      equ 0FFCAh
JIFMK_SOF11                      equ 0FFCBh
JIFMK_SOF13                      equ 0FFCDh
JIFMK_SOF14                      equ 0FFCEh
JIFMK_SOF15                      equ 0FFCFh
JIFMK_DHT                        equ 0FFC4h
JIFMK_DAC                        equ 0FFCCh
JIFMK_RST0                       equ 0FFD0h
JIFMK_RST1                       equ 0FFD1h
JIFMK_RST2                       equ 0FFD2h
JIFMK_RST3                       equ 0FFD3h
JIFMK_RST4                       equ 0FFD4h
JIFMK_RST5                       equ 0FFD5h
JIFMK_RST6                       equ 0FFD6h
JIFMK_RST7                       equ 0FFD7h
JIFMK_SOI                        equ 0FFD8h
JIFMK_EOI                        equ 0FFD9h
JIFMK_SOS                        equ 0FFDAh
JIFMK_DQT                        equ 0FFDBh
JIFMK_DNL                        equ 0FFDCh
JIFMK_DRI                        equ 0FFDDh
JIFMK_DHP                        equ 0FFDEh
JIFMK_EXP                        equ 0FFDFh
JIFMK_APP0                       equ 0FFE0h
JIFMK_APP1                       equ 0FFE1h
JIFMK_APP2                       equ 0FFE2h
JIFMK_APP3                       equ 0FFE3h
JIFMK_APP4                       equ 0FFE4h
JIFMK_APP5                       equ 0FFE5h
JIFMK_APP6                       equ 0FFE6h
JIFMK_APP7                       equ 0FFE7h
JIFMK_JPG0                       equ 0FFF0h
JIFMK_JPG1                       equ 0FFF1h
JIFMK_JPG2                       equ 0FFF2h
JIFMK_JPG3                       equ 0FFF3h
JIFMK_JPG4                       equ 0FFF4h
JIFMK_JPG5                       equ 0FFF5h
JIFMK_JPG6                       equ 0FFF6h
JIFMK_JPG7                       equ 0FFF7h
JIFMK_JPG8                       equ 0FFF8h
JIFMK_JPG9                       equ 0FFF9h
JIFMK_JPG10                      equ 0FFFAh
JIFMK_JPG11                      equ 0FFFBh
JIFMK_JPG12                      equ 0FFFCh
JIFMK_JPG13                      equ 0FFFDh
JIFMK_COM                        equ 0FFFEh
JIFMK_TEM                        equ 0FF01h
JIFMK_RES                        equ 0FF02h
JIFMK_00                         equ 0FF00h
JIFMK_FF                         equ 0FFFFh
JPEG_Y                           equ 1
JPEG_YCbCr                       equ 2
JPEG_RGB                         equ 3
MIXERCONTROL_CONTROLTYPE_SRS_MTS equ MIXERCONTROL_CONTROLTYPE_BOOLEAN + 6
MIXERCONTROL_CONTROLTYPE_SRS_ONOFF equ MIXERCONTROL_CONTROLTYPE_BOOLEAN + 7
MIXERCONTROL_CONTROLTYPE_SRS_SYNTHSELECT equ MIXERCONTROL_CONTROLTYPE_BOOLEAN + 8

;;;; end mmreg

ELSE
echo -------------------------------------------
echo WARNING Duplicate include file winextra.inc
echo -------------------------------------------
ENDIF
