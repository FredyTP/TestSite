
#include <bg/net/session.hpp>

#include <curl/curl.h>

#include <string>

namespace bg {
namespace net {

Session::Session()
	:_sessionHandler(nullptr)
{

}

Session::~Session() {
	destroy();
}

void Session::start() {
	destroy();
	CURL * curl = curl_easy_init();
	_sessionHandler = curl;
	
	std::string cookieFile = "cookieFile";
	curl_easy_setopt(curl, CURLOPT_COOKIEFILE, cookieFile.c_str());
	curl_easy_setopt(curl, CURLOPT_COOKIEJAR, cookieFile.c_str());
}

void Session::destroy() {
	if (_sessionHandler) {
		CURL* curl = impl_cast<CURL*>(_sessionHandler);
		curl_easy_cleanup(curl);
		_sessionHandler = nullptr;
	}
}

}
}
