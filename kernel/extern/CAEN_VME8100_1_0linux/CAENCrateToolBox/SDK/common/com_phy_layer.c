/**************************************************************************
------------------------------------------------------
--	H E A D E R
------------------------------------------------------
-- File:		COM_PHY_LAYER.C  
-- Company:		
-- Basics:		
-- Others:						
-- Resources:	
-- Usage:
------------------------------------------------------
**************************************************************************/

/*******
* File includes
*******/
#include <stdio.h>
#include <string.h>
#include <errno.h>

#ifdef LINUX
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <sys/file.h>
	#include <sys/ioctl.h>
	#include <fcntl.h>
	#include <unistd.h>
//#include <stropts.h>
	#include <termios.h>
	#include <linux/serial.h>       /* Per il flag di bassa latenza */
#endif

#include "com_phy_layer.h"

#ifdef LINUX
//	#define CPL_LOCK_DEVICE							// Flag per bloccare device
#define SOCKET_ERROR	-1
#endif
/*******
* File private defines 
*******/
#define MAX_SOCKET_CONNECTIONS		1			/* Numero max di connessioni su socket */

/*******
* File global variable definition 
*******/
#ifndef CDTRDSR
	#define CDTRDSR	  004000000000			/* DTR DSR flow control */
#endif
#define CPL_RX_TIMEOUT_MSEC		10			/* timeout RX char da seriale */
/****
* Mutex
****/
#if defined( LINUX)
	#define CPL_LOCK( pCPLObj)			pthread_mutex_lock( &pCPLObj->m_mutex);				
	#define CPL_UNLOCK( pCPLObj)		pthread_mutex_unlock( &pCPLObj->m_mutex);
#elif defined( WIN32)
	#define CPL_LOCK( pCPLObj)			WaitForSingleObject( pCPLObj->m_mutex, INFINITE);
	#define CPL_UNLOCK( pCPLObj)		ReleaseMutex( pCPLObj->m_mutex);
#else
	???
#endif

/*******
* Module only visible function prototypes
*******/
/*
static BOOL CPL_GetCtlSignal (CPL_OBJ* pCPLObj, uint uSignal);
static void CPL_SetCtlSignal (CPL_OBJ* pCPLObj, BOOL bSet, uint uSignal);
static BOOL CPL_GetDsr (CPL_OBJ* pCPLObj);
static BOOL CPL_GetDtr (CPL_OBJ* pCPLObj);
static BOOL CPL_GetCts ( CPL_OBJ* pCPLObj);
static BOOL CPL_GetRts ( CPL_OBJ* pCPLObj);
static BOOL CPL_CanTx ( CPL_OBJ* pCPLObj);
static BOOL CPL_CanRx ( CPL_OBJ* pCPLObj);
*/
static BOOL CPL_OpenCom( CPL_OBJ* pCPLObj);
static BOOL CPL_OpenEth( CPL_OBJ* pCPLObj);

/*******
* Function implementation 
*******/

/****************************************************************************
* Name:		CPL_BiosInit
* Purpose:	Set MCU proper regiter & port IO
* Usage:	interrupt vector table handler must be setted
* Input:	
* Return:	
* Globals:	
*****************************************************************************/
void CPL_BiosInit( CPL_OBJ* pCPLObj )  
{
	/* flush both TX and RX buffers */
	memset( pCPLObj, 0, sizeof( CPL_OBJ));

	pCPLObj->m_comm.m_tag= INVALID_HANDLE_VALUE;
#if defined( LINUX)
	pthread_mutex_init( &pCPLObj->m_mutex, 0);
#elif defined( WIN32)
	pCPLObj->m_mutex= CreateMutex(NULL, FALSE, "CPL_MUTEX");
#else
	???
#endif

	/* flush both TX and RX buffers */
	CPL_Flush( pCPLObj, BOTH_BUFFER );
	
}
/****************************************************************************
* Name:		CPL_OpenCom
* Purpose:	Apre e configura il dispositivo prescelto di comunicazione seriale
* Usage:	
* Input:	
* Return:	BOOL: apertura ok
* Globals:	pCPLObj->m_hSerial
*****************************************************************************/
static BOOL CPL_OpenCom( CPL_OBJ* pCPLObj)
{
#ifdef LINUX
	struct termios info;
#endif

	// Apertura dispositivo
	CPL_Flush( pCPLObj, BOTH_BUFFER );

#if defined( LINUX)
	if( ( pCPLObj->m_comm.m_serial= open ( pCPLObj->m_config.m_device_config.m_com_device.m_com_str, O_RDWR | O_NOCTTY | O_NDELAY | O_EXCL))< 0) 
	{
		pCPLObj->m_comm.m_serial= INVALID_HANDLE_VALUE;
		return FALSE;
	}
#elif defined( WIN32)
	if( ( pCPLObj->m_comm.m_serial= CreateFile( pCPLObj->m_config.m_device_config.m_com_device.m_com_str, GENERIC_READ| GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL))== INVALID_HANDLE_VALUE)
	{
		pCPLObj->m_comm.m_serial= INVALID_HANDLE_VALUE;
		return FALSE;
	}
#endif
	// Lock dispositivo
	#ifdef CPL_LOCK_DEVICE	
	if (flock (pCPLObj->m_comm.m_serial, LOCK_EX|LOCK_NB) == -1)
	{
		close ( pCPLObj->m_comm.m_serial);
		pCPLObj->m_comm.m_serial= INVALID_HANDLE_VALUE;
		return FALSE;
	}
	#endif

	// Configurazione dispositivo
#if defined( LINUX) 
	fcntl ( pCPLObj->m_comm.m_serial, F_SETFL, FNDELAY);			// Indica operazioni Read() non bloccanti#

	tcgetattr ( pCPLObj->m_comm.m_serial, &info);
	switch( pCPLObj->m_config.m_device_config.m_com_device.m_baudrate)
	{
		case SERBAUD_9600:
			cfsetispeed (&info, B9600);				// Speed
			break;
		case SERBAUD_19200:
			cfsetispeed (&info, B19200);			// Speed
			break;
		case SERBAUD_38400:
			cfsetispeed (&info, B38400);			// Speed
			break;
		case SERBAUD_57600:
			cfsetispeed (&info, B57600);			// Speed
			break;
		case SERBAUD_115200:
			cfsetispeed (&info, B115200);			// Speed
			break;
		default:
			return FALSE;
	}
	info.c_cflag |= ( CLOCAL| CREAD);		// Ricevitore abilitato, modo locale
	info.c_cflag &= ~CSIZE;					// 8bit dati
	switch( pCPLObj->m_config.m_device_config.m_com_device.m_dataBit)
	{
		case DATA_SEVEN:
			info.c_cflag |= CS7;
			break;
		case DATA_EIGHT:
			info.c_cflag |= CS8;
			break;
		default:
			return FALSE;
	}
	switch( pCPLObj->m_config.m_device_config.m_com_device.m_stopBit)
	{
		case STOP_ONE:
			info.c_cflag &= ~CSTOPB;				// 1bit stop
			break;
		case STOP_TWO:
			info.c_cflag |= CSTOPB;					// 2bit stop
			break;
		default:
			return FALSE;
	}
	switch( pCPLObj->m_config.m_device_config.m_com_device.m_parity)
	{
		case SERPARITY_NONE:
			break;
		case SERPARITY_EVEN:
			info.c_cflag |= PARENB;				// Parita' Pari
			break;
		case SERPARITY_ODD:
			info.c_cflag |= PARODD;				// Parita' Dispari
			break;
		default:
			return FALSE;
	}

	info.c_lflag &= ~(ICANON | ECHO 
					| ECHOE | ISIG);		// Raw input (non processati)

	info.c_iflag = 0;
	info.c_iflag |= INPCK;					// Input parity check
	info.c_iflag |= pCPLObj->m_config.m_device_config.m_com_device.m_enable_Xon_Xoff? ( IXOFF| IXON): 0;	// XOn/Xoff

	//
	// Special characters
	if( pCPLObj->m_config.m_device_config.m_com_device.m_enable_Xon_Xoff) {
		info.c_cc[ VSTART]= pCPLObj->m_config.m_device_config.m_com_device.m_Xon;
		info.c_cc[ VSTOP]= pCPLObj->m_config.m_device_config.m_com_device.m_Xoff;
	}

	info.c_oflag = 0;				
	switch( pCPLObj->m_config.m_device_config.m_com_device.m_RTSFlow)
	{
		case RTS_DISABLE:
			break;
		case RTS_HANDSHAKE:
			info.c_cflag |= CRTSCTS;				// Controllo hardware RTS/CTS
			break;
		default:
			return FALSE;
	}
	switch( pCPLObj->m_config.m_device_config.m_com_device.m_DTRFlow)
	{
		case DTR_DISABLE:
			break;
		case DTR_HANDSHAKE:
			info.c_cflag |= CDTRDSR;				// Controllo hardware DTR/DSR
			break;
		default:
			return FALSE;
	}
	tcsetattr ( pCPLObj->m_comm.m_serial, TCSAFLUSH, &info);
	/* Imposto la bassa latenza sulla seriale */
	{
		struct serial_struct serial_info;

    	ioctl( pCPLObj->m_comm.m_serial, TIOCGSERIAL, &serial_info);
    	serial_info.flags |= ASYNC_LOW_LATENCY;
    	ioctl( pCPLObj->m_comm.m_serial, TIOCSSERIAL, &serial_info);
	}
#elif defined( WIN32)
	{
		DCB	dcb;
	    COMMTIMEOUTS  comm_time_outs;

		// Set comm timeout
	    comm_time_outs.ReadIntervalTimeout = 0xFFFFFFFF ;
		comm_time_outs.ReadTotalTimeoutMultiplier = 0 ;
		comm_time_outs.ReadTotalTimeoutConstant = 0 ;
		comm_time_outs.WriteTotalTimeoutMultiplier = 40;
		comm_time_outs.WriteTotalTimeoutConstant = 1000 ;
		SetCommTimeouts( pCPLObj->m_comm.m_serial, &comm_time_outs ) ;

		// Setup in and out queue size
		SetupComm( pCPLObj->m_comm.m_serial, 1024, 1024); 
		// Configure dcb
		memset( &dcb, 0, sizeof( DCB));
		dcb.DCBlength = sizeof (DCB);


		switch( pCPLObj->m_config.m_device_config.m_com_device.m_baudrate)
		{
			case SERBAUD_9600:
				dcb.BaudRate= CBR_9600;
				break;
			case SERBAUD_19200:
				dcb.BaudRate= CBR_19200;
				break;
			case SERBAUD_38400:
				dcb.BaudRate= CBR_38400;
				break;
			case SERBAUD_57600:
				dcb.BaudRate= CBR_57600;
				break;
			case SERBAUD_115200:
				dcb.BaudRate= CBR_115200;
				break;
			default:
				return FALSE;
		}
		switch( pCPLObj->m_config.m_device_config.m_com_device.m_dataBit)
		{
			case DATA_SEVEN:
				dcb.ByteSize= 7;
				break;
			case DATA_EIGHT:
				dcb.ByteSize= 8;
				break;
			default:
				return FALSE;
		}
		switch( pCPLObj->m_config.m_device_config.m_com_device.m_stopBit)
		{
			case STOP_ONE:
				dcb.StopBits= ONESTOPBIT;
				break;
			case STOP_TWO:
				dcb.StopBits= TWOSTOPBITS;
				break;
			default:
				return FALSE;
		}

		switch( pCPLObj->m_config.m_device_config.m_com_device.m_parity)
		{
			case SERPARITY_NONE:
				dcb.Parity= NOPARITY;
				dcb.fParity= FALSE;
				break;
			case SERPARITY_EVEN:
				dcb.Parity= EVENPARITY;
				dcb.fParity= TRUE;
				break;
			case SERPARITY_ODD:
				dcb.Parity= ODDPARITY;
				dcb.fParity= TRUE;
				break;
			default:
				return FALSE;
		}

		//dcb.fBinary = TRUE;
		//dcb.fParity = pConfig->parity!= PARITY_NONE;
		//dcb.fOutxDsrFlow = FALSE;
		//dcb.fRtsControl= RTSFlow[ pConfig->RTSFlow];
		//dcb.fDtrControl = DTR_CONTROL_DISABLE;
		//dcb.fDsrSensitivity = FALSE;
		//dcb.fDtrControl = DTR_CONTROL_HANDSHAKE;
		//dcb.fDsrSensitivity = TRUE;

		dcb.fBinary = TRUE;
		switch( pCPLObj->m_config.m_device_config.m_com_device.m_RTSFlow)
		{
			case RTS_DISABLE:
				dcb.fRtsControl= RTS_CONTROL_DISABLE;
				break;
			case RTS_HANDSHAKE:
				dcb.fRtsControl= RTS_CONTROL_HANDSHAKE;
				dcb.fOutxCtsFlow= TRUE;
				break;
			default:
				return FALSE;
		}
		switch( pCPLObj->m_config.m_device_config.m_com_device.m_DTRFlow)
		{
			case DTR_DISABLE:
				dcb.fDtrControl= DTR_CONTROL_DISABLE;
				break;
			case DTR_HANDSHAKE:
				dcb.fDtrControl= DTR_CONTROL_HANDSHAKE;
				dcb.fOutxDsrFlow = TRUE;
				dcb.fDsrSensitivity = TRUE;
				break;
			default:
				return FALSE;
		}

		//
		// Xon / Xoff flow control
		if( pCPLObj->m_config.m_device_config.m_com_device.m_enable_Xon_Xoff) {
			dcb.fInX= 1;
			dcb.fOutX= 1;
			dcb.XoffChar= pCPLObj->m_config.m_device_config.m_com_device.m_Xoff;
			dcb.XonChar= pCPLObj->m_config.m_device_config.m_com_device.m_Xon;
			dcb.XoffLim= 2;
			dcb.XonLim= 2;
		}
	
		SetCommState( pCPLObj->m_comm.m_serial, &dcb);

		FlushFileBuffers( pCPLObj->m_comm.m_serial);
	}
#else
??? 
#endif
	return TRUE;
}

/****************************************************************************
* Name:		CPL_Reconnect
* Purpose:	Riconnette se connessione persa
* Usage:	
* Input:	
* Return:	BOOL: connessione ok
* Globals:	
*****************************************************************************/
BOOL CPL_Reconnect( CPL_OBJ* pCPLObj)
{
	switch( pCPLObj->m_config.m_device)
	{
		case CPL_COM:
			if( pCPLObj->m_comm.m_serial== INVALID_HANDLE_VALUE)
			{
				return CPL_OpenCom( pCPLObj);
			}
			return TRUE;
		case CPL_ETH:
			// attiva connessione su ETH
			CPL_Close( pCPLObj);
			return CPL_OpenEth( pCPLObj);
	}
	return FALSE;

}

/****************************************************************************
* Name:		CPL_OpenEth
* Purpose:	Apre e configura socket 
* Usage:	
* Input:	
* Return:	BOOL: apertura ok
* Globals:	pCPLObj->m_hSerial
*****************************************************************************/
static BOOL CPL_OpenEth( CPL_OBJ* pCPLObj)
{
	CPL_Flush( pCPLObj, BOTH_BUFFER );
	pCPLObj->m_comm.m_socket= socket( AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if( pCPLObj->m_comm.m_socket== INVALID_SOCKET)
	{
		return FALSE;
	}
	/* rendi il socket NON bloccante */
#if defined ( LINUX)
	{
/*		int flags= fcntl( pCPLObj->m_comm.m_socket, F_GETFL);
		fcntl( pCPLObj->m_comm.m_socket, F_SETFL, flags| O_NONBLOCK);
*/
		struct timeval timeout;			// milliseconds timeout
		memset( &timeout, 0, sizeof( timeout));
		timeout.tv_usec= 100000L;
		if( setsockopt( pCPLObj->m_comm.m_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof( timeout))== SOCKET_ERROR)
		{
			close( pCPLObj->m_comm.m_socket);
			pCPLObj->m_comm.m_socket= INVALID_SOCKET;
			return FALSE;
		}
		if( setsockopt( pCPLObj->m_comm.m_socket, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof( timeout))== SOCKET_ERROR)
		{
			close( pCPLObj->m_comm.m_socket);
			pCPLObj->m_comm.m_socket= INVALID_SOCKET;
			return FALSE;
		}
	}
#elif defined ( WIN32)
	{
		UINT32 timeout= 100;			// milliseconds timeout
		if( setsockopt( pCPLObj->m_comm.m_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof( timeout))== SOCKET_ERROR)
		{
			closesocket( pCPLObj->m_comm.m_socket);
			pCPLObj->m_comm.m_socket= INVALID_SOCKET;
			return FALSE;
		}
		if( setsockopt( pCPLObj->m_comm.m_socket, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof( timeout))== SOCKET_ERROR)
		{
			closesocket( pCPLObj->m_comm.m_socket);
			pCPLObj->m_comm.m_socket= INVALID_SOCKET;
			return FALSE;
		}
	}
#else
	???
#endif
	// Connect socket
	{
		struct sockaddr_in address;
		address.sin_family= AF_INET;
		address.sin_port= htons( pCPLObj->m_config.m_device_config.m_eth_device.m_port);
	    address.sin_addr.s_addr= inet_addr( pCPLObj->m_config.m_device_config.m_eth_device.m_address);
		if( connect( pCPLObj->m_comm.m_socket, (struct sockaddr *)&address, sizeof( address))< 0)
		{
#if defined( LINUX)
			switch ( errno) {
				case EINPROGRESS:
					break;
				default:
					close( pCPLObj->m_comm.m_socket);
					pCPLObj->m_comm.m_socket= INVALID_SOCKET;
					return FALSE;
			}
#elif defined( WIN32)
			closesocket( pCPLObj->m_comm.m_socket);
			pCPLObj->m_comm.m_socket= INVALID_SOCKET;
			return FALSE;
#else
			???
#endif
		}
	}
	return TRUE;
}

/****************************************************************************
* Name:		CPL_Open
* Purpose:	Apre e configura il dispositivo prescelto
* Usage:	
* Input:	const SercomConfig* pConfig= ptr a struttura impostazioni
* Return:	BOOL: apertura ok
* Globals:	pCPLObj->m_hSerial
*****************************************************************************/
BOOL CPL_Open( CPL_OBJ* pCPLObj, const CPLConfig* pConfig)
{
	/* tenta di chiudere connessione se aperta */
	CPL_Close( pCPLObj);
	/* Copia i parametri di configurzione */
	memcpy( &pCPLObj->m_config, pConfig, sizeof( CPLConfig));
	
	/* Attiva la connessione */
	switch( pCPLObj->m_config.m_device)
	{
		case CPL_COM:
			// attiva connessione su COM
			return CPL_OpenCom( pCPLObj);
		case CPL_ETH:
			// attiva connessione su ETH
			return CPL_OpenEth( pCPLObj);
		default:
			pCPLObj->m_comm.m_tag= INVALID_HANDLE_VALUE;
			return FALSE;
	}	
	return	TRUE;
}
/*
 * Funzioni di interrogazione segnali di controllo
 */

/*
BOOL CPL_GetCtlSignal (CPL_OBJ* pCPLObj, uint uSignal)
{
	switch( pCPLObj->m_config.m_device)
	{
		case CPL_COM:
		{
			uint status;
			ioctl (pCPLObj->m_comm.m_serial, TIOCMGET, &status);
			return ((status & uSignal) == uSignal)? TRUE: FALSE;
		}
		case CPL_ETH:
			return FALSE;
	}
	return FALSE;
}
void CPL_SetCtlSignal (CPL_OBJ* pCPLObj, BOOL bSet, unsigned int uSignal)
{
	switch( pCPLObj->m_config.m_device)
	{
		case CPL_COM:
		{
			unsigned int status;
			ioctl ( pCPLObj->m_comm.m_serial, TIOCMGET, &status);
			if( bSet== TRUE)
				status|= uSignal;
			else
				status&= (~uSignal);
			ioctl (pCPLObj->m_comm.m_serial, TIOCMSET, &status);
		}
		break;
		case CPL_ETH:
			break;
	}
}
BOOL CPL_GetDsr (CPL_OBJ* pCPLObj)
{
	switch( pCPLObj->m_config.m_device)
	{
		case CPL_COM:
		{
			return CPL_GetCtlSignal ( pCPLObj, TIOCM_DSR);
		}
		case CPL_ETH:
			return FALSE;
	}
	return FALSE;
}

BOOL CPL_GetDtr (CPL_OBJ* pCPLObj)		
{
	switch( pCPLObj->m_config.m_device)
	{
		case CPL_COM:
		{
			return CPL_GetCtlSignal ( pCPLObj, TIOCM_DTR);
		}
		case CPL_ETH:
			return FALSE;
	}
	return FALSE;
}
	
BOOL CPL_GetCts ( CPL_OBJ* pCPLObj)		
{
	switch( pCPLObj->m_config.m_device)
	{
		case CPL_COM:
		{
			return CPL_GetCtlSignal ( pCPLObj, TIOCM_CTS);
		}
		case CPL_ETH:
			return FALSE;
	}
	return FALSE;
}

BOOL CPL_GetRts ( CPL_OBJ* pCPLObj)		
{
	switch( pCPLObj->m_config.m_device)
	{
		case CPL_COM:
		{
			return CPL_GetCtlSignal ( pCPLObj, TIOCM_RTS);
		}
		case CPL_ETH:
			return FALSE;
	}
	return FALSE;
}

BOOL CPL_CanTx ( CPL_OBJ* pCPLObj)			
{
	switch( pCPLObj->m_config.m_device)
	{
		case CPL_COM:
			return CPL_GetDsr( pCPLObj);
		case CPL_ETH:
			// TODO !!
			return TRUE;
	}
	return FALSE;
}
BOOL CPL_CanRx ( CPL_OBJ* pCPLObj)			
{
	switch( pCPLObj->m_config.m_device)
	{
		case CPL_COM:
			return CPL_GetDtr( pCPLObj);
		case CPL_ETH:
			// TODO !!
			return TRUE;
	}
	return FALSE;
}
*/

/****************************************************************************
* Name:		CPL_Close
* Purpose:	Chiude il dispositivo aperto
* Usage:	
* Input:	
* Return:	BOOL: chiusura ok
* Globals:	pCPLObj->m_hSerial
*****************************************************************************/
BOOL CPL_Close( CPL_OBJ* pCPLObj)
{
	if (pCPLObj->m_comm.m_tag== INVALID_HANDLE_VALUE) 
		return FALSE;

	/* Chiudi connessione */
	switch( pCPLObj->m_config.m_device)
	{
		case CPL_COM:
#if defined ( LINUX)
			close( pCPLObj->m_comm.m_serial);
#elif defined ( WIN32)
			CloseHandle( pCPLObj->m_comm.m_serial);
#else
			???
#endif
#ifdef CPL_LOCK_DEVICE
			if (flock ( pCPLObj->m_comm.m_serial, LOCK_UN|LOCK_NB) == -1)
			{
				return FALSE;
			}
#endif	
			break;
		case CPL_ETH:
#if defined ( LINUX)
			close( pCPLObj->m_comm.m_socket);
#elif defined ( WIN32)
			closesocket( pCPLObj->m_comm.m_socket);
#else
#endif
			break;
	}
	pCPLObj->m_comm.m_tag= INVALID_HANDLE_VALUE;
	return TRUE;
}

/****************************************************************************
* Name:		CPL_Flush
* Purpose:	Flushes sercom buffer: buffered data will be lost !
* Usage:	
* Input:	char type: buffer type ( TX_BUFFER, TX_BUFFER, BOTH_BUFFER)
* Return:	
* Globals:	wBuffTXIni, wBuffTXEnd, bBuffTX
*			wBuffRXIni, wBuffRXEnd, bBuffRX
*****************************************************************************/
void CPL_Flush( CPL_OBJ* pCPLObj, CPLBufferType type )  
{
	switch ( type)
	{
	case TX_BUFFER:
		CPL_LOCK( pCPLObj);
		pCPLObj->m_wBuffTXLen= pCPLObj->m_wBuffTXIni= 0;
		CPL_UNLOCK( pCPLObj);
		break;
	case RX_BUFFER:
		switch( pCPLObj->m_config.m_device)
		{
			case CPL_COM:
				while( CPL_RX( pCPLObj, 1))
					Sleep( 300);
				CPL_LOCK( pCPLObj);
				pCPLObj->m_wBuffRXLen= 0;
#if defined ( LINUX)
				ioctl ( pCPLObj->m_comm.m_serial, TCFLSH, 0);
#elif defined ( WIN32)
				FlushFileBuffers( pCPLObj->m_comm.m_serial);
#else
				???
#endif
				CPL_UNLOCK( pCPLObj);
				break;
			case CPL_ETH:
				CPL_LOCK( pCPLObj);
				pCPLObj->m_wBuffRXLen= 0;
				CPL_UNLOCK( pCPLObj);
				break;
		}
	case BOTH_BUFFER:
		switch( pCPLObj->m_config.m_device)
		{
			case CPL_COM:
				CPL_LOCK( pCPLObj);	
				pCPLObj->m_wBuffTXIni= pCPLObj->m_wBuffTXLen= pCPLObj->m_wBuffRXLen= 0;
#if defined ( LINUX)
				ioctl ( pCPLObj->m_comm.m_serial, TCFLSH, 0);
#elif defined ( WIN32)
				FlushFileBuffers( pCPLObj->m_comm.m_serial);
#else
				???
#endif
				CPL_UNLOCK( pCPLObj);
				break;
			case CPL_ETH:
				CPL_LOCK( pCPLObj);	
				pCPLObj->m_wBuffTXIni= pCPLObj->m_wBuffTXLen= pCPLObj->m_wBuffRXLen= 0;
				CPL_UNLOCK( pCPLObj);
				break;
		}
	}
}


/****************************************************************************
* Name:		CPL_GetBuffTX
* Purpose:	Get TX Buffer to be filled with data to Transmit
*			call SerTX to perform trasmission
* Usage:	Fill TX buffer with data up to wSize Byte
* Input:	WORD *wSize : TX buffer Free size
* Return:	BYTE*: TX Buffer: NULL if old pack transmission is ON
* Globals:	wBuffTXIni, wBuffTXLen
*****************************************************************************/
UINT8* CPL_GetBuffTX( CPL_OBJ* pCPLObj, UINT16 *wSize)
{
	/* check if no TX running and no buffer overflow */
	UINT8* buff= NULL;

	CPL_LOCK( pCPLObj);	
	if(( !pCPLObj->m_wBuffTXIni)&& ( pCPLObj->m_wBuffTXLen< CPL_DIM_TX_BUFF))
	{
		/* ok, we have some byte read to handle: return buffer len and TX buffer pointer */
		*wSize= CPL_DIM_TX_BUFF- pCPLObj->m_wBuffTXLen;
		buff= pCPLObj->m_bBuffTX+ pCPLObj->m_wBuffTXIni;
	}
	else
	{
		/* TX is running or buffer is overflow */
		*wSize= 0;
	}
	CPL_UNLOCK( pCPLObj);
	return buff;
}

/****************************************************************************
* Name:		CPL_TX
* Purpose:	First byte trasmission
* Usage:	TX buffer must be filled: call GetBuffTX before
* Input:	WORD wCount: number of bytes to transmit
* Return:	BOOL: TRUE= OK	FALSE= some problems
* Globals:	wBuffTXIni, wBuffTXLen
*****************************************************************************/
BOOL CPL_TX( CPL_OBJ* pCPLObj, UINT16 wCount)
{
	int sent_bytes= 0;
	if( pCPLObj->m_comm.m_tag== INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	if( ( !wCount)|| pCPLObj->m_wBuffTXIni)
	{
		return FALSE;
	}
	/* Set TX Buffer Length */
	pCPLObj->m_wBuffTXLen= wCount;

	// Emissione della stringa
	switch( pCPLObj->m_config.m_device)
	{
		case CPL_COM:
#if defined ( LINUX)
			sent_bytes= write( pCPLObj->m_comm.m_serial, pCPLObj->m_bBuffTX, wCount);
#elif defined ( WIN32)
			WriteFile( pCPLObj->m_comm.m_serial, pCPLObj->m_bBuffTX, wCount, &sent_bytes, NULL );
#else
				???
#endif
			break;
		case CPL_ETH:
			sent_bytes= send( pCPLObj->m_comm.m_socket, pCPLObj->m_bBuffTX, wCount, 0);
			break;
	}
	CPL_LOCK( pCPLObj);
		pCPLObj->m_wBuffTXIni+= sent_bytes;
	CPL_UNLOCK( pCPLObj);
	if ((sent_bytes < 0)||(  (UINT16)sent_bytes< wCount))
	{
		return FALSE;
	}
	return	TRUE;
}
/****************************************************************************
* Name:		GetBuffRX
* Purpose:	Get RX Buffer filled with received data 
* Usage:	Get received and buffered data 
*			call SerFlush( RX_BUFF) to perform buffer flush
* Input:	WORD *wSize : RX data buffer size
* Return:	BYTE*: RX Buffer: NULL if pack receiving is ON
* Globals:	wBuffRXLen
*****************************************************************************/
const UINT8* CPL_GetBuffRX( CPL_OBJ* pCPLObj, UINT16 *wSize)
{
	UINT8* buff= NULL;
	/* check if byte timer exipred and some bytes read */
	CPL_LOCK( pCPLObj);
	if(pCPLObj->m_wBuffRXLen)
	{
		/* OK we have bytes */
		*wSize= pCPLObj->m_wBuffRXLen;
		buff= pCPLObj->m_bBuffRX;
	}
	else
	{
		/* sorry.. RX is running or no new bytes into RX buffer */
		*wSize= 0;
	}
	CPL_UNLOCK( pCPLObj);
	return buff;
}
/****************************************************************************
* Name:		CPL_IsDataReady
* Purpose:	Check if some data ready
* Usage:	
* Input:	
* Return:	TRUE: some data ready
* Globals:	
*****************************************************************************/
BOOL CPL_IsDataReady( CPL_OBJ* pCPLObj)
{
	/* check if byte timer exipred and some bytes read */
	BOOL ret;
	CPL_LOCK( pCPLObj);
	ret= (( clock()> pCPLObj->m_rx_tout)&& ( pCPLObj->m_wBuffRXLen))? TRUE: FALSE;
	CPL_UNLOCK( pCPLObj);
	return ret;
}
/****************************************************************************
* Name:		CPL_RX
* Purpose:	Riceve <wCount> bytes su buffer RX
* Usage:	
* Input:	UINT16 wCount: numero di bytes da ricevere
* Return:	BOOL : TRUE -> RX Ok (tutti i wCount bytes ricevuti)
* Globals:	
*****************************************************************************/
BOOL CPL_RX( CPL_OBJ* pCPLObj, UINT16 wCount)
{
	UINT32 read_bytes= 0;
	if( pCPLObj->m_comm.m_tag== INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	if (wCount == 0) 
	{
		return TRUE;
	}
	if ( wCount+ pCPLObj->m_wBuffRXLen>= CPL_DIM_RX_BUFF)
	{
		return FALSE;
	}
	// Emissione della stringa
	switch( pCPLObj->m_config.m_device)
	{
		case CPL_COM:
			// Verifica se dati disponibili
			{
				int in_q;
#if defined ( LINUX)
				ioctl( pCPLObj->m_comm.m_serial, FIONREAD, &in_q);
#elif defined ( WIN32)
				COMSTAT cs;                     // Serial Communication Port State
				DWORD	errorCode;
				ClearCommError( pCPLObj->m_comm.m_serial, &errorCode, &cs);
				if( errorCode & ( CE_OVERRUN| CE_RXOVER| CE_RXPARITY))
				{
					return FALSE;
				}
				in_q= cs.cbInQue;
#else
				???
#endif
				if( in_q< wCount)
				{
					return FALSE;
				}
				pCPLObj->m_rx_tout= clock()+ (CPL_RX_TIMEOUT_MSEC* CLOCKS_PER_SEC)/ 1000;
				// Lettura dati
#if defined ( LINUX)
				read_bytes= read( pCPLObj->m_comm.m_serial, pCPLObj->m_bBuffRX+ pCPLObj->m_wBuffRXLen, wCount);
#elif defined ( WIN32)
				// Check for pending IO
				if( !ReadFile( pCPLObj->m_comm.m_serial, pCPLObj->m_bBuffRX+ pCPLObj->m_wBuffRXLen, wCount, &read_bytes, NULL ))
				{
					if (GetLastError() == ERROR_IO_PENDING)
					{
						ClearCommError( pCPLObj->m_comm.m_serial, &errorCode, &cs);
						return FALSE;
					}
				}
#else
				???
#endif
			}
			break;
		case CPL_ETH:
			pCPLObj->m_rx_tout= clock()+ (CPL_RX_TIMEOUT_MSEC*CLOCKS_PER_SEC)/ 1000;
			// Lettura dati
			{
				int byte_avail= 0;
				if( ( byte_avail= recv( pCPLObj->m_comm.m_socket, pCPLObj->m_bBuffRX+ pCPLObj->m_wBuffRXLen, wCount, MSG_PEEK))< (int)wCount)
				{
/* Riabilita con socket bloccanti
					if( byte_avail< 0)
					{
						// persa connessione ?!?!
						pCPLObj->m_config.m_device_config.m_eth_device.m_hAcceptDevice= INVALID_HANDLE_VALUE;
					}
*/					
					return FALSE;
				}
			}
			read_bytes= recv( pCPLObj->m_comm.m_socket, pCPLObj->m_bBuffRX+ pCPLObj->m_wBuffRXLen, wCount, 0);
			break;
	}
	CPL_LOCK( pCPLObj);
		pCPLObj->m_wBuffRXLen+= (UINT16)read_bytes;
	CPL_UNLOCK( pCPLObj);
	
	return ((read_bytes>= wCount))? TRUE: FALSE;
}
/****************************************************************************
* Name:		CPL_GetCount
* Purpose:	Get number of bytes stored into buffer
* Usage:	
* Input:	char type: buffer type ( TX_BUFFER, TX_BUFFER)
* Return:	WORD: number of stored bytes
* Globals:	wBuffTXIni, wBuffTXEnd, bBuffTX
			wBuffRXIni, wBuffRXEnd, bBuffRX
*****************************************************************************/
UINT16 CPL_GetCount( CPL_OBJ* pCPLObj, CPLBufferType type )  
{
	UINT16 count= 0;
	if( pCPLObj->m_comm.m_tag== INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	CPL_LOCK( pCPLObj);
	
	switch ( type)
	{
	case TX_BUFFER:
		count= pCPLObj->m_wBuffTXLen- pCPLObj->m_wBuffTXIni;
		break;
	case RX_BUFFER:
		count= pCPLObj->m_wBuffRXLen;
		break;
	case BOTH_BUFFER:
		break;
	}
	CPL_UNLOCK( pCPLObj);
	return count;
}

/****************************************************************************
* Name:		SerGetFree
* Purpose:	Get number of bytes free from buffer
* Usage:	
* Input:	char type: buffer type ( TX_BUFFER, TX_BUFFER)
* Return:	WORD: number of bytes free
* Globals:	wBuffTXIni, wBuffTXEnd, bBuffTX
			wBuffRXIni, wBuffRXEnd, bBuffRX
*****************************************************************************/
UINT16 CPL_GetFree( CPL_OBJ* pCPLObj, CPLBufferType type )  
{
	UINT16 count= 0;
	CPL_LOCK( pCPLObj);
	switch ( type)
	{
	case TX_BUFFER:
		count= CPL_DIM_TX_BUFF- pCPLObj->m_wBuffTXLen;
	case RX_BUFFER:
		count= CPL_DIM_RX_BUFF- pCPLObj->m_wBuffRXLen;
	case BOTH_BUFFER:
		break;
	}
	CPL_UNLOCK( pCPLObj);
	return count;
}
/****************************************************************************
* Name:		CPL_direct_TX
* Purpose:	Transmit data buffer directly
* Usage:	
* Input:	CPL_OBJ* pCPLObj: CPL_OBJ pointer
            const UINT8* bBuff: data buffer pointer
            WORD wCount: number of bytes to transmit
* Return:	BOOL: TRUE= OK	FALSE= some problems
* Globals:	wBuffTXIni, wBuffTXLen
*****************************************************************************/
BOOL CPL_direct_TX( CPL_OBJ* pCPLObj, const UINT8* bBuff, UINT16 wCount)
{
	int sent_bytes= 0;
	if( pCPLObj->m_comm.m_tag== INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	// Emissione della stringa
	switch( pCPLObj->m_config.m_device)
	{
		case CPL_COM:
#if defined ( LINUX)
			sent_bytes= write ( pCPLObj->m_comm.m_serial, bBuff, wCount);
#elif defined ( WIN32)
			WriteFile( pCPLObj->m_comm.m_serial, bBuff, wCount, &sent_bytes, NULL );
#else
				???
#endif
			break;
		case CPL_ETH:
			sent_bytes= send( pCPLObj->m_comm.m_socket, bBuff, wCount,0);
			break;
	}
	if ((sent_bytes < 0)||(  (UINT16)sent_bytes< wCount))
	{
		return FALSE;
	}
	return	TRUE;
}
/****************************************************************************
* Name:		CPL_direct_RX
* Purpose:	Riceve fino a <*wCount> bytes su buffer RX
* Usage:	
* Input:	CPL_OBJ* pCPLObj: CPL_OBJ pointer
            UINT8* bBuff: data buffer
            UINT16 *wCount: numero max di bytes da ricevere ( su ingresso) e 
							numero di bytes effettivamanete ricevuti ( su uscita)
* Return:	BOOL : TRUE -> RX Ok 
* Globals:	
*****************************************************************************/
BOOL CPL_direct_RX( CPL_OBJ* pCPLObj, UINT8* bBuff, UINT16 *wCount)
{
	UINT32 read_bytes= 0;
	if( pCPLObj->m_comm.m_tag== INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	if (*wCount == 0) 
	{
		return TRUE;
	}
	// Emissione della stringa
	switch( pCPLObj->m_config.m_device)
	{
		case CPL_COM:
			// Verifica se dati disponibili
			{
				int in_q;
#if defined ( LINUX)
				ioctl( pCPLObj->m_comm.m_serial, FIONREAD, &in_q);
#elif defined ( WIN32)
				COMSTAT cs;                     // Serial Communication Port State
				DWORD	errorCode;
				ClearCommError( pCPLObj->m_comm.m_serial, &errorCode, &cs);
				if( errorCode & ( CE_OVERRUN| CE_RXOVER| CE_RXPARITY))
				{
					return FALSE;
				}
				in_q= cs.cbInQue;

#else
				???
#endif
				if( in_q> *wCount)
				{
					in_q= *wCount;
				}
				*wCount= 0;
				pCPLObj->m_rx_tout= clock()+ (CPL_RX_TIMEOUT_MSEC* CLOCKS_PER_SEC)/ 1000;
				// Lettura dati
#if defined ( LINUX)
				read_bytes= read( pCPLObj->m_comm.m_serial, bBuff, in_q);
#elif defined ( WIN32)
				{
					// Check for pending IO
					if( !ReadFile( pCPLObj->m_comm.m_serial, bBuff, in_q, &read_bytes, NULL ))
					{
						if (GetLastError() == ERROR_IO_PENDING)
						{
							ClearCommError( pCPLObj->m_comm.m_serial, &errorCode, &cs);
							return FALSE;
						}
					}
				}
#else
				???
#endif
			}
			break;
		case CPL_ETH:
			// Lettura dati
			{
				int in_q= recv( pCPLObj->m_comm.m_socket, bBuff, *wCount, MSG_PEEK);
				if( in_q< 0) {
#if defined (LINUX)
					switch( errno){
						case EWOULDBLOCK:
							*wCount= 0;
							return TRUE;
					}
#elif defined (WIN32)
					switch( WSAGetLastError()){ 
						case WSAEWOULDBLOCK: 
						case WSAETIMEDOUT:
							*wCount= 0;
							return TRUE;
					}
#else
		???
#endif
					return FALSE;
				}
				if( in_q> *wCount)
				{
					in_q= *wCount;
				}

				read_bytes= recv( pCPLObj->m_comm.m_socket, bBuff, in_q, 0);
			}
			break;
	}
	*wCount= ( UINT16)read_bytes;

	return TRUE;
}
