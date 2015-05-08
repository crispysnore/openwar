// Copyright (C) 2013 Felix Ungman
//
// This file is part of the openwar platform (GPL v3 or later), see LICENSE.txt

#ifndef SmoothHeightMap_H
#define SmoothHeightMap_H

#include <glm/glm.hpp>
#include "Algebra/geometry.h"

class GroundMap;


class HeightMap
{
	bounds2f _bounds;
	int _cacheStride{256};
	int _cacheMaxIndex{254};
	float* _cacheHeights{};
	glm::vec3* _cacheNormals{};

public:
	HeightMap(bounds2f bounds);
	~HeightMap();

	bounds2f GetBounds() const { return _bounds; }

	void Update(const GroundMap* groundMap);

	int GetMaxIndex() const { return _cacheMaxIndex; }
	float GetHeight(int x, int y) const;
	glm::vec3 GetNormal(int x, int y) const;

	float InterpolateHeight(glm::vec2 position) const;
	glm::vec3 GetPosition(glm::vec2 p, float h) { return glm::vec3(p, InterpolateHeight(p) + h); }

	std::pair<bool, float> Intersect(ray r);

private:
	std::pair<bool, float> InternalIntersect(ray r);

	void UpdateHeights(const GroundMap* groundMap);
	void UpdateNormals();
};


#endif
