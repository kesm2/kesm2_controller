// Settings.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "StageController.h"
#include "Settings.h"


// CSettings 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSettings, CDialog)
CSettings::CSettings(CWnd* pParent /*=NULL*/)
	: CDialog(CSettings::IDD, pParent)
{
    m_nCuttingMode;
    m_dSliceWidth;
    m_dSliceThickness;
    m_dLiftHeight;
    m_dReturnSpeed;
    m_dAverageSpeed;
    m_dDeviation;
    m_nSlicePerPlank;
    m_nPlankDepth;
    m_nPlankWidth;
    m_dFireStep;
    m_dFirePulseWidth;
}

CSettings::~CSettings()
{
}

void CSettings::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_CBIndex(pDX, IDC_COMBO_CUTTING_MODE, m_nCuttingMode);
    DDX_Text(pDX, IDC_EDIT_SLICE_WIDTH, m_dSliceWidth);
    DDX_Text(pDX, IDC_EDIT_SLICE_THICKNESS, m_dSliceThickness);
    DDX_Text(pDX, IDC_EDIT_LIFT_HEIGHT, m_dLiftHeight);
    DDX_Text(pDX, IDC_EDIT_RETURN_SPEED, m_dReturnSpeed);
    DDX_Text(pDX, IDC_EDIT_AVERAGE_SPEED, m_dAverageSpeed);
    DDX_Text(pDX, IDC_EDIT_DEVIATION_SPEED, m_dDeviation);
    DDX_Text(pDX, IDC_EDIT_SLICE_PER_PLANK, m_nSlicePerPlank);
    DDX_Text(pDX, IDC_EDIT_PLANK_DEPTH, m_nPlankDepth);
    DDX_Text(pDX, IDC_EDIT_PLANK_WIDTH, m_nPlankWidth);
    DDX_Text(pDX, IDC_EDIT_FIRE_STEP, m_dFireStep);
    DDX_Text(pDX, IDC_EDIT_FIRE_PULSE_WIDTH, m_dFirePulseWidth);
}


BEGIN_MESSAGE_MAP(CSettings, CDialog)
END_MESSAGE_MAP()


// CSettings 메시지 처리기입니다.
