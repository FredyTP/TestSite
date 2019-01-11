#ifndef _bg2e_net_download_queue_hpp_
#define _bg2e_net_download_queue_hpp_

#include <bg/export.hpp>
#include <bg/net/url.hpp>
#include <bg/net/session.hpp>
#include <bg/net/download.hpp>

#include <string>
#include <thread>
#include <mutex>
#include <map>
#include <vector>
#include <functional>

namespace bg {
namespace net {

class BG2E_EXPORT DownloadQueue {
public:
	typedef std::function<void(DownloadQueue*)> DownloadQueueClosure;
	
	DownloadQueue();
    ~DownloadQueue();

	inline void addResource(const Url & url, const std::string & dst) { addResource(url.getString(), dst); }
	inline void addResource(const std::string & src, const std::string & dst) {
		std::lock_guard<std::mutex> lock(_m);
		for (auto dl : _downloadVector) {
			// This file is already in the queue, and have the same destination directory
			if (dl->getDestination()==dst && dl->getSource()==src) {
				return;
			}
		}
		Download * download = new Download();
		download->prepare(src, dst);
		download->onComplete([&](Download * dl) {
			if (this->_onItemComplete) {
				this->_onItemComplete(dl);
			}
		});
		download->onProgress([&](Download * dl) {
			if (this->_onItemProgress) {
				this->_onItemProgress(dl);
			}
		});
		_downloadVector.push_back(download);
	}
	inline int getNumberOfResources() {
		std::lock_guard<std::mutex> lock(_m);
		return static_cast<int>(_downloadVector.size());
	}
	
	inline bool setMaxConcurrency(int max) {
		if (_total==0) {
			_maxDownloads = max;
			return true;
		}
		return false;
	}
	
	bool start();
	void cancel();
	inline bool isCancelled() const { return _cancel; }
	
	inline bool isDone() {
		std::lock_guard<std::mutex> lock(_m);
		return (_completed + _failed)==_total;
	}
	
	inline int inProgress() {
		std::lock_guard<std::mutex> lock(_m);
		return _inProgress;
	}
	
	inline int count() {
		return _total;
	}
	
	inline int completed() {
		std::lock_guard<std::mutex> lock(_m);
		return _completed;
	}
	
	inline int failed() {
		std::lock_guard<std::mutex> lock(_m);
		return _failed;
	}
	
	void eachDownload(std::function<void(Download *)> closure) {
		std::lock_guard<std::mutex> lock(_m);
		for (Download * d : _downloadVector) {
			closure(d);
		}
	}
	
	inline void onComplete(DownloadQueueClosure closure) {
		_onComplete = closure;
	}
	
	inline void onProgress(DownloadQueueClosure closure) {
		_onProgress = closure;
	}
	
	inline void onItemComplete(Download::DownloadClosure closure) {
		_onItemComplete = closure;
	}
	
	inline void onItemProgress(Download::DownloadClosure closure) {
		_onItemProgress = closure;
	}
	
	inline bool clearIfDone() {
		std::lock_guard<std::mutex> lock(_m);
		if ((_completed + _failed)==_total) {
			_downloadVector.clear();
			_current = _downloadVector.end();
			_inProgress = 0;
			_total = 0;
			_completed = 0;
			_failed = 0;
			_cancel = false;
			return true;
		}
		else {
			return false;
		}
	}
	
	void destroy();
	
	inline void waitUntilCompleted() {
		if (_dlThread.joinable()) {
			_dlThread.join();
		}
	}
	
protected:
    std::vector<Download*> _downloadVector;
	std::vector<Download*>::iterator _current;
	
    std::thread _dlThread;
    std::mutex _m;
    
	int _maxDownloads;
	
	int _inProgress;
	int _total;
	int _completed;
	int _failed;
	bool _cancel;
	
	DownloadQueueClosure _onComplete;
	DownloadQueueClosure _onProgress;
	Download::DownloadClosure _onItemComplete;
	Download::DownloadClosure _onItemProgress;
	
    static void threadFunction(DownloadQueue * download);
};


}
}

#endif
