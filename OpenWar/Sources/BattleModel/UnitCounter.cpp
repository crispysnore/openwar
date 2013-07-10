// Copyright (C) 2013 Felix Ungman
//
// This file is part of the openwar platform (GPL v3 or later), see LICENSE.txt

#include "UnitCounter.h"
#include "BattleModel.h"
#include "PlainRenderer.h"
#include "TextureBillboardRenderer.h"
#include "TextureRenderer.h"
#include "BattleView.h"


UnitCounter::UnitCounter(BattleModel* battleModel, Unit* unit) :
_battleModel(battleModel),
_unit(unit),
_routingTimer(0)
{
}


UnitCounter::~UnitCounter()
{
}


bool UnitCounter::Animate(float seconds)
{
	if (_battleModel->GetUnit(_unit->unitId) == 0)
		return false;

	float routingBlinkTime = _unit->state.GetRoutingBlinkTime();

	if (!_unit->state.IsRouting() && routingBlinkTime != 0)
	{
		_routingTimer -= seconds;
		if (_routingTimer < 0)
			_routingTimer = routingBlinkTime;
	}

	return true;
}


void UnitCounter::AppendUnitMarker(TextureBillboardRenderer* renderer1, TextureBillboardRenderer* renderer2, bool flip)
{
	bool routingIndicator = false;
	float routingBlinkTime = _unit->state.GetRoutingBlinkTime();

	if (_unit->state.IsRouting())
	{
		routingIndicator = true;
	}
	else if (routingBlinkTime != 0 && 0 <= _routingTimer && _routingTimer < 0.2f)
	{
		routingIndicator = true;
	}

	int state = 0;
	if (routingIndicator)
		state = 2;
	else if (_unit->player != _battleModel->bluePlayer)
		state = 1;

	glm::vec3 position = _battleModel->terrainSurface->GetPosition(_unit->state.center, 0);
	glm::vec2 texsize(0.1875, 0.1875); // 48 / 256
	glm::vec2 texcoord1 = texsize * glm::vec2(state, 0);
	glm::vec2 texcoord2 = texsize * glm::vec2((int)_unit->stats.unitWeapon, 1 + (int)_unit->stats.unitPlatform);

	if (flip)
	{
		texcoord1 += texsize;
		texcoord2 += texsize;
		texsize = -texsize;
	}

	renderer1->AddBillboard(position, 32, affine2(texcoord1, texcoord1 + texsize));
	renderer2->AddBillboard(position, 32, affine2(texcoord2, texcoord2 + texsize));
}


void UnitCounter::AppendFacingMarker(TextureTriangleRenderer* renderer, BattleView* battleView)
{
	if (_unit->state.unitMode != UnitModeStanding)
		return;

	int index = 0;
	if (_unit->command.holdFire)
	{
		index = 11;
	}
	else if (_unit->state.loadingDuration != 0)
	{
		index = 2 + (int)glm::round(9 * _unit->state.loadingTimer / _unit->state.loadingDuration);
		index = glm::min(10, index);
	}

	float txs = 0.0625f;
	float tx1 = index * txs;
	float tx2 = tx1 + txs;


	bounds2f bounds = battleView->GetUnitCurrentFacingMarkerBounds(_unit);
	glm::vec2 p = bounds.center();
	float size = bounds.height();
	float direction = index != 0 ? -glm::half_pi<float>() : (_unit->state.direction - battleView->GetCameraFacing());

	glm::vec2 d1 = size * vector2_from_angle(direction - glm::half_pi<float>() / 2.0f);
	glm::vec2 d2 = glm::vec2(d1.y, -d1.x);
	glm::vec2 d3 = glm::vec2(d2.y, -d2.x);
	glm::vec2 d4 = glm::vec2(d3.y, -d3.x);

	renderer->AddVertex(glm::vec3(p + d1, 0), glm::vec2(tx1, 1));
	renderer->AddVertex(glm::vec3(p + d2, 0), glm::vec2(tx1, 0));
	renderer->AddVertex(glm::vec3(p + d3, 0), glm::vec2(tx2, 0));

	renderer->AddVertex(glm::vec3(p + d3, 0), glm::vec2(tx2, 0));
	renderer->AddVertex(glm::vec3(p + d4, 0), glm::vec2(tx2, 1));
	renderer->AddVertex(glm::vec3(p + d1, 0), glm::vec2(tx1, 1));
}


void UnitCounter::AppendFighterWeapons(PlainLineRenderer* renderer)
{
	if (_unit->stats.weaponReach > 0)
	{
		for (Fighter* fighter = _unit->fighters, * end = fighter + _unit->fightersCount; fighter != end; ++fighter)
		{
			glm::vec2 p1 = fighter->state.position;
			glm::vec2 p2 = p1 + _unit->stats.weaponReach * vector2_from_angle(fighter->state.direction);

			renderer->AddLine(
					_battleModel->terrainSurface->GetPosition(p1, 1),
					_battleModel->terrainSurface->GetPosition(p2, 1));
		}
	}
}


void UnitCounter::AppendFighterBillboards(BillboardModel* billboardModel)
{
	for (Fighter* fighter = _unit->fighters, * end = fighter + _unit->fightersCount; fighter != end; ++fighter)
	{
		float size = 2.0;
		//float diff = angle_difference(GetCameraFacing(), fighter->state.direction);
		//float absdiff = fabsf(diff);

		int shape = 0;
		//int i = unit->player == Player2 ? 2 : 1;
		//int j = 0;
		switch (_unit->stats.unitPlatform)
		{
			case UnitPlatformCav:
			case UnitPlatformGen:
				shape = _unit->player == Player2 ? billboardModel->_billboardShapeFighterCavRed : billboardModel->_billboardShapeFighterCavBlue;
				size = 3.0;
				/*if (absdiff < 0.33 * M_PI)
					j = 5;
				else if (absdiff < 0.66 * M_PI)
					j = 6;
				else
					j = 7;*/
				break;

			case UnitPlatformSam:
				shape = _unit->player == Player2 ? billboardModel->_billboardShapeFighterSamRed : billboardModel->_billboardShapeFighterSamBlue;
				size = 2.0;
				//j = absdiff < M_PI_2 ? 2 : 1;
				break;

			case UnitPlatformAsh:
				shape = _unit->player == Player2 ? billboardModel->_billboardShapeFighterAshRed : billboardModel->_billboardShapeFighterAshBlue;
				size = 2.0;
				//j = absdiff < M_PI_2 ? 4 : 3;
				break;
		}


		const float adjust = 0.5 - 2.0 / 64.0; // place texture 2 texels below ground
		glm::vec3 p = _battleModel->terrainSurface->GetPosition(fighter->state.position, adjust * size);
		float facing = glm::degrees(fighter->state.direction);
		billboardModel->dynamicBillboards.push_back(Billboard(p, facing, size, shape));
	}
}
