/*
 *  CoreWindow_x11.cpp
 *
 *  Created by kioku on 13/01/11.
 *  Copyright 2013 System K. All rights reserved.
 *
 */

#include "CoreWindow_x11.h"

#ifdef EMSCRIPTEN
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#else
#include <GL/gl.h>
#include <GL/glx.h>
#endif

#include <X11/Xlib.h>
#include <stdio.h>
//--------------------------

#ifdef USE_GTK
#include <gtk/gtk.h>
#include <string.h>
#endif

namespace {
    const int MAXFILEPATH = 1024;
    static char filename[MAXFILEPATH];
    int btnmode = 0;
	
  const char* gtkFileDialog (const char* ext, bool save)
	{
#ifdef USE_GTK
		GtkWidget *filew;
		
		int argc = 1;
		char** argv;
		char name[] = "Dialog";
	        char* argv_i[] = {name,0};
		argv = reinterpret_cast<char**>(&argv_i);
		gtk_init (&argc, &argv);
		const char* title = (save ? "Save" : "Open");
		GtkWidget* dialog = gtk_file_chooser_dialog_new(title,
								0,save ? GTK_FILE_CHOOSER_ACTION_SAVE : GTK_FILE_CHOOSER_ACTION_OPEN,
														GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
								(save ? GTK_STOCK_SAVE : GTK_STOCK_OPEN), GTK_RESPONSE_ACCEPT, NULL);
		gtk_widget_show_all(dialog);
		gint res = gtk_dialog_run(GTK_DIALOG(dialog));
		if (res == GTK_RESPONSE_ACCEPT) {
			gchar* fname = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
			strncpy(filename, fname, MAXFILEPATH);
			//g_free(fname);
			gtk_widget_hide(dialog);
			gtk_widget_destroy(dialog);
			while (gtk_events_pending())
			  gtk_main_iteration();
			return filename;
		}
		gtk_widget_hide(dialog);
		gtk_widget_destroy(dialog);
		while (gtk_events_pending())
		  gtk_main_iteration();
#endif
		return 0;
	}
	const char* gtkFileSaveDialog (const char* ext)
	{
	   return gtkFileDialog(ext, true);
	}
	const char* gtkFileOpenDialog (const char* ext)
	{
	  return gtkFileDialog(ext, false);
	}

	
} // namespace

//--------------------------

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
	| ButtonPressMask | ButtonReleaseMask | ButtonMotionMask;
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
	glViewport(0,0,width,height);
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
    glXMakeCurrent(m_display, m_imp->m_win, m_imp->m_ctx);
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
	
	if (!XPending(m_display))
	  return;
	
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
		  // get last motion event(for overflow motion event)
		  while (XPending(m_display)) {
		    XPeekEvent(m_display, &evt );
		    if (evt.type != MotionNotify)
		      break;
		    XNextEvent(m_display, &evt);
		  }
		  if (evt.xmotion.x != mx
		  ||  evt.xmotion.y != my){
			mx = evt.xmotion.x;
			my = evt.xmotion.y;
			g_win->MouseMove(mx, my);
			return;
		  }	
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
			
	        case ConfigureNotify:
				if (g_win->m_imp->m_w != evt.xconfigure.width
				||  g_win->m_imp->m_h != evt.xconfigure.height) {
					g_win->m_imp->m_w = evt.xconfigure.width;
					g_win->m_imp->m_h = evt.xconfigure.height;
					g_win->Resize(evt.xconfigure.width, evt.xconfigure.height);
				}
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

void CoreWindow::GoFullscreen(bool fullscreen, bool cursor)
{
	// TODO
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

const char* CoreWindow::FileOpenDialog(const char* ext) const
{
    return gtkFileOpenDialog(ext);
}
const char* CoreWindow::FileSaveDialog(const char* ext) const
{
	return gtkFileSaveDialog(ext);
}
