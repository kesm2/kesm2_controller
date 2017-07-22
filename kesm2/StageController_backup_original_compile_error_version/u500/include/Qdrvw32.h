
#ifndef __QDRVW32_H__
#define __QDRVW32_H__

// ***MCG #include <AerqCode.h>
#include "global.h"


#ifdef UNIX
struct unixIoctl {
    void * InputBuffer;
    unsigned long InputLength;
    void * OutputBuffer;
    unsigned long OutputLength;
    unsigned long eRc;
};


/* this resides as static data upon instantiation of the driver 
 code during boot, will be initialized to zero if the driver stops 
 running somehow (if modularized) */ 

struct unixKernelIoctl {
    void * InputBuffer;
    unsigned long InputLength;
    void * OutputBuffer;
    unsigned long OutputLength;
    BOARDDRVDATA tDrvData;
    long lCmdPos[U500_NUM_AXES] ;     
};
#endif




// defined in quick.h also
#define U500BASE         0
#define U500PLUS         1
#define U500ULTRA        2

/******   NT driver Codes and Data Structures   *****/

#define AER_U500_DEVICE_TYPE     0xA500   // Between 0x8000-0xFFFF
#define AER_U600_DEVICE_TYPE     0xA600   // Between 0x8000-0xFFFF
#define AER_U631_DEVICE_TYPE     (AER_U600_DEVICE_TYPE)

#define U500PCI_DEVICE_TYPE     AER_U500_DEVICE_TYPE
#define U500ISA_DEVICE_TYPE     AER_U500_DEVICE_TYPE

#define AER_IOCTL_CODE           0x800    // Between 0x800-0xFFF 
                                          // Same for all controllers 

// CTL_CODE is from winioctl.h 
#undef CTL_CODE
#define CTL_CODE( DeviceType, Function, Method, Access ) (ULONG)(          \
    ((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method) \
)

#define AER_U500_CODE( ioctl )   \
   ((ULONG) (CTL_CODE( AER_U500_DEVICE_TYPE, AER_IOCTL_CODE+ioctl, 0, 0 )))

#define AER_IOCTL_SENDGPB                       AER_U500_CODE( 0x01 )
#define AER_IOCTL_REALTIME_ASK                  AER_U500_CODE( 0x02 )
#define AER_IOCTL_ABORT_MOTION                  AER_U500_CODE( 0x03 )
#define AER_IOCTL_CHECK_STATUS                  AER_U500_CODE( 0x04 )
#define AER_IOCTL_DOWNLOAD_FIRMWARE             AER_U500_CODE( 0x05 )
#define AER_IOCTL_DOWNLOAD_PARAMETERS           AER_U500_CODE( 0x06 )
#define AER_IOCTL_PAUSE                         AER_U500_CODE( 0x07 )
#define AER_IOCTL_ACKNOWLEDGE_FAULT             AER_U500_CODE( 0x08 )
#define AER_IOCTL_QUICK_STATUS                  AER_U500_CODE( 0x09 )
#define AER_IOCTL_MONITOR_INTERRUPTS            AER_U500_CODE( 0x0A )
#define AER_IOCTL_SET_EXTERNAL_INITIALIZATION   AER_U500_CODE( 0x0B )
#define AER_IOCTL_READ_EXTERNAL_INITIALIZATION  AER_U500_CODE( 0x0C )
#define AER_IOCTL_SET_EXTERNAL_HOMED            AER_U500_CODE( 0x0D )
#define AER_IOCTL_READ_EXTERNAL_HOMED           AER_U500_CODE( 0x0E )
#define AER_IOCTL_GET_DIAG                      AER_U500_CODE( 0x0F )
#define AER_IOCTL_UPDATE_CMDPOS                 AER_U500_CODE( 0x10 )
#define AER_IOCTL_AUX_DEBUG                     AER_U500_CODE( 0x11 )
#define AER_IOCTL_DOWNLOAD                      AER_U500_CODE( 0x12 ) // new download 
#define AER_IOCTL_DSPMEM_READ                   AER_U500_CODE( 0x13 )
#define AER_IOCTL_DSPMEM_WRITE                  AER_U500_CODE( 0x14 )
#define AER_IOCTL_GET_VERSION                   AER_U500_CODE( 0x15 )

#define AER_IOCTL_SCOPE                         AER_U500_CODE( 0x16 )
#define AER_IOCTL_SCOPE_DUMP                    AER_U500_CODE( 0x17 )

#define AER_IOCTL_DOWNLOAD_PSO                  AER_U500_CODE( 0x18 )
#define AER_IOCTL_GET_DRVDATA                   AER_U500_CODE( 0x19 )

#define AER_IOCTL_MXH_READSTAT                  AER_U500_CODE( 0x1b )
#define AER_IOCTL_MXH_ERASE_CHIP                AER_U500_CODE( 0x1c )
#define AER_IOCTL_MXH_READLOC                   AER_U500_CODE( 0x1d )
#define AER_IOCTL_MXH_WRITEBLOCK                AER_U500_CODE( 0x1e )
#define AER_IOCTL_MXH_RETURNDATA                AER_U500_CODE( 0x1f )

#define AER_IOCTL_SET_DRVDATA                   AER_U500_CODE( 0x20 )
#define AER_IOCTL_CMDVEC_WR_RD                  AER_U500_CODE( 0x21 )
#define AER_IOCTL_READ_IOPORT                   AER_U500_CODE( 0x22 )
#define AER_IOCTL_WRITE_PSO_DPRAM               AER_U500_CODE( 0x23 )
#define AER_IOCTL_GET_FIRMWARE_INFO             AER_U500_CODE( 0x24 )
#define AER_IOCTL_TRIGGER_PLANES                AER_U500_CODE( 0x25 )
#define AER_IOCTL_PSO_DSPMEM_READ               AER_U500_CODE( 0x26 )
#define AER_IOCTL_PSO_DSPMEM_WRITE              AER_U500_CODE( 0x27 )
#define AER_IOCTL_PSO_GET_DIAG                  AER_U500_CODE( 0x28 )
#define AER_IOCTL_PSO_REALTIME_COMMAND          AER_U500_CODE( 0x29 )
#define AER_IOCTL_ISBX_ADC_VEL                  AER_U500_CODE( 0x2a )
#define AER_IOCTL_SET_VEL_TIMEBASE              AER_U500_CODE( 0x2b )
#define AER_IOCTL_RESET_INTERRUPT               AER_U500_CODE( 0x2c )
#define AER_IOCTL_TIMEOUT                       AER_U500_CODE( 0x2d )
#define AER_IOCTL_BOOT_PCI                      AER_U500_CODE( 0x2e )
#define AER_IOCTL_HARDWARE_REV                  AER_U500_CODE( 0x2f )
#define AER_IOCTL_COMM_READY                    AER_U500_CODE( 0x30 )
#define AER_IOCTL_SLEW_CANCEL                   AER_U500_CODE( 0x31 )
#define AER_IOCTL_WAIT_COMM_READY               AER_U500_CODE( 0x32 )
#define AER_IOCTL_GET_DIAG2                     AER_U500_CODE( 0x33 )

#define AER_IOCTL_FLASH                         AER_U500_CODE( 0x34 )

#define AER_IOCTL_POSGRAB_SCAN_ENABLE           AER_U500_CODE( 0x35 )
#define AER_IOCTL_POSGRAB_DISABLE               AER_U500_CODE( 0x36 )
#define AER_IOCTL_POSGRAB_READINDEX             AER_U500_CODE( 0x37 )
#define AER_IOCTL_POSGRAB_READPOS               AER_U500_CODE( 0x38 )
#define AER_IOCTL_POSGRAB_RESET_INDEX           AER_U500_CODE( 0x39 )
#define AER_IOCTL_POSGRAB_READ_ARRAY_SIZE       AER_U500_CODE( 0x3a )
#define AER_IOCTL_POSGRAB_HARDWARE_ENABLE       AER_U500_CODE( 0x3b )
#define AER_IOCTL_GET_PCI_HEADER                AER_U500_CODE( 0x3c )
#define AER_IOCTL_SET_PCI_HEADER                AER_U500_CODE( 0x3d )

#define AER_IOCTL_EVENT_CREATE                  AER_U500_CODE( 0x3e )
#define AER_IOCTL_EVENT_CLOSE                   AER_U500_CODE( 0x3f )
#define AER_IOCTL_SIMULATE_INTERRUPT            AER_U500_CODE( 0x40 )
#define AER_IOCTL_WRITE_IOPORT                  AER_U500_CODE( 0x41 )
#define AER_IOCTL_SET_PAUSE                     AER_U500_CODE( 0x42 )
#define AER_IOCTL_PLANE_READY                   AER_U500_CODE( 0x43 )
#define AER_IOCTL_IMMED_CONTROL                 AER_U500_CODE( 0x44 )
#define AER_IOCTL_IMMED_READ                    AER_U500_CODE( 0x45 )
#define AER_IOCTL_CHECK_STATUS2                 AER_U500_CODE( 0x46 )
#define AER_IOCTL_BUS_TEST                      AER_U500_CODE( 0x47 )

#define AUX_BUFFER_SIZE 2048

#define STATUSWORDS     24 // 24 Hardware generated status words 
#define DSP_DIAG_WORDS  STATUSWORDS
#define NUM_DIAGWORDS   28 // 28 Words total 
#define NUM_FIRMWARE_INFO 10

 // U500PCI diag #2 
#define DIAG2_LONG_WORDS_READ_FROM_DSP    24
#define DIAG2_LONG_WORDS_READ_FROM_DRIVER 24

#define CHECK_STATUS2_LONG_WORDS_READ_FROM_DSP 32

#define AER_DSPMEM_X       0x010000L    // DSP memory access flags 
#define AER_DSPMEM_Y       0x020000L
#define AER_DSPMEM_P       0x040000L
#define AER_DSPMEM_L       0x080000L
#define AER_DSPMEM_READ    0x000000L    // DSP memory functions / commands 
#define AER_DSPMEM_WRITE   0x100000L
#define AER_DSPMEM_AND     0x200000L
#define AER_DSPMEM_OR      0x300000L
#define AER_DSPMEM_KILL    0x400000L    // command to stop host transmitter SHORT 

#ifndef U511
typedef struct tagAER_UPDATE_CMDPOS
{
   ULONG          axes;                         // Axis mask changed from SHORT 95 VxD 
   
   LARGE_INTEGER  lCmdPos[U500_NUM_AXES];     // values to add in 
   DOUBLE         dCmdPos[U500_NUM_AXES];    // changed from long to double for Version 5.19 -- DCS
   ULONG          dwReset;             // Add in or reset? 

} AER_UPDATE_CMDPOS;

typedef AER_UPDATE_CMDPOS  *PAER_UPDATE_CMDPOS;
#endif

typedef struct tagAER_DIAG_SEND_PACKET
{
   ULONG dwDebug;
} AER_DIAG_SEND_PACKET;
typedef AER_DIAG_SEND_PACKET *PAER_DIAG_SEND_PACKET;

typedef struct tagAER_DIAG_RECV_PACKET
{
   ULONG       dwStatus[NUM_DIAGWORDS];
} AER_DIAG_RECV_PACKET;
typedef AER_DIAG_RECV_PACKET *PAER_DIAG_RECV_PACKET;

typedef struct tagAER_FIRMWARE_INFO_PACKET
{
   LONG       lFirmInfo[NUM_FIRMWARE_INFO];
} AER_FIRMWARE_INFO_PACKET;
typedef AER_FIRMWARE_INFO_PACKET *PAER_FIRMWARE_INFO_PACKET;

typedef struct tagAER_MXH_PACKET
{
   ULONG       data;	// either addr or length of dataarray 
   SHORT		chip;	// chip 0 or 1 
   ULONG		startaddr;// start addr for programming flash 
   LONG		*pdataarray;  //array of data 
   LONG		*pdataret;	//array of data read back from flash 
} AER_MXH_PACKET;
typedef AER_MXH_PACKET *PAER_MXH_PACKET;

#define OPCODE_AUX_DEBUG_INIT 0
#define OPCODE_AUX_DEBUG_FREE 1
#define OPCODE_AUX_DEBUG_READ 2

typedef struct tagAER_AUX_DEBUG_SEND_PACKET
{
   ULONG dwOpCode;
} AER_AUX_DEBUG_SEND_PACKET;
typedef AER_AUX_DEBUG_SEND_PACKET   *PAER_AUX_DEBUG_SEND_PACKET;

typedef struct tagAER_AUX_DEBUG_RECV_PACKET
{
   AERERR_CODE eRc;
   char        szBuffer[AUX_BUFFER_SIZE];
} AER_AUX_DEBUG_RECV_PACKET;
typedef AER_AUX_DEBUG_RECV_PACKET   *PAER_AUX_DEBUG_RECV_PACKET;


typedef struct tagAER_DSPMEM_SEND_PACKET
{
   ULONG dwAddr;   // Address to write on DSP (16-bit) 
   ULONG dwMode;   // AER_DSPMEM_xxxx constants 
   ULONG dwData;   // Data to Write 
   ULONG dwData2;  // Data to Write 
} AER_DSPMEM_SEND_PACKET;
typedef AER_DSPMEM_SEND_PACKET   *PAER_DSPMEM_SEND_PACKET;

typedef struct tagAER_DSPMEM_RECV_PACKET
{
   ULONG dwLSW;  // return data (LSW) 
   ULONG dwMSW;  // return data (MSW) 
} AER_DSPMEM_RECV_PACKET;
typedef AER_DSPMEM_RECV_PACKET   *PAER_DSPMEM_RECV_PACKET;

typedef struct tagAER_PSO_REALTIME_COMMAND
{
   ULONG  gpawords ;     // number of (long) words to send as GPA ( in gpadata ) 
   ULONG  gpadata[20];
   ULONG  hostvector ;   // command vector to send after GPA data 
} AER_PSO_REALTIME_COMMAND;
typedef AER_PSO_REALTIME_COMMAND *PAER_PSO_REALTIME_COMMAND;

  #if 0
#define GROUP1_FBK            25
  #else      // 
#define GROUP1_FBK            27   // for "NEWAXISFLT" in firmware 
  #endif

#define CHECK_STATUS_WORDS    GROUP1_FBK
typedef struct tagAER_CHECKSTAT_RECV_PACKET
{
   ULONG dwFbk[CHECK_STATUS_WORDS];
} AER_CHECKSTAT_RECV_PACKET;
typedef AER_CHECKSTAT_RECV_PACKET   *PAER_CHECKSTAT_RECV_PACKET;

typedef struct tagAER_CMDVEC_SEND_PACKET
{
   SHORT CmdVec;           // host command vector 
   SHORT ReadFlag;         // read back flag 
   LONG  WriteData;        // data to write after host command 
} AER_CMDVEC_SEND_PACKET;
typedef AER_CMDVEC_SEND_PACKET   *PAER_CMDVEC_SEND_PACKET;


typedef struct tagAER_DOWNLOAD_SEND_PACKET
{
   PCHAR pFirmData;  // firmware data 
   ULONG dwFirmSize; // firmware size 
   PCHAR pParmData;  // parameter data 
   ULONG dwParmSize; // parameter size; 
} AER_DOWNLOAD_SEND_PACKET;
typedef AER_DOWNLOAD_SEND_PACKET *PAER_DOWNLOAD_SEND_PACKET;

typedef struct tagXER_DOWNLOAD_SEND_PACKET
{
   ULONG dwFirmSize; // firmware size 
   ULONG dwParmSize; // parameter size; 
#ifdef UNIX
   CHAR  *szFirmData ;
#else
   CHAR  szFirmData[];  // data ( actually variable size data, 
#endif			//   this section contains the firmware and parameters ) 
			//   this structure is really only used to access 
			//   the first two elements 
} XER_DOWNLOAD_SEND_PACKET;
typedef XER_DOWNLOAD_SEND_PACKET *PXER_DOWNLOAD_SEND_PACKET;

#define PSODIAGRETSIZE        12
typedef struct tagAER_PSO_DIAG_RECV_PACKET
{
   ULONG dwFbk[PSODIAGRETSIZE];
} AER_PSO_DIAG_RECV_PACKET;
typedef AER_PSO_DIAG_RECV_PACKET  *PAER_PSO_DIAG_RECV_PACKET;

typedef struct tagAER_DIAG2_RECV_PACKET
{
   ULONG       dwStatus[DIAG2_LONG_WORDS_READ_FROM_DRIVER];
} AER_DIAG2_RECV_PACKET;
typedef AER_DIAG2_RECV_PACKET *PAER_DIAG2_RECV_PACKET;

#define FLASH_DATA_SIZE  10
typedef struct tagAER_FLASH_SEND_PACKET
{
     LONG      operation;     // op code 
     LONG      datasize;      // # words to send to DSP 
     LONG      dspdata[FLASH_DATA_SIZE];     //actual data to go to dsp 
} AER_FLASH_SEND_PACKET;
typedef AER_FLASH_SEND_PACKET  *PAER_FLASH_SEND_PACKET;

typedef struct tagAER_POSGRAB_RECV_PACKET
{
     LONG      retcode;       // used for 1 word return data 
     LONG      msw[4];        // upper 24 bits of axis position 
     LONG      lsw[4];        // lower 32 bits of axis position 
} AER_POSGRAB_RECV_PACKET;
typedef AER_POSGRAB_RECV_PACKET  *PAER_POSGRAB_RECV_PACKET;

typedef struct tagAER_POSGRAB_SEND_PACKET
{
     LONG      cmd;           // used to send misc info to DSP 
} AER_POSGRAB_SEND_PACKET;
typedef AER_POSGRAB_SEND_PACKET  *PAER_POSGRAB_SEND_PACKET;

// structure for reading / writing the PCI configuration header info 
typedef struct tagAER_PCIHEADER_PACKET
{
     ULONG bus;
     ULONG slot;
     ULONG length;
     UCHAR buf[64];
} AER_PCIHEADER_PACKET;
typedef AER_PCIHEADER_PACKET  *PAER_PCIHEADER_PACKET;

#define MAX_TEXT_LEN 80

typedef struct tagAER_EVENT_CREATE_PACKET
{
    HANDLE   hDrvEvent;                 // Win95 - OpenVxDHandle() 
                                       // WinNT - Win32 Create Event 
   	DWORD dwEvent;
	DWORD dwNum;

} AER_EVENT_CREATE_PACKET;
typedef AER_EVENT_CREATE_PACKET *PAER_EVENT_CREATE_PACKET;

typedef struct tagAER_CHECK_STATUS2_RECV_PACKET
{
   LONG  axisfaults[8];        // axis 1-8 faults - same as fault mask
   LONG  axisflags[8];         // internal axis status
   LONG  refpos2msw[4];        // realtime time command position msw
   LONG  refpos2lsw[4];        //                                lsw
   LONG  machpos2msw[4];       // encoder feedback position msw ( not implemented ! )
   LONG  machpos2lsw[4];       //                           lsw

} AER_CHECK_STATUS2_RECV_PACKET;
typedef AER_CHECK_STATUS2_RECV_PACKET *PAER_CHECK_STATUS2_RECV_PACKET;

#endif
// __QDRVW32_H__ 
