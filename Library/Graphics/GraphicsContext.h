// Copyright (C) 2013 Felix Ungman
//
// This file is part of the openwar platform (GPL v3 or later), see LICENSE.txt

#ifndef GraphicsContext_H
#define GraphicsContext_H

#include "GraphicsOpenGL.h"
#include "ShaderProgram.h"
#include <map>


class GraphicsContext
{
	float _pixeldensity;
	std::map<std::string, ShaderProgramBase*> _shaders;

public:
	GraphicsContext(float pixelDensity);
	~GraphicsContext();

	float GetPixelDensity() const { return _pixeldensity; }

	template <class _ShaderProgram> _ShaderProgram* GetShaderProgram()
	{
		const char* name = typeid(_ShaderProgram).name();
		_ShaderProgram* result = dynamic_cast<_ShaderProgram*>(_shaders[name]);
		if (result == nullptr)
		{
			result = new _ShaderProgram(this);
			_shaders[name] = result;
		}
		return result;
	}

private:
	GraphicsContext(const GraphicsContext&) { }
	GraphicsContext& operator=(const GraphicsContext&) { return *this; }
};


#endif
