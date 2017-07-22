#pragma once

#define dMAX_BLOCK_HEIGHT	12		// 12 mm

class CStairStep
{
public:
	CStairStep();
	~CStairStep();

	void Init(double dBlockWidth, double dColumnWidth);
	//void CreatePosZ(double dBlockWidth, double dColumnWidth);

	double CalculateHeightDifferenceMinus(int nCol);
	double CalculateHeightDifferencePlus(int nCol);
	int CalculateNextSection();


public:
	enum eCuttingEdge {
		CE_LEFT,
		CE_RIGHT
	};

	eCuttingEdge m_CuttingEdge;

	double m_dBlockWidth;
	double m_dColumnWidth;
	int m_nNumCols;
	double m_dThickness;
	int m_nPlankDepth;

	double *m_pdPosZ;
	double m_dStartX, m_dStartY, m_dStartZ;
	int m_nCurCol;
};