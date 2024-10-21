/* Copyright (C) 2002 Martin J. Muench <mjm@codito.de> */

#include "config.h"

struct hostent * Gethostbyname ( char * host) {
	struct hostent * temp;
	temp = Malloc(sizeof(struct hostent *));
	temp = gethostbyname(host);

	if (!temp) 
		perror("Hostname:"), exit(-1);
    
	return temp;
}

int get_raw_sock (void) {
	int sock, null = 0;

	if ((sock=socket(AF_INET,SOCK_RAW,IPPROTO_ICMP)) < 0) 
 		perror("RawSocket:"), exit(-1); 

	if (setsockopt(sock,IPPROTO_IP,IP_HDRINCL,&null,sizeof(null)) < 0) 
  		close(sock), perror("SetSockOpt:"), exit(-1); 

	return sock;
}


int Select(int max, fd_set *readset) {
	int numActiveFDs = select(max, readset, NULL, NULL, NULL);
	if (numActiveFDs < 0)
		perror("Select:"), exit(-1);

	return numActiveFDs;
}


void * Malloc(int size) {
	void *buf;
	buf = malloc(size); 

	if(!buf)
		perror("malloc"), exit(-1);

	return(buf);
}

