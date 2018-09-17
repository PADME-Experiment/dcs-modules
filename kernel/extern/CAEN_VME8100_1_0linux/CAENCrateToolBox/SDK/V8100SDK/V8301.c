////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file    V8301.c
*   \brief   V8301 VME board implementation
*   \author  NDA
*   \version 1.0
*   \date    09/2008
*            
*            Provides methods, properties and defines to handle V8301 VME boards
*/
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
// File includes
////////////////////////////////////////////////////////////////////////////////////////////////
#include <string.h>
#include <memory.h>
#include <stdlib.h>
#include "V8301.h"

////////////////////////////////////////////////////////////////////////////////////////////////
// File local defines
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
// Static variables declaration
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \var     static const cmd_table V8301_CMD_TABLE[]
*   \brief   The commands table
*            
*            Provides an entry for each supported command: keep synched with \ref V8301_COMMANDS
*/
////////////////////////////////////////////////////////////////////////////////////////////////
static const cmd_table V8301_CMD_TABLE[ ]=
{
	{ "$CMD:MON,CH:%d,PAR:NAME\r\n",			0,				V8301_MAX_CH- 1,	0,	0.0f,	0.0f,		CMD_PTYPE_STRING},		/*!< \brief Readout channel name (ex. +5V..) */
	{ "$CMD:MON,CH:%d,PAR:VSET\r\n",			0,				V8301_MAX_CH- 1,	0,	0.0f,	0.0f,		CMD_PTYPE_FLOAT},		/*!< \brief Readout value of the voltage Set */
	{ "$CMD:MON,CH:%d,PAR:VMIN\r\n",			0,				V8301_MAX_CH- 1,	0,	0.0f,	0.0f,		CMD_PTYPE_FLOAT},		/*!< \brief Readout minimum value of the voltage Set */
	{ "$CMD:MON,CH:%d,PAR:VMAX\r\n",			0,				V8301_MAX_CH- 1,	0,	0.0f,	0.0f,		CMD_PTYPE_FLOAT},		/*!< \brief Readout maximum value of the voltage Set */
	{ "$CMD:MON,CH:%d,PAR:VRES\r\n",			0,				V8301_MAX_CH- 1,	0,	0.0f,	0.0f,		CMD_PTYPE_FLOAT},		/*!< \brief Readout resolution value of the voltage Set */
	{ "$CMD:MON,CH:%d,PAR:OVP\r\n",				0,				V8301_MAX_CH- 1,	0,	0.0f,	0.0f,		CMD_PTYPE_FLOAT},		/*!< \brief Readout value Over Voltage Protection */
	{ "$CMD:MON,CH:%d,PAR:UVP\r\n",				0,				V8301_MAX_CH- 1,	0,	0.0f,	0.0f,		CMD_PTYPE_FLOAT},		/*!< \brief Readout value Under Voltage Protection */
	{ "$CMD:MON,CH:%d,PAR:VMON\r\n",			0,				V8301_MAX_CH- 1,	0,	0.0f,	0.0f,		CMD_PTYPE_FLOAT},		/*!< \brief Readout value of the voltage Monitor */
	{ "$CMD:MON,CH:%d,PAR:ISET\r\n",			0,				V8301_MAX_CH- 1,	0,	0.0f,	0.0f,		CMD_PTYPE_FLOAT},		/*!< \brief Readout value of current limit */
	{ "$CMD:MON,CH:%d,PAR:IMIN\r\n",			0,				V8301_MAX_CH- 1,	0,	0.0f,	0.0f,		CMD_PTYPE_FLOAT},		/*!< \brief Readout minimum value of current limit */
	{ "$CMD:MON,CH:%d,PAR:IMAX\r\n",			0,				V8301_MAX_CH- 1,	0,	0.0f,	0.0f,		CMD_PTYPE_FLOAT},		/*!< \brief Readout maximum value of current limit */
	{ "$CMD:MON,CH:%d,PAR:IRES\r\n",			0,				V8301_MAX_CH- 1,	0,	0.0f,	0.0f,		CMD_PTYPE_FLOAT},		/*!< \brief Readout value resolution of current limit */
	{ "$CMD:MON,CH:%d,PAR:IMON\r\n",			0,				V8301_MAX_CH- 1,	0,	0.0f,	0.0f,		CMD_PTYPE_FLOAT},		/*!< \brief Readout value of the current Monitor */
	{ "$CMD:MON,CH:%d,PAR:STAT\r\n",			0,				V8301_MAX_CH- 1,	0,	0.0f,	0.0f,		CMD_PTYPE_INT},			/*!< \brief Readout value of channel status */

	{ "$CMD:SET,CH:%d,PAR:VSET,VAL:%05.2f\r\n",	0,				V8301_MAX_CH- 1,	2,	0.0f,	1000.0f,	CMD_PTYPE_NONE},		/*!< \brief Set voltage Value */
	{ "$CMD:SET,CH:%d,PAR:ISET,VAL:%05.1f\r\n",	0,				V8301_MAX_CH- 1,	2,	0.0f,	1000.0f,	CMD_PTYPE_NONE},		/*!< \brief Set Value of current limit */
	{ "$CMD:SET,CH:%d,PAR:OVP,VAL:%02d\r\n",	0,				V8301_MAX_CH- 1,	1,	0.0f,	100.0f,		CMD_PTYPE_NONE},		/*!< \brief Set Value OVP (in % di Vset) */
	{ "$CMD:SET,CH:%d,PAR:UVP,VAL:%02d\r\n",	0,				V8301_MAX_CH- 1,	1,	0.0f,	100.0f,		CMD_PTYPE_NONE},		/*!< \brief Set Value UVP (in % di Vset) */

	{ "$CMD:MON,CH:%d,PAR:CRNAME\r\n",			V8301_CR_CH_ID,	V8301_CR_CH_ID,		0,	0.0f,	0.0f,		CMD_PTYPE_STRING},		/*!< \brief Readout Crate name : 'VME8100' */
	{ "$CMD:MON,CH:%d,PAR:NUMCH\r\n",			V8301_CR_CH_ID,	V8301_CR_CH_ID,		0,	0.0f,	0.0f,		CMD_PTYPE_INT},			/*!< \brief Readout the number of channels present */
	{ "$CMD:MON,CH:%d,PAR:PSFREL\r\n",			V8301_CR_CH_ID,	V8301_CR_CH_ID,		0,	0.0f,	0.0f,		CMD_PTYPE_STRING},		/*!< \brief Readout the PS Release Firmware */
	{ "$CMD:MON,CH:%d,PAR:PSTEMP\r\n",			V8301_CR_CH_ID,	V8301_CR_CH_ID,		0,	0.0f,	0.0f,		CMD_PTYPE_FLOAT},		/*!< \brief Readout the PS Temperature */
	{ "$CMD:MON,CH:%d,PAR:PSSNUM\r\n",			V8301_CR_CH_ID,	V8301_CR_CH_ID,		0,	0.0f,	0.0f,		CMD_PTYPE_INT},			/*!< \brief Readout the PS serial number */
	{ "$CMD:MON,CH:%d,PAR:FANSP\r\n",			V8301_CR_CH_ID,	V8301_CR_CH_ID,		0,	0.0f,	0.0f,		CMD_PTYPE_INT},			/*!< \brief Readout Set value of FAN SPEED \ref _V8301_FAN_SPEED_SET */
	{ "$CMD:MON,CH:%d,PAR:FAN1\r\n",			V8301_CR_CH_ID,	V8301_CR_CH_ID,		0,	0.0f,	0.0f,		CMD_PTYPE_INT},			/*!< \brief Readout value Fan Speed 1 (RPM) */
	{ "$CMD:MON,CH:%d,PAR:FAN2\r\n",			V8301_CR_CH_ID,	V8301_CR_CH_ID,		0,	0.0f,	0.0f,		CMD_PTYPE_INT},			/*!< \brief Readout value Fan Speed 2 (RPM) */
	{ "$CMD:MON,CH:%d,PAR:FAN3\r\n",			V8301_CR_CH_ID,	V8301_CR_CH_ID,		0,	0.0f,	0.0f,		CMD_PTYPE_INT},			/*!< \brief Readout value Fan Speed 3 (RPM) */
	{ "$CMD:MON,CH:%d,PAR:FUFREL\r\n",			V8301_CR_CH_ID,	V8301_CR_CH_ID,		0,	0.0f,	0.0f,		CMD_PTYPE_STRING},		/*!< \brief Readout the FAN UNIT Release Firmware */
	{ "$CMD:MON,CH:%d,PAR:FUTEMP\r\n",			V8301_CR_CH_ID,	V8301_CR_CH_ID,		0,	0.0f,	0.0f,		CMD_PTYPE_FLOAT},		/*!< \brief Readout value of the FAN UNIT Temperature */
	{ "$CMD:MON,CH:%d,PAR:FUSNUM\r\n",			V8301_CR_CH_ID,	V8301_CR_CH_ID,		0,	0.0f,	0.0f,		CMD_PTYPE_INT},			/*!< \brief Readout value FAN UNIT serial number */
	{ "$CMD:MON,CH:%d,PAR:CRST\r\n",			V8301_CR_CH_ID,	V8301_CR_CH_ID,		0,	0.0f,	0.0f,		CMD_PTYPE_INT},			/*!< \brief Readout value Crate status */
	{ "$CMD:MON,CH:%d,PAR:VPMAX\r\n",			V8301_CR_CH_ID,	V8301_CR_CH_ID,		0,	0.0f,	0.0f,		CMD_PTYPE_FLOAT},		/*!< \brief Readout value maximum of OVP/UVP */
	{ "$CMD:MON,CH:%d,PAR:VPMIN\r\n",			V8301_CR_CH_ID,	V8301_CR_CH_ID,		0,	0.0f,	0.0f,		CMD_PTYPE_FLOAT},		/*!< \brief Readout value minimum of OVP/UVP */
	{ "$CMD:MON,CH:%d,PAR:RS232BR\r\n",			V8301_CR_CH_ID,	V8301_CR_CH_ID,		0,	0.0f,	0.0f,		CMD_PTYPE_INT},			/*!< \brief Readout Bit Rate RS232 : \ref _V8301_RS232_BITRATE */
	{ "$CMD:MON,CH:%d,PAR:CANBR\r\n",			V8301_CR_CH_ID,	V8301_CR_CH_ID,		0,	0.0f,	0.0f,		CMD_PTYPE_INT},			/*!< \brief Readout Bit Rate CANBUS : \ref _V8301_CANBUS_BITRATE */
	{ "$CMD:MON,CH:%d,PAR:CANADD\r\n",			V8301_CR_CH_ID,	V8301_CR_CH_ID,		0,	0.0f,	0.0f,		CMD_PTYPE_INT},			/*!< \brief Readout Address CANBUS : 0..255 */
	{ "$CMD:MON,CH:%d,PAR:IPADD\r\n",			V8301_CR_CH_ID,	V8301_CR_CH_ID,		0,	0.0f,	0.0f,		CMD_PTYPE_STRING},		/*!< \brief Readout Address IP : xxx.xxx.xxx.xxx */
	{ "$CMD:MON,CH:%d,PAR:IPMSK\r\n",			V8301_CR_CH_ID,	V8301_CR_CH_ID,		0,	0.0f,	0.0f,		CMD_PTYPE_STRING},		/*!< \brief Readout IP Netmask : xxx.xxx.xxx.xxx */
	{ "$CMD:MON,CH:%d,PAR:IPGTW\r\n",			V8301_CR_CH_ID,	V8301_CR_CH_ID,		0,	0.0f,	0.0f,		CMD_PTYPE_STRING},		/*!< \brief Readout Gateway : xxx.xxx.xxx.xxx */
	{ "$CMD:MON,CH:%d,PAR:MACADD\r\n",			V8301_CR_CH_ID,	V8301_CR_CH_ID,		0,	0.0f,	0.0f,		CMD_PTYPE_STRING},		/*!< \brief Readout Mac Address : xx.xx.xx.xx.xx.xx */

	{ "$CMD:SET,CH:%d,PAR:ON\r\n",				V8301_CR_CH_ID,	V8301_CR_CH_ID,		0,	0.0f,	0.0f,		CMD_PTYPE_NONE},		/*!< \brief Crate ON */
	{ "$CMD:SET,CH:%d,PAR:OFF\r\n",				V8301_CR_CH_ID,	V8301_CR_CH_ID,		0,	0.0f,	0.0f,		CMD_PTYPE_NONE},		/*!< \brief Crate OFF */
	{ "$CMD:SET,CH:%d,PAR:SYSR\r\n",			V8301_CR_CH_ID,	V8301_CR_CH_ID,		0,	0.0f,	0.0f,		CMD_PTYPE_NONE},		/*!< \brief Sends VME System Reset */
	{ "$CMD:SET,CH:%d,PAR:CLR\r\n",				V8301_CR_CH_ID,	V8301_CR_CH_ID,		0,	0.0f,	0.0f,		CMD_PTYPE_NONE},		/*!< \brief Clear Alarm */
	{ "$CMD:SET,CH:%d,PAR:FANSP,VAL:%1d\r\n",	V8301_CR_CH_ID,	V8301_CR_CH_ID,		1,	0.0f,	6.0f,		CMD_PTYPE_NONE},		/*!< \brief Set Fan Speed \ref _V8301_FAN_SPEED_SET */

};

////////////////////////////////////////////////////////////////////////////////////////////////
// Static methods declaration
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
// Global visible variables declaration
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
//
//     B O A R D S   H A N D L I N G
//
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_open_com( V8301_data* p_data, const char *com_str, V8301_RS232_BITRATE bitrate)
{
	CPLConfig cpl_config;
	memset( p_data, 0, sizeof( V8301_data));

	// Communication device setup
	// COM:9600,N,8,1 Xon/Xoff flow control
	memset( &cpl_config, 0, sizeof( CPLConfig));
	cpl_config.m_device= CPL_COM;
	strncpy( cpl_config.m_device_config.m_com_device.m_com_str, com_str, sizeof( cpl_config.m_device_config.m_com_device.m_com_str)- 1);
	cpl_config.m_device_config.m_com_device.m_com_str[ sizeof( cpl_config.m_device_config.m_com_device.m_com_str)- 1]= '\0';
	switch( bitrate) {
		case V8301_RS232_BITRATE_9600:
			cpl_config.m_device_config.m_com_device.m_baudrate= SERBAUD_9600;
			break;
		case V8301_RS232_BITRATE_19200:
			cpl_config.m_device_config.m_com_device.m_baudrate= SERBAUD_19200;
			break;
		case V8301_RS232_BITRATE_38400:
			cpl_config.m_device_config.m_com_device.m_baudrate= SERBAUD_38400;
			break;
		case V8301_RS232_BITRATE_57600:
			cpl_config.m_device_config.m_com_device.m_baudrate= SERBAUD_57600;
			break;
		case V8301_RS232_BITRATE_115200:
			cpl_config.m_device_config.m_com_device.m_baudrate= SERBAUD_115200;
			break;
		default:
			return FALSE;		
	}
	cpl_config.m_device_config.m_com_device.m_dataBit= DATA_EIGHT;
	cpl_config.m_device_config.m_com_device.m_parity= SERPARITY_NONE;
	cpl_config.m_device_config.m_com_device.m_DTRFlow= DTR_DISABLE;
	cpl_config.m_device_config.m_com_device.m_RTSFlow= RTS_DISABLE;
	cpl_config.m_device_config.m_com_device.m_stopBit= STOP_ONE;
	cpl_config.m_device_config.m_com_device.m_enable_Xon_Xoff= TRUE;
	cpl_config.m_device_config.m_com_device.m_Xon= 0x11;
	cpl_config.m_device_config.m_com_device.m_Xoff= 0x13;

	// basic data V8301ToolBox initialization
	if( !board_open( &p_data->m_common_data, &cpl_config, V8301_CMD_TABLE)) {
		return FALSE;
	}
	//
	// board specific data initialization

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_open_eth( V8301_data* p_data, const char *ip_add)
{
	CPLConfig cpl_config;
	memset( p_data, 0, sizeof( V8301_data));

	// Communication device setup
	// ip_addr:8100
	memset( &cpl_config, 0, sizeof( CPLConfig));
	cpl_config.m_device= CPL_ETH;
	strncpy( cpl_config.m_device_config.m_eth_device.m_address, ip_add, sizeof( cpl_config.m_device_config.m_eth_device.m_address)- 1);
	cpl_config.m_device_config.m_eth_device.m_address[ sizeof( cpl_config.m_device_config.m_eth_device.m_address)- 1]= '\0';
	cpl_config.m_device_config.m_eth_device.m_port= 8100;

	// basic data V8301ToolBox initialization
	if( !board_open( &p_data->m_common_data, &cpl_config, V8301_CMD_TABLE)) {
		return FALSE;
	}
	//
	// board specific data initialization

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_close( V8301_data* p_data) {
	return board_close( &p_data->m_common_data);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
//     L E V E L   0   A P I s
//
////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////
//
//     L E V E L   1   A P I s
//
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_mon_name( V8301_data* p_data, UINT8 ch_id, char *value, int value_size, CMD_ERROR_CODE *error_code) {
	return send_cmd( &p_data->m_common_data, V8301_CMD_CH_MON_NAME, ch_id, NULL, value, value_size, error_code);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_mon_vset( V8301_data* p_data, UINT8 ch_id, float *value, CMD_ERROR_CODE *error_code) {
	return send_cmd( &p_data->m_common_data, V8301_CMD_CH_MON_VSET, ch_id, NULL, value, sizeof( *value), error_code);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_mon_vmin( V8301_data* p_data, UINT8 ch_id, float *value, CMD_ERROR_CODE *error_code) {
	return send_cmd( &p_data->m_common_data, V8301_CMD_CH_MON_VMIN, ch_id, NULL, value, sizeof( *value), error_code);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_mon_vmax( V8301_data* p_data, UINT8 ch_id, float *value, CMD_ERROR_CODE *error_code) {
	return send_cmd( &p_data->m_common_data, V8301_CMD_CH_MON_VMAX, ch_id, NULL, value, sizeof( *value), error_code);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_mon_vres( V8301_data* p_data, UINT8 ch_id, float *value, CMD_ERROR_CODE *error_code) {
	return send_cmd( &p_data->m_common_data, V8301_CMD_CH_MON_VRES, ch_id, NULL, value, sizeof( *value), error_code);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_mon_ovp( V8301_data* p_data, UINT8 ch_id, float *value, CMD_ERROR_CODE *error_code) {
	return send_cmd( &p_data->m_common_data, V8301_CMD_CH_MON_OVP, ch_id, NULL, value, sizeof( *value), error_code);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_mon_uvp( V8301_data* p_data, UINT8 ch_id, float *value, CMD_ERROR_CODE *error_code) {
	return send_cmd( &p_data->m_common_data, V8301_CMD_CH_MON_UVP, ch_id, NULL, value, sizeof( *value), error_code);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_mon_vmon( V8301_data* p_data, UINT8 ch_id, float *value, CMD_ERROR_CODE *error_code) {
	return send_cmd( &p_data->m_common_data, V8301_CMD_CH_MON_VMON, ch_id, NULL, value, sizeof( *value), error_code);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_mon_iset( V8301_data* p_data, UINT8 ch_id, float *value, CMD_ERROR_CODE *error_code) {
	return send_cmd( &p_data->m_common_data, V8301_CMD_CH_MON_ISET, ch_id, NULL, value, sizeof( *value), error_code);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_mon_imin( V8301_data* p_data, UINT8 ch_id, float *value, CMD_ERROR_CODE *error_code) {
	return send_cmd( &p_data->m_common_data, V8301_CMD_CH_MON_IMIN, ch_id, NULL, value, sizeof( *value), error_code);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_mon_imax( V8301_data* p_data, UINT8 ch_id, float *value, CMD_ERROR_CODE *error_code) {
	return send_cmd( &p_data->m_common_data, V8301_CMD_CH_MON_IMAX, ch_id, NULL, value, sizeof( *value), error_code);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_mon_ires( V8301_data* p_data, UINT8 ch_id, float *value, CMD_ERROR_CODE *error_code) {
	return send_cmd( &p_data->m_common_data, V8301_CMD_CH_MON_IRES, ch_id, NULL, value, sizeof( *value), error_code);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_mon_imon( V8301_data* p_data, UINT8 ch_id, float *value, CMD_ERROR_CODE *error_code) {
	return send_cmd( &p_data->m_common_data, V8301_CMD_CH_MON_IMON, ch_id, NULL, value, sizeof( *value), error_code);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_mon_stat( V8301_data* p_data, UINT8 ch_id, V8301_channel_status *value, CMD_ERROR_CODE *error_code) {
	int int_val;
	BOOL result= send_cmd( &p_data->m_common_data, V8301_CMD_CH_MON_STAT, ch_id, NULL, &int_val, sizeof( int_val), error_code);
	value->m_word= int_val;
	return result;

}

//
// Channel set methods
// 

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_set_vset( V8301_data* p_data, UINT8 ch_id, float value, CMD_ERROR_CODE *error_code) {
	return send_cmd( &p_data->m_common_data, V8301_CMD_CH_SET_VSET, ch_id, &value, NULL, 0, error_code);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_set_iset( V8301_data* p_data, UINT8 ch_id, float value, CMD_ERROR_CODE *error_code) {
	return send_cmd( &p_data->m_common_data, V8301_CMD_CH_SET_ISET, ch_id, &value, NULL, 0, error_code);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_set_ovp( V8301_data* p_data, UINT8 ch_id, UINT8 value, CMD_ERROR_CODE *error_code) {
	int int_val= value;
	return send_cmd( &p_data->m_common_data, V8301_CMD_CH_SET_OVP, ch_id, &int_val, NULL, 0, error_code);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_set_uvp( V8301_data* p_data, UINT8 ch_id, UINT8 value, CMD_ERROR_CODE *error_code) {
	int int_val= value;
	return send_cmd( &p_data->m_common_data, V8301_CMD_CH_SET_UVP, ch_id, &int_val, NULL, 0, error_code);
}

//
// Crate monitor methods
// 

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_mon_crname( V8301_data* p_data, char *value, int value_size, CMD_ERROR_CODE *error_code) {
	return send_cmd( &p_data->m_common_data, V8301_CMD_CR_MON_CRNAME, V8301_CR_CH_ID, NULL, value, value_size, error_code);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_mon_numch( V8301_data* p_data, UINT8 *value, CMD_ERROR_CODE *error_code) {
	int int_val;
	BOOL result= send_cmd( &p_data->m_common_data, V8301_CMD_CR_MON_NUMCH, V8301_CR_CH_ID, NULL, &int_val, sizeof( int_val), error_code);
	*value= int_val;
	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_mon_psfrel( V8301_data* p_data, char *value, int value_size, CMD_ERROR_CODE *error_code) {
	return send_cmd( &p_data->m_common_data, V8301_CMD_CR_MON_PSFREL, V8301_CR_CH_ID, NULL, value, value_size, error_code);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_mon_pstemp( V8301_data* p_data, float *value, CMD_ERROR_CODE *error_code) {
	return send_cmd( &p_data->m_common_data, V8301_CMD_CR_MON_PSTEMP, V8301_CR_CH_ID, NULL, value, sizeof( *value), error_code);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_mon_pssnum( V8301_data* p_data, UINT32 *value, CMD_ERROR_CODE *error_code) {
	int int_val;
	BOOL result= send_cmd( &p_data->m_common_data, V8301_CMD_CR_MON_PSSNUM, V8301_CR_CH_ID, NULL, &int_val, sizeof( int_val), error_code);
	*value= int_val;
	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_mon_fansp( V8301_data* p_data, V8301_FAN_SPEED_SET *value, CMD_ERROR_CODE *error_code) {
	int int_val;
	BOOL result= send_cmd( &p_data->m_common_data, V8301_CMD_CR_MON_FANSP, V8301_CR_CH_ID, NULL, &int_val, sizeof( int_val), error_code);
	*value= int_val;
	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_mon_fan1( V8301_data* p_data, UINT32 *value, CMD_ERROR_CODE *error_code) {
	int int_val;
	BOOL result= send_cmd( &p_data->m_common_data, V8301_CMD_CR_MON_FAN1, V8301_CR_CH_ID, NULL, &int_val, sizeof( int_val), error_code);
	*value= int_val;
	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_mon_fan2( V8301_data* p_data, UINT32 *value, CMD_ERROR_CODE *error_code) {
	int int_val;
	BOOL result= send_cmd( &p_data->m_common_data, V8301_CMD_CR_MON_FAN2, V8301_CR_CH_ID, NULL, &int_val, sizeof( int_val), error_code);
	*value= int_val;
	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_mon_fan3( V8301_data* p_data, UINT32 *value, CMD_ERROR_CODE *error_code) {
	int int_val;
	BOOL result= send_cmd( &p_data->m_common_data, V8301_CMD_CR_MON_FAN3, V8301_CR_CH_ID, NULL, &int_val, sizeof( int_val), error_code);
	*value= int_val;
	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_mon_fufrel( V8301_data* p_data, char *value, int value_size, CMD_ERROR_CODE *error_code) {
	return send_cmd( &p_data->m_common_data, V8301_CMD_CR_MON_FUFREL, V8301_CR_CH_ID, NULL, value, value_size, error_code);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_mon_futemp( V8301_data* p_data, float *value, CMD_ERROR_CODE *error_code) {
	return send_cmd( &p_data->m_common_data, V8301_CMD_CR_MON_FUTEMP, V8301_CR_CH_ID, NULL, value, sizeof( *value), error_code);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_mon_fusnum( V8301_data* p_data, UINT32 *value, CMD_ERROR_CODE *error_code) {
	int int_val;
	BOOL result= send_cmd( &p_data->m_common_data, V8301_CMD_CR_MON_FUSNUM, V8301_CR_CH_ID, NULL, &int_val, sizeof( int_val), error_code);
	*value= int_val;
	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_mon_crst( V8301_data* p_data, V8301_crate_status *value, CMD_ERROR_CODE *error_code) {
	int int_val;
	BOOL result= send_cmd( &p_data->m_common_data, V8301_CMD_CR_MON_CRST, V8301_CR_CH_ID, NULL, &int_val, sizeof( int_val), error_code);
	value->m_word= int_val;
	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_mon_vpmax( V8301_data* p_data, float *value, CMD_ERROR_CODE *error_code) {
	return send_cmd( &p_data->m_common_data, V8301_CMD_CR_MON_VPMAX, V8301_CR_CH_ID, NULL, value, sizeof( *value), error_code);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_mon_vpmin( V8301_data* p_data, float *value, CMD_ERROR_CODE *error_code) {
	return send_cmd( &p_data->m_common_data, V8301_CMD_CR_MON_VPMIN, V8301_CR_CH_ID, NULL, value, sizeof( *value), error_code);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_mon_rs232br( V8301_data* p_data, V8301_RS232_BITRATE *value, CMD_ERROR_CODE *error_code) {
	int int_val;
	BOOL result= send_cmd( &p_data->m_common_data, V8301_CMD_CR_MON_RS232BR, V8301_CR_CH_ID, NULL, &int_val, sizeof( int_val), error_code);
	*value= int_val;
	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_mon_canbr( V8301_data* p_data, V8301_CANBUS_BITRATE *value, CMD_ERROR_CODE *error_code) {
	int int_val;
	BOOL result= send_cmd( &p_data->m_common_data, V8301_CMD_CR_MON_CANBR, V8301_CR_CH_ID, NULL, &int_val, sizeof( int_val), error_code);
	*value= int_val;
	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_mon_canadd( V8301_data* p_data, UINT8 *value, CMD_ERROR_CODE *error_code) {
	int int_val;
	BOOL result= send_cmd( &p_data->m_common_data, V8301_CMD_CR_MON_CANADD, V8301_CR_CH_ID, NULL, &int_val, sizeof( int_val), error_code);
	*value= int_val;
	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_mon_ipadd( V8301_data* p_data, char *value, int value_size, CMD_ERROR_CODE *error_code) {
	return send_cmd( &p_data->m_common_data, V8301_CMD_CR_MON_IPADD, V8301_CR_CH_ID, NULL, value, value_size, error_code);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_mon_ipmsk( V8301_data* p_data, char *value, int value_size, CMD_ERROR_CODE *error_code) {
	return send_cmd( &p_data->m_common_data, V8301_CMD_CR_MON_IPMSK, V8301_CR_CH_ID, NULL, value, value_size, error_code);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_mon_ipgtw( V8301_data* p_data, char *value, int value_size, CMD_ERROR_CODE *error_code) {
	return send_cmd( &p_data->m_common_data, V8301_CMD_CR_MON_IPGTW, V8301_CR_CH_ID, NULL, value, value_size, error_code);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_mon_macadd( V8301_data* p_data, char *value, int value_size, CMD_ERROR_CODE *error_code) {
	return send_cmd( &p_data->m_common_data, V8301_CMD_CR_MON_MACADD, V8301_CR_CH_ID, NULL, value, value_size, error_code);
}

//
// Crate set methods
// 

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_set_on( V8301_data* p_data, CMD_ERROR_CODE *error_code) {
	return send_cmd( &p_data->m_common_data, V8301_CMD_CR_SET_ON, V8301_CR_CH_ID, NULL, NULL, 0, error_code);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_set_off( V8301_data* p_data, CMD_ERROR_CODE *error_code) {
	return send_cmd( &p_data->m_common_data, V8301_CMD_CR_SET_OFF, V8301_CR_CH_ID, NULL, NULL, 0, error_code);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_set_sysr( V8301_data* p_data, CMD_ERROR_CODE *error_code) {
	return send_cmd( &p_data->m_common_data, V8301_CMD_CR_SET_SYSR, V8301_CR_CH_ID, NULL, NULL, 0, error_code);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_set_clr( V8301_data* p_data, CMD_ERROR_CODE *error_code) {
	return send_cmd( &p_data->m_common_data, V8301_CMD_CR_SET_CLR, V8301_CR_CH_ID, NULL, NULL, 0, error_code);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8301_set_fansp( V8301_data* p_data, V8301_FAN_SPEED_SET value, CMD_ERROR_CODE *error_code) {
	int int_val= value;
	return send_cmd( &p_data->m_common_data, V8301_CMD_CR_SET_FANSP, V8301_CR_CH_ID, &int_val, NULL, 0, error_code);
}


////////////////////////////////////////////////////////////////////////////////////////////////
//
//     L E V E L   2   A P I s
//
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
//
//     M I S C E L L A N E O U S   A P I s
//
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////
const char* V8301_SW_rev( void)
{
	return "V8301SDK Rev. 1.0";
}
