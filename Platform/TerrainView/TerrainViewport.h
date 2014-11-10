#ifndef TerrainViewport_H
#define TerrainViewport_H

#include "Viewport.h"
#include "WidgetViewport.h"


class TerrainViewport : public Viewport
{
	bounds2f _terrainBounds;
	glm::vec3 _cameraPosition;
	float _cameraTilt;
	float _cameraFacing;
	bool _flip;

public:
	TerrainViewport(GraphicsContext* gc);
	virtual ~TerrainViewport();

	virtual glm::mat4 GetTransform() const;

	glm::mat4x4 GetProjectionMatrix() const;
	glm::mat4x4 GetViewMatrix() const;

	bounds2f GetTerrainBounds() const;
	void SetTerrainBounds(const bounds2f& value);

	float GetContentRadius() const;

	glm::vec3 GetCameraPosition() const;
	void SetCameraPosition(const glm::vec3& value);

	float GetCameraFacing() const;
	void SetCameraFacing(float value);
	glm::vec3 GetCameraDirection() const;

	float GetCameraTilt() const;
	void SetCameraTilt(float value);
	glm::vec3 GetCameraUpVector() const;

	bool GetFlip() const;
	void SetFlip(bool value);

};



#endif
