/*----------------------------------------------------------
   File Name      :  wapi
   Author         :  
   Creation Date  :  06-06-96
   Description    :  Encapsulation of U500 aerq_ functions to be
							built in a Windows NT DLL.
   Modifications  :  
------------------------------------------------------------
   Copyright (c) 1996 -- Aerotech Inc., Pittsburgh PA
   Aerotech Inc. makes no representation or warranties with   
   respect to the performance of this computer program, and          
   specifically disclaims any responsibility for any damages, special
   or consequential, connected with the use of this program.
   Permission is hereby granted to use in whole or in parts as you
   may see fit.
----------------------------------------------------------*/
#include <process.h>
#ifdef AEROCrtdbg
  #include <crtdbg.h>  // C-Runtime debugging 
#endif
#include <winnt.h>


#ifndef WAPI_INCLUDE
#define WAPI_INCLUDE

#ifdef __cplusplus
extern "C" {
#endif

//#ifndef NEW_AUTO_TUNE
//	#define NEW_AUTO_TUNE
//#endif


AERERR_CODE AER_LIBENTRY WAPIAerFaultAck (VOID);
AERERR_CODE AER_LIBENTRY WAPIAerFaultAckWait (SHORT waitflag);
AERERR_CODE AER_LIBENTRY WAPIAerInitialize (LPSTR lpsConfig, LPSTR lpsFirm, LPSTR lpsParam);
AERERR_CODE AER_LIBENTRY WAPIMXHDownloadFirmware (LPSTR lpsFirm);
AERERR_CODE AER_LIBENTRY WAPIMXHReadStatus ( LONG *status );
AERERR_CODE AER_LIBENTRY WAPIMXHEraseChip ( SHORT chip );
AERERR_CODE AER_LIBENTRY WAPIMXHReadLoc ( LONG addr, SHORT chip, LONG *data );
AERERR_CODE AER_LIBENTRY WAPIMXHWriteBlock ( LONG blksize, SHORT chip, ULONG startaddr, LONG *data );
AERERR_CODE AER_LIBENTRY WAPIMXHReturnData ( LONG blksize, LONG *pdatain );

LONG        AER_LIBENTRY WAPIAerInitLZR2100 (INT lzrbase, CHAR *lzrdatfile, INT *dsp_code);
LONG        AER_LIBENTRY WAPIAerInitializePSO ( LPSTR lpsFirm );
//AERERR_CODE AER_LIBENTRY WAPIAerInitializeU500 (LPSTR lpsConfig, LPSTR lpsFirm, LPSTR lpsParam); 
AERERR_CODE AER_LIBENTRY WAPIAerSaveConfig(LPSTR lpsConfigFile);
AERERR_CODE AER_LIBENTRY WAPIAerOpenConfig(LPSTR lpsConfigFile);
AERERR_CODE AER_LIBENTRY WAPIAerOpen (SHORT boardNum);
AERERR_CODE AER_LIBENTRY WAPIAerClose (SHORT nBoard);
LONG        AER_LIBENTRY WAPIAerCheckInitz (SHORT nBoard);
LONG AER_LIBENTRY WAPIGetBoardPointer( SHORT boardnum );

// user function prototypes 
AERERR_CODE AER_LIBENTRY WAPIAerCheckStatus (VOID);
AERERR_CODE AER_LIBENTRY WAPIAerPause (VOID);
AERERR_CODE AER_LIBENTRY WAPIAerSetPause (SHORT on_off); 
AERERR_CODE AER_LIBENTRY WAPIAerAbort (VOID);
AERERR_CODE AER_LIBENTRY WAPIAerSend (LPSTR lpsCmd);
AERERR_CODE AER_LIBENTRY WAPIAerSendGlbSub (LPSTR lpsCmd);
DOUBLE      AER_LIBENTRY WAPIAerReadPosition (SHORT nPos);
ULONG       AER_LIBENTRY WAPIAerReadStatus (SHORT nStatus);
LONG        AER_LIBENTRY WAPIAerSoftwareInitialize(CHAR *prmfile);
AERERR_CODE AER_LIBENTRY WAPIAerResetIntr (VOID);
LONG        AER_LIBENTRY WAPIAerRetry ();

// Program execution function prototypes 
LONG        AER_LIBENTRY WAPIAerGetProgAuxMsg (LPSTR text);
LONG        AER_LIBENTRY WAPIAerGetProgErrMsg (LPSTR text);
LONG        AER_LIBENTRY WAPIAerGetProgAuxCode(void);
LONG        AER_LIBENTRY WAPIAerGetProgAuxInfo(SHORT idx);
LONG        AER_LIBENTRY WAPIAerGetProgLine( void );
LONG        AER_LIBENTRY WAPIAerSetProgLine( LONG linenum );

//LONG        AER_LIBENTRY WAPIAerLoadGlbMSub (CHAR *gmsub); 
//LONG        AER_LIBENTRY WAPIAerLoadGlbSub (CHAR *gsub); 
//LONG        AER_LIBENTRY WAPIAerLoadProgram (CHAR *progName); 
LONG		  AER_LIBENTRY WAPIAerLoadProgram (CHAR *prog, CHAR *glbsub, CHAR *glbmsub );
//LONG        AER_LIBENTRY WAPIAerUnloadGlbMSub (VOID); 
//LONG        AER_LIBENTRY WAPIAerUnloadGlbSub (VOID); 
LONG        AER_LIBENTRY WAPIAerUnloadProgram (VOID);
AERERR_CODE AER_LIBENTRY WAPIAerStepProgram ();
LONG        AER_LIBENTRY WAPIAerGetProgName ( LPSTR fileName);
LONG        AER_LIBENTRY WAPIAerGetMsgInfo ( SHORT arraynum , SHORT idx , LPSTR bufp);
LONG AER_LIBENTRY WAPIAerSetProgNum( short prognum );
LONG AER_LIBENTRY WAPIAerRunProgram( short prognum, short nloop);
LONG AER_LIBENTRY WAPIAerStopProgram(short prognum);
LONG AER_LIBENTRY WAPIAerSuspendProgram( short prognum);
LONG AER_LIBENTRY WAPIAerSetProgramFaultMask(SHORT prognum, SHORT axis, ULONG errormask);


AERERR_CODE AER_LIBENTRY WAPIAerWrite8x3Iobus(UBYTE bank, UBYTE mask, UBYTE data );
AERERR_CODE AER_LIBENTRY WAPIAerRead8x3Iobus(UBYTE bank, UBYTE *data );
AERERR_CODE AER_LIBENTRY WAPIAerConfig8x3Iobus(UBYTE bank, UBYTE dir );
AERERR_CODE AER_LIBENTRY WAPIAerCheckStatus2(VOID);
DOUBLE      AER_LIBENTRY WAPIAerReadPosition2(SHORT nPos);
ULONG       AER_LIBENTRY WAPIAerReadStatus2(SHORT nStatus);



typedef struct AERPROGPACKET_
{
		long curline ;
		int pauseflag;			// wait for pause input bit		 
		int cycleflag;			// wait for cycle start input bit 
		int runmode;			// single or auto 
		int progdone;
		int maxprg;
		int curprg;
		char progname[80];			// program name 


} AERPROGPACKET;


LONG AER_LIBENTRY WAPIAerGetProgPacket(short prognum, AERPROGPACKET* progpacket);
LONG AER_LIBENTRY WAPIAerSetProgPacket(short prognum, AERPROGPACKET* progpacket);
LONG AER_LIBENTRY WAPIAerSetProgExecMode(short prognum, int runmode);


LONG        AER_LIBENTRY WAPIAerIORead ( SHORT bank, LONG *retvel);
LONG        AER_LIBENTRY WAPIAerIOReadBit ( SHORT bank, SHORT bitnum, LONG *retval);
AERERR_CODE AER_LIBENTRY WAPIAerIOWrite ( SHORT bank, LONG data);
LONG        AER_LIBENTRY WAPIAerIOWriteBit ( SHORT bank, SHORT bitnum, LONG highlow);
DOUBLE      AER_LIBENTRY WAPIAerReadAtd (SHORT atd_number);
LONG        AER_LIBENTRY WAPIAerReadAuxStatus (INT index);
DOUBLE      AER_LIBENTRY WAPIAerReadVariable (SHORT nVar);
DOUBLE      AER_LIBENTRY WAPIAerReadVelocity(SHORT axis);

LONG        AER_LIBENTRY WAPIAerScopeCommand (LONG timeout , LONG cmd);
LONG        AER_LIBENTRY WAPIAerScopeDump (LONG timeout, LONG *scopebuf, LONG count, LONG *firstdata);

LONG        AER_LIBENTRY WAPIAerSetAtd (SHORT en_disable);
LONG        AER_LIBENTRY WAPIAerSetPsoPtr (VOID);
LONG        AER_LIBENTRY WAPIAerSetVelocityTimebase (LONG millisec);
LONG        AER_LIBENTRY WAPIAerSetVelocityTracking (SHORT axis, SHORT en_disable);

LONG        AER_LIBENTRY WAPIAerDSPMemRead ( LONG *reterr, LONG dspmem, LONG mtype, LONG *dspmsw);
LONG        AER_LIBENTRY WAPIAerDSPMemWrite ( LONG far *reterr, LONG dspmem, LONG mtype, LONG data, LONG mode);
DOUBLE      AER_LIBENTRY WAPIAerUserMath (CHAR *newbufr, INT *newindex, INT *errorcode);

AERERR_CODE AER_LIBENTRY WAPICommReady (VOID);
AERERR_CODE AER_LIBENTRY WAPIErrorToAscii (AERERR_CODE error, LPSTR psErr);
LONG        AER_LIBENTRY WAPIGetAxis (SHORT nAxisNum, CHAR *pcAxis);
AERERR_CODE AER_LIBENTRY WAPIGetBoardType (VOID);
AERERR_CODE AER_LIBENTRY WAPIGetDiag    (ULONG *statword, SHORT debugflag);
AERERR_CODE AER_LIBENTRY WAPIGetDiag2 (ULONG *statword, SHORT debugflag);
AERERR_CODE AER_LIBENTRY WAPIPsoGetDiag (ULONG *statword );
LONG        AER_LIBENTRY WAPIGetModuleHandle (VOID);
LONG        AER_LIBENTRY WAPIGetPlane (SHORT nAxisNum);
DOUBLE		AER_LIBENTRY WAPIGetScaleFactor (SHORT nAxisNum);
LONG        AER_LIBENTRY WAPIGetProgInfo (SHORT nWhat);
LONG        AER_LIBENTRY WAPIGetUbFerd (SHORT nIndex);
LONG        AER_LIBENTRY WAPIGetUbJ (SHORT nIndex);
LONG        AER_LIBENTRY WAPILoadString (INT id, CHAR *bufaddr, INT bufsize);

LONG        AER_LIBENTRY WAPIAerAdjustMotion (DOUBLE ldAxis1, DOUBLE ldAxis2, DOUBLE ldAxis3, DOUBLE ldAxis4);

LONG        AER_LIBENTRY WAPIAerCalcAutotuneGains (FLOAT nsamptime, SHORT nsamples,
			      FLOAT servofreq, LONG *atunevel, LONG *atunetorq, FLOAT zeta,
			      FLOAT centerfreq, LONG *Kpos, LONG *Ki, LONG *Kp,
			      DOUBLE *J_Kt, DOUBLE *B_Kt, FLOAT alpha, LONG *Aff, LONG mode);
/*
#ifdef NEW_AUTO_TUNE
LONG        AER_LIBENTRY WAPIAerCalcAutotuneGains (FLOAT nsamptime, SHORT nsamples,
			      FLOAT servofreq, LONG *atunevel, LONG *atunetorq, FLOAT zeta,
			      FLOAT centerfreq, LONG *Kpos, LONG *Ki, LONG *Kp,
			      DOUBLE *J_Kt, DOUBLE *B_Kt, FLOAT alpha, LONG *Aff);
#else
LONG        AER_LIBENTRY WAPIAerCalcAutotuneGains (FLOAT nsamptime, SHORT nsamples,
			      FLOAT servofreq, LONG *atunevel, LONG *atunetorq, FLOAT zeta,
			      FLOAT centerfreq, LONG *Kpos, LONG *Ki, LONG *Kp,
			      DOUBLE *J_Kt, DOUBLE *B_Kt);
#endif
				  */

LONG				AER_LIBENTRY WAPIAerCalcFreqResp (FLOAT nsamptime, SHORT nsamples, SHORT numcycles,
						LONG *atunevel, LONG *atunevelcmd, FLOAT *ampl, FLOAT *phase,
						SHORT *tph, SHORT *txng);

LONG AER_LIBENTRY WAPIAerGetPauseFlag( VOID );
LONG AER_LIBENTRY WAPIAerGetCycleFlag( VOID );
LONG AER_LIBENTRY WAPIAerGetSkeyName( SHORT skeynum  , CHAR *name );
LONG AER_LIBENTRY WAPIAerSkeyInt( SHORT skeynum );
LONG AER_LIBENTRY WAPIAerGetIntMask( SHORT intnum , LONG *intmask );
LONG AER_LIBENTRY WAPIAerProgInt( SHORT intnum );
LONG AER_LIBENTRY WAPIAerSendStep( );
LONG AER_LIBENTRY WAPIAerReadiSBX( LONG address , LONG *data );
LONG AER_LIBENTRY WAPIAerWriteiSBX( LONG address, SHORT bitvalid, SHORT bit, LONG value );
LONG AER_LIBENTRY WAPILoadCalFile( CHAR *file );

AERERR_CODE AER_LIBENTRY WAPIAerOpenParam (LPSTR paramFile);
AERERR_CODE AER_LIBENTRY WAPIAerSaveParam (LPSTR paramFile);
AERERR_CODE AER_LIBENTRY WAPIAerReadParam (SHORT nParam, LPSTR lpsResult);
AERERR_CODE AER_LIBENTRY WAPIAerWriteParam (LPSTR lpsParamStr);
AERERR_CODE AER_LIBENTRY WAPIAerReadAxcal (LPSTR configFile, LPSTR axcalFile);
AERERR_CODE AER_LIBENTRY WAPIAerSaveAxcal (LPSTR origconfigFile, LPSTR newconfigfile, LPSTR axcalFile);
LONG        AER_LIBENTRY WAPIAerGetParamSize( VOID );

AERERR_CODE AER_LIBENTRY WAPIAerOpenConfigLabview( LPSTR config, LPSTR axcalfile );

AERERR_CODE AER_LIBENTRY WAPIAerGetQlibVersion( short *maj, short *min );
AERERR_CODE AER_LIBENTRY WAPIAerGetDrvVersion( short *maj, short *min );
AERERR_CODE AER_LIBENTRY WAPIAerGetWapiVersion( short *maj, short *min );
AERERR_CODE AER_LIBENTRY WAPIAerGetFirmwareInfo(long *plInfo);
AERERR_CODE AER_LIBENTRY WAPIAerAxisErrorToAscii( ULONG axiserror, char *psErr );
AERERR_CODE AER_LIBENTRY WAPIAerGetQlibError( char *psErr);
AERERR_CODE AER_LIBENTRY WAPIAerFindMarker(short axis);

SHORT AER_LIBENTRY WAPIAerComputeFFT(short NumSamples, double *input, double *realout, double *imagout);
AERERR_CODE AER_LIBENTRY WAPIAerCreateDefaultParamFile (LPSTR paramFile);
//SHORT AER_LIBENTRY WAPIAerCheckDefaultParam(SHORT *params, SHORT paramnum);
AERERR_CODE AER_LIBENTRY WAPIAerCheckDefaultParam(SHORT paramnum);
AERERR_CODE AER_LIBENTRY WAPIAerGetDefaultCfg(SHORT nParam, LPSTR lpsResult);
AERERR_CODE AER_LIBENTRY WAPIAerCreateDefaultCfgFile (LPSTR cfgFile);

DOUBLE AER_LIBENTRY WAPIAerReadPlaneFeedrate(short plane);

AERERR_CODE AER_LIBENTRY WAPIAerEditFlash( LONG *retcode );
AERERR_CODE AER_LIBENTRY WAPIAerSaveFlash( LONG *retcode );
AERERR_CODE AER_LIBENTRY WAPIAerReadFpgaSetupCode(LONG fpganum, LONG *setupcode );
AERERR_CODE AER_LIBENTRY WAPIAerWriteFpgaSetupCode(LONG fpganum, LONG setupcode );
AERERR_CODE AER_LIBENTRY WAPIAerReadOptionCode(LONG optionnum, LONG *option );
SHORT AER_LIBENTRY WAPIAerGetBusType( );

AERERR_CODE AER_LIBENTRY WAPIAerPosgrabScanEnable( void );
AERERR_CODE AER_LIBENTRY WAPIAerPosgrabHardwareEnable( void );
AERERR_CODE AER_LIBENTRY WAPIAerPosgrabDisable( void );
AERERR_CODE AER_LIBENTRY WAPIAerPosgrabReadIndex( LONG *index );
AERERR_CODE AER_LIBENTRY WAPIAerPosgrabResetIndex( void );
AERERR_CODE AER_LIBENTRY WAPIAerPosgrabReadArraySize( LONG *size );
AERERR_CODE AER_LIBENTRY WAPIAerPosgrabReadPosition( LONG idx, double *pos );

AERERR_CODE AER_LIBENTRY WAPIAerSingleSetup( LONG load, LONG cmd, LONG units, LONG *args );
AERERR_CODE AER_LIBENTRY WAPIAerDualSetup( LONG load, LONG cmd, LONG units, LONG *args );
AERERR_CODE AER_LIBENTRY WAPIAerLoadArray( LONG *array, LONG u500idx, LONG npnts );
AERERR_CODE AER_LIBENTRY WAPIAerAnalogVector( LONG cmd, LONG units, SHORT axes,
		SHORT dacnum, double minvolt, double maxvolt , double maxvelpos, long sampletime );
AERERR_CODE AER_LIBENTRY WAPIAerInPosition( SHORT axes, double time, LONG port, LONG bit);
AERERR_CODE AER_LIBENTRY WAPIAerQueuePosCompare( short axis, LONG position, LONG opcode );

AERERR_CODE AER_LIBENTRY WAPIAerReadIOPort( LONG offset, LONG *val );
AERERR_CODE AER_LIBENTRY WAPIAerWriteIOPort( SHORT offset, SHORT data );

AERERR_CODE AER_LIBENTRY WAPIAerCreateEvent( PHANDLE phEvent, BOOL bReclaim );
AERERR_CODE AER_LIBENTRY WAPIAerCloseEvent( HANDLE hEvent );
AERERR_CODE AER_LIBENTRY WAPIAerCreateThreadEx( PVOID pfnCallBack, PVOID pvData, BOOL bReclaim );
AERERR_CODE AER_LIBENTRY WAPIAerTerminateThreadEx( );
AERERR_CODE AER_LIBENTRY WAPIAerSetEnvComp(SHORT mode, SHORT axis, SHORT analog_mask,
										   DOUBLE set_temp, DOUBLE set_pressure, DOUBLE set_humidity);
AERERR_CODE AER_LIBENTRY WAPIAerLoadEnvCompFile(CHAR *ecfile);
double AER_LIBENTRY WAPIAerConv48ToDouble(long msw, long lsw);
AERERR_CODE AER_LIBENTRY WAPIAerGetPlaneReady(LONG *status);

//*********************************************************************
//	Fiber Alignment Routines
//*********************************************************************
void AER_LIBENTRY WAPIAerFiberFreeMatrix(VOID);
AERERR_CODE AER_LIBENTRY WAPIAerFiberGetSavedData1D(double *column_data, long column);
AERERR_CODE AER_LIBENTRY WAPIAerFiberGetNumPnts(long *numpnts);
AERERR_CODE AER_LIBENTRY WAPIAerFiberGetGeoCoord(double *axis1_coord, double *axis2_coord);
AERERR_CODE AER_LIBENTRY WAPIAerFiberGetAdval(double *adval);
AERERR_CODE AER_LIBENTRY WAPIAerFiberGetParam(long routine, long paramnum, double *value);
AERERR_CODE AER_LIBENTRY WAPIAerFiberGetDefaultParam(long routine, long paramnum, double *value);
AERERR_CODE AER_LIBENTRY WAPIAerFiberSetParam(long routine, long paramnum, double value);
AERERR_CODE AER_LIBENTRY WAPIAerFiberSetDataSaveOn(long datasave_on);


AERERR_CODE AER_LIBENTRY WAPIAerFiberHillClimb(VOID);
AERERR_CODE AER_LIBENTRY WAPIAerFiberSpiralFine(VOID);
AERERR_CODE AER_LIBENTRY WAPIAerFiberSpiralRough(VOID);
AERERR_CODE AER_LIBENTRY WAPIAerFiberFastAlign(VOID);
AERERR_CODE AER_LIBENTRY WAPIAerFiberFastAlign3D(VOID);
AERERR_CODE AER_LIBENTRY WAPIAerFiberFastAlign4D(VOID);
AERERR_CODE AER_LIBENTRY WAPIAerFiberGeoCenter2D(VOID);
AERERR_CODE AER_LIBENTRY WAPIAerFiberCentroid1D(VOID);
AERERR_CODE AER_LIBENTRY WAPIAerFiberCentroid2D(VOID);
AERERR_CODE AER_LIBENTRY WAPIAerFiberCentroid3D(VOID);
long AER_LIBENTRY WAPIAerFiberCheckStatus(DWORD timeout, AERERR_CODE *error_code);
AERERR_CODE AER_LIBENTRY WAPIAerFiberSetSleepMode(long mode, long sleepTime);
// GPIB specific functions

AERERR_CODE AER_LIBENTRY WAPIGPIBSetParams(long devnum, long bdindx, long pad, long sad, long tmo, long eot, long eos);
AERERR_CODE AER_LIBENTRY WAPIGPIBSetDev(long devnum, long mode, long *Dev, long *iberror, long *ibstat);
//AERERR_CODE AER_LIBENTRY WAPIGPIBSetOn_Off(long devnum, long on_off);
AERERR_CODE AER_LIBENTRY WAPIGPIBSetWriteString(long devnum, char *wstring);
AERERR_CODE AER_LIBENTRY WAPIGPIBSetBytesToRead(long devnum, long bytes);
AERERR_CODE AER_LIBENTRY WAPIGPIBSetDevOffline(long devnum, long *iberror, long *ibstat);
AERERR_CODE AER_LIBENTRY WAPIGPIBClose();
AERERR_CODE AER_LIBENTRY WAPIGPIBOpen();
AERERR_CODE AER_LIBENTRY WAPIGPIBWriteString(long devnum, char *wstring, long *iberror, long *ibstat);
AERERR_CODE AER_LIBENTRY WAPIGPIBReadString(long devnum, char *rstring, long numbytes, long *iberror, long *ibstat);
AERERR_CODE AER_LIBENTRY WAPIGPIBReadNumber(long devnum, double *value, long numbytes, long *iberror, long *ibstat);

//*********************************************************************


AERERR_CODE AER_LIBENTRY WAPIAerImmedRead(LONG opcode, LONG offset, LONG *ret, DOUBLE *dret );
AERERR_CODE AER_LIBENTRY WAPIAerWaitForInterrupt( DWORD timeout );

AERERR_CODE AER_LIBENTRY WAPIAerVBDblToPreciseString( DOUBLE dValue, DWORD dwPrecision, LPSTR psString );
AERERR_CODE AER_LIBENTRY WAPIAerSetDemoMode( LONG on_off);
#ifdef WIN32
AERERR_CODE AER_LIBENTRY WAPIAerReadRegistry( PREGSTRUCT pReg );
AERERR_CODE AER_LIBENTRY WAPIAerWriteRegistry( PREGSTRUCT pReg );
#endif

AERERR_CODE AER_LIBENTRY WAPIAerSetSecondaryAxisPosition( SHORT axis, SHORT mode, DOUBLE *pos);

#ifdef __cplusplus
}
#endif 
 

#endif         // WAPI_INCLUDE 
