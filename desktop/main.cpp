#include <app\App.h>
#include <time.h>
#include <Leap.h>

//
//int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPSTR     lpCmdLine, _In_ int       nCmdShow)
int main(int argc, char* argv[])
{
//	WinExec("cmd /K consoleapp.exe", argc);
	int result = ts::App::Get().run();
	return result;
}