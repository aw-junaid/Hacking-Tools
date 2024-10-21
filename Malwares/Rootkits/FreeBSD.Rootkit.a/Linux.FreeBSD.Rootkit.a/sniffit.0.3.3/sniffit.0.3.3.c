/* Sniffit -- coded by Brecht Claerhout                                   */
/*            Time wasted: too much                                       */
/* formatted code and added support for hostname resolution - Godmar Back */ 

#include "config.h"                                  /* Config header file */
#include "sn_defines.h"                                 /* Various defines */
#include "sn_global.h"                                 /* some global defs */
#include "sn_oldether.h"                 /* Leftover from 0.0.X and 0.1.X  */
#include "sn_packets.h"                           /* My own packet structs */
#include "sn_data.h"                                          /* data defs */
#include "sn_logfile.h"
#ifdef INCLUDE_INTERFACE
#include "sn_interface.h"                            /* all ncurses stuff  */
#endif
#include "sn_cfgfile.h"                            /* Config file handling */

static char Copyright[]= 
	"Sniffit - Brecht Claerhout - Copyright 1996";

void quit (char *prog_name)                    /* Learn to use the program */
{
	printf(
"usage: %s [-xdabvn] [-P proto] [-A char] [-p port]\n"
"       [-l sniflen] [-L loglevel] [-F snifdevice]\n"
#ifdef INCLUDE_INTERFACE
"       [-D tty]" 
" (-t<Target IP> | -s<Source IP>)"
" | -i | -c<config file>\n",
#else
"       (-t<Target IP> | -s<Source IP>) | -c<config file>\n",
#endif
		prog_name); 
	exit(0); 
}

void my_exit (void)	{	exit(0);};

/* DEBUGGING INFO */
#ifdef DEBUG
void close_debug_device (void)
{
fclose(debug_dev);
}

void debug_msg(char *debug_text)
{
fprintf(debug_dev,"%s (%d)\n",debug_text,debug_cnt);
debug_cnt++;
}
#endif

char *strlower (char *string)
{
	int i;

	for(i=0;i<strlen(string);i++)
        	*(string+i) = ( isupper(*(string+i)) ? tolower(*(string+i)) : *(string+i) ); 
	return string;
}

void reset_all (void) 
{
	start_dynam=NULL;
	dynam_len=0;
}

/* if do_file == 0, then don't handle the files */
/* this is for the global logfile option        */
struct file_info *add_dynam (char *file, char ptype, char do_file, 
					           unsigned long cur_seq, int len) 
{
	int i;
	FILE *f;
	int last_time_out=0;
	struct file_info *dummy_pointer; 
	struct file_info *search_pointer; 

	if(dynam_len>=MAXCOUNT)
		{
		                             /* remove less effective connection from list */
		search_pointer=start_dynam; dummy_pointer=start_dynam;          
		do
			{
			if(search_pointer->time_out > last_time_out)
				{last_time_out=search_pointer->time_out;
				dummy_pointer=search_pointer;}
			search_pointer=search_pointer->next;
			}
		while(search_pointer != NULL);
#ifdef DEBUG
        	debug_msg("Auto timeout engaged (filename follows)");
        	debug_msg(dummy_pointer->filename);
#endif
		if(dummy_pointer->f==NULL)
		  delete_dynam(dummy_pointer->filename,dummy_pointer->proto,0);
		else
		  delete_dynam(dummy_pointer->filename,dummy_pointer->proto,1);
		printf("Too many connections... auto timeout\n"); 
		}
	if( (dummy_pointer=(struct file_info *)malloc(sizeof(struct file_info))) == NULL)
		{printf("Couldn't allocate memory.\n"); exit(0);};
	dummy_pointer->bytes=0;
	dummy_pointer->proto=ptype;
	strcpy(dummy_pointer->filename,file);
	if(do_file!=0)
		{
		f = fopen(file,"a");   
			if(f==NULL)
			perror("Couldn't open logfile:"),exit(0);
		dummy_pointer->f=f;
		}
	else	{
		dummy_pointer->f=NULL;
		}
	dummy_pointer->next=NULL;
	dummy_pointer->buffer=NULL;
	dummy_pointer->log=0;
	dummy_pointer->exp_seq=cur_seq+len;       
	for(i=0;i<SCBUF;i++)	dummy_pointer->scroll_buf[i]=' ';       
	dummy_pointer->scroll_buf[SCBUF]=0;       
 	
	if(start_dynam==NULL)
		start_dynam=dummy_pointer; 
	else
		{
 		search_pointer=start_dynam;
		while(search_pointer->next != NULL)
			search_pointer=search_pointer->next;
		search_pointer->next=dummy_pointer;
		}
	dynam_len++;
	return dummy_pointer;
}

void delete_dynam (char *file, char ptype,char do_file)
{
	struct file_info *search_pointer;	
	struct file_info *dummy_pointer;	

	if(start_dynam==NULL) return;
	search_pointer=start_dynam;
	if( (strcmp(search_pointer->filename,file)==0) &&
						(search_pointer->proto==ptype))
		{
		if(do_file!=0)	fclose(search_pointer->f);
		start_dynam=search_pointer->next;
		dynam_len--;
		return;
		}
 	search_pointer=start_dynam;
	if(search_pointer->next==NULL) return;
	while(search_pointer->next != NULL)
		{
		if( (strcmp(search_pointer->next->filename,file)==0) &&
					(search_pointer->next->proto==ptype))
			{
			if(do_file!=0) 	fclose(search_pointer->next->f);
			dummy_pointer=search_pointer->next;
			search_pointer->next=search_pointer->next->next;
			if(dummy_pointer->buffer!=NULL) free(dummy_pointer->buffer);
			free(dummy_pointer);
			dynam_len--;
			return;
			}
		search_pointer=search_pointer->next;
		}
}

/* returns NULL on failure */
struct file_info *search_dynam(char *file, char ptype)
{
	struct file_info *search_pointer;

	if(start_dynam==NULL) return NULL;

 	search_pointer=start_dynam;               /* time_out add */
	do
		{
		search_pointer->time_out += 1;
		search_pointer=search_pointer->next;
		}
	while(search_pointer != NULL);

 	search_pointer=start_dynam;              /* actual search */
	do
		{
		if( (strcmp(search_pointer->filename,file)==0) &&
					(search_pointer->proto==ptype))
			{search_pointer->time_out=0;   /* timeout reset */
			return search_pointer;}
		search_pointer=search_pointer->next;
		}
	while(search_pointer != NULL);
	return NULL;
}

/* Type 0: TELNET  */
void record_buf(struct file_info *dummy_pointer, unsigned long cur_seq_nr, 
						     char *data, int len, int type) 
{
int i,j,noloop=0;

if(dummy_pointer->exp_seq != cur_seq_nr) 
	{return;}
noloop=0;
for(i=0;i<len;i++)
  {
  j=dummy_pointer->bytes;
  switch(type)
  	{
	case 0: if(data[i]==TELNET_ENTER)                 /* return found */
			{dummy_pointer->log++;noloop=1;};
		break;
	default: break;
	}
  if(noloop==1)	break;
  if(j>=LOG_PASS_BUF)	break;
  if(isprint(data[i]))
	{dummy_pointer->buffer[j]=data[i];}
  else  {dummy_pointer->buffer[j]='~';};
  dummy_pointer->buffer[j+1]=0;
  dummy_pointer->bytes+=1;
  }
#ifdef	DEBUG_ONSCREEN
	printf("Record buffer: >%s<  (%d)\n",dummy_pointer->buffer,dummy_pointer->bytes); 
#endif
}

void sb_shift(struct file_info *dummy_pointer)
{
int i,j;

for(i=1;i<SCBUF;i++)
	dummy_pointer->scroll_buf[i-1]=dummy_pointer->scroll_buf[i];
}

void sbuf_update(struct file_info *dummy_pointer, unsigned long cur_seq_nr, 
							      char *data, int len) 
{
int i;

if(dummy_pointer->exp_seq != cur_seq_nr)
	{return;}

for(i=0;i<len;i++)
  {
  sb_shift(dummy_pointer);
  if(data[i]!=0)      /* used to be 'isprint', not possible for ftp logging */
  	{dummy_pointer->scroll_buf[SCBUF-1]=data[i];}
  else	{dummy_pointer->scroll_buf[SCBUF-1]='.';};        
  }
#ifdef DEBUG_ONSCREEN
	printf("scr_buf: %s\n",dummy_pointer->scroll_buf);
#endif
}

void print_iphead (struct IP_header *iphead, char icmp_or_plain)
{
        int dummy;
	u_char *so,*dest;
	
	if(icmp_or_plain!=0)
		printf("ICMP message concerned following IP packet:\n");
	so=(u_char *)&(iphead->source);
       	dest=(u_char *)&(iphead->destination);
	printf("from %u.%u.%u.%u to %u.%u.%u.%u\n",
					so[0],so[1],so[2],so[3],
                                        dest[0],dest[1],dest[2],dest[3]);
        dummy=iphead->type; dummy>>=5;
        printf("IP Packet precedence: %s   (%c%c%c)\n",
	                                IP_TYPE_precedence[dummy],
                                        (iphead->type&IP_DELAY)?'D':'-',
                                        (iphead->type&IP_THROUGHPUT)?'T':'-',
                                        (iphead->type&IP_RELIABILITY)?'R':'-');
        dummy=ntohs(iphead->flag_offset); dummy>>=13;
        printf("FLAGS: %s %s     Time to live (secs): %d\n",
	                                (dummy&IP_DF)?"DF":"--",
                                        (dummy&IP_DELAY)?"MF":"--",
	                                 iphead->TTL);
        if(iphead->protocol < 34)
                printf("Protocol (%d): %s\n",iphead->protocol,
		                IP_PROTOCOL_number[iphead->protocol]);
        else    printf("Protocol (%d) not recognised\n",iphead->protocol);
	printf("\n");
}

int check_packet(u_long ipaddr,
		const struct packetheader *p_header, 
		const u_char *sp,
		char *file,
		char *file2,
		struct unwrap *info,
		char *detail,
		int MODE)       
                                          /* MODE 0: -t   MODE 1: -s */
                                          /* MODE 2: -b              */ 
{
        u_char *so,*dest;
	char wc_so[20], wc_dest[20];
	struct IP_header iphead;
	struct TCP_header tcphead;
	struct ICMP_header icmphead;
	struct UDP_header udphead;
	int proto;
	unsigned int prior; char selected; int i;    /* Wildcard stuff */
        unsigned char *str_IP;
	unsigned int n_s;           /* strlen short notation for wc bugfix */

	proto=unwrap_packet(sp, info); 
	if(proto == NO_IP)	return DONT_EXAMINE; /* no use in trying */
        memcpy(&iphead,(sp+PROTO_HEAD),sizeof(struct IP_header));
	so=(u_char *)&(iphead.source);
       	dest=(u_char *)&(iphead.destination);

	if((proto==TCP)&&(PROTOCOLS&F_TCP)) 
		{
		memcpy(&tcphead,(sp+PROTO_HEAD+info->IP_len),
						sizeof(struct TCP_header));
		memcpy(detail,&tcphead,sizeof(struct TCP_header));

		if((WILDCARD==0)&&(CFG_FILE==0))     /* Selection criteria */
		  {
		  if (MODE == DEST && ipaddr != iphead.destination   /* -t */
			||
	   	      MODE == SOURCE && ipaddr != iphead.source      /* -s */
			||
		      MODE == BOTH && ipaddr != iphead.destination   /* -b */
			      && ipaddr != iphead.source
	   	      )  return DONT_EXAMINE; /* Check destination/source IP */
		  }
		else
		  {
		  if (WILDCARD!=0) /* Wildcards */
		    {   
		    sprintf(wc_so,"%u.%u.%u.%u",so[0],so[1],so[2],so[3]);
		    sprintf(wc_dest,"%u.%u.%u.%u",dest[0],dest[1],dest[2],dest[3]);
		    n_s = strlen(IP);
		    if (MODE == DEST && (strncmp(wc_dest,IP,n_s)!=0) /* -t */
	           		||
	   	        MODE == SOURCE && (strncmp(wc_so,IP,n_s)!=0) /* -s */
		        	||
		        MODE == BOTH && (strncmp(wc_dest,IP,n_s)!=0) /* -b */
			         && (strncmp(wc_so,IP,n_s)!=0)
	   	        )   return DONT_EXAMINE; 
					      /* Check destination/source IP */
		    }
		  else 	{ /* We are using the config file (4*Check) */
		       	prior=0; selected=0;
		        sprintf(wc_so,"%u.%u.%u.%u",so[0],so[1],so[2],so[3]);
		        sprintf(wc_dest,"%u.%u.%u.%u",dest[0],dest[1],dest[2],dest[3]);
#ifdef DEBUG_ONSCREEN
printf("CONCERNING: %s  %d - %s  %d\n",wc_so,ntohs(tcphead.source),wc_dest,ntohs(tcphead.destination));
#endif
                        /* Highest prior last (backward+break for speed)  */
			for(i=(select_from_length-1);i>=0;i--)
			  {
			  if(prior > select_from_list[i].priority) break;
			  str_IP = select_from_list[i].host;
#ifdef DEBUG_ONSCREEN
printf("SF: %s  %d   Prior:%d\n",str_IP,select_from_list[i].port,select_from_list[i].priority);
#endif
		          if((select_from_list[i].port==0) || 
			     (select_from_list[i].port==ntohs(tcphead.source)))
				{
				if(select_from_list[i].wildcard==0)
				  {                          /* NO wildcard */ 
     				  if(strcmp(wc_so,str_IP)==0)
					{selected=1; 
		                        prior=select_from_list[i].priority;
			                break;} 
				  }
				else
				  {                            /* wildcard */
     				  if(strncmp(wc_so,str_IP,strlen(str_IP))==0)
				 	{selected=1; 
		              		prior=select_from_list[i].priority;
			      		break;} 
				  }
			        if(str_IP[0]==0)
					{selected=1; break;};
				}
			  }

			for(i=(select_to_length-1);i>=0;i--)
			  {
			  if(prior > select_to_list[i].priority) break;
			  str_IP = select_to_list[i].host;
#ifdef DEBUG_ONSCREEN
printf("ST: %s  %d   Prior:%d\n",str_IP,select_to_list[i].port,select_to_list[i].priority);
#endif
		          if((select_to_list[i].port==0) || 
			     (select_to_list[i].port==ntohs(tcphead.destination)))
				{
				if(select_to_list[i].wildcard==0)
				  {                          /* NO wildcard */ 
     				  if(strcmp(wc_dest,str_IP)==0)
					{selected=1;
		                        prior=select_to_list[i].priority;
			                break;} 
				  }
				else
				  {                            /* wildcard */
     				  if(strncmp(wc_dest,str_IP,strlen(str_IP))==0)
				 	{selected=1; 
		              		prior=select_to_list[i].priority;
			      		break;} 
				  }
			        if(str_IP[0]==0)
					{selected=1; break;};
			        }
			  }

			for(i=(deselect_from_length-1);i>=0;i--)
			  {
			  if(prior > deselect_from_list[i].priority) break;
			  str_IP = deselect_from_list[i].host;
#ifdef DEBUG_ONSCREEN
printf("DF: %s  %d   Prior:%d\n",str_IP,deselect_from_list[i].port,deselect_from_list[i].priority);
#endif
		          if((deselect_from_list[i].port==0) || 
			     (deselect_from_list[i].port==ntohs(tcphead.source)))
				{
				if(deselect_from_list[i].wildcard==0)
				  {                          /* NO wildcard */ 
     				  if(strcmp(wc_so,str_IP)==0)
					{selected=0; 
		                        prior=deselect_from_list[i].priority;
			                break;} 
				  }
				else
				  {                            /* wildcard */
     				  if(strcmp(wc_so,str_IP)==0)
				 	{selected=0; 
		              		prior=deselect_from_list[i].priority;
			      		break;} 
				  }
			        if(str_IP[0]==0)
					{selected=0; break;};

			        }
			  }

			for(i=(deselect_to_length-1);i>=0;i--)
			  {
			  if(prior > deselect_to_list[i].priority) break;
			  str_IP = deselect_to_list[i].host;
#ifdef DEBUG_ONSCREEN
printf("DT: %s  %d   Prior:%d\n",str_IP,deselect_to_list[i].port,deselect_to_list[i].priority);
#endif
		          if((deselect_to_list[i].port==0) || 
			     (deselect_to_list[i].port==ntohs(tcphead.destination)))
				{
				if(deselect_to_list[i].wildcard==0)
				  {                          /* NO wildcard */ 
     				  if(strncmp(wc_dest,str_IP,strlen(str_IP))==0)
					{selected=0; 
		                        prior=deselect_to_list[i].priority;
			                break;} 
				  }
				else
				  {                            /* wildcard */
     				  if(strncmp(wc_dest,str_IP,strlen(str_IP))==0)
				 	{selected=0; 
		              		prior=deselect_to_list[i].priority;
			      		break;} 
				  }
			        if(str_IP[0]==0)
					{selected=0; break;};

			        }
			  }

#ifdef DEBUG_ONSCREEN
printf("Selected: %d\n",selected);
#endif
                       	if(selected==0) return DONT_EXAMINE; 

			}
		  }

		if( DEST_PORT && ntohs(tcphead.destination) != DEST_PORT) 
			return DONT_EXAMINE; 	/* Check dest. PORT */

                          /* inet_ntoa gave wrong output sometimes */
		sprintf(file,"%u.%u.%u.%u.%u-%u.%u.%u.%u.%u",
					so[0],so[1],so[2],so[3],
					(unsigned short)ntohs(tcphead.source),
					dest[0],dest[1],dest[2],dest[3],
					(unsigned short)ntohs(tcphead.destination));
		sprintf(file2,"%u.%u.%u.%u.%u-%u.%u.%u.%u.%u",
					dest[0],dest[1],dest[2],dest[3],
					(unsigned short)ntohs(tcphead.destination),
					so[0],so[1],so[2],so[3],
					(unsigned short)ntohs(tcphead.source));

		if ((ntohs(tcphead.offset_flag) & FIN) != 0)  
						/* check for reset conn. */
			return TCP_FINISH;            /* packet is a FIN */

		/*
		Used to be for speed, '-x' needs all info, so this too!
		if (info->DATA_len == 0)
  			return DONT_EXAMINE;        
		*/

		return TCP_EXAMINE;                 /* interprete packet */
		};
	if((proto==ICMP)&&(PROTOCOLS&F_ICMP))      /* ICMP packet checking */
		{
		memcpy(&icmphead,(sp+PROTO_HEAD+info->IP_len),
						sizeof(struct ICMP_header));
		memcpy(detail,&icmphead,sizeof(struct ICMP_header));
		sprintf(file,"%u.%u.%u.%u > %u.%u.%u.%u",
					so[0],so[1],so[2],so[3],
					dest[0],dest[1],dest[2],dest[3]);
		return ICMP_EXAMINE;
		};
	if((proto==UDP)&&(PROTOCOLS&F_UDP))       /* UDP packet checking */
		{
		memcpy(&udphead,(sp+PROTO_HEAD+info->IP_len),
						sizeof(struct UDP_header));
		memcpy(detail,&udphead,sizeof(struct UDP_header));
		sprintf(file,"%u.%u.%u.%u.%u-%u.%u.%u.%u.%u",
					so[0],so[1],so[2],so[3],
					ntohs(udphead.source),
					dest[0],dest[1],dest[2],dest[3],
					ntohs(udphead.destination));
		return UDP_EXAMINE;
		}
	return DONT_EXAMINE;
}

/* Default Processing of packets */
pcap_handler packethandler(	u_char *ipaddrpoint, 
			const struct packetheader *p_header, 
			const u_char *sp) 
{ 
	char filename[50], filename2[50],header[SNAPLEN];
	FILE *f;
	struct file_info *dummy_pointer;
	u_char status=0;
	char *help;
	int dummy,finish;                    /* look out it's signed */
	u_long datalen, position, total_length, i, n, ipaddr;
	struct unwrap info;
        struct IP_header iphead;
	struct TCP_header tcphead;
	struct ICMP_header icmphead;
	struct IP_header iphead_icmp;
        struct UDP_header udphead;

	memcpy(&ipaddr,ipaddrpoint,sizeof(u_long));
   	
	finish=check_packet(ipaddr,p_header,sp,filename,filename2,&info,header,SNIFMODE); 
        if(PROTOCOLS & F_IP)
                memcpy(&iphead,(sp+PROTO_HEAD),sizeof(struct IP_header)),
							print_iphead(&iphead,0);
        if(finish==DONT_EXAMINE) 
		return;                         /* Packet is not for us */

	if((DUMPMODE & 32)&&(finish<10))	/* extended info TCP only */
		{
		memcpy(&tcphead,header,sizeof(struct TCP_header));
		dummy=ntohs(tcphead.offset_flag);  
		printf("\n");
		printf("TCP Packet ID (from_IP.port-to_IP.port): %s\n",filename);
		printf("   SEQ (hex): %lX   ",ntohl(tcphead.seq_nr));
		if(dummy&ACK) printf("ACK (hex): %lX\n",ntohl(tcphead.ACK_nr));
		printf("   FLAGS: %c%c%c%c%c%c",
				(dummy&URG)?'U':'-',(dummy&ACK)?'A':'-',
				(dummy&PSH)?'P':'-',(dummy&RST)?'R':'-',
				(dummy&SYN)?'S':'-',(dummy&FIN)?'F':'-');
		if(dummy&ACK) 
			printf("   Window: %X\n",ntohs(tcphead.window));
		else 	printf("\n");
		};

	if((finish<10)&&(LOGLEVEL==0))          	/* TCP packet */
		/* I didn't use flags for later extention, and they */ 
		/* don't come in pairs anyhow */
		/* use return instead of else {if}, for later extention */
		{
		memcpy(&tcphead,header,sizeof(struct TCP_header));
		switch(DUMPMODE & 223) 
		  {
		  case 0:                     /* LOG mode */
		  status=0;

	  	  dummy_pointer=search_dynam(filename, TCP);
		  if(dummy_pointer!=NULL) status=1; 
		  /* make a new entry unless it's reset */
		  if(status==0)               
		  	{
  			if(finish==TCP_FINISH) return;      
					/* there was never data transmitted */
			/* seq_nr & datalen not important here yet */
			if((dummy_pointer=add_dynam(filename, TCP, 1, 0, 0))==NULL)
				return;
			}
		  f=dummy_pointer->f;

		  if(dummy_pointer->bytes<=SNIFLEN) {
			const u_char *data = 
					sp+PROTO_HEAD+info.IP_len+info.TCP_len;
			if(SNIFLEN!=0)
				dummy_pointer->bytes+=info.DATA_len;       
						/* last packet is written  */
                                                /* don't care about length */
			if(ASC==0)
				{for(i=0;i<info.DATA_len;i++) 
					fprintf(f,"%c", data[i]);}
			else    {
				for(i=0;i<info.DATA_len;i++)
					fprintf(f,"%c", isprint(data[i])?data[i]:non_printable);
				};
  			fflush(f);                                                  /* write all */
			}
		  if(finish==TCP_FINISH) /* let's reset the connection */
			delete_dynam(filename, TCP, 1);
		  break;
		
		  case 1:         /* DUMP mode */
	  	  case 2:
		  case 3:
		  	printf("Packet ID (from_IP.port-to_IP.port): %s\n",filename);
			total_length=info.IP_len+info.TCP_len+info.DATA_len;
			n = 0;
			for(i=0;i<total_length;i++) {
				u_char c = sp[PROTO_HEAD+i];
				if(n > 75)
					n=0, printf("\n");
				if(DUMPMODE & 1)
					n += printf(" %02X",c); 
				if(DUMPMODE & 2)
					n += printf(" %c",isprint(c)?c:'.');
			}
			printf("\n\n");
			break;
		
  		  default: 
			printf("\nImpossible error! Sniffer Hartattack!\n");
			exit(0); 
		  }
	      	return;
		}

	if((finish<10)&&(LOGLEVEL!=0))        /* TCP packet - logfile   */
		/* This mode will grow, so I just copied the other if() */
		/* instead of adding a dumpmode, I think this will keep */
		/* things more simpel. Also I use the smart dynam       */ 
		/* managment of connections                             */
		{
		memcpy(&tcphead,header,sizeof(struct TCP_header));
	  	
		dummy_pointer=search_dynam(filename, TCP);
		if(dummy_pointer!=NULL) status=1; 
		/* make a new entry unless it's reset */
		if(status==0)               
		  	{
  			if(finish!=TCP_FINISH) 
				if((dummy_pointer=add_dynam(filename, TCP, 0,ntohl(tcphead.seq_nr),info.DATA_len))==NULL)
					return;
			}

		if(finish==TCP_FINISH) /* let's reset the connection */
			{delete_dynam(filename, TCP, 0);}
                /*** return before using any search or pointer!!!!!!!! ***/
                /* Connections with FIN have deleted entries! */
 
		dummy=ntohs(tcphead.offset_flag);
	
		if(LOGLEVEL<10)          /* Raw logging */
		  {
         	  if(dummy&SYN)	
         		print_conn(filename,"Connection initiated. (SYN)");
         	  if(dummy&FIN)   
         		print_conn(filename,"Connection ending. (FIN)");
         	  if(dummy&RST)   
         		print_conn(filename,"Connection reset. (RST)");
		  return;
		  }
		if(LOGLEVEL<30)          /* more sophisticated */
		  {
		  const u_char *data = sp+PROTO_HEAD+info.IP_len+info.TCP_len;

		  if(search_dynam(filename2, TCP)==NULL)
         	     {
		     if(dummy&SYN)	
         			print_conn(filename,"Connection initiated.");
	
         	     if(dummy&FIN)   
         			print_conn(filename2,"Connection closed.");
				
		     }
         	  if(dummy&FIN) return; /* needed, cauz entry don't exist  */  
		  if(LOGLEVEL>=12)            /* loglevel 12 */
		    {
		    dummy_pointer=search_dynam(filename, TCP);
                    /* don't forget to check dummy_pointer!!! */
			
		    if( (ntohs(tcphead.source) == 23) &&
				         (dummy_pointer!=NULL))/* from telnet */
		      {
		      sbuf_update(dummy_pointer,ntohl(tcphead.seq_nr),data,info.DATA_len);
		      /* detect login */
		      strlower(dummy_pointer->scroll_buf);

		      if((dummy_pointer->log!=99)&&(strstr(dummy_pointer->scroll_buf,"login")!=NULL))
		 	{
			dummy_pointer->log=99;
			dummy_pointer=search_dynam(filename2, TCP);
                        if(dummy_pointer!=NULL)
				{dummy_pointer->log=1; 
				dummy_pointer->bytes=0;
				dummy_pointer->buffer=malloc(LOG_PASS_BUF);
				if(dummy_pointer->buffer==NULL) exit(1);
				dummy_pointer->buffer[0]=0;
				}
			else	{
				print_conn(filename2,"Password missed due to overload.");
				};
#ifdef DEBUG_ONSCREEN
			printf("Login detected, data initialised.\n");
#endif
			}
 		      }

		    if( (ntohs(tcphead.destination) == 23) &&
				         (dummy_pointer!=NULL))/* TO telnet */
		      {
		      if(dummy_pointer->log==1)	/* log login */
			{
			record_buf(dummy_pointer,ntohl(tcphead.seq_nr),data,info.DATA_len,0);
         		if(dummy_pointer->log==2) /* login recorded */ 
				{
                                print_login(filename,dummy_pointer->buffer);
	         		dummy_pointer->log=3; dummy_pointer->bytes=0;
	         		dummy_pointer->buffer[0]=0;
				}	
			}
		      else
			{
		        if(dummy_pointer->log==3)	/* log pwd */
			  {
			  record_buf(dummy_pointer,ntohl(tcphead.seq_nr),data,info.DATA_len,0);
         		  if(dummy_pointer->log==4) /* passwd recorded */ 
				{
                                print_pwd(filename,dummy_pointer->buffer);
	         		dummy_pointer->log=0; dummy_pointer->bytes=0;
	         		dummy_pointer->buffer[0]=0;
				}	
			   }
			}
 		      }
                    }

		    if( (ntohs(tcphead.destination) == 21) &&
				         (dummy_pointer!=NULL))/* to FTP */
		      {
		      sbuf_update(dummy_pointer,ntohl(tcphead.seq_nr),data,info.DATA_len);
		      /* detect USER en PASS  */
		      if((help=strstr(dummy_pointer->scroll_buf,"USER"))!=NULL)
		 	{
			help+=strlen("USER ");
			for(i=0;i<SCBUF;i++)
				if(dummy_pointer->scroll_buf[i]==FTP_ENTER)
					dummy_pointer->scroll_buf[i]=0;
			print_ftp_user(filename,help);
			for(i=0;i<SCBUF;i++)	dummy_pointer->scroll_buf[i]=' ';       
			}
		      if((help=strstr(dummy_pointer->scroll_buf,"PASS"))!=NULL)
		 	{
			help+=strlen("PASS ");
			for(i=0;i<SCBUF;i++)
				if(dummy_pointer->scroll_buf[i]==FTP_ENTER)
					dummy_pointer->scroll_buf[i]=0;
			print_ftp_pass(filename,help);
			for(i=0;i<SCBUF;i++)	dummy_pointer->scroll_buf[i]=' ';       
			}

 		      }


		  if( (dummy_pointer=search_dynam(filename, TCP)) !=NULL)
		    	{
			if(ntohl(tcphead.seq_nr)==dummy_pointer->exp_seq)
		       		dummy_pointer->exp_seq+=info.DATA_len;
		  /* if we miss a packet... no probs seq nr's get updated */
		  /* cauz' we can't rely on ACK's from other side         */
	  	  /* it's pretty good this way                            */
			if(ntohl(tcphead.seq_nr)>dummy_pointer->exp_seq)
		       		dummy_pointer->exp_seq=ntohl(tcphead.seq_nr)+info.DATA_len;
			}
		  return;
		  }
		return;  /* DON'T FORGET THEM!!!! */
		}
	
	if(finish<20)			/* ICMP packet */
		{
		memcpy(&icmphead,header,sizeof(struct ICMP_header));
		memcpy(&iphead_icmp,
			(sp+PROTO_HEAD+info.IP_len+ICMP_HEADLENGTH+4),
			sizeof(struct IP_header));
		printf("ICMP message id: %s\n",filename);
		printf("  ICMP type: ");
		switch(icmphead.type)
			{
			case 0: printf("%s\n",ICMP_TYPE_0);
				break;
			case 3: printf("%s\n",ICMP_TYPE_3);
				printf("  Error: %s\n",
					ICMP_type_3_code[icmphead.code]);
				print_iphead(&iphead_icmp,1);
				break;
			case 4: printf("%s\n",ICMP_TYPE_4);
				print_iphead(&iphead_icmp,1);
				break;
			case 5: printf("%s\n",ICMP_TYPE_5);
				printf("  Error: %s\n",
					ICMP_type_5_code[icmphead.code]);
				print_iphead(&iphead_icmp,1);
				break;
			case 8: printf("%s\n",ICMP_TYPE_8);
				break;
			case 11:printf("%s\n",ICMP_TYPE_11);
				printf("  Error: %s\n",
					ICMP_type_11_code[icmphead.code]);
				print_iphead(&iphead_icmp,1);
				break;
			case 12:printf("%s\n",ICMP_TYPE_12);
				print_iphead(&iphead_icmp,1);
				break;
			case 13:printf("%s\n",ICMP_TYPE_13);
				break;
			case 14: printf("%s\n",ICMP_TYPE_14);
				break;
			case 15: printf("%s\n",ICMP_TYPE_15);
				break;
			case 16: printf("%s\n",ICMP_TYPE_16);
				break;
			case 17: printf("%s\n",ICMP_TYPE_17);
				break;
			case 18: printf("%s\n",ICMP_TYPE_18);
				break;
			default: printf("Unknown ICMP type!\n");break;
		 	}
		printf("\n");
		return;
		}
	if(finish<30)                   /* nothing yet */
		{
 		memcpy(&udphead,header,sizeof(struct UDP_header));
		switch(DUMPMODE & 223) 
		  {
		  case 0: break;
		  case 1:         /* DUMP mode */
	  	  case 2:
		  case 3:
		  	printf("UDP Packet ID (from_IP.port-to_IP.port): %s\n",filename);
			total_length=info.IP_len+info.UDP_len+info.DATA_len;
			n = 0;
			for(i=0;i<total_length;i++) 
				{
				u_char c = sp[PROTO_HEAD+i];
				if(n > 75)
					n=0, printf("\n");
				if(DUMPMODE & 1)
					n += printf(" %02X",c); 
				if(DUMPMODE & 2)
					n += printf(" %c",isprint(c)?c:'.');
				}
			printf("\n\n");
			break;
  		  default: 
			printf("\nImpossible error! Sniffer Hartattack!\n");
			exit(0);
		  } 
		return;
		}
}


#ifdef INCLUDE_INTERFACE                         /* Interactive packethandling */ 
int check_mask (const struct packetheader *p_header, 
		const unsigned char *sp,
		char *conn_name,
		struct unwrap *info)
                                          /* return -1 : packet not for us */
                                          /* else finish value             */
{
        u_char *so,*dest;
	struct IP_header iphead;
	struct TCP_header tcphead;
	int proto;

	proto=unwrap_packet(sp, info);

	(*IP_nr_of_packets)++;
	if(proto==ICMP)  
		{(*ICMP_nr_of_packets)++; return DONT_EXAMINE;}
	if(proto==UDP)  
		{(*UDP_nr_of_packets)++; 
		(*UDP_bytes_in_packets)+=(info->UDP_len+info->DATA_len+info->IP_len);
		return DONT_EXAMINE;} 
	if(proto!=TCP)  return DONT_EXAMINE;  
	/* Packet info */
	(*TCP_nr_of_packets)++;
	(*TCP_bytes_in_packets)+=(info->TCP_len+info->DATA_len+info->IP_len);
							/* Not a TCP packet */ 
	memcpy(&iphead,(sp+PROTO_HEAD),sizeof(struct IP_header));
	memcpy(&tcphead,(sp+PROTO_HEAD+info->IP_len),sizeof(struct TCP_header));

        if(mask->source_ip!=0 && iphead.source!=mask->source_ip)
		return DONT_EXAMINE;
        if(mask->destination_ip!=0 && iphead.destination!=mask->destination_ip)
		return DONT_EXAMINE;
	if(mask->destination_port && ntohs(tcphead.destination) != mask->destination_port) 
		return DONT_EXAMINE; 
	if(mask->source_port && ntohs(tcphead.source) != mask->source_port) 
		return DONT_EXAMINE; 

                          /* inet_ntoa gave wrong output sometimes */
	so=(u_char *)&(iphead.source);
        dest=(u_char *)&(iphead.destination);
	sprintf(conn_name,"from %u.%u.%u.%u %u to %u.%u.%u.%u %u",
				so[0],so[1],so[2],so[3],
				ntohs(tcphead.source),
				dest[0],dest[1],dest[2],dest[3],
				ntohs(tcphead.destination));

	if ((ntohs(tcphead.offset_flag) & FIN) != 0)/* check for reset conn. */
		return TCP_FINISH;                        /* packet is a FIN */
	if (info->DATA_len == 0)
  		return DONT_EXAMINE;                   /* packet not for us */
	return TCP_EXAMINE;                            /* interprete packet */
}

pcap_handler interactive_packethandler(	char *dummy, 
			 	const struct packetheader *p_header, 
				const unsigned char *sp) 
{ 
	char conn_name[CONN_NAMELEN];
	int finish;                    /* look out it's signed */
	struct unwrap info;

	finish=check_mask(p_header,sp,conn_name,&info); 
	if(finish==DONT_EXAMINE) return;         /* Packet is not for us */

	if(finish!=TCP_FINISH) /* finish: already logged, or to short to add */
		add_itemlist(running_connections,conn_name);
	if(strcmp(log_conn->log_enter, conn_name)==0)
		{
                const u_char *data=sp+PROTO_HEAD+info.IP_len+info.TCP_len;
 		if(*DATAlength+info.DATA_len < LENGTH_OF_INTERPROC_DATA)
			{
			memcpy((connection_data+*DATAlength),data,info.DATA_len);
			*DATAlength+=info.DATA_len;
			}             
		}
	if(finish==TCP_FINISH)
		del_itemlist(running_connections,conn_name);
	kill(getppid(),SIGUSR1);
}
#endif

static u_long getaddrbyname(char *name)
{
    struct hostent *he;

    if(isdigit(*name))
	return inet_addr(name);
    if(!(he = gethostbyname(name)))
        {
#ifdef DEBUG
	debug_msg("GetAddr: Couldn't get host.");
#endif
	/* perror(name); */
	}

    return he ? *(long*)*he->h_addr_list : 0;
}

int main(int argc,char *argv[])
{
	char *dev, forced_dev[20], buffer[SNAPLEN];
	int c;
	u_long ipaddr, memsize;
	int flag=0, doboth=0, FORCE_DEV=0;
	extern char *optarg;

	signal (SIGINT, my_exit);       /* controled CTRL-C exit */
	SNIFLEN=300;                             /* Set defaults */
	DEST_PORT=0;                             /* Dest Port    */
	SNIFMODE=DUMPMODE=PROTOCOLS=ASC=WILDCARD=CFG_FILE=NO_CHKSUM=0;
	LOGLEVEL=0;
	Logfile[0]=0;
	IP=logging_device=NULL;  

#ifdef DEBUG
	if((debug_dev=fopen(DEBUG_DEVICE,"a"))<0)
		{printf("Couldn't open DEBUG device!\n");exit(0);}
	else
		{
		fprintf(debug_dev,"\n\nDEVICE OPENED FOR SNIFFIT DEBUGGING\n\n");
		exit_func(close_debug_device);	
		}
#endif

#ifdef INCLUDE_INTERFACE        
	while((c=getopt(argc,argv,"D:A:P:idp:l:xabt:s:F:c:nvL:"))!=-1) { 
#else
	while((c=getopt(argc,argv,"A:P:dp:l:xabt:s:F:c:nvL:"))!=-1) { 
#endif
                                                    /* Argument treating */
  		switch(c) {
			case 'v':
				printf("Sniffit Version %s - Copyright 1996 Brecht Claerhout\n",VERSION);
#ifdef __DATE__
				printf("Binary build %s %s\n",__DATE__,__TIME__);
#endif
				exit(0);
				break;
			case 'd':
				DUMPMODE|=1;
				break;
			case 'a':
				DUMPMODE|=2;
				break;
			case 'x':
				DUMPMODE|=32;
				break;
			case 'p':
				DEST_PORT=atoi(optarg);
				break;
			case 'l':
				SNIFLEN=atol(optarg);
				break;
			case 'L':
				LOGLEVEL=atoi(optarg);
				break;
			case 'b':
				doboth=1;
				break;
			case 'A':
				ASC=1;
				non_printable=*optarg;
				break; 
			case 'D':
				logging_device=optarg;
				break; 
			case 'P':
				optarg=strlower(optarg);
				if(strstr(optarg,"tcp")) PROTOCOLS |= F_TCP;
				if(strstr(optarg,"icmp")) PROTOCOLS |= F_ICMP;
				if(strstr(optarg,"udp")) PROTOCOLS |= F_UDP;
                                if(strstr(optarg,"ip")) PROTOCOLS |= F_IP;
		                break;
			case 's':
				flag++;
				SNIFMODE=SOURCE;
				IP=optarg;
				break; 
			case 't':
				flag++;
				SNIFMODE=DEST;
				IP=optarg;
				break; 
                        case 'i':
                                flag++;
				SNIFMODE=INTERACTIVE;
                                break;
                        case 'n':
				NO_CHKSUM=1;
                                break;
			case 'F':
				strcpy(forced_dev,optarg);
                                FORCE_DEV=1;
				break;
			case 'c':
				flag++;
				read_cfg_file(optarg); 
#ifdef DEBUG_ONSCREEN
				printf("FINISHED INTERPRETING\n");
				printf("sf:%d st:%d df:%d dt:%d\n",select_from_length,select_to_length, deselect_from_length, deselect_to_length);
#endif
                                CFG_FILE=1;
				break;
      			default : break;
		}
	}
	if (getuid()!=0)
		printf("You should be root to run this program!\n"), exit(1);
	if(flag!=1) 	quit(argv[0]);
	if( (LOGLEVEL!=0) && (CFG_FILE==0) )	quit(argv[0]);

	if(LOGLEVEL != 0)	open_logfile();
	if(PROTOCOLS==0) PROTOCOLS |= F_TCP;
	if(doboth) SNIFMODE=BOTH;
        if((SNIFMODE!=INTERACTIVE)&&(CFG_FILE==0))  
		{
/*		if(index(IP,'x')) For SunOS  */
		if(strchr(IP,'x'))
		  {printf("Wildcard detected, IP nr. not checked...\n");
		  WILDCARD=1;
/*		  strcpy(index(IP,'x'),"\0"); */
		  strcpy(strchr(IP,'x'),"\0");
		  }
		else
		  {
		  ipaddr = getaddrbyname(IP);
		  if(ipaddr==0) 
			printf("Non existing host!\n"), exit(1);
		  }
		}
	reset_all();       /* just to be sure */

	if( (dev=pcap_lookupdev(NULL))==NULL )  
		{
		printf("No network devices found.... Sniffit giving up.\n");
		exit(1);
		}
	
	if(FORCE_DEV!=0)
		{
		strcpy(dev,forced_dev);
		printf("Forcing device to %s (user requested)...\n",dev);
		printf("Make sure you have read the docs carefully.\n");
		PROTO_HEAD=FORCED_HEAD_LENGTH;
	      	}

	if(strstr(dev,ETH_DEV))                /* For expansion */
		{PROTO_HEAD=ETHERHEAD;
		printf("Supported ethernet device found. (%s)\n",dev); 
		}

	if(strstr(dev,PPP_DEV))               
		{PROTO_HEAD=PPPHEAD;
		printf("Supported PPP device found. (%s)\n",dev); 
		}

	if((dev_desc=pcap_open_live(dev,SNAPLEN,1,MSDELAY,NULL))==NULL)
		{printf("Couldn't open device.\n");
		exit(0);}

#ifdef INCLUDE_INTERFACE
        if (SNIFMODE==INTERACTIVE)
 		{
		memsize=sizeof(int)+sizeof(int)+LENGTH_OF_INTERPROC_DATA+
			sizeof(int)+sizeof(struct snif_mask)+
			sizeof(struct shared_logged_conn)+
			(CONNECTION_CAPACITY*sizeof(struct shared_conn_data))+
			sizeof(int)+
			sizeof(long)+sizeof(int)+sizeof(int)+sizeof(long)+
			sizeof(int);
		memory_id = shmget(0,memsize,0700);
		if(memory_id<0)
  			{perror("Interactive Sniffer Hartattack (No Shared mem avail!)");
			exit(0);}
		exit_func(mem_exit);
		if((SHARED=shmat(memory_id,0,SHM_RND))==NULL)
  			{perror("Interactive Sniffer Hartattack (Wow something is wrong here)");
   			exit(0);};
                printf("Entering Shared memory at %p\n",SHARED);
  		printf("Shared %d\n",memsize);

		timing = SHARED;                    /* set all pointers */
		DATAlength = timing + sizeof(int);
		connection_data = DATAlength + sizeof(int);
		LISTlength = connection_data + LENGTH_OF_INTERPROC_DATA;
		mask = LISTlength + sizeof(int);
		logged_connections = mask + sizeof(struct snif_mask);
		log_conn=(struct shared_logged_conn *)logged_connections;
		running_connections = logged_connections + sizeof(struct shared_logged_conn);
		TCP_nr_of_packets= running_connections+(sizeof(struct shared_conn_data)*CONNECTION_CAPACITY);
		TCP_bytes_in_packets= TCP_nr_of_packets+sizeof(int); 
		ICMP_nr_of_packets= TCP_bytes_in_packets+sizeof(long); 
		UDP_nr_of_packets= ICMP_nr_of_packets+sizeof(int); 
		UDP_bytes_in_packets= UDP_nr_of_packets+sizeof(int); 
		IP_nr_of_packets= UDP_bytes_in_packets+sizeof(long); 
		clear_shared_mem(0);

		if ((Pid=fork())<0)
  			{perror("Interactive Sniffer Hartattack (Couldn't fork)");
   			exit(0);};
		if(Pid==0)
			{
			sleep(4);
			while(1)
  			  if(pcap_dispatch(dev_desc,PACKETS,
					interactive_packethandler,NULL)<0)
	    			printf("Capturing Packets Failed\n"), exit(0);
			}
		else	{
                        exit_func(child_exit);
			signal(SIGCHLD,SIG_IGN);
			if(logging_device != NULL)
				{
				if(stat(logging_device,&log_dev_stat)<0)
					perror("\'-D\' option error"),exit(0);
				if((log_dev_stream=fopen(logging_device,"a")) 
									== NULL)
					printf("Couldn't open device for logging output\n"),exit(0);
				}
			run_interface();
			}
		}
 	else 	{
#endif
		if(CFG_FILE==0)
		  printf("Sniffit.%s is up and running.... (%s)\n\n",VERSION,IP);
		else 
		  printf("Sniffit.%s is up and running.... (Config File Used)\n\n",VERSION);
		while(1)
  		  if(pcap_dispatch(dev_desc,PACKETS,packethandler,(u_char *)&ipaddr)<0)
    			printf("Capturing Packets Failed\n"), exit(0);
#ifdef INCLUDE_INTERFACE
		}
#endif	
/* Close device?  Nahhh.... fuck it! we don't get here anyway!*/
}

