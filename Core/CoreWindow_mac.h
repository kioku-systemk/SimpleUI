/*
 *  CoreWindow_mac.h
 *
 *  Created by kioku on 10/08/11.
 *  Copyright 2010 System K. All rights reserved.
 *
 */

#ifndef COREWINDOW_MAC_HEADER
#define COREWINDOW_MAC_HEADER

#include "CoreWindow.h"

#ifdef __OBJC__
#define ObjC_ID id
#else
#define ObjC_ID void*
#endif


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
    
    int GetWidth() const  { return m_w; }
    int GetHeight() const { return m_h; }
    
    const char* GetExePath() const;

protected:
	ObjC_ID m_win;
	ObjC_ID m_view;
    int m_w, m_h;
	
};

void CoreWindow_DoEvents();


#endif // COREWINDOW_MAC_HEADER



