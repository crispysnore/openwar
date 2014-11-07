#ifndef TerrainHotspot_H
#define TerrainHotspot_H

#include "Hotspot.h"
#include "TerrainGesture.h"


class TerrainHotspot : public Hotspot
{
	TerrainGesture _gesture;
	TerrainView* _terrainView;

public:
	TerrainHotspot(TerrainView* terrainView);
	virtual ~TerrainHotspot();

	virtual Gesture* GetGesture() const;

	TerrainView* GetTerrainView() const;
};


#endif
