
/*
 * MIT License
 *
 * Copyright (c) 2020 magistermaks
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
 * General Toy Graphics Library (TGL) documentation:
 *
 *		TGL is a minimalistic, configurable and dependency-less graphics library
 *		created mainly as a passion project. It can, however, be used as graphics
 *		library for microcontrollers (like Arduino) because of it's relatively small
 *		size, or as a blueprint for other graphics libraries. TGL uses only basic C++
 *		features, and does NOT require C++11.
 *
 *		TGL is better than CHAD ENGINE.
 *
 * 1. Sources:
 *
 * 		en.wikipedia.org/wiki/3D_projection
 * 		learnopengl.com/Advanced-OpenGL/Depth-testing
 * 		learnopengl.com/Getting-started/Transformations
 * 		github.com/willnode/N-Matrix-Programmer
 *
 * 2. Preprocessor:
 *
 * 		define TOYGL_IMPLEMENT to implement TGL header
 * 		define TOYGL_ENABLE_3D to add 3D functions
 * 		define TOYGL_ENABLE_DEPTH to enable depth buffer
 * 		define TOYGL_RISCY_PIXEL to skip pixel position check
 * 		define TOYGL_CLAMP_PRIMITIVES to clamp 2D primitives to canvas size
 * 		define TOYGL_LOGO to add draw_logo method
 * 		define TOYGL_ENABLE_TEXTURES to add texture system
 * 		define TOYGL_RGB_CONSTS to add basic RGB color constants
 *
 * 		It is advised to use `TOYGL_RISCY_PIXEL` and `TOYGL_CLAMP_PRIMITIVES` together to avoid Segmentation Faults
 *
 * 3. TODO
 *
 */

#pragma once

#include <cmath>
#include <cfloat>

// define depth test enable/disable macros
#ifdef TOYGL_ENABLE_DEPTH
#	define __TOYGL_DEPTH_BEGIN this->depth_flag = this->depth_enable
#	define __TOYGL_DEPTH_END this->depth_flag = false
#	define __TOYGL_DEPTH_SET( val ) this->depth = (val)
#	define TOYGL_DEPTH( val, ... ) __TOYGL_DEPTH_BEGIN; __TOYGL_DEPTH_SET(val); __VA_ARGS__ __TOYGL_DEPTH_END;
#else
#	define __TOYGL_DEPTH_BEGIN
#	define __TOYGL_DEPTH_END
#	define TOYGL_DEPTH( val, ... ) __VA_ARGS__
#endif

#ifdef TOYGL_LOGO
#	define TOYGL_LOGO_HEIGHT( scale ) (scale * 24)
#	define TOYGL_LOGO_WIDTH( scale ) (scale * 41.333)
#endif

namespace tgl {

	typedef unsigned char byte;
	typedef unsigned int uint;
	typedef byte* color;
	typedef void (*pixel_placer)( byte, int, int, color );

	struct vec3f {

		float x;
		float y;
		float z;

		vec3f( float x, float y, float z );

	};

	struct vec3i {

		int x;
		int y;
		int z;

		vec3i( int x, int y, int z );

	};

	struct vec2f {

		float x;
		float y;

		vec2f();
		vec2f( float x, float y );

	};


	struct vec2i {

		int x;
		int y;

		vec2i( int x, int y );
		vec2f f();

	};

	struct mat3x3f {

		float m00, m01, m02;
		float m10, m11, m12;
		float m20, m21, m22;

		vec3f operator*( const vec3f& v );
		mat3x3f operator*( const mat3x3f& v );

	};

	struct trig2f {

		vec2f v1, v2, v3;

		trig2f( vec2f v1, vec2f v2, vec2f v3 );

	};

#ifdef TOYGL_RGB_CONSTS
	namespace rgb {

		const color white = (tgl::byte[]) {255, 255, 255};
		const color black = (tgl::byte[]) {0, 0, 0};
		const color red = (tgl::byte[]) {255, 0, 0};
		const color green = (tgl::byte[]) {0, 255, 0};
		const color blue = (tgl::byte[]) {0, 0, 255};

	}
#endif

	class Renderer {

		public:
			Renderer( uint x, uint y, pixel_placer placer, byte channels = 3 );
			~Renderer();

			void set_color( color col );

#ifdef TOYGL_ENABLE_DEPTH
			void set_depth( bool enable );
			void clear_depth();
#endif

#ifdef TOYGL_ENABLE_TEXTURES
		protected:
			void set_color_from_texture( int x, int y );

		public:
			void set_texture( bool enable );
			void set_texture_src( tgl::byte* buffer, uint width, uint height );
			void set_texture_uv( trig2f uv );

			mat3x3f triangle_mapping_matrix( trig2f t1, trig2f t2 );
#endif

#ifdef TOYGL_ENABLE_3D
			void set_rotation( vec3f rot );
			void set_camera( vec3f pos );
			void set_distance( float dist );
			void set_scale( float scale );
			void set_fov( float fov );
			void set_clip( float near, float far );
#endif

			inline void draw_pixel( uint x, uint y );
			void draw_line( vec2i v1, vec2i v2 );
			void draw_triangle( vec2i v1, vec2i v2, vec2i v3 );
			void draw_square( vec2i v1, vec2i v2 );
			void draw_circle( vec2i pos, int r );
			void draw_glyph( uint x, uint y, const byte* glyph, unsigned int scale = 1 );
			void draw_string( uint x, uint y, const char* text, const byte font[][8], unsigned int scale = 1, int vs = 1, int hs = 0, int fmax = 127, bool special = true );

#ifdef TOYGL_LOGO
			void draw_logo( vec2i pos, uint size, color fg, color a, color b, color c, const byte font[][8] );
#endif

#ifdef TOYGL_ENABLE_3D
			void project_vector( vec3f& vec );
			void draw_3d_line( vec3f v1, vec3f v2 );
			void draw_3d_triangle( vec3f v1, vec3f v2, vec3f v3 );
			void draw_3d_cube( vec3f pos );

		protected:
			float rxc, rxs;
			float ryc, rys;
			float rzc, rzs;

			vec3f cam = vec3f( 0, 0, 0 );
			float far, near, dist, fov, scale;
#endif

		protected:
			const uint width, xo;
			const uint height, yo;
			const int wen, hen;
			const byte channels;

			pixel_placer placer;
			color col;

#ifdef TOYGL_ENABLE_DEPTH
		protected:
			float* depth_buffer;
			float depth;
			bool depth_flag: 1;
			bool depth_enable: 1;
#endif

#ifdef TOYGL_ENABLE_TEXTURES
			bool texture_flag: 1;
			mat3x3f texture_matrix;
			uint texture_width, texture_height;
			tgl::byte* texture;
			trig2f texture_triangle = trig2f( vec2f(0, 0), vec2f(0, 0), vec2f(0, 0) );
#endif

	};

	namespace math {

		bool invert_matrix( const mat3x3f& m, mat3x3f& im );
		int max_clamp( int value, int max );
		float cross( float x1, float y1, float x2, float y2, float x3, float y3 );
		int max( int a, int b, int c );
		int max( int a, int b, int c, int d );
		int min( int a, int b, int c );
		int min( int a, int b, int c, int d );
		unsigned long rgb( byte r, byte g, byte b );
		int signum( int value );

	}

}

// BEGIN LIBRARY IMPLEMENTATION
#ifdef TOYGL_IMPLEMENT

inline void tgl::Renderer::draw_pixel( uint x, uint y ) {

#ifndef TOYGL_RISCY_PIXEL
	if( x >= width || y >= height ) return;
#endif

#ifdef TOYGL_ENABLE_DEPTH
	if( depth_flag ) {

		const int index = y * width + x;

		if( depth_buffer[index] > depth ) {
			depth_buffer[index] = depth;
		}else return;

	}
#endif

	placer( channels, x, y, col );

}

void tgl::Renderer::set_color( color col ) {
	this->col = col;
}

#ifdef TOYGL_ENABLE_DEPTH
void tgl::Renderer::set_depth( bool enable ) {
	this->depth_enable = enable;
}

void tgl::Renderer::clear_depth() {
	for( unsigned int i = 0; i < width * height; i ++ ) {
		depth_buffer[i] = FLT_MAX;
	}
}
#endif

#ifdef TOYGL_ENABLE_TEXTURES

void tgl::Renderer::set_texture( bool enable ) {
	texture_flag = ( texture == nullptr ) ? false : enable;
}

void tgl::Renderer::set_texture_src( tgl::byte* buffer, uint w, uint h ) {
	texture = buffer;
	texture_width = w - 1;
	texture_height = h - 1;
}

void tgl::Renderer::set_texture_uv( trig2f uv ) {
	texture_triangle = uv;
}
#endif

#ifdef TOYGL_ENABLE_3D
void tgl::Renderer::set_rotation( vec3f rot ) {
	this->rxc = cos(rot.x);
	this->rxs = sin(rot.x);
	this->ryc = cos(rot.y);
	this->rys = sin(rot.y);
	this->rzc = cos(rot.z);
	this->rzs = sin(rot.z);
}

void tgl::Renderer::set_camera( vec3f pos ) {
	this->cam = pos;
}

void tgl::Renderer::set_distance( float dist ) {
	this->dist = dist;
}

void tgl::Renderer::set_scale( float scale ) {
	this->scale = scale;
}

void tgl::Renderer::set_fov( float fov ) {
	this->fov = tan( fov / 2.0f );
}

void tgl::Renderer::set_clip( float near, float far ) {
	this->near = near;
	this->far = far;
}
#endif

#ifdef TOYGL_LOGO
void tgl::Renderer::draw_logo( vec2i pos, uint size, color fg, color a, color b, color c, const byte font[][8] ) {

	const int x = pos.x;
	const int y = pos.y;
	const int s = size * 8;
	const int h = size * 4;

	set_color( a );
	draw_triangle( vec2i( x + h, y + s ), vec2i( x + s + h, y + s ), vec2i( x + s, y ) );

	set_color( b );
	draw_square( vec2i( x + s * 2, y ), vec2i( x + s * 3, y + s ) );

	set_color( c );
	draw_circle( vec2i( x + s * 4, y + h ), h );

	set_color( fg );
	draw_string( x + h + (h/3), y + s + h, "TGL", font, size, 0, h, 255, false );

}
#endif

void tgl::Renderer::draw_line( vec2i v1, vec2i v2 ) {

#ifdef TOYGL_CLAMP_PRIMITIVES
	v1.x = tgl::math::max_clamp( v1.x, wen );
	v1.y = tgl::math::max_clamp( v1.y, hen );
	v2.x = tgl::math::max_clamp( v2.x, wen );
	v2.y = tgl::math::max_clamp( v2.y, hen );
#endif

	int lx = v2.x - v1.x;
	int ly = v2.y - v1.y;

	float inter = 0;
	const int xf = tgl::math::signum(lx);
	const int yf = tgl::math::signum(ly);

	lx *= xf;
	ly *= yf;

	if (lx > ly) {
		const float step = (float) ly / lx;
		while (inter < lx) {
			draw_pixel( inter * xf + v1.x, inter * step * yf + v1.y );
			inter ++;
		}
	}else{
		const float step = (float) lx / ly;
		while (inter < ly) {
			draw_pixel( inter * step * xf + v1.x, inter * yf + v1.y );
			inter ++;
		}
	}

}


void tgl::Renderer::draw_square( vec2i v1, vec2i v2 ) {

#ifdef TOYGL_CLAMP_PRIMITIVES
	v1.x = tgl::math::max_clamp( v1.x, wen );
	v1.y = tgl::math::max_clamp( v1.y, hen );
	v2.x = tgl::math::max_clamp( v2.x, wen );
	v2.y = tgl::math::max_clamp( v2.y, hen );
#endif

	const int xmax = std::max( v1.x, v2.x );
	const int xmin = std::min( v1.x, v2.x );
	const int ymax = std::max( v1.y, v2.y );
	const int ymin = std::min( v1.y, v2.y );

	for( int x = xmax; x >= xmin; x -- ) {
		for( int y = ymax; y >= ymin; y -- ) {

			draw_pixel( x, y );

		}
	}

}

void tgl::Renderer::draw_circle( vec2i pos, int r ) {

#ifdef TOYGL_CLAMP_PRIMITIVES
	const int xmax = tgl::math::max_clamp( pos.x + r, wen );
	const int ymax = tgl::math::max_clamp( pos.y + r, hen );
	const int xmin = tgl::math::max_clamp( pos.x - r, wen );
	const int ymin = tgl::math::max_clamp( pos.y - r, hen );
#else
	const int xmax = pos.x + r;
	const int xmin = pos.x - r;
	const int ymax = pos.y + r;
	const int ymin = pos.y - r;
#endif

	const int powr = r * r;

	for( int x = xmax; x >= xmin; x -- ) {
		bool painted = false;

		for( int y = ymax; y >= ymin; y -- ) {

			// simplified distance check
			if( std::pow( x - pos.x, 2 ) + std::pow( y - pos.y, 2 ) <= powr ) {

				draw_pixel( x, y );
				painted = true;

			}else if( painted ) {
				break;
			}

		}
	}

}

void tgl::Renderer::draw_glyph( uint x, uint y, const byte* glyph, unsigned int scale ) {

#ifdef TOYGL_CLAMP_PRIMITIVES
	x = x > (uint) wen ? wen : x;
	y = y > (uint) hen ? hen : y;
#endif

	for( int gx = 0; gx < 8; gx ++ ) {
		const unsigned int bit = 1 << gx;

		for( int gy = 0; gy < 8; gy ++ ) {
			if( glyph[gy] & bit ) {

				if( scale == 1 ) {
					draw_pixel( x + gx, y + gy );
				}else{
					const int sx = x + gx * scale;
					const int sy = y + gy * scale;
					draw_square( vec2i(sx, sy), vec2i(sx + scale - 1, sy + scale - 1) );
				}

			}
		}
	}

}

void tgl::Renderer::draw_string( uint x, uint y, const char* text, const byte font[][8], unsigned int scale, int vs, int hs, int fmax, bool special ) {

#ifdef TOYGL_CLAMP_PRIMITIVES
	x = x > (uint) wen ? wen : x;
	y = y > (uint) hen ? hen : y;
#endif

	const int x0 = x;

	for( int i = 0; true; i ++ ) {

		const char chr = text[i];
		if( chr != '\0' ) {

			if( (byte) chr > fmax ) continue;

			draw_glyph( x, y, (byte*) font[ (int) chr ], scale );

			if( special ) {

				if( chr == '\n' ) {
					y += 8 + vs;
					continue;
				}

				if( chr == '\r' ) {
					x = x0;
					continue;
				}

			}

			x += 8 * scale + hs;

		}else break;

	}

}

void tgl::Renderer::draw_triangle( vec2i v1, vec2i v2, vec2i v3 ) {

#ifdef TOYGL_CLAMP_PRIMITIVES
	const int xmax = tgl::math::max_clamp( tgl::math::max( v1.x, v2.x, v3.x ), wen );
	const int xmin = tgl::math::max_clamp( tgl::math::min( v1.x, v2.x, v3.x ), wen );
	const int ymax = tgl::math::max_clamp( tgl::math::max( v1.y, v2.y, v3.y ), hen );
	const int ymin = tgl::math::max_clamp( tgl::math::min( v1.y, v2.y, v3.y ), hen );
#else
	const int xmax = tgl::math::max( v1.x, v2.x, v3.x );
	const int xmin = tgl::math::min( v1.x, v2.x, v3.x );
	const int ymax = tgl::math::max( v1.y, v2.y, v3.y );
	const int ymin = tgl::math::min( v1.y, v2.y, v3.y );
#endif

#ifdef TOYGL_ENABLE_TEXTURES
	if( texture_flag ) texture_matrix = triangle_mapping_matrix( trig2f( v1.f(), v2.f(), v3.f() ), texture_triangle );
#endif

	for( int x = xmax; x >= xmin; x -- ) {
		bool painted = false;

		for( int y = ymax; y >= ymin; y -- ) {

			// is point (x,y) inside given triangle?
			const bool b1 = tgl::math::cross( x, y, v1.x, v1.y, v2.x, v2.y ) < 0;
			const bool b2 = tgl::math::cross( x, y, v2.x, v2.y, v3.x, v3.y ) < 0;
			const bool b3 = tgl::math::cross( x, y, v3.x, v3.y, v1.x, v1.y ) < 0;

			if( (b1 == b2) && (b2 == b3) ) {

#ifdef TOYGL_ENABLE_TEXTURES
				if( texture_flag ) set_color_from_texture( x, y );
#endif

				draw_pixel( x, y );
				painted = true;

			}else if( painted ) {
				break;
			}

		}
	}

}

#ifdef TOYGL_ENABLE_TEXTURES

void tgl::Renderer::set_color_from_texture( int x, int y ) {
	vec3f uv = texture_matrix * vec3f( x, y, 1 );

	const uint uvx = tgl::math::max_clamp( std::floor(uv.x), texture_width );
	const uint uvy = tgl::math::max_clamp( std::floor(uv.y), texture_height );

	col = texture + (uvy * (texture_width + 1) + uvx) * channels;
}

tgl::mat3x3f tgl::Renderer::triangle_mapping_matrix( trig2f t1, trig2f t2 ) {

	mat3x3f invm = {0};

	{
		const mat3x3f m1 = {
			t1.v1.x, t1.v2.x, t1.v3.x,
			t1.v1.y, t1.v2.y, t1.v3.y,
			1, 1, 1
		};

		if( !tgl::math::invert_matrix( m1, invm ) ) return mat3x3f {0};
	}

	mat3x3f m2 = {
		t2.v1.x, t2.v2.x, t2.v3.x,
		t2.v1.y, t2.v2.y, t2.v3.y,
		1, 1, 1
	};

	return m2 * invm;

}

#endif

#ifdef TOYGL_ENABLE_3D

	void tgl::Renderer::project_vector( vec3f& vec ) {

		vec.x -= cam.x;
		vec.y -= cam.y;
		vec.z -= cam.z;

		vec.x *= scale;
		vec.y *= scale;
		vec.z *= scale;

		const float a = rzs * vec.y + rzc * vec.x;
		const float b = rzc * vec.y - rzs * vec.x;
		const float c = ryc * vec.z + rys * a;

		vec.x = ryc * a - rys * vec.z;
		vec.y = rxs * c + rxc * b;
		vec.z = rxc * c - rxs * b;

		const float m = width / ((vec.z + dist) * fov);
		vec.x *= m;
		vec.y *= m;

		vec.z = (vec.z + dist) * (255.0f / far);
		vec.x += xo;
		vec.y += yo;

	}

	void tgl::Renderer::draw_3d_line( vec3f v1, vec3f v2 ) {

		project_vector( v1 );
		project_vector( v2 );

		TOYGL_DEPTH( (v1.z + v2.z) * 0.5f, {

			vec2i p1( std::round(v1.x), std::round(v1.y) );
			vec2i p2( std::round(v2.x), std::round(v2.y) );

			draw_line( p1, p2 );

		} );

	}

	void tgl::Renderer::draw_3d_triangle( vec3f v1, vec3f v2, vec3f v3 ) {

		project_vector( v1 );
		project_vector( v2 );
		project_vector( v3 );

		TOYGL_DEPTH( (v1.z + v2.z + v3.z) * 0.33f, {

			if (v1.z <= near || v2.z <= near || v3.z <= near || v1.z > far || v2.z > far || v3.z > far) return;

			vec2f a( v2.x - v1.x, v2.y - v1.y );
			vec2f b( v3.x - v1.x, v3.y - v1.y );

			// check normal
			if( (a.x * b.y - a.y * b.x) >= 0 ) {

				vec2i p1( std::round(v1.x) + 0, std::round(v1.y) + 0 );
				vec2i p2( std::round(v2.x) + 0, std::round(v2.y) + 0 );
				vec2i p3( std::round(v3.x) + 0, std::round(v3.y) + 0 );

				draw_triangle( p1, p2, p3 );

			}

		} );

	}

	void tgl::Renderer::draw_3d_cube( vec3f v ) {

		set_color( tgl::rgb::red );
		draw_3d_triangle( vec3f(v.x - 1, v.y - 1, v.z + 1), vec3f(v.x - 1, v.y + 1, v.z + 1), vec3f(v.x + 1, v.y - 1, v.z + 1) );
		draw_3d_triangle( vec3f(v.x + 1, v.y + 1, v.z + 1), vec3f(v.x + 1, v.y - 1, v.z + 1), vec3f(v.x - 1, v.y + 1, v.z + 1) );

		set_color( tgl::rgb::red );
		draw_3d_triangle( vec3f(v.x - 1, v.y - 1, v.z - 1), vec3f(v.x + 1, v.y - 1, v.z - 1), vec3f(v.x - 1, v.y + 1, v.z - 1) );
		draw_3d_triangle( vec3f(v.x + 1, v.y + 1, v.z - 1), vec3f(v.x - 1, v.y + 1, v.z - 1), vec3f(v.x + 1, v.y - 1, v.z - 1) );

		set_color( tgl::rgb::blue );
		draw_3d_triangle( vec3f(v.x + 1, v.y - 1, v.z - 1), vec3f(v.x + 1, v.y - 1, v.z + 1), vec3f(v.x + 1, v.y + 1, v.z - 1) );
		draw_3d_triangle( vec3f(v.x + 1, v.y + 1, v.z + 1), vec3f(v.x + 1, v.y + 1, v.z - 1), vec3f(v.x + 1, v.y - 1, v.z + 1) );

		set_color( tgl::rgb::blue );
		draw_3d_triangle( vec3f(v.x - 1, v.y - 1, v.z - 1), vec3f(v.x - 1, v.y + 1, v.z - 1), vec3f(v.x - 1, v.y - 1, v.z + 1) );
		draw_3d_triangle( vec3f(v.x - 1, v.y + 1, v.z + 1), vec3f(v.x - 1, v.y - 1, v.z + 1), vec3f(v.x - 1, v.y + 1, v.z - 1) );

		//set_color( tgl::rgb::green );
		set_texture( true );
		set_texture_uv( trig2f( vec2f(0, 0), vec2f(0, 8), vec2f(8, 0) ) );
		draw_3d_triangle( vec3f(v.x - 1, v.y + 1, v.z - 1), vec3f(v.x + 1, v.y + 1, v.z - 1), vec3f(v.x - 1, v.y + 1, v.z + 1) );

		set_texture_uv( trig2f( vec2f(8, 8), vec2f(8, 0), vec2f(0, 8) ) );
		draw_3d_triangle( vec3f(v.x + 1, v.y + 1, v.z + 1), vec3f(v.x - 1, v.y + 1, v.z + 1), vec3f(v.x + 1, v.y + 1, v.z - 1) );
		set_texture( false );

		set_color( tgl::rgb::green );
		draw_3d_triangle( vec3f(v.x - 1, v.y - 1, v.z - 1), vec3f(v.x - 1, v.y - 1, v.z + 1), vec3f(v.x + 1, v.y - 1, v.z - 1) );
		draw_3d_triangle( vec3f(v.x + 1, v.y - 1, v.z + 1), vec3f(v.x + 1, v.y - 1, v.z - 1), vec3f(v.x - 1, v.y - 1, v.z + 1) );

	}

#endif

tgl::Renderer::Renderer( uint x, uint y, pixel_placer placer, byte _channels ):
		width( x ),
		xo( x >> 1 ),
		height( y ),
		yo( y >> 1 ),
		wen( x - 1 ),
		hen( y - 1 ),
		channels( _channels ) {

	this->placer = placer;
	this->col = _channels == 3 ? tgl::rgb::black : nullptr;

#ifdef TOYGL_ENABLE_DEPTH
	this->depth_buffer = (float*) malloc( x * y * sizeof( float ) );
	this->depth = 0;
	this->depth_flag = false;
	this->depth_enable = true;
#endif

#ifdef TOYGL_ENABLE_3D
	set_rotation( vec3f(0, 0, 0) );
	set_camera( vec3f(0, 0, 0) );
	set_distance( 10 );
	set_fov( 80 );
	set_clip( 1, 255 );
	set_scale( 1 );
#endif

#ifdef TOYGL_ENABLE_TEXTURES
	set_texture_src( nullptr, 0, 0 );
	set_texture( false );
#endif

}

tgl::Renderer::~Renderer() {
#ifdef TOYGL_ENABLE_DEPTH
	free( this->depth_buffer );
#endif
}

tgl::vec3f::vec3f( float x, float y, float z ) {
	this->x = x;
	this->y = y;
	this->z = z;
}

tgl::vec3i::vec3i( int x, int y, int z ) {
	this->x = x;
	this->y = y;
	this->z = z;
}

tgl::vec2f::vec2f() {
	this->x = 0;
	this->y = 0;
}

tgl::vec2f::vec2f( float x, float y ) {
	this->x = x;
	this->y = y;
}

tgl::vec2i::vec2i( int x, int y ) {
	this->x = x;
	this->y = y;
}

tgl::vec2f tgl::vec2i::f() {
	return tgl::vec2f( x, y );
}

tgl::vec3f tgl::mat3x3f::operator*( const vec3f& b ) {
	return vec3f(

		b.x * m00 + b.y * m01 + b.z * m02,
		b.x * m10 + b.y * m11 + b.z * m12,
		b.x * m20 + b.y * m21 + b.z * m22

	);
}

tgl::mat3x3f tgl::mat3x3f::operator*( const mat3x3f& b ) {
	return mat3x3f {

		b.m00 * m00 + b.m10 * m01 + b.m20 * m02,
		b.m01 * m00 + b.m11 * m01 + b.m21 * m02,
		b.m02 * m00 + b.m12 * m01 + b.m22 * m02,

		b.m00 * m10 + b.m10 * m11 + b.m20 * m12,
		b.m01 * m10 + b.m11 * m11 + b.m21 * m12,
		b.m02 * m10 + b.m12 * m11 + b.m22 * m12,

		b.m00 * m20 + b.m10 * m12 + b.m20 * m22,
		b.m01 * m20 + b.m11 * m12 + b.m21 * m22,
		b.m02 * m20 + b.m12 * m12 + b.m22 * m22,

	};
}

tgl::trig2f::trig2f( vec2f v1, vec2f v2, vec2f v3 ) {
	this->v1 = v1;
	this->v2 = v2;
	this->v3 = v3;
}

bool tgl::math::invert_matrix( const mat3x3f& m, mat3x3f& im ) {

	float det =
		  m.m00 * ( m.m11 * m.m22 - m.m12 * m.m21 )
		- m.m01 * ( m.m10 * m.m22 - m.m12 * m.m20 )
		+ m.m02 * ( m.m10 * m.m21 - m.m11 * m.m20 );

	if( det == 0 ) return false;

	det = 1 / det;

	im.m00 = det *   ( m.m11 * m.m22 - m.m12 * m.m21 );
	im.m01 = det * - ( m.m01 * m.m22 - m.m02 * m.m21 );
	im.m02 = det *   ( m.m01 * m.m12 - m.m02 * m.m11 );
	im.m10 = det * - ( m.m10 * m.m22 - m.m12 * m.m20 );
	im.m11 = det *   ( m.m00 * m.m22 - m.m02 * m.m20 );
	im.m12 = det * - ( m.m00 * m.m12 - m.m02 * m.m10 );
	im.m20 = det *   ( m.m10 * m.m21 - m.m11 * m.m20 );
	im.m21 = det * - ( m.m00 * m.m21 - m.m01 * m.m20 );
	im.m22 = det *   ( m.m00 * m.m11 - m.m01 * m.m10 );

	return true;
}

int tgl::math::max_clamp( int value, int max ) {
	const int v = value < 0 ? 0 : value;
	return v > max ? max : v;
}

float tgl::math::cross( float x1, float y1, float x2, float y2, float x3, float y3 ) {
	return (x1 - x3) * (y2 - y3) - (x2 - x3) * (y1 - y3);
}

int tgl::math::max( int a, int b, int c ) {
	return std::max( std::max( a, b ), c );
}

int tgl::math::max( int a, int b, int c, int d ) {
	return std::max( std::max( a, b ), std::max( c, d ) );
}

int tgl::math::min( int a, int b, int c ) {
	return std::min( std::min( a, b ), c );
}

int tgl::math::min( int a, int b, int c, int d ) {
	return std::min( std::min( a, b ), std::min( c, d ) );
}

int tgl::math::signum( int value ) {
	return (0 < value) - (value < 0);
}

unsigned long tgl::math::rgb( byte r, byte g, byte b ) {
	return b + (g << 8) + (r << 16);
}

#endif
