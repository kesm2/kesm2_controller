#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "aerotype.h"

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

//#ifdef __BORLANDC__
//   #define  ALLMEM(arg1,arg2) farmalloc(arg1*arg2)
//#else

   #ifdef WIN32
      #define  ALLMEM(arg1,arg2) malloc(arg1*arg2)
      #define  FREEMEM(arg1)     free(arg1)
   #else
      #ifdef UNIX
      #define  ALLMEM(arg1,arg2) malloc(arg1*arg2)
      #define  FREEMEM(arg1)     free(arg1)
	  #else
      #define  ALLMEM(arg1,arg2) halloc(arg1,arg2)
      #define  FREEMEM(arg1)     hfree(arg1)
	  #endif 
      #define NORMALIZE(p) if(1) { \
	  FP_SEGMENT(p) += FP_OFFSET(p) >> 4;  \
	  FP_OFFSET(p)  &= 0x000f; } else

   #endif
//#endif
// __BOLANDC__

#ifndef U11EMULATE
     #define AXIS1NAME 'X'    // U500 NAMES
     #define AXIS2NAME 'Y'
     #define AXIS3NAME 'Z'
     #define AXIS4NAME 'U'
#else
     #define AXIS1NAME 'X'    // U11 NAMES
     #define AXIS2NAME 'Y'
     #define AXIS3NAME 'U'
     #define AXIS4NAME 'V'
#endif

#define AXIS5NAME 'A'    // names for U500PCI axis 5-8
#define AXIS6NAME 'B'
#define AXIS7NAME 'C'
#define AXIS8NAME 'D'

// definitions for "USEBLOCKMOVE"

typedef struct _CIRCLEDATA      // circle data for BLOCKMOVE
{
     char      axis1;
     char      axis2;
     long      sang;
     long      dang;
     double    rad;           // circle radius ( int+frac )
     double    xc,yc;         // incremental center positions ( int+frac )
     int       full ;         // full circle flag
}  CIRCLEDATA;

// This structure is passed thru the "long gpbdata[]" buffer as an alternative
// to packing and unpacking data as "longs"
// This block can hold up to 2 moves, 1 circle, 2 circles, 1 linear, 1cir+1lin
//
// Note: Axis 0-3 represents XYZU ( AXIS1,2,3,4 ) not drive 0-3
//
typedef struct _BLOCKMOVE
{
     char      plane;              // current plane 
     short     flag;               // see "BLOCKMOVEFLAG_xxx" below 
     double    feed;               // user programmed feedrate      (prog steps/sec) 
     double    endfeed;            // ""     ""       end feedrate  (prog steps/sec) 
     double    ramptime;           // user programmed ramp time (ms) 
     double    maxaccel;           // user programmed max vector accel (prog steps/sec/sec) 
     char      trig_accel_plane;   // "SA" command 
     char      trig_decel_plane;   // "SD" command 
     double    norm;               // total vector length (prog steps) 
     double    feedtime;           // optional feedtime (sec) 
     long      dist[4];            // distance for each axis (prog steps)
//     double    dist[4];            // distance for each axis (prog steps)
     long      roundtime;          // optional roundtime

     char      type[2];       // move 1/2 types, none, linear, circle 
     char      axisflag[4];   // axis move type: BLOCKMOVE_LINEAR, or _CIRCLE, 0 none 
     CIRCLEDATA c[2];         // circle data ( up to 2 circles ) 

} BLOCKMOVE;

#define BLOCKMOVE_CW      0x08
#define BLOCKMOVE_CCW     0x09
#define BLOCKMOVE_LINEAR  0x07
#define BLOCKMOVE_CIRCLE  0x0a
#define BLOCKMOVE_INDEX   0x00

#define BLOCKMOVEFLAG_G8         0x01
#define BLOCKMOVEFLAG_TRIG_ACCEL 0x02
#define BLOCKMOVEFLAG_TRIG_DECEL 0x04
#define BLOCKMOVEFLAG_ROUND      0x08
#define BLOCKMOVEFLAG_SINERAMP   0x10

#define BLOCKMOVE_GPB     0x7fffff
#define BLOCKMOVE_LONGSIZE    ((sizeof(BLOCKMOVE)/4) + 1 )

#define U500PCIBUS       1    // for board->bustype 
#define U500ISABUS       0    // """" 

#define U500_OS_NT              0
#define U500_OS_95		1
#define U500_OS_98		2
#define U500_OS_UNIX            3      //unix, currently system V only

#define U500_AXIS_LENGTH        14    // max length of "long" axis name

//*****************************************************************************
// GPB codes to DSP

#define GPB_NORM       0x11
#define GPB_LINEAR     0x07
#define GPB_CWCIRCLE   0x08
#define GPB_CCWCIRCLE  0x09

#define GPB_RAMP       0x5a
#define GPB_TRIG_ACCEL 0x5b
#define GPB_TRIG_DECEL 0x5c
#define GPB_DPFEED     0x5d
#define GPB_ENDFEED    0x60

//******************************************************************************


/* new structures for parts rotation and cutter compensation added V4.02 */
typedef struct _GPBMOVE
{
      SHORT       type;             // u500 code 7,8,9 = g1,g2,g3 
      SHORT       plane;            // 0,1-4 
      SHORT       cmdsiz;           // command size - number of 24 bit words -2 
      SHORT       haxis;            // circle horizontal axis 
      SHORT       vaxis;            // ""     vertical   "" 
      long        dist[4];          // move distance 
      long        norm;             // normal vector of contour move 
      long        dang;             // angle change 0-$ffffff = 0,360 degrees 
      long        sang;             // start angle 
      long        radm;             // radius msw 
      long        radl;             //        lsw 
      double      feed;             //v5.04 
      double      ramptime;         //v5.04 
      long        trig_accel;       //v5.04 
      long        trig_decel;       //v5.04 
      double      endfeed;          // v505 
      SHORT       helix;            // flag for helix bit #1,0 - axes defined 
      SHORT       helixaxis1;       // helix axis 1 0-3 
      SHORT       helixaxis2;       // helix axis 2 0-3 

}GPBMOVETYPE;

typedef struct _PARTROTATE
{
      SHORT       flag;             // bit #0 - do rotation 
      SHORT       X,Y;              // H,V axis 0-3 
      double      angle;            // angle in degrees 
      double      oldx,oldy;        // accumulated position ( prog steps ) 
} PARTROTATE;

#define U500_NUM_AXES        4

#if !defined U11EMULATE
	#define U500PCI_NUM_AXES     8	// up to 8 axes for U500PCI ( for EN,DI,HOME, etc. only )   
#else
	#define U500PCI_NUM_AXES     4	
#endif

#define U500_NUM_PLANES       4
#define NUM_STATS        13
#define CHECK_STATUS2_LONG_WORDS_READ_FROM_DRIVER 40   // for structure defintion

#ifdef U511
#define MAX_NUM_BOARDS   (2)    // Maximum number of boards that can be installed 
#else
#define MAX_NUM_BOARDS   (4)    // Maximum number of boards that can be installed 
#endif

#if defined WIN32

typedef struct tagAERDRV_EVENT
{
   HANDLE   hDrvEvent;     // Handle from OpenVxDHandle(Win95) 
                           // IoCreateSynchronizationEvent(WinNT) 
   PVOID    pkEvent;       // Not Used (Win95) 
                           // PKEVENT (WinNT) 
   ULONG    dwOpenCount;   // Number of times event has been set 
} AERDRV_EVENT;
typedef AERDRV_EVENT *PAERDRV_EVENT;

typedef DWORD (STDCALL *PFN_USERNOTIFY) ( PVOID pvData );

typedef struct tagAER_EVENT_FUNCTION_ARGS
{
	PFN_USERNOTIFY	pfnCallBack;
	HANDLE			hDrvEvent[2];
	PVOID			pBoard;
	PVOID			pvData;
	HANDLE			hThread;
} AER_EVENT_FUNCTION_ARGS;
typedef AER_EVENT_FUNCTION_ARGS *PAER_EVENT_FUNCTION_ARGS;

#endif    // WIN32 

typedef struct BOARDDRVDATA_
{

     SHORT     type ;               // board type 0-base, 1-plus, 2-ultra 
     SHORT     maxplane ;           // maximum plane allowed by parameter file 
     SHORT     u500init;            // 1 = U500 has been initialized 
     SHORT     psoinit;             // 1 = PSO has been initialized 

     DOUBLE    engsf[U500_NUM_AXES];      // English mode, axis 1..4 scale factor 
     DOUBLE    metsf[U500_NUM_AXES];      // metric  mode, axis 1..4 scale factor 
     SHORT     axismap[U500_NUM_AXES];    // channel 0-3 bits #7-4 plane of axis 1-4 
					 //                  #3-0 axis bit name qlib AXIS1-AXIS4 
     #if defined WIN32
     AERDRV_EVENT  tEvent;
     #endif    // WIN32 

} BOARDDRVDATA;
typedef BOARDDRVDATA *PBOARDDRVDATA;

///***************************************************************************
// PSO ISA DPRAM format 
// 
//   0-0x3ff : data area ( 1024 bytes ) 
// 
//   0x400-0x47f : pointer area. each pointer consists of 4 bytes... 
// 
//        +0 - 1 == valid data 
//        +1 - X - not used 
//        +2 - msb number of bytes in data area for this transfer 
//        +3 - lsb  """" 
// 
//  0x480-0x7ff - spare 
// 
//   The PC writes array data to the PSO for PSOD1 PSOD2 PSOM0 PSOP3 commands. 
//***************************************************************************** 

typedef struct PSO_ISA_DPRAM_
{
     LONG      baseadd;          // base address of PSO dpram ( usually d800:0000 ) 
     int       writeidx;         // offset into 1024 byte data section of DPRAM 
     int       bytesused;        // bytes used in dpram data area 
     int       nextpntr;         // next available write pointer in 0x400 pointer section 
     int       lastpntr;         // last offset checked for cleanup 0x400 pointer area 

} PSO_ISA_DPRAM;

typedef struct _ENVCOMP
{
	SHORT		enable;
	SHORT		axismask;
	SHORT		analog_input_mask;		
	SHORT		cal_file_loaded;
	DOUBLE	    temp1_scale,
				temp1_offset,
				press_scale,
				press_offset,
				humid_scale,
				humid_offset,
				temp2_scale,
				temp2_offset,
				set_temperature, 
				set_pressure, 
				set_humidity;
}ENVCOMP;


// used for displaying position when the scf is used 
typedef struct _NOSCALE
{
	DOUBLE    last_disp[U500_NUM_AXES];	// v5.12, displayed pos before scaling turned on 	 
	DOUBLE    last_fdbk[U500_NUM_AXES];	// v5.12, mach pos before scaling turned on 	 
	DOUBLE    eff_scf[U500_NUM_AXES];   // v5.12, overriding scale factor values 
}NOSCALE;


/***** Structure to hold data from fiber alignment routines ******/
typedef struct FIBER_RET_DATA_
	{
		short       done;				   // status flag 1=routine done, 0=thread running
		AERERR_CODE error;			   // returned error code
		double      adval;				// current A/D or GPIB value	
		double	   axis1_coord;		// used for geo_center / centroid alignment routine
		double	   axis2_coord;		// used for geo_center / centroid alignment routine
      double	   axis3_coord;		// used for centroid alignment routine
		long	      maxpnts;			   // maximum possible number of saved data points
		double	   *saved_data;		// pointer to saved coordinates & power reading data
		long	      saved_points;		// actual number of saved data points
		long	      datasave_on;		// switch to save data  1-save data, 0-off
		long	      ibstat;				// to report errors and data during GPIB reads
		long	      iberror;
		long	      GPIB_devNumber;	// -1 GPIB wasn't used, else this is the device number that was used
		long	      limit[4][2];		// limit coordinates -- for Fast Align routines
      short       useLimits[4][2];
		long	      start[4];			// start coordinates -- for Fast Align routines
		long        adinvert;			// variable to mult by -1 A/D reading.  For A/D readings that become
		                              // more negative with increasing power
	} FIBER_RET_DATA;
typedef FIBER_RET_DATA *PFIBER_RET_DATA; 

// Structure used for fiber alignment
typedef struct HILLCLIMB_ARG_LIST_
	{
		long	scan_inc;		// dist between successive readings (prog steps)
		long	maxdisp;		// max allowed displacement in either search direction (prog steps)
		double	threshold;		// lowest permitted power reading (volts)
		long	axis;			// search axis (8-11)
		long	adchannel;		// sampled A/D channel (0-7) or GPIB Device 0 - 3
		long	nDelay;			// delay between successive power readings (milliseconds)
		long    whole_window;   // search entire window distance (maxdisp) regardless if peak is found
		long	inputmode;		// 0 = A/D, 1 = GPIB
	} HILLCLIMB_ARG_LIST; 

// Structure used for fiber alignment
typedef	struct SPIRALFINE_ARG_LIST_
	{
		long radius;			// end radius (prog steps)
		long numspirals;		// number of spirals within the search radius
		long segment_length;	// linear distance between readings (prog steps)
		long axis1;				// first axis of the search plane (8-11)
		long axis2;				// second axis of the search plane (8-11)
		long adchannel;			// sampled A/D channel (0-7)
		long nDelay;			// delay between successive power readings (milliseconds)
		long scan_mode;			// Normal - segmented motion / Continuous - special scan
		long inputmode;			// 0 = A/D, 1 = GPIB
	} SPIRALFINE_ARG_LIST;

// Structure used for fiber alignment
typedef	struct SPIRALROUGH_ARG_LIST_
	{
		long maxradius;			// max allowed search radius (prog steps)
		long  numspirals;		// number of spirals within the search radius
		long segment_length;	// linear distance between readings (prog steps)
		double threshold;		// A/D value where the search will terminate (volts)
		long axis1;				// first axis of the search plane (8-11)
		long axis2;				// second axis of the search plane (8-11)
		long adchannel;			// sampled A/D channel (0-7)
		long nDelay;			// delay between successive power readings (milliseconds)
		long scan_mode;			// Normal - segmented motion / Continuous - special scan
		long inputmode;			// 0 = A/D, 1 = GPIB
	} SPIRALROUGH_ARG_LIST;

// Structure used for fiber alignment
typedef	struct FASTALIGN_ARG_LIST_
	{
		long offset1;			// initial search distance along Axis #1 (prog steps)
		double ftol;			// termination tolerance (volts)
		long maxit;				// maximum search iterations
		long axis1;				// first axis of the search plane (8-11)
		long axis2;				// second axis of the search plane (8-11)
		long adchannel;			// sampled A/D channel (0-7)
		long nDelay;			// delay between successive power readings (milliseconds)
		long axis3;				// third, OPTIONAL axis for 3D fastalign (8-11)
		long offset2;			// initial search distance along Axis #2 (prog steps)
		long offset3;			// initial search distance along Axis #3 (prog steps)
		long axis4;				// fourth, OPTIONAL axis for 4D fastalign (8-11)
		long offset4;			// initial search distance along Axis #4 (prog steps)
		double sat_val;			// top voltage threshold - causes immediate thread termination (volts)
		long inputmode;			// 0 = A/D, 1 = GPIB
		long returnToStart;
		long limit[4][2];
	} FASTALIGN_ARG_LIST;

// Structure used for fiber alignment
typedef	struct GEOCENTER_ARG_LIST_
	{
		long square_size;		// size of the search square (prog steps)
		long scan_inc;			// raster horizontal & vertical scanning increment (prog steps)
		long scan_lines;		// horizontal & vertical scan lines (same for both)
		double edge_val;		// specified edge drop off value (volts)
		long axis1;				// first axis of the search plane (8-11)
		long axis2;				// second axis of the search plane (8-11)
		long adchannel;			// sampled A/D channel (0-7)
		long nDelay;			// delay between successive power readings (milliseconds)
		long inputmode;			// 0 = A/D, 1 = GPIB
		long sr_mode;			// 1 = Single Raster Mode. Scan_lines parameter not used.  
								// One raster scan is performed with axis1 as the scan axis and axis 2 as the step axis
								// The A/D is read at every scan_inc over the square_size.
								// The number of A/D readings will be made equal in the axis1 and axis2 direction and
								// the readings from the raster scan are used to find the x and y center cooordinates
								// 0 = raster scans performed in both the axis1 and axis2 directions using the scan_lines
								// parameter
	} GEOCENTER_ARG_LIST;

// Structure used for fiber alignment
typedef	struct CENTROID_ARG_LIST_
	{
		long max_x_dist;		// +/- search distance in axis1 direction (prog steps)
		long max_y_dist;		// +/- search distance in axis2 direction (prog steps)
      long max_z_dist;		// +/- search distance in axis3 direction (prog steps)
		long scan_inc;			// raster horizontal & vertical scanning increment (prog steps)
		double edge_val;		// specified edge drop off value (volts)
		long axis1;				// first axis of the search plane (8-11)
		long axis2;				// second axis of the search plane (8-11)
      long axis3;				// third axis of the search plane (8-11)
		long adchannel;		// sampled A/D channel (0-7)
		long nDelay;			// delay between successive power readings (milliseconds)
		long inputmode;		// 0 = A/D, 1 = GPIB
		long retcenter;		// 0 = stay at end of scan, 1 = go to computed center
	} CENTROID_ARG_LIST;

typedef struct BOARD_
{
     USHORT    brdadd;              // base address ( = 0x300 default ) 
     USHORT    psoadd;              // PSO base address 

     PSO_ISA_DPRAM  psodpram;      // 

     HANDLE    hDevice;             // handle to device driver 
     ULONG     timeout;             // number of retries during bus communications 
     SHORT     active;              // active axes bit = AXIS1 - AXIS4, software command 
     SHORT     plane ;              // plane to send command 1-4 
     SHORT     enabled;             // actual axis enable flags 
     SHORT     nOpenCount;          // reference counter 
     SHORT     spline;              // in spline mode 

     SHORT     halt;                // v4.15, halt flags per plane 
     SHORT     moveCW;              // v4.15, 1 if param x11 + move CW = no, per axis 

     SHORT     absolute;             // 0/1 = incremental/absolute 
     DOUBLE    cmdpos[U500_NUM_AXES];     // software commanded position w.r.t. hardware home 
     DOUBLE    machpos[U500_NUM_AXES];    // realtime feedback position 
	 DOUBLE    rawmachpos[U500_NUM_AXES];    // realtime feedback position - no scaling or g92
     DOUBLE    refpos[U500_NUM_AXES];     // realtime servo command position 
     DOUBLE    g92offset[U500_NUM_AXES];  // offset from cmdpos to relpos 
     DOUBLE    modprm[U500_NUM_AXES];     // axis modulo rollover in mach steps 
     SHORT     metric[U500_NUM_PLANES] ;  // 1 = metric mode 

     DOUBLE    scf[U500_NUM_AXES];        // v4.16, overriding scale factor values 
     SHORT     scfflag;              // v4.16, bit 0-3 - axis 1-4 override SCF enabled 
     DOUBLE    scalemove[U500_NUM_AXES];  // v4.16, distance moved following scaling 
     DOUBLE    noscalemove[U500_NUM_AXES];// v4.16, distance "moved" pre-scaling 

// g8 mode - new contour info 
     SHORT     new_cntr[U500_NUM_PLANES];         // 1 = new contouring mode 
     SHORT     g8mode[U500_NUM_PLANES];           // VELOCITY_ON / VELOCITY_OFF 
     DOUBLE    g8ramptime[U500_NUM_PLANES];       // ramp time im ms 
     DOUBLE    g8feed[U500_NUM_PLANES];           // last feedrate pr st / se 
     SHORT     g8ramptype[U500_NUM_PLANES];       // TRAJ_SINE / TRAJ_LINEAR 
     SHORT     g8active[U500_NUM_PLANES];         // g8 mode active 
     DOUBLE    g8lastfeed[U500_NUM_PLANES];       // last g8 feedrate 
     SHORT     plcmode ;                     // 1 = plc mode active 

     ULONG     stats[NUM_STATS];     // updated by check status 

     BOARDDRVDATA   drv;             // static data kept by device driver 
     PARTROTATE     rotate;          // parts rotation buffer 
     DOUBLE    feedtime[U500_NUM_PLANES];      // "feedtime" specified for newcntr (sec) 
     SHORT     roundflag[U500_NUM_PLANES];    // corner rounding flag 
     DOUBLE    roundtime[U500_NUM_PLANES];     // corner rounding time (ms) 
     DOUBLE    maxaccel[U500_NUM_PLANES];      // max acceleration rate 0 = off prog steps / sec**2 
     SHORT     g8old[U500_NUM_PLANES];         // g8 mode after sendgpb 
     LONG      adjpos[U500_NUM_AXES];          // add to command position to get actual command position 
					  // for corner rounding mode 
     DOUBLE    indexaccel[U500_NUM_AXES];       // accel for G0 [mach st/msec**2] 
     DOUBLE    indexfeed[U500_NUM_AXES];        // speed for G0 [prog st/msec] 
     DOUBLE    cm1cmdpos[U500_NUM_AXES];        // software commanded position w.r.t. hardware home 

     SHORT     bustype;                     // 0=ISA, 1=PCI ( U500ISABUS/U500PCIBUS) 
     SHORT     ostype;                      // 0=NT, 1=95 or 98 

// for PCI card only 
     SHORT     fpga1type;                    // fpga1 configuration 
     SHORT     fpga2type;                    // fpga2 configuration 
     SHORT     optioncode;                   // factory enabled option codes 
     SHORT     onboardpso;                   // indicates U500PCI on board PSO hardware 

// for PSOT,4/6 mode - must be sent with PSOF,3 command since axes are unknown 
     SHORT     psot4flag;                    // send PSOT,4 with PSOF,3 
     SHORT     psot6flag;                    // send PSOT,6 with PSOF,3 
     SHORT     dacnum;                       // 0/1 
     DOUBLE    vmin;                         // min out voltage 
     DOUBLE    vmax;                         // max out voltage 
     LONG      maxvelpos;                    // max speed o r position 
   #if defined WIN32
     PAER_EVENT_FUNCTION_ARGS       ptEventInfo;    //event/thread info for interrupt 
	 HANDLE							event_handle;
   #endif      // WIN32 
	 ENVCOMP   ec;
	 NOSCALE   ns;

   #if defined WIN32
	  HANDLE hfaultthread;					// handle for faultack thread 
	  HANDLE hfault_event;					// handle for faultack thread close event 
	  SHORT waiting_for_fault_to_clear;     // thread completion variable

 // for Fiber Alignment routines
	  HANDLE fiberthread;					// handle for fiber align thread
	  SHORT fiberabort;						// flag to keep aerq_abort from terminating fiber threads
     LONG  fiberSleepMode;
     LONG  fiberSleepTime;          

	  FIBER_RET_DATA fiberstat;				// status/return data for fiber align
	  HILLCLIMB_ARG_LIST hillclimbargs;		// argument list for HillClimb
	  SPIRALFINE_ARG_LIST spiralfineargs;	// argument list for SpiralFine
	  SPIRALROUGH_ARG_LIST spiralroughargs;	// argument list for SpiralRough
	  FASTALIGN_ARG_LIST fastalignargs;		// argument list for FastAlign
	  GEOCENTER_ARG_LIST geocenterargs;		// argument list for GeoCenter
	  CENTROID_ARG_LIST centroidargs;		// argument list for Centroid
	  //GPIB_ARG_LIST gpibargs;                                     // GPIB parameter data
    #endif      // WIN32

// this data updated by "aerq_checkstatus2()"
     long   axisflags[8];          // axis 1-8 flags
     long   axisfaults[8];         // axis 1-8 faults ( same as fault mask )
     DOUBLE refpos2[4];            // realtime target position
     DOUBLE machpos2[4];           // feedback position ( not implemented !! )
     DOUBLE cmdpos2[4];            // accum commanded postion - handled by PC
	 long	demo;

    #ifdef U11CHECKAXISFAULT  // U11 Emulation Mode only - special build to clear limits when a move command issued
         SHORT limitSwitchNC[4];     // 1 = NC
    #endif
    

}BOARD;

typedef BOARD  *PBOARD;


typedef struct CENTROID_THREAD_DATA_
{
   LONG     nDim;
   PBOARD   bp; 
} CENTROID_THREAD_DATA;

// cutter comp data definitions 
#define CRCQUESIZ 256

typedef struct _CUTTERCOMP
{
      int         flag;                 // bit #0 - do comp 
      int         firstmove;            // first move flag 
      int         lastmove;             // last move flag ( crc done ) 
					//  1 do now, 2 wait for next move 
      int         quedump;              // que dump mode 
      double      rad;                  // tool radius 
      int         X,Y;                  // H,V axis 0-3 
      int         g41;                  // G41/G42 - 1/0 
      double      lastx,lasty;          // last X,Y corrected positions 
      long        que[CRCQUESIZ];       // dsp command buffer 
      int         qcmd;                 // number of commands in queue 
      int         qread;                // read index 
      int         qwrite;               // write pointer in queue 

} CRCBUFTYPE;


typedef struct _VECTOR
{
	 int    type;               // gpb move type 
	 int    flag;               // flags bit #0  1 = last crc move (CRCDONE) 
				    //           #1 
				    //           #2 
				    //           #3 
	 int    full  ;             // 1 = do full circle 
	 double x1,y1 ;             // start point of move 
	 double x2,y2 ;             // end point ... 
	 double x0,y0;              // circle center point x,y 

	 double sang,eang;
	 double rad;

       #if 1 // !defined USEBLOCKMOVE 
	 long   ramp[4];                // 0x5a , ramptime, 80h/rt, 80h/(rt+1) 
	 long   trig_accel[2];          // 0x5b , plane 
	 long   trig_decel[2];          // 0x5c , plane 
	 long   dpfeed[3] ;             // 0x5d , 24.24 "double precision" feedrate 
	 long   endfeed[3] ;            // 0x60 , 24.24 "double precision" feedrate 
       #endif

	 BLOCKMOVE bm;                  // origional block move data 

} VECTOR;

#ifndef PI
#define PI  3.141592653589793
#endif

#define HEXANGTORAD    (PI/0x800000)  // convert rad to hex (DSP) angle 
#define RADTOHEXANG    (0x800000/PI)  //         hex to rad 

#define HEXANGTORAD_CM1  (PI/0x400000)  // convert rad to hex (DSP) angle 
#define RADTOHEXANG_CM1  (0x400000/PI)  //         hex to rad 


typedef struct _CRND
{
     int    on;                         // 1 = on, 0 = off 
     int    xidx;                       // x axis index 0-3 
     int    yidx;                       // y 
     int    xmask;                      // x axis qlib code AXIS1-4 
     int    ymask;                      // y 
     int    clear;                      // remove corner round corrections 
     double rad;                        // radius of circle to add 
     double p0[U500_NUM_AXES];               // (x0,y0) - programmed "last" position 
     double p1[U500_NUM_AXES];               // (x1,y1) -            "new" 

} CRND;

#define CB curBoardNum

#define U500DAC1    1
#define U500DAC2    2
#define U500DAC3    3
#define U500DAC4    4
#define U500DAC5    5
#define U500DAC6    6
#define U500DAC7    7
#define U500DAC8    8
#define U500DAC9    9
#define U500DAC10   10

#endif         // __GLOBAL_H__ 
