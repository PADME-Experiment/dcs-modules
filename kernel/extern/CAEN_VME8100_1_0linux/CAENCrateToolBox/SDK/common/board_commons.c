////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file    board_commons.c
*   \brief   Board common features implementation
*   \author  NDA
*   \version 1.0
*   \date    03/2006
*            
*            boards common APIs: These APIs provide features common to every board.
*/
////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////
// File includes
////////////////////////////////////////////////////////////////////////////////////////////////
#include <memory.h>
#include <string.h>
#include <stdlib.h>

#ifdef LINUX
	#include <unistd.h>
    #include <sys/time.h>
#endif

#ifdef WIN32
    #include <time.h>
    #include <sys/timeb.h>
#endif

#include "com_phy_layer.h"
#include "board_commons.h"

////////////////////////////////////////////////////////////////////////////////////////////////
// File local defines
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
// Static variables declaration
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
// Static methods declaration
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
UINT8 ror_bits( UINT8 data, int num_bits)
{
	while( num_bits--)
	{
		data= ( data& 0x01)? (data>> 1)|0x80: (data>> 1);
	}
	return data;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
UINT8 swap_bits( UINT8 data)
{
	UINT8 tmp= 0x00;
	UINT8 msk_l, msk_r;
	for( msk_l= 0x80, msk_r= 0x01; msk_l; msk_l>>= 1, msk_r<<= 1)
	{
		tmp|= (data& msk_l)? msk_r: 0x00;
	}
	return tmp;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
void delay(int msec)
{
#if defined (LINUX)
	usleep( msec* 1000); 
#elif defined (WIN32)
	Sleep( msec);
#else
	???
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
UINT32 get_time()
{
	static UINT32 startup= 0;
	UINT32 time_ms;
#if defined (LINUX)
	struct timeval t1;
	struct timezone tz;

	gettimeofday(&t1, &tz);
	if( !startup) {
		startup= (UINT32 )t1.tv_sec;
	}
	time_ms = (t1.tv_sec- startup) * 1000 + t1.tv_usec / 1000;

#elif defined (WIN32)
	struct _timeb timebuffer;

	_ftime( &timebuffer );
	if( !startup) {
		startup= (long)timebuffer.time;
	}
	time_ms = (UINT32 )( timebuffer.time- startup) * 1000 + (UINT32 )timebuffer.millitm;
#else
	???
#endif

	return time_ms;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Global visible variables declaration
////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef WIN32
/*
        -----------------------------------------------------------------------------

        DllMain
        Entry point della libreria (inizializzare qui i dati globali)

        -----------------------------------------------------------------------------
*/
BOOL WINAPI DllMain( HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved )
{
    static int m_num_instances= 0;

        switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
                {
					if( !m_num_instances)
					{
						/* Socket library setup */
						WSADATA wsaData;
						if ( WSAStartup( MAKEWORD(2,2), &wsaData )!= NO_ERROR )
						{
							return	FALSE;
						}
					}
					++m_num_instances;
                }
                break;
        case DLL_PROCESS_DETACH:
				--m_num_instances;
				if( !m_num_instances){
					/* Socket library cleanup */
					WSACleanup();
				}
				break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
                break;
        }
        return TRUE;
}

#else   //Linux
/*
        -----------------------------------------------------------------------------

        _init
        Library entry point

        -----------------------------------------------------------------------------
*/
void _init( void )
{
}

/*
        -----------------------------------------------------------------------------

        _fini
        Library exit point

        -----------------------------------------------------------------------------
*/
void _fini( void )
{
}

#endif // WIN32

////////////////////////////////////////////////////////////////////////////////////////////////
//
//     B O A R D S   H A N D L I N G
//
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL board_open( board_data* p_data, const CPLConfig *p_com_config, const cmd_table* p_cmd_table)
{
	memset( p_data, 0, sizeof( board_data));

	// init fields
	p_data->m_p_cmd_table= p_cmd_table;
	p_data->m_p_last_sent_cmd= malloc( LAST_SEND_CMD_SIZE);
	*p_data->m_p_last_sent_cmd= '\0';

	p_data->m_p_asw_buff= malloc( ASW_BUFF_SIZE);
	p_data->m_p_tmp_asw_buff= malloc( ASW_BUFF_SIZE);
	*p_data->m_p_asw_buff= '\0';
	*p_data->m_p_tmp_asw_buff= '\0';

	p_data->m_asw_buff_len= 0;
	memset( p_data->m_p_asw_buff, 0, ASW_BUFF_SIZE);

	p_data->m_p_cpl_object= malloc( sizeof( CPL_OBJ));

	CPL_BiosInit( p_data->m_p_cpl_object);
	if( !CPL_Open( p_data->m_p_cpl_object, p_com_config))
	{
		free( p_data->m_p_cpl_object);
		p_data->m_p_cpl_object= (CPL_OBJ*)0;

		free( p_data->m_p_last_sent_cmd);
		p_data->m_p_last_sent_cmd= (char*)0;

		return FALSE;
	}
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL board_close( board_data* p_data)
{
	if( p_data->m_p_cpl_object){
		if( !CPL_Close( p_data->m_p_cpl_object)) {
			
			return FALSE;
		}
		free( p_data->m_p_cpl_object);
		p_data->m_p_cpl_object= (CPL_OBJ*)0;
	}
	if( p_data->m_p_last_sent_cmd) {
		free( p_data->m_p_last_sent_cmd);
		p_data->m_p_last_sent_cmd= (char*)0;
	}
	if( p_data->m_p_asw_buff){
		free( p_data->m_p_asw_buff);
		p_data->m_p_asw_buff= (char*)0;
	}
	if( p_data->m_p_tmp_asw_buff){
		free( p_data->m_p_tmp_asw_buff);
		p_data->m_p_tmp_asw_buff= (char*)0;
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
//     L E V E L   0   A P I s
//
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL send_buffer( board_data* p_data, const UINT8* buff, UINT16 buff_size)
{
	p_data->m_last_cmd_err= CMD_ERR_OK;
	CPL_Flush( p_data->m_p_cpl_object, BOTH_BUFFER );
	if( !CPL_direct_TX( p_data->m_p_cpl_object, buff, buff_size)) {
		p_data->m_last_cmd_err= CMD_ERR_TX;
		return FALSE;
	}
/*
	if( !buff_size)
		return TRUE;
	while( buff_size--)
	{
		if( !CPL_direct_TX( p_data->m_p_cpl_object, buff, 1))
			return FALSE;
		buff++;
		Sleep( 50);
	}
*/	
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL receive_buffer( board_data* p_data, UINT8* buff, UINT16 *buff_size, int timeout_msec)
{
	UINT16 bytes_to_read= *buff_size- 1;
	UINT32 timeout= 4000;		// First byte timeout (mesc)
	UINT32 start_time= get_time();


	p_data->m_last_cmd_err= CMD_ERR_OK;
	*buff_size= 0;
	while( (*buff_size< bytes_to_read)&& (( get_time()- start_time)< timeout))
	{
		UINT16 bytes_read= bytes_to_read- *buff_size;

		if( !CPL_direct_RX( p_data->m_p_cpl_object, buff+ *buff_size, &bytes_read))
		{
			p_data->m_last_cmd_err= CMD_ERR_RX;
			return FALSE;
		}
		if( bytes_read)
		{
			*buff_size+= bytes_read;
			timeout= timeout_msec;
			start_time= get_time();
		}
		else
		{
			Sleep( 1);
		}
	}
	if(!*buff_size) {
		// Answer timeout
		p_data->m_last_cmd_err= CMD_ERR_TOUT;
		return FALSE;
	}

	p_data->m_asw_buff_len= *buff_size;
	buff[ *buff_size]= '\0';
	
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
const char* get_last_answer( board_data* p_data) 
{
	int buff_len= p_data->m_asw_buff_len;
	char *src_buff= p_data->m_p_asw_buff;
	char *dest_buff= p_data->m_p_tmp_asw_buff;
	// strip away null chars from the last read buffer
	while( buff_len--)
	{
		if( *src_buff)
		{
			*dest_buff++= *src_buff;
		}
		++src_buff;
	}
	*dest_buff++= '\0';
	return p_data->m_p_tmp_asw_buff;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
CMD_ERROR_CODE get_last_error( board_data* p_data) {
	return p_data->m_last_cmd_err;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
//     L E V E L   1   A P I s
//
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL send_cmd( board_data* p_data, int cmd_index, int ch_index, const void *cmd_value, void *cmd_result, int cmd_result_size, CMD_ERROR_CODE* error_code) {
	UINT16 asw_buff_size= ASW_BUFF_SIZE;

	if( ( p_data->m_p_cmd_table[ cmd_index].m_min_ch_index> ch_index)||
		( p_data->m_p_cmd_table[ cmd_index].m_max_ch_index< ch_index)) {
		return FALSE;
	}
	switch( p_data->m_p_cmd_table[ cmd_index].m_param_type) { 
		case CMD_PTYPE_NONE:		// The command supports no parameter
			sprintf( p_data->m_p_last_sent_cmd, p_data->m_p_cmd_table[ cmd_index].m_cmd_string, ch_index);
			break;
		case CMD_PTYPE_INT:			// The command supports one integer parameter
			if( ( p_data->m_p_cmd_table[ cmd_index].m_min_value> (float)*(int*)cmd_value)||
				( p_data->m_p_cmd_table[ cmd_index].m_max_value< (float)*(int*)cmd_value)) {
				p_data->m_last_cmd_err= CMD_ERR_VAL;
				return FALSE;
			}
			sprintf( p_data->m_p_last_sent_cmd, p_data->m_p_cmd_table[ cmd_index].m_cmd_string, ch_index, *(int*)cmd_value);
			break;
		case CMD_PTYPE_FLOAT:		// The command supports one float parameter
			if( ( p_data->m_p_cmd_table[ cmd_index].m_min_value> *(float*)cmd_value)||
				( p_data->m_p_cmd_table[ cmd_index].m_max_value< *(float*)cmd_value)) {
				p_data->m_last_cmd_err= CMD_ERR_VAL;
				return FALSE;
			}
			sprintf( p_data->m_p_last_sent_cmd, p_data->m_p_cmd_table[ cmd_index].m_cmd_string, ch_index, *(float*)cmd_value);
			break;
		default:
			return FALSE;
	}
	//
	// Send the command
	if( !send_buffer( p_data, (UINT8*)p_data->m_p_last_sent_cmd, (UINT16)strlen( p_data->m_p_last_sent_cmd))){
		if( error_code) {
			*error_code= p_data->m_last_cmd_err;
		}
		return FALSE;
	}
	//
	// Receive the answer 
	if( !receive_buffer( p_data, (UINT8*)p_data->m_p_asw_buff, &asw_buff_size, 40)) {
		if( error_code) {
			*error_code= p_data->m_last_cmd_err;
		}
		return FALSE;
	}
	if( p_data->m_last_cmd_err== CMD_ERR_OK) {
		// Verify the answer
		if( !strnicmp( CMD_ASW_CMD_ERR, p_data->m_p_asw_buff, strlen( CMD_ASW_CMD_ERR))){
			p_data->m_last_cmd_err= CMD_ERR_CMD;
		} else if( !strnicmp( CMD_ASW_CH_ERR, p_data->m_p_asw_buff, strlen( CMD_ASW_CH_ERR))){
			p_data->m_last_cmd_err= CMD_ERR_CH;
		} else if( !strnicmp( CMD_ASW_PAR_ERR, p_data->m_p_asw_buff, strlen( CMD_ASW_PAR_ERR))){
			p_data->m_last_cmd_err= CMD_ERR_PAR;
		} else if( !strnicmp( CMD_ASW_VAL_ERR, p_data->m_p_asw_buff, strlen( CMD_ASW_VAL_ERR))){
			p_data->m_last_cmd_err= CMD_ERR_VAL;
		} else if( !strnicmp( CMD_ASW_OK, p_data->m_p_asw_buff, strlen( CMD_ASW_OK))){
			p_data->m_last_cmd_err= CMD_ERR_OK;
		} else {
			p_data->m_last_cmd_err= CMD_ERR_BADASW;
		}
	}
	if( p_data->m_last_cmd_err== CMD_ERR_OK) {
		switch( p_data->m_p_cmd_table[ cmd_index].m_result_type){ 
		case CMD_PTYPE_NONE:		// No result expected
			break;
		case CMD_PTYPE_INT:			// Get the result as integer value
			sscanf( p_data->m_p_asw_buff, CMD_ASW_OK_INT, (int*)cmd_result);
			break;
		case CMD_PTYPE_FLOAT:		// Get the result as floating point value
			sscanf( p_data->m_p_asw_buff, CMD_ASW_OK_FLOAT, (float*)cmd_result);
			break;
		case CMD_PTYPE_STRING:		// Get the result as string value
			{
				char tmp_buff[ 100];
				if( strlen( p_data->m_p_asw_buff)> sizeof( tmp_buff))
				{
					p_data->m_last_cmd_err= CMD_ERR_BADASW;
					break;
				}
				sscanf( p_data->m_p_asw_buff, CMD_ASW_OK_STRING, tmp_buff);
				strncpy( cmd_result, tmp_buff, (size_t)cmd_result_size);
				((char*)cmd_result)[ cmd_result_size- 1]= '\0';
			}
			break;
		default:
			return FALSE;
		}
	}
	if( error_code) {
		*error_code= p_data->m_last_cmd_err;
	}
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////
//
//     G L O B A L   A P I s
//
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
//
//     M I S C E L L A N E O U S   A P I s
//
////////////////////////////////////////////////////////////////////////////////////////////////
