#define WIN32_LEAN_AND_MEAN
#include<Windows.h>
#include"Application.h"
#ifdef _DEBUG
#include <crtdbg.h>
#include<stdio.h>
int main()
{
	printf("デバッグモードで開始\n");
	/* メモリリークを検出してくれる(COMのリークは検出できない) */
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#else
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
#endif
	Application* app = new Application();
	if (!app->Initialize())
	{
		app->Terminate();
		delete app;
		return 0;
	}
	app->Run();
	app->Terminate();
	delete app;
	return 0;
}
