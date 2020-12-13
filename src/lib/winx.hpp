
#ifndef WINX_HPP_
#define WINX_HPP_

#include <stdexcept>

#ifdef WINX_WINAPI
#include <windows.h>
#include <string>
#endif

#ifdef WINX_X11
#include <X11/Xlib.h>
#endif

class WinxWindow;
class WinxException;
struct WinxWindowState;

#ifdef WINX_X11
typedef void (*WinxCallback) (XEvent evt, WinxWindow& ww);
struct WinxWindowState {

	WinxCallback callback;
	Display* display;
	Window window;
	int con;

};
#endif

#ifdef WINX_WINAPI
struct WinxWindowState {

	HWND hwnd;
	WNDCLASSEX wc;
	std::string className;

};

LRESULT CALLBACK _WinxWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif


class WinxException: public std::exception {

	private:
		std::string error;

	public:
		explicit WinxException ( const std::string error );
		virtual const char* what() const throw();
};

// Fail if no interface was specified
#ifndef WINX_X11
#	ifndef WINX_WINAPI
#		error "Winx error: please specify backend library, define WINX_X11 or WINX_WINAPI."
#	endif
#endif

#define WINX_FAIL( err ) throw WinxException( err )

/// Main window class ///
class WinxWindow {

	public:
		~WinxWindow();
		void setTitle( const char* title );
		WinxWindowState& getState();
		bool tick();

#ifdef WINX_X11
		WinxWindow( int x, int y, int w, int h, WinxCallback callback = [] (XEvent evt, WinxWindow& ww) {} );
#endif

#ifdef WINX_WINAPI
		WinxWindow( int x, int y, int w, int h, WNDPROC callback = _WinxWndProc );
#endif

	private:
		WinxWindowState state;

};

#ifdef WINX_IMPLEMENT

#ifdef WINX_WINAPI
LRESULT CALLBACK _WinxWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
#endif

/// Winx exception constructor ///
WinxException::WinxException(const std::string err) {
	this->error = err;
}

/// Get exception reason ///
const char* WinxException::what() const throw() {
	return this->error.c_str();
}

#ifdef WINX_X11
WinxWindow::WinxWindow( int x, int y, int w, int h, WinxCallback callback ) {

	// open connection to the server and create X11 window
	state.display = XOpenDisplay( NULL );
	if( state.display == NULL ) WINX_FAIL( "Unable to open display" );
	state.con = DefaultScreen( state.display );
	state.window = XCreateSimpleWindow(
				state.display,
				RootWindow(state.display, state.con),
				x, y, w, h, 1,
				BlackPixel(state.display, state.con),
				WhitePixel(state.display, state.con)
			);

	// Select some events
	XSelectInput(state.display, state.window,
				KeyPressMask |
				KeyReleaseMask |
				ButtonPressMask |
				ButtonReleaseMask |
				EnterWindowMask |
				LeaveWindowMask |
				PointerMotionMask |
				FocusChangeMask |
				ExposureMask
			);

	// map (show) the window
	XMapWindow(state.display, state.window);

	state.callback = callback;

}
#endif

#ifdef WINX_WINAPI
WinxWindow::WinxWindow( int x, int y, int w, int h, WNDPROC callback ) {

	HINSTANCE hInstance = GetModuleHandle(NULL);

	// Generate pseudo-random class name
	const char charset[] = "abcdefghijklmnopqrstuvwxyz";
	state.className = "winx_";
	for( int i = 0; i < 8; i ++ ) {
		state.className += charset[ rand() % sizeof(charset) - 1 ];
	}

	// Create window class
    state.wc = {0};
	state.wc.cbSize        = sizeof(WNDCLASSEX);
	state.wc.style         = CS_HREDRAW | CS_VREDRAW;
	state.wc.lpfnWndProc   = callback;
	state.wc.cbClsExtra    = 0;
	state.wc.cbWndExtra    = 0;
	state.wc.hInstance     = hInstance;
	state.wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	state.wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	state.wc.hbrBackground = (HBRUSH) (COLOR_WINDOW+1);
	state.wc.lpszMenuName  = NULL;
	state.wc.lpszClassName = state.className.c_str();
	state.wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

	if( !RegisterClassEx( &(state.wc) ) ) WINX_FAIL( "Window class registration failed" );

	// create window
	state.hwnd = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			state.className.c_str(),
			"",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, w, h,
			NULL, NULL, hInstance, NULL
		);

	if( state.hwnd == NULL ) WINX_FAIL( "Window creation failed" );

	// show window
	ShowWindow(state.hwnd, SW_SHOW);
	UpdateWindow(state.hwnd);

}
#endif

/// Window Destructor ///
WinxWindow::~WinxWindow() {

#ifdef WINX_X11
	XCloseDisplay( state.display );
#endif

#ifdef WINX_WINAPI
	DestroyWindow( state.hwnd );
#endif

}

/// Set Window Title ///
void WinxWindow::setTitle( const char* title ) {

#ifdef WINX_X11
	XStoreName( state.display, state.window, title );
	XSetIconName( state.display, state.window, title );
#endif

#ifdef WINX_WINAPI
	SetWindowTextA( state.hwnd, title );
#endif

}

/// Get platform depended internal state object reference ///
WinxWindowState& WinxWindow::getState() {
	return this->state;
}


#ifdef WINX_X11
bool WinxWindow::tick() {

	XEvent event;
	XNextEvent(this->state.display, &event);
	this->state.callback(event, *this);

	return true;
}
#endif

#ifdef WINX_WINAPI
bool WinxWindow::tick() {
	MSG msg;

	if( GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
	    DispatchMessage(&msg);
	    return true;
	}

	return false;
}
#endif

#undef WINX_IMPLEMENT

#endif // WINX_IMPLEMENT

#endif /* WINX_HPP_ */
