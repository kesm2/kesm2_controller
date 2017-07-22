#pragma once

#include "StageControllerDlg.h"


// CSettingsDlg ��ȭ �����Դϴ�.

class CSettingsDlg : public CDialog
{
	DECLARE_DYNAMIC(CSettingsDlg)

public:
	CSettingsDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSettingsDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SETTINGS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
    int m_nCuttingMode;
    double m_dSliceWidth;
    double m_dSliceThickness;
    double m_dSliceLength;
    double m_dLiftHeight;
    double m_dReturnSpeed;
    double m_dAverageSpeed;
    double m_dDeviation;
	int m_nWaitBetweenSlice; // seconds
    int m_nSlicePerPlank;
    int m_nPlankDepth;
    int m_nPlankWidth;
    double m_dFireStep;
    double m_dFirePulseWidth;

protected:
    CStageControllerDlg *m_pParentDlg;

protected:
    virtual void OnOK();
	void CSettingsDlg::SaveData();
public:
	afx_msg void OnBnClickedButtonApply();
//	afx_msg void OnBnClickedCancel();
//	afx_msg void OnClose();
//	afx_msg void OnDestroy();
//	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	UINT m_nNumCols;
	UINT m_nNumSlices;
	double m_dBlockWidth;
	double m_dSliceThicknessSS;
	double m_dColumnWidth;
	UINT m_nPlankDepthSS;
	int m_nCuttingEdgeLeft;
};
