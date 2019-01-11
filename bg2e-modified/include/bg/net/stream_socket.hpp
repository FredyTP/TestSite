
#ifndef _bg2e_net_streamsocket_hpp_
#define _bg2e_net_streamsocket_hpp_

#include <bg/net/socket.hpp>
#include <bg/base/exception.hpp>

namespace bg {
namespace net {

class BG2E_EXPORT StreamSocket {
public:
	StreamSocket(Socket * s);
	~StreamSocket();
	
	StreamSocket & operator>> (char& val);
	StreamSocket & operator>> (bool& val);
	StreamSocket & operator>> (short& val);
	StreamSocket & operator>> (unsigned short& val);
	StreamSocket & operator>> (int& val);
	StreamSocket & operator>> (unsigned int& val);
	StreamSocket & operator>> (long& val);
	StreamSocket & operator>> (unsigned long& val);
	StreamSocket & operator>> (long long& val);
	StreamSocket & operator>> (unsigned long long& val);
	StreamSocket & operator>> (float& val);
	StreamSocket & operator>> (double& val);
	StreamSocket & operator>> (std::string & val);
	
	StreamSocket & operator<< (char val);
	StreamSocket & operator<< (bool val);
	StreamSocket & operator<< (short val);
	StreamSocket & operator<< (unsigned short val);
	StreamSocket & operator<< (int val);
	StreamSocket & operator<< (unsigned int val);
	StreamSocket & operator<< (long val);
	StreamSocket & operator<< (unsigned long val);
	StreamSocket & operator<< (long long val);
	StreamSocket & operator<< (unsigned long long val);
	StreamSocket & operator<< (float val);
	StreamSocket & operator<< (double val);
	StreamSocket & operator<< (const std::string & val);
	
protected:
	Socket * _socket;
};

}
}

#endif
