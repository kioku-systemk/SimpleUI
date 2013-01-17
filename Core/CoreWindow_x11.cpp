/*
 *  CoreWindow_x11.cpp
 *
 *  Created by kioku on 13/01/11.
 *  Copyright 2013 System K. All rights reserved.
 *
 */

#include "CoreWindow_x11.h"
#include <GL/gl.h>
#include <GL/glx.h>

#include <X11/Xlib.h>
#include <stdio.h>

class CoreWindow::Impl
{
public:
    Impl(){}
    ~Impl(){}
	
    int m_w, m_h;
    Window m_win;
    GLXContext m_ctx;
};

Display* m_display;
CoreWindow* g_win = 0;

bool make_window( Display *dpy, const char *name, int colorbit, int depthbit,
				 int width, int height, Window *winRet, GLXContext *ctxRet)
{
	int attrib[] = {
		GLX_RGBA, GLX_DOUBLEBUFFER,
		GLX_RED_SIZE, 8, GLX_GREEN_SIZE, 8, GLX_BLUE_SIZE, 8,
		GLX_DEPTH_SIZE, depthbit,
		None
	};
	int scrnum;
	XSetWindowAttributes attr;
	unsigned long mask;
	Window root;
	Window win;
	GLXContext ctx;
	XVisualInfo *visinfo;
	
	scrnum = DefaultScreen(dpy);
	//scrnum = 0;
	root = RootWindow(dpy, scrnum);
	
	visinfo = glXChooseVisual( dpy, scrnum, attrib );
	if (!visinfo) {
		printf("Error: couldn't get an RGB, Double-buffered visual\n");
		return false;
	}
	
	/* window attributes */
	attr.background_pixel = 0;
	attr.border_pixel = 0;
	attr.colormap = XCreateColormap( dpy, root, visinfo->visual, AllocNone);
	attr.event_mask = StructureNotifyMask | ExposureMask
	| KeyPressMask | KeyReleaseMask
	| PointerMotionMask
	| ButtonPressMask | ButtonReleaseMask | ButtonMotionMask
	| ResizeRedirectMask;
	mask = CWBackPixel | CWBorderPixel | CWColormap | CWEventMask;
	
	win = XCreateWindow( dpy, root, 0, 0, width, height,
						0, visinfo->depth, InputOutput,
						visinfo->visual, mask, &attr );
	if (!win)
	{
		printf("Error: couldn't create window.");
		return false;
	}
	
	/* set hints and properties */
	XSizeHints sizehints;
	sizehints.x = 0;
	sizehints.y = 0;
	sizehints.width  = width;
	sizehints.height = height;
	sizehints.flags = USSize | USPosition;
	XSetNormalHints(dpy, win, &sizehints);
	XSetStandardProperties(dpy, win, name, name, None, (char **)NULL, 0, &sizehints);
	
	ctx = glXCreateContext( dpy, visinfo, NULL, True );
	if (!ctx) {
		printf("Error: glXCreateContext failed\n");
		return false;
	}
	
	XFree(visinfo);
	*winRet = win;
	*ctxRet = ctx;
	return true;
}

CoreWindow::CoreWindow(int x, int y, int width, int height, const char* title, bool fullscreenMode)
{
	m_imp = new Impl();
	m_display = XOpenDisplay(0);
	
	m_imp->m_w = width;
	m_imp->m_h = height;
	bool r = make_window(m_display, title, 32, 24, width, height, &m_imp->m_win, &m_imp->m_ctx);
	XMapWindow(m_display, m_imp->m_win);
	glXMakeCurrent(m_display, m_imp->m_win, m_imp->m_ctx);
	g_win = this;
}
CoreWindow::~CoreWindow()
{
    // GL Context
    if (m_imp->m_ctx)
    {
        glXDestroyContext(m_display, m_imp->m_ctx);
		m_imp->m_ctx = 0;
    }
    if (m_imp->m_win){
        XDestroyWindow(m_display, m_imp->m_win);
		m_imp->m_win = 0;
    }
	
    // XWindow
    if (m_display)
    {
        XCloseDisplay(m_display);
		m_display = 0;
    }
	delete m_imp;
}
void CoreWindow::Active()
{
	
}
void CoreWindow::Toplevel(bool top)
{
	
}
void CoreWindow::SwapBuffer()
{
    glXSwapBuffers(m_display, g_win->m_imp->m_win);
}
void CoreWindow::DoEvents(void)
{
	XEvent evt;
	KeySym key;
	char text[255];
	int mx,my;
	
	XNextEvent(m_display, &evt );
	//printf("EVENT=%d\n",evt.type);
	switch ( evt.type ) {
		case Expose:
			if ( evt.xexpose.count == 0 ) {
				g_win->Draw();
			}
			return;
			
		case ButtonPress:
			mx = evt.xbutton.x;
			my = evt.xbutton.y;
			if( evt.xbutton.button == 1 ){
				g_win->MouseLeftDown(mx,my);
			} else if( evt.xbutton.button == 2 ){
				g_win->MouseMiddleDown(mx,my);
			} else if( evt.xbutton.button == 3 ){
				g_win->MouseRightDown(mx,my);
			} else if( evt.xbutton.button == 4 ){
				g_win->Wheel(0.0f, 1.0f, 0.0f);
			} else if( evt.xbutton.button == 5 ){
				g_win->Wheel(0.0f,-1.0f, 0.0f);
			}
			return;
		case ButtonRelease:
			mx = evt.xbutton.x;
			my = evt.xbutton.y;
			if( evt.xbutton.button == 1 ){
				g_win->MouseLeftUp(mx,my);
			} else if( evt.xbutton.button == 2 ){
				g_win->MouseMiddleUp(mx,my);
			} else if( evt.xbutton.button == 3 ){
				g_win->MouseRightUp(mx,my);
			}
			return;
		case MotionNotify:
			mx = evt.xbutton.x;
			my = evt.xbutton.y;
			g_win->MouseMove(mx, my);
			return;
			
		case KeyPress:
			if (XLookupString(&evt.xkey,text,255,&key,0) == 1) {
				g_win->KeyDown(text[0]);
			}
			return;
		case KeyRelease:
			if (XLookupString(&evt.xkey,text,255,&key,0) == 1) {
				g_win->KeyUp(text[0]);
			}
			return ;
			
		case ResizeRequest:
			g_win->Resize(evt.xresizerequest.width, evt.xresizerequest.height);
			return;
	}
}
void CoreWindow::MainLoop(void)
{
    while( 1 ) {
		g_win->Idle();
		DoEvents();
    }
}
const char* CoreWindow::GetExePath() const
{
    return 0; // TODO:
}

int CoreWindow::GetWidth() const
{
    return m_imp->m_w;
}

int CoreWindow::GetHeight() const
{
    return m_imp->m_h;
}

