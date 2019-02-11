#pragma once

#include <Windows.h>
#include <tlhelp32.h>

#include <string>
#include <iostream>

namespace ts
{
	namespace system
	{

		int FindRunningProcess(const std::string & process); //returns the times the process is running 0 means not found
		void killProcessByName(const char *filename);
	}
}