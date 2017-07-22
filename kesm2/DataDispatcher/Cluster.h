#ifndef _CLUSTER_H_
#define _CLUSTER_H_

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class CClusterID
{
public:
    string m_strIP;
    int m_nPort;
	int m_nID;

    CClusterID() : m_strIP(""), m_nPort(0), m_nID(0) {}
    CClusterID(int nID, string strIP, int nPort) : m_strIP(strIP), m_nPort(nPort), m_nID(nID) { }
};

class CCluster
{
public:
	enum eClusterID {
		CLUSTER_0_E,
		CLUSTER_1_E,
		CLUSTER_2_E,
		CLUSTER_3_E
	};

// Construction
public:
	CCluster() {};

	bool Add(eClusterID nID, string strIP, int nPort); // return m_nID
	bool Get(eClusterID nID, CClusterID& ClusterID);
	//bool Delete();

// Implemetation
protected:
	vector<CClusterID> m_Clusters;
};

#endif /* _CLUSTER_H_ */
