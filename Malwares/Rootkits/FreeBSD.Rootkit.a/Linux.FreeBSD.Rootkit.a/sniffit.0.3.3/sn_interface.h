/* Sniffit Interface File -- Brecht Claerhout                               */

/*** Screen Parameters ***/
int MASK_WINDOW_ROWS, MASK_WINDOW_COLS;
int MAIN_WINDOW_ROWS, MAIN_WINDOW_COLS;
int INFO_WINDOW_ROWS, INFO_WINDOW_COLS;
int DATA_WINDOW_ROWS, DATA_WINDOW_COLS;
int INFO_WINDOW_X, INFO_WINDOW_Y;
int MASK_WINDOW_X, MASK_WINDOW_Y;
int DATA_WINDOW_X, DATA_WINDOW_Y;

/*** Sreen operations ***/
void init_screen (void)
{
initscr();
cbreak();
noecho();
nonl();
clear();
if(has_colors()!=-1)	
	{
  	COLOR_AVAIL=1;
 	start_color();
 	init_pair(WIN_COLOR_NORMAL,COLOR_WHITE,COLOR_BLUE);
  	init_pair(WIN_COLOR_POINT,COLOR_BLUE,COLOR_CYAN);
  	init_pair(WIN_COLOR_DATA,COLOR_BLUE,COLOR_CYAN);
 	init_pair(WIN_COLOR_INPUT,COLOR_BLUE,COLOR_CYAN);
  	init_pair(WIN_COLOR_MENU,COLOR_BLUE,COLOR_CYAN);
  	init_pair(WIN_COLOR_PACKET_INFO,COLOR_BLUE,COLOR_CYAN);
  	}
else 
	{
	COLOR_AVAIL=0;
	}
MAIN_WINDOW_ROWS = LINES-5; MAIN_WINDOW_COLS = COLS;

MASK_WINDOW_ROWS = 4; MASK_WINDOW_COLS = COLS;
MASK_WINDOW_X = 0; MASK_WINDOW_Y = LINES-5;

INFO_WINDOW_ROWS = 8; INFO_WINDOW_COLS = 35;
INFO_WINDOW_X = 3; INFO_WINDOW_Y = MAIN_WINDOW_ROWS-INFO_WINDOW_ROWS-2;

DATA_WINDOW_ROWS = (MAIN_WINDOW_ROWS/3)*2; DATA_WINDOW_COLS = (MAIN_WINDOW_COLS/3)*2; 
DATA_WINDOW_X = COLS-DATA_WINDOW_COLS-2; DATA_WINDOW_Y = 3; 
exit_func(screen_exit);
if( (COLS<80)||(LINES<18) )
	exit(0);
};

void f_box_window (struct box_window *Win, 
                 int num_lines, int num_cols, int begy,int begx, int col_mode) 
/*  col_mode : color selection   */
{
int i;

Win->main_window=newwin(num_lines,num_cols,begy,begx);
Win->work_window=subwin(Win->main_window,num_lines-2,num_cols-2,begy+1,begx+1);
if(COLOR_AVAIL)	
	{
	switch(col_mode)
		{
		case 0:
	  		wattrset(Win->main_window,COLOR_PAIR(WIN_COLOR_NORMAL));
	  		wattrset(Win->work_window,COLOR_PAIR(WIN_COLOR_NORMAL));
			break;
		case 1:
	 		wattrset(Win->main_window,COLOR_PAIR(WIN_COLOR_PACKET_INFO));
  			wattrset(Win->work_window,COLOR_PAIR(WIN_COLOR_PACKET_INFO));
			break;
		default:break;
		}
  	for(i=0;i<=(num_lines-2);i++)
		{
	  	wmove(Win->work_window,i,0);
		whline(Win->work_window,' ',num_cols-2);
		}
  	}
keypad(Win->work_window,1);
box(Win->main_window,ACS_VLINE,ACS_HLINE);
mvwprintw(Win->main_window,0,3,"Sniffit %s",VERSION);
wmove(Win->work_window,0,0);
wnoutrefresh(Win->main_window);wnoutrefresh(Win->work_window);
doupdate();
}

void data_window (struct box_window *Win, struct box_window *P_Win,
                 int num_lines, int num_cols, int begy,int begx,
                 char *buffer, int listitem) 
{
int i=0, j=0;	
struct shared_conn_data *conn;

conn = (struct shared_conn_data *) buffer;
while((j<listitem)&&(i<(CONNECTION_CAPACITY+1)))
	{
  	if(conn[i].connection[0]!=0)
    		j++;
	i++;
	}
while( (conn[i].connection[0]==0)&&(i<(CONNECTION_CAPACITY+1)) )	
	i++;
if(i>=CONNECTION_CAPACITY+1) return;
j=0;

Win->main_window=newwin(num_lines,num_cols,begy,begx);
Win->work_window=subwin(Win->main_window,num_lines-5,num_cols-2,begy+1,begx+1);
scrollok(Win->work_window,1);
if(COLOR_AVAIL)	
  	wattrset(Win->main_window,COLOR_PAIR(WIN_COLOR_DATA));

box(Win->main_window,ACS_VLINE,ACS_HLINE);
wmove(Win->main_window,num_lines-3,1);
whline(Win->main_window,ACS_HLINE,num_cols-2);
wmove(Win->main_window,num_lines-2,1);
whline(Win->main_window,' ',num_cols-2);
wmove(Win->main_window,num_lines-2,2);
waddstr(Win->main_window, conn[i].connection);
strcpy(log_conn->log_enter,conn[i].connection);
wmove(Win->work_window,0,0);
wnoutrefresh(Win->main_window);wnoutrefresh(Win->work_window);
doupdate();
}

void data_device (char *buffer, int listitem) 
{
int i=0, j=0;
struct shared_conn_data *conn;

conn = (struct shared_conn_data *) buffer;
while((j<listitem)&&(i<(CONNECTION_CAPACITY+1)))
	{
  	if(conn[i].connection[0]!=0)
    		j++;
	i++;
	}
while( (conn[i].connection[0]==0)&&(i<(CONNECTION_CAPACITY+1)) )	
	i++;
if(i>=CONNECTION_CAPACITY+1) return;
strcpy(log_conn->log_enter, conn[i].connection);
}

void mask_status (struct box_window *Work_win)
{
unsigned char *ad;
int i;

wmove(Work_win->work_window,0,1);
for(i=0;i<2;i++)
	{wmove(Work_win->work_window,i,0);
	whline(Work_win->work_window,' ',COLS-2);}
wmove(Work_win->work_window,0,1);
wprintw(Work_win->work_window,"Source IP     : ");
ad=&(mask->source_ip);
if(mask->source_ip==0)
 	wprintw(Work_win->work_window,"All");
else wprintw(Work_win->work_window,"%u.%u.%u.%u",ad[0],ad[1],ad[2],ad[3]);
wmove(Work_win->work_window,1,1);
wprintw(Work_win->work_window,"Destination IP: ");
ad=&(mask->destination_ip);
if(mask->destination_ip==0)
 	wprintw(Work_win->work_window,"All");
else wprintw(Work_win->work_window,"%u.%u.%u.%u",ad[0],ad[1],ad[2],ad[3]);
wmove(Work_win->work_window,0,35);
wprintw(Work_win->work_window,"Source PORT     : ");
if(mask->source_port==0)
  	wprintw(Work_win->work_window,"All");
else wprintw(Work_win->work_window,"%u",mask->source_port);
wmove(Work_win->work_window,1,35);
wprintw(Work_win->work_window,"Destination PORT: ");
if(mask->destination_port==0)
 	wprintw(Work_win->work_window,"All");
else wprintw(Work_win->work_window,"%u",mask->destination_port);
wnoutrefresh(Work_win->main_window);
wnoutrefresh(Work_win->work_window);
doupdate();
}

void fill_box_window (struct box_window *Work_win, char *buffer, 
                      int begin_item, int boxlen, int rowlen)
                                                 /* 0 is the first item  */
{
int i=0, j=0, line=0;
struct shared_conn_data *conn;

conn = (struct shared_conn_data *) buffer;
while((j<begin_item)&&(i<(CONNECTION_CAPACITY+1)))
	{
  	if(conn[i].connection[0]!=0)
    		j++;
	i++;
	}
if(i>=CONNECTION_CAPACITY+1) return;
j=0;

while((line<boxlen)&& ((i+j)<CONNECTION_CAPACITY) )	
	{
  	if(conn[i+j].connection[0] != 0)	
		{
		wmove(Work_win->work_window,line,0);
		whline(Work_win->work_window,' ',rowlen);
		if(strcmp(log_conn->log_enter,conn[i+j].connection) != 0) 
    			wprintw(Work_win->work_window," %s",conn[i+j].connection);
		else
    			wprintw(Work_win->work_window," %s           *LOGGED*",
						        conn[i+j].connection);

    		line++;
    		}
  	j++;
  	}
for(i=line;i<boxlen;i++)	
	{
  	wmove(Work_win->work_window,i,0);
	whline(Work_win->work_window,' ',rowlen);
  	};
wnoutrefresh(Work_win->work_window);
}

void point_item (struct box_window *Work_win, char *buffer, 
                 int item, int begin_item, int boxlen, int rowlen)
{
int i=0, j=0;
struct shared_conn_data *conn;


if(item<0) return;      /* POINTpos   0 = first item   -1 = no items */
                        /* LISTlength 0 = 1            -1 = no items */
                        /* DANGER - there should always be >=        */
                        /*          connections than 'item'          */
conn = (struct shared_conn_data *) buffer;
while((j<item)&&(i<(CONNECTION_CAPACITY+1)))
	{
	if(conn[i].connection[0] !=0)
    		j++;
	i++;
	}
while( (conn[i].connection[0]==0)&&(i<(CONNECTION_CAPACITY+1)) )	
	i++;
if(i>=CONNECTION_CAPACITY+1) return;
j=0;

#ifdef DEBUG
		debug_msg(conn[i].connection);
#endif

if(COLOR_AVAIL!=0)
	wattrset(Work_win->work_window,COLOR_PAIR(WIN_COLOR_POINT));
else 	wattron(Work_win->work_window,A_REVERSE); 
wmove(Work_win->work_window,item-begin_item,0);
whline(Work_win->work_window,' ',rowlen);

if(strcmp(log_conn->log_enter,conn[i].connection)!=0)
	mvwprintw(Work_win->work_window,item-begin_item,0,
						" %s", conn[i].connection );
else
	mvwprintw(Work_win->work_window,item-begin_item,0,
					        " %s           *LOGGED*",
                                             	conn[i].connection );
  
wnoutrefresh(Work_win->work_window);
if(COLOR_AVAIL!=0)
	wattrset(Work_win->work_window,COLOR_PAIR(WIN_COLOR_NORMAL));
else 	wattroff(Work_win->work_window,A_REVERSE); 
}

void forced_refresh (void)	
{
#ifdef DEBUG
char debug_line[200];
#endif

if((POINTpos<0)&&(*LISTlength>=0)) POINTpos=0;
if((POINTpos>*LISTlength)&&(*LISTlength>=0)) POINTpos=*LISTlength; 
if((POINTpos>*LISTlength)&&(*LISTlength<0)) POINTpos=-1; 

while(screen_busy!=0) {};    /* wait till screen operations stop */
#ifdef DEBUG
sprintf(debug_line,"FIX: POINTpos: %d  LISTlength: %d  LISTpos: %d\n",POINTpos,*LISTlength,LISTpos);
debug_msg(debug_line);
#endif

fill_box_window(&main_box, running_connections,LISTpos,
			               MAIN_WINDOW_ROWS-2,MAIN_WINDOW_COLS-2);
point_item(&main_box, running_connections, POINTpos,LISTpos,
			               MAIN_WINDOW_ROWS-2,MAIN_WINDOW_COLS-2);
if((LOGGING==1)&&(logging_device==NULL))	
	{
	touchwin(data_box.main_window);touchwin(data_box.work_window);
  	wnoutrefresh(data_box.main_window);wnoutrefresh(data_box.work_window);
   	}
if(PACKET_INFO==1)	
	{
	touchwin(packets_box.main_window);touchwin(packets_box.work_window);
  	wnoutrefresh(packets_box.main_window);
	wnoutrefresh(packets_box.work_window);
	}
doupdate();
}

void menu_line (void)
{
int i;

if(menu_window==NULL)
  	menu_window=newwin (1,COLS,LINES-1,0);
if(COLOR_AVAIL!=0)	
	{
	wattrset(menu_window,COLOR_PAIR(WIN_COLOR_MENU));
	wmove(menu_window,0,0);
	whline(menu_window,' ',COLS);
	}
mvwaddstr(menu_window,0,0,MENU);
wnoutrefresh(menu_window);
}

char *input_field(char *string, char *input)
{
int i;
WINDOW *Work_txt, *Work_inp;

#ifdef DEBUG
debug_msg("IntAct: Input Field activated");
#endif

Work_txt=newwin(1,COLS,LINES-1,0);
Work_inp=newwin(1,50,LINES-1,strlen(string));
if(COLOR_AVAIL!=0)
 	{
	wattrset(Work_inp,COLOR_PAIR(WIN_COLOR_INPUT));
  	wattrset(Work_txt,COLOR_PAIR(WIN_COLOR_NORMAL));
	whline(Work_txt,' ',COLS);
	whline(Work_inp,' ',50);
	}
mvwaddstr(Work_txt,0,0,string);
while(screen_busy!=0) {};   
wnoutrefresh(Work_txt);wnoutrefresh(Work_inp);
doupdate();
echo();mvwgetstr(Work_inp,0,0,input);noecho();
delwin(Work_inp);delwin(Work_txt);
menu_line();
forced_refresh();   
return input;
}

void exec_mask (void)
{
LISTpos=0;
POINTpos=-1;             /* otherwise we get never ending loop */
clear_shared_mem(1);
mask_status(&mask_box);
if(LOGGING==1) stop_logging();
forced_refresh();
}

/* signaling */

void sig_blocking(char on_off, int sig)
{
sigset_t set;

sigemptyset(&set);sigaddset(&set,sig);
if(on_off==1)
	{sigprocmask(SIG_BLOCK,&set,NULL);}
else	{sigprocmask(SIG_UNBLOCK,&set,NULL);}
}

void set_signal (int signum, sig_hand new_action)
{	
struct sigaction new_sigusr;
sigset_t sig_mask;

sigemptyset(&sig_mask);
sigaddset(&sig_mask,SIGUSR1);
sigaddset(&sig_mask,SIGALRM);
new_sigusr.sa_handler=new_action;
new_sigusr.sa_mask=sig_mask;	
new_sigusr.sa_flags=0;
sigaction(signum,&new_sigusr,NULL);
}

void interaction (int sig)              /* invoked when data arrives */
{
int i;
struct shared_conn_data *conn;


/* timeout increase */
conn = (struct shared_conn_data *) running_connections;
for(i=0;i<CONNECTION_CAPACITY;i++)
  	if(conn[i].connection[0]!=0)
		conn[i].timeout+=1;

if((LOGGING==1)&&(log_conn->log_enter[0]==0)) stop_logging();
screen_busy=1;   
if((LOGGING==1)&&(*DATAlength!=0))
  	{
	if(logging_device==NULL)
		{
	  	for(i=0;i<*DATAlength;i++)
    			waddch(data_box.work_window,
    			isprint(*(connection_data+i))?
					*(connection_data+i):'.');
		}
	else	{
  		for(i=0;i<*DATAlength;i++) 
   			fputc(*(connection_data+i),log_dev_stream);
		fflush(log_dev_stream);
		}
  	*DATAlength=0;
 	}
screen_busy=0;
forced_refresh();
set_signal(SIGUSR1,interaction); 
}

void packet_info_handler (int signum)
{
#ifdef DEBUG
		debug_msg("ALARM RANG");
#endif
screen_busy=1; 
mvwprintw(packets_box.work_window,0,1,"IP packets/sec.  : % 12u",(*IP_nr_of_packets)/INFO_TIMER);
mvwprintw(packets_box.work_window,1,1,"TCP packets/sec. : % 12u",(*TCP_nr_of_packets)/INFO_TIMER);
mvwprintw(packets_box.work_window,2,1,"ICMP packets/sec.: % 12u",(*ICMP_nr_of_packets)/INFO_TIMER);
mvwprintw(packets_box.work_window,3,1,"UDP packets/sec. : % 12u",(*UDP_nr_of_packets)/INFO_TIMER);
mvwprintw(packets_box.work_window,4,1,"bytes/sec. (TCP) : % 12ld",(*TCP_bytes_in_packets)/INFO_TIMER);
mvwprintw(packets_box.work_window,5,1,"bytes/sec. (UDP) : % 12ld",(*UDP_bytes_in_packets)/INFO_TIMER);
screen_busy=0; 

forced_refresh();
/* reinstall handler, reset alarm */
*IP_nr_of_packets=0; 
*TCP_nr_of_packets=*TCP_bytes_in_packets=0; 
*ICMP_nr_of_packets=0;
*UDP_nr_of_packets=*UDP_bytes_in_packets=0; 
set_signal(SIGALRM, packet_info_handler);
alarm(INFO_TIMER);
}


/* at/on_exit's  */

void child_exit (void)
{
kill(Pid,SIGKILL);
};

void screen_exit (void)
{
clear();
endwin();
};

void mem_exit (void)
{
if(shmctl(memory_id,IPC_RMID,0)<0)
  	{perror("Sniffer Hartattack (you are fucked!) ");exit(0);};
}

/* Some other stuff */

void stop_logging (void)
{	
LOGGING=0;
log_conn->log_enter[0]=0;
if(logging_device==NULL)
	{delwin(data_box.work_window); delwin(data_box.main_window);}
forced_refresh();
}

void stop_packet_info (void)
{	
PACKET_INFO=0;
alarm(0);
delwin(packets_box.work_window), delwin(packets_box.main_window);
forced_refresh();
}

int add_itemlist(char *buffer, char *string)
{
int i, to_help, to_item;
struct shared_conn_data *conn;

/*invoked every time a packet comes in */

conn = (struct shared_conn_data *) buffer;
for(i=0;i<CONNECTION_CAPACITY;i++)
	if(strcmp( conn[i].connection, string)==0)
		{
		conn[i].timeout=0;
		return -1;
		}
for(i=0;i<CONNECTION_CAPACITY;i++)
	if(conn[i].connection[0]==0)
    		{
		strcpy(conn[i].connection, string);
		conn[i].timeout=0;
    		(*LISTlength)++;
		return i;
    		}

/* everything full - timeout procedure  */
to_help=to_item=0;
for(i=0;i<CONNECTION_CAPACITY;i++)
	if(conn[i].timeout>to_help)
		if(strcmp(conn[i].connection,log_conn->log_enter)!=0)
			{
			to_help=conn[i].timeout;
			to_item=i;
			}
strcpy(conn[to_item].connection, string);
conn[to_item].timeout=0;
return to_item;
}

int del_itemlist(char *buffer, char *string)
{
int i;
struct shared_conn_data *conn;

conn = (struct shared_conn_data *) buffer;
for(i=0;i<CONNECTION_CAPACITY;i++)
	if(strcmp( conn[i].connection, string)==0)
    		{
		conn[i].connection[0]=0;
		conn[i].timeout=0;
    		(*LISTlength)--;
	        if(strcmp( log_conn->log_enter, string)==0) 
			{log_conn->log_enter[0]=0;}
		return i;
    		}
return -1;
}

void clear_shared_mem(char mode) 
			/* mode = 0    all               */
			/* mode = 1    keep mask         */
			/*             keep packet count */
{
int i;
struct shared_conn_data *conn;

*timing=1;
*DATAlength=0;
*LISTlength=-1;
if(mode==0)
  {
  mask->source_ip=mask->destination_ip=mask->source_port=mask->destination_port=0;
  *IP_nr_of_packets=0; 
  *TCP_nr_of_packets=*TCP_bytes_in_packets=0; 
  *ICMP_nr_of_packets=0;
  *UDP_nr_of_packets=*UDP_bytes_in_packets=0; 
  }

log_conn->log_enter[0]=0;
conn = (struct shared_conn_data *) running_connections;
for(i=0;i<CONNECTION_CAPACITY;i++)
	{
	conn[i].connection[0]=0;
	conn[i].timeout=0;
	} 
*timing=0;
};

void create_arguments(char *esource, char *es_port, char *edest, 
				        char *ed_port, char *buffer, int item)
{
char e_dummy[CONN_NAMELEN];
int i=0, j=0;
struct shared_conn_data *conn;

if(item<0) return;    
conn = (struct shared_conn_data *) buffer;
while((j<item)&&(i<(CONNECTION_CAPACITY+1)))
	{
  	if(conn[i].connection[0] !=0)
    			j++;
	i++;
	}
while( (conn[i].connection[0]==0)&&(i<(CONNECTION_CAPACITY+1)) )	
	i++;
if(i>=CONNECTION_CAPACITY+1) return;
j=0;

strcpy(e_dummy,conn[i].connection);
strtok(e_dummy," ");
strcpy(esource,strtok(NULL," "));
strcpy(es_port,strtok(NULL," "));
strtok(NULL," ");
strcpy(edest,strtok(NULL," "));
strcpy(ed_port,strtok(NULL," "));
}

/*** Main interface program */

void run_interface(void)
{
int i,key_hit;
char dummy[50];
char exec_s[20],exec_sp[20],exec_d[20],exec_dp[20];  
	

POINTpos=-1;
*LISTlength=-1;
LISTpos=0;
LOGGING=0;
PACKET_INFO=0;
screen_busy=0;


set_signal (SIGCHLD, SIG_IGN);
set_signal(SIGUSR1,interaction);

init_screen();	        	/* The whole screen setup */
f_box_window(&mask_box,MASK_WINDOW_ROWS,MASK_WINDOW_COLS,MASK_WINDOW_Y,MASK_WINDOW_X,0);
mask_status(&mask_box);
f_box_window(&main_box,MAIN_WINDOW_ROWS,MAIN_WINDOW_COLS,0,0,0);
fill_box_window(&main_box, running_connections,LISTpos,
			               MAIN_WINDOW_ROWS-2,MAIN_WINDOW_COLS-2);
point_item(&main_box, running_connections,POINTpos,LISTpos,
			               MAIN_WINDOW_ROWS-2,MAIN_WINDOW_COLS-2);
menu_window=NULL;
menu_line();
doupdate();                      /* And..... draw it! */

while(1)
  	{
 	key_hit=wgetch(main_box.work_window);
#ifdef DEBUG
	debug_msg("IntAct: Key Hit Received");
#endif
	sig_blocking(1, SIGALRM);
	sig_blocking(1, SIGUSR1);
 	switch(key_hit)
    		{
    		case KEY_DOWN:
		case 'J':
		case 'j':
       			if(POINTpos>=*LISTlength) break;
       			if( POINTpos<(LISTpos+(MAIN_WINDOW_ROWS-3)) )
         			POINTpos++;
      			else
        			{if(LISTpos>=*LISTlength) break; 
				LISTpos++; POINTpos++;};
			forced_refresh();
       			break;
   		case KEY_UP:
		case 'K':
		case 'k':
      			if(POINTpos==0) break;
       			if(POINTpos>LISTpos)
         			POINTpos--;
       			else
         			{if(LISTpos==0) break; 
				LISTpos--; POINTpos--;};
			forced_refresh();
       			break;
    		case ENTER:
       			if(*LISTlength<0) break; 
       			if(LOGGING==0)
        			{
         			if(logging_device==NULL)
				data_window(&data_box,&main_box,DATA_WINDOW_ROWS,
						DATA_WINDOW_COLS,DATA_WINDOW_Y, DATA_WINDOW_X,
						running_connections,POINTpos);
				else 
				  data_device(running_connections,POINTpos);
        			 
				LOGGING=1;
         			}
      			 else
         			{
         			stop_logging();
        			if(logging_device==NULL)
			        	data_window(&data_box,&main_box,DATA_WINDOW_ROWS,
						DATA_WINDOW_COLS,DATA_WINDOW_Y, DATA_WINDOW_X,
						running_connections,POINTpos);
				else 
			 	        data_device(running_connections,POINTpos);
				LOGGING=1;
         			};
       			break;
    		case 'N':
    		case 'n':
       			if(PACKET_INFO==0)
				{
				f_box_window(&packets_box,INFO_WINDOW_ROWS,INFO_WINDOW_COLS,
							INFO_WINDOW_Y, INFO_WINDOW_X ,1);
				PACKET_INFO=1;
				*IP_nr_of_packets=0; 
				*TCP_nr_of_packets=*TCP_bytes_in_packets=0; 
				*ICMP_nr_of_packets=0;
				*UDP_nr_of_packets=*UDP_bytes_in_packets=0; 
				packet_info_handler(SIGALRM);
				}
			else
				{stop_packet_info();}
       			break;
		case 'q':
		case 'Q':
    		case KEY_F(10):
       			 if(LOGGING==1)
          			{stop_logging();}
       			 else
				{kill(Pid,SIGKILL);exit(0);}
			break;
		case '1':
		case KEY_F(1):
       			input_field("Source Ip: ",dummy); 
			mask->source_ip=getaddrbyname(dummy);
			exec_mask();
			break;
		case '2':
    		case KEY_F(2):
       			input_field("Destination Ip: ",dummy);
			mask->destination_ip=getaddrbyname(dummy);
			exec_mask();
       			break;
		case '3':
   			case KEY_F(3):
       			input_field("Source Port: ",dummy);
                        mask->source_port=atoi(dummy);
			exec_mask();      	
		 	break;
		case '4':
    		case KEY_F(4):
       			input_field("Destination Port: ",dummy);
                        mask->destination_port=atoi(dummy);
			exec_mask();
       			break;

		case '5':
    		case KEY_F(5):
       			if(*LISTlength<0) break;        
			create_arguments(exec_s,exec_sp,exec_d,
					exec_dp,running_connections,POINTpos);
			if(fork()==0)     /* CHILD */
		 	  {
		 	  close(0); close(1); close(2);
		 	  execl("./sniffit_key5","sniffit_key5",exec_s,exec_sp,exec_d,exec_dp,NULL);
		          exit(0);
		 	  };			
			break;
		case '6':
    		case KEY_F(6):
       			if(*LISTlength<0) break;        
			create_arguments(exec_s,exec_sp,exec_d,
					exec_dp,running_connections,POINTpos);
			if(fork()==0)     /* CHILD */
		 	  {
		 	  close(0); close(1); close(2);
		 	  execl("./sniffit_key6","sniffit_key6",exec_s,exec_sp,exec_d,exec_dp,NULL);
		          exit(0);
		 	  };			
			break;
		case '7':
    		case KEY_F(7):
       			if(*LISTlength<0) break;        
			create_arguments(exec_s,exec_sp,exec_d,
					exec_dp,running_connections,POINTpos);
			if(fork()==0)     /* CHILD */
		 	  {
		 	  close(0); close(1); close(2);
		 	  execl("./sniffit_key7","sniffit_key7",exec_s,exec_sp,exec_d,exec_dp,NULL);
		          exit(0);
		 	  };			
			break;
		case '8':
    		case KEY_F(8):
       			if(*LISTlength<0) break;        
			create_arguments(exec_s,exec_sp,exec_d,
					exec_dp,running_connections,POINTpos);
			if(fork()==0)     /* CHILD */
		 	  {
		 	  close(0); close(1); close(2);
		 	  execl("./sniffit_key8","sniffit_key8",exec_s,exec_sp,exec_d,exec_dp,NULL);
		          exit(0);
		 	  };			
			break;
		case 'r':
		case 'R':           /* mask does an auto reset */
			exec_mask();
			break;
    		default: break;
    		}
	sig_blocking(0, SIGALRM);
	sig_blocking(0, SIGUSR1);
  	}
};
