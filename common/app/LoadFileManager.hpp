#pragma once
#include <bg/bg2e.hpp>

#include <thread>
#include <mutex>
#include <atomic>
#include <fstream>
#include <chrono>

class LoadFileManager
{
public:

	LoadFileManager();
	void run();
	void stop();
	void openFile(bg::system::Path &path);

private:
	static void ThreadFunction(LoadFileManager *manager);
	std::thread _thread;
	std::atomic<bool> _running;
	std::mutex _mutex;

	bool loaded;
	bg::system::Path path;
	std::fstream _file;

	

};