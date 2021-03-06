#ifndef InputWidget_H
#define InputWidget_H

#include "StringWidget.h"
#include "Surface/Animation.h"
#include "Surface/ClickHotspot.h"

class InputEditor;


class InputWidget : public StringWidget, AnimationHost
{
	friend class InputEditor;
	bounds2f _bounds;
	bool _editing{};
	std::function<void()> _enterAction;
	ClickHotspot _inputHotspot;
	InputEditor* _inputEditor{};

public:
	InputWidget(WidgetOwner* widgetOwner);
	virtual ~InputWidget();

	void SetEnterAction(std::function<void()> value);

	bool IsEditing() const;
	void SetEditing(bool value);

	size_t GetMaxLength() const { return 140; } // hard coded to 140 for now (chat message limit)

	virtual void SetPosition(glm::vec2 value);
	virtual void SetWidth(float value);

	virtual const char* GetString() const;
	virtual void SetString(const char* value);

	virtual bounds2f GetBounds() const;
	virtual void SetBounds(const bounds2f& value);

	virtual void OnTouchBegin(Touch* touch);

	virtual void RenderVertices(std::vector<Vertex_2f_2f_4f_1f>& vertices);

private:
	void RefreshContent();

	virtual void Animate(double secondsSinceLastLoop);

	void ShowInputEditor();
	void HideInputEditor();

	//void RenderSolid(const glm::mat4& transform, bounds2f bounds, glm::vec4 color) const;
};


#endif
