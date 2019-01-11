
#ifndef _bg2e_net_http_request_hpp_
#define _bg2e_net_http_request_hpp_

#include <bg/net/session.hpp>
#include <bg/net/url.hpp>

#include <ostream>
#include <sstream>
#include <map>

namespace bg {
namespace net {
namespace http {
	
typedef std::map<std::string,std::string> Headers;
typedef std::map<std::string,std::string> Parameters;

class BG2E_EXPORT Request : public SessionResource {
public:
	enum Method {
		kMethodGet = 1,
		kMethodPost,
		kMethodPatch,
		kMethodPut,
		kMethodDelete,
		kMethodHead
	};
	

	Request();
	Request(Session *);
	Request(Method);
	Request(Method, Session *);

	inline void setHeader(const std::string & headerName, const std::string & value) { _sendHeaders[headerName] = value; }
	
	// parameters are sent appending them to the URL for GET request, and x-www-form-urlencoded for the rest of them (POST, PUT, PATCH, etc)
	inline void setParameter(const std::string & paramName, const std::string & value) { _params[paramName] = value; }
	
	// The body is ignored if any parameter is set. To send raw body, do not set any parameter.
	inline void setBody(const std::string & body, const std::string & bodyType = "application/x-www-form-urlencoded") { _body = body; _bodyType = bodyType; }
	
	inline bool load(const Url & url, std::ostream & body) { return load(url.getString(), body); }
	bool load(const std::string &, std::ostream & body);
	
	inline int getStatusCode() const { return _statusCode; }
	
	inline const Headers & responseHeaders() const { return _responseHeaders; }
	inline void eachHeader(std::function<void(const std::string &,const std::string&)> closure) {
		for (auto header : _responseHeaders) {
			closure(header.first,header.second);
		}
	}

protected:

	Method _method;
	Headers _sendHeaders;
	Parameters _params;
	std::string _bodyType;
	std::string _body;
	
	int _statusCode;
	Headers _responseHeaders;
	static size_t readFunction(void * ptr, size_t size, size_t nmemb, void * stream);
};

}
}
}

#endif
