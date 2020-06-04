#define WIN32_LEAN_AND_MEAN
#define INITGUID //make sure directx guids are include
#define _CRT_SECURE_NO_WARNINGS


#include <cstdio>
#include <windows.h>   // include important windows stuff
#include <windowsx.h>
#include <ddraw.h>
#include <fstream>
#include <cstdarg>



using namespace std;


#pragma region 声明/定义

typedef unsigned short USHORT;
typedef unsigned short WORD;
typedef unsigned char UCHAR;
typedef unsigned char BYTE;


#define WINDOW_CLASS_NAME L"WINDOW_CLASS"
#define SCREEN_WIDTH 1920//屏幕宽度
#define SCREEN_HEIGHT 1080//屏幕高度
#define SCREEN_BPP 32//BPP


HWND main_window_handle;
HINSTANCE main_instance;
LPDIRECTDRAW7 lpdd;
ofstream fout;



#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)


bool GameInit();
void GameMain();
void GameQuit();


const int bufferLen = 1024;
char buffer[bufferLen];

#define LOG_INFO(format,...) \
	fout <<  __FILE__  << "["  << __FUNCTION__<<  ":" <<  __LINE__ << "]" << formatStr(format,__VA_ARGS__) << endl;


char* formatStr(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	// vprintf(format,args);
	memset(buffer, 0, bufferLen);
	vsprintf(buffer, format, args);
	va_end(args);
	return buffer;
}



#pragma endregion


#pragma region window窗口

LRESULT CALLBACK WindowProc(HWND hwnd,
                            UINT msg,
                            WPARAM wparam,
                            LPARAM lparam)
{
	// this is the main message handler of the system
	PAINTSTRUCT ps; // used in WM_PAINT
	HDC hdc; // handle to a device context

	// what is the message 
	switch (msg)
	{
	case WM_CREATE:
		{
			// do initialization stuff here

			// return success
			return (0);
		}
		break;

	case WM_PAINT:
		{
			// simply validate the window
			hdc = BeginPaint(hwnd, &ps);
			// you would do all your painting here
			EndPaint(hwnd, &ps);

			// return success
			return (0);
		}
		break;

	case WM_DESTROY:
		{
			// kill the application, this sends a WM_QUIT message 
			PostQuitMessage(0);

			// return success
			return (0);
		}
		break;

	default: break;
	} // end switch

	// process any messages that we didn't take care of 
	return (DefWindowProc(hwnd, msg, wparam, lparam));
} // end WinProc

// WINMAIN ////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hinstance,
                   HINSTANCE hprevinstance,
                   LPSTR lpcmdline,
                   int ncmdshow)
{
	WNDCLASSEX winclass; // this will hold the class we create
	//HWND	   hwnd;	 // generic window handle
	MSG msg; // generic message

	// first fill in the window class stucture
	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_DBLCLKS | CS_OWNDC |
		CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc = WindowProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = hinstance;
	winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName = NULL;
	winclass.lpszClassName = WINDOW_CLASS_NAME;
	winclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	main_instance = hinstance;

	// register the window class
	if (!RegisterClassEx(&winclass))
		return (0);

	// create the first window
	if (!(main_window_handle = CreateWindowEx(NULL, // extended style
	                                          WINDOW_CLASS_NAME, // class
	                                          L"我的WIndow窗口", // title
	                                          WS_OVERLAPPEDWINDOW | WS_VISIBLE,
	                                          0, 0, // initial x,y
	                                          SCREEN_WIDTH,SCREEN_HEIGHT, // initial width, height
	                                          NULL, // handle to parent 
	                                          NULL, // handle to menu
	                                          hinstance, // instance of this application
	                                          NULL))) // extra creation parms
		return (0);

	if (!GameInit())return 1;
	// enter main event loop, but this time we use PeekMessage()
	// instead of GetMessage() to retrieve messages
	while (TRUE)
	{
		// test if there is a message in queue, if so get it
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// test if this is a quit
			if (msg.message == WM_QUIT)
				break;

			// translate any accelerator keys
			TranslateMessage(&msg);

			// send the message to the window proc
			DispatchMessage(&msg);
		} // end if

		// main game processing goes here
		GameMain();
	} // end while

	GameQuit();
	// return to Windows like this
	return (msg.wParam);
} // end WinMain

#pragma endregion


#pragma region 游戏逻辑

bool GameInit()
{
	fout.open("log.log",ios_base::out);

	LOG_INFO("GameInit");
	
	
	//创建IDD7对象
	if (FAILED(DirectDrawCreateEx(NULL, (void**)&lpdd, IID_IDirectDraw7, NULL))) return false;
	//设置CooperativeLevel
	//if (FAILED(lpdd->SetCooperativeLevel(main_window_handle, DDSCL_FULLSCREEN | DDSCL_ALLOWMODEX | DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT)))return false;

	if (FAILED(lpdd->SetCooperativeLevel(main_window_handle,DDSCL_NORMAL)))return false;
	if (FAILED(lpdd->SetDisplayMode(SCREEN_WIDTH,SCREEN_HEIGHT,SCREEN_BPP,0,0)))return false;


	return true;
}

void GameMain()
{
	if (KEYDOWN(VK_ESCAPE))
	{
		SendMessage(main_window_handle,WM_CLOSE, 0, 0);
	}
}

void GameQuit()
{

	LOG_INFO("GameQuit");
	if (lpdd)
	{
		lpdd->Release();
		lpdd = nullptr;
	}


	
	fout.close();
}


#pragma endregion
