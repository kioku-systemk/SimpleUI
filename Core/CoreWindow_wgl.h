/*
 *  CoreWindow_wgl.h
 *
 *  Created by kioku on 11/02/01.
 *  Copyright 2011 System K. All rights reserved.
 *
 */

#ifndef COREWINDOW_WGL_HEADER
#define COREWINDOW_WGL_HEADER

#include "CoreWindow.h"
#include <Windows.h>

class CoreWindow : public CoreWindowPlatform
{
protected:
	bool createWindow(int x, int y, int width, int height, const TCHAR* title,  bool fullscreenmode = false);
	
	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK BaseWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool initGL(HWND hWnd);
	void KillGLWindow();
	void resize(int width, int height);

	HWND  m_hWnd;
	HGLRC m_hRC;
	HDC   m_hDC;
	int m_w,m_h;
	
public:
	CoreWindow(int x, int y, int width, int height, const TCHAR* title, bool fullscreenmode = false);
	~CoreWindow();

	virtual void MouseLeftDown  (int x, int y){}
	virtual void MouseLeftUp    (int x, int y){}
	virtual void MouseRightDown (int x, int y){}
	virtual void MouseRightUp   (int x, int y){}
	virtual void MouseMiddleDown(int x, int y){}
	virtual void MouseMiddleUp  (int x, int y){}
	virtual void MouseMove      (int x, int y){}
	virtual void Wheel          (float dx, float dy, float dz){}
	virtual void Magnify        (float z)     {}
	virtual void Swipe          (float dx, float dy, float dz){}
	virtual void Rotate         (float r)     {}
	virtual void KeyDown        (int key)     {}
	virtual void KeyUp          (int key)     {}
	virtual void Resize         (int w, int h){}
	virtual void Close          (void)        {}
	virtual void Idle           (void)        {}
	virtual void Draw           (void)        {}
	virtual void Toplevel       (bool top);
	void Active(void);
	void SwapBuffer();
	
	HDC GetHDC();
	int GetWidth() const  { return m_w; }
	int GetHeight() const { return m_h; }
	
	const char* GetExePath() const;

	static void DoEvents();
	static void MainLoop();
};



#endif // COREWINDOW_WGL_HEADER

