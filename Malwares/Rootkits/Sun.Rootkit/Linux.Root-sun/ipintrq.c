/*
 * Print the IP Interupt queue overflows
 *
 * @(#)ipintrq.c 1.1 91/11/13 
 */
 
#include <kvm.h>
#include <nlist.h>
#include <stdio.h>
 
struct nlist	ip_nl[] = { {"_ipintrq"}, {""} };
extern kvm_t	*kd;
 
static struct  ifqueue {
	int	ifq_head;	/* struct  mbuf *ifq_head; */
	int	ifq_tail;	/* struct  mbuf *ifq_tail; */
        int     ifq_len;
        int     ifq_maxlen;
        int     ifq_drops;
} buf;
 
ipintrq_stats()
{
	if (kvm_nlist(kd, ip_nl) < 0) {
		fprintf(stderr, "netstat: bad namelist\n");
		exit(1);
	}
	kread(ip_nl[0].n_value, &buf, sizeof(buf) );
	printf("\t%d ip input queue drops\n", buf.ifq_drops);
 
}

