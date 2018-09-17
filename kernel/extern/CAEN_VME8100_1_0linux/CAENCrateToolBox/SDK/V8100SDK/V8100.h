////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file    V8100.h
*   \brief   V8100 board definitions
*   \author  NDA
*   \version 1.0
*   \date    09/2008
*            
*            Provides methods, properties and defines to handle V8100 boards
*/
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __V8100_DEF_H
#define __V8100_DEF_H

////////////////////////////////////////////////////////////////////////////////////////////////
// File includes
////////////////////////////////////////////////////////////////////////////////////////////////
#include "../common/common_defs.h"
#include "../common/board_commons.h"

////////////////////////////////////////////////////////////////////////////////////////////////
// Global defines
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \def     V8100_CR_CH_ID
*   \brief   The V8100 crate channel identifier
*/
////////////////////////////////////////////////////////////////////////////////////////////////
#define V8100_CR_CH_ID		8		

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \def     V8100_MAX_CH
*   \brief   The V8100 crate maximum channels' number
*/
////////////////////////////////////////////////////////////////////////////////////////////////
#define V8100_MAX_CH		8

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \struct  V8100_data
*   \brief   V8100 board data structure
*            
*            This structure stores any board data, common and specific ones. 
*            It must be passed to each V8100's API 
*/
////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	board_data	m_common_data;					/*!< Boards common data structure: always let at offset 0 */
	//
	// Board specific data
} V8100_data;

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \union   V8100_channel_status
*   \brief   V8100 channel status as word and as bitfields
*            
*/
////////////////////////////////////////////////////////////////////////////////////////////////
typedef union
{
	struct {
		BOOL m_ON:1;						/*!< \brief ON/OFF status */
		BOOL m_OverCurrent:1;				/*!< \brief Current provided > Iset */
		BOOL m_OverVoltageProtection:1;		/*!< \brief Vout > (Vset * (100 + OVP)) / 100 */
		BOOL m_UnderVoltageProtection:1;	/*!< \brief VMON < (Vset * (100 � UVP)) / 100 */
		BOOL m_OverTemperature:1;			/*!< \brief Temperature channel > 90�C */
		UINT8 m_filler0:3;					/*!< \brief Undefined fillers */
		BOOL m_CalibrationError:1;			/*!< \brief Error on coefficient of calibration */
	} m_bit;								/*!< \brief The channel status bit fields */
	UINT16 m_word;							/*!< \brief The channel status word */
} V8100_channel_status;

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \union   V8100_crate_status
*   \brief   V8100 crate status as word and as bitfields
*            
*/
////////////////////////////////////////////////////////////////////////////////////////////////
typedef union
{
	struct {
		BOOL m_ON:1;						/*!< \brief ON/OFF status */
		BOOL m_VCCSupplyFail:1;				/*!< \brief Problems on local power supplies */
		BOOL m_PSTemperatureFail:1;			/*!< \brief Temperature on PS < 5�C or > 65�C */
		BOOL m_ACFail:1;					/*!< \brief Problems on MAIN POWER SUPPLY */
		BOOL m_VMESystemFail:1;				/*!< \brief System Fail VME active */
		UINT8 m_filler0:4;					/*!< \brief Undefined fillers */
		BOOL m_FanON:1;						/*!< \brief Fan ON/OFF status */
		BOOL m_Fan1Fail:1;					/*!< \brief Fan 1 Speed < 10% or > 10% nominal value */
		BOOL m_Fan2Fail:1;					/*!< \brief Fan 2 Speed < 10% or > 10% nominal value */
		BOOL m_Fan3Fail:1;					/*!< \brief Fan 3 Speed < 10% or > 10% nominal value */
		BOOL m_FANTemperatureFail:1;		/*!< \brief Temperature FAN < 5�C or > 65�C */
	} m_bit;								/*!< \brief The crate status bit fields */
	UINT16 m_word;							/*!< \brief The crate status word */
} V8100_crate_status;

////////////////////////////////////////////////////////////////////////////////////////////////
// Commands index
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \enum    _V8100_COMMANDS
*   \brief   The board commands
*            
*            Provides an entry for supported command: keep synched with \ref cmd_table V8100_CMD_TABLE[ ]
*/
////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum _V8100_COMMANDS {
	//
	// Channel Monitor commands
	V8100_CMD_CH_MON_NAME= 0,				/*!< \brief Readout channel name (ex. +5V..) */
	V8100_CMD_CH_MON_VSET,					/*!< \brief Readout value of the voltage Set */
	V8100_CMD_CH_MON_VMIN,					/*!< \brief Readout minimum value of the voltage Set */
	V8100_CMD_CH_MON_VMAX,					/*!< \brief Readout maximum value of the voltage Set */
	V8100_CMD_CH_MON_VRES,					/*!< \brief Readout resolution value of the voltage Set */
	V8100_CMD_CH_MON_OVP,					/*!< \brief Readout value Over Voltage Protection */
	V8100_CMD_CH_MON_UVP,					/*!< \brief Readout value Under Voltage Protection */
	V8100_CMD_CH_MON_VMON,					/*!< \brief Readout value of the voltage Monitor */
	V8100_CMD_CH_MON_ISET,					/*!< \brief Readout value of current limit */
	V8100_CMD_CH_MON_IMIN,					/*!< \brief Readout minimum value of current limit */
	V8100_CMD_CH_MON_IMAX,					/*!< \brief Readout maximum value of current limit */
	V8100_CMD_CH_MON_IRES,					/*!< \brief Readout value resolution of current limit */
	V8100_CMD_CH_MON_IMON,					/*!< \brief Readout value of the current Monitor */
	V8100_CMD_CH_MON_STAT,					/*!< \brief Readout value of channel status */
	//
	// Channel Set commands
	V8100_CMD_CH_SET_VSET,					/*!< \brief Set voltage Value */
	V8100_CMD_CH_SET_ISET,					/*!< \brief Set Value of current limit */
	V8100_CMD_CH_SET_OVP,					/*!< \brief Set Value OVP (in % di Vset) */
	V8100_CMD_CH_SET_UVP,					/*!< \brief Set Value UVP (in % di Vset) */
	//
	// Crate Monitor commands
	V8100_CMD_CR_MON_CRNAME,				/*!< \brief Readout Crate name : 'VME8100' */
	V8100_CMD_CR_MON_NUMCH,					/*!< \brief Readout number of channels present */
	V8100_CMD_CR_MON_PSFREL,				/*!< \brief Readout of the PS Release Firmware */
	V8100_CMD_CR_MON_PSTEMP,				/*!< \brief Readout of the PS Temperature */
	V8100_CMD_CR_MON_PSSNUM,				/*!< \brief Readout of the PS serial number */
	V8100_CMD_CR_MON_FANSP,					/*!< \brief Readout Set value of FAN SPEED (0..6) */
	V8100_CMD_CR_MON_FAN1,					/*!< \brief Readout value Fan Speed 1 (RPM) */
	V8100_CMD_CR_MON_FAN2,					/*!< \brief Readout value Fan Speed 2 (RPM) */
	V8100_CMD_CR_MON_FAN3,					/*!< \brief Readout value Fan Speed 3 (RPM) */
	V8100_CMD_CR_MON_FUFREL,				/*!< \brief Readout of the FAN UNIT Release Firmware */
	V8100_CMD_CR_MON_FUTEMP,				/*!< \brief Readout of the FAN UNIT Temperature */
	V8100_CMD_CR_MON_FUSNUM,				/*!< \brief Readout of the FAN UNIT serial number */
	V8100_CMD_CR_MON_CRST,					/*!< \brief Readout value Crate status */
	V8100_CMD_CR_MON_VPMAX,					/*!< \brief Readout value maximum of OVP/UVP */
	V8100_CMD_CR_MON_VPMIN,					/*!< \brief Readout value minimum of OVP/UVP */
	V8100_CMD_CR_MON_RS232BR,				/*!< \brief Readout Bit Rate RS232 : 0..4 */
	V8100_CMD_CR_MON_CANBR,					/*!< \brief Readout Bit Rate CANBUS : 0..5 */
	V8100_CMD_CR_MON_CANADD,				/*!< \brief Readout Address CANBUS : 0..255 */
	V8100_CMD_CR_MON_IPADD,					/*!< \brief Readout Address IP : xxx.xxx.xxx.xxx */
	V8100_CMD_CR_MON_IPMSK,					/*!< \brief Readout IP Netmask : xxx.xxx.xxx.xxx */
	V8100_CMD_CR_MON_IPGTW,					/*!< \brief Readout Gateway : xxx.xxx.xxx.xxx */
	V8100_CMD_CR_MON_MACADD,				/*!< \brief Readout Mac Address : xx.xx.xx.xx.xx.xx */
	//
	// Crate Set commands
	V8100_CMD_CR_SET_ON,					/*!< \brief Crate ON */
	V8100_CMD_CR_SET_OFF,					/*!< \brief Crate OFF */
	V8100_CMD_CR_SET_SYSR,					/*!< \brief Sends VME System Reset */
	V8100_CMD_CR_SET_CLR,					/*!< \brief Clear Alarm */
	V8100_CMD_CR_SET_FANSP,					/*!< \brief Set Fan Speed. Values 0..6 */

} V8100_COMMANDS;

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \enum    _V8100_RS232_BITRATE
*   \brief   RS232 bitrate values
*            
*/
////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum _V8100_RS232_BITRATE {
	V8100_RS232_BITRATE_9600= 0,
	V8100_RS232_BITRATE_19200,
	V8100_RS232_BITRATE_38400,
	V8100_RS232_BITRATE_57600,
	V8100_RS232_BITRATE_115200,
} V8100_RS232_BITRATE;

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \enum    _V8100_CANBUS_BITRATE
*   \brief   CANBUS bitrate values
*            
*/
////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum _V8100_CANBUS_BITRATE {
	V8100_CANBUS_BITRATE_1M= 0,
	V8100_CANBUS_BITRATE_500K,
	V8100_CANBUS_BITRATE_250K,
	V8100_CANBUS_BITRATE_100K,
	V8100_CANBUS_BITRATE_50K,
	V8100_CANBUS_BITRATE_10K,
} V8100_CANBUS_BITRATE;

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \enum    _V8100_FAN_SPEED_SET
*   \brief   Fan speed setting values (RPMs)
*            
*/
////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum _V8100_FAN_SPEED_SET {
	V8100_FANSP_OFF= 0,					/*!< \brief Fan OFF */
	V8100_FANSP_1500RPM,
	V8100_FANSP_1800RPM,
	V8100_FANSP_2000RPM,
	V8100_FANSP_2300RPM,
	V8100_FANSP_2600RPM,
	V8100_FANSP_3000RPM,
} V8100_FAN_SPEED_SET;

////////////////////////////////////////////////////////////////////////////////////////////////
// Global variables declaration
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
// Global methods declaration
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
//
//     B O A R D S   H A N D L I N G
//
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn      BOOL V8100_open_com( V8100_data* p_data, const char *com_str, V8100_RS232_BITRATE bitrate);
*   \brief   V8100 boards data initialization and serial communication device opening.
*            
*            Provides specific handling for V8100 boards opening on serial communication device.
*   \param   p_data Pointer to board data
*   \param   com_str The serial communication device to open 
*                    e.g COM1.. COM# for Windows or /dev/ttyS0.. /dev/ttyS# or /dev/ttyUSB0.. /dev/ttyUSB# for Linux
*   \param   bitrate The serial device communication baudrate
*   \return  TRUE: board successfully opened
*   \note    Must be called before any other board specific API.
*/
////////////////////////////////////////////////////////////////////////////////////////////////
 BOOL V8100_open_com( V8100_data* p_data, const char *com_str, V8100_RS232_BITRATE bitrate);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn      BOOL V8100_open_eth( V8100_data* p_data, const char *ip_add);
*   \brief   V8100 boards data initialization and socket communication device opening.
*            
*            Provides specific handling for V8100 boards opening on socket communication device.
*   \param   p_data Pointer to board data
*   \param   ip_add The crate ip address expressed as four eight-bit octets 
*                    e.g xxx.xxx.xxx.xxx
*   \return  TRUE: board successfully opened
*   \note    Must be called before any other board specific API.
*/
////////////////////////////////////////////////////////////////////////////////////////////////
 BOOL V8100_open_eth( V8100_data* p_data, const char *ip_add);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn      BOOL V8100_close( V8100_data* p_data);
*   \brief   V8100 boards closing and resource free
*            
*            Provides specific handling for V8100 boards closing.
*   \param   p_data Pointer to board data
*   \return  TRUE: board successfully closed
*   \note    Must be called when done with any other board specific API.
*/
////////////////////////////////////////////////////////////////////////////////////////////////
 BOOL V8100_close( V8100_data* p_data);

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


//
// Channel monitor methods
// 

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn      BOOL V8100_mon_name( V8100_data* p_data, UINT8 ch_id, char *value, int value_size, CMD_ERROR_CODE *error_code);
*   \brief   Gets the channel name
*            
*   \param   p_data Pointer to board data
*   \param   ch_id The channel identifier
*   \param   value The read value buffer (caller allocated)
*   \param   value_size The value buffer size (BYTEs)
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_mon_name( V8100_data* p_data, UINT8 ch_id, char *value, int value_size, CMD_ERROR_CODE *error_code);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_mon_vset( V8100_data* p_data, UINT8 ch_id, float *value, CMD_ERROR_CODE *error_code);
*   \brief   Gets the value of the voltage Set
*            
*   \param   p_data Pointer to board data
*   \param   ch_id The channel identifier
*   \param   value The pointer to the read value
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_mon_vset( V8100_data* p_data, UINT8 ch_id, float *value, CMD_ERROR_CODE *error_code);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_mon_vmin( V8100_data* p_data, UINT8 ch_id, float *value, CMD_ERROR_CODE *error_code);
*   \brief   Gets the minimum value of the voltage Set
*            
*   \param   p_data Pointer to board data
*   \param   ch_id The channel identifier
*   \param   value The pointer to the read value
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_mon_vmin( V8100_data* p_data, UINT8 ch_id, float *value, CMD_ERROR_CODE *error_code);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_mon_vmax( V8100_data* p_data, UINT8 ch_id, float *value, CMD_ERROR_CODE *error_code);
*   \brief   Gets the maximum value of the voltage Set
*            
*   \param   p_data Pointer to board data
*   \param   ch_id The channel identifier
*   \param   value The pointer to the read value
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_mon_vmax( V8100_data* p_data, UINT8 ch_id, float *value, CMD_ERROR_CODE *error_code);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_mon_vres( V8100_data* p_data, UINT8 ch_id, float *value, CMD_ERROR_CODE *error_code);
*   \brief   Gets the resolution value of the voltage Set
*            
*   \param   p_data Pointer to board data
*   \param   ch_id The channel identifier
*   \param   value The pointer to the read value
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_mon_vres( V8100_data* p_data, UINT8 ch_id, float *value, CMD_ERROR_CODE *error_code);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_mon_ovp( V8100_data* p_data, UINT8 ch_id, float *value, CMD_ERROR_CODE *error_code);
*   \brief   Gets the value of the Over Voltage Protection
*            
*   \param   p_data Pointer to board data
*   \param   ch_id The channel identifier
*   \param   value The pointer to the read value
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_mon_ovp( V8100_data* p_data, UINT8 ch_id, float *value, CMD_ERROR_CODE *error_code);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_mon_uvp( V8100_data* p_data, UINT8 ch_id, float *value, CMD_ERROR_CODE *error_code);
*   \brief   Gets the value of the Under Voltage Protection
*            
*   \param   p_data Pointer to board data
*   \param   ch_id The channel identifier
*   \param   value The pointer to the read value
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_mon_uvp( V8100_data* p_data, UINT8 ch_id, float *value, CMD_ERROR_CODE *error_code);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_mon_vmon( V8100_data* p_data, UINT8 ch_id, float *value, CMD_ERROR_CODE *error_code);
*   \brief   Gets the value of the voltage Monitor
*            
*   \param   p_data Pointer to board data
*   \param   ch_id The channel identifier
*   \param   value The pointer to the read value
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_mon_vmon( V8100_data* p_data, UINT8 ch_id, float *value, CMD_ERROR_CODE *error_code);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_mon_iset( V8100_data* p_data, UINT8 ch_id, float *value, CMD_ERROR_CODE *error_code);
*   \brief   Gets the value of the current limit
*            
*   \param   p_data Pointer to board data
*   \param   ch_id The channel identifier
*   \param   value The pointer to the read value
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_mon_iset( V8100_data* p_data, UINT8 ch_id, float *value, CMD_ERROR_CODE *error_code);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_mon_imin( V8100_data* p_data, UINT8 ch_id, float *value, CMD_ERROR_CODE *error_code);
*   \brief   Gets the minimum value of current limit
*            
*   \param   p_data Pointer to board data
*   \param   ch_id The channel identifier
*   \param   value The pointer to the read value
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_mon_imin( V8100_data* p_data, UINT8 ch_id, float *value, CMD_ERROR_CODE *error_code);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_mon_imax( V8100_data* p_data, UINT8 ch_id, float *value, CMD_ERROR_CODE *error_code);
*   \brief   Gets the maximum value of current limit
*            
*   \param   p_data Pointer to board data
*   \param   ch_id The channel identifier
*   \param   value The pointer to the read value
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_mon_imax( V8100_data* p_data, UINT8 ch_id, float *value, CMD_ERROR_CODE *error_code);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_mon_ires( V8100_data* p_data, UINT8 ch_id, float *value, CMD_ERROR_CODE *error_code);
*   \brief   Gets the resolution of current limit
*            
*   \param   p_data Pointer to board data
*   \param   ch_id The channel identifier
*   \param   value The pointer to the read value
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_mon_ires( V8100_data* p_data, UINT8 ch_id, float *value, CMD_ERROR_CODE *error_code);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_mon_imon( V8100_data* p_data, UINT8 ch_id, float *value, CMD_ERROR_CODE *error_code);
*   \brief   Gets the value of the current Monitor
*            
*   \param   p_data Pointer to board data
*   \param   ch_id The channel identifier
*   \param   value The pointer to the read value
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_mon_imon( V8100_data* p_data, UINT8 ch_id, float *value, CMD_ERROR_CODE *error_code);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_mon_stat( V8100_data* p_data, UINT8 ch_id, V8100_channel_status *value, CMD_ERROR_CODE *error_code);
*   \brief   Gets the value of the channel status
*            
*   \param   p_data Pointer to board data
*   \param   ch_id The channel identifier
*   \param   value The pointer to the read value ( \ref V8100_channel_status )
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_mon_stat( V8100_data* p_data, UINT8 ch_id, V8100_channel_status *value, CMD_ERROR_CODE *error_code);

//
// Channel set methods
// 

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_set_vset( V8100_data* p_data, UINT8 ch_id, float value, CMD_ERROR_CODE *error_code);
*   \brief   Sets the value of voltage
*            
*   \param   p_data Pointer to board data
*   \param   ch_id The channel identifier
*   \param   value The value to set
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_set_vset( V8100_data* p_data, UINT8 ch_id, float value, CMD_ERROR_CODE *error_code);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_set_iset( V8100_data* p_data, UINT8 ch_id, float value, CMD_ERROR_CODE *error_code);
*   \brief   Sets the value of current limit
*            
*   \param   p_data Pointer to board data
*   \param   ch_id The channel identifier
*   \param   value The value to set
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_set_iset( V8100_data* p_data, UINT8 ch_id, float value, CMD_ERROR_CODE *error_code);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_set_ovp( V8100_data* p_data, UINT8 ch_id, UINT8 value, CMD_ERROR_CODE *error_code);
*   \brief   Sets the value of Value OVP (as % di Vset)
*            
*   \param   p_data Pointer to board data
*   \param   ch_id The channel identifier
*   \param   value The value to set
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_set_ovp( V8100_data* p_data, UINT8 ch_id, UINT8 value, CMD_ERROR_CODE *error_code);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_set_uvp( V8100_data* p_data, UINT8 ch_id, UINT8 value, CMD_ERROR_CODE *error_code);
*   \brief   Sets the value of Value UVP (as % di Vset)
*            
*   \param   p_data Pointer to board data
*   \param   ch_id The channel identifier
*   \param   value The value to set
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_set_uvp( V8100_data* p_data, UINT8 ch_id, UINT8 value, CMD_ERROR_CODE *error_code);

//
// Crate monitor methods
// 

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_mon_crname( V8100_data* p_data, char *value, int value_size, CMD_ERROR_CODE *error_code);
*   \brief   Gets the crate name
*            
*   \param   p_data Pointer to board data
*   \param   value The read value buffer (caller allocated)
*   \param   value_size The value buffer size (BYTEs)
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_mon_crname( V8100_data* p_data, char *value, int value_size, CMD_ERROR_CODE *error_code);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_mon_numch( V8100_data* p_data, UINT8 *value, CMD_ERROR_CODE *error_code);
*   \brief   Gets the value of the voltage Set
*            
*   \param   p_data Pointer to board data
*   \param   value The pointer to the read value
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_mon_numch( V8100_data* p_data, UINT8 *value, CMD_ERROR_CODE *error_code);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_mon_psfrel( V8100_data* p_data, char *value, int value_size, CMD_ERROR_CODE *error_code);
*   \brief   Gets the PS Release Firmware
*            
*   \param   p_data Pointer to board data
*   \param   value The read value buffer (caller allocated)
*   \param   value_size The value buffer size (BYTEs)
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_mon_psfrel( V8100_data* p_data, char *value, int value_size, CMD_ERROR_CODE *error_code);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_mon_pstemp( V8100_data* p_data, float *value, CMD_ERROR_CODE *error_code);
*   \brief   Gets the value of the PS Temperature
*            
*   \param   p_data Pointer to board data
*   \param   value The pointer to the read value
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_mon_pstemp( V8100_data* p_data, float *value, CMD_ERROR_CODE *error_code);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_mon_pssnum( V8100_data* p_data, UINT32 *value, CMD_ERROR_CODE *error_code);
*   \brief   Gets the value of the PS Serial Number
*            
*   \param   p_data Pointer to board data
*   \param   value The pointer to the read value
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_mon_pssnum( V8100_data* p_data, UINT32 *value, CMD_ERROR_CODE *error_code);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_mon_fansp( V8100_data* p_data, V8100_FAN_SPEED_SET *value, CMD_ERROR_CODE *error_code);
*   \brief   Gets the Set value of FAN SPEED \ref _V8100_FAN_SPEED_SET
*            
*   \param   p_data Pointer to board data
*   \param   value The pointer to the read value
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_mon_fansp( V8100_data* p_data, V8100_FAN_SPEED_SET *value, CMD_ERROR_CODE *error_code);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_mon_fan1( V8100_data* p_data, UINT32 *value, CMD_ERROR_CODE *error_code);
*   \brief   Gets the value of the Fan Speed 1 (RPM)
*            
*   \param   p_data Pointer to board data
*   \param   value The pointer to the read value
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_mon_fan1( V8100_data* p_data, UINT32 *value, CMD_ERROR_CODE *error_code);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_mon_fan2( V8100_data* p_data, UINT32 *value, CMD_ERROR_CODE *error_code);
*   \brief   Gets the value of the Fan Speed 2 (RPM)
*            
*   \param   p_data Pointer to board data
*   \param   value The pointer to the read value
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_mon_fan2( V8100_data* p_data, UINT32 *value, CMD_ERROR_CODE *error_code);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_mon_fan3( V8100_data* p_data, UINT32 *value, CMD_ERROR_CODE *error_code);
*   \brief   Gets the value of the Fan Speed 3 (RPM)
*            
*   \param   p_data Pointer to board data
*   \param   value The pointer to the read value
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_mon_fan3( V8100_data* p_data, UINT32 *value, CMD_ERROR_CODE *error_code);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_mon_fufrel( V8100_data* p_data, char *value, int value_size, CMD_ERROR_CODE *error_code);
*   \brief   Gets the FAN UNIT Release Firmware
*            
*   \param   p_data Pointer to board data
*   \param   value The read value buffer (caller allocated)
*   \param   value_size The value buffer size (BYTEs)
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_mon_fufrel( V8100_data* p_data, char *value, int value_size, CMD_ERROR_CODE *error_code);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_mon_futemp( V8100_data* p_data, float *value, CMD_ERROR_CODE *error_code);
*   \brief   Gets the value of the FAN UNIT Temperature
*            
*   \param   p_data Pointer to board data
*   \param   value The pointer to the read value
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_mon_futemp( V8100_data* p_data, float *value, CMD_ERROR_CODE *error_code);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_mon_fusnum( V8100_data* p_data, UINT32 *value, CMD_ERROR_CODE *error_code);
*   \brief   Gets the value of the FAN UNIT Serial Number
*            
*   \param   p_data Pointer to board data
*   \param   value The pointer to the read value
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_mon_fusnum( V8100_data* p_data, UINT32 *value, CMD_ERROR_CODE *error_code);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_mon_crst( V8100_data* p_data, V8100_crate_status *value, CMD_ERROR_CODE *error_code);
*   \brief   Gets the value of the crate status
*            
*   \param   p_data Pointer to board data
*   \param   value The pointer to the read value ( \ref V8100_crate_status )
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_mon_crst( V8100_data* p_data, V8100_crate_status *value, CMD_ERROR_CODE *error_code);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_mon_vpmax( V8100_data* p_data, float *value, CMD_ERROR_CODE *error_code);
*   \brief   Gets the value maximum of OVP/UVP
*            
*   \param   p_data Pointer to board data
*   \param   value The pointer to the read value
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_mon_vpmax( V8100_data* p_data, float *value, CMD_ERROR_CODE *error_code);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_mon_vpmin( V8100_data* p_data, float *value, CMD_ERROR_CODE *error_code);
*   \brief   Gets the value minimum of OVP/UVP
*            
*   \param   p_data Pointer to board data
*   \param   value The pointer to the read value
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_mon_vpmin( V8100_data* p_data, float *value, CMD_ERROR_CODE *error_code);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_mon_rs232br( V8100_data* p_data, V8100_RS232_BITRATE *value, CMD_ERROR_CODE *error_code);
*   \brief   Gets the value of Bit Rate RS232 : \ref _V8100_RS232_BITRATE
*            
*   \param   p_data Pointer to board data
*   \param   value The pointer to the read value
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_mon_rs232br( V8100_data* p_data, V8100_RS232_BITRATE *value, CMD_ERROR_CODE *error_code);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_mon_canbr( V8100_data* p_data, V8100_CANBUS_BITRATE *value, CMD_ERROR_CODE *error_code);
*   \brief   Gets the value of Bit Rate CANBUS : \ref _V8100_CANBUS_BITRATE
*            
*   \param   p_data Pointer to board data
*   \param   value The pointer to the read value
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_mon_canbr( V8100_data* p_data, V8100_CANBUS_BITRATE *value, CMD_ERROR_CODE *error_code);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_mon_canadd( V8100_data* p_data, UINT8 *value, CMD_ERROR_CODE *error_code);
*   \brief   Gets the Address CANBUS : 0..255
*            
*   \param   p_data Pointer to board data
*   \param   value The pointer to the read value
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_mon_canadd( V8100_data* p_data, UINT8 *value, CMD_ERROR_CODE *error_code);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_mon_ipadd( V8100_data* p_data, char *value, int value_size, CMD_ERROR_CODE *error_code);
*   \brief   Gets the Address IP : xxx.xxx.xxx.xxx
*            
*   \param   p_data Pointer to board data
*   \param   value The read value buffer (caller allocated)
*   \param   value_size The value buffer size (BYTEs)
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_mon_ipadd( V8100_data* p_data, char *value, int value_size, CMD_ERROR_CODE *error_code);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_mon_ipmsk( V8100_data* p_data, char *value, int value_size, CMD_ERROR_CODE *error_code);
*   \brief   Gets the IP Netmask : xxx.xxx.xxx.xxx
*            
*   \param   p_data Pointer to board data
*   \param   value The read value buffer (caller allocated)
*   \param   value_size The value buffer size (BYTEs)
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_mon_ipmsk( V8100_data* p_data, char *value, int value_size, CMD_ERROR_CODE *error_code);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_mon_ipgtw( V8100_data* p_data, char *value, int value_size, CMD_ERROR_CODE *error_code);
*   \brief   Gets the Gateway : xxx.xxx.xxx.xxx
*            
*   \param   p_data Pointer to board data
*   \param   value The read value buffer (caller allocated)
*   \param   value_size The value buffer size (BYTEs)
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_mon_ipgtw( V8100_data* p_data, char *value, int value_size, CMD_ERROR_CODE *error_code);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_mon_macadd( V8100_data* p_data, char *value, int value_size, CMD_ERROR_CODE *error_code);
*   \brief   Gets the Mac Address : xx.xx.xx.xx.xx.xx
*            
*   \param   p_data Pointer to board data
*   \param   value The read value buffer (caller allocated)
*   \param   value_size The value buffer size (BYTEs)
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_mon_macadd( V8100_data* p_data, char *value, int value_size, CMD_ERROR_CODE *error_code);


//
// Crate set methods
// 

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_set_on( V8100_data* p_data, CMD_ERROR_CODE *error_code);
*   \brief   Switches the crate ON
*            
*   \param   p_data Pointer to board data
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_set_on( V8100_data* p_data, CMD_ERROR_CODE *error_code);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_set_off( V8100_data* p_data, CMD_ERROR_CODE *error_code);
*   \brief   Switches the crate OFF
*            
*   \param   p_data Pointer to board data
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_set_off( V8100_data* p_data, CMD_ERROR_CODE *error_code);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_set_sysr( V8100_data* p_data, CMD_ERROR_CODE *error_code);
*   \brief   Sends VME System Reset
*            
*   \param   p_data Pointer to board data
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_set_sysr( V8100_data* p_data, CMD_ERROR_CODE *error_code);

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_set_clr( V8100_data* p_data, CMD_ERROR_CODE *error_code);
*   \brief   Clear Alarm
*            
*   \param   p_data Pointer to board data
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_set_clr( V8100_data* p_data, CMD_ERROR_CODE *error_code);


////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn     BOOL V8100_set_fansp( V8100_data* p_data, V8100_FAN_SPEED_SET value, CMD_ERROR_CODE *error_code);
*   \brief   Set Fan Speed \ref _V8100_FAN_SPEED_SET
*            
*   \param   p_data Pointer to board data
*   \param   value The value to set
*   \param   error_code The command result code (may be null)
*   \return  TRUE: operation successfully completed
*/
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL V8100_set_fansp( V8100_data* p_data, V8100_FAN_SPEED_SET value, CMD_ERROR_CODE *error_code);

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
/*! \fn      const char* V8100_SW_rev( void);
*   \brief   Returns the SDK software release
*            
*   \return  const char*: The SDK revision string
*/
////////////////////////////////////////////////////////////////////////////////////////////////
 const char* V8100_SW_rev( void);

#endif  // __CLT_V8100_DEF_H
