//
//  main.cpp
//  download
//
//  Created by Fernando Serrano Carpena on 28/9/15.
//  Copyright © 2015 Fernando Serrano Carpena. All rights reserved.
//

#include <bg/net/download_queue.hpp>
#include <bg/net/net.hpp>

#include <bg/net/http/head.hpp>

#include <iostream>
#include <fstream>
#include <chrono>
#include <regex>
#include <sstream>

#include <curl/curl.h>

namespace bg {
namespace net {


DownloadQueue::DownloadQueue()
	:_maxDownloads(5)
	,_inProgress(0)
	,_total(0)
	,_completed(0)
	,_failed(0)
	,_cancel(false)
	,_onComplete(nullptr)
	,_onProgress(nullptr)
{
}


DownloadQueue::~DownloadQueue() {
    destroy();
}
	
bool DownloadQueue::start() {
	if (_downloadVector.size()>0) {
		_inProgress = 0;
		_total = static_cast<int>(_downloadVector.size());
		_completed = 0;
		_failed = 0;
		_current = _downloadVector.begin();
		_dlThread = std::thread(threadFunction, this);
		return true;
	}
	return false;
}
	
void DownloadQueue::cancel() {
	{
		std::lock_guard<std::mutex> lock(_m);
		_cancel = true;
	}
	waitUntilCompleted();
}
	
void DownloadQueue::threadFunction(DownloadQueue * download) {
	int total = static_cast<int>(download->_downloadVector.size());
	int completed = 0;
	int failed = 0;
	int inProgress = 0;
	
	while (total>(completed + failed)) {
		total = static_cast<int>(download->_downloadVector.size());	// Update total
		completed = 0;
		failed = 0;
		inProgress = 0;
		{
			std::lock_guard<std::mutex> lock(download->_m);
			download->_total = static_cast<int>(download->_downloadVector.size());
			for (Download * dl : download->_downloadVector) {
				if (dl->isStarted() && dl->isDone()) {
					if (dl->isFail()) {
						++failed;
					}
					else {
						++completed;
					}
				}
				else if (dl->isStarted() && !dl->isDone()) {
					if (download->_cancel) {
						dl->cancel();
					}
					++inProgress;
				}
				else if (!dl->isStarted() && inProgress<download->_maxDownloads && !download->_cancel) {
					dl->start();
					++inProgress;
				}
				else {
					break;
				}
			}
			download->_inProgress = inProgress;
			download->_completed = completed;
			download->_failed = failed;
			if (download->_cancel) {
				break;
			}
		}
		
		if (download->_onProgress) {
			download->_onProgress(download);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
	}
	
	if (download->_onComplete) {
		download->_onComplete(download);
	}

}
	
void DownloadQueue::destroy() {
	if (_dlThread.joinable()) {
		_dlThread.join();
	}
	
	for (Download * dl : _downloadVector) {
		delete dl;
	}
	_downloadVector.clear();
	
	
}

}
}
