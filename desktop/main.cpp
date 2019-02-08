#include <app\App.h>
#include <time.h>
#ifdef _LEAP_H
#include <Leap.h>
#endif //_LEAP_H

#include<iostream>
#include <fstream>

#include <bg/bg2e.hpp>



//
//int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPSTR     lpCmdLine, _In_ int       nCmdShow)
int main(int argc, char* argv[])
{
	//	WinExec("cmd /K consoleapp.exe", argc);
	int result = 0;
	if (ts::App::Get().processArgs(argc, argv) == 1)
	{
		result = ts::App::Get().run();
	}

	return result;
}