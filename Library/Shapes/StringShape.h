#ifndef STRINGSHAPE_H
#define STRINGSHAPE_H

#include <map>
#include <glm/gtc/matrix_transform.hpp>


#include "Algebra/bounds.h"
#include "ShaderProgram.h"
#include "TextureAtlas.h"
#include "TextureFont.h"

class GraphicsContext;
class StringShape;


class StringShader : public ShaderProgram3<glm::vec2, glm::vec2, float>
{
	friend class GraphicsContext;
	/*
		attribute vec2 position;
		attribute vec2 texcoord;
		attribute float alpha;
		uniform mat4 transform;
		uniform sampler2D texture;
		uniform vec4 color;
	 */
	StringShader(GraphicsContext* gc);
};


struct StringFont
{
	TextureAtlas _textureAtlas;
	TextureFont* _textureFont;

public:
	StringFont(GraphicsContext* gc, const char* name, float size);
	StringFont(GraphicsContext* gc, bool bold, float size);
	~StringFont();

private:
	StringFont(const StringFont&) : _textureAtlas(nullptr) { }
	StringFont& operator=(const StringFont&) { return *this; }
};


class StringGlyph
{
	friend class StringShape;

	StringShape* _shape;
	std::string _string;
	glm::mat4x4 _transform;
	float _alpha;
	float _delta;

public:
	StringGlyph();
	StringGlyph(const char* string, glm::vec2 translate, float alpha = 1, float delta = 0);
	StringGlyph(const char* string, glm::mat4x4 transform, float alpha = 1, float delta = 0);
	~StringGlyph();

	const char* GetString() const { return _string.c_str(); }
	void SetString(const char* value) { _string = value; }

	const glm::mat4x4 GetTransform() const { return _transform; }
	void SetTransform(glm::mat4x4 value) { _transform = value; }
	void SetTranslate(glm::vec2 value) { _transform = glm::translate(glm::mat4(), glm::vec3(value, 0)); }

	const float GetAlpha() const { return _alpha; }
	void SetAlpha(float value) { _alpha = value; }

	const float GetDelta() const { return _delta; }
	void SetDelta(float value) { _delta = value; }

private:
	StringGlyph(const StringGlyph&) { }
	StringGlyph& operator=(const StringGlyph&) { return *this; }
};


class StringShape
{
	class StringVertexBuffer : public VertexBuffer<Vertex_2f_2f_1f>
	{
		StringShape* _shape;
	public:
		StringVertexBuffer(StringShape* shape);
		virtual void Update();
	};

	StringVertexBuffer _vertices;
	std::vector<StringGlyph*> _glyphs;

public:
	StringFont* _font;

	explicit StringShape(StringFont* font);
	~StringShape();

	VertexBuffer<Vertex_2f_2f_1f>* GetVertices();

	void ClearGlyphs();
	void AddGlyph(StringGlyph* glyph);
	void RemoveGlyph(StringGlyph* glyph);

private:
	void UpdateVertexBuffer();
	void AppendStringGlyph(std::vector<Vertex_2f_2f_1f>& vertices, StringGlyph* glyph);

	StringShape(const StringShape&) : _vertices(nullptr) { }
	StringShape& operator=(const StringShape&) { return *this; }
};


#endif
