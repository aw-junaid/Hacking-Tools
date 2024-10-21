//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// KIP (NT-kernel TCP/IP implementation library)
//	
// module: ethernetif.c
// $Revision: 79 $
// $Date: 2012-01-18 12:59:39 +0400 (Ср, 18 янв 2012) $
// description: 
//	Ethernet network interface for lwip

#include "project.h"
#include "ndisif.h"

#include "etharp.h"
#include "ppp_oe.h"
#include "lwip\netifapi.h"

/* Define those to better describe your network interface. */
#define IFNAME0 'e'
#define IFNAME1 'n'

/* Forward declarations. */
static void  ethernetif_input(struct netif *netif);

/**
 * lwip interface status call back
 * it's called every time when if status was changed up/down
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
static void ethernetif_status(struct netif *netif)
{
	return;
}

/**
 * In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
FORCEINLINE
static void
low_level_init(struct netif *netif)
{
  PKIP_NDIS_PROTOCOL pProtocol = (PKIP_NDIS_PROTOCOL)netif->state;
  PKIP_NDIS_INTERFACE pInterface = pProtocol->pInterface;
  
  /* set MAC hardware address */
  netif->name[0] = IFNAME0;
  netif->name[1] = IFNAME1;
  netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;

  /* Do whatever else is needed to initialize interface. */  
  netif->mtu = pInterface->MTU;
  netif->hwaddr_len = (u8_t)pInterface->HWAddressLength;
  memcpy(netif->hwaddr,pInterface->CurrentMacAddress,pInterface->HWAddressLength);
}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become available since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */

static err_t
low_level_output(struct netif *netif, struct pbuf *p)
{
	PKIP_NDIS_PROTOCOL pProtocol = (PKIP_NDIS_PROTOCOL)netif->state;
	// in our case, PSendBuffer will free buffer
	pProtocol->PSendBuffer( pProtocol, p );
	LWIP_DEBUGF(DHCP_DEBUG,("Done 2\n"));
	//ExFreePool(total);
	LWIP_DEBUGF(DHCP_DEBUG,("packet sent\r\n"));
	return ERR_OK;
}

/*-----------------------------------------------------------------------------------*/
/*
 * ethernetif_output():
 *
 * This function is called by the TCP/IP stack when an IP packet
 * should be sent. It calls the function called low_level_output() to
 * do the actual transmission of the packet.
 *
 */
/*-----------------------------------------------------------------------------------*/
static err_t ethernetif_output(struct netif *netif, struct pbuf *p, struct ip_addr *ipaddr)
{
	// fill arp header
	if(etharp_output(netif, p, ipaddr))
	{
		// send it to NIC
		return low_level_output(netif, p);
	}
	return ERR_OK;
}

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t
ethernetif_init(struct netif *netif)
{
	PKIP_NDIS_PROTOCOL pProtocol = (PKIP_NDIS_PROTOCOL)netif->state;
	PKIP_NDIS_INTERFACE pInterface = pProtocol->pInterface;
	LWIP_ASSERT("netif != NULL", (netif != NULL));

	/*
	* Initialize the snmp variables and counters inside the struct netif.
	* The last argument should be replaced with your link speed, in units
	* of bits per second.
	*/
	NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, LINK_SPEED_OF_YOUR_NETIF_IN_BPS);

	/* We directly use etharp_output() here to save a function call.
	* You can instead declare your own function an call etharp_output()
	* from it if you have to do some checks before sending (e.g. if link
	* is available...) */
	netif->output = etharp_output;
	netif->linkoutput = low_level_output;
//	netif->status_callback = ethernetif_status;

	/* initialize the hardware */
	low_level_init(netif);

	if ( pProtocol->EnableDHCP ){
		netif->flags |= NETIF_FLAG_DHCP_NO_DISCOVER;
		// start dhcp listener
		dhcp_start(netif);
	}

	// TODO: synchronize with main initialization function
	// my_InitMiniport

	return ERR_OK;
}
