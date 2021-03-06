// Copyright (C) 2013 Felix Ungman
//
// This file is part of the openwar platform (GPL v3 or later), see LICENSE.txt

#ifndef ClickHotspot_H
#define ClickHotspot_H

#include "ClickGesture.h"
#include "Hotspot.h"
#include <functional>


class ClickHotspot : public Hotspot
{
	ClickGesture _gesture{this};
	std::function<glm::vec2(glm::vec2)> _distance{};
	std::function<glm::vec2(Touch*)> _tolerance{};
	std::function<void()> _action{};
	bool _highlight{};
	bool _immediate{};

public:
	Gesture* GetGesture() override;

	virtual void SetDistance(std::function<glm::vec2(glm::vec2)> distance);
	virtual void SetTolerance(std::function<glm::vec2(Touch*)> tolerance);

	static glm::vec2 GetDefaultTolerance(Touch* touch, glm::vec2 size);
	virtual float GetDistance(glm::vec2 position) const;
	virtual bool IsTouchInside(Touch* touch) const;

	virtual std::function<void()> GetClickAction() const;
	virtual void SetClickAction(std::function<void()> value);

	virtual bool IsImmediateClick() const;
	virtual void SetImmediateClick(bool value);

	virtual bool IsHighlight() const;
	virtual void SetHighlight(bool value);
};


#endif
