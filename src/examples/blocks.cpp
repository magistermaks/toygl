
#include <iostream>

#define CANVAS_SIZE 400
#define CANVAS_SCALE 2
#define CANVAS_TARGET (CANVAS_SCALE * CANVAS_SIZE)

#include "../common/window.hpp"
#include "../common/framerate.hpp"
#include "../common/font.h"

#define TOYGL_IMPLEMENT
#define TOYGL_ENABLE_DEPTH
#define TOYGL_ENABLE_3D
#define TOYGL_ENABLE_LOGO
#define TOYGL_ENABLE_TEXTURES
#define TOYGL_ENABLE_RGB
#include "../toygl.hpp"

using tgl::vec3f;
using tgl::vec2f;
using tgl::color;
using tgl::trig2f;
using tgl::byte;

void window_event_key( unsigned int keycode, bool pressed ) {

}

void draw_3d_cube( tgl::renderer* ctx, tgl::vec3f v ) {

	ctx->set_texture( true );

	ctx->set_texture_uv( trig2f( vec2f(0, 0), vec2f(0, 16), vec2f(16, 0) ) );
	ctx->draw_3d_triangle( vec3f(v.x - 1, v.y - 1, v.z + 1), vec3f(v.x - 1, v.y + 1, v.z + 1), vec3f(v.x + 1, v.y - 1, v.z + 1) );
	ctx->draw_3d_triangle( vec3f(v.x - 1, v.y - 1, v.z - 1), vec3f(v.x + 1, v.y - 1, v.z - 1), vec3f(v.x - 1, v.y + 1, v.z - 1) );
	ctx->draw_3d_triangle( vec3f(v.x + 1, v.y - 1, v.z - 1), vec3f(v.x + 1, v.y - 1, v.z + 1), vec3f(v.x + 1, v.y + 1, v.z - 1) );
	ctx->draw_3d_triangle( vec3f(v.x - 1, v.y - 1, v.z - 1), vec3f(v.x - 1, v.y + 1, v.z - 1), vec3f(v.x - 1, v.y - 1, v.z + 1) );
	ctx->draw_3d_triangle( vec3f(v.x - 1, v.y + 1, v.z - 1), vec3f(v.x + 1, v.y + 1, v.z - 1), vec3f(v.x - 1, v.y + 1, v.z + 1) );
	ctx->draw_3d_triangle( vec3f(v.x - 1, v.y - 1, v.z - 1), vec3f(v.x - 1, v.y - 1, v.z + 1), vec3f(v.x + 1, v.y - 1, v.z - 1) );

	ctx->set_texture_uv( trig2f( vec2f(16, 16), vec2f(16, 0), vec2f(0, 16) ) );
	ctx->draw_3d_triangle( vec3f(v.x + 1, v.y + 1, v.z + 1), vec3f(v.x + 1, v.y - 1, v.z + 1), vec3f(v.x - 1, v.y + 1, v.z + 1) );
	ctx->draw_3d_triangle( vec3f(v.x + 1, v.y + 1, v.z - 1), vec3f(v.x - 1, v.y + 1, v.z - 1), vec3f(v.x + 1, v.y - 1, v.z - 1) );
	ctx->draw_3d_triangle( vec3f(v.x + 1, v.y + 1, v.z + 1), vec3f(v.x + 1, v.y + 1, v.z - 1), vec3f(v.x + 1, v.y - 1, v.z + 1) );
	ctx->draw_3d_triangle( vec3f(v.x - 1, v.y + 1, v.z + 1), vec3f(v.x - 1, v.y - 1, v.z + 1), vec3f(v.x - 1, v.y + 1, v.z - 1) );
	ctx->draw_3d_triangle( vec3f(v.x + 1, v.y + 1, v.z + 1), vec3f(v.x - 1, v.y + 1, v.z + 1), vec3f(v.x + 1, v.y + 1, v.z - 1) );
	ctx->draw_3d_triangle( vec3f(v.x + 1, v.y - 1, v.z + 1), vec3f(v.x + 1, v.y - 1, v.z - 1), vec3f(v.x - 1, v.y - 1, v.z + 1) );

	ctx->set_texture( false );

}

int main(void) {

	window_open( "Blocks", CANVAS_TARGET, CANVAS_TARGET );

	std::cout << "Version: " << TOYGL_VERSION << std::endl;

	// create 16x16 RGB texture
	tgl::byte texture[ 16 * 16 * 3 ] = {
			56,32,20,56,32,20,56,32,20,56,32,20,56,32,20,56,32,20,56,32,20,56,32,20,56,32,20,56,32,20,56,32,20,56,32,20,56,32,20,56,32,20,56,32,20,56,32,20
			,56,32,20,68,52,20,56,32,20,148,104,48,228,152,72,228,152,72,148,104,48,56,32,20,56,32,20,148,104,48,228,152,72,228,152,72,148,104,48,56,32,20,68,52,20,56,32,20
			,56,32,20,56,32,20,148,104,48,228,152,72,244,216,180,240,188,116,228,152,72,68,52,20,56,32,20,228,152,72,240,188,116,244,216,180,228,152,72,148,104,48,56,32,20,56,32,20
			,56,32,20,68,52,20,228,152,72,244,216,180,244,216,180,228,152,72,148,104,48,56,32,20,56,32,20,148,104,48,228,152,72,244,216,180,244,216,180,228,152,72,68,52,20,56,32,20
			,56,32,20,148,104,48,244,216,180,244,216,180,228,152,72,148,104,48,56,32,20,148,104,48,228,152,72,56,32,20,148,104,48,228,152,72,244,216,180,244,216,180,148,104,48,56,32,20
			,56,32,20,228,152,72,240,188,116,228,152,72,148,104,48,56,32,20,228,152,72,240,188,116,240,188,116,228,152,72,56,32,20,148,104,48,228,152,72,240,188,116,228,152,72,56,32,20
			,56,32,20,148,104,48,228,152,72,148,104,48,56,32,20,228,152,72,244,216,180,244,216,180,244,216,180,240,188,116,228,152,72,56,32,20,148,104,48,228,152,72,148,104,48,56,32,20
			,56,32,20,56,32,20,68,52,20,56,32,20,148,104,48,240,188,116,244,216,180,244,216,180,244,216,180,240,188,116,240,188,116,228,152,72,56,32,20,68,52,20,56,32,20,56,32,20
			,56,32,20,56,32,20,56,32,20,56,32,20,228,152,72,228,152,72,240,188,116,244,216,180,240,188,116,240,188,116,228,152,72,240,188,116,56,32,20,56,32,20,56,32,20,56,32,20
			,56,32,20,228,152,72,148,104,48,68,52,20,56,32,20,148,104,48,228,152,72,240,188,116,240,188,116,228,152,72,240,188,116,56,32,20,68,52,20,228,152,72,148,104,48,56,32,20
			,56,32,20,228,152,72,240,188,116,228,152,72,68,52,20,56,32,20,148,104,48,228,152,72,228,152,72,240,188,116,56,32,20,68,52,20,228,152,72,240,188,116,228,152,72,56,32,20
			,56,32,20,228,152,72,244,216,180,244,216,180,228,152,72,68,52,20,56,32,20,148,104,48,228,152,72,56,32,20,68,52,20,228,152,72,244,216,180,244,216,180,228,152,72,56,32,20
			,56,32,20,148,104,48,228,152,72,244,216,180,244,216,180,228,152,72,68,52,20,56,32,20,56,32,20,68,52,20,228,152,72,244,216,180,244,216,180,228,152,72,148,104,48,56,32,20
			,56,32,20,56,32,20,148,104,48,228,152,72,244,216,180,240,188,116,148,104,48,68,52,20,56,32,20,228,152,72,240,188,116,244,216,180,228,152,72,148,104,48,56,32,20,56,32,20
			,56,32,20,68,52,20,56,32,20,68,52,20,148,104,48,228,152,72,228,152,72,56,32,20,56,32,20,148,104,48,228,152,72,148,104,48,68,52,20,56,32,20,68,52,20,56,32,20
			,56,32,20,56,32,20,56,32,20,56,32,20,56,32,20,56,32,20,56,32,20,56,32,20,56,32,20,56,32,20,56,32,20,56,32,20,56,32,20,56,32,20,56,32,20,56,32,20
	};

	size_t frame_count = 0;

	// create new TGL renderer
	tgl::renderer rend( CANVAS_SIZE, CANVAS_SIZE, window_draw );
	rend.set_distance( 15 );
	rend.set_texture_src( texture, 16, 16 );

	float radx = 0;
	float rady = 0;
	float scale = 0;

	// orbit
	float radc = 0;
	float radius = 8.0f;

	std::string text = "Idle: 0ns\n\rFPS: 0";

    while( true ) {

    	window_scan();
    	window_clear( 0xFFFFFF );
    	rend.clear_depth();

    	{ // draw

    		// draw TGL logo for first 2 seconds
    		if( frame_count < 120 ) {
    			int w = (CANVAS_SIZE - TOYGL_LOGO_WIDTH(5)) / 2;
    			int h = (CANVAS_SIZE - TOYGL_LOGO_HEIGHT(5)) / 2;
    			rend.draw_logo( tgl::vec2i(w, h), 5, tgl::rgb::black, tgl::rgb::red, tgl::rgb::green, tgl::rgb::blue, font8x8_basic );

    			goto next;
    		}

    		float px = radius * cos(radc);
			float pz = radius * sin(radc);

    		radx += 0.005;
    		rady += 0.005;
    		radc -= 0.01;

    		scale += std::min(0.01 * (2 - scale), 0.1);

    		// set rotation and scale
    		rend.set_scale( scale / 2 );
    		rend.set_rotation( tgl::vec3f(radx, rady, 0) );

    		draw_3d_cube( &rend, tgl::vec3f(0, 0, 0) );
    		draw_3d_cube( &rend, tgl::vec3f(3, 0, 0) );
    		draw_3d_cube( &rend, tgl::vec3f(-3, 0, 0) );
    		draw_3d_cube( &rend, tgl::vec3f(0, 3, 0) );
    		draw_3d_cube( &rend, tgl::vec3f(0, -3, 0) );
    		draw_3d_cube( &rend, tgl::vec3f(0, 0, 3) );
    		draw_3d_cube( &rend, tgl::vec3f(0, 0, -3) );

    		draw_3d_cube( &rend, tgl::vec3f(px, 0, pz) );

    		rend.set_color( tgl::rgb::black );
    		rend.draw_string(4, 4, text.c_str(), font8x8_basic);

    	}

    	next:

    	long_time_t fps, frame, wait;
    	if( framerate( 60, fps, frame, wait ) ) {
    		text = "Idle: " + std::to_string(wait) + "ns\n\rFPS: " + std::to_string(fps);
    	}

    	window_update();
    	frame_count ++;

    };

    window_close();

    return 0;

}
