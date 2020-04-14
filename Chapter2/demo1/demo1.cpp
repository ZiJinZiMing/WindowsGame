#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <windowsx.h>


int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow)
{
	MessageBoxA(NULL, "this is content", "this is title", MB_OK);
	return 0;
}
