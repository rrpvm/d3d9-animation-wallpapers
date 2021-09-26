#include "Loader.h"
#include "StringEcnryption.h"
#include <Windows.h>
#include <thread>
#include <chrono>
/*includes for factory*/
#include "Factory.h"
#include "Renderer.h"
#ifdef _DEBUG
#include <iostream>
#endif
#include "WallpaperStars.h"
/**/
Loader::~Loader()
{
	this->Release();
}
Loader::Loader()
{
	srand(GetTickCount64());//srand called only from here
	createWorkerInstance();
	getWorkerHandle();
}
void Loader::Update(const float elapsedTime)
{
	Renderer* render = reinterpret_cast<Renderer*>(Factory::get().createObject(Factory::ControllerType::renderer, RENDERERINTERFACEVERSION));
	createOverlay();
	render->Init(reinterpret_cast<HWND>(this->workerHandle), reinterpret_cast<HWND>(/*workerHandle*/this->overlayHandle), MAXVERTICIES);
	WallpaperStars* wpstrs = reinterpret_cast<WallpaperStars*>(Factory::get().createObject(Factory::ControllerType::wpstars, WPSTARSINTERFACEVERSION));
	MSG msg;
	while (GetMessage(&msg, (HWND)this->overlayHandle, 0, 0) && !(GetAsyncKeyState(VK_HOME) || GetAsyncKeyState(VK_MENU))) {
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
		static DWORD lastFps = 0;
		MSG msg;
		static DWORD lastProc = GetTickCount64();
		if (GetTickCount64() - lastProc > 1000 / LOOPFPS) {
			this->elapsedTime = GetTickCount64() - lastProc;
			lastProc = GetTickCount64();
			lastFps += this->elapsedTime;
			if (lastFps >= 1000) {
#ifdef _DEBUG
				std::cout << fps << std::endl;
#endif
				lastFps = 0;
				fps = 0;
			}
		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));//небольшая задержка. Может быть небольшой рассинхрон
			continue;
		}
		fps++;
		render->Update(this->elapsedTime);
		wpstrs->Update(this->elapsedTime);
		render->Paint(this->elapsedTime);
	}
}
void Loader::Paint(const float elapsedTime)
{
	/*we dont need paint*/
}
void Loader::Release()
{

}
bool Loader::createWorkerInstance()
{
	int result = -1;
	HWND progman = FindWindow("Progman", 0);
	assert(progman && ECRYPT("Can't find a Program wnd class"));
	SendMessageTimeout(progman, MSG_CREATEWORKERW, 0x04, 0, SMTO_NORMAL, 1000, (PDWORD_PTR)&result);
	return result;
}
unsigned __int32 Loader::getWorkerHandle()
{
	const HWND progman = FindWindow("Progman", 0);
	const HWND desktop = GetDesktopWindow();
	HWND tmpWorker = 0;
	HWND shell = 0;
	do {
		tmpWorker = FindWindowEx(desktop, tmpWorker, "WorkerW", 0);
		shell = FindWindowEx(tmpWorker, 0, "SHELLDLL_DefView", 0);
		if (shell) {
			this->workerHandle = (uintptr_t)FindWindowEx(0, tmpWorker, "WorkerW", 0);
		}
	} while (!this->workerHandle);//данный цикл может быть бесконечным. Стоит перевести этот цикл в луп между всеми окнам(ENumrate Windows)
	return workerHandle;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
void Loader::createOverlay()
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_PARENTDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = (HINSTANCE)this->workerHandle;
	wc.hIcon =0;
	wc.hCursor = 0;
	wc.hbrBackground = (HBRUSH)(COLOR_SCROLLBAR);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "Overlay";
	wc.hIconSm = 0;
	RegisterClassEx(&wc);
	this->overlayHandle = (uintptr_t)CreateWindowEx(WS_EX_LEFT| WS_EX_LTRREADING|WS_EX_RIGHTSCROLLBAR|WS_EX_NOACTIVATE, "Overlay", "Overlaywnd", WS_CHILDWINDOW|WS_VISIBLE,
		0, 0, 1920, 1080, (HWND)this->workerHandle, nullptr, 0, nullptr);
	//SetLayeredWindowAttributes((HWND)overlayHandle, RGB(0, 0, 0), 0, LWA_COLORKEY);
	ShowWindow((HWND)overlayHandle, 1);
	UpdateWindow((HWND)overlayHandle);
	assert(overlayHandle);
}
/*void Loader::Loop()
{
	while (!GetAsyncKeyState(VK_HOME) || !GetAsyncKeyState(VK_MENU)) {
		static DWORD lastProc = GetTickCount64();
		if (GetTickCount64() - lastProc > 1000 / LOOPFPS) {
			elapsedTime = GetTickCount64() - lastProc;
			lastProc = GetTickCount64();		
		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			continue;
		}
		fndts->run();
		this->render->paint();
		
	}
}*/
