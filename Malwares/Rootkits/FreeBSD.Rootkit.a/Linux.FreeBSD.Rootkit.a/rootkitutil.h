#define ERR(a) {\
fprintf(stderr,"%s: ",pg);\
perror(a);\
exit(1);\
}

#define BASENAME(a) {\
if((pg=(char *)strrchr(a,'/'))) {\
pg++;\
}\
else {\
pg=a;\
}\
}

#define USAGE(a) {\
fprintf(stderr,"Usage: %s %s\n",pg,a);\
exit(1);\
}

char *pg;

#define RK_PROG	pg
