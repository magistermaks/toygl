
/*
 * This is not a part of TGL, its a simple
 * window API wrapper used to define the canvas for TGL to draw on.
 * But it isn't necessary, by any means, for its operation.
 */

#pragma once

#ifndef CANVAS_SCALE
#	define CANVAS_SCALE 1
#endif

#include <cstdlib>
#include <cstring>
#include "wxgl.h"
#include <sys/time.h>
#include <unistd.h>

// used for a time function to calculate fps
#if defined(_WIN32) || defined(_WIN64)
#	include <windows.h>
#endif

typedef unsigned long long long_time_t;

// quad
const GLfloat data[] = {
	-1.0f, -1.0f,  0.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,  0.0f,
	-1.0f, -1.0f,  0.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,  0.0f,
	-1.0f,  1.0f,  0.0f,  0.0f,
};

GLuint shader(GLenum type, const char* source) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	return shader;
}

void vertexAttribute(int index, int count, int stride, long offset) {
	glVertexAttribPointer(index, count, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*) (offset * sizeof(float)));
	glEnableVertexAttribArray(index);
}

void window_open( const char* name, int width, int height );
void window_close();
void window_update();
void window_draw( unsigned int x, unsigned int y, unsigned char* color );
void window_clear( unsigned int rgb );
void window_scan();

bool mouse_down_flag = false;
bool should_close = false;
int mouse_pos_x = 0;
int mouse_pos_y = 0;
int width, height;
int tw, th;

unsigned char* canvas;

void window_event_key( unsigned int keycode, bool pressed );

void window_open(const char* name, int w, int h, bool vsync) {
	winxHint(WINX_HINT_VSYNC, vsync ? WINX_VSYNC_ENABLED : WINX_VSYNC_DISABLED);

	winxOpen(w, h, name);
	width = w;
	height = h;

	winxSetButtonEventHandle([] (int status, int button) {
		mouse_down_flag = (status == WINX_PRESSED);
	});

	winxSetMouseHandle([] (int x, int y) {
		mouse_pos_x = x;
		mouse_pos_y = y;
	});

	winxSetKeybordEventHandle([] (int status, int keycode) {
		window_event_key(keycode, status == WINX_PRESSED);
	});

	winxSetCloseEventHandle([] () {
		should_close = true;
	});

	// init OpenGL to create texture canvas
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// make the canvas
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	tw = w / CANVAS_SCALE;
	th = h / CANVAS_SCALE;
	canvas = (unsigned char*) malloc(tw * th * 3);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tw, th, 0, GL_RGB, GL_UNSIGNED_BYTE, canvas);

	GLuint vert = shader( GL_VERTEX_SHADER,
		"#version 330 core\n"
		"layout (location = 0) in vec2 pos;\n"
		"layout (location = 1) in vec2 tuv;\n"
		"out vec2 uv;\n"
		"void main(){gl_Position = vec4(pos.x, pos.y, -1.0, 1.0); uv = tuv;\n}"
	);

	GLuint frag = shader(GL_FRAGMENT_SHADER,
		"#version 330 core\n"
		"uniform sampler2D canvas;\n"
		"in vec2 uv;\n"
		"out vec4 frag;\n"
		"void main(){frag = texture(canvas, uv);}"
	);

	GLuint program = glCreateProgram();
	glAttachShader(program, vert);
	glAttachShader(program, frag);
	glLinkProgram(program);

	glDeleteShader(vert);
	glDeleteShader(frag);

	glUseProgram(program);

	// create and bind VAO
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// create and fill VBO
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

	// configure VAO
	vertexAttribute(0, 2, 4, 0); // vec2: pos
	vertexAttribute(1, 2, 4, 2); // vec2: uv

}

void window_close() {
	winxClose();
	free(canvas);
}

void window_update() {
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tw, th, 0, GL_RGB, GL_UNSIGNED_BYTE, canvas);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	winxSwapBuffers();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void window_draw(unsigned int x, unsigned int y, unsigned char* c) {
	memcpy(canvas + (x + y * tw) * 3, c, 3);
}

void window_clear( unsigned int rgb ) {
	if (rgb <= 0xFF || rgb == 0xFFFFFF /* white */) {
		memset(canvas, 255, tw * th * 3);
	} else {
		for(int x = 0; x < tw; x ++) {
			for(int y = 0; y < th; y ++) {
				memcpy(canvas + (x + y * tw) * 3, &rgb, 3);
			}
		}
	}
}

void window_scan() {
	winxPollEvents();
}

long_time_t get_time() {
#if defined(_WIN32) || defined(_WIN64)
	SYSTEMTIME tp;
	GetSystemTime(&tp);
	return tp.wSecond * 1000ull * 1000ull + tp.wMilliseconds;
#else
	struct timeval tp;
	gettimeofday(&tp, NULL);
	return tp.tv_sec * 1000ull * 1000ull + tp.tv_usec;
#endif
}

bool framerate(long_time_t& fps) {
	const long_time_t now = get_time();

	static int count = 0;
	static long_time_t start = now;

	fps = ++count;

	if( now - start >= 1000000ull ) {
		start = now;
		count = 0;
		return true;
	}

	return false;
}

