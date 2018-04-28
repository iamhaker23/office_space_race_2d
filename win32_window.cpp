#include "win32_window.h"


//win32 global variables
HDC			win32_window::hDC = NULL;		// Private GDI Device Context
HGLRC		win32_window::hRC = NULL;		// Permanent Rendering Context
HWND		win32_window::hWnd = NULL;		// Holds Our Window Handle
HINSTANCE	win32_window::hInstance = NULL;		// Holds The Instance Of The Application


system_clock::time_point win32_window::START_CTIME;
system_clock::time_point win32_window::WIN_CTIME;
bool	win32_window::active = TRUE;		// Window Active Flag Set To TRUE By Default
bool	win32_window::fullscreen = TRUE;	// Fullscreen Flag Set To Fullscreen Mode By Default

Loop* win32_window::loop = NULL;

InputStates win32_window::inputs = InputStates();
DebugInfo win32_window::debugger = DebugInfo();
LoopManagerOSR win32_window::loops = LoopManagerOSR();


int screenWidth = 800, screenHeight = 800;

bool win32_window::isLeftPressed() {
	return inputs.LeftPressed;
}

InputStates win32_window::getInputs() {
	return inputs;
}

void win32_window::printMessage(string message) {
	debugger.addMessage(message);
}

int WINAPI win32_window::WinMainHandler(HINSTANCE	hInstance,			// Instance
	HINSTANCE	hPrevInstance,		// Previous Instance
	LPSTR		lpCmdLine,			// Command Line Parameters
	int			nCmdShow,
	char* name)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	bool	done = false;								// Bool Variable To Exit Loop
	int localFrameCount = 0;
														// Create Our OpenGL Window
	if (!CreateGLWindow(name, screenWidth, screenHeight))
	{
		return 0;									// Quit If Window Was Not Created
	}

	//Window was created
	START_CTIME = system_clock::now();
	WIN_CTIME = START_CTIME;

	//TODO: toggle pause and display a menu via a scenemanager
	bool paused = false;
	system_clock::time_point pausedAt = START_CTIME;
	bool pauseMessageDisplayed = false;
	int pauseKey = VK_TAB;
	bool DISABLE_FRAMERATE_CAP = false;
	
	int oldTicks = -1;

	while (!done){

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message == WM_QUIT)				// Have We Received A Quit Message?
			{
				done = true;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}

		if (inputs.keys[VK_ESCAPE]) done = true; //close game

		bool changed = loops.hasLoopChanged();
		if (!done && changed) {
			loop = loops.getActiveLoop();
		}

		if (!done && loop != NULL) {
								
			system_clock::time_point cNow = system_clock::now();
			duration<double> duration = cNow - START_CTIME;
			debugger.setDuration(duration);

			int timeSincePaused = (int)std::chrono::duration_cast<std::chrono::milliseconds>(cNow - pausedAt).count();


			if (!paused) {

				if (loops.isActivePausable() && timeSincePaused >= 300 && inputs.keys[pauseKey]) {
					pausedAt = cNow;
					paused = true;
				}

				pauseMessageDisplayed = false;

				//Every other tick of the given frequency render a frame
				//std::ratio<1, 120> gives 60fps
				//std::ratio<1, 100> gives 50fps
				//std::ratio<1, 60> gives 30fps
				//std::ratio<1, 70> gives 35fps
				int ticks = (int)(std::chrono::duration<float, std::ratio<1, 70>>(cNow - WIN_CTIME).count());

				if (DISABLE_FRAMERATE_CAP || ((ticks % 2 == 1) && ticks != oldTicks)) {

					oldTicks = ticks;
					localFrameCount+=1;

					loop->display();	// Draw The Scene

					loop->drawUI();
					SwapBuffers(hDC);				// Swap Buffers (Double Buffering)

				}


			}
			else {

				if (timeSincePaused >= 300 && inputs.keys[pauseKey]) {
					pausedAt = cNow;
					paused = false;
				}
				else if (inputs.keys[0x51]) {
					Loop::requestedActiveLoop = 1;
					paused = false;
				}

				if (!pauseMessageDisplayed && loop != NULL) {
					loop->writeMessage("Paused\nQ - Main Menu\nTAB - Resume");
					pauseMessageDisplayed = true;
					//show pause message
					SwapBuffers(hDC);
				}

			}


			//if it's been a second since last tic, update framesamples
			if (std::chrono::duration_cast<std::chrono::milliseconds>(cNow - WIN_CTIME).count() >= 1000) {
					
				debugger.addFrameSample(localFrameCount);
				localFrameCount = 0;
				WIN_CTIME = cNow;
			}
		}
	}

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (int)(msg.wParam);						// Exit The Program
}

//WIN32 Processes function - useful for responding to user inputs or other events.
LRESULT CALLBACK win32_window::WndProc(HWND	hWnd,			// Handle For This Window
	UINT	uMsg,			// Message For This Window
	WPARAM	wParam,			// Additional Message Information
	LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
	case WM_CLOSE:								// Did We Receive A Close Message?
	{
		PostQuitMessage(0);						// Send A Quit Message
		return 0;								// Jump Back
	}
	break;

	case WM_SIZE:								// Resize The OpenGL Window
	{
		win32_window::reshape(LOWORD(lParam), HIWORD(lParam));  // LoWord=Width, HiWord=Height
		return 0;								// Jump Back
	}
	break;

	case WM_LBUTTONDOWN:
	{

		inputs.LeftPressed = true;
	}
	break;

	case WM_LBUTTONUP:
	{
		inputs.LeftPressed = false;
	}
	break;

	case WM_MOUSEMOVE:
	{
		inputs.mouse_x = LOWORD(lParam) - (screenWidth / 2);
		inputs.mouse_y = (screenHeight / 2) - HIWORD(lParam);
	}
	break;
	case WM_KEYDOWN:							// Is A Key Being Held Down?
	{
		
		
		inputs.keys[wParam] = true;					// If So, Mark It As TRUE
		//inputs->keysActivated[wParam] = WIN_CTIME;

		return 0;								// Jump Back
	}
	break;
	case WM_KEYUP:								// Has A Key Been Released?
	{
		inputs.keys[wParam] = false;					// If So, Mark It As FALSE

		return 0;								// Jump Back
	}
	case WM_CHAR:
	{
		if ((wParam > 64 && wParam < 91) || (wParam > 96 && wParam < 123)) {
			inputs.character = (char)wParam;
			inputs.characterIsNew = true;
		}
		return 0;
	}
	break;
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void win32_window::KillGLWindow()								// Properly Kill The Window
{
	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL, NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		hRC = NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd, hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hDC = NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL", hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;									// Set hInstance To NULL
	}

	//Free data from game loops
	//loops.freeData();
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
*	title			- Title To Appear At The Top Of The Window				*
*	width			- Width Of The GL Window Or Fullscreen Mode				*
*	height			- Height Of The GL Window Or Fullscreen Mode			*/

bool win32_window::CreateGLWindow(char* title, int width, int height)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left = (long)0;			// Set Left Value To 0
	WindowRect.right = (long)width;		// Set Right Value To Requested Width
	WindowRect.top = (long)0;				// Set Top Value To 0
	WindowRect.bottom = (long)height;		// Set Bottom Value To Requested Height

	hInstance = GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc = (WNDPROC)WndProc;					// WndProc Handles Messages
	wc.cbClsExtra = 0;									// No Extra Window Data
	wc.cbWndExtra = 0;									// No Extra Window Data
	wc.hInstance = hInstance;							// Set The Instance
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor = LoadCursor(NULL, IDC_CROSS);			// Load The Arrow Pointer
	wc.hbrBackground = NULL;									// No Background Required For GL
	wc.lpszMenuName = NULL;									// We Don't Want A Menu
	wc.lpszClassName = "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;											// Return FALSE
	}

	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
	dwStyle = WS_OVERLAPPEDWINDOW;							// Windows Style

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

																	// Create The Window
	if (!(hWnd = CreateWindowEx(dwExStyle,							// Extended Style For The Window
		"OpenGL",							// Class Name
		title,								// Window Title
		dwStyle |							// Defined Window Style
		WS_CLIPSIBLINGS |					// Required Window Style
		WS_CLIPCHILDREN,					// Required Window Style
		0, 0,								// Window Position
		WindowRect.right - WindowRect.left,	// Calculate Window Width
		WindowRect.bottom - WindowRect.top,	// Calculate Window Height
		NULL,								// No Parent Window
		NULL,								// No Menu
		hInstance,							// Instance
		NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		24,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		24,											// 24Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(hDC = GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!SetPixelFormat(hDC, PixelFormat, &pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!(hRC = wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!wglMakeCurrent(hDC, hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	ShowWindow(hWnd, SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	
	if (hDC != NULL) {
		loops.init(hDC, debugger, inputs);
	}
	
	win32_window::reshape(width, height);					// Set Up Our Perspective GL Screen
	
	return true;									// Success
}

void win32_window::reshape(int width, int height)		// Resize the OpenGL window
{
	screenWidth = width; screenHeight = height;           // to ensure the mouse coordinates match 
														  // we will use these values to set the coordinate system

	//notify loop
	Loop::screenHeight = (float)screenHeight;
	Loop::screenWidth = (float)screenWidth;

	glViewport(0, 0, width, height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	double heightRatio = (double)height / (double)width;
	
	//Do I really want this?
	double scale = (width < 1000.0) ? 1.0 : ((double)width / 1000.0);

	// set the coordinate system for the window, effectively tweaking aspect ratio
	if (heightRatio <= 1.0) {
		//height is squeezed
		gluOrtho2D(-1.0*scale, 1.0*scale, -heightRatio*scale, heightRatio*scale);
	}
	else {
		//width is squeezed
		gluOrtho2D((-1.0 / heightRatio), (1.0 / heightRatio), -1.0, 1.0);
	}
	

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}