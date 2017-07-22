// MotionController.cpp : main header file for the PROJECT_NAME application
//
#include "stdafx.h"
#include <math.h>

#define sU500_DIR				"c:\\u500\\mmi"

#include "..\DataShare.h"
#include "StageControllerDlg.h"
#include "MotionController.h"
#include "c:\u500\mmi\include\quick.h"

#define nMAX_STR_LEN	256
#define sU500_PARAM_FILE		sU500_DIR"\\KESM\\KESM.prm"
#define sU500_FIRMWARE_FILE     sU500_DIR"\\u500pci.jwp"

CMotionController::CMotionController(CWnd* pWnd)
{
	m_hEventStopStairStep = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hEventStopReadPosition = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hEventStopPlank = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hEventStopPause = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hEventStepByStep = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_bStepByStep = false;
	m_bFirstStepByStep = false;

	m_hThread4StairStep = m_hThread4Plank = NULL;
	m_hThread4ReadPosition = NULL;
	m_pParentWnd = (CStageControllerDlg*)pWnd;

    // cutting parameters
    m_CuttingMode = CM_RANDOM;
    m_dLiftHeight = 0.1;
    m_dReturnSpeed = 21.0;

    m_dSliceSizeX = 8.0;
    m_dSliceSizeY = 0.3;
    m_dSliceSizeZ = 0.002;
    m_dAverageCuttingSpeed = 10.0;
    m_dDeviation = 0.5;
	m_nWaitBetweenSlice = 2;
    m_nSlicesPerPlank = 100;
    m_nStairPlankDepth = 1000;
    m_nStairPlankWidth = 25;

    // cutting session variables
    m_dTotalZ = 0.0;
    m_nTotalSlices = 0;

    // camera fire parameters
    m_dFireStep = 0.0006; // 10x
    //m_dFireStep = 0.0003; // 40x
    m_dFirePulseWidth = 0.1;

	//m_nNumCols = 5;
	//m_nNumSlices = 10;

	// Jan 11-07
	//m_nCurCol = 0;
	//m_dSafeGap = 0.1;
	//m_nMaxSlices = 1000;

	//m_nCurPlankNo = 0;
	m_bStairStepCutting = false;
	m_bStairStepPause = false;

	//
	m_pStairStep = new CStairStep();
	m_pStairStep->m_CuttingEdge = CStairStep::CE_LEFT;

	m_pStairStep->m_dBlockWidth = 30;
	m_pStairStep->m_dColumnWidth = 0.3;
	m_pStairStep->m_dThickness = 0.002;
	m_pStairStep->m_nPlankDepth = 3;

	m_bReady4Log = false;
	//m_fpLog = fopen("command_log.txt", "w+t");
}

CMotionController::~CMotionController()
{
	Shutdown();

	// all of thread should be ended before coming here 
	// but just in case
	TerminateThread(m_hThread4Plank, 0);
	TerminateThread(m_hThread4StairStep, 0);
	TerminateThread(m_hThread4ReadPosition, 0);

	delete m_pStairStep;

	/* ------------------
	if(m_hThread4Plank != NULL)
	{
		WaitForSingleObject( m_hThread4Plank, INFINITE );
		// Destroy the thread object.
		CloseHandle( m_hThread4Plank );
	}

	TerminateThread(m_hThread4StairStep, 0);
	if(m_hThread4StairStep != NULL)
	{
		WaitForSingleObject(m_hThread4StairStep, INFINITE );
		// Destroy the thread object.
		CloseHandle(m_hThread4StairStep);
	}

	// terminates thread created by WAPIAerCreateThreadEx , will no longer handle the interrupt
	// make sure that this function is not called until after the interrupts have occurred and have been

	// handled properly
	//WAPIAerTerminateThreadEx ( );
	TerminateThread(m_hThread4ReadPosition, 0);
	if(m_hThread4ReadPosition != NULL)
	{
		WaitForSingleObject(m_hThread4ReadPosition, INFINITE );
		// Destroy the thread object.
		CloseHandle(m_hThread4ReadPosition);
	}
	---------------------- */

	//fclose(m_fpLog);
}

void CMotionController::SetLogFilenames()
{
	// set log filename
	m_strXYZLogFile.Format("%s\\XYZ.txt", ((CStageControllerDlg*)m_pParentWnd)->m_strFolderName);
	m_strCmdLogFile.Format("%s\\Command.txt", ((CStageControllerDlg*)m_pParentWnd)->m_strFolderName);

	m_bReady4Log = true;
}

// basic operations
void CMotionController::Initialize()
{
    long lErr;
    char szErr[nMAX_STR_LEN];

	// init variables
	m_bStepByStep = false;

    lErr = WAPIAerOpen(0);
    if(lErr != 0)
    {
        // Couldn't open board
		((CStageControllerDlg*)m_pParentWnd)->AddLog("Couldn't open an interface card");
        return;
    }
    //if(WAPIAerCheckInitz(0))
    {
		lErr = WAPIAerInitialize ( NULL, sU500_FIRMWARE_FILE, sU500_PARAM_FILE);
	}
	//else
	//{
	//	lErr = WAPIAerSoftwareInitialize ( sU500_PARAM_FILE) ; // resets system software without hardware reset
	//}
  
	if( lErr ) 
	{
		WAPIErrorToAscii (lErr, szErr);
		AfxMessageBox(szErr, MB_ICONSTOP|MB_OK);
        return; // exit(0); should close app
	}

	DWORD dwThreadId;

    m_hThread4ReadPosition = (HANDLE)CreateThread( 
        NULL,                        // default security attributes 
        0,                           // use default stack size  
        ReadPositionThread,                  // thread function 
        this,		                 // argument to thread function 
        0,                           // use default creation flags 
        &dwThreadId);                // returns the thread identifier 
}

// not working... check the manual
void CMotionController::Reset()
{
	long lErr;
	char szErr[nMAX_STR_LEN];

	Shutdown();
	//lErr = WAPIAerInitialize ( NULL, sU500_FIRMWARE_FILE, sU500_PARAM_FILE);
    //if(WAPIAerCheckInitz(0))
    {
		lErr = WAPIAerInitialize ( NULL, sU500_FIRMWARE_FILE, sU500_PARAM_FILE);
	}
	//else
	//{
	//	lErr = WAPIAerSoftwareInitialize ( sU500_PARAM_FILE) ; // resets system software without hardware reset
	//}

	if(lErr)
	{
		WAPIErrorToAscii (lErr, szErr);
		AfxMessageBox(szErr, MB_ICONSTOP|MB_OK);
	}
}

void CMotionController::Abort()
{
    WAPIAerAbort();
}

void CMotionController::Shutdown()
{
    WAPIAerClose(0);
}

void CMotionController::SetMetric()
{
	/* program in metric units for distance and units/second for velocity */
	//SendCommand( "PR ME UN/SE" );
}

void CMotionController::CommandLog(char *pszCmd)
{
	FILE *fpLog;

	if(m_bReady4Log == false)
		return;

	//jrkwon 2008-01-30
	/*
	fpLog = fopen(m_strCmdLogFile, "a+t");
 	if(fpLog)
	{
		fprintf(fpLog, "%s\n", pszCmd);
		fclose(fpLog);
	}*/
}

void CMotionController::SendCommand(char *pszCmd)
{
	char szErr[nMAX_STR_LEN];

	long lErr = WAPIAerSend ( pszCmd );
	if(lErr)
	{
		WAPIErrorToAscii (lErr, szErr);
		AfxMessageBox(szErr, MB_ICONSTOP|MB_OK);
		//((CStageControllerDlg*)m_pParentWnd)->OnBnClickedBtnStop();
	}

	CommandLog(pszCmd);
}

AERERR_CODE CMotionController::WaitForMoveDone(short axes)
{
	 AERERR_CODE    eRc;
     long           mask;
	 long			status;

     mask = 0x8FFF00;  // check command buffer and all planes 

	 mask |= ( (long) axes << 4 ) ;

     do
	 {
     	// Update status. 
		if( eRc = WAPIAerCheckStatus() )
            	break;
        
		status = WAPIAerReadStatus(5);
           	    
         // Check for axis error. Break loop if one occurs.
		if( WAPIAerReadStatus(1) || WAPIAerReadStatus(2)
			|| WAPIAerReadStatus(3) || WAPIAerReadStatus(4) )
         		break;

     }while( status & mask );

     return( eRc );
}

void CMotionController::SetFire(double dFireStep, double dPulseWidth)
{
    char szCmd[256];

    sprintf(szCmd, "fire X%f PULSE %f ABS ON", dFireStep, dPulseWidth);
    SendCommand(szCmd);
}

double CMotionController::GetSpeed()
{
    double dSpeed;

    if(m_CuttingMode == CM_FIXED)
        dSpeed = m_dAverageCuttingSpeed;
    else if(m_CuttingMode == CM_RANDOM)
        dSpeed = (((double)rand()/(double)RAND_MAX)*2*m_dDeviation) - m_dDeviation + m_dAverageCuttingSpeed;
	else if(m_CuttingMode == CM_ALTERNATING)
	{
		if(rand() % 2 == 0)
            dSpeed = m_dAverageCuttingSpeed + m_dDeviation;
		else
			dSpeed = m_dAverageCuttingSpeed - m_dDeviation;
	}
    else
        dSpeed = 0.0;

    return dSpeed;
}

void CMotionController::SaveStartXYZ(CMotionController *pMC, CStairStep *pSS)
{
	pSS->m_dStartX = pMC->m_dXPos;
	pSS->m_dStartY = pMC->m_dYPos;
	pSS->m_dStartZ = pMC->m_dZPos;
	
	SaveCurState(pMC);

	pMC->m_bFirstStepByStep = false;
}

bool CMotionController::IncrementStage(double dX, double dY, double dZ, double dFeedRateSpeed)
{
    char szCmd[256], szTmp[80];
	bool bX, bY, bZ;

	bX = bY = bZ = false;

    if(dX == 0 && dY == 0 && dZ == 0)
        return false;

	strcpy(szCmd, "LINEAR");

    if(dX != 0)
    {
        sprintf(szTmp, " X%f", dX);
        strcat(szCmd, szTmp);
		bX = true;
    }
    if(dY != 0)
    {
        sprintf(szTmp, " Y%f", dY);
        strcat(szCmd, szTmp);
		bY = true;
    }
    if(dZ != 0)
    {
        sprintf(szTmp, " Z%f", dZ);
        strcat(szCmd, szTmp);
		bZ = true;
    }
    if(dFeedRateSpeed != 0)
    {
        sprintf(szTmp, " F%f", dFeedRateSpeed);
        strcat(szCmd, szTmp);
    }
    
    SendCommand(szCmd);

	/* wait until move is done */
	/* jrkwon 07-02-07 */
	if(bX)
		WaitForMoveDone(AXIS1);
	if(bY)
		WaitForMoveDone(AXIS2);
	if(bZ)
		WaitForMoveDone(AXIS3);

	// x, y, z position log (abolute)
	FILE *fpXYZLog;

	if(m_bReady4Log)
	{
		//jrkwon: 2008-01-30
		/*fpXYZLog = fopen(m_strXYZLogFile, "a+t");
		if(fpXYZLog)
		{
			CTime CurTime = CTime::GetCurrentTime();

			fprintf(fpXYZLog, "%04d.%02d.%02d %02d:%02d:%02d %f\t%f\t%f\n", 
				CurTime.GetYear(), CurTime.GetMonth(), CurTime.GetDay(),
				CurTime.GetHour(), CurTime.GetMinute(), CurTime.GetSecond(), 
				m_dXPos, m_dYPos, m_dZPos);
			fclose(fpXYZLog);
		}*/
	}

    return true;
}

/* --------------------------
// cut a single slice
// 1. go up specimen
// 2. move right (cutting)
// 3. move down significantly to avoid dragging  
// 4. get back to the staring position
// 5. repeat from 1
------------------------------ */
bool CMotionController::CutSlice(double dCuttingSpeed)
{
	CWnd *pWnd = m_pParentWnd->FindWindow(NULL, BSS_DCAPTURE_TITLE);
	//if(pWnd == NULL)
	//{
	//	return false;
	//}
	if(pWnd && ((CStageControllerDlg*)m_pParentWnd)->m_fSaveImages)
	{
		CTime CurTime = CTime::GetCurrentTime();

		//CString strFileName;
		if(m_bStairStepCutting)
		{
			CString strFolder4Col;
			strFolder4Col.Format("%s\\%05d", ((CStageControllerDlg*)m_pParentWnd)->m_strFolderName,
								m_pStairStep->m_nCurCol);
			CreateDirectory(strFolder4Col, NULL);
			m_strImageFileName.Format("%s\\%05d\\%04d%02d%02d_%02d%02d%02d_x%.4fy%.4fz%.4f_t%4f_v%.4f.tif", 
								((CStageControllerDlg*)m_pParentWnd)->m_strFolderName,
								m_pStairStep->m_nCurCol,
								CurTime.GetYear(), CurTime.GetMonth(), CurTime.GetDay(),
								CurTime.GetHour(), CurTime.GetMinute(), CurTime.GetSecond(),
								//m_anCurPlank[m_nCurCol], m_nCurSliceNo,
								m_dXPos, m_dYPos, m_dZPos,
								m_pStairStep->m_dThickness,
								dCuttingSpeed);
		}
		else
		{
			m_strImageFileName.Format("%s\\%04d%02d%02d_%02d%02d%02d_x%.4fy%.4fz%.4f_v%.4f.tif", 
								((CStageControllerDlg*)m_pParentWnd)->m_strFolderName,
								CurTime.GetYear(), CurTime.GetMonth(), CurTime.GetDay(),
								CurTime.GetHour(), CurTime.GetMinute(), CurTime.GetSecond(),
								m_dXPos, m_dYPos, m_dZPos,
								dCuttingSpeed);
		}

		//AfxMessageBox(strFileName);

		// copy data
		COPYDATASTRUCT cpd;
		cpd.dwData = 0;
		cpd.cbData = m_strImageFileName.GetLength()+1; //strDataToSend.GetLength();
		cpd.lpData = (void*)m_strImageFileName.GetBuffer(); //strDataToSend.GetBuffer(cpd.cbData);
		pWnd->SendMessage(WM_COPYDATA, (WPARAM)AfxGetApp()->m_pMainWnd->GetSafeHwnd(), (LPARAM)&cpd);

		pWnd->PostMessage(WM_APP_BSS, WP_START_CAPTURE); 
	}

	CommandLog("Start -----------------------");

	if(!m_bStairStepCutting)
	{
		//stage motion code for one complete slice
		//microtome startes at top of block
		if(!IncrementStage(0, 0, m_dSliceSizeZ, 1))
			return false;//translate z down ZSliceSize
	}

	int nStartTime = time(NULL);				//allot time for z-axis to settle
	while(time(NULL) <= nStartTime + 2)
        ;

	//turn camera firing on
	//SendCommand("fire ON");
	
    SetFire(m_dFireStep, m_dFirePulseWidth);
	
	if(!IncrementStage(m_dSliceSizeX, 0, 0, dCuttingSpeed))
		return false;//translate x forward XSliceSize at CuttingSpeed

	//turn camera firing off
	SendCommand("fire OFF");

	if(!IncrementStage(0 ,0, -m_dLiftHeight,1))
		return false;//translate z up LiftHeight
	if(!IncrementStage(-m_dSliceSizeX, 0, 0, m_dReturnSpeed))
		return false;//translate x backward XSliceSize at ReturnSpeed
	if(!IncrementStage(0, 0, m_dLiftHeight, 1))
		return false;//translate z down LiftHeight

	return true;
}

// cut several slices
DWORD WINAPI CMotionController::CutPlankThread(LPVOID lpArg)
{
	CMotionController *pMC = (CMotionController *)lpArg;
	//stage motion code for one complete block
	//microtome starts at top of block
	pMC->m_nCurSliceNo = 0;
	for(int i=0; i<pMC->m_nSlicesPerPlank; i++)			//cut a sequence of slices
	{
		double CurrentSpeed = pMC->GetSpeed();		//retrieve a valid cutting speed
		//pMC->m_nCurPlankNo = i;
		((CStageControllerDlg*)pMC->m_pParentWnd)->m_sttLeftSlices.Format("%04d", pMC->m_nSlicesPerPlank-i);

//		CString strSpeed;
//		strSpeed.Format("%f", CurrentSpeed);
//		((CStageControllerDlg*)(pMC->m_pParentWnd))->AddLog(strSpeed);

		pMC->m_nCurSliceNo = i;
		if(!pMC->CutSlice(CurrentSpeed))
		{
			//cout<<"Terminating block"<<endl;
			//TerminateThread(pMC->m_hThread4Plank, 0);
			// jrkwon: Need to do something???!!!!????
			break;
			//return 0;
		}
		if(((CStageControllerDlg*)(pMC->m_pParentWnd))->m_fSaveImages)
		{
			while(!pMC->m_bGo)
			{
				Sleep(10);
			}
			pMC->m_bGo = false;
			
			// wait for cleaning
			Sleep(pMC->m_nWaitBetweenSlice); //nWAIT_TIME_BETWEEN_SECTIONING);
		}
		if(WaitForSingleObject(pMC->m_hEventStopPlank, 10) == WAIT_OBJECT_0)
			break;
	}

	// display the last cout which is zero
	((CStageControllerDlg*)pMC->m_pParentWnd)->m_sttLeftSlices.Format("%04d", pMC->m_nSlicesPerPlank-i);

	//jrkwon 2007-07-06 TerminateThread(pMC->m_hThread4Plank, 0);
	return 1;
}

bool CMotionController::CutPlank()
{
	DWORD dwThreadId;
	
	//m_nCurPlankNo = 0;
	m_nCurSliceNo = 0;

	m_bGo = false;
    m_hThread4Plank = (HANDLE)CreateThread( 
        NULL,                        // default security attributes 
        0,                           // use default stack size  
        CutPlankThread,                  // thread function 
        this,		                 // argument to thread function 
        0,                           // use default creation flags 
        &dwThreadId);                // returns the thread identifier 
 
   // Check the return value for success. 
 
   if (m_hThread4Plank == NULL) 
   {
	  return false;
   }

   return true;
}

void CMotionController::Stop()
{
	//TerminateThread(m_hThread4Plank, 0);
	SetEvent(m_hEventStopPlank);
	CloseHandle(m_hThread4Plank);

	//TerminateThread(m_hThread4StairStep, 0);
	SetEvent(m_hEventStopStairStep);
	CloseHandle(m_hThread4StairStep);

	//TerminateThread(m_hThread4ReadPosition, 0);
	SetEvent(m_hEventStopReadPosition);
	CloseHandle(m_hThread4ReadPosition);

	SetEvent(m_hEventStopPause);

	SetEvent(m_hEventStepByStep);
	m_bStepByStep = false;

	m_hThread4StairStep = m_hThread4Plank = m_hThread4ReadPosition = NULL;
	m_bStairStepCutting = false;

	//m_nCurPlankNo = 0;
}

// session: cut one slice at a time
// need to change m_dSliceSizeZ value every each CutSlice
void CMotionController::SliceSession()
{
    //m_dTotalZ = 0.0;
    //m_nTotalSlices = 0;

    // Z step can't go to zero, and minimum thickness is cDynamicSliceStep
    if(m_dSliceSizeZ < cDynamicSliceStep) 
        m_dSliceSizeZ = cDynamicSliceStep;
    // maximum thickness is cMaxSliceThickness microns
    if(m_dSliceSizeZ > cMaxSliceThickness)
        m_dSliceSizeZ = cMaxSliceThickness;

    double dCurrentSpeed = GetSpeed();
    CutSlice(dCurrentSpeed);
    m_dTotalZ += m_dSliceSizeZ;
    m_nTotalSlices++;
}

void CMotionController::PlankSession()
{
    m_dTotalZ = 0.0;
    m_nTotalSlices = 0;

    CutPlank();
    m_dTotalZ += m_dSliceSizeZ * m_nSlicesPerPlank;
    m_nTotalSlices += m_nSlicesPerPlank;
}

void CMotionController::SaveCurState(CMotionController *pMC)
{
	FILE *fpCurState;
	CStairStep *pSS = pMC->m_pStairStep;

	fpCurState = fopen(pMC->m_strStateFile, "wt");
	if(fpCurState)
	{
		fprintf(fpCurState, "%f\t%f\t%f\n", pSS->m_dStartX, pSS->m_dStartY, pSS->m_dStartZ);
		fprintf(fpCurState, "%d\n", pSS->m_nCurCol);
		for(int i = 0; i < pSS->m_nNumCols; i++) 
			fprintf(fpCurState, "%f\n", pSS->m_pdPosZ[i]);
		fprintf(fpCurState, "\n");
		fclose(fpCurState);
	}
}

void CMotionController::LoadCurState(CMotionController *pMC)
{
	FILE *fpCurState;
	CStairStep *pSS = pMC->m_pStairStep;

	fpCurState = fopen(pMC->m_strStateFile, "rt");
	if(fpCurState != NULL)
	{
		fscanf(fpCurState, "%lf\t%lf\t%lf\n", &(pSS->m_dStartX), &(pSS->m_dStartY), &(pSS->m_dStartZ));
		fscanf(fpCurState, "%d\n", &(pSS->m_nCurCol));
		for(int i = 0; i < pSS->m_nNumCols; i++) 
			fscanf(fpCurState, "%lf\n", &(pSS->m_pdPosZ[i]));
		fclose(fpCurState);
	}
	else
	{
		// default init value
		pSS->m_dStartX = 155.0;
		pSS->m_dStartY = 22.0;
		pSS->m_dStartZ = 4;
	}
}

void CMotionController::MoveStage(double dX, double dY, double dZ, LPVOID lpArg)
{
#if 0
    CMotionController *pMC = (CMotionController *)lpArg;
	char szCmd[256];

	pMC->CommandLog("MoveStage -------------------");

	// absolute positioning
	pMC->SendCommand("PR ME UN/SE AB");
	sprintf(szCmd, "LINEAR X%f Y%f Z%f F%f", dX, dY, dZ, 1);
	pMC->SendCommand(szCmd);
	pMC->SendCommand("PR ME UN/SE IN");
	/*
	WAPIAerSend("PR ME UN/SE AB");
	sprintf(szCmd, "LINEAR X%f Y%f Z%f F%f", dX, dY, dZ, 1);
	WAPIAerSend(szCmd);
	WAPIAerSend("PR ME UN/SE IN");
	*/
#endif
}

void CMotionController::GoHome()
{
	SendCommand("ENABLE X Y Z");
	SendCommand("PR ME UN/SE");	// set metric

	// go to home
	SendCommand("HOME Z");
	WaitForMoveDone(AXIS3);
	SendCommand("HOME Y");
	WaitForMoveDone(AXIS2);
	SendCommand("HOME X");
	WaitForMoveDone(AXIS1);
}

void CMotionController::ResetStartPos()
{
	m_pStairStep->m_dStartX = m_dXPos;
	m_pStairStep->m_dStartY = m_dYPos;
	m_pStairStep->m_dStartZ = m_dZPos;
	
	SaveCurState(this);
}

bool CMotionController::InitStairStep() 
{
	int nResult = AfxMessageBox("Are you SURE to start Stair Step Cutting?", MB_YESNO|MB_ICONQUESTION);
	if(nResult == IDNO)
	{
		return false;
	}

	CStairStep *pSS = m_pStairStep;

	m_strStateFile.Format("%s\\CurState.txt", ((CStageControllerDlg*)m_pParentWnd)->m_strFolderName);
	m_bStairStepCutting = true;
	m_bStepByStep = true;
	
	//double dCurX, dCurY, dCurZ;

	// should be init before calling Init()
	/*dCurX = pMC->m_dXPos;
	dCurY = pMC->m_dYPos;
	dCurZ = pMC->m_dZPos;*/

	// jrkwon 2007-09-05
	pSS->Init(pSS->m_dBlockWidth, pSS->m_dColumnWidth);

	// if the cur status file exists, args will be ignored
	LoadCurState(this);
	// pSS->m_dStartX,Y,Z should be ready before pSS->Init

	GoHome();

	// move the stage to the start point!!!!!
	int nUnitX, nUnitY;
	double dDiff;
	nUnitX = 1;
	nUnitY = 1;

	dDiff = pSS->m_dStartX; //fabs(pSS->m_dStartX-dCurX);
	if(dDiff > 20)
		nUnitX = 20;
	else if(dDiff > 10)
		nUnitX = 10;

	dDiff = pSS->m_dStartY; //fabs(pSS->m_dStartY-dCurY);
	if(dDiff > 2)
		nUnitY = 2;

	//pMC->IncrementStage(pSS->m_dStartX-dCurX, 0, 0, nUnitX);
	//pMC->IncrementStage(0, pSS->m_dStartY-dCurY, 0, nUnitY);
	//pMC->IncrementStage(0, 0, pSS->m_dStartZ-dCurZ, 1);
	IncrementStage(pSS->m_dStartX, 0, 0, nUnitX);
	IncrementStage(0, pSS->m_dStartY, 0, nUnitY);
	IncrementStage(0, 0, pSS->m_dStartZ, 1);

	return true;
}


DWORD WINAPI CMotionController::StairSteppingThread(LPVOID lpArg)
{
	CMotionController *pMC = (CMotionController *)lpArg;
	CStairStep *pSS = pMC->m_pStairStep;

#if 0
	pMC->m_bStairStepCutting = true;

	//double dCurX, dCurY, dCurZ;

	//// should be init before calling Init()
	//dCurX = pMC->m_dXPos;
	//dCurY = pMC->m_dYPos;
	//dCurZ = pMC->m_dZPos;

	// if the cur status file exists, args will be ignored
	pMC->LoadCurState(pMC);
	// pSS->m_dStartX,Y,Z should be ready before pSS->Init
	//pSS->Init(pSS->m_dBlockWidth, pSS->m_dColumnWidth);
	pMC->GoHome();

	// move the stage to the start point!!!!!
	int nUnitX, nUnitY;
	double dDiff;
	nUnitX = 1;
	nUnitY = 1;

	dDiff = pSS->m_dStartX; //fabs(pSS->m_dStartX-dCurX);
	if(dDiff > 20)
		nUnitX = 20;
	else if(dDiff > 10)
		nUnitX = 10;

	dDiff = pSS->m_dStartY; //fabs(pSS->m_dStartY-dCurY);
	if(dDiff > 2)
		nUnitY = 2;

	//pMC->IncrementStage(pSS->m_dStartX-dCurX, 0, 0, nUnitX);
	//pMC->IncrementStage(0, pSS->m_dStartY-dCurY, 0, nUnitY);
	//pMC->IncrementStage(0, 0, pSS->m_dStartZ-dCurZ, 1);
	pMC->IncrementStage(pSS->m_dStartX, 0, 0, nUnitX);
	pMC->IncrementStage(0, pSS->m_dStartY, 0, nUnitY);
	pMC->IncrementStage(0, 0, pSS->m_dStartZ, 1);
#endif

	//jrkwon 2007-09-05 pSS->Init(pSS->m_dBlockWidth, pSS->m_dColumnWidth);

	double dNewY, dNewZ;
	double dOldY, dOldZ;
	int nCol;

	dOldY = pMC->m_dYPos; //dCurY;
	dOldZ = pMC->m_dZPos; //dCurZ;

	do {
		nCol = pSS->CalculateNextSection();

		//move the stage to the coordinates for cutting
		if(pSS->m_CuttingEdge == CStairStep::CE_LEFT)
			dNewY = pSS->m_dStartY + (pSS->m_nNumCols - (nCol+1)) * pSS->m_dColumnWidth;
		else
			dNewY = pSS->m_dStartY - nCol * pSS->m_dColumnWidth;

		dNewZ = pSS->m_pdPosZ[nCol] + pSS->m_dThickness;	//get new z-axis position
	
		//MoveStage(pSS->m_dStartX, dNewY, dNewZ, lpArg);
		pMC->IncrementStage(0, dNewY-dOldY, dNewZ-dOldZ, 1);

		/*while(pMC->m_bStepByStep)
		{
			if(WaitForSingleObject(pMC->m_hEventStepByStep, 10) == WAIT_OBJECT_0)
				break;
		}*/

		if(pMC->m_bStepByStep && pMC->m_bFirstStepByStep)
		{
			SaveStartXYZ(pMC, pSS);
		}

		pMC->CutSlice(pMC->GetSpeed());
		pSS->m_pdPosZ[nCol] = dNewZ;

		dOldY = dNewY;
		dOldZ = dNewZ;

		pMC->SaveCurState(pMC);

		while(pMC->m_bStepByStep)
		{
			if(WaitForSingleObject(pMC->m_hEventStepByStep, 10) == WAIT_OBJECT_0)
				break;
		}

		while(pMC->m_bStairStepPause)
		{
			//Sleep(100);
			if(WaitForSingleObject(pMC->m_hEventStopPause, 10) == WAIT_OBJECT_0)
				break;
		}

		if(WaitForSingleObject(pMC->m_hEventStopStairStep, 10) == WAIT_OBJECT_0)
			break;

		// update current status to the file
	} while((pSS->m_pdPosZ[nCol] - pSS->m_dStartZ) < dMAX_BLOCK_HEIGHT);

	pMC->SaveCurState(pMC);
	pMC->m_bStairStepCutting = false;
	return 1;
}

bool CMotionController::StairSteppingSession()
{
	DWORD dwThreadId;

//	m_strStateFile.Format("%s\\CurState.txt", ((CStageControllerDlg*)m_pParentWnd)->m_strFolderName);
	
    m_hThread4StairStep = (HANDLE)CreateThread( 
        NULL,                        // default security attributes 
        0,                           // use default stack size  
        StairSteppingThread,                  // thread function 
        this,		                 // argument to thread function 
        0,                           // use default creation flags 
        &dwThreadId);                // returns the thread identifier 
 
   // Check the return value for success. 
 
   if (m_hThread4StairStep == NULL) 
   {
	  return false;
   }

	return true;
}

// callback function to be called by the U500 libraries upon a hardware interrupt.  Set up
// by the WAPIAerCreateThreadEx function.
DWORD WINAPI CMotionController::ReadPositionThread( void *pvData )
{
	//short *i;
	//double dpos;

	//i = (short *) pvData;
	do //while(1)
	{
		CMotionController *pMC = (CMotionController *)pvData;

		WAPIAerCheckStatus ();
		pMC->m_dXPosFB = WAPIAerReadPosition (8)*0.0001;
		pMC->m_dYPosFB = WAPIAerReadPosition (9)*0.0001;
		pMC->m_dZPosFB = WAPIAerReadPosition (10)*0.0001;

		pMC->m_dXPos = WAPIAerReadPosition (12)*0.0001;
		pMC->m_dYPos = WAPIAerReadPosition (13)*0.0001;
		pMC->m_dZPos = WAPIAerReadPosition (14)*0.0001;

		((CStageControllerDlg*)(pMC->m_pParentWnd))->m_strXPos.Format("%.4f", pMC->m_dXPos);
		((CStageControllerDlg*)(pMC->m_pParentWnd))->m_strYPos.Format("%.4f", pMC->m_dYPos);
		((CStageControllerDlg*)(pMC->m_pParentWnd))->m_strZPos.Format("%.4f", pMC->m_dZPos);

		((CStageControllerDlg*)(pMC->m_pParentWnd))->m_strXPosFB.Format("%.4f", pMC->m_dXPosFB);
		((CStageControllerDlg*)(pMC->m_pParentWnd))->m_strYPosFB.Format("%.4f", pMC->m_dYPosFB);
		((CStageControllerDlg*)(pMC->m_pParentWnd))->m_strZPosFB.Format("%.4f", pMC->m_dZPosFB);

		//((CStageControllerDlg*)(pMC->m_pParentWnd))->UpdatePosition();
 		if(WaitForSingleObject(pMC->m_hEventStopReadPosition, 10) == WAIT_OBJECT_0)
			break;
	} while(1);//WaitForSingleObject(pMC->m_hEventStopReadPosition, INFINITE) == WAIT_OBJECT_0);

	//printf( "%d, pos = %f\n", *i, dpos );
	return(0);
}