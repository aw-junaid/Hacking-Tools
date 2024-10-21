/* Sniffit Config File -- Brecht Claerhout                                */ 

/* if this variable is defined, the '-i' will be available, if not, it    */ 
/* won't be available.                                                    */
/* If you don't need interactive sniffing (like logging when not there)   */
/* compile it with this value #undef, this will make the program over 3   */ 
/* times smaller and it doesn't allocate all that memory.                 */
/* If you 'define' INCLUDE_INTERFACE, your kernel should support          */
/* System V IPC (it probably does)                                        */

#ifdef LINUX
/* if you undef INCLUDE INTERFACE, don't forget to adjust the Makefile! */
/* information is in that file!                                         */
#define INCLUDE_INTERFACE  
/*#undef INCLUDE_INTERFACE */
#endif

#ifdef SUNOS
/* if you undef INCLUDE INTERFACE, don't forget to adjust the Makefile! */
#define INCLUDE_INTERFACE  
/*#undef INCLUDE_INTERFACE */
#endif

#ifdef FREEBSD
/* if you undef INCLUDE INTERFACE, don't forget to adjust the Makefile! */
/* information is in that file!                                         */
#define INCLUDE_INTERFACE  
/*#undef INCLUDE_INTERFACE */
#endif

#ifdef IRIX
#undef INCLUDE_INTERFACE 
#endif

/* Not supported yet */
#ifdef BSDI
#undef INCLUDE_INTERFACE 
#endif

/* If you don't have the atexit() function, you will probably have on_exit */
/* so remove the atexit line and uncomment the on_exit line.               */

#define exit_func(x)    atexit(x)
/* #define exit_func(x)    on_exit (x,0) */


/* Following parameters describe the connections that can be handled at */ 
/* once, MAXCOUNT stands for connections handled in normal mode. As     */
/* memory in normal mode is now handled dynamically, you can pump this  */
/* number up without having to much trouble (Watch it, the machine      */
/* could be slowed down a lot, and packets could get missed)            */
/* CONNECTION_CAPACITY is the same, except in interactive mode, this is */
/* more dangerous to change, if you machine goes to slow (when sniffing */
/* in interactive mode), lower this number.                             */

#define MAXCOUNT  		100
#ifdef INCLUDE_INTERFACE
#define CONNECTION_CAPACITY  	50
#endif

/* This is the interval time for the netstatistics */

#define INFO_TIMER	3       /* In seconds */

/* Read about forcing the sniff device in the README.FIRST file */

#define FORCED_HEAD_LENGTH	ETHERHEAD

/* MTU: this could need a change on interfaces different from ethernet or on non-standard */
/*      configured systems. Get the info out of 'ifconfig'.                               */
/*      But 1500 is a standard.                                                           */

#define MTU	1500

/*************** Don't change anything below this line *********************/

#undef DEBUG                   /* Debugging (to tty) - sigh */
/* #define DEBUG */
#define DEBUG_DEVICE	"/dev/tty9"

#undef DEBUG_ONSCREEN                  /* Debugging (to screen) - sigh */
/*#define DEBUG_ONSCREEN */

