;//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
;// BkDrv project
;//	
;// module: x64stubs.asm
;// $Revision: 39 $
;// $Date: 2012-05-02 15:38:09 +0400 (Ср, 02 май 2012) $
;// description: 
;//	Kernel-mode hooking engine implementation. 64-bit hook stubs.

;------------------------------------------------------------------------------
;	structures
;

ASM_FIELDS			STRUC
HookFn				dq	?
Context				dq	?
ASM_FIELDS			ENDS

RUNTIME_FUNCTION	STRUC
BeginAddress		dq	?
EndAddress			dq	?
UnwindData			dq	?
RUNTIME_FUNCTION	ENDS

;------------------------------------------------------------------------------
;	MISC MACROS
;

DESCRIBE_PUBLIC		MACRO name:req, pname:req
				IFDEF _X86_
					name	EQU	_&name&
				ENDIF ;_X86_
					name	dq	pname	
					public	name
					ENDM

_TEXT segment

FasterCodeAlignement	EQU		10h

;------------------------------------------------------------------------------
;

ALIGN				FasterCodeAlignement

;asmfields1			ASM_FIELDS <>
;CallStub1_START LABEL PTR PROC
;RIPFIX1 = $ - CallStub1_START
;_CallStub1	PROC
;	mov     rdx, rcx        ; a2
;	mov     rcx, [asmfields1.Context + RIPFIX1] ; a1
;	jmp    [asmfields1.HookFn + RIPFIX1]
;	ret
;_CallStub1	ENDP
;CallStub1_END LABEL PTR PROC 

asmfields3			ASM_FIELDS <>
CallStub3_START LABEL PTR PROC
RIPFIX3 = $ - CallStub3_START
_CallStub3	PROC
	mov     r9, r8        ; a4
	mov     r8, rdx        ; a3
	mov     rdx, rcx        ; a2
	mov     rcx, [asmfields3.Context + RIPFIX3] ; a1
	jmp     [asmfields3.HookFn + RIPFIX3]
	ret
_CallStub3	ENDP
CallStub3_END LABEL PTR PROC 

asmfields6			ASM_FIELDS <>
CallStub6_START LABEL PTR PROC 
RIPFIX6 = $ - CallStub6_START
_CallStub6	PROC
	sub     rsp, 48h
	mov     rax, [rsp+78h]
	mov     [rsp+30h], rax
	mov     rax, [rsp+70h]
	mov     [rsp+28h], rax
	mov     [rsp+20h], r9
	mov     r9, r8          ; a4
	mov     r8, rdx         ; a3
	mov     rdx, rcx        ; a2
	mov     rcx, [asmfields6.Context + RIPFIX6] ; a1
	call    [asmfields6.HookFn + RIPFIX6]
	add     rsp, 48h
	ret
;	pop     rax ; pop retn
;	push    r9
;	push    rax
;	mov     r9, r8          ; a4
;	mov     r8, rdx         ; a3
;	mov     rdx, rcx        ; a2
;	mov     rcx, [asmfields6.Context + RIPFIX6] ; a1
;	jmp    [asmfields6.HookFn + RIPFIX6]
_CallStub6	ENDP
CallStub6_END LABEL PTR PROC 

;asmfields8			ASM_FIELDS <>
;CallStub8_START LABEL PTR PROC
;RIPFIX8 = $ - CallStub8_START
;_CallStub8	PROC
;	sub     rsp, 58h
;	mov     rax, [rsp+98h]
;	mov     [rsp+40h], rax
;	mov     rax, [rsp+90h]
;	mov     [rsp+38h], rax
;	mov     rax, [rsp+88h]
;	mov     [rsp+30h], rax
;	mov     rax, [rsp+80h]
;	mov     [rsp+28h], rax
;	mov     [rsp+20h], r9
;	mov     r9, r8          ; a4
;	mov     r8, rdx         ; a3
;	mov     rdx, rcx        ; a2
;	mov     rcx, [asmfields8.Context + RIPFIX8]    ; a1
;	call    [asmfields8.HookFn + RIPFIX8]
;	add     rsp, 58h
;	ret
;_CallStub8	ENDP
;CallStub8_END LABEL PTR PROC 

;------------------------------------------------------------------------------
; PUBLICS
;

;DESCRIBE_PUBLIC CallStub1, CallStub1_START
;DESCRIBE_PUBLIC CallStub1SIZE, <CallStub1_END - CallStub1_START>

DESCRIBE_PUBLIC CallStub3, CallStub3_START
DESCRIBE_PUBLIC CallStub3SIZE, <CallStub3_END - CallStub3_START>

DESCRIBE_PUBLIC CallStub6, CallStub6_START
DESCRIBE_PUBLIC CallStub6SIZE, <CallStub6_END - CallStub6_START>

;DESCRIBE_PUBLIC CallStub8, CallStub8_START
;DESCRIBE_PUBLIC CallStub8SIZE, <CallStub8_END - CallStub8_START>


_TEXT ends

;------------------------------------------------------------------------------
; PDATA
;

end
