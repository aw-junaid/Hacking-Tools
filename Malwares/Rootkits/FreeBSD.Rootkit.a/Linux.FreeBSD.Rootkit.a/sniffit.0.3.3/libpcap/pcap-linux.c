/*
 * Linux interface for packet capture lib
 * ported by Adam Caldwell (acaldwel@ace.cs.ohiou.edu)
 * Based on the pcap-snoop.c file
 * May be freely redistributed as per the GNU license
 */
#include <sys/param.h>
#include <stdio.h>
#include <netdb.h>
#include <ctype.h>
#include <signal.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <malloc.h>
#include <memory.h>
#include <unistd.h>


#include <net/if.h>
#include <net/if_arp.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_var.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <net/bpf.h>

#include "pcap-int.h"

static char snoop_device[255];
struct ifreq ifr_orig;

void restore_interface()
{
  int fd;

  fd = socket(PF_INET, SOCK_PACKET, htons(0x0003));
  if (fd < 0) {
    printf("Warning: could not restore interface to normal.\n");
    return;
  }

  if (ioctl(fd, SIOCSIFFLAGS, &ifr_orig)<0)
    printf("Warning: could not restore interface to normal.\n");
}

int pcap_read(pcap_t *p, int cnt, pcap_handler callback, u_char *user)
{
  register int datalen;
  register int caplen;
  struct sockaddr from;
  int from_len;
  char *buf;
  int bufsize;

  if (p->linktype == DLT_SLIP) {
    buf = (char *)p->buffer+16;
    bufsize = p->bufsize - 16;
    memset(p->buffer,0,16);
  } else if (p->linktype == DLT_PPP) {
    buf = (char *)p->buffer+4;
    bufsize = p->bufsize - 4;
    memset(p->buffer,0,4);
  } else {
    buf = (char *)p->buffer;
    bufsize = p->bufsize;
  }

  do {
    from_len = sizeof(from);
    datalen = recvfrom(p->fd,buf,bufsize,0,&from,&from_len);

    if (datalen < 0) {
      switch (errno) {
	case EWOULDBLOCK:
	  return (0);
      }
      sprintf(p->errbuf, "read: %s", pcap_strerror(errno));
      return (-1);
    }
  } while (strcmp(snoop_device,from.sa_data)); /* go until we find something
						 from the right interface */

  if (p->linktype == DLT_SLIP)
    datalen+=16;
  else if (p->linktype == DLT_PPP)
    datalen+=4;

  caplen = (datalen > p->bufsize) ? datalen : p->bufsize;

  if (caplen > p->snapshot)
    caplen = p->snapshot;

  if (p->fcode.bf_insns == NULL ||
      bpf_filter(p->fcode.bf_insns, (char *)p->buffer, datalen, caplen)) {
    struct pcap_pkthdr h;
    ++p->md.stat.ps_recv;

#ifdef SIOCGSTAMP
    if (ioctl(p->fd,SIOCGSTAMP,&h.ts)<0) /* ask for the timestamp */
#endif
      gettimeofday(&h.ts,0);

    h.len = datalen;
    h.caplen = caplen;
    (*callback)(user, &h, (char *)p->buffer);
    return (1);
  }
  return (0);
}

int pcap_stats(pcap_t *p, struct pcap_stat *ps)
{
  ps->ps_drop = 0;
  ps->ps_recv = p->md.stat.ps_recv;
  ps->ps_ifdrop = 0;
  return (0);
}

pcap_t *
pcap_open_live(char *device, int snaplen, int promisc, int to_ms, char *ebuf)
{
  pcap_t *p;
  struct ifreq ifr;

  p = (pcap_t *)malloc(sizeof(*p));
  if (p == NULL) {
    strcpy(ebuf, "no swap");
    return (0);
  }
  bzero(p, sizeof(*p));
  if (strncmp("et", device, 2) == 0)
    p->linktype = DLT_EN10MB;
  else if (strncmp("sl", device, 2) == 0)
    p->linktype = DLT_SLIP;
  else if (strncmp("pp", device, 2) == 0)
    p->linktype = DLT_PPP;
  else {
    sprintf(ebuf, "snoop: unknown physical layer type");
    goto bad;
  }
  p->fd = -1;
  p->bufsize = 4096;
  p->buffer = (u_char *)malloc(p->bufsize);
  if (p->buffer == NULL) {
    strcpy(ebuf, "no swap");
    goto bad;
  }
  p->fd = socket(PF_INET, SOCK_PACKET, htons(0x0003));
  if (p->fd < 0) {
    sprintf(ebuf, "snoop socket: %s", pcap_strerror(errno));
    goto bad;
  }

  if (p->linktype != DLT_SLIP && p->linktype != DLT_PPP && promisc) {
    strcpy(ifr.ifr_name, device);       /* interface we're gonna use */
    if (ioctl(p->fd, SIOCGIFFLAGS, &ifr) < 0 ) {    /* get flags */
      sprintf(ebuf, "socket ioctl get: %s", pcap_strerror(errno));
      goto bad;
    }
    ifr_orig = ifr;
    atexit(restore_interface);
    ifr.ifr_flags |= IFF_PROMISC;         /* set promiscuous mode */

    if (ioctl(p->fd, SIOCSIFFLAGS, &ifr) < 0 ) {    /* set flags */
      sprintf(ebuf, "socket ioctl set: %s", pcap_strerror(errno));
      goto bad;
    }
  }
  strcpy(snoop_device,device);

  p->snapshot = snaplen;
  return (p);
bad:
  if (p->fd >= 0)
	  close(p->fd);
  if (p->buffer != NULL)
	  free(p->buffer);
  free(p);
  return (0);
}

int
pcap_setfilter(pcap_t *p, struct bpf_program *fp)
{

	p->fcode = *fp;
	return (0);
}
