#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/time.h>

G(s)
  char	*s;
{
  struct stat b;
  stat(s, &b);
  printf("%s %d %d %d %d %d %d\n",
    s, b.st_atime, b.st_mtime, b.st_ctime, b.st_uid, b.st_gid, b.st_mode);
}

S(s)
  char	*s;
{
  char *f=strtok(s, " ");
  struct timeval	a[2],
			old,
			new;
  struct timezone	z;
  int			o;

  a[0].tv_sec	= atol(strtok(NULL, " "));
  a[1].tv_sec	= atol(strtok(NULL, " "));
  new.tv_sec	= atol(strtok(NULL, " "));
  a[0].tv_usec	= a[1].tv_usec=new.tv_usec=0;

  o=atol(strtok(NULL, " "));
  chown(f, o, atol(strtok(NULL, " ")));
  chmod(f, atol(strtok(NULL, " ")));
  gettimeofday(&old, &z);
  settimeofday(&new, &z);
  utimes(f, a);
  settimeofday(&old, &z);
}

main(c, v)
  int	c;
  char	**v;
{
  char s[512];
  if(c>1) {
    for(++v;--c;++v)
      G(*v);
    putchar('\n');
  } else
    while(strlen(gets(s)) > 4)
      S(s);
}
