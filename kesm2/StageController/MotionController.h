// MotionController.h : main header file for the PROJECT_NAME application
//

#pragma once

#include "u500\include\aerotype.h"
#include "u500\include\build.h"
#include "u500\include\wapi.h"

#include "StairStep.h"

//#define nMAX_COLS		500

const double cMaxSliceThickness = 0.01;
const double cDynamicSliceStep = 0.0005;

// UNIDEX 5000 Motion Controller
class CMotionController
{
public:
    CMotionController(CWnd* m_pParentWnd);
    ~CMotionController();

    // basic operations
    void Initialize();
	void Reset();
    void Abort();
    void Shutdown();
	
	void SetMetric();
	//AERERR_CODE WaitForMoveDone(short axes);

    // commands
    void SendCommand(char *pszCmd);
	AERERR_CODE WaitForMoveDone(short axes);
    void SetFire(double dFireStep, double dPulseWidth);

    // get variables
    double GetSpeed();

    // cutting job
    bool IncrementStage(double dX, double dY, double dZ, double dFeedRateSpeed);
    bool CutSlice(double dCuttingSpeed);
	static DWORD WINAPI CutPlankThread(LPVOID lpArg);
	void Stop();
	bool CutPlank();
    
    // session
	static DWORD WINAPI StairSteppingThread(LPVOID lpArg);
    void SliceSession();
    void PlankSession();
	bool StairSteppingSession();

	//
	static DWORD WINAPI ReadPositionThread( void *pvData );

	static void SaveCurState(CMotionController *pMC);
	static void LoadCurState(CMotionController *pMC);

	static void MoveStage(double dX, double dY, double dZ, LPVOID lpArg);
	
	void SetLogFilenames();
	void CommandLog(char* pszCmd);
	void GoHome();
	static void SaveStartXYZ(CMotionController *pMC, CStairStep *pSS);
	void ResetStartPos();
	bool InitStairStep();

public:
    enum eCuttingMode {
        CM_RANDOM,
        CM_FIXED,
        CM_ALTERNATING
    };

    // cutting parameters
    eCuttingMode m_CuttingMode;
    double m_dLiftHeight;
    double m_dReturnSpeed;

    double m_dSliceSizeX;
    double m_dSliceSizeY;  // slice width
    double m_dSliceSizeZ; // slice thickness
    double m_dAverageCuttingSpeed;
    double m_dDeviation;
	int m_nWaitBetweenSlice;
    int m_nSlicesPerPlank;
	//int m_nCurPlankNo;
    int m_nStairPlankDepth; // # of Planks deep for stair stepping
    int m_nStairPlankWidth; // # of Planks wide for stair stepping

//	int m_nNumCols;		// # of Columns
//	int m_nNumSlices;	// # of Slices in a stair step

//	int m_nCurCol;	// Current column number
//	double m_adCurZ[nMAX_COLS];
	//int m_anCurPlank[nMAX_COLS];

	//int m_nCurPlankNo;
	int m_nCurSliceNo; // slice # in a plank
	bool m_bStairStepCutting;
	bool m_bStairStepPause;

	//double m_dStartZPos; // only for stair step cutting (more accurate than m_dZPosFB?)
	//double m_dSafeGap; // when the knife moves y axis, it should be lifed a little more than actual need
	//int m_nMaxSlices; // maximum slices in stair step cutting --> should be added in a setting dlg

    // cutting session variables
    double m_dTotalZ;
    int m_nTotalSlices;

    // camera fire parameters
    double m_dFireStep;
    double m_dFirePulseWidth;

	HANDLE m_hEventStopStairStep;
	HANDLE m_hEventStopReadPosition;
	HANDLE m_hEventStopPlank;
	HANDLE m_hEventStopPause;
	HANDLE m_hEventStepByStep;
	bool m_bStepByStep;
	bool m_bFirstStepByStep;

	CWnd* m_pParentWnd;
	
	HANDLE m_hThread4Plank;
	HANDLE m_hThread4StairStep;
	bool m_bGo;

	// absolute:
	double m_dXPos;
	double m_dYPos;
	double m_dZPos;
	// feedback pos
	double m_dXPosFB;
	double m_dYPosFB;
	double m_dZPosFB;
	HANDLE m_hThread4ReadPosition;

	// stair step
	CStairStep *m_pStairStep;
	CString m_strStateFile;
	CString m_strXYZLogFile;
	//FILE *m_fpXYZLog;
	
	CString m_strCmdLogFile;
	CString m_strImageFileName;
	bool m_bReady4Log;
	// debug
	//FILE* m_fpLog;
};
