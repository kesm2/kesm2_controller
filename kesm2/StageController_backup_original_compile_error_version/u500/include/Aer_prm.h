#ifndef __AER_PRM_H__
#define __AER_PRM_H__

#include "build.h"
#include "aerotype.h"

#ifdef __cplusplus
   extern "C" {
#endif

AERERR_CODE AER_LIBENTRY aer_open_param(CHAR *filename);
AERERR_CODE AER_LIBENTRY aer_read_param(SHORT which, CHAR *psParam);
AERERR_CODE AER_LIBENTRY aer_write_param(CHAR *sdata);
AERERR_CODE AER_LIBENTRY aer_save_param(CHAR *filename);
AERERR_CODE AER_LIBENTRY aer_prm_save(SHORT *params, LPSTR filename);
AERERR_CODE AER_LIBENTRY aer_close_param( void );
LONG        AER_LIBENTRY aer_is_param_open( void );
AERERR_CODE AER_LIBENTRY aer_create_default_param_file( CHAR *fname );

#ifdef __cplusplus
}
#endif 

typedef struct _prmBufr
{
     SHORT     distunit ;     // distance unit flag 
     SHORT     feedunit ;     // feedrate unit flag 
     SHORT     cntrmode ;     // current contouring mode flag G0,G1,G2,G3 
     SHORT     maxplane;      // Number of planes 1,2,or 4 

   struct plane
   {
      SHORT    ramptime;      // 19, 37, 55, 73 Plane contour ramping time 
      SHORT    metric  ;      // 20, 38, 56, 74 Plane metric flag 
      SHORT    linramp;       // 21, 39, 57, 75 Linear type ac/de(y/n) for contour plane 
      DOUBLE   cfeed ;        // 22, 40, 58, 76 Contour feedrate for plane 
      DOUBLE   ifeed[4] ;     // (23, 41, 59, 77 X)(24, 42, 60, 78 Y)(25, 43, 61, 79 Z)(26, 44, 62, 80, U) 
                              //  axis point to point feedrate 
      DOUBLE   clampfeed;     // 27, 45, 63, 81 Clamp feedrate (prog. steps/ms) for plane 
      SHORT    metdig;        // 29, 47, 65, 83 Metric mode number of decimal digits 
      SHORT    engdig  ;      // 30, 48, 66, 84 English mode number of decimal digits 
      double   nominalfeed;   // user defined feedrate "Fxxx"  ( prog steps / sec ) 
      double   currentfeed;   // clamped feedrate due to accel limitations ( prog steps/sec ) 
      double   endfeed;       // user defined end feedrate "EFxxx" ( prog steps / sec ) 
      int      g8on;          // programmed G8 mode on 
      double   parallelangle;  // CM 1 max angle between vectors for G8 (degrees) 
   } plane[4];

   struct axis
   {
      DOUBLE   metsf;         // 100, 200, 300, 400 Metric scale factor 
      DOUBLE   engsf;         // 101, 201, 301, 401 English scale factor 
      SHORT    plane;         //  3,  4,  5,  6 Map axis to plane 
      CHAR     name;          //  as XYZU
      CHAR     name_s[U500_AXIS_LENGTH+1];    //  axis name - string
   } axis[4];

      LONG            laserpower;

} PRMBUFR;

extern PRMBUFR  prmBufr[MAX_NUM_BOARDS];

#endif    // __AER_PRM_H__

