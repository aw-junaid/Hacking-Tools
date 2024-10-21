#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in_systm.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

/* Set this to whatever you want (both sides need the same entries) */
#define	SID		"icmp_sid"	/* session_id for packet selection	  */
#define SRVSEQ		9		/* icmp_id the server uses		  */
#define CLISEQ		14		/* icmp_id the client uses		  */

/* Better do not change these */
#define VERSION		"0.2"		/* well, the version 			  */
#define CHARS		256		/* max. chars for each packet		  */
#define PING		"PING"		/* authentication String		  */
#define PONG		"PONG"		/* 	"	"			  */


struct sockaddr_in saddr;
struct ip *ip;
struct icmp *icmp;
int rawsock, icmpcode;

void * Malloc(int size);
struct hostent * Gethostbyname( char *host);
int get_raw_sock (void);
int Select(int max, fd_set *readset);
struct hostent *temp;
