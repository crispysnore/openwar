// Copyright (C) 2013 Felix Ungman
//
// This file is part of the openwar platform (GPL v3 or later), see LICENSE.txt

#ifndef BATTLEVIEW_H
#define BATTLEVIEW_H

#include "BattleMap/BattleMap.h"
#include "BattleModel/BattleSimulator.h"
#include "Shapes/VertexShape.h"
#include "Shapes/BillboardTextureShape.h"
#include "TerrainView/TerrainView.h"
#include "TiledTerrain/TiledTerrainRenderer.h"
#include "SmoothTerrain/SmoothTerrainRenderer.h"
#include "Graphics/CommonShaders.h"
#include "Graphics/RenderLoopObserver.h"

class BattleCommander;
class CasualtyMarker;
class UnitMovementMarker;
class RangeMarker;
class ShootingCounter;
class UnitTrackingMarker;
class UnitCounter;
class SmoothTerrainWater;
class SmoothTerrainSky;
class SmokeCounter;
class BattleHotspot;
class Touch;


class BattleView : public TerrainView, BattleObserver, BattleMapObserver, RenderLoopObserver
{
	GraphicsContext* _gc{};
	BattleSimulator* _simulator{};
	BattleCommander* _commander{};

	glm::vec3 _lightNormal{};

	BillboardTextureSheet* _billboardTexture{};
	BillboardModel* _billboardModel{};
	BillboardTextureShape* _textureBillboardShape{};
	BillboardTextureShape* _textureBillboardShape1{};
	BillboardTextureShape* _textureBillboardShape2{};

	CasualtyMarker* _casualtyMarker{};
	std::vector<UnitMovementMarker*> _movementMarkers{};
	std::vector<UnitTrackingMarker*> _trackingMarkers{};

	VertexShape_3f* _plainLineVertices{};
	VertexShape_3f_4f* _gradientLineVertices{};
	VertexShape_3f_4f* _gradientTriangleVertices{};
	VertexShape_3f_4f* _gradientTriangleStripVertices{};
	VertexShape_3f_4f_1f* _colorBillboardVertices{};
	VertexShape_3f_2f* _textureTriangleVertices{};
	VertexShape_2f_2f* _textureTriangleVertices2{};

	Texture* _textureUnitMarkers{};
	Texture* _textureTouchMarker{};

	std::vector<SmokeCounter*> _smokeMarkers{};
	std::vector<ShootingCounter*> _shootingCounters{};
	std::vector<UnitCounter*> _unitMarkers{};

	SmoothTerrainRenderer* _smoothTerrainSurface{};
	SmoothTerrainWater* _smoothTerrainWater{};
	SmoothTerrainSky* _smoothTerrainSky{};
	TiledTerrainRenderer* _tiledTerrainRenderer{};
	std::shared_ptr<BattleHotspot> _battleHotspot{};

public:
	BattleView(Surface* surface);
	~BattleView();

	BattleSimulator* GetSimulator() const { return _simulator; }
	void SetSimulator(BattleSimulator* simulator);

	BattleCommander* GetCommander() const { return _commander; }
	void SetCommander(BattleCommander* value) { _commander = value; }

	SmoothTerrainRenderer* GetSmoothTerrainRenderer() const { return _smoothTerrainSurface; }
	SmoothTerrainWater* GetSmoothTerrainWater() const { return _smoothTerrainWater; }

private: // BattleObserver
	void OnAddUnit(Unit* unit) override;
	void OnRemoveUnit(Unit* unit) override;
	void OnCommand(Unit* unit, float timer) override;
	void OnShooting(const Shooting& shooting, float timer) override;
	void OnRelease(const Shooting& shooting) override;
	void OnCasualty(const Fighter& fighter) override;
	void OnRouting(Unit* unit) override;

private: // BattleMapObserver
	void OnBattleMapChanged(const BattleMap* battleMap) override;

public:
	void AddCasualty(Unit* unit, glm::vec2 position);

	const std::vector<UnitCounter*>& GetUnitCounters() const { return _unitMarkers; }

	UnitMovementMarker* AddMovementMarker(Unit* unit);
	UnitMovementMarker* GetMovementMarker(Unit* unit);
	UnitMovementMarker* GetNearestMovementMarker(glm::vec2 position, BattleCommander* commander);

	UnitTrackingMarker* AddTrackingMarker(Unit* unit);
	UnitTrackingMarker* GetTrackingMarker(Unit* unit);
	void RemoveTrackingMarker(UnitTrackingMarker* trackingMarker);

	void Initialize();
	void InitializeTerrainTrees();
	void UpdateTerrainTrees(bounds2f bounds);
	void InitializeCameraPosition();

public: // View
	void Render() override;
	void OnTouchBegin(Touch* touch) override;

private: // RenderLoopObserver
	void OnRenderLoop(double secondsSinceLastUpdate) override;

public:
	bounds2f GetBillboardBounds(glm::vec3 position, float height);

	bounds2f GetUnitCurrentIconViewportBounds(Unit* unit);
	bounds2f GetUnitFutureIconViewportBounds(Unit* unit);

	bounds2f GetUnitFacingMarkerBounds(glm::vec2 center, float direction);
	bounds2f GetUnitCurrentFacingMarkerBounds(Unit* unit);
	bounds2f GetUnitFutureFacingMarkerBounds(Unit* unit);

	bounds1f GetUnitIconSizeLimit() const;

	/***/

	void AddShootingAndSmokeCounters(const Shooting& shooting);

	void AddShootingCounter(const Shooting& shooting);
	ShootingCounter* AddShootingCounter(MissileType missileType);
	void RemoveAllShootingMarkers();

	void AddSmokeMarker(const Shooting& shooting);
	SmokeCounter* AddSmokeMarker(MissileType missileType);
	void RemoveAllSmokeMarkers();

	UnitCounter* GetNearestUnitCounter(glm::vec2 position, int filterTeam, BattleCommander* filterCommander, bool filterDeployed);

	void UpdateSoundPlayer();
	void UpdateDeploymentZones();
};


#endif
