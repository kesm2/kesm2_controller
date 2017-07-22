// Implementation of the CSocketJr class.

#include "stdafx.h"
#include "SocketJr.h"
#include <string.h>
#include <errno.h>
#include <fcntl.h>

CSocketJr::CSocketJr() :  m_nSock ( -1 )
{
	memset ( &m_Addr, 0, sizeof ( m_Addr ) );
	m_DataChunk.m_pData = new char[MAX_DATA_CHUNK_SIZE];

#ifdef WINDOWS_XP
	WSADATA wsaData;
//	WSAStartup(0x101, &wsaData);
	WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
}

CSocketJr::~CSocketJr()
{
	if ( IsValid() )
		//::close ( m_nSock );
		closesocket(m_nSock);
	
	delete m_DataChunk.m_pData;

#ifdef WINDOWS_XP 
	WSACleanup();
#endif
}

bool CSocketJr::Create()
{
	m_nSock = socket ( AF_INET, SOCK_STREAM, 0 );

	if(!IsValid())
		return false;

	// TIME_WAIT - argh
	int on = 1;
	if ( setsockopt ( m_nSock, SOL_SOCKET, SO_REUSEADDR, ( const char* ) &on, sizeof ( on ) ) == -1 )
		return false;

	return true;
}



bool CSocketJr::Bind ( const int port )
{
	if(!IsValid())
    {
		return false;
    }
	m_Addr.sin_family = AF_INET;
	m_Addr.sin_addr.s_addr = INADDR_ANY;
	m_Addr.sin_port = htons ( port );

	if(::bind(m_nSock, (struct sockaddr *)&m_Addr, sizeof(m_Addr)) == -1)
	{
		return false;
	}

	return true;
}


bool CSocketJr::Listen() const
{
	if(!IsValid())
	{
		return false;
	}

	if(::listen ( m_nSock, MAXCONNECTIONS ) == -1)
	{
		return false;
	}

	return true;
}


bool CSocketJr::Accept ( CSocketJr& NewSocket ) const
{
	int addr_length = sizeof ( m_Addr );
//	NewSocket.m_nSock = ::accept ( m_nSock, ( sockaddr * ) &m_Addr, ( socklen_t * ) &addr_length );
	NewSocket.m_nSock = ::accept ( m_nSock, ( sockaddr * ) &m_Addr, ( int * ) &addr_length );

	if ( NewSocket.m_nSock <= 0 )
		return false;
	else
		return true;
}


bool CSocketJr::Send ( const CDataChunk& DataChunk ) const
{
//	if(::send ( m_nSock, pData, nSize, MSG_NOSIGNAL ) == -1)
	if( ::send ( m_nSock, DataChunk.m_pData, DataChunk.m_nSize, 0 ) == -1)
	{
		return false;
	}
	else
	{
		return true;
	}
}


int CSocketJr::Recv (CDataChunk& DataChunk)// const
{
	//char cBuffers[MAX_DATA_CHUNK_SIZE+1];

	m_DataChunk.m_nSize = ::recv ( m_nSock, m_DataChunk.m_pData, MAX_DATA_CHUNK_SIZE, 0 );

	if ( m_DataChunk.m_nSize == -1 || m_DataChunk.m_nSize == 0)
	{
		return 0;
	}
	else
	{
		DataChunk = m_DataChunk;
		//.m_pData = cBuffers;
		//DataChunk.m_nSize = nRecv;
		return m_DataChunk.m_nSize;
	}
}



bool CSocketJr::Connect ( const char* pszHostName, const int port )
{
	if(!IsValid())
		return false;

	m_Addr.sin_family = AF_INET;
    m_Addr.sin_addr.s_addr = inet_addr(pszHostName);
	m_Addr.sin_port = htons ( port );

/*	int status = inet_pton ( AF_INET, pszHostName, &m_Addr.sin_addr );

	if(errno == EAFNOSUPPORT ) 
		return false;
*/
	if(::connect ( m_nSock, ( sockaddr * ) &m_Addr, sizeof ( m_Addr ) ) == 0)
		return true;
	else
		return false;
}

void CSocketJr::SetNonBlocking ( const bool b )
{
/*	int opts;

	opts = fcntl ( m_nSock, F_GETFL );

	if ( opts < 0 )
	{
		return;
	}

	if ( b )
		opts = ( opts | O_NONBLOCK );
	else
		opts = ( opts & ~O_NONBLOCK );

	fcntl ( m_nSock, F_SETFL, opts );*/
}
