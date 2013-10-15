// Copyright (C) 2013 Felix Ungman
//
// This file is part of the openwar platform (GPL v3 or later), see LICENSE.txt

#ifndef RangeMarker_H
#define RangeMarker_H

#include "BattleSimulator.h"

class GradientTriangleStripRenderer;


class RangeMarker
{
public:
	BattleSimulator* _battleSimulator;
	Unit* _unit;

public:
	RangeMarker(BattleSimulator* battleSimulator, Unit* unit);

	void Render(GradientTriangleStripRenderer* renderer);

private:
	void RenderMissileRange(GradientTriangleStripRenderer* renderer, const UnitRange& unitRange);
	void RenderMissileTarget(GradientTriangleStripRenderer* renderer, glm::vec2 target);

	glm::vec3 GetPosition(glm::vec2 p) const;
};


#endif
