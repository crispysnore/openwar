// Copyright (C) 2013 Felix Ungman
//
// This file is part of the openwar platform (GPL v3 or later), see LICENSE.txt

#ifndef UnitTrackingMarker_H
#define UnitTrackingMarker_H

#include "UnitMarker.h"
#include "BattleModel/BattleSimulator_v1_0_0.h"
#include "Shapes/VertexShape.h"

class BattleView;
class BillboardTextureShape;


class UnitTrackingMarker : public UnitMarker
{
	BattleObjects::Unit* _meleeTarget{};
	glm::vec2 _destination{};
	bool _hasDestination{};

	BattleObjects::Unit* _missileTarget{};
	glm::vec2 _orientation{};
	glm::vec2 _orientationX{};
	bool _hasOrientation{};
	bool _renderOrientation{};

	bool _running{};

public:
	std::vector<glm::vec2> _path{};

public:
	UnitTrackingMarker(BattleView* battleView, BattleObjects::Unit* unit);
	~UnitTrackingMarker();

	void SetRunning(bool value) { _running = value; }
	bool GetRunning() const { return _running; }

	void SetMeleeTarget(BattleObjects::Unit* value) { _meleeTarget = value; }
	BattleObjects::Unit* GetMeleeTarget() const { return _meleeTarget; }

	void SetMissileTarget(BattleObjects::Unit* value) { _missileTarget = value; }
	BattleObjects::Unit* GetMissileTarget() const { return _missileTarget; }

	/***/


	void SetDestination(glm::vec2* value)
	{
		if (value) _destination = *value;
		_hasDestination = value !=  nullptr;
	}

	void SetOrientation(glm::vec2* value)
	{
		if (value) _orientation = *value;
		_hasOrientation = value != nullptr;
	}

	void SetRenderOrientation(bool value) { _renderOrientation = value; }

	glm::vec2* GetOrientationX()
	{
		if (_missileTarget)
		{
			_orientationX = _missileTarget->GetCenter();
			return &_orientationX;
		}

		if (_hasOrientation)
			return &_orientation;

		return nullptr;
	}

	glm::vec2 DestinationXXX() const
	{
		return GetMeleeTarget() ? GetMeleeTarget()->GetCenter()
			: _path.size() != 0 ? *(_path.end() - 1)
			: _hasDestination ? _destination
			: GetUnit()->GetCenter();
	}


	float GetFacing() const;

	void RenderTrackingFighters(VertexShape_3f_4f_1f* vertices);
	void RenderTrackingMarker(BillboardTextureShape* renderer);
	void AppendFacingMarker(VertexShape_2f_2f* vertices, BattleView* battleView);
	void RenderTrackingShadow(BillboardTextureShape* renderer);
	void RenderTrackingPath(VertexShape_3f_4f* vertices);
	void RenderOrientation(VertexShape_3f_4f* vertices);
};


#endif
