#ifndef __AER_U500_H__
#define __AER_U500_H__

     #if defined U511
#define   NUMGLBPROG  1
     #else
#define   NUMGLBPROG  4
     #endif


AERERR_CODE AER_LIBENTRY aer_open (SHORT nBoard);
AERERR_CODE AER_LIBENTRY aer_close (SHORT nBoard);
LONG        AER_LIBENTRY aer_is_open (SHORT nBoard);
AERERR_CODE AER_LIBENTRY aer_retry (VOID);
AERERR_CODE AER_LIBENTRY aer_initialize (CHAR *config, CHAR *program, CHAR *parameter);
AERERR_CODE AER_LIBENTRY aer_mxh_download_firmware(char *frmfile);
AERERR_CODE AER_LIBENTRY aer_mxh_read_status( ULONG *status );
AERERR_CODE AER_LIBENTRY aer_mxh_erase_chip( SHORT chip );
AERERR_CODE AER_LIBENTRY aer_mxh_read_loc( LONG addr, SHORT chip, LONG *data );
AERERR_CODE AER_LIBENTRY aer_mxh_write_block( LONG blksize, SHORT chip, ULONG startaddr, LONG *data );
AERERR_CODE AER_LIBENTRY aer_mxh_return_data( LONG blksize, LONG *pdatain );
AERERR_CODE AER_LIBENTRY aer_initialize_u500 (CHAR *config, CHAR *program, CHAR *parameter);
AERERR_CODE AER_LIBENTRY aer_initialize_pso (CHAR *firmware);

//SHORT AER_LIBENTRY aer_reset (SHORT which); 
double AER_LIBENTRY aer_read_plane_feedrate(short plane);
double AER_LIBENTRY aer_get_scalefactor (SHORT nAxisNum);

AERERR_CODE AER_LIBENTRY aer_software_initialize( CHAR *prmfile );
AERERR_CODE AER_LIBENTRY aer_abort (VOID);
AERERR_CODE AER_LIBENTRY aer_checkstatus (VOID);
AERERR_CODE AER_LIBENTRY aer_checkstatus2(VOID);
AERERR_CODE AER_LIBENTRY aer_pause (VOID);
AERERR_CODE AER_LIBENTRY aer_set_pause(SHORT on_off);
AERERR_CODE AER_LIBENTRY aer_faultack (VOID);
DOUBLE      AER_LIBENTRY aer_read_variable (SHORT idx);
VOID        AER_LIBENTRY aer_write_variable( SHORT idx , DOUBLE val );
DOUBLE      AER_LIBENTRY aer_read_position (SHORT which);
DOUBLE      AER_LIBENTRY aer_read_position2( SHORT idx );
ULONG       AER_LIBENTRY aer_read_status(SHORT which);
ULONG       AER_LIBENTRY aer_read_status2(SHORT which);

AERERR_CODE AER_LIBENTRY aer_send (char *cmd);
AERERR_CODE AER_LIBENTRY aer_send_glbsub( char *cmd);
AERERR_CODE AER_LIBENTRY aer_loadprogram (char *name, char *glbsun, char *glbmsub );
AERERR_CODE AER_LIBENTRY aer_loadglbsub( char *gsub )   ;
AERERR_CODE AER_LIBENTRY aer_loadglbmsub( char *gmsub ) ;

VOID AER_LIBENTRY       aer_unloadprogram( void )      ;
VOID AER_LIBENTRY       aer_unloadglbsub( void )       ;
VOID AER_LIBENTRY       aer_unloadglbmsub( void )      ;

VOID AER_LIBENTRY       unloadprgblk( BLKEXESTRUCT *startblk )  ;

AERERR_CODE  AER_LIBENTRY aer_execprogblock (void);
INT  AER_LIBENTRY       aer_get_auxcode (void);
INT  AER_LIBENTRY       aer_get_auxinfo( int idx  );

LPSTR AER_LIBENTRY      aer_get_aux_message (void );
LPSTR AER_LIBENTRY      aer_get_err_message (void );

INT  AER_LIBENTRY       aer_get_linenumber (void);
LPSTR AER_LIBENTRY      aer_get_filename (void ) ;
INT  AER_LIBENTRY       aer_goto_line ( int linenum);
VOID AER_LIBENTRY       aer_get_prgexe (PRGEXESTRUCT  **pexe);

LONG AER_LIBENTRY       aer_get_pauseflag( void );
LONG AER_LIBENTRY       aer_get_cycleflag( void );

/*
SHORT AER_LIBENTRY aer_write_param (CHAR *sdata);
*/

#ifdef WIN32
   // messer is copied into supplied buffer 
//   SHORT AER_LIBENTRY aer_read_param (SHORT which, CHAR *psParam); 
#else
//   SHORT AER_LIBENTRY aer_read_param (SHORT which); 
#endif
// WIN32 
   
//SHORT AER_LIBENTRY aer_save_param (CHAR *parameter); 
//SHORT AER_LIBENTRY aer_open_param (CHAR *parameter); 

// next 4 added v4.01, creates copy for prm editor in windows software 
SHORT AER_LIBENTRY write_param_edit (CHAR *sdata, SHORT copyfile);
SHORT AER_LIBENTRY read_param_edit (SHORT which, SHORT copyfile);
SHORT AER_LIBENTRY save_param_edit (CHAR *parameter, SHORT copyfile);
SHORT AER_LIBENTRY open_param_edit (CHAR *parameter, SHORT copyfile);

//SHORT AER_LIBENTRY aer_write_config (CHAR *sdata); 

#ifdef WIN32
   // messer is copied into supplied buffer 
//   SHORT AER_LIBENTRY aer_read_config (SHORT which, CHAR *psCfg); 
#else
//   SHORT AER_LIBENTRY aer_read_config (SHORT which); 
#endif
// WIN32 
   
//SHORT AER_LIBENTRY aer_save_config (CHAR *config); 
//SHORT AER_LIBENTRY aer_open_config (CHAR *config); 

LONG AER_LIBENTRY aer_check_initz (SHORT board);

AERERR_CODE AER_LIBENTRY aer_reset_intr (VOID);
AERERR_CODE AER_LIBENTRY aer_adjust_motion (LONG ax1, LONG ax2, LONG ax3, LONG ax4);
AERERR_CODE AER_LIBENTRY _aerq_psodpramwait (LONG *base , LONG timeout);
AERERR_CODE AER_LIBENTRY aerq_psodlarray (SHORT board, LONG *data, LONG timeout, LONG nPoints);
AERERR_CODE AER_LIBENTRY aer_set_atd (SHORT en_disable);
DOUBLE  AER_LIBENTRY aer_read_atd (SHORT atd_number);
AERERR_CODE AER_LIBENTRY aer_set_velocity_timebase (LONG minisec);
AERERR_CODE AER_LIBENTRY aer_set_velocity_tracking (SHORT axis_number, SHORT en_disable);
LONG  AER_LIBENTRY aer_read_velocity (SHORT axis_number);

LONG AER_LIBENTRY aer_DSPMemWrite (SHORT dspbase, SHORT *reterr,LONG dspmem,
   LONG mtype ,LONG data, LONG mode);
LONG AER_LIBENTRY aer_DSPMemRead (SHORT dspbase, SHORT *reterr, LONG dspmem,
   LONG mtype, LONG *dspmsw);

LONG AER_LIBENTRY aer_scope_command (LONG timeout , LONG cmd);
LONG AER_LIBENTRY aer_scope_dump (LONG timeout , LONG *scopebuf , LONG count, LONG *firstdata);


AERERR_CODE AER_LIBENTRY aer_ioread ( SHORT bank, LONG *data);
AERERR_CODE AER_LIBENTRY aer_ioreadbit ( SHORT bank, SHORT bitnum, LONG *data);
AERERR_CODE AER_LIBENTRY aer_iowrite    ( SHORT bank, LONG data);
AERERR_CODE AER_LIBENTRY aer_iowritebit ( SHORT bank, SHORT bitnum, LONG highlow);
AERERR_CODE AER_LIBENTRY aer_get_diag(BOOL bDebug, PAER_DIAG_RECV_PACKET pRecv );
AERERR_CODE AER_LIBENTRY aer_get_diag2(BOOL bDebug, PAER_DIAG2_RECV_PACKET pRecv );
AERERR_CODE AER_LIBENTRY aer_pso_get_diag(PLONG pRecv );
AERERR_CODE AER_LIBENTRY aer_get_firmware_info( PAER_FIRMWARE_INFO_PACKET pfi );

AERERR_CODE SendCmdVec (SHORT vec , LONG timeout , SHORT base);
LONG  ReadHost (LONG timeout , SHORT *errorcode , SHORT base);
AERERR_CODE WriteHost (LONG time, SHORT base, LONG senddata);
AERERR_CODE aer_set_pso_mem_ptr (CHAR *memPtr);

AERERR_CODE init_pso (char *firmware);

LONG AER_LIBENTRY  aer_calc_autotune_gains( float nsamptime, SHORT nsamples,
		       float servofreq, long *atunevel, long *atunetorq, float zeta,
		       float centerfreq, long *Kpos, long *Ki, long *Kp,
		       double *J_Kt, double *B_Kt, FLOAT alpha, long *aff, long mode );
/*
#ifdef NEW_AUTO_TUNE
	LONG AER_LIBENTRY  aer_calc_autotune_gains( float nsamptime, SHORT nsamples,
		       float servofreq, long *atunevel, long *atunetorq, float zeta,
		       float centerfreq, long *Kpos, long *Ki, long *Kp,
		       double *J_Kt, double *B_Kt, FLOAT alpha, long *aff  );
#else
	LONG AER_LIBENTRY  aer_calc_autotune_gains( float nsamptime, SHORT nsamples,
		       float servofreq, long *atunevel, long *atunetorq, float zeta,
		       float centerfreq, long *Kpos, long *Ki, long *Kp,
		       double *J_Kt, double *B_Kt  );
#endif
			   */

LONG AER_LIBENTRY aer_calc_freqresp( FLOAT nsamptime, SHORT nsamples,
		      SHORT numcycles, LONG *atunevel, LONG *atunevelcmd, FLOAT *ampl, FLOAT *phase,
		      SHORT *tph, SHORT *txng);

LONG        AER_LIBENTRY aer_get_skeyname( int skeynum  , char *name );
LONG        AER_LIBENTRY aer_skey_int( int skeynum );
LONG        AER_LIBENTRY aer_prog_int( int intnum );
LONG        AER_LIBENTRY aer_get_intmask( int intnum , long *intmask );
AERERR_CODE AER_LIBENTRY aer_read_ioport( LONG offset, LONG *val );
AERERR_CODE AER_LIBENTRY aer_comm_ready( VOID );
AERERR_CODE AER_LIBENTRY aer_send_step();
AERERR_CODE AER_LIBENTRY aer_loadcalfile( char *filename );
LONG        AER_LIBENTRY aer_get_msginfo ( SHORT prognum, SHORT idx);
LONG        AER_LIBENTRY aer_get_msgfile( SHORT prognum , CHAR **buf );
LONG        AER_LIBENTRY aer_get_waitmode( SHORT prognum )              ;
AERERR_CODE AER_LIBENTRY aer_set_waitmode( SHORT prognum, long mode )    ;
AERERR_CODE AER_LIBENTRY aer_get_qlib_error(char *psErr);
AERERR_CODE qlib_error( AERERR_CODE qliberr );
AERERR_CODE AER_LIBENTRY aer_set_prognum( short prognum );
AERERR_CODE AER_LIBENTRY aer_get_plane_flags( SHORT plane, LONG *flag );

#endif
