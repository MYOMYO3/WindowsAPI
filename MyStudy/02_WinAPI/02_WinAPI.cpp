// 02_WinAPI.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//
#include "stdafx.h"
#include "02_WinAPI.h"
#include <WINDOW.h>
#include "GAMECORE.h"
#include <Debug.h>



int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	
	Debug::Inst().LeakCheck();
	
	Window::Init(hInstance);

	//WINDOW* NewWindow1 = WINDOW::CreateHWindow(L"MainWindow1");
	//NewWindow1->Size(300, 300);
	//WINDOW* NewWindow2 = WINDOW::CreateHWindow(L"MainWindow2");
	   	
	GameCore::Start();
	MSG msg = GameCore::Loop();
	GameCore::End();
	
	return (int)msg.wParam;
}
