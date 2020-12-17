
/*
 * This is not a part of TGL, its a simple X11/WinAPI
 * window API used as a canvas for TGL to draw on.
 * But it isn't necessary, by any means, for its operation.
 */

#pragma once

#define WINDOW_X11

#ifndef CANVAS_SCALE
#	define CANVAS_SCALE 1
#endif

#include <cstdlib>

#ifdef WINDOW_X11
#	include <X11/Xlib.h>
#endif

#ifdef WINDOW_WINAPI
#	include <windows.h>
#endif

void window_open( const char* name, int width, int height );
void window_close();
void window_update();
void window_draw( unsigned int x, unsigned int y, unsigned char* color );
void window_clear( unsigned int rgb );
void window_scan();

bool mouse_down_flag = false;
int mouse_pos_x = 0;
int mouse_pos_y = 0;

void window_event_key( unsigned int keycode, bool pressed );

// X11 implementation
#ifdef WINDOW_X11

Display* display;
Window window;
GC gc;
Pixmap pixmap;
int width, height;
int connection;

void window_open( const char* name, int w, int h ) {

	// open connection to the server and create X11 window
	display = XOpenDisplay( NULL );

	// Unable to open display
	if( display == NULL ) {
		exit( -1 );
	}

	connection = DefaultScreen( display );
	window = XCreateSimpleWindow(
		display,
		RootWindow(display, connection),
		1, 1, w, h, 1,
		BlackPixel(display, connection),
		WhitePixel(display, connection)
	);

	// Select some events
	XSelectInput(display, window,
		PointerMotionMask |
		KeyPressMask |
		KeyReleaseMask |
		ButtonPressMask |
		ButtonReleaseMask |
		PointerMotionMask |
		ExposureMask
	);

	// map (show) the window
	XMapWindow(display, window);

	// set window name
	XStoreName( display, window, name );
	XSetIconName( display, window, name );

	// create pixmap and gc
	gc = DefaultGC(display, connection);
	pixmap = XCreatePixmap( display, window, w, h, 24 /* RGB */ );

	width = w;
	height = h;

}

void window_close() {
	XFreeGC( display, gc );
	XDestroyWindow( display, window );
	XCloseDisplay( display );
}

void window_update() {
	XCopyArea( display, pixmap, window, gc, 0, 0, CANVAS_SCALE * width, CANVAS_SCALE * height, 0, 0 );
}

void window_draw( unsigned int x, unsigned int y, unsigned char* c ) {
	XSetForeground( display, gc, c[0] + (c[1] << 8) + (c[2] << 16) );
	XFillRectangle( display, pixmap, gc, x * CANVAS_SCALE, y * CANVAS_SCALE, CANVAS_SCALE, CANVAS_SCALE );
}

void window_clear( unsigned int rgb ) {
	XSetForeground( display, gc, rgb );
	XFillRectangle( display, pixmap, gc, 0, 0, CANVAS_SCALE * width, CANVAS_SCALE * height );
}

void window_scan() {
	XEvent event;
	XNextEvent(display, &event);

	if (event.type == ButtonPress) {
		mouse_down_flag = true;
	}else if (event.type == ButtonRelease) {
		mouse_down_flag = false;
	}else if (event.type == MotionNotify) {
		mouse_pos_x = event.xbutton.x;
		mouse_pos_y = event.xbutton.y;
	}else if (event.type == KeyPress) {
		window_event_key( event.xkey.keycode, true );
	}else if (event.type == KeyRelease) {
		window_event_key( event.xkey.keycode, false );
	}
}

#endif

// WinAPI implementation
#ifdef WINDOW_WINAPI

void window_open( const char* name, int width, int height ) {

}

void window_close() {

}

void window_update() {

}

void window_draw( unsigned int x, unsigned int y, unsigned char* color ) {

}

void window_clear() {

}

void window_scan() {

}

#endif


