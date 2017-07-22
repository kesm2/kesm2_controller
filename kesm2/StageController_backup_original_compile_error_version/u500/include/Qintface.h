/******************************************************************************

		  AEROTECH quick library

		  Filename: "qintface.h"

******************************************************************************/
#include "qdrvw32.h"   //v4.16 

#ifndef __QINTFACE_H__
#define __QINTFACE_H__

#include "aerqver.h"
#define BYTE  char

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif



// plane memory area in the U500's DSP memory 
#define PDEF1  0x1228               // plane 1 start address in Y memory 

// this is read from DSP Y:1B08 - V500 - arw 
// #define PDEF1SZ 0x25             // plane size - was 0x21 

#define U500PRM_RAMPTIME   6        // offset to ramp time 
#define U500PRM_FLAGS      0X0B     // offset to misc flags 
#define U500PRM_CFEED      0X0C     // offset to contour feed 
#define U500PDEF_24_24_FEED 0x23     // offset to 24.24 feedrate ( 24. int ) 
				    // .24 is at +1 
#define U500PRM_ROUNDTIME 0x14      // offset to rounding time (ms) 

// 386EX parallel port defs 
#define P1LTC  0xf862              // output latch 
#define P1PIN  0xf860              // pin state 

#if !defined WIN32
   #define PAER_DIAG_RECV_PACKET   LONG *
   #define PAER_FIRMWARE_INFO_PACKET LONG *
#endif

#ifdef __cplusplus
extern "C" {
#endif

PBOARD      AER_LIBENTRY aerq_open(SHORT boardnum );
AERERR_CODE AER_LIBENTRY aerq_close( PBOARD pBoard );
AERERR_CODE AER_LIBENTRY aerq_free( PBOARD pBoard );
AERERR_CODE AER_LIBENTRY aerq_init(BOARD *pb, CHAR *frmfile, CHAR *parmfile );
AERERR_CODE AER_LIBENTRY aerq_abort( PBOARD pBoard );
AERERR_CODE AER_LIBENTRY aerq_init_pso( BOARD *pb, CHAR *frmfile );
AERERR_CODE AER_LIBENTRY aerq_pause(PBOARD pb);
AERERR_CODE AER_LIBENTRY aerq_set_pause( PBOARD pBoard, SHORT on_off);

#ifdef WIN32
   unsigned _stdcall WaitForFaultClear(void *bp);
#endif

AERERR_CODE AER_LIBENTRY aerq_faultack(PBOARD pb);
AERERR_CODE AER_LIBENTRY aerq_faultack_wait( PBOARD pBoard, SHORT waitflag );

//AERERR_CODE AER_LIBENTRY aerq_quickstat( PBOARD pb , PLONG status );

AERERR_CODE AER_LIBENTRY aerq_mem_write( PBOARD pBoard, ULONG dwAddr, ULONG dwMode, ULONG dwData );
AERERR_CODE AER_LIBENTRY aerq_mem_read( PBOARD pBoard, ULONG dwAddr, ULONG dwMode, ULONG *pdwVal, ULONG *pdwMSW );
AERERR_CODE AER_LIBENTRY aerq_pso_mem_write( PBOARD pBoard, ULONG dwAddr, ULONG dwMode, ULONG dwData );
AERERR_CODE AER_LIBENTRY aerq_pso_mem_read( PBOARD pBoard, ULONG dwAddr, ULONG dwMode, ULONG *pdwVal, ULONG *pdwMSW );
AERERR_CODE AER_LIBENTRY aerq_scope_command( PBOARD pBoard, ULONG dwCmd, PULONG pdwVal );
AERERR_CODE AER_LIBENTRY aerq_scope_dump( PBOARD pBoard, PULONG pdwBuffer, ULONG  dwBufferCount );

AERERR_CODE AER_LIBENTRY aerq_checkstatus( PBOARD pBoard );
AERERR_CODE AER_LIBENTRY aerq_get_diag( PBOARD pBoard, BOOL bDebug, PAER_DIAG_RECV_PACKET pRecv );
AERERR_CODE AER_LIBENTRY aerq_get_diag2( PBOARD pBoard, BOOL bDebug, PAER_DIAG2_RECV_PACKET pRecv );
AERERR_CODE AER_LIBENTRY aerq_pso_get_diag( PBOARD pBoard, PLONG pRecv );
AERERR_CODE AER_LIBENTRY aerq_cmdvec_wr_rd( BOARD *pb, SHORT cmdvec, SHORT read, LONG writedata, LONG *fbkdata );
AERERR_CODE AER_LIBENTRY aerq_read_ioport( PBOARD pBoard, LONG add, LONG *fbkdata );
AERERR_CODE AER_LIBENTRY aerq_write_ioport( PBOARD pBoard, SHORT offset, SHORT data );
int         AER_LIBENTRY aerq_comm_ready( PBOARD pBoard );
AERERR_CODE AER_LIBENTRY aerq_drv_get_firmware_info( PBOARD pBoard, BOOL bDebug, PAER_FIRMWARE_INFO_PACKET pRecv );
AERERR_CODE AER_LIBENTRY aerq_trigger( PBOARD pBoard , SHORT planes );

AERERR_CODE AER_LIBENTRY aerq_mxh_download_firmware (PBOARD pb, CHAR *frmfile );
AERERR_CODE AER_LIBENTRY aerq_mxh_read_status( PBOARD pBoard, LONG *status );
AERERR_CODE AER_LIBENTRY aerq_mxh_erase_chip( PBOARD pBoard, SHORT chip );
AERERR_CODE AER_LIBENTRY aerq_mxh_read_loc( PBOARD pBoard, LONG addr, SHORT chip, LONG *data );
AERERR_CODE AER_LIBENTRY aerq_mxh_write_block( PBOARD pBoard, LONG blksize, SHORT chip, ULONG startaddr, LONG *data );
AERERR_CODE AER_LIBENTRY aerq_mxh_return_data( PBOARD pBoard, LONG blksize, LONG *pdatain );

SHORT       AER_LIBENTRY aerq_loadPrmBufr (LPSTR filename);
AERERR_CODE AER_LIBENTRY aerq_software_init ( PBOARD pb );

AERERR_CODE _aerq_sendgpb(BOARD *pb, LONG *gpbdata);
AERERR_CODE _aerq_sendgpb_(BOARD *pb, LONG *gpbdata );

AERERR_CODE       _aerq_realtime_ask(PBOARD pb, LONG token);
void AER_LIBENTRY _aerq_translate_status( BOARD *pb, PLONG fbkarr);

void _aerq_updatecmdpos( BOARD *pb, SHORT axes, DOUBLE *dpdata, int updatecm1pos );
void _aerq_resetcmdpos( BOARD *pb, SHORT axes, DOUBLE *dpdata );

AERERR_CODE AER_LIBENTRY aerq_get_system_info( PBOARD pBoard );

#ifdef WIN32
   AERERR_CODE AER_LIBENTRY aerq_get_board_addr( PBOARD pBoard, PUSHORT puAddr );
   AERERR_CODE AER_LIBENTRY aerq_set_driver_debug( PBOARD pBoard, int enable );
   AERERR_CODE AER_LIBENTRY aerq_get_drv_version( PBOARD pBoard, short *maj, short *min );
   AERERR_CODE AER_LIBENTRY aerq_device_driver_msg( PBOARD pBoard , PCHAR msg );
#endif

/********** win32/tcor specific ************/
AERERR_CODE AER_LIBENTRY aerq_ReadExternalHomed( PBOARD pboard, BOOLEAN *pbHomed );
AERERR_CODE AER_LIBENTRY aerq_SetExternalHomed( PBOARD pboard, BOOLEAN bHomed );
AERERR_CODE AER_LIBENTRY aerq_ReadExternalInitialization( PBOARD pboard, BOOLEAN *pbInit );
AERERR_CODE AER_LIBENTRY aerq_SetExternalInitialization( PBOARD pboard, BOOLEAN bInit );
/********** to here ***********************/

AERERR_CODE AER_LIBENTRY aerq_write_pso_dpram( BOARD *bp, double *arraystart, int numpoints , int startidx, int arraysize );
AERERR_CODE AER_LIBENTRY aerq_pso_velocity_abort( PBOARD pBoard, double end_voltage, ULONG msec, long flags, long outmask, long outdata );

AERERR_CODE AER_LIBENTRY aerq_isbx_adc_vel( PBOARD pBoard, SHORT readflag, LONG writedata, LONG *fbkdata );
AERERR_CODE AER_LIBENTRY aerq_set_vel_timebase( PBOARD pBoard, SHORT readflag, LONG writedata, LONG *fbkdata );

char *aerq_last_gpbbuf( void );

AERERR_CODE AER_LIBENTRY aerq_reset_interrupt( BOARD *pb );
AERERR_CODE AER_LIBENTRY aerq_timeout( BOARD *pb );
AERERR_CODE aerq_boot_pci( PBOARD pBoard, char *filename );
AERERR_CODE AER_LIBENTRY aerq_word_status( PBOARD pBoard, SHORT readflag, LONG writedata, LONG *fbkdata );
AERERR_CODE AER_LIBENTRY aerq_slew_cancel( BOARD *pb );
AERERR_CODE AER_LIBENTRY aerq_wait_comm_ready( PBOARD pBoard );
AERERR_CODE AER_LIBENTRY aerq_flash( PBOARD pBoard , LONG *indata, LONG *outdata );

AERERR_CODE AER_LIBENTRY aerq_read_plane_feedrate(BOARD *pb, short plane, double *feedrate );

AERERR_CODE AER_LIBENTRY aerq_posgrab_disable( PBOARD pBoard );
AERERR_CODE AER_LIBENTRY aerq_posgrab_read_index( PBOARD pBoard, LONG *index );
AERERR_CODE AER_LIBENTRY aerq_posgrab_reset_index( PBOARD pBoard );
AERERR_CODE AER_LIBENTRY aerq_posgrab_read_array_size( PBOARD pBoard, LONG *size );
AERERR_CODE AER_LIBENTRY aerq_posgrab_read_position( PBOARD pBoard, LONG idx, double *pos );
AERERR_CODE AER_LIBENTRY aerq_posgrab_scan_enable( PBOARD pBoard );
AERERR_CODE AER_LIBENTRY aerq_posgrab_hardware_enable( PBOARD pBoard );
AERERR_CODE AER_LIBENTRY aerq_get_pci_header( PBOARD pBoard, PAER_PCIHEADER_PACKET pHeader);
AERERR_CODE AER_LIBENTRY aerq_set_pci_header( PBOARD pBoard, PAER_PCIHEADER_PACKET pHeader);

#ifdef WIN32
AERERR_CODE AER_LIBENTRY aerq_dlfirmware( PBOARD pBoard, PXER_DOWNLOAD_SEND_PACKET pXDown, DWORD dwSize, PLONG plBoardType );
#endif
AERERR_CODE AER_LIBENTRY aerq_get_hardware_rev( PBOARD pBoard, LONG *rev);

     #if defined WIN32
AERERR_CODE AER_LIBENTRY aerq_create_event( PBOARD pBoard, PHANDLE phEvent, BOOL bReclaim);
AERERR_CODE AER_LIBENTRY aerq_close_event( PBOARD pBoard, HANDLE hEvent );
AERERR_CODE AER_LIBENTRY aerq_create_thread_ex( PBOARD pBoard, PFN_USERNOTIFY pfnCallBack, PVOID pvData, BOOL bReclaim);
AERERR_CODE AER_LIBENTRY aerq_terminate_thread_ex( PBOARD pBoard );
VOID aerEventThreadProc( LPVOID *pvEventTest );
AERERR_CODE AER_LIBENTRY aerq_wait_for_interrupt( PBOARD pBoard, DWORD timeout);
	#endif

AERERR_CODE AER_LIBENTRY aerq_get_plane_ready( PBOARD pBoard, LONG *status);
AERERR_CODE AER_LIBENTRY aerq_immed_control( PBOARD pBoard, LONG opcode, LONG subcode1, LONG subcode2  );
AERERR_CODE AER_LIBENTRY aerq_immed_read( PBOARD pBoard, LONG opcode, LONG offset, LONG *ret, DOUBLE *dret );
AERERR_CODE AER_LIBENTRY aerq_checkstatus2( PBOARD pBoard );
AERERR_CODE AER_LIBENTRY aerq_bus_test( BOARD *bp, LONG *ret );

#ifdef __cplusplus
}
#endif

#endif
// __QINTFACE_H__ 
