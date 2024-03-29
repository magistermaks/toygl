
#include <iostream>

#define CANVAS_SIZE 400
#define CANVAS_SCALE 2
#define CANVAS_TARGET (CANVAS_SCALE * CANVAS_SIZE)

#include "../common/window.hpp"
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

	// apply texture
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

	window_open( "Cubes", CANVAS_TARGET, CANVAS_TARGET, true );

	std::cout << "Version: " << TOYGL_VERSION << std::endl;

#define R 255, 0, 0
#define G 0, 255, 0
#define B 0, 0, 255
#define F 100, 230, 143

	// create 8x8 RGB texture
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

	// create new TGL renderer
	tgl::renderer rend( CANVAS_SIZE, CANVAS_SIZE, window_draw );
	rend.set_distance( 15 );
	rend.set_texture_src( texture, 8, 8 );

	float radx = 0;
	float rady = 0;
	float scale = 0;

	// orbit
	float radc = 0;
	float radius = 8.0f;

	std::string text = "FPS: 0";

	while( !should_close ) {

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

			draw_3d_cube( &rend,  tgl::vec3f(0, 0, 0) );
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

		long_time_t fps;
		if( framerate(fps) ) {
			text = "FPS: " + std::to_string(fps);
		}

		window_update();
		frame_count ++;

	};

	window_close();

	return 0;

}
