// Copyright (C) 2013 Felix Ungman
//
// This file is part of the openwar platform (GPL v3 or later), see LICENSE.txt

#ifndef UnitCounter_H
#define UnitCounter_H

#include "../../BattleModel/BattleSimulator.h"
#include "../../BattleModel/SamuraiModule.h"
#include "../../Library/Algebra/bounds.h"

class BattleSimulator;
class BattleView;
class BillboardModel;
class PlainLineRenderer;
class TextureBillboardRenderer;
class TextureTriangleRenderer;
class Unit;


class UnitCounter
{
public:
	BattleView* _battleView;
	Unit* _unit;
	float _routingTimer;
	int _weaponIndex;
	SamuraiWeapon _samuraiWeapon;
	SamuraiPlatform _samuraiPlatform;

public:
	UnitCounter(BattleView* battleView, Unit* unit);
	~UnitCounter();

	Unit* GetUnit() const { return _unit; }

	bool Animate(float seconds);

	void AppendUnitMarker(TextureBillboardRenderer* renderer1, TextureBillboardRenderer* renderer2, bool flip);
	void AppendFacingMarker(TextureTriangleRenderer* renderer, BattleView* battleView);

	void AppendFighterWeapons(PlainLineRenderer* renderer);
	void AppendFighterBillboards(BillboardModel* billboardModel);
};


#endif
