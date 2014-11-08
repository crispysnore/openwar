// Copyright (C) 2013 Felix Ungman
//
// This file is part of the openwar platform (GPL v3 or later), see LICENSE.txt

#ifndef ButtonHotspot_H
#define ButtonHotspot_H

#include <functional>
#include "Hotspot.h"
#include "ButtonGesture.h"

class Content;


class ButtonHotspot : public Hotspot
{
	ButtonGesture _gesture;
	Content* _content;
	std::function<void()> _action;
	bool _highlight;
	bool _immediate;

public:
	ButtonHotspot(Content* content);
	virtual ~ButtonHotspot();

	virtual Gesture* GetGesture() const;

	virtual bool IsInside(glm::vec2 position) const;

	virtual std::function<void()> GetClickAction() const;
	virtual void SetClickAction(std::function<void()> value);

	virtual bool IsImmediateClick() const;
	virtual void SetImmediateClick(bool value);

	virtual bool IsHighlight() const;
	virtual void SetHighlight(bool value);
};


#endif
