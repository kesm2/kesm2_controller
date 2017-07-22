#include "stdafx.h"
#include <math.h>
#include "StairStep.h"

#define DOUBLE_MAX			9999	// flag

CStairStep::CStairStep()
{
	m_pdPosZ = NULL;
}

CStairStep::~CStairStep()
{
	if(m_pdPosZ)
		delete[] m_pdPosZ;
}

// m_dStartX, Y, Z should be initialized before this
void CStairStep::Init(double dBlockWidth, double dColumnWidth)
{
	m_dBlockWidth = dBlockWidth;
	m_dColumnWidth = dColumnWidth;
	
	m_nNumCols = (int)ceil(m_dBlockWidth / m_dColumnWidth);
	m_pdPosZ = new double[m_nNumCols];

	if(m_CuttingEdge == CE_LEFT)
	{
		m_nCurCol = m_nNumCols - 1;
	}
	else
	{
		m_nCurCol = 0;
	}
	// no use
	// should load CurState.txt
	for(int i = 0; i < m_nNumCols; i++)
	{
		m_pdPosZ[i] = m_dStartZ;
	}
}

/*void CStairStep::CreatePosZ(double dBlockWidth, double dColumnWidth)
{
	delete[] m_pdPosZ;
	
	Init(dBlockWidth, dColumnWidth, m_dStartZ);
}*/

double CStairStep::CalculateHeightDifferenceMinus(int nCol)
{
	if(nCol == 0)
		return -DOUBLE_MAX;
	return m_pdPosZ[nCol] - m_pdPosZ[nCol-1];
}

double CStairStep::CalculateHeightDifferencePlus(int nCol)
{
	if(nCol == m_nNumCols - 1)
		return DOUBLE_MAX;
	return m_pdPosZ[nCol+1] - m_pdPosZ[nCol];
}

// return next column number to be cut
int CStairStep::CalculateNextSection()
/*this function returns the integer index of the column from which
to cut the next block.  This calculation is based on the column heights
as well as the specified number of sections per block and section thickness.
*/
{
	double dPlankThickness = m_nPlankDepth*m_dThickness;
	double dDzDyMinus = CalculateHeightDifferenceMinus(m_nCurCol);
	double dDzDyPlus = CalculateHeightDifferencePlus(m_nCurCol);

	if(m_CuttingEdge == CE_RIGHT)
	{
		//see if the current column can be cut any more
		if(dDzDyPlus > -2*dPlankThickness &&
			dDzDyMinus < -dPlankThickness)
		return m_nCurCol;

		for(int i=m_nNumCols-1; i >= 0; i--)
		{
			dDzDyMinus = CalculateHeightDifferenceMinus(i);
			dDzDyPlus = CalculateHeightDifferencePlus(i);

			m_nCurCol = i;
			if(dDzDyMinus <= -2*dPlankThickness)
				return i;			
		}
	}
	else
	{
		//see if the current column can be cut any more
		if(dDzDyPlus > dPlankThickness &&
			dDzDyMinus < 2*dPlankThickness)
		return m_nCurCol;

		for(int i = 0; i < m_nNumCols; i++)
		{
			dDzDyMinus = CalculateHeightDifferenceMinus(i);
			dDzDyPlus = CalculateHeightDifferencePlus(i);

			m_nCurCol = i;
			if(dDzDyPlus >= 2*dPlankThickness)
				return i;			
		}
	}
	return -1; // error
}
