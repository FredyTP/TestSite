//
//  main.cpp
//  download
//
//  Created by Fernando Serrano Carpena on 28/9/15.
//  Copyright © 2015 Fernando Serrano Carpena. All rights reserved.
//

#include <bg/net/download.hpp>
#include <bg/net/net.hpp>

#include <bg/net/http/head.hpp>

#include <iostream>
#include <fstream>
#include <chrono>
#include <regex>
#include <iomanip>
#include <sstream>

#include <curl/curl.h>

namespace bg {
namespace net {

std::unordered_map<void *, Download *> Download::s_downloadInstances;

Download::Download()
    :SessionResource()
	,_downloaded(0)
	,_total(0)
	,_started(false)
    ,_done(false)
    ,_fail(true)
	,_cancel(false)
	,_deleteSession(true)
	,_onComplete(nullptr)
	,_onProgress(nullptr)
{
	setSession(new Session());
	getSession()->start();
}

Download::Download(Session * s)
	:SessionResource(s)
	,_downloaded(0)
	,_total(0)
	,_started(false)
	,_done(false)
	,_fail(true)
	,_cancel(false)
	,_deleteSession(false)
	,_onComplete(nullptr)
	,_onProgress(nullptr)
{
}


Download::~Download() {
	if (_dlThread.joinable()) {
		_dlThread.join();		
	}

	if (_deleteSession) {
		Session * session = getSession();
		delete session;
		setSession(nullptr);
	}
}
	
size_t Download::fileSize(const std::string & src) {
	if (!assertSession()) return -1;

	http::Head head(_session);
	head.load(src);
	size_t size = 0;
	
	if (head.getStatus()==200) {
		size = head.getContentLength();
	}
	
	return size;
}

void Download::start() {
	if (assertSession()) {
		_started = true;
		_done = false;
		_fail = false;
		_downloaded = 0;
		_total = fileSize(_src);
		_dlThread = std::thread(threadFunction, this);
	}
	else {
		_started = true;
		_done = true;
		_fail = true;
		_downloaded = 0;
		_total = 0;
	}
    
}
	
bool Download::downloadSync() {
	if (!assertSession()) {
		return false;
	}
	else if (_src.empty() || _dst.empty()) {
		return false;
	}
	
	_started = true;
	_done = false;
	_fail = false;
	_downloaded = 0;
	_total = fileSize(_src);
	
	CURL * curl = impl_cast<CURL*>(getSession()->getSessionHandler());
	FILE * f;
#if BG2E_WINDOWS
	fopen_s(&f, _dst.c_str(), "wb");
#else
	f = fopen(_dst.c_str(), "wb");
#endif
	CURLcode res;
	
	std::ofstream file(_dst, std::ofstream::out);
	bool err = false;
	
	if (f && curl) {
		s_downloadInstances[f] = this;
		curl_easy_setopt(curl, CURLOPT_URL, _src.c_str());
		curl_easy_setopt(curl, CURLOPT_HEADER, 0);
		curl_easy_setopt(curl, CURLOPT_NOBODY, 0);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, downloadFunction);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, f);
		
		res = curl_easy_perform(curl);
		fclose(f);
		s_downloadInstances[f] = nullptr;
	}
	else {
		if (f) fclose(f);
		err = true;
	}
	
	if (_onComplete) {
		_onComplete(this);
	}
	
	_done = true;
	_fail = err;
	
	return true;
}

void Download::cancel() {
	{
		std::lock_guard<std::mutex> lock(_m);
		_cancel = true;
	}
	waitUntilCompleted();
}

void Download::formatSize(size_t size, std::string & sizeString) {
	std::stringstream sstream;
	if (size<1000000) {
		sstream << std::fixed << std::setprecision(2) << (size / 1000) << " KB";
	}
	else if (size<1000000000) {
		sstream << std::fixed << std::setprecision(2) << (static_cast<float>(size) / 1000000.0f) << " MB";
	}
	else {
		sstream << std::fixed << std::setprecision(2) << (static_cast<float>(size) / 1000000000.0f) << " GB";
	}
	sizeString = sstream.str();
}

void Download::threadFunction(Download * download) {
	if (!download->assertSession()) {
		return;
	}

	CURL * curl = impl_cast<CURL*>(download->getSession()->getSessionHandler());
    FILE * f;
#if BG2E_WINDOWS
    fopen_s(&f, download->_dst.c_str(), "wb");
#else
    f = fopen(download->_dst.c_str(), "wb");
#endif
    CURLcode res;
    
    std::ofstream file(download->_dst, std::ofstream::out);
    bool err = false;
    
    if (f && curl) {
        s_downloadInstances[f] = download;
        curl_easy_setopt(curl, CURLOPT_URL, download->_src.c_str());
		curl_easy_setopt(curl, CURLOPT_HEADER, 0);
		curl_easy_setopt(curl, CURLOPT_NOBODY, 0);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, download->downloadFunction);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, f);
		
        res = curl_easy_perform(curl);
        fclose(f);
        s_downloadInstances[f] = nullptr;
    }
    else {
        if (f) fclose(f);
        err = true;
    }
	
	if (download->_onComplete) {
		download->_onComplete(download);
	}
	
	{
		std::lock_guard<std::mutex> lock(download->_m);
		download->_done = true;
		download->_fail = err;
	}
}

size_t Download::downloadFunction(void * ptr, size_t size, size_t nmemb, FILE * stream) {
    Download * download = s_downloadInstances[stream];
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    
    {
        std::lock_guard<std::mutex> lock(download->_m);
        download->_downloaded += written;
		
		if (download->_cancel) {
			download->_done = true;
			download->_fail = true;
			return -1;
		}
    }
	
	if (download->_onProgress) {
		download->_onProgress(download);
	}
    return written;
}

}
}
