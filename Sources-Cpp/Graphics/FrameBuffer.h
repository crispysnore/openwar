// Copyright (C) 2013 Felix Ungman
//
// This file is part of the openwar platform (GPL v3 or later), see LICENSE.txt

#ifndef FrameBuffer_H
#define FrameBuffer_H

#include "GraphicsContext.h"


struct RenderBuffer;
struct Texture;


class FrameBuffer
{
	friend class Image;
	friend class RenderCallBase;

	GLuint _id;
	GLenum _status{};
	bool _hasColor{};
	bool _hasDepth{};

public:
	FrameBuffer();
	~FrameBuffer();

	FrameBuffer(FrameBuffer&&);
	FrameBuffer& operator=(FrameBuffer&&);

	bool IsComplete() const;
	const char* GetStatus() const;
	bool HasColor() const;
	bool HasDepth() const;

	void AttachColor(RenderBuffer* value);
	void AttachColor(Texture* value);

	void AttachDepth(RenderBuffer* value);
	void AttachDepth(Texture* value);
};


#endif
