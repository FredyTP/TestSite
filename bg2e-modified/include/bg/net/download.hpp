#ifndef _bg2e_net_download_hpp_
#define _bg2e_net_download_hpp_

#include <bg/export.hpp>
#include <bg/net/url.hpp>
#include <bg/net/session.hpp>

#include <string>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <functional>

namespace bg {
namespace net {

class BG2E_EXPORT Download : public SessionResource {
public:
	typedef std::function<void(Download * )> DownloadClosure;
	
	Download();
	Download(Session * session);
    ~Download();

	size_t fileSize(const std::string & src);
	void start();
	bool downloadSync();
	
	inline void prepare(const bg::net::Url & url, const std::string & dst) { _src = url.getString(); _dst = dst; }
	inline void prepare(const std::string & src, const std::string & dst) { _src = src; _dst = dst; }
	
	inline void start(const bg::net::Url & url, const std::string & dst) { prepare(url, dst); start(); }
    inline void start(const std::string & src, const std::string & dst) { prepare(src, dst); start(); }
	inline bool downloadSync(const bg::net::Url &url, const std::string & dst) { prepare(url, dst); return downloadSync(); }
	inline bool downloadSync(const std::string &url, const std::string & dst) { prepare(url, dst); return downloadSync(); }
	
    
    inline const std::string & getSource() const { return _src; }
    inline const std::string & getDestination() const { return _dst; }
	
	inline bool isStarted() {
		return _started;
	}
	
    inline bool isDone() {
        std::lock_guard<std::mutex> lock(_m);
        return _done;
    }
    
    inline bool isFail() {
        std::lock_guard<std::mutex> lock(_m);
        return _fail;
    }
    
    inline size_t downloaded() {
        std::lock_guard<std::mutex> lock(_m);
        return _downloaded;
    }
	
	inline size_t total() {
		std::lock_guard<std::mutex> lock(_m);
		return _total;
	}
	
	inline float completed() {
		std::lock_guard<std::mutex> lock(_m);
		float percent = 0.0f;
		
		if (_total>0) {
			percent = static_cast<float>(_downloaded) / static_cast<float>(_total) * 100.0f;
		}
		
		return percent;
	}
	
	inline bool isCanceled() {
		std::lock_guard<std::mutex> lock(_m);
		return _cancel;
	}
	
	void cancel();
	
	inline void waitUntilCompleted() {
		if (_dlThread.joinable()) {
			_dlThread.join();
		}
	}
	
	inline void onComplete(DownloadClosure closure) {
		_onComplete = closure;
	}
	
	inline void onProgress(DownloadClosure closure) {
		_onProgress = closure;
	}
	
	static void formatSize(size_t size, std::string & sizeString);
	
protected:
    std::string _src;
    std::string _dst;
    std::thread _dlThread;
    std::mutex _m;
    
    size_t _downloaded;
	size_t _total;
	bool _started;
    bool _done;
    bool _fail;
	bool _cancel;
	
	bool _deleteSession;
	
	DownloadClosure _onComplete;
	DownloadClosure _onProgress;
    
    static std::unordered_map<void *, Download *> s_downloadInstances;
    
    static void threadFunction(Download * download);
    
    static size_t downloadFunction(void * ptr, size_t size, size_t nmemb, FILE * stream);
};


}
}

#endif
