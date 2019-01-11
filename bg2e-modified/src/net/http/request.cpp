
#include <bg/net/http/request.hpp>
#include <bg/net/http/head.hpp>

#include <curl/curl.h>

#include <iostream>
#include <regex>

namespace bg {
namespace net {
namespace http {

Request::Request()
	:SessionResource()
	,_method(kMethodGet)
	,_statusCode(0)
{

}

Request::Request(Session * s)
	:SessionResource(s)
	,_method(kMethodGet)
	,_statusCode(0)
{

}

Request::Request(Method m)
	:SessionResource()
	,_method(m)
	,_statusCode(0)
{

}

Request::Request(Method m, Session * s)
	:SessionResource(s)
	,_method(m)
	,_statusCode(0)
{

}

bool Request::load(const std::string & url, std::ostream & body) {
	if (!assertSession()) {
		return false;
	}
	else {
		_responseHeaders.clear();
		_statusCode = 0;
		CURL * curl = impl_cast<CURL*>(_session->getSessionHandler());
		
		CURLcode res;
		
		std::stringstream contents;
		
		if (curl) {
			
			
			switch (_method) {
				case kMethodGet:
					curl_easy_setopt(curl, CURLOPT_POST, 0);
					curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
					break;
					
				case kMethodPost:
					curl_easy_setopt(curl, CURLOPT_POST, 1);
					curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
					break;
					
				case kMethodPut:
					curl_easy_setopt(curl, CURLOPT_POST, 1);
					curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
					break;
					
				case kMethodPatch:
					curl_easy_setopt(curl, CURLOPT_POST, 1);
					curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
					break;
				
				case kMethodDelete:
					curl_easy_setopt(curl, CURLOPT_POST, 1);
					curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
					break;
					
				default:
					break;
			}
		
			std::string requestUrl = url;
			if (_params.size()>0) {
				_body = "";
				_bodyType = "application/x-www-form-urlencoded";
				std::string separator = "";
				for (auto p : _params) {
					_body += separator + p.first + "=" + p.second;
					separator = "&";
				}
			}
			
			if (!_body.empty() && _method!=kMethodGet) {
				_sendHeaders["Content-Type"] = _bodyType;
				curl_easy_setopt(curl, CURLOPT_POSTFIELDS, _body.c_str());
			}
			else if (_method==kMethodGet) {
				requestUrl += "?" + _body;
			}
			
			curl_slist * slist = nullptr;
			if (_sendHeaders.size()>0) {
				for (auto h : _sendHeaders) {
					std::string headerString = h.first + ": " + h.second;
					slist = curl_slist_append(slist, headerString.c_str());
				}
				curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
			}
			else {
				curl_easy_setopt(curl, CURLOPT_HTTPHEADER, nullptr);
			}
			
			curl_easy_setopt(curl, CURLOPT_URL, requestUrl.c_str());
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, this->readFunction);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &contents);
			curl_easy_setopt(curl, CURLOPT_HEADER, 1);
			
			res = curl_easy_perform(curl);
			
			std::string line;
			bool headerDone = false;
			while (!contents.eof()) {
				std::getline(contents, line);
				if (_statusCode==0) {
					// Read status code
					_statusCode = Head::parseStatus(line);
				}
				else if (!headerDone) {
					// Read header
					std::string k, v;
					std::regex headerRE("([a-zA-Z-]+):\\s*(.*)\r");
					std::smatch match;
					if (std::regex_search(line, match, headerRE) && match.length()>3) {
						_responseHeaders[match.str(1)] = match.str(2);
					}
					headerDone = line=="\r";
				}
				else {
					// Append to body
					body << line;
				}
			}
			
			if (slist) {
				curl_slist_free_all(slist);
			}
		}
		return true;
	}
}
	
size_t Request::readFunction(void * ptr, size_t size, size_t nmemb, void * stream) {
	std::string buffer(static_cast<char*>(ptr), size * nmemb);
	std::ostream * outStream = static_cast<std::ostream*>(stream);
	(*outStream) << buffer;
	return buffer.size();
}

}
}
}
