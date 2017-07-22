
#ifndef __QROTATE_H__
#define __QROTATE_H__

#include "boards.h"
#include "global.h"

     #if 1 // defined USEBLOCKMOVE 
void do_parts_rotation( BOARD *bp, BLOCKMOVE *bm );
     #else
void do_parts_rotation( void *bp, long *dspgpb );
     #endif

void reset_rotate_buffer( BOARD *bp );

#endif
