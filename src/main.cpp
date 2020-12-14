
#define WINX_X11
#define WINX_IMPLEMENT
#include "lib/winx.hpp"

typedef unsigned long long long_time_t;

#define TOYGL_IMPLEMENT
#define TOYGL_ENABLE_DEPTH
#define TOYGL_ENABLE_3D
#define TOYGL_ENABLE_LOGO
#define TOYGL_ENABLE_TEXTURES
#define TOYGL_ENABLE_RGB
#include "toygl.hpp"

using tgl::vec3f;
using tgl::vec2f;
using tgl::color;
using tgl::trig2f;
using tgl::byte;

#include "lib/font.h"
#include <sys/time.h>
#include <unistd.h>
#include <iostream>

#define CANVAS_SIZE 400
#define CANVAS_SCALE 2
#define CANVAS_TARGET (CANVAS_SCALE * CANVAS_SIZE)

WinxWindow win( 10, 10, CANVAS_TARGET, CANVAS_TARGET );
GC gc = DefaultGC(win.getState().display, win.getState().con);
Pixmap pixmap = XCreatePixmap(win.getState().display, win.getState().window, CANVAS_TARGET, CANVAS_TARGET, 24 );

long_time_t get_time() {
	struct timeval tp;
	gettimeofday(&tp, NULL);
	return tp.tv_sec * 1000ull * 1000ull + tp.tv_usec;
}

void X11_pixmap_draw( uint x, uint y, tgl::color c ) {
	auto& state = win.getState();
	XSetForeground( state.display, gc, tgl::math::rgb(c[0], c[1], c[2]) );
	XFillRectangle( state.display, pixmap, gc, x * CANVAS_SCALE, y * CANVAS_SCALE, CANVAS_SCALE, CANVAS_SCALE );
}

void X11_pixmap_update() {
	XCopyArea( win.getState().display, pixmap, win.getState().window, gc, 0, 0, CANVAS_TARGET, CANVAS_TARGET, 0, 0 );
}

void X11_pixmap_clear( byte r, byte g, byte b ) {
	XSetForeground( win.getState().display, gc, tgl::math::rgb(r, g, b) );
	XFillRectangle( win.getState().display, pixmap, gc, 0, 0, CANVAS_TARGET, CANVAS_TARGET );
}

bool framerate( int target, long_time_t& fps, long_time_t& frame, long_time_t& wait ) {

	const long_time_t now = get_time();
	const long_time_t max = 1000000 / target;

	static int count = 0;
	static long_time_t start = now;
	static long_time_t begin = now;

	fps = ++count;
	frame = (now - begin);
	wait = max - frame;

	if( now >= begin && max >= frame ) {

		usleep( wait );

	}else{

		wait = 0;

	}

	begin = get_time();

	if( now - start >= 1000000ull ) {
	    start = now;
	    count = 0;
	    return true;
	}

	return false;

}

void draw_3d_cube( tgl::renderer* ctx, tgl::vec3f v ) {

	ctx->set_color( tgl::rgb::red );
	ctx->draw_3d_triangle( vec3f(v.x - 1, v.y - 1, v.z + 1), vec3f(v.x - 1, v.y + 1, v.z + 1), vec3f(v.x + 1, v.y - 1, v.z + 1) );
	ctx->draw_3d_triangle( vec3f(v.x + 1, v.y + 1, v.z + 1), vec3f(v.x + 1, v.y - 1, v.z + 1), vec3f(v.x - 1, v.y + 1, v.z + 1) );

	ctx->set_color( tgl::rgb::red );
	ctx->draw_3d_triangle( vec3f(v.x - 1, v.y - 1, v.z - 1), vec3f(v.x + 1, v.y - 1, v.z - 1), vec3f(v.x - 1, v.y + 1, v.z - 1) );
	ctx->draw_3d_triangle( vec3f(v.x + 1, v.y + 1, v.z - 1), vec3f(v.x - 1, v.y + 1, v.z - 1), vec3f(v.x + 1, v.y - 1, v.z - 1) );

	ctx->set_color( tgl::rgb::blue );
	ctx->draw_3d_triangle( vec3f(v.x + 1, v.y - 1, v.z - 1), vec3f(v.x + 1, v.y - 1, v.z + 1), vec3f(v.x + 1, v.y + 1, v.z - 1) );
	ctx->draw_3d_triangle( vec3f(v.x + 1, v.y + 1, v.z + 1), vec3f(v.x + 1, v.y + 1, v.z - 1), vec3f(v.x + 1, v.y - 1, v.z + 1) );

	ctx->set_color( tgl::rgb::blue );
	ctx->draw_3d_triangle( vec3f(v.x - 1, v.y - 1, v.z - 1), vec3f(v.x - 1, v.y + 1, v.z - 1), vec3f(v.x - 1, v.y - 1, v.z + 1) );
	ctx->draw_3d_triangle( vec3f(v.x - 1, v.y + 1, v.z + 1), vec3f(v.x - 1, v.y - 1, v.z + 1), vec3f(v.x - 1, v.y + 1, v.z - 1) );

	//set_color( tgl::rgb::green );
	ctx->set_texture( true );
	ctx->set_texture_uv( trig2f( vec2f(0, 0), vec2f(0, 8), vec2f(8, 0) ) );
	ctx->draw_3d_triangle( vec3f(v.x - 1, v.y + 1, v.z - 1), vec3f(v.x + 1, v.y + 1, v.z - 1), vec3f(v.x - 1, v.y + 1, v.z + 1) );

	ctx->set_texture_uv( trig2f( vec2f(8, 8), vec2f(8, 0), vec2f(0, 8) ) );
	ctx->draw_3d_triangle( vec3f(v.x + 1, v.y + 1, v.z + 1), vec3f(v.x - 1, v.y + 1, v.z + 1), vec3f(v.x + 1, v.y + 1, v.z - 1) );
	ctx->set_texture( false );

	ctx->set_color( tgl::rgb::green );
	ctx->draw_3d_triangle( vec3f(v.x - 1, v.y - 1, v.z - 1), vec3f(v.x - 1, v.y - 1, v.z + 1), vec3f(v.x + 1, v.y - 1, v.z - 1) );
	ctx->draw_3d_triangle( vec3f(v.x + 1, v.y - 1, v.z + 1), vec3f(v.x + 1, v.y - 1, v.z - 1), vec3f(v.x - 1, v.y - 1, v.z + 1) );

}

int main(void) {

	std::cout << "Version: " << TOYGL_VERSION << std::endl;

#define R 255, 0, 0
#define G 0, 255, 0
#define B 0, 0, 255
#define F 200, 30, 143

	tgl::byte texture[ 8 * 8 * 3 ] = {
			F, F, F, F, F, F, F, F,
			F, B, B, R, R, B, B, F,
			F, B, G, R, R, G, B, F,
			F, R, R, R, R, R, R, F,
			F, G, G, R, R, G, G, F,
			F, B, G, G, G, G, B, F,
			F, B, B, B, B, B, B, F,
			F, F, F, F, F, F, F, F
	};

	size_t frame_count = 0;

	tgl::renderer rend( CANVAS_SIZE, CANVAS_SIZE, X11_pixmap_draw );
	rend.set_distance( 15 );
	rend.set_texture_src( texture, 8, 8 );

	win.setTitle( "Hello ToyGL/WINX!" );

//	tgl::byte bg_circle_color[] = { 200, 200, 200 };

	float radx = 0;
	float rady = 0;
	float scale = 0;

	// orbit
	float radc = 0;
	float radius = 8.0f;

	std::string text = "Idle: 0ns\n\rFPS: 0";

    while( win.tick() ) {

    	X11_pixmap_clear( 255, 255, 255 );
    	rend.clear_depth();

    	{ // render

    		if( frame_count < 120 ) {
    			int w = (CANVAS_SIZE - TOYGL_LOGO_WIDTH(5)) / 2;
    			int h = (CANVAS_SIZE - TOYGL_LOGO_HEIGHT(5)) / 2;
    			rend.draw_logo( tgl::vec2i(w, h), 5, tgl::rgb::black, tgl::rgb::red, tgl::rgb::green, tgl::rgb::blue, font8x8_basic );

    			goto next;
    		}

//    		renderer.set_color( bg_circle_color );
//    		renderer.draw_circle( tgl::vec2i( CANVAS_SIZE/2, CANVAS_SIZE/2 ), CANVAS_SIZE/10 );

    		float px = radius * cos(radc);
			float pz = radius * sin(radc);

    		radx += 0.005;
    		rady += 0.005;
    		radc -= 0.01;

    		scale += std::min(0.01 * (2 - scale), 0.1);

    		rend.set_scale( scale / 2 );
    		rend.set_rotation( tgl::vec3f(radx, rady, 0) );

    		draw_3d_cube( &rend,  tgl::vec3f(0, 0, 0) );
    		draw_3d_cube( &rend, tgl::vec3f(3, 0, 0) );
    		draw_3d_cube( &rend, tgl::vec3f(-3, 0, 0) );
    		draw_3d_cube( &rend, tgl::vec3f(0, 3, 0) );
    		draw_3d_cube( &rend, tgl::vec3f(0, -3, 0) );
    		draw_3d_cube( &rend, tgl::vec3f(0, 0, 3) );
    		draw_3d_cube( &rend, tgl::vec3f(0, 0, -3) );

    		draw_3d_cube( &rend, tgl::vec3f(px, 0, pz) );

//    		renderer.set_texture(true);
//    		renderer.draw_texture( 0, 0 );
//    		renderer.set_texture(false);

    		rend.set_color( tgl::rgb::black );
    		rend.draw_string(4, 4, text.c_str(), font8x8_basic);

    	}

    	next:

    	long_time_t fps, frame, wait;
    	if( framerate( 60, fps, frame, wait ) ) {
    		text = "Idle: " + std::to_string(wait) + "ns\n\rFPS: " + std::to_string(fps);
    	}

    	X11_pixmap_update();
    	frame_count ++;

    };

    return 0;

}
