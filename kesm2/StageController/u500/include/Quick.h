/******************************************************************************

		  AEROTECH quick library

		  Filename: "quick.h"


******************************************************************************/
#include "build.h"

#ifndef __QUICK_H__
#define __QUICK_H__

#ifdef __BORLANDC__
   //  #include <alloc.h>
   //  #include <mem.h>
#else
   #include <memory.h>
#endif

#include "aerotype.h"   // Windows types for those who do not know them
#include "aerqcode.h"   // Error Codes
#include "aerqver.h"    // Version Information
#include "boards.h"     // Basic board information
#include "global.h"

#ifdef UNIX
    #define max(x,y)	( (x) > (y) ? (x) : (y) )
    #define min(x,y)	( (x) < (y) ? (x) : (y) )
#endif



/******   Circular Move Type Flags  ******/
#define NOMOVE           0x00
#define MOVECW           0x01
#define MOVECCW          0x02
#define LINEAR_MOVE      0x03

/******   Indexaxes, Linear and Circular Move Feedrate Flags   ***/
#define CHG_FEEDTIME     0x10
#define CHG_FEEDRATE     0x20

#define UPDATECNTRFEED   0x01
#define UPDATECNTRTIME   0x02
#define UPDATECNTRENDFEED 0x04      // ramp down at end of move ( CM 1 only )

#define CONTOUR_TRIGGER_ACCEL 0x04
#define CONTOUR_TRIGGER_DECEL 0x08

#define UPDATEIDXFEED1   0x01
#define UPDATEIDXFEED2   0x02
#define UPDATEIDXFEED3   0x04
#define UPDATEIDXFEED4   0x08

/******   Trajectory Types   ******/
#define TRAJ_LINEAR      0x00
#define TRAJ_SINE        0x01

/******   MFO Types   ******/
#define UMFO_OFF         0x00
#define UMFO_ON          0x01

/******   Brake Types   ******/
#define BRAKE_OFF        0x00
#define BRAKE_ON         0x01

/******   Rounding Types   ******/
#define ROUNDING_OFF     0x00
#define ROUNDING_ON      0x01
#define ROUNDING_TIME    0x02

/******   Interpolation Types - CAM_WRITE   ******/
//#define LINEAR_INTERP    0x00
//#define CSPLINE_INTERP   0x01

/******   Output Types - CAM_WRITE   ******/
//#define NO_OUTPUT        0x00
//#define HEX_OUTPUT       0x01
//#define BIT_OUTPUT       0x02

/******   Synchronization Types - CAM_SYNC   ******/
//#define SYNC_OFF         0x00
//#define SYNC_CAM_MODE    0x01
//#define SYNC_RATIO_MODE  0x02

/******   Velocity Types    ******/
#define VELOCITY_OFF     0x00
#define VELOCITY_ON      0x01

//*****************************************************************************
// used in aerq_queue() function to control internal queue (plane) program flow
#define Q_AGAIN          0x00 // repeat entire plane from start
#define Q_CANCEL         0x01
#define Q_INPUT          0x02 // halt plane until specifed input condition is true
#define Q_POSITION       0x03 // wait for axis to be in position
#define Q_WAITPSO        0x04  // wait for PSO pulse to finish
#define Q_RESERVED1      0x05 // place holder - do not use this value !
#define Q_AXISFLAG       0x06 // wait for axis flags true
#define Q_OUTPUT         0x07 // wait for output condition to be true


/******   Gain Flags   ******/
#define END_FLAG         0x00
#define KPOS_FLAG        0x02
#define KI_FLAG          0x03
#define KP_FLAG          0x04
#define KF1_FLAG         0x05
#define KF2_FLAG         0x06
#define SCALE_FLAG       0x07
#define N1_FLAG          0x08
#define N2_FLAG          0x09
#define D1_FLAG          0x0a
#define D2_FLAG          0x0b
#define NOTCH_FLAG       0x0e
#define DEADBAND_FLAG    0x0c
#define CLAMP_FLAG       0x0d

/****** Freerun Flags ******/
#define FREERUNCCW  0x040000
#define FREERUNSTOP 0x010000
#define FREERUN1SHT 0x020000

/****** Freerun 2 Flags ******/
#define FREERUN2_VELOCITY_MODE   0
#define FREERUN2_DISTANCE_MODE   1

/******   Miscellaneous   ******/
#define MINPLANE         1
#define MAXPLANE         4
#define NUM_GAIN_PARMS   13
#define NUM_CAMTABLES    10
#define CAMPOINT_SZ      6
#define MAX_CAMPTS       5000

#define AXIS1            0x01
#define AXIS2            0x02
#define AXIS3            0x04
#define AXIS4            0x08
#define AXIS5            0x10
#define AXIS6            0x20
#define AXIS7            0x40
#define AXIS8            0x80
#define AXISALL          0xFF

#define TABRAMP          0x01
#define ASYNCINT         0x02
#define MAXFAULTMASK     6
#define DSP_SIZ          610

#define SPLINE_OFF       1
#define SPLINE_ON        2
#define SPLINE_LINEAR    3

#define U500BASE         0
#define U500PLUS         1
#define U500ULTRA        2


#define SCMDKILL  0x0L           // kill tracking
#define SCMDACTVEL  0x10000L     // feedback actual velocity
#define SCMDCMDVEL  0x20000L     // feedback command vel
#define SCMDAUX  0x30000L        // feedback aux buffer
#define SCMDSETTB  0x40000L      // set time base value (ms)
#define SCMDENABLE  0x50000L     // enable tracking
#define SCMDSTOP  0x60000L       // stop sampling
#define SCMDNUMSAMP  0x70000L    // set number of sample
#define SCMDSETAUX  0x80000L     // set aux data fetch case 0 - torque, 1 ADC, 2 - intergal
#define SCMDATVEL  0x90000L      // feedback auto tune vel
#define SCMDATTRQ  0xA0000L      // feedback auto tune torque

#define U500QLIB_ZA_TAB_CMD 1    // special for aerq_za_command() set table location
#define U500QLIB_ZA_VX_CMD  2    // ""                   set velocity tracking mode
#define U500QLIB_ZA_CX_CMD  3    // ""                   set current control mode


#define MAXBLOCK    ( ( 2048 - 128 ) / 3)  - 1
#define MAXPOINTS   40960L

#define DPRINT      0x01
#define DPRFIRST    0x02
#define DPRLAST     0x04

#define LSHIFT24       16777216     // 2^24

//#define CALC_VOLTS(v) ((v)/10*(0x7fffff))
#define U500_VOLTS_TO_DSP(v) ((v/10.0)*(0x7fffff))

#define METRIC_MODE     0
#define ENGLISH_MODE    1
#define ADC_TO_VOLTS    (10.0/8388607.0)

#define ABSOLUTE_MODE     0
#define INCREMENTAL_MODE  1



// CONSTANT definitions for DSPMemRead / DSPMemWrite functions
#define DSPMEMX        0x010000L    // DSP memory access flags
#define DSPMEMY        0x020000L
#define DSPMEMP        0x040000L
#define DSPMEML        0x080000L
#define DSPMEMREAD     0x000000L    // DSP memory functions / commands
#define DSPMEMWRITE    0x100000L
#define DSPMEMAND      0x200000L
#define DSPMEMOR       0x300000L
#define DSPMEMKILL     0x400000L    // command to stop host transmitter short

#define COMMRETRY      1000000L     // DSP communications - number of retries



// PLC mode constants
#define PLC_MAX_LENGTH 40
#define PLC_ALL        -1
#define PLC_ON         0x00
#define PLC_OFF        0x01
#define PLC_CLOSE      0x02
#define PLC_ENABLE     0x03
#define PLC_DISABLE    0x04
#define PLC_FREE       0x05
#define PLC_OPEN       0x06
#define PLC_IF         0x07
#define PLC_WHILE      0x08
#define PLC_ELSE       0x09
#define PLC_ENDIF      0x0a
#define PLC_ENDWHILE   0x0b
#define PLC_AND        0x0c
#define PLC_OR         0x0d
#define PLC_LPAREN     0x0e
#define PLC_RPAREN     0x0f
#define PLC_I          0x10
#define PLC_P          0x11
#define PLC_F          0x12
#define PLC_PO         0x13
#define PLC_ISBXBIT    0x14
#define PLC_ISBXBYTE   0x15

#define PLC_EQ         0
#define PLC_GT         1
#define PLC_LT         2
#define PLC_END        0x1b

#define TWOTOTHE24 16777216
#define TWOTOTHE23 8388608

//*****************************************************************************
// THESE ARE DSP CODES TO FOR THE 1 WORD STATUS INTERRUPT
// THESE ARE USED TO READ THE ADC CHANNELS, VELOCITY FEEDBACK
// ALSO TO ENABLE/DISABLE VELOCITY/ADC TRACKING
//
// "aerq_word_status()"
#define STATUS_DISABLE_ADC        0x080000
#define STATUS_ENABLE_ADC         0x080001

#define STATUS_READ_ADC1          0x080002
#define STATUS_READ_ADC2          0x080003
#define STATUS_READ_ADC3          0x080004
#define STATUS_READ_ADC4          0x080005

#define STATUS_SET_VEL_TIMEBASE   0x080006

#define STATUS_DISABLE_VEL1       0x080007
#define STATUS_DISABLE_VEL2       0x080008
#define STATUS_DISABLE_VEL3       0x080009
#define STATUS_DISABLE_VEL4       0x08000A

#define STATUS_ENABLE_VEL1        0x08000B
#define STATUS_ENABLE_VEL2        0x08000C
#define STATUS_ENABLE_VEL3        0x08000D
#define STATUS_ENABLE_VEL4        0x08000E

#define STATUS_READ_VEL1_MSW      0x08000F
#define STATUS_READ_VEL1_LSW      0x080010
#define STATUS_READ_VEL2_MSW      0x080011
#define STATUS_READ_VEL2_LSW      0x080012
#define STATUS_READ_VEL3_MSW      0x080013
#define STATUS_READ_VEL3_LSW      0x080014
#define STATUS_READ_VEL4_MSW      0x080015
#define STATUS_READ_VEL4_LSW      0x080016

#define STATUS_READ_8X3_LOW_BYTE  0x080017
#define STATUS_READ_8X3_MID_BYTE  0x080018
#define STATUS_READ_8X3_HIGH_BYTE 0x080019

#define STATUS_READ_FEED_MSW_PL1  0x08001A
#define STATUS_READ_FEED_LSW_PL1  0x08001B
#define STATUS_READ_FEED_MSW_PL2  0x08001C
#define STATUS_READ_FEED_LSW_PL2  0x08001D
#define STATUS_READ_FEED_MSW_PL3  0x08001E
#define STATUS_READ_FEED_LSW_PL3  0x08001F
#define STATUS_READ_FEED_MSW_PL4  0x080020
#define STATUS_READ_FEED_LSW_PL4  0x080021

#define STATUS_READ_ADC5          0x080022
#define STATUS_READ_ADC6          0x080023
#define STATUS_READ_ADC7          0x080024
#define STATUS_READ_ADC8          0x080025

#define STATUS_READ_PLANE1FLAGS   0x080026
#define STATUS_READ_PLANE2FLAGS   0x080027
#define STATUS_READ_PLANE3FLAGS   0x080028
#define STATUS_READ_PLANE4FLAGS   0x080029

#define MIN_DAC_CHANNEL            1
#define MAX_DAC_CHANNEL            10

//*****************************************************************************
// These are dsp codes to for the "aerq_immed_control()" function
// some commands take 1 or 2 sub arguments (+1/+2)
#define AERQ_IMCTL_DISABLE_AD                0  // stop A/D update
#define AERQ_IMCTL_ENABLE_AD                 1  // enable A/D update
#define AERQ_IMCTL_SET_VELOCITY_TIMEBASE     2  // set timembase in msec, +1 = num msec
#define AERQ_IMCTL_DISABLE_VELTRACK1_4       3  // dis axis vel calc, +1 = axis 0-3
#define AERQ_IMCTL_ENABLE_VELTRACK1_4        4  // en  axis vel calc  +1 = axis 0-3
#define AERQ_IMCTL_SET_AXIS5_POS_FDBK        5  // set axis 5 internal position feedback
#define AERQ_IMCTL_SET_AXIS6_POS_FDBK        6  //          6 
#define AERQ_IMCTL_SET_AXIS7_POS_FDBK        7  //          7
#define AERQ_IMCTL_SET_AXIS8_POS_FDBK        8  //          8
#define AERQ_IMCTL_SET_AXIS5_POS_CMD         9  // set axis 5 internal position cmd
#define AERQ_IMCTL_SET_AXIS6_POS_CMD        10  //          6
#define AERQ_IMCTL_SET_AXIS7_POS_CMD        11  //          7
#define AERQ_IMCTL_SET_AXIS8_POS_CMD        12  //          8


//*****************************************************************************
// These are dsp codes to for the "aerq_immed_read()" function
// some commands take a sub argument (+1)
// which is usually an axis/channel index (0-?)

#define AERQ_IMRD_AD1_8                      0  // A/D convertor reading
#define AERQ_IMRD_VEL1_4                     1  // axis vel, +1 = axis index
#define AERQ_IMRD_8X3LOW                     2  // 8x3 inputs lower 8 bits
#define AERQ_IMRD_8X3MID                     3  //            middle 8 bits
#define AERQ_IMRD_8X3HIGH                    4  //            high   8 bits
#define AERQ_IMRD_DPFEED1_4                  5  // plane 1-4 contour feed  +1 = plane 0-3
#define AERQ_IMRD_TRAJFLAG1_4                6  // plane 1-4 trajectory flags +1 = plane 0-3
#define AERQ_IMRD_ISBX                       7  // read iSBX inputs
#define AERQ_IMRD_KPOS                       8  // read KPOS
#define AERQ_IMRD_KI                         9  // read KI
#define AERQ_IMRD_KP                        10  // read KP
#define AERQ_IMRD_VFF                       11  // read VFF
#define AERQ_IMRD_AFF                       12  // read AFF
#define AERQ_IMRD_AXIS_FAULT                13  // same as "aerq_read_status(1-4)"
#define AERQ_IMRD_AXIS_FLAGS                14  // misc axis flags
#define AERQ_IMRD_FEEDBACK_STATUS           15  // feedback transducer data
#define AERQ_IMRD_PSO_INPUTS                16  // ISA: read the PSO inputs
#define AERQ_IMRD_ABORT_AD_ENCPOS           17  // PCI: "A/D abort" axis encoder positions
#define AERQ_IMRD_ABORT_AD_MAXMIN           18  // PCI: "A/D abort" max A/D value
#define AERQ_IMRD_NOTCH2_BUFFER_ADDRESS     19  // PCI: start of notch2 flag X: memory
#define AERQ_IMRD_GLOBAL_FAULT_MASK         20  // Global Fault Mask for a specified axis
#define AERQ_IMRD_AXIS_FAULT_ADDRESS        21  // Start of axis fault X: memory
#define AERQ_IMRD_CAL_ACTIVE                22  // check if Linear/Ortho Cal Active for an axis
#define AERQ_IMRD_PRIMARY_OUTPUT_BUS        23  // read the standard 8 outputs
#define AERQ_IMRD_8X3DIR                    24  // PCI: direction of the 8x3 I/O banks
#define AERQ_IMRD_FIRE_ARRAY_IDX            25  // PCI: current Fire Array Location
#define AERQ_IMRD_PID_OUTPUT                26  // PID Loop Output By Axis
#define AERQ_IMRD_PCI_INPUTS                27  // PCI: read 4 inputs on P9 connector


//*****************************************************************************
// Defines which data to update to single axis firing FPGA
//  ("WAPIAerSingleSetup()/aerq_single_setup()")
// These shoud be ORed together and passed to the "load" argument.
// Data in the corresponding argument array will be loaded to the FPGA
//  if the flag is set.

#define SINGLE_LOAD_FIREAXIS       0x000400   // fire axis select
#define SINGLE_LOAD_WIN1AXIS       0x000001   // window 1 axis select
#define SINGLE_LOAD_WIN2AXIS       0x000002   // window 2 axis select
#define SINGLE_LOAD_FIREDIST       0x000004   // fire distance
#define SINGLE_LOAD_PULSEWIDTH     0x030008   // output pulse width
#define SINGLE_LOAD_WIN1LOW        0x000010   // window 1 low
#define SINGLE_LOAD_WIN1HIGH       0x000020   // window 1 high
#define SINGLE_LOAD_WIN1CNT        0x000040   // load counter 1
#define SINGLE_LOAD_WIN2LOW        0x000080   // window 2 low
#define SINGLE_LOAD_WIN2HIGH       0x000100   // window 2 high
#define SINGLE_LOAD_WIN2CNT        0x000200   // load counter 2
#define SINGLE_LOAD_FIREMRK        0x000800   // fire axis marker setup
#define SINGLE_LOAD_WIN1MRK        0x001000   // win1 axis marker setup
#define SINGLE_LOAD_WIN2MRK        0x002000   // win2 axis marker setup
#define SINGLE_LOAD_ARRAY_IDX      0x004000   // set current array index
#define SINGLE_LOAD_ARRAY_LENGTH   0x008000   // set array length
#define SINGLE_LOAD_FIRE_COUNT     0x010000   // load "fire N pulses" register ( "TOGGLE N" )
#define SINGLE_LOAD_PULSE_OFFTIME  0x020000   // load pulse off time data

// long argument array for "aerq_single_setup()".
#define SINGLE_ARG_ARRAY_SIZE      17+1   // array size (longs)

/******************************************************************************
1) The following are argument indexes into the above array which is sent to
      "WAPIAerSingleSetup()/aerq_single_setup()"
2) The corresponding "load data" flag (SINGLE_LOAD_xxxx) should be set in the
      "load" argument passed to "WAPIAerSingleSetup()/aerq_single_setup()".
3) All unused arguments should be cleared.
******************************************************************************/
#define SINGLE_ARG_FIREAXIS        0    // fire axis select      (AXIS1-AXIS8)
#define SINGLE_ARG_WIN1AXIS        1    // window 1 axis select  (AXIS1-AXIS8)
#define SINGLE_ARG_WIN2AXIS        2    // window 2 axis select  (AXIS1-AXIS8)
#define SINGLE_ARG_FIREDIST        3    // fire distance
#define SINGLE_ARG_PULSEWIDTH      4    // output pulse width    (.1usec increments)
#define SINGLE_ARG_WIN1LOW         5    // window 1 low
#define SINGLE_ARG_WIN1HIGH        6    // window 1 high
#define SINGLE_ARG_WIN1CNT         7    // load counter 1
#define SINGLE_ARG_WIN2LOW         8    // window 2 low
#define SINGLE_ARG_WIN2HIGH        9    // window 2 high
#define SINGLE_ARG_WIN2CNT         10   // load counter 2
#define SINGLE_ARG_FIREMRK         11   // fire axis marker trigger setup
#define SINGLE_ARG_WIN1MRK         12   // window 1  marker trigger setup
#define SINGLE_ARG_WIN2MRK         13   // window 2  marker trigger setup
#define SINGLE_ARG_ARRAY_IDX       14   // current array index
#define SINGLE_ARG_ARRAY_LENGTH    15   // array length
#define SINGLE_ARG_FIRE_COUNT      16   // fire N pulses  ( "TOGGLE N" command )
#define SINGLE_ARG_PULSE_OFFTIME   17   // pulse off time (.1 usec increments )

//*****************************************************************************
// Defines "action" commands for single axis firing FPGA
// These should be ORed together and passed to the "cmd" argument.
// These command flags do not have data directly associated with them.

#define SINGLE_ENABLE_WINDOW1      0x000001       // enable window 1
#define SINGLE_DISABLE_WINDOW1     0x000002       // disable window 1
#define SINGLE_ENABLE_WINDOW2      0x000004       // enable window 2
#define SINGLE_DISABLE_WINDOW2     0x000008       // disable window 2
#define SINGLE_PULSE_OUTMAIN       0x000010       // set main output as pulse signal
#define SINGLE_WINDOW_OUTMAIN      0x000020       // set main output as window signal

#define SINGLE_TRACK_ABS           0x000040       // when fire off, maintain position
#define SINGLE_TRACK_INC           0x000080       // when fire off, reset position 
#define SINGLE_WINDOW_ABS          0x000100       // when outside window, maintain position
#define SINGLE_WINDOW_INC          0x000200       // when outside window, reset position 
#define SINGLE_ARRAY_ON            0x000400       // do firing with array 
#define SINGLE_ARRAY_OFF           0x000800       // turn array mode off, use fixed distance 
#define SINGLE_ARRAY_LOAD          0x001000       // load count register and next register 
#define SINGLE_ARRAY_LOADNEXT      0x002000       // load next register only
// Removed in V5.17 - use "PULSE ontime,offtime,count" instead of "TOGGLE count"
//#define SINGLE_TOGGLE_OUTPUT_COUNT 0x008000       // fire N pulses ( "TOGGLE N" command )
#define SINGLE_SINGLE_PULSE        0x004000       // generate one pulse event
#define SINGLE_TOGGLE_OUTPUT       0x008000       // continuous pulse output

#define SINGLE_TRACK_ONE_DIR       0x010000       // enable one directional tracking
#define SINGLE_TRACK_NEG           0x020000       //   tracks in neg direction only
#define SINGLE_TRACK_POS           0x000000       //     ""   "  pos    ""      ""

#define SINGLE_ENABLE_OUTPUT       0x800000       // fire output on
#define SINGLE_DISABLE_OUTPUT      0x400000       // fire off after current pulse 
#define SINGLE_ABORT_OUTPUT        0x200000       // fire off immediate

//***************************************************************************** 
// These are for the "units" argument to "WAPIAerSingleSetup()/aerq_single_setup()" 
// and may be ORed together to specify the format of the data. 
// If the flag is set, the data is interpreted in encoder steps. Otherwise 
// units are in "PROGRAM" steps. 

#define SINGLE_UNITS_DIST_IN_STEPS      0x01 // fire distance in enc steps, else prog steps 
#define SINGLE_UNITS_COUNTER_IN_STEPS   0x02 // counter load in enc steps, else prog steps 
#define SINGLE_UNITS_WINLOW_IN_STEPS    0x04 // window max in enc steps, else prog steps 
#define SINGLE_UNITS_WINHIGH_IN_STEPS   0x08 // window min in enc steps, else prog steps 
#define SINGLE_UNITS_ENCODER_X2         0x10 // encoder in X2 mode, not X4 

// internal for bounds checking 
//#define SINGLE_MAX_PUSLE_WIDTH     0x0ffffff  // 24 bit counter

#define SINGLE_MAX_FIRE_DISTANCE   0x07ffff0  // 24 bit up / down counter 
#define SINGLE_MAX_WINDOW          0x07fffffff // 32 bit up / down counter 
#define SINGLE_MIN_WINDOW          0x080000000 //    and register
#define SINGLE_CLOCKS_PER_USEC     40             // clocks per micro second

#define SINGLE_MAX_NUM_PULSES      0xffffff
#define SINGLE_MAX_ON_TIME_uSEC    (16777216.0/40.0)
#define SINGLE_MAX_OFF_TIME_uSEC   (16777216.0/40.0)


// END OF "SINGLE AXIS FIRING" DEFINITIONS


//*****************************************************************************
// single axis firing for U511
//  ( requires XC10 FPGA option and bit #6 of prm #99 set to 1 )

#define SINGLE_U511_ARG_ARRAY_SIZE      3  // array size (longs)

// these are "LOAD DATA" bit flags for DSP   ( load flags )
#define SINGLE_U511_LOAD_FIREDIST   0x000001   // fire distance
#define SINGLE_U511_LOAD_PULSEWIDTH 0x000002   // output pulse width
#define SINGLE_U511_LOAD_FIREAXIS   0x000004   // fire axis select

// these are "COMMAND"  bits for DSP         ( command flags )
#define SINGLE_U511_ENABLE_OUTPUT   0x800000       // fire output on
#define SINGLE_U511_DISABLE_OUTPUT  0x400000       // fire off after current pulse
#define SINGLE_U511_TRACK_ABS       0x000040       // when fire off, maintain position
#define SINGLE_U511_TRACK_INC       0x000080       // when fire off, reset position

// array offsets into "args" array passed to "aerq_single_setup_u511()"
#define SINGLE_U511_ARG_FIREAXIS   0
#define SINGLE_U511_ARG_FIREDIST   1
#define SINGLE_U511_ARG_PULSEWIDTH 2

// bounds checking
#define SINGLE_U511_MAX_FIRE_DISTANCE   0xfffe    // 16 bit
#define SINGLE_U511_CLOCKS_PER_USEC     20        // 20Mhz clock

//***************************************************************************** 
// Dual axis firing definitions

// aerq_dual() "load" data flags. Tells DSP to update FPGA hardware if set
#define DUAL_LOAD_AXIS1       0x000001       // load channel 1 axis number 
#define DUAL_LOAD_AXIS2       0x000002       // load channel 2 axis number 
#define DUAL_LOAD_VECTORDIST  0x000004
#define DUAL_LOAD_PULSEWIDTH  0x030008       // load on time, off time, and count
					     // for single pulse, set off time and pulse count to 0
					     // in the aerq_dual() argument array

// aerq_dual() "args" argument array offsets and definitions
// array size...
#define DUAL_ARG_ARRAY_SIZE    6              // size of array to hold following arguments
// array offsets...
#define DUAL_ARG_AXIS1         0              // channel 1 axis
#define DUAL_ARG_AXIS2         1              // channel 2 axis
#define DUAL_ARG_VECTORDIST    2              //
#define DUAL_ARG_PULSEWIDTH    3              // ( use DUAL_ARG_PULSE_ONTIME )
// V5.19 - specify ontime, offtime, and pulse count instead of just PULSEWIDTH
#define DUAL_ARG_PULSE_ONTIME  3              // on time ( replaces ...PULSEWIDTH)
#define DUAL_ARG_PULSE_OFFTIME 4              // off time
#define DUAL_ARG_FIRE_COUNT    5              // pulse count

// aerq_dual() "cmd" command flags
#define DUAL_CMD_FIREON       0x800000       // main output on
#define DUAL_CMD_FIREOFF      0x400000       // main output off 
#define DUAL_CMD_AXIS2_OFF    0x008000       // force second counter in reset
#define DUAL_CMD_SINGLE_PULSE     0x002000   // command one pulse event
#define DUAL_CMD_CONTINUOUS_PULSE 0x004000   // command continuous pulse event

// aerq_dual() "units" units flag
#define DUAL_UNITS_DIST_IN_STEPS   0x01
#define DUAL_UNITS_ENCODER_X2      0x02
#define DUAL_UNITS_ENCODER_X1      0x04

// internal stuff
#define DUAL_MAX_FIRE_DISTANCE   0xffffff
#define DUAL_CLOCKS_PER_USEC     40
#define DUAL_MAX_NUM_PULSES      0xffffff
#define DUAL_MAX_ON_TIME_uSEC    (16777216.0/40.0)
#define DUAL_MAX_OFF_TIME_uSEC   (16777216.0/40.0)

//***************************************************************************** 
// FPGA setup codes ( for PCI card ) 
// these are the codes that are returned / used in... 
// "aerq_write_fpga_setup_code( )" AND "aerq_read_fpga_setup_code( )" 
// 
#define U500PCI_FPGA1              0         // FPGA1 code 
#define U500PCI_FPGA2              1         // FPGA2 code 

#define U500PCI_FPGA1_DIFFENC      0         // default differential encoder 
#define U500PCI_FPGA1_SYNCENC      1         // MXH sync interface 
#define U500PCI_FPGA1_RES2         2         // reserved 
#define U500PCI_FPGA1_RES3         3         // 
#define U500PCI_FPGA1_RES4         4         // 
#define U500PCI_FPGA1_RES5         5         // 
#define U500PCI_FPGA1_RES6         6         // 
#define U500PCI_FPGA1_RES7         7         // 

#define U500PCI_FPGA2_VELENC       0         // velocity loop feedback 
#define U500PCI_FPGA2_SINGLE       1         // single axis firing 
#define U500PCI_FPGA2_RES2         2         // 
#define U500PCI_FPGA2_RES3         3         // 
#define U500PCI_FPGA2_DUAL         4         // dual loop firing ( requires factory option ) 
#define U500PCI_FPGA2_RES5         5         // 
#define U500PCI_FPGA2_RES6         6         // 
#define U500PCI_FPGA2_RES7         7         // 

// "aerq_read_option_code()" definitions 
#define U500PCI_FPGA2_OPTION0_DUAL  1        // option code 0, dual axis firing 

//***************************************************************************** 
// defines for "aerq_analog_vector()" function, "cmd" argument 
#define ANALOG_VECTOR_VEL_ON       0x000001  // turn velocity tracking on 
#define ANALOG_VECTOR_POS_ON       0x000002  // turn position tracking on 
#define ANALOG_VECTOR_OFF          0x000004  // turn tracking off 
#define ANALOG_VECTOR_RAMPDOWN     0x000008  // ramp current analog output to 0 
#define ANALOG_VECTOR_RESTART      0x000010  // restart tracking with previous configuration 

// defines for "aerq_analog_vector()" function, "units" argument 
#define ANALOG_VECTOR_UNITS_STEPS  0x000001  // encoder counts instead of prog steps 

// for error checking 
#define ANALOG_VECTOR_MAX_POS      0x7fffff  // maximum position / velocity argument 

// for "aerq_queue_pos_compare()" function 
// queue waits while encoder position .... 
#define U500_POS_COMP_WAIT_LT      0x000001   // is less than specified value 
#define U500_POS_COMP_WAIT_GT      0x000002   // is greater than specified value 

#define U500_MAX_SHAPER_COEFF      40         // max number of pre-shaper coeff 
#define U500_SHAPER_ON             0x000001   // turn shaping on  for axis 
#define U500_SHAPER_OFF            0x000002   // turn shaping off for axis 

//***************************************************************************** 

/******* Linear Move structure. Up to 4 axis of contoured motion ************/
typedef struct INDEXMOVE_
{
  short   flag;               // ( for internal use only )
  short   axes;               // combine AXIS1 | AXIS2 | AXIS3 | AXIS4 
  long    dist[U500_NUM_AXES];     // axis 1,2,3,4 move lengths in program steps
  double  ifeed[U500_NUM_AXES];    //  individual axis velocities in program steps / sec 
			      //   0 = no change, 
} INDEXMOVE;
typedef INDEXMOVE *PINDEXMOVE;

typedef struct LINEARMOVE_
{
  short   flag;               // flag - UPDATECNTRFEED, UPDATECNTRTIME,...ENDFEED 
  short   axes;               // combine AXIS1 | AXIS2 | AXIS3 | AXIS4 
  long    dist[U500_NUM_AXES];     // axis 1,2,3,4 move lengths in program steps 
  double  cfeed;              // specifies new contour feedrate, 0 = no change, or feedtime 
  short   trig_accel;         // trigger plane 1-4 after accel, 0 - none 
  short   trig_decel;         // trigger plane 1-4 at decel, 0 - none 
  double  endfeed ;           // ending feedrate for vector 
			      //    <> 0 vector velocity in steps / second 
} LINEARMOVE;
typedef LINEARMOVE *PLINEARMOVE;

typedef struct CIRCMOVE_      // circle definition structure 
{
  short  flag;                // flag - UPDATECNTRFEED, UPDATECNTRTIME, ...ENDFEED 
  short  mtype ;              // move type - MOVECW, MOVECCW 
  short  axis1,axis2;         // axis 1,2 flags AXISn 
  long   endpnt1,endpnt2;     // axis 1,2 endpoints 
  long   cen1,cen2 ;          // axis 1,2 center points 
  double cfeed ;              // contour feedrate to update, if above flag is set 
  double endfeed ;           // ending feedrate for vector 
  double radius;
} CIRCMOVE;
typedef CIRCMOVE *PCIRCMOVE;

/******   INTERNAL   ******/
typedef struct CIRCALC_
{
  short   movetype;
  double  norm;
  long    radius;
  long    start_ang;
  long    delta_ang;
  double  xc,yc;                   // center points 
  int     full;                    // full circle flag 
} CIRCALC;

typedef struct GAIN_PARMS_S
{
  short   axis;
  short   parm_flag[NUM_GAIN_PARMS];
  double  value[NUM_GAIN_PARMS];
} GAIN_PARMS;

typedef GAIN_PARMS *PGAIN_PARMS;

/*
typedef struct CAM_POINT_S
{
  short camtable;
  short point;
  long masterpos_time;
  long slavepos;
  short interp_type;
  short output_type;
  short output_bit_num;
  short output_state;
} CAM_POINT;

typedef CAM_POINT *PCAM_POINT;
*/

typedef struct QLIB_ERROR_      // error code to ascii string table 
{
   AERERR_CODE code;
   char        string[80];
} QLIB_ERROR ;


#ifdef __cplusplus
  extern "C" {
#endif

//----------------------------------------------------------------------------- 
// Function declarations 
//----------------------------------------------------------------------------- 

AERERR_CODE AER_LIBENTRY aerq_get_version( PAER_VERSION pVer );
AERERR_CODE AER_LIBENTRY aerq_get_qlib_version( short *maj, short *min );
AERERR_CODE AER_LIBENTRY aerq_enableaxes( BOARD *pb, short axis);
AERERR_CODE AER_LIBENTRY aerq_disableaxes( BOARD *pb, short axis);
AERERR_CODE AER_LIBENTRY aerq_acceleration( BOARD *pb, short axis, double acc);
AERERR_CODE AER_LIBENTRY aerq_brake(BOARD *pb, short on_off);
AERERR_CODE AER_LIBENTRY aerq_dac(BOARD *pb, short channel, double volts);
AERERR_CODE AER_LIBENTRY aerq_dwell(BOARD *pb, long duration);
AERERR_CODE AER_LIBENTRY aerq_gain(BOARD *pb, GAIN_PARMS *gain);
AERERR_CODE AER_LIBENTRY aerq_gear(BOARD *pb, short slave, short master,
   short masterspecial, long slave_ratio, long master_ratio);
AERERR_CODE AER_LIBENTRY aerq_lvdt(BOARD *pb, short axis, short channel,
   short dir);
AERERR_CODE AER_LIBENTRY aerq_motor_commu(BOARD *pb, short axis,
   double volts);
AERERR_CODE AER_LIBENTRY aerq_motor_setup(BOARD *pb, short channel,
   double volts, short phase);
AERERR_CODE AER_LIBENTRY aerq_outmain(BOARD *pb, ULONG pattern, ULONG value);
AERERR_CODE AER_LIBENTRY aerq_queue(BOARD *pb, short q_type, long arg1, long arg2);
AERERR_CODE AER_LIBENTRY aerq_ramp(BOARD *pb, short ramptime);
AERERR_CODE AER_LIBENTRY aerq_rounding(BOARD *pb, short flag,
   short round_time);
AERERR_CODE AER_LIBENTRY aerq_segment(BOARD *pb, short segtime);
AERERR_CODE AER_LIBENTRY aerq_start(BOARD *pb, short planes);
AERERR_CODE AER_LIBENTRY aerq_softlimit(BOARD *pb, short axis,
double ccw, double cw);
AERERR_CODE AER_LIBENTRY aerq_trajectory(BOARD *pb, short traj);
AERERR_CODE AER_LIBENTRY aerq_umfo(BOARD *pb, short on_off, double mfo);
AERERR_CODE AER_LIBENTRY aerq_velocity(BOARD *pb, short on_off);
AERERR_CODE AER_LIBENTRY aerq_get_velocity(BOARD *pb);
AERERR_CODE AER_LIBENTRY aerq_get_old_velocity(BOARD *pb );

AERERR_CODE AER_LIBENTRY aerq_homeaxes(BOARD *pb, short axis);
AERERR_CODE AER_LIBENTRY aerq_halt(BOARD *pb);

AERERR_CODE AER_LIBENTRY aerq_index(BOARD *pb, INDEXMOVE *ims );
AERERR_CODE AER_LIBENTRY aerq_index_feed(BOARD *pb, INDEXMOVE *ims );

AERERR_CODE AER_LIBENTRY aerq_asyncmotion(BOARD *pb, short axis, short flag,
   long dist, double vel, double acc);
AERERR_CODE AER_LIBENTRY aerq_adjmotion(BOARD *pb, short axis, short ramp,
   long dist, double vel, double acc);
AERERR_CODE AER_LIBENTRY aerq_singen(BOARD *pb, double *rampdata, short calcflag);
AERERR_CODE AER_LIBENTRY aerq_freerun(BOARD *pb, short axis,
double feedrate, long dist);

AERERR_CODE AER_LIBENTRY aerq_contour(BOARD *pb, CIRCMOVE *cms1 , CIRCMOVE *cms2 , LINEARMOVE *lms );
int         AER_LIBENTRY aerq_get_contour_mode(BOARD *pb );
AERERR_CODE AER_LIBENTRY aerq_contour_feed( BOARD *pb, double cfeed );

AERERR_CODE AER_LIBENTRY aerq_slew( BOARD *pb , short h1, short v1,
   short h2, short v2, short cancel );

AERERR_CODE AER_LIBENTRY aerq_errormask( BOARD *pb , short axis ,
   short masknum, long maskmsw, long masklsw );

AERERR_CODE AER_LIBENTRY aerq_cint( BOARD *pb , short onflag ,
   short masterin, short slaveout );

AERERR_CODE AER_LIBENTRY aerq_errortoascii( AERERR_CODE error, char *psErr );
AERERR_CODE AER_LIBENTRY aerq_axiserrortoascii( ULONG axiserror, char *psErr );

AERERR_CODE AER_LIBENTRY aerq_dsplservo( BOARD *pb, short axis, short signal ,
double volts, short channel );

AERERR_CODE AER_LIBENTRY aerq_loadcalfile( BOARD *pb , char *filename );

AERERR_CODE AER_LIBENTRY aerq_targetrack_enable  ( BOARD *pb , short axis );
AERERR_CODE AER_LIBENTRY aerq_targetrack_disable ( BOARD *pb , short axis );
AERERR_CODE AER_LIBENTRY aerq_targetrack_position( BOARD *pb , short axis,
   long tarpos,double adjvel, double lpcoeff );

ULONG AER_LIBENTRY aerq_read_status(BOARD *pb, short index);
ULONG AER_LIBENTRY aerq_read_status2(BOARD *pb, short index);

AERERR_CODE AER_LIBENTRY aerq_software_position( BOARD *pb , short axes , int fbkpos );
VOID AER_LIBENTRY _aerq_translate_status( BOARD *pb, PLONG fbkarr);

AERERR_CODE AER_LIBENTRY aerq_autotune( BOARD *pb, short drive, double sfreq ,
		  long amp, short scyc, short numfreq, short stime, short nsamp );

double AER_LIBENTRY aerq_read_position( BOARD *pb , short axis );
double AER_LIBENTRY aerq_read_position2( BOARD *pb , short axis );

//int check_plane( BOARD *pb ); 

AERERR_CODE AER_LIBENTRY aerq_scale( BOARD *pb, short mode );
AERERR_CODE AER_LIBENTRY aerq_g92_offset( BOARD *pb, short axis , double *offset );
AERERR_CODE AER_LIBENTRY aerq_abs_inc( BOARD *pb, short mode );

AERERR_CODE AER_LIBENTRY aerq_psoc(BOARD *pb, short psocase , long *data );

AERERR_CODE AER_LIBENTRY aerq_psod(BOARD *pb, int psocase, double *args, double *array, int arraysize );
AERERR_CODE AER_LIBENTRY aerq_psof(BOARD *pb, short psocase , long numpulses_or_dist , short axismask );
AERERR_CODE AER_LIBENTRY aerq_psop(BOARD *pb, short psocase, long *arg,
	    double *arraystart, short npnts, short startidx, short arraysize );
AERERR_CODE AER_LIBENTRY aerq_psor(BOARD *pb, short psocase );
AERERR_CODE AER_LIBENTRY aerq_psos(BOARD *pb, short psocase , short axis, double scale );
AERERR_CODE AER_LIBENTRY aerq_psot(BOARD *pb, short psocase , long l1 , long l2,
				   double vmin, double vmax );
AERERR_CODE AER_LIBENTRY aerq_psom(BOARD *pb, short psocase ,
       double *arraystart, short npnts, short startidx, short arraysize );

AERERR_CODE AER_LIBENTRY aerq_map(BOARD *pb, short drive, short axis, short plane );
AERERR_CODE AER_LIBENTRY aerq_spline(BOARD *pb, short axis, long *dist ,
		  short on_off_lin, double time, double feed, short outmask , short outdata );
AERERR_CODE AER_LIBENTRY aerq_cubic(BOARD *pb, short axis, long flags, 
		  long time, double *dist, double *a, double *b, double *c); 

double adjust_modulo( BOARD *pb, int axis , double pos , int posneg );

AERERR_CODE AER_LIBENTRY aerq_readisbx(BOARD *pb, long add, long *retdata );
AERERR_CODE AER_LIBENTRY aerq_writeisbx(BOARD *pb, long addr, short bitvalid, short bit, long data); 
AERERR_CODE AER_LIBENTRY aerq_readadc(BOARD *pb, int channel, double *retdata );
AERERR_CODE AER_LIBENTRY aerq_adccontrol(BOARD *pb, int enable );
AERERR_CODE AER_LIBENTRY aerq_plc(BOARD *pb, long *plccmd , int size , int on, int off );

AERERR_CODE AER_LIBENTRY aerq_plcflag(BOARD *pb, long mask, long value );
AERERR_CODE AER_LIBENTRY aerq_immed_output(BOARD *pb, short bank, long mask, long value );
AERERR_CODE AER_LIBENTRY aerq_read_inputs(BOARD *pb, short bank, long *ret );
AERERR_CODE AER_LIBENTRY aerq_axisfilter(BOARD *pb, short axis, long *time );
AERERR_CODE AER_LIBENTRY aerq_part_rotate(BOARD *pb, short haxis, short vaxis, double angle );
AERERR_CODE AER_LIBENTRY aerq_contour_mode(BOARD *pb, short mode );
AERERR_CODE AER_LIBENTRY aerq_set_crc(BOARD *pb, short mode, short Xaxis, short Yaxis, double rad );
AERERR_CODE AER_LIBENTRY aerq_set_plane(BOARD *pb, short plane );
AERERR_CODE AER_LIBENTRY aerq_scf( BOARD *pb, short axis, double scalefactor , int abs );

short AER_LIBENTRY aerq_axis_to_drive( BOARD *pb, short axis );
short AER_LIBENTRY aerq_drive_to_axis( BOARD *pb, short drive, short *plane );
short AER_LIBENTRY aerq_axis_to_drivebit( BOARD *pb, short axis );
double             get_drive_scalefactor( BOARD *pb, short drive );

AERERR_CODE AER_LIBENTRY aerq_config_8x3_iobus(BOARD *pb, UBYTE bank, UBYTE dir );
AERERR_CODE AER_LIBENTRY aerq_write_8x3_iobus(BOARD *pb, UBYTE bank, UBYTE mask, UBYTE data );
AERERR_CODE AER_LIBENTRY aerq_read_8x3_iobus(BOARD *pb, UBYTE bank, UBYTE *data );

AERERR_CODE AER_LIBENTRY aerq_get_firmware_info(BOARD *pb, long *plInfo);
AERERR_CODE AER_LIBENTRY aerq_find_marker(BOARD *pb, short axis);
AERERR_CODE AER_LIBENTRY aerq_scope_trigger(BOARD *pb);
AERERR_CODE AER_LIBENTRY aerq_sync(BOARD *pb);
AERERR_CODE AER_LIBENTRY aerq_dynamic_gain(BOARD *pb, short axisbit , short time, long gain );
AERERR_CODE AER_LIBENTRY aerq_set_plane_max_accel(BOARD *pb, double acc );
double AER_LIBENTRY aerq_get_plane_max_accel(BOARD *pb);
AERERR_CODE AER_LIBENTRY aerq_get_command_position( BOARD *pb , double *cmdpos );
int AER_LIBENTRY aerq_get_abs(BOARD *pb);
AERERR_CODE AER_LIBENTRY aerq_auto_focus(BOARD *pb, short axis, double *arg );

AERERR_CODE AER_LIBENTRY aerq_slave( BOARD *pb, long flags,
     short  slaveaxis,        short  masteraxis,
     double slavedist,        double masterdist,
     short  ramptime,         double maxslavetravel,
     long   syncdelay,        long   outputbit );

#ifdef WIN32
   AERERR_CODE AER_LIBENTRY aerq_read_registry( PREGSTRUCT pReg );
   AERERR_CODE AER_LIBENTRY aerq_write_registry( PREGSTRUCT pReg );
#endif

double atan2_fix( double y, double x );
double norm_ang( double ang );
AERERR_CODE conv_double_8_40( double arg, long *msw , long *lsw );
short AER_LIBENTRY aerq_get_trajectory(BOARD *pb);
AERERR_CODE AER_LIBENTRY aerq_pcbus_irq(BOARD *pb, long on_off );
AERERR_CODE AER_LIBENTRY aerq_save_stat(BOARD *pb, short varnum, short index);

AERERR_CODE AER_LIBENTRY aerq_edit_flash(BOARD *pb, long *retcode );
AERERR_CODE AER_LIBENTRY aerq_save_flash(BOARD *pb, long *retcode );
AERERR_CODE AER_LIBENTRY aerq_read_fpga_setup_code(BOARD *pb, long fpganum, long *setupcode );
AERERR_CODE AER_LIBENTRY aerq_write_fpga_setup_code(BOARD *pb, long fpganum, long setupcode );
AERERR_CODE AER_LIBENTRY aerq_read_option_code(BOARD *pb, long optionnum, long *optcode );
AERERR_CODE AER_LIBENTRY aerq_single_setup(BOARD *pb, long load, long cmd, long units, long *args );
AERERR_CODE AER_LIBENTRY aerq_dual_setup(BOARD *pb, long load, long cmd, long units, long *args );
AERERR_CODE AER_LIBENTRY aerq_analog_vector(BOARD *pb, long cmd, long units, SHORT axes,
		SHORT dacnum, double minvolt, double maxvolt, double maxvelpos, long sampletime );
AERERR_CODE AER_LIBENTRY aerq_in_position(BOARD *pb, SHORT axes, double time, long port, long bit);
AERERR_CODE AER_LIBENTRY aerq_queue_pos_compare(BOARD *pb, short axis, long position, long opcode );
AERERR_CODE AER_LIBENTRY aerq_load_array( PBOARD pBoard, long *parray, long arridx, long npnts );
AERERR_CODE AER_LIBENTRY aerq_preshape( BOARD *pb, SHORT axis, long flags,long *time, long *coeff, long numcoeff );

AERERR_CODE AER_LIBENTRY aerq_set_env_comp(BOARD *pb, SHORT mode, SHORT axis, SHORT analog_mask,
										   DOUBLE set_temp, DOUBLE set_pressure, DOUBLE set_humidity);
AERERR_CODE AER_LIBENTRY aerq_read_env_comp(BOARD *pb);
AERERR_CODE AER_LIBENTRY aerq_load_env_comp_file(BOARD *pb, CHAR *ecfile);

double AER_LIBENTRY conv_48_to_double( long *msw , long *lsw );
AERERR_CODE AER_LIBENTRY aerq_segment_array( BOARD *bp, short axes, long startidx, long numseg );
AERERR_CODE AER_LIBENTRY aerq_freerun2(BOARD *pb, short axis, double feedrate, long dist, double accel, short mode  );

AERERR_CODE AER_LIBENTRY aerq_set_velocity_timebase( BOARD *bp, LONG millisec);
AERERR_CODE AER_LIBENTRY aerq_set_velocity_tracking(BOARD *bp, short axis_number, short en_disable);
AERERR_CODE AER_LIBENTRY aerq_read_velocity(BOARD *bp, short axis_number, long *vel );
AERERR_CODE AER_LIBENTRY aerq_get_plane_flags( BOARD *bp, short plane, long *flag );
AERERR_CODE AER_LIBENTRY aerq_read_servo_gains( BOARD *pb, short axis, long *gains );
AERERR_CODE AER_LIBENTRY aerq_get_mfo_scale( BOARD *bp , double *scale );
AERERR_CODE AER_LIBENTRY aerq_raw_encoder_to_progsteps(BOARD *pb, int drive, double *rawenc, double *retval );

AERERR_CODE AER_LIBENTRY aerq_axis_flags( BOARD *pb, short axis, long *flag );
AERERR_CODE AER_LIBENTRY aerq_axis_fault( BOARD *pb, short axis, long *fault );
AERERR_CODE AER_LIBENTRY aerq_feedback_status( BOARD *pb, short axis, long *status);
AERERR_CODE AER_LIBENTRY aerq_homeaxes2(BOARD *pb, short axes, double speed[4] );
AERERR_CODE AER_LIBENTRY aerq_index2(BOARD *pb, INDEXMOVE *ims);
AERERR_CODE AER_LIBENTRY aerq_single_setup_u511(BOARD *pb, long load, long cmd, long units, long *args );
AERERR_CODE AER_LIBENTRY aerq_abort_on_AD(BOARD *pb, long adc_channel, double threshold, long flag); 
AERERR_CODE AER_LIBENTRY aerq_loop_transmission (BOARD *pb, short master, short slave, short adchannel, double adscale, short da1, short da2 );
AERERR_CODE AER_LIBENTRY aerq_phase_adjust(BOARD *pb, SHORT axis, double phase );
AERERR_CODE AER_LIBENTRY aerq_set_demo_mode(PBOARD pBoard, long on_off);
AERERR_CODE AER_LIBENTRY aerq_za_command(BOARD *pb, SHORT za_cmd_type, SHORT axis, double position, long flags );

AERERR_CODE AER_LIBENTRY aerq_set_secondary_axis_position( BOARD *pb, short axis, short mode, double *pos);

void double_to_dsp_double(double arg, long *msw, long *lsw, long *exp);

#ifdef __cplusplus
}
#endif

#include "qintface.h"

#endif
// __QUICK_H__ 
  
