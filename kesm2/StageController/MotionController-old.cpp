// MotionController.cpp : main header file for the PROJECT_NAME application
//
#include "stdafx.h"
#include "..\DataShare.h"
#include "StageControllerDlg.h"
#include "MotionController.h"

#define nMAX_STR_LEN	256
#define sU500_PARAM_FILE		"c:\\u500\\mmi\\KESM\\KESM.prm"
#define sU500_FIRMWARE_FILE     "c:\\u500\\mmi\\u500pci.jwp"

CMotionController::CMotionController(CWnd* pWnd)
{
	m_hThread4StairStepping = m_hThread = NULL;
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

	m_nNumCols = 5;
	m_nNumSlices = 10;

	// Jan 11-07
	m_nCurCol = 0;
	//m_dSafeGap = 0.1;
	//m_nMaxSlices = 1000;

	//m_nCurPlankNo = 0;
	m_bStairStepCutting = false;
}

CMotionController::~CMotionController()
{
	TerminateThread(m_hThread, 0);
	if(m_hThread != NULL)
	{
		WaitForSingleObject( m_hThread, INFINITE );
		// Destroy the thread object.
		CloseHandle( m_hThread );
	}

	TerminateThread(m_hThread4StairStepping, 0);
	if(m_hThread4StairStepping != NULL)
	{
		WaitForSingleObject(m_hThread4StairStepping, INFINITE );
		// Destroy the thread object.
		CloseHandle(m_hThread4StairStepping);
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
}

// basic operations
void CMotionController::Initialize()
{
    long lErr;
    char szErr[nMAX_STR_LEN];

    lErr = WAPIAerOpen(0);
    if(lErr != 0)
    {
        // Couldn't open board
		((CStageControllerDlg*)m_pParentWnd)->AddLog("Couldn't open an interface card");
        return;
    }
    if(WAPIAerCheckInitz(0))
    {
		lErr = WAPIAerInitialize ( NULL, sU500_FIRMWARE_FILE, sU500_PARAM_FILE);
	}
	else
	{
		lErr = WAPIAerSoftwareInitialize ( sU500_PARAM_FILE) ; // resets system software without hardware reset
	}
  
	if( lErr ) 
	{
		WAPIErrorToAscii (lErr, szErr);
        return; // exit(0); should close app
	}

	// init commands
	SetMetric();
	SendCommand("ENABLE X Y Z");

	// Sets up thread for interrupt handling.  On interrupt, the libraries will call
	// aerIntHandler with the argument the address of i.  TRUE indicates that the event
	// will be reclaimed by this program if a thread was created previously
	//lErr = WAPIAerCreateThreadEx ( ReadPositionThread, this, TRUE );	//if (lErr)
	//{
	//	((CStageControllerDlg*)m_pParentWnd)->AddLog("Couldn't startIntHandler thread");
	//	return; //exit(0);
	//}
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
	lErr = WAPIAerInitialize ( NULL, sU500_FIRMWARE_FILE, sU500_PARAM_FILE);
	if(lErr)
	{
		WAPIErrorToAscii (lErr, szErr);
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
	SendCommand( "PR ME UN/SE" );
}

// commands
void CMotionController::SendCommand(char *pszCmd)
{
	char szErr[nMAX_STR_LEN];

	long lErr = WAPIAerSend ( pszCmd );
	if(lErr)
	{
		WAPIErrorToAscii (lErr, szErr);
	}
}

void CMotionController::SetFire(double dFireStep, double dPulseWidth)
{
    char szCmd[256];

    sprintf(szCmd, "fire X%f PULSE %f ABS ON", dFireStep, dPulseWidth);
    SendCommand(szCmd);
}


// get variables
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

// cutting job
bool CMotionController::IncrementStage(double dX, double dY, double dZ, double dFeedRateSpeed)
{
    char szCmd[256], szTmp[80];

    if(dX == 0 && dY == 0 && dZ == 0)
        return false;

    strcpy(szCmd, "LINEAR");

    if(dX != 0)
    {
        sprintf(szTmp, " X%f", dX);
        strcat(szCmd, szTmp);
    }
    if(dY != 0)
    {
        sprintf(szTmp, " Y%f", dY);
        strcat(szCmd, szTmp);
    }
    if(dZ != 0)
    {
        sprintf(szTmp, " Z%f", dZ);
        strcat(szCmd, szTmp);
    }
    if(dFeedRateSpeed != 0)
    {
        sprintf(szTmp, " F%f", dFeedRateSpeed);
        strcat(szCmd, szTmp);
    }
    
    SendCommand(szCmd);
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

		CString strFileName;
		if(m_bStairStepCutting)
		{
			strFileName.Format("%s\\%05d\\%04d%02d%02d_%02d%02d%02d_p%05d_s%04d_x%.4fy%.4fz%.4f_v%.4f.tif", 
								((CStageControllerDlg*)m_pParentWnd)->m_strFolderName,
								m_nCurCol,
								CurTime.GetYear(), CurTime.GetMonth(), CurTime.GetDay(),
								CurTime.GetHour(), CurTime.GetMinute(), CurTime.GetSecond(),
								m_anCurPlank[m_nCurCol], m_nCurSliceNo,
								m_dXPos, m_dYPos, m_dZPos,
								dCuttingSpeed);
		}
		else
		{
			strFileName.Format("%s\\%04d%02d%02d_%02d%02d%02d_x%.4fy%.4fz%.4f_v%.4f.tif", 
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
		cpd.cbData = strFileName.GetLength()+1; //strDataToSend.GetLength();
		cpd.lpData = (void*)strFileName.GetBuffer(); //strDataToSend.GetBuffer(cpd.cbData);
		pWnd->SendMessage(WM_COPYDATA, (WPARAM)AfxGetApp()->m_pMainWnd->GetSafeHwnd(), (LPARAM)&cpd);

		pWnd->PostMessage(WM_APP_BSS, WP_START_CAPTURE); 
	}

	//stage motion code for one complete slice
	//microtome startes at top of block
	if(!IncrementStage(0, 0, m_dSliceSizeZ, 1))
		return false;//translate z down ZSliceSize

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
	CMotionController *pMotionController = (CMotionController *)lpArg;
	//stage motion code for one complete block
	//microtome starts at top of block
	pMotionController->m_nCurSliceNo = 0;
	for(int i=0; i<pMotionController->m_nSlicesPerPlank; i++)			//cut a sequence of slices
	{
		double CurrentSpeed = pMotionController->GetSpeed();		//retrieve a valid cutting speed
		//pMotionController->m_nCurPlankNo = i;
		((CStageControllerDlg*)pMotionController->m_pParentWnd)->m_sttLeftSlices.Format("%04d", pMotionController->m_nSlicesPerPlank-i);

//		CString strSpeed;
//		strSpeed.Format("%f", CurrentSpeed);
//		((CStageControllerDlg*)(pMotionController->m_pParentWnd))->AddLog(strSpeed);

		pMotionController->m_nCurSliceNo = i;
		if(!pMotionController->CutSlice(CurrentSpeed))
		{
			//cout<<"Terminating block"<<endl;
			TerminateThread(pMotionController->m_hThread, 0);
			return 0;
		}
		if(((CStageControllerDlg*)(pMotionController->m_pParentWnd))->m_fSaveImages)
		{
			while(!pMotionController->m_bGo)
			{
				Sleep(10);
			}
			pMotionController->m_bGo = false;
			
			// wait for cleaning
			Sleep(pMotionController->m_nWaitBetweenSlice); //nWAIT_TIME_BETWEEN_SECTIONING);
		}
		//WaitForSingleObject(m_hThread, INFINITE );
	}

	// display the last cout which is zero
	((CStageControllerDlg*)pMotionController->m_pParentWnd)->m_sttLeftSlices.Format("%04d", pMotionController->m_nSlicesPerPlank-i);

	TerminateThread(pMotionController->m_hThread, 0);
	return 1;
}

bool CMotionController::CutPlank()
{
	DWORD dwThreadId;
	
	//m_nCurPlankNo = 0;
	m_nCurSliceNo = 0;

	m_bGo = false;
    m_hThread = (HANDLE)CreateThread( 
        NULL,                        // default security attributes 
        0,                           // use default stack size  
        CutPlankThread,                  // thread function 
        this,		                 // argument to thread function 
        0,                           // use default creation flags 
        &dwThreadId);                // returns the thread identifier 
 
   // Check the return value for success. 
 
   if (m_hThread == NULL) 
   {
	  return false;
   }
	//WaitForSingleObject(m_hThread, INFINITE);
	//CloseHandle(m_hThread);
	return true;
}

void CMotionController::Stop()
{
	TerminateThread(m_hThread, 0);
	CloseHandle(m_hThread);

	TerminateThread(m_hThread4StairStepping, 0);
	CloseHandle(m_hThread4StairStepping);

	m_hThread4StairStepping = m_hThread = NULL;
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

void CMotionController::SaveCurState(CString strFileName, CMotionController *pMC)
{
	FILE *fpCurState;

	fpCurState = fopen(strFileName, "wt");
	if(fpCurState)
	{
		fprintf(fpCurState, "%f\t%f\t%f\n", pMC->m_dXPos, pMC->m_dYPos, pMC->m_dZPos);
		fprintf(fpCurState, "%d\t%d\n", pMC->m_nCurCol, pMC->m_nCurSliceNo);
		for(int i = 0; i < nMAX_COLS; i++) 
			fprintf(fpCurState, "%d,", pMC->m_anCurPlank[i]);
		fprintf(fpCurState, "\n");
		fclose(fpCurState);
	}
}

void CMotionController::LoadCurState(CString strFileName, CMotionController *pMC)
{
	FILE *fpCurState;

	fpCurState = fopen(strFileName, "rt");
	if(fpCurState != NULL)
	{
		fscanf(fpCurState, "%f\t%f\t%f\n", &pMC->m_dXPos, &pMC->m_dYPos, &pMC->m_dZPos);
		fscanf(fpCurState, "%d\t%d\n", &pMC->m_nCurCol, &pMC->m_nCurSliceNo);
		for(int i = 0; i < nMAX_COLS; i++) 
			fscanf(fpCurState, "%d,", &pMC->m_anCurPlank[i]);
		fclose(fpCurState);
	}
}

#define DOUBLE_MAX			9999


DWORD WINAPI CMotionController::StairSteppingThread(LPVOID lpArg)
{
	CMotionController *pMC = (CMotionController *)lpArg;

	pMC->m_bStairStepCutting = true;

	// !!!!!!!!!!!!!!!!!!!!!!!1
	// should've used the height
	int nMaxPlankDepth = pMC->m_nStairPlankDepth;
	
	// reset current column number
	int nCurSliceNo = 0;
	
	// init cur z pos
    memset(pMC->m_adCurZ, 0, sizeof(double)*nMAX_COLS);
	//double dPlankThickness = pMC->m_dSliceSizeZ * pMC->m_nSlicesPerPlank;
	CString strCurState;

	strCurState.Format("%s\\CurState.txt", ((CStageControllerDlg*)pMC->m_pParentWnd)->m_strFolderName);

	pMC->m_nCurSliceNo = 0;
	pMC->m_nCurCol = 0;

	LoadCurState(strCurState, pMC);

	do {
		pMC->CutSlice(pMC->GetSpeed());

		// now increase the plank # in the current column
		pMC->m_adCurZ[pMC->m_nCurCol] += pMC->m_dSliceSizeZ;

		// if reach to the last col, simply go back to the first col
		if((pMC->m_nCurCol + 1) == pMC->m_nNumCols)
		{
			// go back to the first col
			// make safe space to move
			//pMC->IncrementStage(0, 0, -(pMC->m_dSafeGap), 2);
			pMC->IncrementStage(0, 0, -(pMC->m_dLiftHeight), 2);
			
			// go to init y pos
			pMC->IncrementStage(0, pMC->m_dSliceSizeY*pMC->m_nCurCol, 0, 2);
			// go to the previous z pos
			//pMC->IncrementStage(0, 0, (pMC->m_nCurCol*dPlankThickness + pMC->m_dSafeGap), 2);
			pMC->IncrementStage(0, 0, (pMC->m_adCurZ[pMC->m_nCurCol]-pMC->m_adCurZ[0] + pMC->m_dLiftHeight), 2);

			pMC->m_nCurCol = 0;
		}
		else //if((pMC->m_nCurCol + 1) < pMC->m_nNumCols)
		{
#define dTHRESHOLD_THICKNESS		0.1
			if((pMC->m_adCurZ[pMC->m_nCurCol] - pMC->m_adCurZ[pMC->m_nCurCol+1]) < dTHRESHOLD_THICKNESS)
			{
				// keep cutting
			}
			else
			{
				if(can i cut next col?)
				{
					// go to next col
					// move to
					//pMC->IncrementStage(0, 0, -(2*dPlankThickness + pMC->m_dSafeGap), 2);
					pMC->IncrementStage(0, 0, -(2*dPlankThickness + pMC->m_dLiftHeight), 2);
					pMC->IncrementStage(0, -pMC->m_dSliceSizeY, 0, 2);
					//pMC->IncrementStage(0, 0, pMC->m_dSafeGap, 2);
					pMC->IncrementStage(0, 0, pMC->m_dLiftHeight, 2);

					pMC->m_nCurCol++;
				}
				else
				{
					// go back to the first col
					// make safe space to move
					//pMC->IncrementStage(0, 0, -(pMC->m_dSafeGap), 2);
					pMC->IncrementStage(0, 0, -(pMC->m_dLiftHeight), 2);
					// go to init y pos
					pMC->IncrementStage(0, pMC->m_dSliceSizeY*pMC->m_nCurCol, 0, 2);
					// go to the previous z pos
					//pMC->IncrementStage(0, 0, (pMC->m_nCurCol*dPlankThickness + pMC->m_dSafeGap), 2);
					pMC->IncrementStage(0, 0, (pMC->m_adCurZ[pMC->m_nCurCol]-pMC->m_adCurZ[0] + pMC->m_dLiftHeight), 2);

					pMC->m_nCurCol = 0;
				}
			}
		}
		// write down the current state
		SaveCurState(strCurState, pMC);

	} while(pMC->m_anCurPlank[0] < nMaxPlankDepth); //pMC->m_nCurCol < pMC->m_nNumCols);

/*
		if(pMC->m_nNumCols == 0) // not valid
		{
			pMC->m_bStairStepCutting = true;
			return 0;
		}

		pMC->IncrementStage(0, -(pMC->m_dSliceSizeY*(pMC->m_nNumCols - 1)), 0, 2);//move over YSliceSize
		// or use the sum of m_dSliceSizeY
 		//pMC->IncrementStage(0, 0, pMC->m_dSafeGap, 2);
 		pMC->IncrementStage(0, 0, pMC->m_dLiftHeight, 2);

		nCurSliceNo += pMC->m_nSlicesPerPlank;
	//} while(nCurSliceNo < pMC->m_nMaxSlices);
	//} while(pMC->m_anCurPlank[0] < nMaxPlankDepth);
*/
	// restore the original
	pMC->m_nSlicesPerPlank = nSlicesPerPlank;

	pMC->m_bStairStepCutting = false;
	return 1;
}

bool CMotionController::StairSteppingSession()
{
	DWORD dwThreadId;
	
    m_hThread4StairStepping = (HANDLE)CreateThread( 
        NULL,                        // default security attributes 
        0,                           // use default stack size  
        StairSteppingThread,                  // thread function 
        this,		                 // argument to thread function 
        0,                           // use default creation flags 
        &dwThreadId);                // returns the thread identifier 
 
   // Check the return value for success. 
 
   if (m_hThread4StairStepping == NULL) 
   {
	  return false;
   }
	//WaitForSingleObject(m_hThread, INFINITE);
	//CloseHandle(m_hThread);

	return true;
}

// callback function to be called by the U500 libraries upon a hardware interrupt.  Set up
// by the WAPIAerCreateThreadEx function.
DWORD WINAPI CMotionController::ReadPositionThread( void *pvData )
{
	//short *i;
	//double dpos;

	//i = (short *) pvData;
	while(1)
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
	}

	//printf( "%d, pos = %f\n", *i, dpos );
	return(0);
}