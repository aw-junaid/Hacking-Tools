#ifndef _ROS_IP_H_
#define _ROS_IP_H_

#include "lwip/tcp.h"
#include "lwip/pbuf.h"
#include "lwip/ip_addr.h"
//#include "tcpip.h"

#ifndef LWIP_TAG
    #define LWIP_TAG 'PIwl'
#endif

/* IP functions */
void LibIPInitialize(void);
void LibIPShutdown(void);

#endif