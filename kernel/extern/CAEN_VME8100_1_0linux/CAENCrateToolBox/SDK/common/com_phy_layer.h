/*************************************************************************
------------------------------------------------------
--	H E A D E R
------------------------------------------------------
-- File:		COM_PHY_LAYER.H
-- Company:		
-- Purpose:		Communication Physical Layer
-- Basics:		
------------------------------------------------------
*************************************************************************/

#ifndef	_CPL_DEF_H
#define _CPL_DEF_H

#include <time.h>
#ifdef LINUX
	#include <pthread.h>
#endif

#include "common_defs.h"

/*******
* File includes 
*******/

/*******
* Defines declaration 
*******/
#ifdef LINUX 
	typedef int HANDLE;
	#define INVALID_HANDLE_VALUE -1
	typedef int SOCKET;
	#define INVALID_SOCKET -1
#endif

#define CPL_DIM_RX_BUFF		260
#define CPL_DIM_TX_BUFF		260


// Macro per Little/Big endian
#define CPL_LOW_BYTE( w)					( *( UINT8*)( (UINT16*)&w))
#define CPL_HIGH_BYTE( w)					( *((( UINT8*) ((UINT16*) &w))+ 1 ))
#define CPL_MAKE_WORD( a, b)				(((UINT16)((UINT8)(a)))| (((UINT16)((UINT8)(b)))<< 8)) 

#define MSK_BYTE( bCh)						( 1<< (( UINT8)bCh))
#define MSK_WORD( wCh)						( 1<< (( UINT16)wCh))

#define CPL_SWAP_WORD( value)				CPL_MAKE_WORD( CPL_HIGH_BYTE( value), CPL_LOW_BYTE( value))

#define CPL_GET_WORD( pWord)				( CPL_MAKE_WORD( *(UINT8*)(( UINT8*)(pWord)+1), *( UINT8*)(pWord)))
#define CPL_GET_NATIVE_WORD( pWord)			( CPL_MAKE_WORD( *(UINT8*)(( UINT8*)(pWord)), *( UINT8*)(pWord)+1))

#define CPL_SET_NATIVE_WORD( pWord, wValue)	 *( UINT8*)(pWord)= CPL_LOW_BYTE(wValue);*(UINT8*)(( UINT8*)(pWord)+1)= CPL_HIGH_BYTE(wValue);
#define CPL_SET_WORD( pWord, wValue)	 	*( UINT8*)(pWord)= CPL_HIGH_BYTE(wValue);*(UINT8*)(( UINT8*)(pWord)+1)= CPL_LOW_BYTE(wValue);

typedef enum 
{
	RX_BUFFER,
	TX_BUFFER,
	BOTH_BUFFER,
} CPLBufferType;

typedef enum 
{
	CPL_COM,
	CPL_ETH,
} CPLDevice;

typedef enum 
{
	SERBAUD_9600,
	SERBAUD_19200,
	SERBAUD_38400,
	SERBAUD_57600,
	SERBAUD_115200,
} CPLComBaudrate;
typedef enum 
{
	SERPARITY_NONE,
	SERPARITY_EVEN,
	SERPARITY_ODD,
} CPLComParity;

typedef enum 
{
	STOP_ONE,
	STOP_5ONE,
	STOP_TWO,
} CPLComStopBit;

typedef enum 
{
	DATA_SEVEN,
	DATA_EIGHT,
} CPLComDataBit;

typedef enum 
{
	RTS_DISABLE,
	// RTS_TOGGLE,
	RTS_HANDSHAKE,
} CPLComRtsFlow;

typedef enum 
{
	DTR_DISABLE,
	DTR_HANDSHAKE,
} CPLComDtrFlow;

typedef struct
{
	char m_com_str[ 256];						/* Communication device : e.g COM1.. COM# for Windows or /dev/ttyS0.. /dev/ttyS# or /dev/ttyUSB0.. /dev/ttyUSB# for Linux */
	CPLComBaudrate m_baudrate;
	CPLComParity m_parity;
	CPLComDataBit m_dataBit;
	CPLComStopBit m_stopBit;
	CPLComRtsFlow m_RTSFlow;					/* RTS Flow type */
	CPLComDtrFlow m_DTRFlow;					/* DTR Flow type */
	BOOL m_enable_Xon_Xoff;						/* Enable XOn/XOff Flow type */
	char m_Xon;									/* The Xon char */ 
	char m_Xoff;								/* The Xoff char */ 
} CPLComDevice;

typedef struct
{
	char m_address[ 64];						/* Server Address */
	int m_port;									/* Server port number */
} CPLEthDevice;

typedef struct
{
	CPLDevice m_device;
	union
	{
		CPLComDevice m_com_device;
		CPLEthDevice m_eth_device;
	} m_device_config;
} CPLConfig;

typedef struct
{
	UINT8 m_bBuffTX[ CPL_DIM_TX_BUFF];			/* RX Buffers */
	UINT8 m_bBuffRX[ CPL_DIM_RX_BUFF];			/* TX Buffers */
	UINT16 m_wBuffTXIni;						/* Buff TX Ini */
	UINT16 m_wBuffRXLen;						/* Buff char count */
	UINT16 m_wBuffTXLen;						/* Buff char count */
	union 
	{
		HANDLE m_serial;						/* Handle dispositivo di comunicazione seriale */
		SOCKET m_socket;						/* Handle socket */
#if defined (LINUX)
		UINT32 m_tag;							/* tag handler */
#elif defined (WIN32)
		UINT32 *m_tag;							/* tag handler */
#else
		???
#endif
	} m_comm;
	clock_t m_rx_tout;							/* timeout per rx */
	CPLConfig m_config;							/* parametri di config */
#ifdef LINUX
	pthread_mutex_t m_mutex;
#endif
#ifdef WIN32
	HANDLE m_mutex;
#endif
} CPL_OBJ;


/*******
* Macros definition 
********/

/*******
* Global variable declaration 
*******/

/*******
* External visibile Function prototype 
*******/
void CPL_BiosInit( CPL_OBJ* pCPLObj );
BOOL CPL_Open( CPL_OBJ* pCPLObj, const CPLConfig* pConfig);
BOOL CPL_Close( CPL_OBJ* pCPLObj);
void CPL_Flush( CPL_OBJ* pCPLObj, CPLBufferType type );

UINT8* CPL_GetBuffTX( CPL_OBJ* pCPLObj, UINT16 *wSize);
BOOL CPL_TX( CPL_OBJ* pCPLObj, UINT16 wCount);
const UINT8* CPL_GetBuffRX( CPL_OBJ* pCPLObj, UINT16 *wSize);
BOOL CPL_RX( CPL_OBJ* pCPLObj, UINT16 wCount);
UINT16 CPL_GetCount( CPL_OBJ* pCPLObj, CPLBufferType type );
UINT16 CPL_GetFree( CPL_OBJ* pCPLObj, CPLBufferType type );
BOOL CPL_IsDataReady( CPL_OBJ* pCPLObj);
BOOL CPL_Reconnect( CPL_OBJ* pCPLObj);
BOOL CPL_direct_TX( CPL_OBJ* pCPLObj, const UINT8* bBuff, UINT16 wCount);
BOOL CPL_direct_RX( CPL_OBJ* pCPLObj, UINT8* bBuff, UINT16 *wCount);

#endif			/* _CPL_DEF_H */
