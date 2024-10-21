
#include "rk_driver.h"
#include "rk_router.h"

/* ROUTING ROUTINES
 * Redirection and routing routines, including
 * multicast and broadcast lists.  Lists are
 * protected with Mutex.  Enables complete
 * compromise of any firewall.
 */

/* route a packet to another host using 
 * encrypted encapsulation
 */
void RedirectPacketTo(){
	DbgPrint(("RedirectPacket called\n"));
}

/* get an encapsulated packet & send over interface */
void OnRoutedPacket(){
	DbgPrint(("RoutedPacket called\n"));
}
