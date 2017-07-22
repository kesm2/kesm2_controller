// Definition of the Socket class

#ifndef _SOCKET_H_
#define _SOCKET_H_

//const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 5;
//const int MAXRECV = 500;

const int MAX_DATA_CHUNK_SIZE		(1024);		// 1k

class CDataChunk
{
public:
	char* m_pData;
	int m_nSize;
};

class CSocketJr
{
public:
	CSocketJr();
	virtual ~CSocketJr();

	// Server initialization
	bool Create();
	bool Bind ( const int port );
	bool Listen() const;
	bool Accept ( CSocketJr& ) const;

	// Client initialization
	bool Connect ( const char* pszHostName, const int port );

	// Data Transimission
	bool Send(const CDataChunk& Data) const;
	int Recv(CDataChunk& Data);// const;

	void SetNonBlocking( const bool );

	bool IsValid() const { return m_nSock != -1; }

 protected:
	int m_nSock;
	sockaddr_in m_Addr;
	CDataChunk m_DataChunk;
};


#endif // _SOCKET_H_
