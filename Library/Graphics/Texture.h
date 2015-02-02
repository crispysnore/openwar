// Copyright (C) 2013 Felix Ungman
//
// This file is part of the openwar platform (GPL v3 or later), see LICENSE.txt

#ifndef Texture_H
#define Texture_H

#include "GraphicsContext.h"

class Image;


class Texture
{
	friend class RenderCallTexture;
	friend class FrameBuffer;

protected:
	GLuint _id;

public:
	explicit Texture(GraphicsContext* gc);
	virtual ~Texture();

	Texture(Texture&&);
	Texture& operator=(Texture&&);

	virtual void UpdateTexture() = 0;

	void LoadTextureFromImage(const Image& image);
	void LoadTextureFromData(int width, int height, const void* data);

	void GenerateMipmap();
	void ResizeDepth(int width, int height);
};


#endif
