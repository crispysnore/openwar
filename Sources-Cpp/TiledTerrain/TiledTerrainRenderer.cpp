// Copyright (C) 2013 Felix Ungman
//
// This file is part of the openwar platform (GPL v3 or later), see LICENSE.txt

#include "BattleMap/HeightMap.h"
#include "Graphics/ShaderProgram.h"
#include "Shapes/VertexShape.h"
#include "TiledTerrainRenderer.h"
#include "Graphics/CommonShaders.h"
#include "Graphics/GraphicsContext.h"
#include "Graphics/Image.h"
#include "Graphics/TextureAtlas.h"


TiledTerrainRenderer::TiledTerrainRenderer(GraphicsContext* gc, const TiledGroundMap* tiledGroundMap) :
_gc(gc),
_tiledGroundMap(tiledGroundMap)
{
	glm::ivec2 size = _tiledGroundMap->GetSize();
	for (int x = 0; x < size.x; ++x)
		for (int y = 0; y < size.y; ++y)
		{
			TiledGroundMap::Tile* tile = _tiledGroundMap->GetTile(x, y);

			if (_textures.find(tile->texture) == _textures.end())
			{
				Texture* t = nullptr;//new TextureAtlas(gc, Image(gc, resource(tile->texture.c_str())));
				_textures[tile->texture] = t;
			}
		}
}


TiledTerrainRenderer::~TiledTerrainRenderer()
{
}




void TiledTerrainRenderer::Render(Viewport* viewport, const glm::mat4& transform, const glm::vec3& lightNormal)
{
	//HeightMap* heightMap = _tiledGroundMap->GetHeightMap();
	bounds2f bounds = _tiledGroundMap->GetBounds();
	glm::ivec2 size = _tiledGroundMap->GetSize();

	VertexShape_3f_2f _vertices;
	_vertices._mode = GL_TRIANGLES;

	glm::vec2 delta = bounds.size() / glm::vec2(size);

	for (int x = 0; x < size.x; ++x)
		for (int y = 0; y < size.y; ++y)
		{
			TiledGroundMap::Tile* tile = _tiledGroundMap->GetTile(x, y);

			glm::vec2 p0 = bounds.min + delta * glm::vec2(x, y);
			glm::vec2 p1 = p0 + delta;

			float h00 = _tiledGroundMap->CalculateHeight(x, y); //heightMap->InterpolateHeight(glm::vec2(p0.x, p0.y));
			float h01 = _tiledGroundMap->CalculateHeight(x, y + 1); //heightMap->InterpolateHeight(glm::vec2(p0.x, p1.y));
			float h10 = _tiledGroundMap->CalculateHeight(x + 1, y); //heightMap->InterpolateHeight(glm::vec2(p1.x, p0.y));
			float h11 = _tiledGroundMap->CalculateHeight(x + 1, y + 1); //heightMap->InterpolateHeight(glm::vec2(p1.x, p1.y));

			glm::vec2 t00 = glm::vec2(0, 0);
			glm::vec2 t01 = glm::vec2(0, 1);
			glm::vec2 t10 = glm::vec2(1, 0);
			glm::vec2 t11 = glm::vec2(1, 1);

			for (int i = 0; i < 3; ++i)
			{
				glm::vec2 tmp = t00;
				t00 = t01;
				t01 = t11;
				t11 = t10;
				t10 = tmp;
			}

			if (tile->mirror)
			{
				t00.x = 1 - t00.x;
				t01.x = 1 - t01.x;
				t10.x = 1 - t10.x;
				t11.x = 1 - t11.x;
			}

			for (int i = 0; i < tile->rotate; ++i)
			{
				glm::vec2 tmp = t00;
				t00 = t01;
				t01 = t11;
				t11 = t10;
				t10 = tmp;
			}

			_vertices.Clear();
			_vertices.AddVertex(Vertex_3f_2f(glm::vec3(p0.x, p0.y, h00), t01));
			_vertices.AddVertex(Vertex_3f_2f(glm::vec3(p1.x, p0.y, h10), t11));
			_vertices.AddVertex(Vertex_3f_2f(glm::vec3(p1.x, p1.y, h11), t10));
			_vertices.AddVertex(Vertex_3f_2f(glm::vec3(p1.x, p1.y, h11), t10));
			_vertices.AddVertex(Vertex_3f_2f(glm::vec3(p0.x, p1.y, h01), t00));
			_vertices.AddVertex(Vertex_3f_2f(glm::vec3(p0.x, p0.y, h00), t01));

			RenderCall<TextureShader_3f>(_gc)
				.SetVertices(&_vertices, "position", "texcoord")
				.SetUniform("transform", transform)
				.SetTexture("texture", _textures[tile->texture])
				.Render(*viewport);
		}
}
