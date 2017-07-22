// Implementation of the ClientSocket class

#include "stdafx.h"
#include "ClientSocket.h"
#include "SocketException.h"

CClientSocket::CClientSocket ( const char* pszHostName, int nPort )
{
	if(CSocketJr::Create() == false)
		throw CSocketException("Could not create client socket");
	
	if(CSocketJr::Connect ( pszHostName, nPort ) == false)
	{
		// could not bind
		throw CSocketException ( "Could not bind to port." );
	}
}

const CClientSocket& CClientSocket::operator << ( const CDataChunk& DataChunk ) const
{
	if(!CSocketJr::Send(DataChunk))
	{
		throw CSocketException ( "Could not write to socket." );
	}

	return *this;
}


const CClientSocket& CClientSocket::operator >> ( CDataChunk& DataChunk )// const
{
	if(!CSocketJr::Recv ( DataChunk ) )
	{
		throw CSocketException ( "Could not read from socket." );
	}

	return *this;
}
