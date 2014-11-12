#ifndef PatchGlyph_H
#define PatchGlyph_H

#include "WidgetShape.h"


class PatchGlyph : public WidgetGlyph
{
public:
	bounds2f outer_xy;
	bounds2f inner_xy;
	bounds2f outer_uv;
	bounds2f inner_uv;
	glm::vec4 _colorize;
	float _alpha;

	PatchGlyph();
	PatchGlyph(std::shared_ptr<TextureImage> tile, bounds2f bounds, glm::vec2 inset);

	void Reset();
	void Reset(std::shared_ptr<TextureImage> tile, bounds2f bounds, glm::vec2 inset);

private:
	virtual void AppendVertices(std::vector<Vertex_2f_2f_4f_1f>& vertices);

	void AppendRectangle(std::vector<Vertex_2f_2f_4f_1f>& vertices, bounds2f xy, bounds2f uv);

private:
	PatchGlyph(const PatchGlyph&) { }
	PatchGlyph& operator=(const PatchGlyph&) { return *this; }
};


#endif