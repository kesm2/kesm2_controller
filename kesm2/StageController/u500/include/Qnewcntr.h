


typedef struct MOVECALC_
{

     int    flag;         // #0 - 0/1 ramp up/down during first part 
          			 // #1 - 0/1 linear/sine ramp 
		                // #2 - 1 - skip calc this time - because of MFO 
// accel parameters 
     double avel;        // accel target velocity 
     long   atime ;      //       ramp time (ms) 
     double acvel ;      //       constant vel 
     double adang ;      //       delta angle (sine ramp)
     long   adist ;      //       ramp distance (machine steps)
     long   acadjstep;   //       adjust counts during accel ramp 

// steady state parameters 
     double ssvel;       // steady state velocity 
     long   sstime ;     //              time (ms) 
     long   ssdist ;     //              distance (machine steps) 

// decel parameters 
     double dvel;        // decel start velocity 
     long   dtime ;      //       ramp time (ms) 
     double ddang ;      //       delta angle (sine ramp)
     long   ddist ;      //       ramp distance (machine steps) 
     double dcvel ;      //       constant vel 
     long   dcadjstep;   //       adjust counts during decel ramp 

// MFO parameters 
     long  mfoinc   ;   // MFO accel / decel rate 

// each axis output parameters 
     int    mtype[4];           // output type linear/sine 
                                // bit #0 = 1 linear   (0x01) 
                                //     #1 = 1 CW       (0x02) 
                                //     #2 = 1 CCW      (0x04) 
                                //     #3 = 1 if linear -> do neg move 
                                //            if circle -> reverse axis output polarity 

     long   sang[4] ;           // start angle 
     double scale_radius[4];    // output % 
     long   axisstep[4];        // axis total move distance 
                                // for linear moves, this data is always positive 
                                //      neg move is set with bit 3 of mtype 
                                // for circluar moves, this data is signed 
                                //      axis velocity is reversed by bit #3 of mtype           
     double    endfeed;         // ending feedrate of g8 move 
} MOVECALC;

void    reset_new_g8( BOARD *bp );
int     axlet2drive( BOARD *bp, int axlet, int plane );

     #if 1 // defined USEBLOCKMOVE 

AERERR_CODE blockmove_to_gpb( BLOCKMOVE *bm, long *dspbuf );
int   new_circle( BOARD *bp, BLOCKMOVE *bm, long *outdata );
int   new_linear( BOARD *bp, BLOCKMOVE *bm, long *outdata );
int   do_new_contour( BOARD *bp, BLOCKMOVE *bm, long *outdata );
void  setup_master( BOARD *bp, MOVECALC *move, double scale, double max, BLOCKMOVE *bm );
int   send_move   ( BOARD *bp, long *dspgpb, MOVECALC *move, BLOCKMOVE *bm );
double calc_ramp_time( BLOCKMOVE *bm, double deltafeed , double scale );
void blockmove_to_gpb7c( BOARD *bp, long *dspbuf, BLOCKMOVE *bm );

     #else

int  new_circle    ( BOARD *bp, long *gpbdata, int type, long dang, long sang, long rad,
		       char a1, long end1, char a2, long end2 , GPBMOVETYPE *gpbmove );
int  new_linear    ( BOARD *bp, long *gpbdata, long dist[] , GPBMOVETYPE *gpbmove );
int  do_new_contour( BOARD *bp, long *inbuf );
int  send_move( BOARD *bp, long *dspgpb, MOVECALC *move, long progstep[] , GPBMOVETYPE *gpbmove );
void setup_master( BOARD *bp, MOVECALC *move, double scale , double max );
     
     #endif    // BLOCKMOVE 

int calc_circle_angles( int ccw, double xe, double ye, double xc, double yc,
			 double *startang, double *endang, double *deltaang, double *radius );

