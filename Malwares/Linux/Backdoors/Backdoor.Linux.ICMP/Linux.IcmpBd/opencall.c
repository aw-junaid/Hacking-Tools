/* Copyright (C) 2002 Martin J. Muench <mjm@codito.de> 	   */
/* Idea from IPC tutorial, modifications from code bighawk */

#include "config.h"

static  pid_t *childpid = NULL;

int popen2(const char *commandstring) {
    int i, fds[2];
    pid_t pid;

    if(!childpid)
        if(!(childpid = calloc(1024, sizeof(pid_t))))
            return -1;

    if(socketpair(AF_UNIX, SOCK_STREAM, 0, fds) == -1)
        return -1;

    if((pid = fork()) < 0)
        return -1;
        
    else if(!pid) {
        close(fds[1]);
        if(fds[0] != STDIN_FILENO) {
            dup2(fds[0], STDIN_FILENO);
            close(fds[0]);
        }
        dup2(STDIN_FILENO, STDOUT_FILENO);
        dup2(STDOUT_FILENO, STDERR_FILENO);
      
        for(i = 0; i < 1024; i++) {
            if(childpid[i] > 0)
            	close(i);
        }
        execl("/bin/sh", "sh", "-c", commandstring, NULL);
        _exit(127);
    }

    close(fds[0]);
    childpid[fds[1]] = pid;
    return(fds[1]);
}

int pclose2(int fd) {
    int stat;
    pid_t pid;   

    if(!childpid)
        return -1;
    if(!childpid[fd])
        return -1;

    close(fd);
  
    pid = childpid[fd];

    while(waitpid(pid, &stat, 0) == -1)
        if(errno != EINTR)
            return -1;

    return(stat);
}

