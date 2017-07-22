// Definition of the ClientSocket class

#ifndef _CLIENTSOCKET_H_
#define _CLIENTSOCKET_H_

#include "SocketJr.h"

class CClientSocket : private CSocketJr
{
public:
	CClientSocket();
	CClientSocket(const char* pszHostName, int port );
	virtual ~CClientSocket(){};

	const CClientSocket& operator << ( const CDataChunk& ) const;
	const CClientSocket& operator >> ( CDataChunk& );// const;
};

#endif // _CLIENTSOCKET_H_
