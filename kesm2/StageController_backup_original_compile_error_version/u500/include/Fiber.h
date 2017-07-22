/******************************************************************************

		  AEROTECH fiber library

		  Filename: "fiber.h"

******************************************************************************/
#include "build.h"

#ifndef __FIBER_H__
#define __FIBER_H__


#include "aerotype.h"   // Windows types for those who do not know them
#include "aerqcode.h"   // Error Codes
#include "aerqver.h"    // Version Information
#include "boards.h"     // Basic board information
#include "global.h"
#include "quick.h"

//#include "fiber-gpib.h"	// GPIB function definitions
//#include "decl-32.h"		// needed for GPIB

#ifdef __cplusplus
  extern "C" {
#endif


#define GET_PSUM \
				for (j=1; j<=ndim; j++) {\
				for (sum=0.0,i=1;i<=mpts;i++) sum += p[i][j];\
				psum[j]=sum;}
#define SWAP(a,b) {swap=(a);(a)=(b);(b)=swap;}

/*----------------------------------------------------------------------------- 
   Function declarations 
  ----------------------------------------------------------------------------- 
*/

double FindSpiralAngle(double curangle, long segment_length, double k, double rotate_guess);	
double aerq_fiber_CollectData( BOARD *bp, long inputMode, long inputChannel, long nDelay, PAERERR_CODE eRc);
double **aerq_fiber_alloc_matrix(long nrl, long nrh, long ncl, long nch);
void aerq_fiber_free_matrix_special(double **m, long nrl, long nrh, long ncl, long nch );
void AER_LIBENTRY aerq_fiber_free_matrix(BOARD *bp);

AERERR_CODE AER_LIBENTRY  aerq_fiber_get_numpnts(BOARD *bp, long *numpnts);
AERERR_CODE AER_LIBENTRY aerq_fiber_get_geocoord(BOARD *bp, double *axis1_coord, double *axis2_coord);
AERERR_CODE AER_LIBENTRY aerq_fiber_get_adval(BOARD *bp, double *adval);
AERERR_CODE AER_LIBENTRY aerq_fiber_get_saveddata1D(BOARD *bp, double *column_data, long column);
AERERR_CODE AER_LIBENTRY aerq_fiber_set_datasave_on(BOARD *bp, long datasave_on);
AERERR_CODE AER_LIBENTRY aerq_fiber_set_param(BOARD *bp, long routine, long paramnum, double value);
AERERR_CODE AER_LIBENTRY aerq_fiber_get_param(BOARD *bp, long routine, long paramnum, double *value);
AERERR_CODE AER_LIBENTRY aerq_fiber_get_default_param(BOARD *bp, long routine, long paramnum, double *value);

AERERR_CODE AER_LIBENTRY aerq_fiber_hillclimb(BOARD *bp);
AERERR_CODE AER_LIBENTRY aerq_fiber_spiral_fine(BOARD *bp);
AERERR_CODE AER_LIBENTRY aerq_fiber_spiral_rough(BOARD *bp);
AERERR_CODE AER_LIBENTRY aerq_fiber_fast_align(BOARD *bp);
AERERR_CODE AER_LIBENTRY aerq_fiber_fast_align3D(BOARD *bp);
AERERR_CODE AER_LIBENTRY aerq_fiber_fast_align4D(BOARD *bp);
AERERR_CODE AER_LIBENTRY aerq_fiber_geocenter2D(BOARD *bp);
AERERR_CODE AER_LIBENTRY aerq_fiber_centroid1D(BOARD *bp);
AERERR_CODE AER_LIBENTRY aerq_fiber_centroid2D(BOARD *bp);
AERERR_CODE AER_LIBENTRY aerq_fiber_centroid3D(BOARD *bp);

long AER_LIBENTRY aerq_fiber_checkstatus(BOARD *bp, DWORD timeout, AERERR_CODE *error_code);
long aerq_fiber_check_motion_done( BOARD *bp);

unsigned __stdcall aerq_fiber_hillclimb_thread(void *);
unsigned __stdcall aerq_fiber_spiral_fine_thread(void *);
unsigned __stdcall aerq_fiber_spiral_rough_thread(void *);
unsigned __stdcall aerq_fiber_fast_align_thread(void *);
unsigned __stdcall aerq_fiber_fast_align3D_thread(void *);
unsigned __stdcall aerq_fiber_fast_align4D_thread(void *);
unsigned __stdcall aerq_fiber_geocenter2D_thread(void *);
unsigned __stdcall aerq_fiber_centroid_thread(void *);

//--------------Fast Align specific functions----------------------------------

void amoeba(double **p, double y[], long ndim, double ftol, 
     double (*funk)(double[], long[], long, PBOARD, long, long, long, long), long axis[],
     BOARD *bp, long inputmode, long adchannel, long maxit, long nDelay, long *nfunk, long save_mode);
      
double amotry(double **p, double y[], double psum[], long ndim, 
       double (*funk)(double[], long[], long, PBOARD, long, long, long, long), long axis[],
       BOARD *bp, long inputmode, long adchannel, long nDelay, long ihi, double fac, long save_mode);

double MoveCollectData(double xy[], long axis[], long ndim, BOARD *bp, long inputmode, long adchannel, long nDelay, long save_mode);

AERERR_CODE FastAlignReturnToStart(long axis[], long ndim, BOARD *bp, long inputmode, long adchannel, long nDelay);

double *vector(long nl, long nh);
void free_vector(double *v, long nl,long nh);
//------------------------------------------------------------------------------------------------------------------

//------------------GPIB specific functions----------------------------------------------------------------------------
AERERR_CODE AER_LIBENTRY aerq_fiber_setup_GPIB(void);
AERERR_CODE AER_LIBENTRY aerq_fiber_close_GPIB(void);
AERERR_CODE AER_LIBENTRY aerq_GPIB_set_params(long devnum, long bdindx, long pad, long sad, long tmo, long eot, long eos);
AERERR_CODE AER_LIBENTRY aerq_GPIB_set_Dev(long devnum, long mode, long *Dev, long *iberror, long *ibstat);
//AERERR_CODE AER_LIBENTRY aerq_GPIB_set_on_off(long meterNum, long on_off);
AERERR_CODE AER_LIBENTRY aerq_GPIB_set_writestring(long devnum, char *wstring);
AERERR_CODE AER_LIBENTRY aerq_GPIB_set_bytes_to_read(long devnum, long bytes);
AERERR_CODE AER_LIBENTRY aerq_GPIB_set_Dev_offline(long devnum, long *iberror, long *ibstat);

AERERR_CODE AER_LIBENTRY aerq_GPIB_write_string(long devnum, char *wstring, long *iberror, long *ibstat);
AERERR_CODE AER_LIBENTRY aerq_GPIB_read_string(long devnum, char *rstring, long numbytes, long *iberror, long *ibstat);
AERERR_CODE AER_LIBENTRY aerq_GPIB_read_number(long devnum, double *value, long numbytes, long *iberror, long *ibstat);

AERERR_CODE AER_LIBENTRY aerq_GPIB_open();
AERERR_CODE AER_LIBENTRY aerq_GPIB_close();

AERERR_CODE AER_LIBENTRY aerq_fiber_set_sleep_mode(BOARD *bp, LONG mode, LONG sleepTime);


static long(__stdcall *PGPIB_Dev)
(long bdindx, long pad, long sad, long tmo,
 long eot, long eos, long *iberror, long *ibstat);

static long(__stdcall *PGPIB_Offline)
(long Dev, long *iberror, long *ibstat);

static long(__stdcall *PGPIB_Power)
(long Dev, char *wstr, long bytes_to_read, double *adval,
 long *iberror, long *ibstat);


static long (__stdcall *PGPIB_Write)
(long Dev, char *wstr, long *iberror, long * ibstat);
		

static long (__stdcall *PGPIB_ReadString)
(long Dev, char *rstr, long numbytes, long *iberror ,long *ibstat);
		

static long (__stdcall *PGPIB_ReadNumber)
(long Dev, double *val, long numbytes, long *iberror,long *ibstat);
//---------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#include "qintface.h"

#endif
// __FIBER_H__ 
  
