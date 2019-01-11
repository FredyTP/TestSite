#ifndef _bg2e_net_socket_hpp_
#define _bg2e_net_socket_hpp_

#include <bg/net/net.hpp>
#include <bg/net/url.hpp>

#if BG2E_WINDOWS==1

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>

#else

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#endif


namespace bg {
namespace net {
    
#if BG2E_WINDOWS==1

typedef SOCKET native_socket_t;

#else

typedef int native_socket_t;

#endif

class BG2E_EXPORT Socket {
public:
	enum Type {
		kTypeTcp = SOCK_STREAM,
		kTypeUdp = SOCK_DGRAM
	};

	enum Flags {
		kFlagPassive = AI_PASSIVE,
		kFlagCanonName = AI_CANONNAME,
		kFlagNumericHost = AI_NUMERICHOST
	};

	enum Error {
		kNoError = 0,
		kErrorGetAddressInfo,
		kErrorSocketCreate,
		kErrorSocketBind,
		kErrorListen,
		kErrorConnect
	};

	Socket(Type t);
	Socket(Type t, Flags f);
	Socket(native_socket_t ns, Type t);
	~Socket();

	int send(const unsigned char * buffer, size_t bufferSize);
	int receive(unsigned char * buffer, size_t bufferSize);

	void close();

protected:
	Socket();

	native_socket_t _socket;
	Type _type;
	int _flags;
};

class BG2E_EXPORT ClientSocket : public Socket {
public:
	ClientSocket(Type t);
	virtual ~ClientSocket();

	inline Error connectTo(const Url & remoteHost) { return connectTo(remoteHost.getServer(), static_cast<unsigned short>(remoteHost.getPort())); }
	inline Error connectTo(const std::string & address, int port) { return connectTo(address, static_cast<unsigned short>(port)); }
	Error connectTo(const std::string & address, unsigned short port);
};

class BG2E_EXPORT ServerSocket : public Socket {
public:
	ServerSocket(Type t);
	virtual ~ServerSocket();

	Error create(int port, int backlog = 100);
	Socket * accept();

};

}
}

#endif
