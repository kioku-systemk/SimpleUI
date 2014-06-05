/*
 *  CoreWindow_egl.cpp
 *
 *  Created by kioku on 14/06/05.
 *  Copyright 2013 System K. All rights reserved.
 *
 */

#include "CoreWindow_egl.h"

//#include "esUtil.h"
//---------------------------------------------------------
#include <GLES2/gl2.h>
#include <EGL/egl.h>

#include <string.h>

#include  <X11/Xlib.h>
#include  <X11/Xutil.h>


typedef struct _escontext
{
    /// Put your user data here...
    void*       userData;
    
    /// Window width
    GLint       width;
    
    /// Window height
    GLint       height;
    
    /// Window handle
    EGLNativeWindowType  hWnd;
    
    /// EGL display
    EGLDisplay  eglDisplay;
    
    /// EGL context
    EGLContext  eglContext;
    
    /// EGL surface
    EGLSurface  eglSurface;
    
    /// Callbacks
//    void (ESCALLBACK *drawFunc) ( struct _escontext * );
//    void (ESCALLBACK *keyFunc) ( struct _escontext *, unsigned char, int, int );
//    void (ESCALLBACK *updateFunc) ( struct _escontext *, float deltaTime );
} ESContext;


#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

/// esCreateWindow flag - RGB color buffer
#define ES_WINDOW_RGB           0
/// esCreateWindow flag - ALPHA color buffer
#define ES_WINDOW_ALPHA         1
/// esCreateWindow flag - depth buffer
#define ES_WINDOW_DEPTH         2
/// esCreateWindow flag - stencil buffer
#define ES_WINDOW_STENCIL       4
/// esCreateWindow flat - multi-sample buffer
#define ES_WINDOW_MULTISAMPLE   8

namespace {

static Display *x_display = NULL;
    
void esInitContext ( ESContext *esContext )
{
    if ( esContext != NULL ) {
        memset( esContext, 0, sizeof( ESContext) );
    }
}

EGLBoolean CreateEGLContext ( EGLNativeWindowType hWnd, EGLDisplay* eglDisplay,
                             EGLContext* eglContext, EGLSurface* eglSurface,
                             EGLint attribList[])
{
    EGLint numConfigs;
    EGLint majorVersion;
    EGLint minorVersion;
    EGLDisplay display;
    EGLContext context;
    EGLSurface surface;
    EGLConfig config;
    EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };
    
    // Get Display
    display = eglGetDisplay((EGLNativeDisplayType)x_display);
    if ( display == EGL_NO_DISPLAY )
    {
        return EGL_FALSE;
    }
    
    // Initialize EGL
    if ( !eglInitialize(display, &majorVersion, &minorVersion) )
    {
        return EGL_FALSE;
    }
    
    // Get configs
    if ( !eglGetConfigs(display, NULL, 0, &numConfigs) )
    {
        return EGL_FALSE;
    }
    
    // Choose config
    if ( !eglChooseConfig(display, attribList, &config, 1, &numConfigs) )
    {
        return EGL_FALSE;
    }
    
    // Create a surface
    surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)hWnd, NULL);
    if ( surface == EGL_NO_SURFACE )
    {
        return EGL_FALSE;
    }
    
    // Create a GL context
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs );
    if ( context == EGL_NO_CONTEXT )
    {
        return EGL_FALSE;
    }
    
    // Make the context current
    if ( !eglMakeCurrent(display, surface, surface, context) )
    {
        return EGL_FALSE;
    }
    
    *eglDisplay = display;
    *eglSurface = surface;
    *eglContext = context;
    return EGL_TRUE;
}


///
//  WinCreate()
//
//      This function initialized the native X11 display and window for EGL
//
EGLBoolean WinCreate(ESContext *esContext, const char *title)
{
    Window root;
    XSetWindowAttributes swa;
    XSetWindowAttributes  xattr;
    Atom wm_state;
    XWMHints hints;
    XEvent xev;
    EGLConfig ecfg;
    EGLint num_config;
    Window win;
    
    /*
     * X11 native display initialization
     */
    
    x_display = XOpenDisplay(NULL);
    if ( x_display == NULL )
    {
        return EGL_FALSE;
    }
    
    root = DefaultRootWindow(x_display);
    
    swa.event_mask  =  ExposureMask | PointerMotionMask | KeyPressMask;
    win = XCreateWindow(
                        x_display, root,
                        0, 0, esContext->width, esContext->height, 0,
                        CopyFromParent, InputOutput,
                        CopyFromParent, CWEventMask,
                        &swa );
    
    xattr.override_redirect = FALSE;
    XChangeWindowAttributes ( x_display, win, CWOverrideRedirect, &xattr );
    
    hints.input = TRUE;
    hints.flags = InputHint;
    XSetWMHints(x_display, win, &hints);
    
    // make the window visible on the screen
    XMapWindow (x_display, win);
    XStoreName (x_display, win, title);
    
    // get identifiers for the provided atom name strings
    wm_state = XInternAtom (x_display, "_NET_WM_STATE", FALSE);
    
    memset ( &xev, 0, sizeof(xev) );
    xev.type                 = ClientMessage;
    xev.xclient.window       = win;
    xev.xclient.message_type = wm_state;
    xev.xclient.format       = 32;
    xev.xclient.data.l[0]    = 1;
    xev.xclient.data.l[1]    = FALSE;
    XSendEvent (
                x_display,
                DefaultRootWindow ( x_display ),
                FALSE,
                SubstructureNotifyMask,
                &xev );
    
    esContext->hWnd = (EGLNativeWindowType) win;
    return EGL_TRUE;
}
    
bool esCreateWindow ( ESContext *esContext, const char* title, GLint width, GLint height, GLuint flags )
{
    EGLint attribList[] =
    {
        EGL_RED_SIZE,       5,
        EGL_GREEN_SIZE,     6,
        EGL_BLUE_SIZE,      5,
        EGL_ALPHA_SIZE,     (flags & ES_WINDOW_ALPHA) ? 8 : EGL_DONT_CARE,
        EGL_DEPTH_SIZE,     (flags & ES_WINDOW_DEPTH) ? 8 : EGL_DONT_CARE,
        EGL_STENCIL_SIZE,   (flags & ES_WINDOW_STENCIL) ? 8 : EGL_DONT_CARE,
        EGL_SAMPLE_BUFFERS, (flags & ES_WINDOW_MULTISAMPLE) ? 1 : 0,
        EGL_NONE
    };
    
    if ( esContext == NULL )
    {
        return GL_FALSE;
    }
    
    esContext->width = width;
    esContext->height = height;
    
    if ( !WinCreate ( esContext, title) )
    {
        return GL_FALSE;
    }
    
    
    if ( !CreateEGLContext ( esContext->hWnd,
                            &esContext->eglDisplay,
                            &esContext->eglContext,
                            &esContext->eglSurface,
                            attribList) )
    {
        return GL_FALSE;
    }
    
    
    return GL_TRUE;
}



} // namespace

#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

#include <stdio.h>


namespace {
    const char* gtkFileSaveDialog (const char* ext)
	{
        return 0;
	}
	const char* gtkFileOpenDialog (const char* ext)
	{
        return 0;
	}

    CoreWindow* g_win = 0;
    
    ESContext esContext;
} // namespace

//--------------------------

class CoreWindow::Impl
{
public:
    Impl(){}
    ~Impl(){}
	
    int m_w, m_h;
//    Window* m_win;
//    GLXContext m_ctx;
};

CoreWindow::CoreWindow(int x, int y, int width, int height, const char* title, bool fullscreenMode)
{
	m_imp = new Impl();
//	m_display = XOpenDisplay(0);
	
	m_imp->m_w = width;
	m_imp->m_h = height;
//	bool r = make_window(m_display, title, 32, 24, width, height, &m_imp->m_win, &m_imp->m_ctx);
//	XMapWindow(m_display, m_imp->m_win);
//	glXMakeCurrent(m_display, m_imp->m_win, m_imp->m_ctx);
    
    esInitContext ( &esContext );
    esCreateWindow ( &esContext, title, width, height, ES_WINDOW_RGB );
	g_win = this;
	glViewport(0,0,width,height);
}
CoreWindow::~CoreWindow()
{
    // GL Context
/*    if (m_imp->m_ctx)
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
    }*/
	delete m_imp;
}
void CoreWindow::Active()
{
//    glXMakeCurrent(m_display, m_imp->m_win, m_imp->m_ctx);
}
void CoreWindow::Toplevel(bool top)
{
	
}
void CoreWindow::SwapBuffer()
{
    eglSwapBuffers(esContext.eglDisplay, esContext.eglSurface);
//    glXSwapBuffers(m_display, g_win->m_imp->m_win);
}
void CoreWindow::DoEvents(void)
{
    printf("Not support function DoEvents() on emscripten platform\n");
}

void idleloop()
{
    g_win->Idle();
}

int keyDownEvent(int eventType, const EmscriptenKeyboardEvent *keyEvent, void *userData)
{
    g_win->KeyDown(keyEvent->keyCode);
    return 0;
}

int keyUpEvent(int eventType, const EmscriptenKeyboardEvent *keyEvent, void *userData)
{
    g_win->KeyUp(keyEvent->keyCode);
    return 0;
}

int mouseEvent(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData)
{
  	static int mx = 0,my = 0;
    //printf("MOUSE EVENT = %d, BTN=%d\n",eventType,mouseEvent->button);
    switch ( eventType ) {
        case EMSCRIPTEN_EVENT_MOUSEDOWN:
            mx = mouseEvent->canvasX;
            my = mouseEvent->canvasY;
            if( mouseEvent->button == 0 ){
                g_win->MouseLeftDown(mx,my);
            } else if( mouseEvent->button == 1 ){
                g_win->MouseMiddleDown(mx,my);
            } else if( mouseEvent->button == 2 ){
                g_win->MouseRightDown(mx,my);
            } else if( mouseEvent->button == 3 ){
                g_win->Wheel(0.0f, 1.0f, 0.0f);
            } else if( mouseEvent->button == 4 ){
                g_win->Wheel(0.0f,-1.0f, 0.0f);
            }
            return 0;
        case EMSCRIPTEN_EVENT_MOUSEUP:
            mx = mouseEvent->canvasX;
            my = mouseEvent->canvasY;
            if( mouseEvent->button == 0 ){
                g_win->MouseLeftUp(mx,my);
            } else if( mouseEvent->button == 1 ){
                g_win->MouseMiddleUp(mx,my);
            } else if( mouseEvent->button == 2 ){
                g_win->MouseRightUp(mx,my);
            }
            return 0;
            
        case EMSCRIPTEN_EVENT_MOUSEMOVE:
            if (mouseEvent->canvasX != mx
            ||  mouseEvent->canvasY != my){
                mx = mouseEvent->canvasX;
                my = mouseEvent->canvasY;
                g_win->MouseMove(mx, my);
                return 0;
            }
            return 0;
        default:
            return 0;
    }
}

int resizeEvent(int eventType, const EmscriptenUiEvent *uiEvent, void *userData)
{
    //glViewport(0,0,uiEvent->windowInnerWidth, uiEvent->windowInnerHeight);
    //g_win->Resize(uiEvent->windowInnerWidth, uiEvent->windowInnerHeight);
    return 0;
}


void CoreWindow::MainLoop(void)
{
    emscripten_set_keyup_callback("#window", NULL, 0, keyUpEvent);
    emscripten_set_keydown_callback("#window", NULL, 0, keyDownEvent);
    emscripten_set_mousedown_callback("#window", NULL, 0, mouseEvent);
    emscripten_set_mouseup_callback("#window", NULL, 0, mouseEvent);
    emscripten_set_mousemove_callback("#window", NULL, 0, mouseEvent);
    
    emscripten_set_resize_callback("#window", NULL, 0, resizeEvent);

    emscripten_set_main_loop (idleloop, 0, 1);
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
