#include <system/Utils.hpp>


namespace ts
{
	namespace system
	{
		int FindRunningProcess(const std::string & process) {
			/*
			Function takes in a string value for the process it is looking for like ST3Monitor.exe
			then loops through all of the processes that are currently running on windows.
			If the process is found it is running, therefore the function returns true.
			*/
			int procRunning = 0;
			std::string compare;
			HANDLE hProcessSnap;
			PROCESSENTRY32 pe32;
			hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

			if (hProcessSnap == INVALID_HANDLE_VALUE) {
				procRunning = false;
			}
			else {
				pe32.dwSize = sizeof(PROCESSENTRY32);
				if (Process32First(hProcessSnap, &pe32)) {
					compare = pe32.szExeFile;
					if (compare == process) {
						procRunning++;
					}
					else {
						// loop through all running processes looking for process
						while (Process32Next(hProcessSnap, &pe32)) {
							// Set to an std::string instead of Char[] to make compare easier
							compare = pe32.szExeFile;
							if (compare == process) {
								// if found process is running, add procCounter
								procRunning++;
							}
						}
					}
					// clean the snapshot object
					CloseHandle(hProcessSnap);
				}
			}
			return procRunning;
		}

		void killProcessByName(const char *filename)
		{
			HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
			PROCESSENTRY32 pEntry;
			pEntry.dwSize = sizeof(pEntry);
			BOOL hRes = Process32First(hSnapShot, &pEntry);
			while (hRes)
			{
				if (_stricmp(pEntry.szExeFile, filename) == 0)
				{
					HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0,
						(DWORD)pEntry.th32ProcessID);
					if (hProcess != NULL)
					{
						TerminateProcess(hProcess, 9);
						CloseHandle(hProcess);
					}
				}
				hRes = Process32Next(hSnapShot, &pEntry);
			}
			CloseHandle(hSnapShot);
		}

	}
}