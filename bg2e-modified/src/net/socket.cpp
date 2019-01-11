
#include <bg/net/socket.hpp>

#if BG2E_WINDOWS==1

static const bg::net::native_socket_t kNoSocket = INVALID_SOCKET;
static const int kSocketError = SOCKET_ERROR;

#else

static const bg::net::native_socket_t kNoSocket = -1;
static const int kSocketError = -1;

#endif

namespace bg {
namespace net {


Socket::Socket()
	:_socket(kNoSocket)
	, _type(kTypeTcp)
	, _flags(0)
{

}

Socket::Socket(Type t)
	:_socket(kNoSocket)
	, _type(t)
	, _flags(0)
{

}

Socket::Socket(Type t, Flags f)
	:_socket(kNoSocket)
	, _type(t)
	, _flags(f)
{

}

Socket::Socket(native_socket_t ns, Type t)
	:_socket(ns)
	, _type(t)
{

}

Socket::~Socket() {
	close();
}

int Socket::send(const unsigned char * buffer, size_t bufferSize) {
	int result = -1;

	if (_socket != kNoSocket) {
#if BG2E_WINDOWS==1
		result = static_cast<int>(::send(_socket, reinterpret_cast<const char*>(buffer), static_cast<int>(bufferSize), 0));
#else
		result = static_cast<int>(::send(_socket, reinterpret_cast<const char*>(buffer), bufferSize, 0));
#endif		
	}

	return result;
}

int Socket::receive(unsigned char * buffer, size_t bufferSize) {
	int result = -1;

	if (_socket != kNoSocket) {
#if BG2E_WINDOWS==1
		result = static_cast<int>(recv(_socket, reinterpret_cast<char*>(buffer), static_cast<int>(bufferSize), 0));
#else
		result = static_cast<int>(recv(_socket, reinterpret_cast<char*>(buffer), bufferSize, 0));
#endif
	}

	return result;
}

void Socket::close() {
	if (_socket != kNoSocket) {
#if BG2E_WINDOWS==1
		if (shutdown(_socket, SD_BOTH) == 0) {
			closesocket(_socket);
		}
#else
		if (shutdown(_socket, SHUT_RDWR) == 0) {
			::close(_socket);
		}
#endif
	}
	_socket = kNoSocket;
}


ClientSocket::ClientSocket(Type t)
	:Socket(t)
{

}

ClientSocket::~ClientSocket() {

}

Socket::Error ClientSocket::connectTo(const std::string & address, unsigned short port) {
	std::string str_port = std::to_string(port);

	addrinfo * addr = nullptr, hints;
	memset(&hints, 0, sizeof(addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = _type;
	hints.ai_protocol = _type == kTypeTcp ? IPPROTO_TCP : IPPROTO_UDP;
	hints.ai_flags = _flags;

	int result = getaddrinfo(address.c_str(), str_port.c_str(), &hints, &addr);
	if (result != 0) {
		return kErrorGetAddressInfo;
	}

	_socket = kNoSocket;
	_socket = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
	if (_socket == kNoSocket) {
		freeaddrinfo(addr);
		return kErrorSocketCreate;
	}

	result = connect(_socket, addr->ai_addr, static_cast<int>(addr->ai_addrlen));
	if (result == kSocketError) {
		freeaddrinfo(addr);
		close();
		_socket = kNoSocket;
		return kErrorConnect;
	}

	freeaddrinfo(addr);
	return kNoError;
}

ServerSocket::ServerSocket(Type t)
	:Socket(t, kFlagPassive)
{

}

ServerSocket::~ServerSocket() {

}

Socket::Error ServerSocket::create(int port, int backlog) {
	std::string str_port = std::to_string(port);

	addrinfo * addr = nullptr, hints;
	memset(&hints, 0, sizeof(addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = _type;
	hints.ai_protocol = _type==kTypeTcp ? IPPROTO_TCP:IPPROTO_UDP;
	hints.ai_flags = _flags;

	int result = getaddrinfo(nullptr, str_port.c_str(), &hints, &addr);
	if (result != 0) {
		return kErrorGetAddressInfo;
	}

	_socket = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
	if (_socket == kNoSocket) {
		freeaddrinfo(addr);
		return kErrorSocketCreate;
	}

	result = bind(_socket, addr->ai_addr, static_cast<int>(addr->ai_addrlen));
	if (result == kSocketError) {
		freeaddrinfo(addr);
		close();
		return kErrorSocketBind;
	}

	freeaddrinfo(addr);
	if (listen(_socket, backlog) == kSocketError) {
		close();
		return kErrorListen;
	}

	return kNoError;
}

Socket * ServerSocket::accept() {
	native_socket_t nativeClientSocket = kNoSocket;
	nativeClientSocket = ::accept(_socket, nullptr, nullptr);
	if (nativeClientSocket == kNoSocket) {
		close();
		return nullptr;
	}

	return new Socket(nativeClientSocket, _type);
}

}
}
