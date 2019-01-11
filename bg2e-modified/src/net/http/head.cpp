//
//  head.cpp
//  VitaminewNET
//
//  Created by Fernando Serrano Carpena on 17/10/15.
//
//

#include <bg/net/http/head.hpp>

#include <iostream>
#include <fstream>
#include <chrono>
#include <regex>
#include <sstream>

#include <curl/curl.h>

namespace bg {
namespace net {
namespace http {
	
Head::Head()
	:SessionResource()
	,_status(0)
	,_contentLength(-1)
{
	
}

Head::Head(Session * s)
	:SessionResource(s)
	,_status(0)
	,_contentLength(-1)
{

}
	
Head::~Head() {
	
}
	
int Head::load(const std::string & src) {
	if (!assertSession()) return 0;

	_src = src;
	CURL * curl = impl_cast<CURL*>(_session->getSessionHandler());

	CURLcode res;
	
	std::stringstream headers;
	
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, _src.c_str());
		
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, this->headerFunction);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &headers);
		curl_easy_setopt(curl, CURLOPT_HEADER, 1);
		curl_easy_setopt(curl, CURLOPT_NOBODY, 1);

		res = curl_easy_perform(curl);
		_headData = headers.str();
		
		parseHeaders();
	}
	
	return _status;
}

int Head::parseStatus(const std::string & headerText) {
	int status = 0;
	std::regex statRegEx("HTTP.{4} ([0-9]{3}) (.*)", std::regex_constants::ECMAScript | std::regex_constants::icase);
	std::smatch match;

	if (std::regex_search(headerText, match, statRegEx) && match.size()>1) {
		status = atoi(match.str(1).c_str());
	}

	return status;
}

bool Head::parseHeader(const std::string & headerText, const std::string & headerName, std::string & result) {
	bool headerFound = false;
	std::regex headerRE(headerName + ":(.*)", std::regex_constants::ECMAScript | std::regex_constants::icase);
	std::smatch match;
	if (std::regex_search(headerText, match, headerRE) && match.size()>1) {
		result = match.str(1);
		headerFound = true;
	}

	return headerFound;
}

size_t Head::headerFunction(void * ptr, size_t size, size_t nmemb, void * stream) {
	std::string buffer(static_cast<char*>(ptr), size * nmemb);
	std::stringstream *sstream = static_cast<std::stringstream*>(stream);
	(*sstream) << buffer;
	
	return buffer.size();
}
	
void Head::parseHeaders() {
	std::regex statRegEx("HTTP.{4} ([0-9]{3}) (.*)", std::regex_constants::ECMAScript | std::regex_constants::icase);
	std::smatch match;
	if (std::regex_search(_headData, match, statRegEx) && match.size()>2) {
		_status = atoi(match.str(1).c_str());
		_statusSting = match.str(2);
	}
	
	std::regex contentLengthRegEx("Content-Length: ([0-9]+)", std::regex_constants::ECMAScript | std::regex_constants::icase);
	if (std::regex_search(_headData, match, contentLengthRegEx) && match.size()>1) {
		_contentLength = atoll(match.str(1).c_str());
	}
	
	std::regex contentTypeRegEx("Content-Type: (.*)", std::regex_constants::ECMAScript | std::regex_constants::icase);
	if (std::regex_search(_headData, match, contentLengthRegEx) && match.size()>1) {
		_contentType = match.str(1);
	}
}

}
}
}
