#include "stdafx.h"
#include "Cluster.h"

/////////////////////////////////////////////////////////////////////////////
// CCluster
bool CCluster::Add(eClusterID nID, string strIP, int nPort)
{
	m_Clusters.push_back(CClusterID(nID, strIP, nPort));
	return true;
}

bool CCluster::Get(eClusterID nID, CClusterID& ClusterID)
{
	vector<CClusterID>::iterator it;
	//vector<CClusterID> *pClusterID;
	int i;

	for(i = 0, it = m_Clusters.begin(); it != m_Clusters.end(); it++, i++)
	{
		if(it->m_nID == nID)
		{
			ClusterID = *it;
			return true;
		}
	}
	return false;
}