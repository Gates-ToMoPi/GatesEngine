#include "Application.h"

Application::Application()
{
	window = new Window(1280, 720, "ƒGƒ“ƒWƒ“§ì");
	keyboard = new Keyboard(window->GetHWND(), window->GetHINSTANCE());
}

Application::~Application()
{
}

bool Application::Initialize()
{
	window->DisplayWindow();
	return true;
}

void Application::Run()
{
	while (!keyboard->CheakHitKey(Key::ESCAPE))
	{
		keyboard->Update();
		if (!window->ProcessMessage()) break;
	}
}

void Application::Terminate()
{
	delete keyboard;
	delete window;
}
