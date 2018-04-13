#include "Main.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	win32_window::WinMainHandler(hInstance,	
		hPrevInstance,		
		lpCmdLine,			
		nCmdShow,
		"OfficeSpaceRace");
}