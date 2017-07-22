Attribute VB_Name = "Winaer"

Declare Function WAPIAerOpen Lib "WIN50032.dll" (ByVal boardnum As Integer) As Long
Declare Function WAPIAerClose Lib "WIN50032.dll" (ByVal boardnum As Integer) As Long
Declare Function WAPIAerInitialize Lib "WIN50032.dll" (ByVal lpsConfig As String, ByVal lpsFirm As String, ByVal lpsParam As String) As Long
Declare Function WAPIAerCheckInitz Lib "WIN50032.dll" (ByVal nBoard As Integer) As Long
Declare Function WAPIAerInitializePSO Lib "WIN50032.dll" (ByVal lpsFirm As String) As Long

Declare Function WAPIAerSend Lib "WIN50032.dll" (ByVal lpsCmd As String) As Long
Declare Function WAPIAerAbort Lib "WIN50032.dll" () As Long
Declare Function WAPIAerPause Lib "WIN50032.dll" () As Long
Declare Function WAPIAerFaultAck Lib "WIN50032.dll" () As Long
Declare Function WAPIAerFaultAckWait Lib "WIN50032.dll" (ByVal waitflag As Integer) As Long

Declare Function WAPIAerReadVariable Lib "WIN50032.dll" (ByVal nVar As Integer) As Double

Declare Function WAPIAerReadPlaneFeedrate Lib "WIN50032.dll" (ByVal nPlane As Integer) As Double

Declare Function WAPIAerCheckStatus Lib "WIN50032.dll" () As Long
Declare Function WAPIAerReadPosition Lib "WIN50032.dll" (ByVal nPos As Integer) As Double
Declare Function WAPIAerReadStatus Lib "WIN50032.dll" (ByVal nStatus As Integer) As Long

Declare Function WAPIAerResetIntr Lib "WIN50032.dll" () As Long

Declare Function WAPIGetPlane Lib "WIN50032.dll" (ByVal nAxisNum As Integer) As Long
Declare Function WAPIGetScaleFactor Lib "WIN50032.dll" (ByVal nAxisNum As Integer) As Double
Declare Function WAPIGetAxis Lib "WIN50032.dll" (ByVal nAxisNum As Integer, ByVal pcAxis As String) As Long
Declare Function WAPIGetProgInfo Lib "WIN50032.dll" (ByVal nWhat As Integer) As Long
Declare Function WAPIGetUbFerd Lib "WIN50032.dll" (ByVal nIndex As Integer) As Long
Declare Function WAPIGetUbJ Lib "WIN50032.dll" (ByVal nIndex As Integer) As Long
Declare Function WAPIAerGetMsgInfo Lib "WIN50032.dll" (ByVal arraynum As Integer, ByVal idx As Integer, ByVal msgbuf As String) As Long
Declare Function WAPIGetBoardType Lib "WIN50032.dll" () As Long
Declare Function WAPIGetDiag Lib "WIN50032.dll" (statword As Long, ByVal debugflag As Integer) As Long
Declare Function WAPIGetDiag2 Lib "WIN50032.dll" (statword As Long, ByVal debugflag As Integer) As Long

Declare Function WAPICommReady Lib "WIN50032.dll" () As Long

Declare Function WAPIAerSetAtd Lib "WIN50032.dll" (ByVal en_disable As Integer) As Long
Declare Function WAPIAerReadAtd Lib "WIN50032.dll" (ByVal atd_number As Integer) As Double
Declare Function WAPIAerSetVelocityTimebase Lib "WIN50032.dll" (ByVal millisec As Long) As Long
Declare Function WAPIAerSetVelocityTracking Lib "WIN50032.dll" (ByVal axis As Integer, ByVal en_disable As Integer) As Long
Declare Function WAPIAerReadVelocity Lib "WIN50032.dll" (ByVal axis As Integer) As Double

Declare Function WAPIAerScopeCommand Lib "WIN50032.dll" (ByVal timeout As Long, ByVal acmd As Long) As Long
Declare Function WAPIAerScopeDump Lib "WIN50032.dll" (ByVal timeout As Long, scopedata As Long, ByVal numpnts As Long, firstdata As Long) As Long

Declare Function WAPIAerDSPMemWrite Lib "WIN50032.dll" (reterr As Long, ByVal dspmem As Long, ByVal mtype As Long, ByVal bdata As Long, ByVal mode As Long) As Long
Declare Function WAPIAerDSPMemRead Lib "WIN50032.dll" (reterr As Long, ByVal dspmem As Long, ByVal mtype As Long, dspmsw As Long) As Long

'Declare Function WAPIAerCalcAutotuneGains Lib "WIN50032.dll" (ByVal nsamptime As Single, _
'  ByVal nsamples As Integer, ByVal servofreq As Single, firstvel As Long, firsttorq As Long, ByVal zeta As Single, ByVal centerfreq As Single, kpos As Long, ki As Long, kp As Long, J_Kt As Double, B_Kt As Double) As Long

Declare Function WAPIAerCalcAutotuneGains Lib "WIN50032.dll" (ByVal nsamptime As Single, _
  ByVal nsamples As Integer, ByVal servofreq As Single, firstvel As Long, firsttorq As Long, ByVal zeta As Single, ByVal centerfreq As Single, kpos As Long, ki As Long, kp As Long, J_Kt As Double, B_Kt As Double, _
        ByVal alpha As Single, aff As Long, ByVal mode As Long) As Long
  
Declare Function WAPIAerCalcFreqResp Lib "WIN50032.dll" (ByVal nsamptime As Single, _
  ByVal nsamples As Integer, ByVal numcycles As Integer, firstvel As Long, firstvelcmd As Long, ampl As Single, phase As Single, _
  totphase As Integer, totxings As Integer) As Long

Declare Function WAPILoadString Lib "WIN50032.dll" (ByVal Id As Integer, ByVal bufstring As String, ByVal bufsize As Integer) As Long
Declare Function WAPIGetModuleHandle Lib "WIN50032.dll" () As Long

Declare Function WAPIAerIOWrite Lib "WIN50032.dll" (ByVal bank As Integer, ByVal ldata As Long) As Long
Declare Function WAPIAerIOWriteBit Lib "WIN50032.dll" (ByVal bank As Integer, ByVal bitnum As Integer, ByVal highlow As Long) As Long
Declare Function WAPIAerIORead Lib "WIN50032.dll" (ByVal bank As Integer, ByRef retvel As Long) As Long
Declare Function WAPIAerIOReadBit Lib "WIN50032.dll" (ByVal bank As Integer, ByVal bitnum As Integer, ByRef retvel As Long) As Long

Declare Function WAPIErrorToAscii Lib "WIN50032.dll" (ByVal error As Long, ByVal psErr As String) As Long

Declare Function WAPIAerLoadProgram Lib "WIN50032.dll" (ByVal prog As String, ByVal glbsub As String, ByVal glbmsub As String) As Long
'Declare Function WAPIAerLoadGlbSub Lib "WIN50032.dll" (ByVal progName As String) As Long
'Declare Function WAPIAerLoadGlbMSub Lib "WIN50032.dll" (ByVal progName As String) As Long

Declare Function WAPIAerUnloadProgram Lib "WIN50032.dll" () As Long
'Declare Function WAPIAerUnloadGlbSub Lib "WIN50032.dll" () As Long
'Declare Function WAPIAerUnloadGlbMSub Lib "WIN50032.dll" () As Long

Declare Function WAPIAerStepProgram Lib "WIN50032.dll" () As Long

'Declare Function WAPIAerGetPrgMsg Lib "U500WapiW32.dll" (ByVal prgExe As Long,ByVal typeData As Integer,ByVal text As String) As Long

Declare Function WAPIAerGetProgAuxMsg Lib "WIN50032.dll" (ByVal text As String) As Long
Declare Function WAPIAerGetProgErrMsg Lib "WIN50032.dll" (ByVal text As String) As Long
Declare Function WAPIAerGetProgAuxCode Lib "WIN50032.dll" () As Long
Declare Function WAPIAerGetProgAuxInfo Lib "WIN50032.dll" (ByVal Index As Integer) As Long
Declare Function WAPIAerGetProgLine Lib "WIN50032.dll" () As Long
Declare Function WAPIAerSetProgLine Lib "WIN50032.dll" (ByVal line As Long) As Long
Declare Function WAPIAerGetProgName Lib "WIN50032.dll" (ByVal filename As String) As Long

'Declare Function WAPIAerGetPrgBlk Lib "Win50032.dll" () As Long
'Declare Function WAPIAerGetPrgExeInfo Lib "win50032.dll" (ByVal prgExe As Long, ByVal typeData As Long) As Long
'Declare Function WAPIAerSetPrgExeInfo Lib "win50032.dll" (ByVal prgExe As Long, ByVal typeData As Long, ByVal data As Long) As Long

Declare Function WAPIAerConsoleWrite Lib "WIN50032.dll" (ByVal text As String) As Long

Declare Function WAPIAerGetPauseFlag Lib "WIN50032.dll" () As Long
Declare Function WAPIAerGetCycleFlag Lib "WIN50032.dll" () As Long
Declare Function WAPIAerGetSkeyName Lib "WIN50032.dll" (ByVal skeynum As Integer, ByVal name As String) As Long
Declare Function WAPIAerSkeyInt Lib "WIN50032.dll" (ByVal skeynum As Integer) As Long
Declare Function WAPIAerGetIntMask Lib "WIN50032.dll" (ByVal intnum As Integer, intmask As Long) As Long
Declare Function WAPIAerProgInt Lib "WIN50032.dll" (ByVal intnum As Integer) As Long
Declare Function WAPIAerReadiSBX Lib "WIN50032.dll" (ByVal addr As Long, data As Long) As Integer
Declare Function WAPIAerSendStep Lib "WIN50032.dll" () As Long

Declare Function WAPIAerSoftwareInitialize Lib "WIN50032.dll" (ByVal lpsParam As String) As Long

Global Const PRM_SIZE = 1000
'Global Const PRM_SIZE = 890
'Global Const PRM_SIZE = 906

Declare Function WAPIPrmSave Lib "WIN50032.dll" _
   (ByRef params As Integer, ByVal filename As String) As Long

Declare Function WAPIPrmOpen Lib "WIN50032.dll" _
   (ByRef params As Integer, ByVal filename As String) As Long

Declare Function WAPIPrmWrite Lib "WIN50032.dll" _
   (ByRef params As Integer, ByVal paramnum As Integer, ByVal paramval As String) As Long

Declare Function WAPIPrmRead Lib "Win50032.dll " _
   (ByRef params As Integer, ByVal paramnum As Integer, ByVal paramval As String) As Long

Declare Function WAPIAerCheckDefaultParam Lib "WIN50032.dll" _
  (ByVal nParam As Integer) As Long
  
Declare Function WAPIAerGetDefaultCfg Lib "WIN50032.dll" _
    (ByVal nParam As Integer, ByVal Result As String) As Long
  
Declare Function WAPIAerCreateDefaultParamFile Lib "WIN50032.dll" _
    (ByVal lpsParamStr As String) As Long
    
Declare Function WAPIAerCreateDefaultCfgFile Lib "WIN50032.dll" _
    (ByVal lpsCfgStr As String) As Long

Declare Function WAPIPrmCopy Lib "Win50032.dll " _
   (ByRef params As Integer, ByVal filename As String) As Long

Declare Function WAPIAerWriteParam Lib "WIN50032.dll" (ByVal lpsParamStr As String) As Long
Declare Function WAPIAerReadParam Lib "WIN50032.dll" (ByVal nParam As Integer, ByVal lpResult As String) As Long
Declare Function WAPIAerOpenParam Lib "WIN50032.dll" (ByVal lpsParamFile As String) As Long
Declare Function WAPIAerSaveParam Lib "WIN50032.dll" (ByVal lpsParamFile As String) As Long
Declare Function WAPIAerCopyParamEdit Lib "WIN50032.dll" (ByRef params As Integer, ByVal lpsParamFile As String, ByVal copyfile As Integer) As Long
Declare Function WAPIAerCloseAllParamBufr Lib "WIN50032.dll" () As Long

Declare Function WAPIAerReadAxcal Lib "WIN50032.dll" (ByVal configfile As String, ByVal axcalfile As String) As Integer

Declare Function WAPIAerSaveAxcal Lib "WIN50032.dll" (ByVal origconfigfile As String, ByVal newconfigfile As String, ByVal axcalfile As String) As Integer


Declare Function WAPILoadCalFile Lib "WIN50032.dll" (ByVal calfile As String) As Long

Declare Function WAPIAerWriteConfig Lib "WIN50032.dll" (ByVal lpsConfigStr As String) As Long
Declare Function WAPIAerReadConfig Lib "WIN50032.dll" (ByVal nConfig As Integer, ByVal lpResult As String) As Long
Declare Function WAPIAerOpenConfig Lib "WIN50032.dll" (ByVal lpsConfigFile As String) As Long
Declare Function WAPIAerSaveConfig Lib "WIN50032.dll" (ByVal lpsConfigFile As String) As Long

Declare Function WAPIAerGetQlibVersion Lib "WIN50032.dll" (maj As Integer, min As Integer) As Long
Declare Function WAPIAerGetDrvVersion Lib "WIN50032.dll" (maj As Integer, min As Integer) As Long
Declare Function WAPIAerGetWapiVersion Lib "WIN50032.dll" (maj As Integer, min As Integer) As Long
Declare Function WAPIAerGetFirmwareInfo Lib "WIN50032.dll" (firminfo As Long) As Long

Declare Function WAPIAerAxisErrorToAscii Lib "WIN50032.dll" (ByVal axiserror As Long, ByVal psErr As String) As Long
Declare Function WAPIAerGetQlibError Lib "WIN50032.dll" (ByVal psErr As String) As Long

Global Const DSPMEMCMD = &H1B   ' dsp memory acces host command vector
Global Const DSPMEMTIMEOUT = 100000    ' 1 is at least 1 micro sec ( 1 AT I/O bus access )

Global Const DSPMEMX = &H10000           ' DSP memory access flags
Global Const DSPMEMY = &H20000
Global Const DSPMEMP = &H40000
Global Const DSPMEML = &H80000

Global Const DSPMEMREAD = &H0            ' DSP memory functions / commands
Global Const DSPMEMWRITE = &H100000
Global Const DSPMEMAND = &H200000
Global Const DSPMEMOR = &H300000
Global Const DSPMEMKILL = &H400000       ' command to stop host transmitter int


Declare Function WAPIAerCopyParam Lib "WIN50032.dll" _
(ByRef paramdata As Integer, ByRef params As Integer) As Long


Type boardentry
   iobase As Long
   timeout As Long
   psoiobase As Long
   psotimeout As Long
   interrupt As Long
End Type
  
Type reg
   BOARD(3) As boardentry
   lzrboard As Long
   boardtype As Long
End Type


Declare Function WAPIAerReadRegistry Lib "WIN50032.dll" (u500reg As reg) As Long
Declare Function WAPIAerWriteRegistry Lib "WIN50032.dll" (u500reg As reg) As Long
Declare Function LZR_Initialize Lib "LZRDLL32.dll" (ByVal address As String, _
        ByVal LZR2100 As String, ByVal LZR1100 As String) As Integer
Declare Sub LZRStop Lib "LZRDLL32.dll" ()
  
  
Declare Function WAPIAerComputeFFT Lib "WIN50032.dll" (ByVal NumSamples _
    As Integer, RealIn As Double, realout As Double, imagout As Double) As Integer

Declare Function WAPIAerSetProgNum Lib "WIN50032.dll" (ByVal prognum As Integer) As Long
Declare Function WAPIAerRunProgram Lib "WIN50032.dll" (ByVal prognum As Integer, ByVal prg_loopnum As Integer) As Long
Declare Function WAPIAerStopProgram Lib "WIN50032.dll" (ByVal prognum As Integer) As Long
Declare Function WAPIAerResumeProgram Lib "WIN50032.dll" (ByVal prognum As Integer) As Long
Declare Function WAPIAerSuspendProgram Lib "WIN50032.dll" (ByVal prognum As Integer) As Integer
Declare Function WAPIAerSetProgExecMode Lib "WIN50032.dll" (ByVal prognum As Integer, ByVal mode As Integer) As Long
Declare Function WAPIAerSetProgramFaultMask Lib "WIN50032.dll" (ByVal prognum As Integer, _
                ByVal axis As Integer, ByVal errormask As Long) As Long
                
Type AerProgRecvPacket
    curline As Long
    pauseflag As Long          ' wait for pause input bit
    cycleflag As Long          ' wait for cycle start input bit
    runmode As Long            ' single or auto
    progdone As Long           ' flag signaling end of progam executiion
    maxprg As Long             ' reserved
    curprg As Long             ' reserved
    progname As String * 255    ' program name  - was 80
End Type

Declare Function WAPIAerGetProgPacket Lib "WIN50032.dll" (ByVal prognum As Integer, _
             progpacket As AerProgRecvPacket) As Long

Declare Function WAPIAerSetProgPacket Lib "WIN50032.dll" (ByVal prognum As Integer, _
             progpacket As AerProgRecvPacket) As Long
             
Declare Function WAPIAerGetBusType Lib "WIN50032.dll" () As Long

Declare Function WAPIAerEditFlash Lib "WIN50032.dll" (retcode As Long) As Long

Declare Function WAPIAerSaveFlash Lib "WIN50032.dll" (retcode As Long) As Long

Declare Function WAPIAerReadFpgaSetupCode Lib "WIN50032.dll" (ByVal fpganum _
            As Long, setupcode As Long) As Long

Declare Function WAPIAerWriteFpgaSetupCode Lib "WIN50032.dll" (ByVal fpganum _
            As Long, ByVal setupcode As Long) As Long

Declare Function WAPIAerReadOptionCode Lib "WIN50032.dll" (ByVal optionnum _
            As Long, Option1 As Long) As Long

Declare Function WAPIAerSingleSetup Lib "WIN50032.dll" (ByVal load As Long, _
                ByVal cmd As Long, ByVal Units As Long, args As Long) As Long

Declare Function WAPIAerDualSetup Lib "WIN50032.dll" (ByVal load As Long, _
                ByVal cmd As Long, ByVal Units As Long, args As Long) As Long

Declare Function WAPIAerAnalogVector Lib "WIN50032.dll" (ByVal cmd As Long, _
                ByVal Units As Long, ByVal axes As Integer, ByVal dacnum As Integer, _
                ByVal minvolt As Double, ByVal maxvolt As Double, _
                ByVal maxvelpos As Double) As Long

Declare Function WAPIAerPosgrabScanEnable Lib "WIN50032.dll" () As Long
Declare Function WAPIAerPosgrabHardwareEnable Lib "WIN50032.dll" () As Long
Declare Function WAPIAerPosgrabDisable Lib "WIN50032.dll" () As Long
Declare Function WAPIAerPosgrabResetIndex Lib "WIN50032.dll" () As Long
Declare Function WAPIAerPosgrabReadIndex Lib "WIN50032.dll" (Index As Long) As Long
Declare Function WAPIAerPosgrabReadArraySize Lib "WIN50032.dll" (size As Long) As Long
Declare Function WAPIAerPosgrabReadPosition Lib "WIN50032.dll" (ByVal idx As Long, _
                                pos As Long) As Long

Declare Function WAPIAerInPosition Lib "WIN50032.dll" (ByVal axes As Integer, _
                                ByVal time As Double, ByVal port As Long, _
                                ByVal bit As Long) As Long

Declare Function WAPIAerQueuePosCompare Lib "WIN50032.dll" (ByVal axis As Integer, _
                                ByVal POSITION As Long, ByVal opcode As Long) As Long

Declare Function WAPIAerLoadArray Lib "WIN50032.dll" (ByRef data_array As Long, _
                        ByVal u500idx As Long, ByVal npnts As Long) As Long

Declare Function WAPIAerLoadEnvCompFile Lib "WIN50032.dll" (ByVal LZR1100CalFile _
                    As String) As Long

Declare Function WAPIAerConv48ToDouble Lib "WIN50032.dll" (ByVal msw As Long, _
                    ByVal lsw As Long) As Double
                    
Declare Function WAPIAerSendGlbSub Lib "WIN50032.dll" (ByVal lpsCmd As String) As Long

Declare Function WAPIAerFiberHillClimb Lib "WIN50032.dll" () As Long
Declare Function WAPIAerFiberSpiralFine Lib "WIN50032.dll" () As Long
Declare Function WAPIAerFiberSpiralRough Lib "WIN50032.dll" () As Long
Declare Function WAPIAerFiberFastAlign Lib "WIN50032.dll" () As Long
Declare Function WAPIAerFiberFastAlign3D Lib "WIN50032.dll" () As Long
Declare Function WAPIAerFiberGeoCenter2D Lib "WIN50032.dll" () As Long
Declare Function WAPIAerFiberCheckStatus Lib "WIN50032.dll" (ByVal timeout As Long, _
                    ByRef error_code As Long) As Long
Declare Function WAPIAerFiberSetParam Lib "WIN50032.dll" (ByVal routine As Long, _
                    ByVal paramnum As Long, ByVal Value As Double) As Long
Declare Function WAPIAerFiberGetParam Lib "WIN50032.dll" (ByVal routine As Long, _
                    ByVal paramnum As Long, ByRef Value As Double) As Long
Declare Function WAPIAerFiberGetDefaultParam Lib "WIN50032.dll" (ByVal routine As Long, _
                    ByVal paramnum As Long, ByRef Value As Double) As Long
Declare Function WAPIAerFiberSetDataSaveOn Lib "WIN50032.dll" (ByVal Data_Save_On _
                    As Long) As Long
Declare Function WAPIAerFiberGetAdVal Lib "WIN50032.dll" (ByRef adval As Double) As Long
Declare Function WAPIAerFiberGetGeoCoord Lib "WIN50032.dll" (ByRef axis1_coord As Double, _
                    ByRef axis2_coord As Double) As Long
Declare Function WAPIAerFiberGetNumPnts Lib "WIN50032.dll" (ByRef num_points As Long) As Long
Declare Function WAPIAerFiberGetSavedData1D Lib "WIN50032.dll" (ByRef column_data As Double, _
                    ByVal column As Long) As Long
Declare Sub WAPIAerFiberFreeMatrix Lib "WIN50032.dll" ()
Declare Function WAPIAerFiberCentroid1D Lib "WIN50032.dll" () As Long
Declare Function WAPIAerFiberCentroid2D Lib "WIN50032.dll" () As Long
Declare Function WAPIAerFiberCentroid3D Lib "WIN50032.dll" () As Long
Declare Function WAPIAerFiberSetSleepMode Lib "WIN50032.dll" (ByVal mode As Long, _
                                                                    ByVal sleepTime As Long) As Long


Declare Function WAPIAerReadStatus2 Lib "WIN50032.dll" (ByVal nStatus As Integer) As Long
Declare Function WAPIAerReadPosition2 Lib "WIN50032.dll" (ByVal nPos As Integer) As Double
Declare Function WAPIAerCheckStatus2 Lib "WIN50032.dll" () As Long


Declare Function WAPIAerImmedRead Lib "WIN50032.dll" (ByVal opcode As Long, ByVal offset As Long, _
                ret As Long, dret As Double) As Long

Declare Function WAPIAerVBDblToPreciseString Lib "WIN50032.dll" (ByVal dValue As Double, _
                ByVal dwPrecision As Long, ByVal psErr As String) As Long

Declare Function WAPIGetBoardPointer Lib "WIN50032.dll" (ByVal boardnum As Integer) As Long

Declare Function WAPIGPIBSetParams Lib "WIN50032.dll" (ByVal devNum As Long, ByVal bdindx As Long, _
                                ByVal pad As Long, ByVal sad As Long, ByVal tmo As Long, _
                                ByVal eot As Long, ByVal eos As Long) As Long

Declare Function WAPIGPIBSetDev Lib "WIN50032.dll" (ByVal devNum As Long, ByVal mode As Long, Dev As Long, iberror As Long, _
                                                ibstat As Long) As Long

'Declare Function WAPIGPIBSetOn_Off Lib "WIN50032.dll" (ByVal on_off As Long) As Long

Declare Function WAPIGPIBSetWriteString Lib "WIN50032.dll" (ByVal devNum As Long, ByVal wstring As String) As Long

Declare Function WAPIGPIBSetBytesToRead Lib "WIN50032.dll" (ByVal devNum As Long, ByVal bytes As Long) As Long

Declare Function WAPIGPIBSetDevOffline Lib "WIN50032.dll" (ByVal Dev As Long, iberror As Long, _
                                                                ibstat As Long) As Long

Declare Function WAPIGPIBOpen Lib "WIN50032.dll" () As Long
Declare Function WAPIGPIBClose Lib "WIN50032.dll" () As Long

Declare Function WAPIAerGetParamSize Lib "WIN50032.dll" () As Long

Declare Function WAPIAerSetDemoMode Lib "WIN50032.dll" (ByVal on_off As Long) As Long

Declare Function WAPIAerConfig8x3Iobus Lib "WIN50032.dll" (ByVal bank As Byte, ByVal dir As Byte) As Long
Declare Function WAPIAerRead8x3Iobus Lib "WIN50032.dll" (ByVal bank As Byte, ByRef data As Byte) As Long
Declare Function WAPIAerWrite8x3Iobus Lib "WIN50032.dll" (ByVal bank As Byte, ByVal mask As Byte, ByVal data As Byte) As Long

Declare Function WAPIAerSetSecondaryAxisPosition Lib "WIN50032.dll" (ByVal axis As Integer, ByVal mode As Integer, pos As Double) As Long

'Diag
'Type AerDiagRecvPacket
'    dwStatus(30) As Long
'End Type




