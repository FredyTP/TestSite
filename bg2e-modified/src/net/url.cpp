
#include <bg/net/url.hpp>

#include <sstream>

namespace bg {
namespace net {

Url::Url()
	:_protocol("http")
	,_server("")
	,_port(80)
	,_path("/")
	,_isValid(false)
{
}

Url::Url(const std::string & server)
	:_protocol("http")
	, _server(server)
	, _port(80)
	, _path("/")
	, _isValid(false)
{
	buildUrl();
}

Url::Url(const std::string & server, const std::string & path)
	:_protocol("http")
	, _server(server)
	, _port(80)
	, _path(path)
	, _isValid(false)
{
	buildUrl();
}

Url::Url(const std::string & protocol, const std::string & server, int port)
	:_protocol(protocol)
	, _server(server)
	, _port(port)
	, _path("/")
	, _isValid(false)
{
	buildUrl();
}

Url::Url(const std::string & protocol, const std::string & server, int port, const std::string & path)
	:_protocol(protocol)
	, _server(server)
	, _port(port)
	, _path(path)
	, _isValid(false)
{
	buildUrl();
}

Url::Url(const std::string & server, int port, const std::string & path)
	:_protocol("http")
	, _server(server)
	, _port(port)
	, _path(path)
	, _isValid(false)
{
	buildUrl();
}

Url::Url(const std::string & server, int port)
	:_protocol("http")
	, _server(server)
	, _port(port)
	, _path("/")
	, _isValid(false)
{
	buildUrl();
}

void Url::buildUrl() {
	_isValid = true;
	_fullUrl = "";
	std::stringstream fullUrl;

	if (_server.empty()) {
		_isValid = false;
	}

	if (_isValid && _protocol == "") {
		_protocol = "http";
	}

	if (_isValid) {
		_protocol = _protocol.empty() ? "http" : _protocol;
		if (_server.back() == '/') {
			_server.pop_back();
		}
		fullUrl << _protocol << "://" << _server << ":" << _port;

		if (_path.front() != '/') {
			fullUrl << "/";
		}

		fullUrl << _path;
	}
	
	if (_isValid && _urlParameters.size() > 0) {
		char separator = '?';
		for (auto kv : _urlParameters) {
			fullUrl << separator << kv.first << "=" << kv.second;
			separator = '&';
		}
	}

	if (_isValid && _hashParameters.size() > 0) {
		char separator = '#';
		for (auto kv : _hashParameters) {
			fullUrl << separator << kv.first << "=" << kv.second;
			separator = '&';
		}
	}
	
	if (_isValid) {
		_fullUrl = fullUrl.str();
	}
}

}
}
