/*
 *  CoreWindow_wgl.cpp
 *
 *  Created by kioku on 11/02/01.
 *  Copyright 2011 System K. All rights reserved.
 *
 */

#include "CoreWindow_wgl.h"
#include <gl/GL.h>
#include <WindowsX.h>
#include <stdio.h>

CoreWindow* g_mainWin = 0;

#if _UNICODE
#define _TX(x) L##x
#else
#define _TX(x) x
#endif


bool CoreWindow::createWindow(int x, int y, int width, int height, const TCHAR* title, bool fullscreenmode)
{
	WNDCLASS wc;
	HWND     hWnd;
	DWORD    dwExStyle;
	DWORD    dwStyle;
	RECT     WindowRect;
	WindowRect.left   = 0;
	WindowRect.right  = width;
	WindowRect.top    = 0;
	WindowRect.bottom = height;
	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	if (fullscreenmode)
		dwStyle = WS_POPUPWINDOW | WS_MAXIMIZE;
	else
		dwStyle   = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	AdjustWindowRect(&WindowRect, dwStyle, FALSE);

	if (!GetClassInfo(GetModuleHandle(NULL), _TX("CoreWindow_wgl"), &wc))
	{
//		memset(&wcx, 0, sizeof(WNDCLASSEX));
//		wcx.cbSize			= sizeof(WNDCLASSEX);
		wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc		= (WNDPROC) CoreWindow::BaseWndProc;
		wc.cbClsExtra		= 0;
		wc.cbWndExtra		= 0;
		wc.hInstance		= GetModuleHandle(NULL);
		wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
		wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
		wc.lpszMenuName	    = 0;
		wc.lpszClassName	= _TX("CoreWindow_wgl");

		if (!RegisterClass(&wc))
		{
			MessageBox(NULL,_TX("Failed To Register The Window Class."),_TX("ERROR"),MB_OK|MB_ICONEXCLAMATION);
			ExitProcess(0);
			return FALSE;
		}
	}
	
	hWnd = CreateWindowEx(dwExStyle,
						_TX("CoreWindow_wgl"),
						title,
						dwStyle,
						x, y,
						WindowRect.right - WindowRect.left,
						WindowRect.bottom - WindowRect.top,
						NULL,
						NULL,
						GetModuleHandle(NULL),
						(void*)this);//BaseWndProcにthisを渡してやる
	if (!hWnd)
	{
		KillGLWindow();
		MessageBox(NULL,_TX("Window Creation Error."),_TX("ERROR"),MB_OK|MB_ICONEXCLAMATION);
		ExitProcess(0);
		return FALSE;
	}

	// ウィンドウハンドルとCWindowBaseオブジェクトを結びつける
	SetProp(hWnd, _TX("CoreWindow_wgl"), (HANDLE)this);
	m_hWnd = hWnd;

	if (!g_mainWin)
		g_mainWin = this;

	return TRUE;
}

bool CoreWindow::initGL(HWND hWnd)
{
	static	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		24,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,	//Z-Buffer
		8,  //Stencil Buffer
		0,
		PFD_MAIN_PLANE,
		0, //Reserved
		0, 0, 0	
	};
	int	PixelFormat;
		
	if (!(m_hDC = ::GetDC(hWnd)))
	{
		KillGLWindow();
		MessageBox(NULL,_TX("Can't Create A GL Device Context."),_TX("ERROR"),MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!(PixelFormat = ChoosePixelFormat(m_hDC, &pfd)))
	{
		KillGLWindow();
		MessageBox(NULL,_TX("Can't Find A Suitable PixelFormat."),_TX("ERROR"),MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!SetPixelFormat(m_hDC, PixelFormat, &pfd))
	{
		KillGLWindow();
		MessageBox(NULL,_TX("Can't Set The PixelFormat."),_TX("ERROR"),MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!(m_hRC = wglCreateContext(m_hDC)))
	{
		KillGLWindow();
		MessageBox(NULL,_TX("Can't Create A GL Rendering Context."),_TX("ERROR"),MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!wglMakeCurrent(m_hDC, m_hRC))
	{
		KillGLWindow();
		MessageBox(NULL,_TX("Can't Activate The GL Rendering Context."),_TX("ERROR"),MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}

	ShowWindow(hWnd,SW_SHOW);

	return TRUE;
}

void CoreWindow::Toplevel(bool top)
{
	if (top)
	{
		SetForegroundWindow(m_hWnd);
		SetFocus(m_hWnd);
	}
}

CoreWindow::CoreWindow(int x, int y, int width, int height, const TCHAR* title, bool fullscreenmode)
{
	 createWindow(x, y, width, height, title, fullscreenmode);
	 initGL(m_hWnd);
	 RECT rect;
	 GetClientRect(m_hWnd, &rect);
	 resize(rect.right - rect.left, rect.bottom - rect.top);
}

CoreWindow::~CoreWindow()
{
}


void CoreWindow::resize(int width, int height)
{
	m_w = width;
	m_h = height;
	glViewport(0, 0, width, height);
	glLoadIdentity();

	Resize(width, height);
}

void CoreWindow::Active()
{
	if (wglGetCurrentContext() != m_hRC)
		wglMakeCurrent(m_hDC, m_hRC);
}

void CoreWindow::DoEvents()
{
	MSG msg;
	while(1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
			break;
	}
}

void CoreWindow::MainLoop()
{
	MSG msg;
	while(1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (g_mainWin)
				g_mainWin->Idle();
			
			if(msg.message == WM_QUIT)
				break;
		}
	}

}

void CoreWindow::KillGLWindow()
{
	if (m_hRC)
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(m_hRC);
		m_hRC = NULL;
	}

	ReleaseDC(m_hWnd, m_hDC);
	DestroyWindow(m_hWnd);
	m_hWnd = 0;
}


LRESULT CALLBACK CoreWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			// LoWord Can Be WA_INACTIVE, WA_ACTIVE, WA_CLICKACTIVE,
			// The High-Order Word Specifies The Minimized State Of The Window Being Activated Or Deactivated.
			// A NonZero Value Indicates The Window Is Minimized.
			//if ((LOWORD(wParam) != WA_INACTIVE) && !((BOOL)HIWORD(wParam)))
			//	m_isActive=true;						// Program Is Active
			//else
			//	m_isActive=false;						// Program Is No Longer Active
			return 0;								// Return To The Message Loop
		}
		
		case WM_ERASEBKGND:
			return 0;
			
		case WM_DESTROY:
			return 0;
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;

		case WM_PAINT:
		{
			Draw();
			break;
		}
		case WM_SIZE:
		{
			resize(LOWORD(lParam),HIWORD(lParam));
			break;
		}

		case WM_LBUTTONDOWN:
			MouseLeftDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
		case WM_LBUTTONUP:
			MouseLeftUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
		case WM_RBUTTONDOWN:
			MouseRightDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
		case WM_RBUTTONUP:
			MouseRightUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
		case WM_MBUTTONDOWN:
			MouseMiddleDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
		case WM_MBUTTONUP:
			MouseMiddleUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
		case WM_MOUSEMOVE:
			MouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;

		case WM_KEYDOWN:
			KeyDown(wParam);
		break;
		case WM_KEYUP:
			KeyUp(wParam);
		break;
	
		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;
		}

		
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}


LRESULT	CALLBACK CoreWindow::BaseWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CoreWindow* pTargetWnd = (CoreWindow*)GetProp(hWnd, _TX("CoreWindow_wgl"));

	if (!pTargetWnd)
	{
		if ((uMsg == WM_CREATE) || (uMsg == WM_NCCREATE))
			pTargetWnd = (CoreWindow*)((LPCREATESTRUCT)lParam)->lpCreateParams;
		else if	( uMsg == WM_INITDIALOG	)
			pTargetWnd = (CoreWindow*)lParam;

		//if (pTargetWnd)
		//	pTargetWnd->Attach(hWnd);
	}
	else
	{
		LRESULT	lResult	= pTargetWnd->WndProc(hWnd,	uMsg, wParam, lParam);
		//if (uMsg ==	WM_DESTROY)
		//	pTargetWnd->Detach();
		return lResult;

	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

HDC CoreWindow::GetHDC()
{
	return m_hDC;
}

void CoreWindow::SwapBuffer()
{
	SwapBuffers(m_hDC);
}

const char* CoreWindow::GetExePath() const
{
	char exefilepath[2048];
	static char exepath[2048];
	GetModuleFileName(NULL, exefilepath, sizeof(exefilepath));
#pragma warning(push)
#pragma warning(disable:4996)
	_splitpath(exefilepath, 0, exepath, 0, 0);
#pragma warning(pop)
	return exepath;
}
