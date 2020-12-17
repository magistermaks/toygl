
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

#define TEX_SIZE 8
#define MAP_SIZE_X 8
#define MAP_SIZE_Y 8

#define A 51, 51, 51
#define B 95, 95, 95
#define C 125, 125, 125
#define D 63, 63, 63

tgl::byte texture_wall[ 8 * 8 * 3 ] = {
		A, A, A, A, A, A, A, C,
		A, B, B, B, B, B, B, C,
		A, B, C, B, B, C, B, C,
		A, B, B, B, B, B, B, C,
		A, B, B, B, B, B, B, C,
		A, B, C, B, B, C, B, C,
		A, B, B, B, B, B, B, C,
		A, C, C, C, C, C, C, C
};

tgl::byte texture_floor[ 8 * 8 * 3 ] = {
		D, A, A, A, D, A, A, A,
		D, C, D, C, D, C, D, C,
		A, A, D, A, A, A, D, A,
		D, C, D, C, D, C, D, C,
		D, A, A, A, D, A, A, A,
		D, C, D, C, D, C, D, C,
		A, A, D, A, A, A, D, A,
		D, C, D, C, D, C, D, C
};

#undef A
#undef B
#undef C
#undef D

vec3f pos( 0, 0, 0 );
vec3f rot( 0, 0, 0 );

bool bw = false, bs = false, ba = false, bd = false;

void move_player( float c ) {

	pos.z += c * cos( rot.y );
	pos.x += c * sin( rot.y );

}

void window_event_key( unsigned int keycode, bool pressed ) {
	if( keycode == 25 ) bw = pressed;
	if( keycode == 38 ) bs = pressed;
	if( keycode == 39 ) ba = pressed;
	if( keycode == 40 ) bd = pressed;
}

void draw_map( tgl::renderer* ctx, tgl::byte map[8][8] ) {

	ctx->set_texture( true );

	// draw all walls
	for( int y = 0; y < MAP_SIZE_Y; y ++ ) {
		for( int x = 0; x < MAP_SIZE_X; x ++ ) {

			if( map[y][x] ) {

				ctx->set_texture_src( texture_wall, TEX_SIZE, TEX_SIZE );

				if( map[y + 1][x] == 0 ) {
					vec3f v( x * 2, 0, y * 2 + 2 );
					ctx->set_texture_uv( trig2f( vec2f(0, 0), vec2f(0, TEX_SIZE), vec2f(TEX_SIZE, 0) ) );
					ctx->draw_3d_triangle( vec3f(v.x - 1, v.y - 1, v.z - 1), vec3f(v.x + 1, v.y - 1, v.z - 1), vec3f(v.x - 1, v.y + 1, v.z - 1) );
					ctx->set_texture_uv( trig2f( vec2f(TEX_SIZE, TEX_SIZE), vec2f(TEX_SIZE, 0), vec2f(0, TEX_SIZE) ) );
					ctx->draw_3d_triangle( vec3f(v.x + 1, v.y + 1, v.z - 1), vec3f(v.x - 1, v.y + 1, v.z - 1), vec3f(v.x + 1, v.y - 1, v.z - 1) );
				}

				if( map[y][x + 1] == 0 ) {
					vec3f v( x * 2 + 2, 0, y * 2);
					ctx->set_texture_uv( trig2f( vec2f(0, 0), vec2f(0, TEX_SIZE), vec2f(TEX_SIZE, 0) ) );
					ctx->draw_3d_triangle( vec3f(v.x - 1, v.y - 1, v.z - 1), vec3f(v.x - 1, v.y + 1, v.z - 1), vec3f(v.x - 1, v.y - 1, v.z + 1) );
					ctx->set_texture_uv( trig2f( vec2f(TEX_SIZE, TEX_SIZE), vec2f(TEX_SIZE, 0), vec2f(0, TEX_SIZE) ) );
					ctx->draw_3d_triangle( vec3f(v.x - 1, v.y + 1, v.z + 1), vec3f(v.x - 1, v.y - 1, v.z + 1), vec3f(v.x - 1, v.y + 1, v.z - 1) );
				}

				if( map[y - 1][x] == 0 ) {
					vec3f v( x * 2, 0, y * 2 - 2);
					ctx->set_texture_uv( trig2f( vec2f(0, 0), vec2f(0, TEX_SIZE), vec2f(TEX_SIZE, 0) ) );
					ctx->draw_3d_triangle( vec3f(v.x - 1, v.y - 1, v.z + 1), vec3f(v.x - 1, v.y + 1, v.z + 1), vec3f(v.x + 1, v.y - 1, v.z + 1) );
					ctx->set_texture_uv( trig2f( vec2f(TEX_SIZE, TEX_SIZE), vec2f(TEX_SIZE, 0), vec2f(0, TEX_SIZE) ) );
					ctx->draw_3d_triangle( vec3f(v.x + 1, v.y + 1, v.z + 1), vec3f(v.x + 1, v.y - 1, v.z + 1), vec3f(v.x - 1, v.y + 1, v.z + 1) );
				}

				if( map[y][x - 1] == 0 ) {
					vec3f v( x * 2 - 2, 0, y * 2);
					ctx->set_texture_uv( trig2f( vec2f(0, 0), vec2f(0, TEX_SIZE), vec2f(TEX_SIZE, 0) ) );
					ctx->draw_3d_triangle( vec3f(v.x + 1, v.y - 1, v.z - 1), vec3f(v.x + 1, v.y - 1, v.z + 1), vec3f(v.x + 1, v.y + 1, v.z - 1) );
					ctx->set_texture_uv( trig2f( vec2f(TEX_SIZE, TEX_SIZE), vec2f(TEX_SIZE, 0), vec2f(0, TEX_SIZE) ) );
					ctx->draw_3d_triangle( vec3f(v.x + 1, v.y + 1, v.z + 1), vec3f(v.x + 1, v.y + 1, v.z - 1), vec3f(v.x + 1, v.y - 1, v.z + 1) );
				}

				ctx->set_texture_src( texture_floor, TEX_SIZE, TEX_SIZE );

				vec3f v( x * 2, -2, y * 2);
				ctx->set_texture_uv( trig2f( vec2f(0, 0), vec2f(0, TEX_SIZE), vec2f(TEX_SIZE, 0) ) );
				ctx->draw_3d_triangle( vec3f(v.x - 1, v.y + 1, v.z - 1), vec3f(v.x + 1, v.y + 1, v.z - 1), vec3f(v.x - 1, v.y + 1, v.z + 1) );
				ctx->set_texture_uv( trig2f( vec2f(TEX_SIZE, TEX_SIZE), vec2f(TEX_SIZE, 0), vec2f(0, TEX_SIZE) ) );
				ctx->draw_3d_triangle( vec3f(v.x + 1, v.y + 1, v.z + 1), vec3f(v.x - 1, v.y + 1, v.z + 1), vec3f(v.x + 1, v.y + 1, v.z - 1) );

			}

		}
	}

	ctx->set_texture( false );

}

int main(void) {

	window_open( "Game", CANVAS_TARGET, CANVAS_TARGET );
	std::cout << "Version: " << TOYGL_VERSION << std::endl;

	tgl::byte map[ 8 ][ 8 ] = {
			{0, 0, 0, 0, 0, 0, 0, 0},
			{0, 1, 1, 1, 1, 1, 1, 0},
			{0, 0, 0, 0, 0, 0, 1, 0},
			{0, 0, 0, 0, 0, 0, 1, 0},
			{0, 1, 1, 1, 0, 0, 1, 0},
			{0, 1, 1, 1, 1, 1, 1, 0},
			{0, 1, 1, 1, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0},
	};

	size_t frame_count = 0;

	// create new TGL renderer
	tgl::renderer rend( CANVAS_SIZE, CANVAS_SIZE, window_draw );
	rend.set_distance( 0 );
	rend.set_clip( 0.001, 255 );

	std::string text = "Idle: 0ns\n\rFPS: 0";

    while( true ) {

    	window_scan();
    	window_clear( 0xFFFFFF ); // 0x3F3F3F
    	rend.clear_depth();

    	if( bw ) move_player( +0.2 ); //pos.z += 0.1;
    	if( bs ) rot.y += 0.05;
    	if( ba ) move_player( -0.2 );//pos.z -= 0.1;
    	if( bd ) rot.y -= 0.05;

    	{ // draw

    		// draw TGL logo for first 2 seconds
    		if( frame_count < 120 ) {
    			int w = (CANVAS_SIZE - TOYGL_LOGO_WIDTH(5)) / 2;
    			int h = (CANVAS_SIZE - TOYGL_LOGO_HEIGHT(5)) / 2;
    			rend.draw_logo( tgl::vec2i(w, h), 5, tgl::rgb::black, tgl::rgb::red, tgl::rgb::green, tgl::rgb::blue, font8x8_basic );

    			goto next;
    		}

    		rend.set_rotation( rot );
    		rend.set_camera( pos );

    		// draw map
    		draw_map( &rend, map );

    		rend.set_color( tgl::rgb::black );
    		rend.draw_string(4, 4, text.c_str(), font8x8_basic);

    	}

    	next:

		window_update();

    	long_time_t fps, frame, wait;
    	if( framerate( 60, fps, frame, wait, window_scan ) ) {
    		text = "Idle: " + std::to_string(wait) + "ns\n\rFPS: " + std::to_string(fps);
    	}

    	frame_count ++;

    };

    window_close();

    return 0;

}
