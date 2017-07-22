// SendDataThread.cpp : implementation file
//

#include "stdafx.h"
#include "DataDispatcher.h"
#include "SendDataThread.h"


// CSendDataThread

IMPLEMENT_DYNCREATE(CSendDataThread, CWinThread)

CSendDataThread::CSendDataThread()
{
}

CSendDataThread::~CSendDataThread()
{
}

BOOL CSendDataThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CSendDataThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CSendDataThread, CWinThread)
END_MESSAGE_MAP()


// CSendDataThread message handlers
