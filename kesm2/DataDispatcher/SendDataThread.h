#pragma once



// CSendDataThread

class CSendDataThread : public CWinThread
{
	DECLARE_DYNCREATE(CSendDataThread)

protected:
	CSendDataThread();           // protected constructor used by dynamic creation
	virtual ~CSendDataThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};


