// Copyright (C) 2013 Felix Ungman
//
// This file is part of the openwar platform (GPL v3 or later), see LICENSE.txt

#ifndef FrameBuffer_H
#define FrameBuffer_H

#ifdef OPENWAR_USE_XCODE_FRAMEWORKS
#if TARGET_OS_IPHONE
#include <OpenGLES/ES2/gl.h>
#else
#include <OpenGL/gl.h>
#endif
#else
#if OPENWAR_USE_GLEW
#include <GL/glew.h>
#endif
#ifdef OPENWAR_USE_GLES2
#include <GLES2/gl2.h>
#else
#include <GL/gl.h>
#endif
#endif

#ifndef CHECK_ERROR_GL
extern void CHECK_ERROR_GL();
#endif

struct RenderBuffer;
struct texture;


struct FrameBuffer
{
	GLuint id;

	FrameBuffer();
	~FrameBuffer();

	void AttachColor(RenderBuffer* value);
	void AttachColor(texture* value);

	void AttachDepth(RenderBuffer* value);
	void AttachDepth(texture* value);

	void AttachStencil(RenderBuffer* value);

private:
	FrameBuffer(const FrameBuffer&) {}
	FrameBuffer& operator = (const FrameBuffer&) {return *this;}
};


class bind_framebuffer
{
	GLint _old;

public:
	bind_framebuffer(const FrameBuffer& fb)
	{
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &_old);
		glBindFramebuffer(GL_FRAMEBUFFER, fb.id);
		CHECK_ERROR_GL();
	}

	~bind_framebuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _old);
	}
};


#endif