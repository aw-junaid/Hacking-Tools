
#include <stdio.h>
#include "code.h"

char getlinel(line,len)
char line[];
int len;
{
  int j;
  
  fflush(stdout);

  j=0;
  while (j<(len-1))
  {
    if (read(0,line+j,1) == 0) 
    {  line[j]='\0';
       return EOF;
    }
    if (line[j] == 0x0d ) continue;
    if (line[j] == '\n' )
    {  line[j] ='\0';
       return NULL;
    }
    if (line[j] == '\b')
      j--;
    else
      j++;
  }
  line[len]='\0';
  return NULL;
}
     



main()
{
  char b[80],*a;
  FILE *fd;

  if((fd=fopen("magic.h","w"))==NULL) {
    printf("couldnt open destination file\n");
    exit(-1);
  }

  a=b;
  printf("Enter Magic word: ");
  getlinel(a,10);
  code(a);
  fprintf(fd,"char magic[] = { ");
  while(*a!='\0') {
    fprintf(fd,"%d, ",*a);
    a++;
  }
  fprintf(fd,"0, };\n");
  fclose(fd);
  return 0;
}
  
