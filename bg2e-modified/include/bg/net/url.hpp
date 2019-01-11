#ifndef _bg2e_net_url_hpp_
#define _bg2e_net_url_hpp_

#include <bg/export.hpp>

#include <string>
#include <map>
#include <functional>

namespace bg {
namespace net {

typedef std::map<std::string, std::string> UrlParams;

class BG2E_EXPORT Url {
public:
	Url();
	Url(const std::string & server);
	Url(const std::string & server, const std::string & path);
	Url(const std::string & protocol, const std::string & server, int port);
	Url(const std::string & protocol, const std::string & server, int port, const std::string & path);
	Url(const std::string & server, int port, const std::string & path);
	Url(const std::string & server, int port);

	inline bool isValid() const { return _isValid; }
	inline const std::string & getString() const { return _fullUrl; }

	inline void setProtocol(const std::string & protocol) { _protocol = protocol; buildUrl(); }
	inline void setServer(const std::string & server) { _server = server; buildUrl(); }
	inline void setPort(int port) { _port = port; buildUrl(); }
	inline void setPath(const std::string & path) { _path = path; buildUrl(); }
	inline void appendPath(const std::string & path) {
		if (_path.size() == 0) { _path = path; }
		else if (_path.back() == '/' && path.front() == '/') {
			_path.pop_back();
			_path += path;
		}
		else if (_path.back() == '/' || path.front()=='/') { _path += path; }
		else { _path += "/" + path; }
		buildUrl();
	}
	inline void setUrlParameter(const std::string & key, const std::string & value) { _urlParameters[key] = value; buildUrl(); }
	inline void setHashParameter(const std::string & key, const std::string & value) { _hashParameters[key] = value; buildUrl(); }

	inline const std::string & getProtocol() const { return _protocol; }
	inline const std::string & getServer() const { return _server; }
	inline int getPort() const { return _port; }
	inline const std::string & getPath() const { return _path; }
	inline const UrlParams & getUrlParameters() const { return _urlParameters; }
	inline const UrlParams & getHashParameter() const { return _hashParameters; }
	inline void eachUrlParameter(std::function<void(const std::string & key, const std::string & value)> closure) {
		for (auto kv : _urlParameters) {
			closure(kv.first, kv.second);
		}
	}
	inline void eachHashParameter(std::function<void(const std::string & key, const std::string & value)> closure) {
		for (auto kv : _hashParameters) {
			closure(kv.first, kv.second);
		}
	}

	inline void operator=(const Url & url) {
		_protocol = url._protocol;
		_server = url._server;
		_port = url._port;
		_path = url._path;
		_urlParameters = url._urlParameters;
		_hashParameters = url._hashParameters;
		_isValid = url._isValid;
		_fullUrl = url._fullUrl;
	}

	inline bool operator==(const Url &url) const {
		return _protocol == url._protocol &&
			_server == url._server &&
			_port == url._port &&
			_path == url._path &&
			_urlParameters == url._urlParameters &&
			_hashParameters == url._hashParameters;
	}

protected:
	std::string _protocol;
	std::string _server;
	int _port;
	std::string _path;

	UrlParams _urlParameters;
	UrlParams _hashParameters;

	bool _isValid;
	std::string _fullUrl;

	void buildUrl();
};

}
}

#endif
