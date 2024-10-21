#define ERRNO
//#define _CRT_ERRNO_DEFINED

/* Implementation specific structs */
typedef struct _sys_sem_t
{
    KEVENT	Event;
    LONG	Valid;
} sys_sem_t;

typedef struct _sys_mbox_t
{
    LIST_ENTRY	ListHead;
    KEVENT		Event;
    KSPIN_LOCK	Lock;
    LONG		Valid;
} sys_mbox_t;

typedef KIRQL sys_prot_t;

typedef u32_t sys_thread_t;

typedef struct _LWIP_MESSAGE_CONTAINER
{
	LIST_ENTRY	ListEntry;
    PVOID		Message;
} LWIP_MESSAGE_CONTAINER, *PLWIP_MESSAGE_CONTAINER;

typedef struct _sys_lock_t
{
	KSPIN_LOCK Lock;
	KIRQL irql;
}sys_lock_t;

#define sys_jiffies() sys_now()

/* NULL definitions */
#define SYS_MBOX_NULL NULL
#define SYS_SEM_NULL NULL
#define SYS_ARCH_NULL NULL

void
sys_arch_protect(sys_prot_t *lev);

void
sys_arch_unprotect(sys_prot_t lev);

void
sys_shutdown(void);

void sys_lock_init( sys_lock_t *lock );
void sys_lock_acquire( sys_lock_t *lock );
void sys_lock_release( sys_lock_t *lock );

