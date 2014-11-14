#ifndef ButtonWidget_H
#define ButtonWidget_H

#include "Widget.h"
#include "ImageWidget.h"
#include "StringWidget.h"

class ButtonHotspot;


class ButtonWidget : public WidgetGroup
{
public:
	std::shared_ptr<ButtonHotspot> _hotspot;
	bounds2f _bounds;

private:
	BorderInset _inset;
	std::shared_ptr<TextureImage> _backgroundNormal;
	std::shared_ptr<TextureImage> _backgroundHighlight;
	std::shared_ptr<TextureImage> _backgroundDisabled;
	ImageWidget _borderImage;
	ImageWidget _iconImage;
	StringWidget _titleString;
	bool _disabled;

public:
	ButtonWidget(WidgetOwner* widgetOwner);

	void SetBackgroundNormal(std::shared_ptr<TextureImage> value);
	void SetBackgroundHighlight(std::shared_ptr<TextureImage> value);
	void SetBackgroundDisabled(std::shared_ptr<TextureImage> value);

	bounds2f GetBounds() const;
	void SetBounds(const bounds2f& value);

	BorderInset GetInset() const;
	void SetInset(const BorderInset& value);

	const char* GetTitle() const;
	void SetTitle(const char* value);

	std::shared_ptr<TextureImage> GetIcon() const;
	void SetIcon(std::shared_ptr<TextureImage> value);

	bool IsDisabled() const;
	void SetDisabled(bool value);

	virtual void OnTouchEnter(Touch* touch);
	virtual void OnTouchBegin(Touch* touch);

	virtual void AppendVertices(std::vector<Vertex_2f_2f_4f_1f>& vertices);
};


#endif
