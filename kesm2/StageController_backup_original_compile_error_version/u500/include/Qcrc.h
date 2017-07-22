

#ifndef __QCRC_H__
#define __QCRC_H__


#define LINEAR         BLOCKMOVE_LINEAR      // 7 gpb contour type constants 
#define CWCIRCLE       BLOCKMOVE_CW          // 8 
#define CCWCIRCLE      BLOCKMOVE_CCW

#define CRCDONE        1            // crc done - see "vector".flag 
#define RADTODEG       180./PI
#define DEGTORAD       PI/180.


AERERR_CODE scan_crc( BOARD *bp , long *dspgpb );
void        reset_crc_buffer(void);
double      norm_ang( double ang );

#endif
