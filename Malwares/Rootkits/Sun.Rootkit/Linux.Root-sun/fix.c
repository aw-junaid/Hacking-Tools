/*
 *  fixer.c
 *  by Idefix 
 *  inspired on sum.c and SaintStat 2.0
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

main(argc,argv)
  int	argc;
  char	**argv;
{
  unsigned	orig_crc,
		current_crc,
		temp;
  unsigned char	diff1,
		diff2,
		buf[20];
  char		systemstr[100];
  struct stat	statbuf;
  struct timeval	ftime[2],
			otime,
			ntime;
  struct timezone	tzp;
  long   position;
  FILE		*f;
  int		i,
		fix=1;


  if (argc!=4) {
    usage(argv[0]);
    exit(1);
  }

  stat(argv[1], &statbuf);

  if(sum(argv[0], argv[1], &orig_crc) != 0)
    exit(1);
  if(sum(argv[0], argv[2], &current_crc) != 0)
    exit(1);

  sprintf(systemstr, "cp %s %s", argv[1], argv[3]);
  system(systemstr);
  sprintf(systemstr, "cp %s %s", argv[2], argv[1]);
  system(systemstr);

  diff1=(orig_crc&0xFF)-(current_crc&0xFF);
  temp=(current_crc+diff1)&0xFFFF;
  for(i=0;i<8;i++) {
    if(temp&1)
      temp = (temp>>1) + 0x8000;
    else
      temp >>= 1;
  }
  diff2=((orig_crc&0xFF00)>>8)-(temp&0xFF);
  temp=(temp+diff2)&0xFFFF;
  for(i=0;i<8;i++) {
    if(temp&1)
      temp = (temp>>1) + 0x8000;
    else
      temp >>= 1;
  }
  if((temp-orig_crc)==1)
    diff1=diff1-1;

  if((f = fopen(argv[1], "r+b")) == NULL) {
      fprintf (stderr, "%s: Can't open %s\n", argv[0], argv[1]);
      exit(1);
  }
  fseek(f, 0L, SEEK_END);
  position=ftell(f)-17;
  fseek(f, position, SEEK_SET);
  fread(buf, 17, 1, f);
  for(i=0;i<17;i++)
    if(buf[i]!=0) {
      fprintf(stderr, "%s: Last 17 bytes not zero\n", argv[0]);
      fprintf(stderr, "%s: Can't fix checksum\n", argv[0]);
      fix=0;
      break;
    }
  if(fix) {
    buf[0]=diff1;
    buf[8]=diff2;
    fseek(f, position, SEEK_SET);
    fwrite(buf, 17, 1, f);
  }
  fclose(f);  
  
  if(chmod(argv[1], statbuf.st_mode)) {
     fprintf(stderr, "%s: No permission to change mode or no such file\n", argv[0]);
     exit(1);
  }
  
  if(chown(argv[1], statbuf.st_uid, statbuf.st_gid)) {
    fprintf(stderr, "%s: No permission to change owner or no such file\n", argv[0]);
    exit(1);
  }
  
  ftime[0].tv_sec	= statbuf.st_atime;
  ftime[1].tv_sec	= statbuf.st_mtime;
  ntime.tv_sec		= statbuf.st_ctime;
  ftime[0].tv_usec	= ftime[1].tv_usec = ntime.tv_usec = 0;
  
  if(gettimeofday(&otime, &tzp)) {
    fprintf(stderr, "%s: Can't read time of day\n", argv[0]);
    exit(1);
  }
  
  if(settimeofday(&ntime, &tzp)) {
    fprintf(stderr, "%s: Can't set time of day\n", argv[0]);
  }
  
  if(utimes(argv[1], ftime)) {
           fprintf(stderr, "%s: Can't change modify time\n", argv[0]);
  }
  settimeofday(&otime, &tzp);
  return 0;  
}

sum(name, file, crc)
  char		*name;
  char		*file;
  unsigned	*crc;
{
  unsigned	sum;
  int		i,
		c;
  FILE		*f;
  long		nbytes;
  int		errflg = 0;

  if((f=fopen(file, "r")) == NULL) {
    fprintf(stderr, "%s: Can't open %s\n", name, file);
    return(1);
  }
  sum = 0;
  nbytes = 0;
  while((c=getc(f)) != EOF) {
    nbytes++;
    if(sum&01)
      sum = (sum>>1) + 0x8000;
    else
      sum >>= 1;
    sum += c;
    sum &= 0xFFFF;
  }
  if(ferror (f)) {
    errflg++;
    fprintf (stderr, "%s: read error on %s\n", name, file);
  }
  fclose(f);
  *crc=sum;
  return(0);
}

usage(name)
  char	*name;
{
  fprintf(stderr, "Usage:\n");
  fprintf(stderr,"%s original replacement backup\n", name);
}
