
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#define PRINT2(x)  printf("%d%d",(x)/10,(x)%10);

printd(t)
long t;
{
  struct tm *local;

  local = localtime(&t);

  PRINT2(local->tm_mon+1);
  PRINT2(local->tm_mday);
  PRINT2(local->tm_hour);
  PRINT2(local->tm_min);
  PRINT2(local->tm_year);

  putchar('\n');
}

main(argc,argv)
char **argv;
int argc;
{
  int a;
  struct stat buf;

  if(argc!=2) {
    printf("usage: %s filename\n",argv[0]);
    exit(-1);
  }
  a=stat(argv[1],&buf);
  if(a<0) {
    printf("error getting files status\n");
    exit(-1);
  }

  printd(buf.st_mtime);
  return(0);
}
