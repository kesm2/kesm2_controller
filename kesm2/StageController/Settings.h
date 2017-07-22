#pragma once


// CSettings 대화 상자입니다.

class CSettings : public CDialog
{
	DECLARE_DYNAMIC(CSettings)

public:
	CSettings(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSettings();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
