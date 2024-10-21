;==================================================================================================================================================
;                 ********         ***        ***********     *********     ***     ***    ***     ***                *******        *******       
;              ***********     **** ****     ***********    ***********    ***     ***    ***     ***              ***** *****    ***** *****      
;             ***     ***    ***     ***        ***        ***     ***    ***     ***     ***   ***               **      ***    **     ****       
;            ***            ***********        ***        ***            ***********       ** **                        ****         ****          
;           ***     ***    ***********        ***        ***     ***    ***********        ***                  **      ***     ****               
;          ***********    ***     ***        ***        ***********    ***     ***        ***                  ***** *****    ***********          
;          *********     ***     ***        ***         *********     ***     ***        ***                    *******      ***********           
;==================================================Catchy32 v1.6 - Length Disassembler Engine 32bit================================================
;SIZE=580 bytes
;Version:
;1.0-test version
;1.1-added: support prefix
;1.2-added: TableEXT
;1.3-added: support for 0F6h and 0F7h groups
;1.4-tables fixed
;   -SIB byte handling fixed
;1.5-code fixed&optimized
;   -processing 0F6h and 0F7h groups is corrected
;   -processing 0A0h-0A3h groups is corrected
;1.6-code fixed
;   -added: max lenght=15 bytes
;==================================================================================================================================================
;in:   esi - pointer to opcode
;out:  eax - opcode length or 0ffffffffh if error
;(c) sars [HI-TECH] 2003
;sars@ukrtop.com
;==================================================================================================================================================
pref66h equ 1
pref67h equ 2

.code
;---------------Initial adjustment----------------
c_Catchy:
	pushad
	call	c_Delta

;------------Delta-offset calculation-------------
c_Delta:
	pop	ebp
	sub	ebp, offset c_Delta  
	xor	ecx, ecx

;----Flags extraction, checks for some opcodes----
c_ExtFlags:
	xor	eax, eax
	xor	ebx, ebx
	cdq
	lodsb				;al <- opcode
	mov 	cl, al			;cl <- opcode
	cmp	al, 0fh			;Test on prefix 0Fh
	je 	c_ExtdTable		
	cmp 	word ptr [esi-1], 20CDh	;Test on VXD call
	jne 	c_NormTable
	inc	esi			;If VXD call (int 20h), then command length is 6 bytes
	lodsd
	jmp 	c_CalcLen

c_ExtdTable:				;Load flags from extended table
	lodsb
	inc 	ah			;EAX=al+100h (100h/2 - lenght first table)

c_NormTable:				;Load flags from normal table
	shr 	eax, 1			;Elements tables on 4 bits
;	mov 	al, byte ptr ebp.[c_Table+eax]
mov 	al, byte ptr [c_Table+eax]

c_CheckC1:	
	jc 	c_IFC1 
	shr	eax, 4			;Get high 4-bits block if offset is odd, otherwise...

c_IFC1:
	and  	eax, 0Fh			;...low
	xchg 	eax, ebx			;EAX will be needed for other purposes

;--------------Opcode type checking---------------
c_CheckFlags:
	cmp 	bl, 0Eh			;Test on ErrorFlag
	je 	c_Error
	cmp 	bl, 0Fh			;Test on PrefixFlag
	je 	c_Prefix
	or 	ebx, ebx			;One byte command   
	jz 	c_CalcLen                   
	btr	ebx, 0			;Command with ModRM byte
	jc 	c_ModRM
	btr 	ebx, 1			;Test on imm8,rel8 etc flag
	jc 	c_incr1
	btr	ebx, 2			;Test on ptr16 etc flag
	jc 	c_incr2

;-----imm16/32,rel16/32, etc types processing-----
c_16_32:
	and 	bl, 11110111b    		;Reset 16/32 sign 

	cmp 	cl, 0A0h			;Processing group 0A0h-0A3h
	jb	c_Check66h
	cmp	cl, 0A3h
	ja	c_Check66h
	test	ch, pref67h
	jnz	c_incr2
	jmp	c_incr4
		
c_Check66h:				;Processing other groups
	test 	ch, pref66h                    
	jz 	c_incr4                            
	jmp 	c_incr2                            

;---------------Prefixes processing---------------
c_Prefix:
	cmp 	cl, 66h
	je 	c_SetFlag66h
	cmp 	cl, 67h
	jne 	c_ExtFlags

c_SetFlag67h:
	or	ch, pref67h
	jmp 	c_ExtFlags

c_SetFlag66h:
	or 	ch, pref66h
	jmp 	c_ExtFlags

;--------------ModR/M byte processing-------------
c_ModRM:
	lodsb

c_Check_0F6h_0F7h:				;Check on 0F6h and 0F7h groups
	cmp 	cl, 0F7h
	je 	c_GroupF6F7
	cmp 	cl, 0F6h
	jne 	c_ModXX			
	
c_GroupF6F7:				;Processing groups 0F6h and 0F7h
	test 	al, 00111000b	
	jnz 	c_ModXX
	test 	cl, 00000001b
	jz	c_incbt1			
	test	ch, 1
	jnz	c_incbt2	
	inc 	esi
	inc 	esi
c_incbt2:	inc 	esi
c_incbt1:	inc 	esi

c_ModXX:					;Processing MOD bits
	mov 	edx, eax
	and 	al, 00000111b		;al <- only R/M bits
	test	dl, 11000000b		;Check MOD bits
	jz  	c_Mod00
	jp  	c_CheckFlags		;Or c_Mod11
	js  	c_Mod10
	
c_Mod01:
	test 	ch, pref67h
	jnz 	c_incr1 			;16-bit addressing
	cmp 	al, 4			;Check SIB
	je 	c_incr2
	jmp 	c_incr1

c_Mod00:
	test 	ch, pref67h
	jz 	c_Mod00_32		;32-bit addressing
	cmp 	al, 6
	je 	c_incr2
	jmp 	c_CheckFlags	
c_Mod00_32:
	cmp 	al, 4			;Check SIB
	jne 	c_disp32

c_SIB:					;Processing SIB byte
	lodsb
	and 	al, 00000111b
	cmp 	al, 5
	je 	c_incr4
	jmp 	c_CheckFlags	

c_disp32:	
	cmp 	al, 5
	je 	c_incr4
	jmp 	c_CheckFlags 

c_Mod10:
	test 	ch, pref67h
	jnz 	c_incr2			;16-bit addressing
	cmp 	al, 4			;Check SIB
	je 	c_incr5
	jmp 	c_incr4

c_incr5:	inc 	esi
c_incr4:	inc 	esi
	inc 	esi
c_incr2:	inc 	esi
c_incr1:	inc 	esi
	jmp 	c_CheckFlags	

;-----------Command length calculation------------
c_CalcLen:
	sub 	esi, [esp+4*1]
	cmp	esi, 15
	ja 	c_Error
	mov 	[esp+4*7], esi
	jmp 	c_Exit

;----------------Setting the error----------------
c_Error:
	xor 	eax, eax
	dec 	eax
	mov 	[esp+4*7], eax	

;---------Restore the registers and exit----------
c_Exit:
	popad
	ret
;-------------------------------------------------
include optable.inc
;==================================================================================================================================================