/*
 *  CoreWindow_egl.h
 *
 *  Created by kioku on 14/06/05.
 *  Copyright 2013 System K. All rights reserved.
 *
 */

#ifndef COREWINDOW_EGL_HEADER
#define COREWINDOW_EGL_HEADER

#include "CoreWindow.h"

class CoreWindow : public CoreWindowPlatform
{
protected:
	CoreWindow(int x, int y, int width, int height, const char* title, bool fullscreenMode = false);
	~CoreWindow();
public:
	
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
	
	void Active();
	void Toplevel(bool top);
    void SwapBuffer();
	static void DoEvents(void);
	static void MainLoop(void);
    
	void GoFullscreen(bool fullscreen, bool cursor=true);

	int GetWidth() const;
	int GetHeight() const;
    
    const char* GetExePath() const;
	const char* FileOpenDialog(const char* ext) const;
	const char* FileSaveDialog(const char* ext) const;

protected:
    class Impl;
    Impl* m_imp;
};

void CoreWindow_DoEvents();


#endif // COREWINDOW_EGL_HEADER



