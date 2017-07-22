#pragma once


// CSettings ��ȭ �����Դϴ�.

class CSettings : public CDialog
{
	DECLARE_DYNAMIC(CSettings)

public:
	CSettings(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSettings();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
    int m_nCuttingMode;
    double m_dSliceWidth;
    double m_dSliceThickness;
    double m_dLiftHeight;
    double m_dReturnSpeed;
    double m_dAverageSpeed;
    double m_dDeviation;
    int m_nSlicePerPlank;
    int m_nPlankDepth;
    int m_nPlankWidth;
    double m_dFireStep;
    double m_dFirePulseWidth;
};
