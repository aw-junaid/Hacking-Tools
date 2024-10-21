#include "project.h"

#include "lwip\netdb.h"
//#define LWIP_PROVIDE_ERRNO
#include "lwip\arch.h"

NTSTATUS RtlErroNoToStatus( int ErrNo )
{
	NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;
	switch ( ErrNo )
	{
	case 0:            /* ERR_OK          0      No error, everything OK. */
		ntStatus = STATUS_SUCCESS;
		break;
	case ENOMEM:        /* ERR_MEM        -1      Out of memory error.     */
		ntStatus = STATUS_NO_MEMORY;
		break;
	case ENOBUFS:       /* ERR_BUF        -2      Buffer error.            */
		ntStatus = STATUS_BUFFER_TOO_SMALL;
		break;
	case EWOULDBLOCK:   /* ERR_TIMEOUT    -3      Timeout                  */
		ntStatus = STATUS_IO_TIMEOUT;
		break;
	case EHOSTUNREACH:  /* ERR_RTE        -4      Routing problem.         */
		ntStatus = STATUS_HOST_UNREACHABLE;
		break;
	case EINPROGRESS:   /* ERR_INPROGRESS -5      Operation in progress    */
		ntStatus = STATUS_PENDING;
		break;
	case EINVAL:        /* STATUS_INVALID_PARAMETER        -6      Illegal value.           */
		ntStatus = STATUS_INVALID_PARAMETER;
		break;
	case EADDRINUSE:    /* ERR_USE        -8      Address in use.          */
		ntStatus = STATUS_ADDRESS_ALREADY_EXISTS;
		break;
	case EALREADY:      /* ERR_ISCONN     -9      Already connected.       */
		ntStatus = STATUS_CONNECTION_ACTIVE;
		break;
	case ECONNABORTED:  /* ERR_ABRT       -10     Connection aborted.      */
		ntStatus = STATUS_CONNECTION_ABORTED;
		break;
	case ECONNRESET:    /* ERR_RST        -11     Connection reset.        */
		ntStatus = STATUS_CONNECTION_REFUSED;
		break;
	//case ENOTCONN:      /* ERR_CLSD       -12     Connection closed.       */
	//	ntStatus = STATUS_CONNECTION_RESET;
	//	break;
	case ENOTCONN:      /* ERR_CONN       -13     Not connected.           */
		ntStatus = STATUS_CONNECTION_INVALID;
		break;
	case EIO:           /* ERR_ARG        -14     Illegal argument.        */
		ntStatus = STATUS_INVALID_PARAMETER;
		break;
	case EAI_NONAME:
		ntStatus = STATUS_HOST_DOWN;
		break;
	case EAI_SERVICE:
		ntStatus = STATUS_HOST_DOWN;
		break;
	case EAI_FAIL:
		ntStatus = STATUS_UNSUCCESSFUL;
		break;
	case EAI_MEMORY:
		ntStatus = STATUS_INSUFFICIENT_RESOURCES;
		break;
	case HOST_NOT_FOUND:
		ntStatus = STATUS_HOST_DOWN;
		break;
	case NO_DATA:
		ntStatus = STATUS_HOST_DOWN;
		break;
	case NO_RECOVERY:
		ntStatus = STATUS_HOST_DOWN;
		break;
	case TRY_AGAIN:
		ntStatus = STATUS_HOST_DOWN;
		break;
	case -1:            /* ERR_IF         -15     Low-level netif error    */
		ntStatus = STATUS_UNSUCCESSFUL;
		break;
	}
	return ntStatus;
}