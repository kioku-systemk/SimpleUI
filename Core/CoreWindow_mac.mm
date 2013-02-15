/*
 *  CoreWindow_mac.cpp
 *
 *  Created by kioku on 10/08/11.
 *  Copyright 2010 System K. All rights reserved.
 *
 */

#include "CoreWindow_mac.h"

#import <Cocoa/Cocoa.h>
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>

//#define TRACE_ENABLE

#ifdef TRACE_ENABLE
#define trace	printf
#else
#include <stdarg.h>
inline void trace(const char* f, ...)
{
}
#endif


static CoreWindow* g_mainWin = 0;

@interface skMacDelegate : NSObject//  < NSApplicationDelegate >
/* Example: Fire has the same problem no explanation */
{
}
@end

@implementation skMacDelegate
- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication;
{
	return YES;
}
@end


@interface skOpenGLWindow : NSWindow
{
	CoreWindow* m_ownerWin;
}
@end

@implementation skOpenGLWindow
- (id) initWithContentRect: (NSRect)rect styleMask:(NSUInteger)wndStyle backing:(NSBackingStoreType)bufferingType defer:(BOOL)deferFlg win:(CoreWindow*)ownerWin
{
	[super initWithContentRect:rect styleMask:wndStyle backing:bufferingType defer:deferFlg];
	m_ownerWin = ownerWin;
	
	[[NSNotificationCenter defaultCenter] 
	 addObserver:self
	 selector:@selector(windowDidResize:)
	 name:NSWindowDidResizeNotification
	 object:self];
		
	[[NSNotificationCenter defaultCenter]
	 addObserver:self
	 selector:@selector(windowWillClose:)
	 name:NSWindowWillCloseNotification
	 object:self];
	
	[self setAcceptsMouseMovedEvents:YES];
	[self makeFirstResponder:self];
	
	trace("%s\n",__FUNCTION__);
	return self;
}

- (void) resizeGL
{
	trace("%s\n",__FUNCTION__);
}

- (void) windowDidResize: (NSNotification *)notification
{
	trace("%s\n",__FUNCTION__);
}

- (void) windowWillClose: (NSNotification *)notification
{
	trace("%s\n",__FUNCTION__);
	m_ownerWin->Close();
	[NSApp terminate:nil];	// This can also be exit(0);
}

@end


@interface skOpenGLView : NSOpenGLView 
{
	CoreWindow* m_ownerWin;
	int m_width;
	int m_height;
	NSTrackingRectTag _tag;
	bool m_isMouseCursorIn;
}
- (void) drawRect: (NSRect) bounds;
@end

@implementation skOpenGLView
-(void) setOwnerWindow: (CoreWindow*) win
{
	m_isMouseCursorIn = false;
	m_ownerWin = win;
}


- (BOOL)acceptsFirstResponder
{
	return YES;
}


//トラッキング領域の生成
- (void)viewDidMoveToWindow {
    _tag = [self addTrackingRect:[self bounds] owner:self userData:NULL assumeInside:NO];
}

- (void)setFrame:(NSRect)frame {    
    [super setFrame:frame];
    [self removeTrackingRect:_tag];
    _tag = [self addTrackingRect:[self bounds] owner:self userData:NULL assumeInside:NO];
}

- (void)setBounds:(NSRect)bounds {
    [super setBounds:bounds];
    [self removeTrackingRect:_tag];
    _tag = [self addTrackingRect:[self bounds] owner:self userData:NULL assumeInside:NO];
}

- (void)mouseEntered:(NSEvent*)event {
    [[self window] setAcceptsMouseMovedEvents:YES];
    [[self window] makeFirstResponder:self];
	m_isMouseCursorIn = true;
}

- (void)mouseExited:(NSEvent*)event {
    [[self window] setAcceptsMouseMovedEvents:NO];
	m_isMouseCursorIn = false;
}


-(void) reshape
{
	trace("%s\n",__FUNCTION__);
	NSRect rect;
	rect = [self frame];
	m_width = rect.size.width;
	m_height = rect.size.height;
	trace("resize -> %d,%d\n", m_width, m_height);
	m_ownerWin->Active();
	glViewport(0,0,m_width,m_height);
	m_ownerWin->Resize(m_width, m_height);
}

-(void) drawRect: (NSRect) bounds
{
	trace("%s\n",__FUNCTION__);
}

-(void) prepareOpenGL
{
	trace("%s\n",__FUNCTION__);
}

-(NSMenu *)menuForEvent: (NSEvent *)theEvent
{
	trace("%s\n",__FUNCTION__);
	return [NSView defaultMenu];
}

- (void) keyDown:(NSEvent *)theEvent
{
	NSString *chrs;//,*chrsNoMod;
	chrs = [theEvent characters];
	int unicode = 0;
	if ([chrs length] > 0)
	{
		unicode=[chrs characterAtIndex:0];
		unicode &= 255;
		if (0<=unicode && unicode<128)
			m_ownerWin->KeyDown(unicode);
	}
	trace("%s : %d\n",__FUNCTION__, unicode);
	
	const int curkey = 256*0xF7;
	if ((curkey & unicode) == curkey)
	{
		int cr = unicode & 0xFF;
		trace("special=%d\n",cr);
		m_ownerWin->KeyDown(129+cr-4);// UP/DOWN/LEFT/RIGHT
	}
}

- (void) keyUp:(NSEvent *)theEvent
{
	NSString *chrs;//,*chrsNoMod;
	chrs=[theEvent characters];
	int unicode = 0;
	if([chrs length]>0)
	{
		unicode=[chrs characterAtIndex:0];
		if (0<=unicode && unicode<128)
			m_ownerWin->KeyUp(unicode);
	}
	trace("%s : %d\n",__FUNCTION__, unicode);
		
	
	const int curkey = 256*0xF7;
	if ((curkey & unicode) == curkey)
	{
		int cr = unicode & 0xFF;
		trace("special=%d\n",cr);
		m_ownerWin->KeyUp(129+cr-4);// UP/DOWN/LEFT/RIGHT
	}
}

- (void)scrollWheel:(NSEvent *)theEvent
{
    //if (! [self isEnabled]) return;
	if (!m_isMouseCursorIn) return;
	
	trace("%s %f %f %f\n",__FUNCTION__,[theEvent deltaX],[theEvent deltaY], [theEvent deltaZ]);
	
    // A delta of 6 represents a large change.  Let's consider a delta of 6 to represent an eighth of a full rotation - that is, pi/4 radians.
    // Therefore, we multiply by pi/4 and divide by 6, which is equivalent to multiplying by pi/24.
    //const CGFloat scrollWheelToRadiansConversionFactor = M_PI/24.;
    CGFloat deltaX = [theEvent deltaX], deltaY = [theEvent deltaY], deltaZ = [theEvent deltaZ];
    //if (deltaX != 0.) [self rotateAboutAxis:kYAxis byRadians:-deltaX * scrollWheelToRadiansConversionFactor];
    //if (deltaY != 0.) [self rotateAboutAxis:kXAxis byRadians:-deltaY * scrollWheelToRadiansConversionFactor];
    //if (deltaZ != 0.) [self rotateAboutAxis:kZAxis byRadians:deltaZ * scrollWheelToRadiansConversionFactor];
    
	m_ownerWin->Wheel(deltaX, deltaY, deltaZ);
    /* Send the action too */
    //if (deltaX || deltaY || deltaZ) {
	//	[NSApp sendAction:[self action] to:[self target] from:self];
    //}
}

- (void) magnifyWithEvent:(NSEvent *)event
{
	trace("%s %f\n",__FUNCTION__,[event magnification]);
	m_ownerWin->Magnify([event magnification]);
}

- (void) swipeWithEvent:(NSEvent *)event
{
	trace("%s %f %f %f\n",__FUNCTION__,[event deltaX],[event deltaY],[event deltaZ]);
	m_ownerWin->Swipe([event deltaX], [event deltaY], [event deltaZ]);
}

- (void) rotateWithEvent:(NSEvent *)event
{
	trace("%s %f\n",__FUNCTION__,[event rotation]);
	m_ownerWin->Rotate([event rotation]);
}

- (void) mouseMoved:(NSEvent *)theEvent
{
	trace("%s %d %d\n",__FUNCTION__,(int)[theEvent locationInWindow].x,(int)[theEvent locationInWindow].y);
	m_ownerWin->MouseMove((int)[theEvent locationInWindow].x, m_height - (int)[theEvent locationInWindow].y);
}

- (void) mouseDragged:(NSEvent *)theEvent
{
	trace("%s %d %d\n",__FUNCTION__,(int)[theEvent locationInWindow].x,(int)[theEvent locationInWindow].y);
	m_ownerWin->MouseMove((int)[theEvent locationInWindow].x, m_height - (int)[theEvent locationInWindow].y);
}

- (void) mouseDown:(NSEvent *)theEvent
{
	trace("%s %d %d\n",__FUNCTION__,(int)[theEvent locationInWindow].x,(int)[theEvent locationInWindow].y);
	m_ownerWin->MouseLeftDown((int)[theEvent locationInWindow].x,m_height - (int)[theEvent locationInWindow].y);
}

- (void) mouseUp:(NSEvent *)theEvent
{
	trace("%s %d %d\n",__FUNCTION__,(int)[theEvent locationInWindow].x,(int)[theEvent locationInWindow].y);
	m_ownerWin->MouseLeftUp((int)[theEvent locationInWindow].x,m_height - (int)[theEvent locationInWindow].y);
}

- (void) rightMouseDown:(NSEvent *)theEvent
{
	trace("%s %d %d\n",__FUNCTION__,(int)[theEvent locationInWindow].x,(int)[theEvent locationInWindow].y);
	m_ownerWin->MouseRightDown((int)[theEvent locationInWindow].x,m_height - (int)[theEvent locationInWindow].y);
}

- (void) rightMouseUp:(NSEvent *)theEvent
{
	trace("%s %d %d\n",__FUNCTION__,(int)[theEvent locationInWindow].x,(int)[theEvent locationInWindow].y);
	m_ownerWin->MouseRightUp((int)[theEvent locationInWindow].x,m_height - (int)[theEvent locationInWindow].y);
}

- (void) rightMouseDragged:(NSEvent *)theEvent
{
	trace("%s %d %d\n",__FUNCTION__,(int)[theEvent locationInWindow].x,(int)[theEvent locationInWindow].y);
	m_ownerWin->MouseMove((int)[theEvent locationInWindow].x, m_height - (int)[theEvent locationInWindow].y);
}

- (void) otherMouseDown:(NSEvent *)theEvent
{
	trace("%s %d %d\n",__FUNCTION__,(int)[theEvent locationInWindow].x,(int)[theEvent locationInWindow].y);
	m_ownerWin->MouseMiddleDown((int)[theEvent locationInWindow].x,m_height - (int)[theEvent locationInWindow].y);
}

- (void) otherMouseUp:(NSEvent *)theEvent
{
	trace("%s %d %d\n",__FUNCTION__,(int)[theEvent locationInWindow].x,(int)[theEvent locationInWindow].y);
	m_ownerWin->MouseMiddleUp((int)[theEvent locationInWindow].x,m_height - (int)[theEvent locationInWindow].y);
}

- (void) otherMouseDragged:(NSEvent *)theEvent
{
	trace("%s %d %d\n",__FUNCTION__,(int)[theEvent locationInWindow].x,(int)[theEvent locationInWindow].y);
	m_ownerWin->MouseMove((int)[theEvent locationInWindow].x, m_height - (int)[theEvent locationInWindow].y);
}


@end

namespace {
   
void skAddMenu(void)
{
 	NSAutoreleasePool *pool=[[NSAutoreleasePool alloc] init];
	
	NSMenu *mainMenu;
	
	mainMenu=[NSMenu alloc];
	[mainMenu initWithTitle:@"Minimum"];
	
	NSMenuItem *fileMenu;
	fileMenu=[[NSMenuItem alloc] initWithTitle:@"File" action:NULL keyEquivalent:[NSString string]];
	[mainMenu addItem:fileMenu];
	
	NSMenu *fileSubMenu;
	fileSubMenu=[[NSMenu alloc] initWithTitle:@"File"];
	[fileMenu setSubmenu:fileSubMenu];
	
	NSMenuItem *fileMenu_Quit;
	fileMenu_Quit=[[NSMenuItem alloc] initWithTitle:@"Quit"  action:@selector(terminate:) keyEquivalent:@"q"];
	[fileMenu_Quit setTarget:NSApp];
	[fileSubMenu addItem:fileMenu_Quit];
	
	[NSApp setMainMenu:mainMenu];
	
	[pool release];
}

void skTestApplicationPath(void)
{
 	NSAutoreleasePool *pool=[[NSAutoreleasePool alloc] init];
	
	char cwd[256];
	getcwd(cwd,255);
	trace("CWD(Initial): %s\n",cwd);
	
	NSString *path;
	path=[[NSBundle mainBundle] bundlePath];
	trace("BundlePath:%s\n",[path UTF8String]);
	
	[[NSFileManager defaultManager] changeCurrentDirectoryPath:path];
	
	getcwd(cwd,255);
	trace("CWD(Changed): %s\n",cwd);
	
	[pool release];
}


void skPollDeviceC(void)
{
 	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	while(1)
	{
	 	[pool release];
	 	pool=[[NSAutoreleasePool alloc] init];
		
		NSEvent *event;
		event = [NSApp
				 nextEventMatchingMask:NSAnyEventMask
				 untilDate: [NSDate distantPast]
				 inMode: NSDefaultRunLoopMode
				 dequeue:YES];
		if([event type] == NSRightMouseDown)
		{
		    trace("R mouse down event\n");
		}
		if(event!=nil)
		{
			[NSApp sendEvent:event];
			[NSApp updateWindows];
		}
		else
		{
			break;
		}
	}
	
	[pool release];	
}

} // namespace 


void CoreWindow::DoEvents()
{
	skPollDeviceC();
}

void CoreWindow::MainLoop()
{
	while (1)
	{
		skPollDeviceC();

		if (g_mainWin)
			g_mainWin->Idle();
	}
}

CoreWindow::CoreWindow(int x, int y, int width ,int height, const char* title, bool fullscreenMode)
{
	if (g_mainWin == 0)
		g_mainWin = this;
	NSAutoreleasePool *pool=[[NSAutoreleasePool alloc] init];
		
	[NSApplication sharedApplication];
		
	[NSApp finishLaunching];

    
	NSRect contRect;
	contRect = NSMakeRect(x, y, width, height);
	
	unsigned int winStyle =
	  NSTitledWindowMask
	//NSBorderlessWindowMask
	| NSClosableWindowMask
	| NSMiniaturizableWindowMask
	| NSResizableWindowMask
    | NSWindowFullScreenButton;
    
	m_win = [skOpenGLWindow alloc];
	[m_win
	 initWithContentRect:contRect
	 styleMask:winStyle
	 backing:NSBackingStoreBuffered
	 defer:NO];
	[m_win setTitle:[NSString stringWithUTF8String:title]];

	// for CloseButton
    skMacDelegate *delegate;
	delegate = [skMacDelegate alloc];
	[delegate init];
    //[NSApp setDelegate: delegate];
    NSNotificationCenter *c = [NSNotificationCenter defaultCenter];
    [c addObserver:NSApp selector: @selector(terminate:) name: @"NSWindowWillCloseNotification" object:m_win];

    
	NSOpenGLPixelFormat *format;
	NSOpenGLPixelFormatAttribute formatAttrib[] =
	{
		NSOpenGLPFAWindow,
		NSOpenGLPFADepthSize, (NSOpenGLPixelFormatAttribute)32,
		NSOpenGLPFAStencilSize, 1,
		NSOpenGLPFADoubleBuffer,
		0
	};
	
	format=[NSOpenGLPixelFormat alloc];
	[format initWithAttributes: formatAttrib];
	
	m_view = [skOpenGLView alloc];
	[m_view setOwnerWindow:this];
	contRect = NSMakeRect( 0, 0, width, height);
	[m_view
	 initWithFrame:contRect
	 pixelFormat:format];
	
	[m_win setContentView:m_view];
	[m_win makeFirstResponder:m_view];
	
	[m_win makeKeyAndOrderFront:nil];
	[m_win makeMainWindow];
	
    if (fullscreenMode)
        [m_view enterFullScreenMode:[NSScreen mainScreen] withOptions:nil];
    
    NSRect rc = [m_view bounds];
    m_w = rc.size.width;
    m_h = rc.size.height;
	[NSApp activateIgnoringOtherApps:YES];
	
	skAddMenu();

	[pool release];

}

CoreWindow::~CoreWindow()
{

}

void CoreWindow::Active()
{
	[[m_view openGLContext] makeCurrentContext];
}

void CoreWindow::SwapBuffer()
{
    glSwapAPPLE();
}

void CoreWindow::Toplevel(bool top)
{
	if (top)
		[m_win setLevel:NSPopUpMenuWindowLevel];
	else
		[m_win setLevel:NSNormalWindowLevel];
}

const char* CoreWindow::GetExePath() const
{
    static char exepath[2048];
    NSString *curDir = [[NSBundle mainBundle] bundlePath];
    strcpy(exepath, [curDir UTF8String]);
    return exepath;
}

