#ifndef _bg2e_net_session_hpp_
#define _bg2e_net_session_hpp_

#include <bg/export.hpp>

namespace bg {
namespace net {

class BG2E_EXPORT Session {
public:
	Session();
	~Session();

	inline bool valid() const { return _sessionHandler != nullptr; }

	void start();

	void destroy();

	inline impl_ptr getSessionHandler() { return _sessionHandler; }

protected:
	impl_ptr _sessionHandler;
};

class SessionResource {
public:
	SessionResource() :_session(nullptr) {}
	SessionResource(Session * s) :_session(s) {}

	inline void setSession(Session * s) { _session = s; }
	inline Session * getSession() { return _session; }
	inline const Session * getSession() const { return _session; }

	inline bool assertSession() const { return _session && _session->valid(); }

protected:
	Session * _session;
};

}
}

#endif
