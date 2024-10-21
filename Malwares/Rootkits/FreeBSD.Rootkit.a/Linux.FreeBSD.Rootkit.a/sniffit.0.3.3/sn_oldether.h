/* Sniffit old Ethernet File -- Brecht Claerhout                            */

struct packetheader
{
	struct timeval ts;      /* time stamp */
	u_long caplen;          /* length of portion present */
	u_long len;             /* length this packet (off wire) */
};

 

