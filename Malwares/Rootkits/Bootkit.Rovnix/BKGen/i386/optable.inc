;Flag tables for normal and extended Intel opcodes
;(c) sars [HI-TECH] 2003
;sars@ukrtop.com
;
;Version:
;01-test version
;02-added: TableEXT
;03-added: new flags
;04-added: support for MMX, SSE, SSE2, 3DNOW
;
;Description:
;Size of table element is 4 bits.
;0h-one byte instruction
;1h-ModRM byte
;2h-imm8,rel8 etc
;4h-ptr16 etc
;8h-imm16/32,rel16/32 etc
;0Fh-prefix
;0Eh-unsupported opcodes

;3DNOW-Supported
;SSE-Supported
;SSE2-Supported
;MMX-Supported
.code
;================NORMAL OPCODES================
c_Table:
;    01  23    45   67   89   AB   CD   EF
db 011h,011h,028h,000h,011h,011h,028h,000h;0Fh
db 011h,011h,028h,000h,011h,011h,028h,000h;1Fh
db 011h,011h,028h,0F0h,011h,011h,028h,0F0h;2Fh
db 011h,011h,028h,0F0h,011h,011h,028h,0F0h;3Fh
db 000h,000h,000h,000h,000h,000h,000h,000h;4Fh
db 000h,000h,000h,000h,000h,000h,000h,000h;5Fh
db 000h,011h,0FFh,0FFh,089h,023h,000h,000h;6Fh
db 022h,022h,022h,022h,022h,022h,022h,022h;7Fh
db 039h,033h,011h,011h,011h,011h,011h,011h;8Fh
db 000h,000h,000h,000h,000h,0C0h,000h,000h;9Fh
db 088h,088h,000h,000h,028h,000h,000h,000h;AFh
db 022h,022h,022h,022h,088h,088h,088h,088h;BFh
db 033h,040h,011h,039h,060h,040h,002h,000h;CFh
db 011h,011h,022h,000h,011h,011h,011h,011h;DFh
db 022h,022h,022h,022h,088h,0C2h,000h,000h;EFh
db 0F0h,0FFh,000h,011h,000h,000h,000h,011h;FFh
;==============================================
Lentable equ $-c_Table
;===============EXTENDED OPCODES===============
c_TableEXT:
;    01  23    45   67   89   AB   CD   EF
db 011h,011h,0E0h,000h,000h,0EEh,0E1h,003h;0Fh
db 011h,011h,011h,011h,01Eh,0EEh,0EEh,0EEh;1Fh
db 011h,011h,01Eh,01Eh,011h,011h,011h,011h;2Fh
db 000h,000h,000h,0EEh,0EEh,0EEh,0EEh,0EEh;3Fh
db 011h,011h,011h,011h,011h,011h,011h,011h;4Fh
db 011h,011h,011h,011h,011h,011h,011h,011h;5Fh
db 011h,011h,011h,011h,011h,011h,011h,011h;6Fh
db 033h,033h,011h,010h,011h,011h,011h,011h;7Fh
db 088h,088h,088h,088h,088h,088h,088h,088h;8Fh
db 011h,011h,011h,011h,011h,011h,011h,011h;9Fh
db 000h,001h,031h,011h,000h,001h,031h,011h;AFh
db 011h,011h,011h,011h,0EEh,031h,011h,011h;BFh
db 011h,031h,033h,031h,000h,000h,000h,000h;CFh
db 0E1h,011h,011h,011h,011h,011h,011h,011h;DFh
db 011h,011h,011h,011h,011h,011h,011h,011h;EFh
db 0E1h,011h,011h,011h,011h,011h,011h,01Eh;FFh
;==============================================