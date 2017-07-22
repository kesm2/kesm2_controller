// SettingsDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "StageController.h"
#include "SettingsDlg.h"
#include ".\settingsdlg.h"

// CSettingsDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSettingsDlg, CDialog)
CSettingsDlg::CSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingsDlg::IDD, pParent)
{
    m_pParentDlg = (CStageControllerDlg *)pParent;

    m_nCuttingMode = (int)m_pParentDlg->m_pMotionController->m_CuttingMode;
    m_dSliceWidth = m_pParentDlg->m_pMotionController->m_dSliceSizeY;
    m_dSliceThickness = m_pParentDlg->m_pMotionController->m_dSliceSizeZ;
    m_dSliceLength = m_pParentDlg->m_pMotionController->m_dSliceSizeX;

    m_dLiftHeight = m_pParentDlg->m_pMotionController->m_dLiftHeight;
    m_dReturnSpeed = m_pParentDlg->m_pMotionController->m_dReturnSpeed;
    m_dAverageSpeed = m_pParentDlg->m_pMotionController->m_dAverageCuttingSpeed;
    m_dDeviation = m_pParentDlg->m_pMotionController->m_dDeviation;
    m_nWaitBetweenSlice = m_pParentDlg->m_pMotionController->m_nWaitBetweenSlice;

    m_nSlicePerPlank = m_pParentDlg->m_pMotionController->m_nSlicesPerPlank;
    m_nPlankDepth = m_pParentDlg->m_pMotionController->m_nStairPlankDepth;
    m_nPlankWidth = m_pParentDlg->m_pMotionController->m_nStairPlankWidth;
    m_dFireStep = m_pParentDlg->m_pMotionController->m_dFireStep;
    m_dFirePulseWidth = m_pParentDlg->m_pMotionController->m_dFirePulseWidth;

	// stair step
	//m_nNumCols = m_pParentDlg->m_pMotionController->m_nNumCols;
	//m_nNumSlices = m_pParentDlg->m_pMotionController->m_nNumSlices;
	if(m_pParentDlg->m_pMotionController->m_pStairStep->m_CuttingEdge == CStairStep::CE_LEFT)
		m_nCuttingEdgeLeft = 0;
	else
		m_nCuttingEdgeLeft = 1;
	m_dBlockWidth = m_pParentDlg->m_pMotionController->m_pStairStep->m_dBlockWidth;
	m_dSliceThicknessSS = m_pParentDlg->m_pMotionController->m_pStairStep->m_dThickness;
	m_dColumnWidth = m_pParentDlg->m_pMotionController->m_pStairStep->m_dColumnWidth;
	m_nPlankDepthSS = m_pParentDlg->m_pMotionController->m_pStairStep->m_nPlankDepth;
}

CSettingsDlg::~CSettingsDlg()
{
}

void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_CUTTING_MODE, m_nCuttingMode);
	DDX_Text(pDX, IDC_EDIT_SLICE_WIDTH, m_dSliceWidth);
	DDX_Text(pDX, IDC_EDIT_SLICE_THICKNESS, m_dSliceThickness);
	DDX_Text(pDX, IDC_EDIT_SLICE_LENGTH, m_dSliceLength);
	DDX_Text(pDX, IDC_EDIT_LIFT_HEIGHT, m_dLiftHeight);
	DDX_Text(pDX, IDC_EDIT_RETURN_SPEED, m_dReturnSpeed);
	DDX_Text(pDX, IDC_EDIT_AVERAGE_SPEED, m_dAverageSpeed);
	DDX_Text(pDX, IDC_EDIT_DEVIATION_SPEED, m_dDeviation);
	DDX_Text(pDX, IDC_EDIT_WAIT, m_nWaitBetweenSlice);
	DDX_Text(pDX, IDC_EDIT_SLICE_PER_PLANK, m_nSlicePerPlank);
	DDX_Text(pDX, IDC_EDIT_PLANK_DEPTH, m_nPlankDepth);
	DDX_Text(pDX, IDC_EDIT_PLANK_WIDTH, m_nPlankWidth);
	DDX_Text(pDX, IDC_EDIT_FIRE_STEP, m_dFireStep);
	DDX_Text(pDX, IDC_EDIT_FIRE_PULSE_WIDTH, m_dFirePulseWidth);
	DDX_Text(pDX, IDC_EDIT_SS_BLOCK_WIDTH, m_dBlockWidth);
	DDX_Text(pDX, IDC_EDIT_SS_SLICE_THICKNESS, m_dSliceThicknessSS);
	DDX_Text(pDX, IDC_EDIT_SS_COLUMN_WIDTH, m_dColumnWidth);
	DDX_Text(pDX, IDC_EDIT_SS_PLANK_DEPTH, m_nPlankDepthSS);
	DDX_Radio(pDX, IDC_RADIO_CE_LEFT, m_nCuttingEdgeLeft);
}


BEGIN_MESSAGE_MAP(CSettingsDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnBnClickedButtonApply)
//	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
//ON_WM_CLOSE()
//ON_WM_DESTROY()
//ON_BN_CLICKED(IDOK, OnBnClickedOk)
ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CSettingsDlg 메시지 처리기입니다.

void CSettingsDlg::SaveData()
{
    UpdateData();

    m_pParentDlg->m_pMotionController->m_CuttingMode = (CMotionController::eCuttingMode)m_nCuttingMode;
    m_pParentDlg->m_pMotionController->m_dSliceSizeY = m_dSliceWidth;
    m_pParentDlg->m_pMotionController->m_dSliceSizeZ = m_dSliceThickness;
    m_pParentDlg->m_pMotionController->m_dSliceSizeX = m_dSliceLength;

    m_pParentDlg->m_pMotionController->m_dLiftHeight = m_dLiftHeight;
    m_pParentDlg->m_pMotionController->m_dReturnSpeed = m_dReturnSpeed;
    m_pParentDlg->m_pMotionController->m_dAverageCuttingSpeed = m_dAverageSpeed;
    m_pParentDlg->m_pMotionController->m_dDeviation = m_dDeviation;
    m_pParentDlg->m_pMotionController->m_nWaitBetweenSlice = m_nWaitBetweenSlice;
	
    m_pParentDlg->m_pMotionController->m_nSlicesPerPlank = m_nSlicePerPlank;
    m_pParentDlg->m_pMotionController->m_nStairPlankDepth = m_nPlankDepth;
    m_pParentDlg->m_pMotionController->m_nStairPlankWidth = m_nPlankWidth;
    m_pParentDlg->m_pMotionController->m_dFireStep = m_dFireStep;
    m_pParentDlg->m_pMotionController->m_dFirePulseWidth = m_dFirePulseWidth;

//    m_pParentDlg->m_pMotionController->m_nNumCols = m_nNumCols;
//    m_pParentDlg->m_pMotionController->m_nNumSlices = m_nNumSlices;
	
	if(m_nCuttingEdgeLeft == 0)
		m_pParentDlg->m_pMotionController->m_pStairStep->m_CuttingEdge = CStairStep::CE_LEFT;
	else
		m_pParentDlg->m_pMotionController->m_pStairStep->m_CuttingEdge = CStairStep::CE_RIGHT;
	m_pParentDlg->m_pMotionController->m_pStairStep->m_dBlockWidth = m_dBlockWidth;
	m_pParentDlg->m_pMotionController->m_pStairStep->m_dThickness = m_dSliceThicknessSS;
	m_pParentDlg->m_pMotionController->m_pStairStep->m_dColumnWidth = m_dColumnWidth;
	m_pParentDlg->m_pMotionController->m_pStairStep->m_nPlankDepth = m_nPlankDepthSS;

	CString strFileName;

	strFileName.Format("%s\\%s", m_pParentDlg->m_strFolderName, szFILE4PARAMS);
	m_pParentDlg->SaveParameters(strFileName.GetBuffer());
}

void CSettingsDlg::OnOK()
{
    // TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	SaveData();
	
	m_pParentDlg->m_fSetDlgOpen = false;
    CDialog::OnOK();
}

void CSettingsDlg::OnBnClickedButtonApply()
{
	// TODO: Add your control notification handler code here
	SaveData();
}

void CSettingsDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	m_pParentDlg->m_fSetDlgOpen = false;
	OnCancel();
}
