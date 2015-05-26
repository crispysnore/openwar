// Copyright (C) 2014 Felix Ungman
//
// This file is part of the openwar platform (GPL v3 or later), see LICENSE.txt

#ifndef ScrollerHotspot_H
#define ScrollerHotspot_H

#include "Hotspot.h"
#include "ScrollerGesture.h"
#include "Algebra/bounds.h"
#include <functional>
#include <glm/glm.hpp>

class ScrollerViewport;


class ScrollerHotspot : public Hotspot
{
	ScrollerGesture _gesture;
	ScrollerViewport* _viewport;

public:
	ScrollerHotspot(ScrollerViewport* viewport);

	Gesture* GetGesture() override;

	ScrollerViewport* GetViewport() const;
};


#endif