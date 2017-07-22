// Implementation of the ServerSocket class
#include "stdafx.h"
#include "ServerSocket.h"
#include "SocketException.h"


CServerSocket::CServerSocket ( int nPort )
{
	if ( ! CSocketJr::Create() )
	{
		throw CSocketException ( "Could not create server socket." );
	}

	if ( ! CSocketJr::Bind ( nPort ) )
	{
		throw CSocketException ( "Could not bind to port." );
	}

	if ( ! CSocketJr::Listen() )
	{
		throw CSocketException ( "Could not listen to socket." );
	}

}

CServerSocket::~CServerSocket()
{
}


const CServerSocket& CServerSocket::operator << ( const CDataChunk& DataChunk ) const
{
	if ( ! CSocketJr::Send ( DataChunk ) )
	{
		throw CSocketException ( "Could not write to socket." );
	}

	return *this;

}


const CServerSocket& CServerSocket::operator >> ( CDataChunk& DataChunk ) //const
{
	if ( ! CSocketJr::Recv ( DataChunk ) )
	{
		throw CSocketException ( "Could not read from socket." );
	}

	return *this;
}

void CServerSocket::Accept ( CServerSocket& Sock )
{
	if ( ! CSocketJr::Accept ( Sock ) )
	{
		throw CSocketException ( "Could not accept socket." );
	}
}
