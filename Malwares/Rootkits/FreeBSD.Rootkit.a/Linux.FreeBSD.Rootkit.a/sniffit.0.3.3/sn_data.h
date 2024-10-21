/* Sniffit Data File -- Brecht Claerhout                                */

/**** Handler function ******************************************************/ 
#ifdef INCLUDE_INTERFACE
typedef void (*sig_hand)(int );  /* sighandler_t gave errors, weird */
#endif

/**** Global data **********************************************************/ 
struct file_info 
{
        char proto, filename[50];
        FILE *f;
        unsigned long bytes;
        unsigned long exp_seq;     /* expected seq to avoid double logging */
	int time_out;   
	char log;                      /* log=0  : do nothing        */
	                               /* log=1  : log 'login'       */
	                               /* log=2  : 'login' logged    */
	                               /* log=3  : log password      */
	                               /* log=4  : password logged   */
	                               /* log=99 : no more detection */
	char scroll_buf[SCBUF+1];                  /* scroll buffer */
	char *buffer;			     /* pointer to a buffer */
	struct file_info *next;
};

pcap_t *dev_desc;
void *start_dynam;
int dynam_len;
char Logfile[250];                                      /* name of logfile */
FILE *LogFILE;                                           /* logfile stream */
char *IP;
unsigned long SNIFLEN;                            /* bytes we need to snif */
short DEST_PORT;                                       /* destination port */
char non_printable, *logging_device;

/**** Global data (config) **************************************************/ 
struct cfg_file_contense
{
unsigned char host[16];
unsigned int priority;
unsigned char wildcard;
unsigned short port;
};

struct cfg_file_contense *select_from_list;     /* pointers for cfg lists */
struct cfg_file_contense *select_to_list;
struct cfg_file_contense *deselect_from_list;
struct cfg_file_contense *deselect_to_list;

int select_from_length=0;                         /* length of cfg lists  */
int select_to_length=0;
int deselect_from_length=0;
int deselect_to_length=0;
int Priority=0;             /* The higher the priority, the more important */
char dot_notation[20];                            /* for easy working, Q&D */

/**** Global data (interactive) *********************************************/ 
#ifdef INCLUDE_INTERFACE                               

/**** shared memory pointers ************************************************/ 
char *SHARED, *connection_data, *timing, *running_connections, 
						  *logged_connections; 
int *LISTlength, *DATAlength, memory_id;
unsigned int  *TCP_nr_of_packets, *ICMP_nr_of_packets, *UDP_nr_of_packets;
unsigned int  *IP_nr_of_packets;
unsigned long *TCP_bytes_in_packets, *UDP_bytes_in_packets;

/**** data structures *******************************************************/ 
struct shared_conn_data           /* shared memory connection datastructure */
  {
  char connection [CONN_NAMELEN];                 /* full ID string of conn */
  int timeout;                   
  };
struct shared_logged_conn            /* shared memory logging datastructure */
  {
  char log_enter [CONN_NAMELEN];                          /* normal logging */
  };
struct snif_mask                                         /* struct for mask */
  {
  unsigned long source_ip, destination_ip;
  unsigned short source_port, destination_port;
  };

struct snif_mask *mask;
struct shared_logged_conn *log_conn;
FILE *log_dev_stream;
struct stat log_dev_stat;

volatile int LOGGING=0, screen_busy=0;
char PACKET_INFO;
int POINTpos=0, LISTpos=0;
unsigned char COLOR_AVAIL=0;
WINDOW *menu_window;
struct box_window
  {WINDOW *main_window,*work_window;};
struct box_window data_box, main_box, mask_box, packets_box;
int Pid=0;
#endif
 
/**** Sniffit functions *****************************************************/ 
int check_packet(unsigned long, const struct packetheader *,
	     const unsigned char *, char *, char *, struct unwrap *, char *, int);
pcap_handler packethandler(u_char *, const struct packetheader *, 
							   const unsigned char *);
void print_iphead (struct IP_header *, char);
static unsigned long getaddrbyname(char *);
void quit (char *);
void reset_all (void);
char *strlower (char *);
struct file_info *add_dynam (char *, char, char, unsigned long, int);
void delete_dynam (char *, char, char);  
void record_buf(struct file_info *, unsigned long, char *, int, int);
void sb_shift(struct file_info *); 
void sbuf_update(struct file_info *, unsigned long, char *, int);
struct file_info *search_dynam(char *, char);
void my_exit (void);

/**** Sniffit functions (config) ********************************************/ 
void clear_list_buffer (struct cfg_file_contense *);
struct cfg_file_contense *adjust_select_from_list (void);
struct cfg_file_contense *adjust_select_to_list (void);
struct cfg_file_contense *adjust_deselect_from_list (void);
struct cfg_file_contense *adjust_deselect_to_list (void);
char *clean_string (char *); 
char *clean_filename (char *);
void make_nr_dot (char *);
void interprete_line (char *);
void read_cfg_file (char *);    

/**** Sniffit functions (logfile) *******************************************/ 
void logfile_exit (void);
char *gettime (void);
void print_logline (char *);
void print_ftp_user (char *, char *);
void print_ftp_pass(char *, char *);
void print_login (char *, char *);
void print_pwd (char *, char *);
void print_conn (char *, char *);
void open_logfile (void);

/**** Sniffit functions (packets) *******************************************/ 
unsigned short in_cksum(unsigned short *,int);
int unwrap_packet (const unsigned char *, struct unwrap *);

/**** Interface functions ***************************************************/ 
#ifdef INCLUDE_INTERFACE
int add_itemlist(char *, char *);
void child_exit (void);
void clear_shared_mem(char);
void data_device (char *, int);
void data_window (struct box_window *, struct box_window *, int, int, int, 
						    	       int, char *, int);
int del_itemlist(char *, char *);
void exec_mask (void);
void f_box_window (struct box_window *, int, int, int, int, int);
void fill_box_window (struct box_window *, char *, int, int, int);
void forced_refresh (void);
void init_screen (void);
char *input_field(char *, char *);
void interaction (int);
void mask_status (struct box_window *);
void mem_exit (void);
void menu_line (void);
void point_item (struct box_window *, char *, int, int, int, int);
void run_interface (void);
void screen_exit (void);
void set_signal (int, sig_hand);
void sig_blocking(char, int);
void stop_logging (void);
int check_mask (const struct packetheader *,const unsigned char *, char *, 
								struct unwrap *);
pcap_handler interactive_packethandler( char *, const struct packetheader *, 
                 				          const unsigned char *);
void stop_packet_info (void);
void packet_info_handler (int);
void create_arguments(char *, char *, char *, char *, char *, int);
#endif

/**** DEBUG section ********************************************************/ 
#ifdef DEBUG
FILE *debug_dev;
unsigned int debug_cnt=0;

void close_debug_device (void);  
void debug_msg(char *);
#endif
