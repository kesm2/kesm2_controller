// SocketException class


#ifndef _SOCKETEXCEPTION_H_
#define _SOCKETEXCEPTION_H_

#include <string>

class CSocketException
{
public:
	CSocketException ( std::string s ) : m_s ( s ) {};
	~CSocketException (){};

	std::string description() { return m_s; }

private:

	std::string m_s;
};

#endif // _SOCKETEXCEPTION_H_
