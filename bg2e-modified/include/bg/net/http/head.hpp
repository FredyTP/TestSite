//
//  head.hpp
//  VitaminewNET
//
//  Created by Fernando Serrano Carpena on 17/10/15.
//
//

#ifndef _bg2e_net_http_head_hpp_
#define _bg2e_net_http_head_hpp_


#include <bg/export.hpp>
#include <bg/net/session.hpp>

#include <string>
#include <thread>
#include <mutex>
#include <unordered_map>

namespace bg {
namespace net {
namespace http {
	
class BG2E_EXPORT Head : public SessionResource {
public:
	Head();
	Head(Session * s);
	~Head();
	
	int load(const std::string & src);
	
	inline const std::string & getSource() const { return _src; }
	
	inline int getStatus() const { return _status; }
	inline const std::string & getStatusString() const { return _statusSting; }
	inline size_t getContentLength() const { return _contentLength; }
	inline const std::string & getContentType() const { return _contentType; }
	
	static int parseStatus(const std::string & headerText);
	static bool parseHeader(const std::string & headerText, const std::string & headerName, std::string & result);

protected:
	std::string _headData;
	std::string _src;
	
	int _status;
	std::string _statusSting;
	size_t _contentLength;
	std::string _contentType;

	static size_t headerFunction(void * ptr, size_t size, size_t nmemb, void * stream);
	
	void parseHeaders();
};
	
}
}
}

#endif /* head_h */
