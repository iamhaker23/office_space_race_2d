#pragma once

#include <windows.h>		// Header File For Windows
#include "gameloop.h"
#include "DebugInfo.h"
#include <chrono>
#include <time.h>
#include "InputStates.h"

#ifndef WIN32_WINDOW_H

#define WIN32_WINDOW_H
using namespace std::chrono;

class win32_window {

public:
	//WIN32 functions
	static LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Handles events sent as messages from OS
	static void KillGLWindow();									// releases and destroys the window
	static bool CreateGLWindow(char* title, int width, int height); //creates the window
	static int WINAPI WinMainHandler(HINSTANCE, HINSTANCE, LPSTR, int, char*);  // Win32 main function
	static void reshape(int width, int height);
	static int getScreenHeight();
	static int getScreenWidth();
	static bool isLeftPressed();
	static InputStates* getInputs();
	static void printMessage(string message);
};



#endif