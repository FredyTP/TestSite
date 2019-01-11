
#include <bg/net/net.hpp>
#include <iostream>

#if BG2E_WINDOWS==1

#include <winsock2.h>
#include <WS2tcpip.h>

#pragma comment(lib,"Ws2_32.lib")

#endif

namespace bg {
namespace net {

bool init() {
#if VWNET_WINDOWS==1
	std::cout << "Initializing winsock" << std::endl;
	WSADATA wsa_data;
	if (WSAStartup(MAKEWORD(1, 1), &wsa_data) == 0) {
		std::cout << "Winsock initialization success" << std::endl;
		return true;
	}
	else {
		std::cerr << "WARNING: Winsock initialization failed" << std::endl;
		return false;
	}
#else
	return true;
#endif
}

void cleanup() {
#if VWNET_WINDOWS==1
	std::cout << "Cleaning winsock" << std::endl;
	WSACleanup();
#else
#endif
}

}
}

