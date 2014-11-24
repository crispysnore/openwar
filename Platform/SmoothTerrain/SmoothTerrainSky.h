// Copyright (C) 2013 Felix Ungman
//
// This file is part of the openwar platform (GPL v3 or later), see LICENSE.txt

#ifndef SmoothTerrainSky_H
#define SmoothTerrainSky_H

#include "TerrainSky/TerrainSky.h"
#include "Graphics/ShaderProgram.h"
#include "Graphics/CommonShaders.h"


class SmoothTerrainSky : public TerrainSky
{
	GraphicsContext* _gc;
	Texture* _textureBackgroundLinen;

public:
	SmoothTerrainSky(GraphicsContext* gc);
	virtual ~SmoothTerrainSky();

	void Render(const glm::mat4& transform, bounds2f bounds, float cameraDirectionZ, bool flip);
	void RenderBackgroundLinen(const glm::mat4& transform, bounds2f bounds);
};



#endif
