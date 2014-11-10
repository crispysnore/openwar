// Copyright (C) 2013 Felix Ungman
//
// This file is part of the openwar platform (GPL v3 or later), see LICENSE.txt

#ifndef TERRAINVIEW_H
#define TERRAINVIEW_H

#include "BattleModel/GroundMap.h"
#include "BattleModel/HeightMap.h"
#include "Surface/Content.h"
#include "Algebra/geometry.h"
#include "Shapes/VertexShape.h"

class EditorModel;
class EditorHotspot;
class TerrainHotspot;
class TerrainViewport;
class Touch;
class WidgetViewport;


class TerrainView
{
	TerrainViewport* _terrainViewport;
	WidgetViewport* _widgetViewport;

	bool _mouseHintVisible;
	glm::vec2 _mouseHintPosition;

	std::shared_ptr<EditorHotspot> _editorHotspot;
	std::shared_ptr<TerrainHotspot> _terrainHotspot;

protected:
	HeightMap* _heightMap;

public:
	TerrainView(GraphicsContext* gc);
	virtual ~TerrainView();

	glm::vec2 GetScreenTop() const;
	glm::vec2 GetScreenLeft() const;
	glm::vec2 GetScreenBottom() const;
	glm::vec2 GetScreenRight() const;

	TerrainViewport* GetTerrainViewport() const;
	WidgetViewport* GetWidgetViewport() const;

	void SetEditorHotspot(std::shared_ptr<EditorHotspot> hotspot);

	void FindTerrainHotspots(Touch* touch);

	void ShowMouseHint(glm::vec2 position);
	void HideMouseHint();
	void RenderMouseHint(VertexShape_3f* vertices);

	void SetHeightMap(HeightMap* heightMap);

	ray GetCameraRay(glm::vec2 screenPosition) const;
	glm::vec3 GetTerrainPosition2(glm::vec2 screenPosition) const;
	glm::vec3 GetTerrainPosition3(glm::vec2 screenPosition) const;

	void Move(glm::vec3 originalContentPosition, glm::vec2 currentScreenPosition);
	void Zoom(glm::vec3 originalContentPosition1, glm::vec3 originalContentPosition2, glm::vec2 currentScreenPosition1, glm::vec2 currentScreenPosition2, float orbitFactor);
	void Orbit(glm::vec2 originalContentPosition, float angle);

	void MoveCamera(glm::vec3 position);
	void ClampCameraPosition();

	glm::vec3 ScreenToContent(glm::vec2 value) const;
	glm::vec2 ContentToScreen(glm::vec3 value) const;

	glm::vec3 GetTerrainPosition(glm::vec2 p, float h = 1) { return _heightMap->GetPosition(p, h); };

};


#endif
