// Definition of the ServerSocket class

#ifndef _SERVERSOCKET_H_
#define _SERVERSOCKET_H_

#include "SocketJr.h"

class CServerSocket : private CSocketJr
{
public:
	CServerSocket(int nPort );
	CServerSocket() {};
	virtual ~CServerSocket();

	const CServerSocket& operator << ( const CDataChunk& ) const;
	const CServerSocket& operator >> ( CDataChunk& ); //const;

	void Accept ( CServerSocket& );
};


#endif
