#include "InputWidget.h"
#include "InputEditor.h"
#include "InputHotspot.h"
#include "ScrollerViewport.h"
#include "Surface.h"
#include "Touch.h"



InputWidget::InputWidget(WidgetOwner* widgetOwner) : StringWidget(widgetOwner),
	_editing(false),
	_inputEditor(nullptr)
{
}


InputWidget::~InputWidget()
{
	delete _inputEditor;
}


void InputWidget::SetEnterAction(std::function<void()> value)
{
	_enterAction = value;
	SetEditing(false);
}


bool InputWidget::IsEditing() const
{
	return _editing;
}


void InputWidget::SetEditing(bool value)
{
	_editing = value;
}


void InputWidget::SetPosition(glm::vec2 value)
{
	bounds2f bounds = _bounds;
	bounds += value - bounds.min;
	SetBounds(bounds);
}


void InputWidget::SetWidth(float value)
{
	bounds2f bounds = _bounds;
	bounds.max.x = bounds.min.x + value;
	SetBounds(bounds);
}


bounds2f InputWidget::GetBounds() const
{
	return _bounds;
}


void InputWidget::SetBounds(const bounds2f& value)
{
	_bounds = value;
	StringWidget::SetPosition(value.min);
	StringWidget::SetWidth(value.x().size());
}


void InputWidget::RenderVertices(std::vector<Vertex_2f_2f_4f_1f>& vertices)
{
	RenderSolid(GetViewport()->GetTransform(), GetBounds(), glm::vec4(1, 1, 1, 0.7f));

	if (_editing && _inputEditor == nullptr)
		ShowInputEditor();
	else if (!_editing && _inputEditor != nullptr)
		HideInputEditor();

	StringWidget::RenderVertices(vertices);
}


void InputWidget::OnTouchBegin(Touch* touch)
{
	if (GetBounds().contains(GetViewport()->GlobalToLocal(touch->GetCurrentPosition())))
	{
		if (_inputHotspot == nullptr)
			_inputHotspot = std::make_shared<InputHotspot>(this);
		_inputHotspot->SubscribeTouch(touch);
	}
}


void InputWidget::ShowInputEditor()
{
	if (_inputEditor == nullptr)
	{
#ifdef ENABLE_SURFACE_ADAPTER_MAC
		_inputEditor = new InputEditorMac(this);
#endif
	}
	else
	{
		_inputEditor->OnInputWidgetChanged();
	}
}


void InputWidget::HideInputEditor()
{
	delete _inputEditor;
	_inputEditor = nullptr;
}



#include "CommonShaders.h"
#include "VertexShape.h"

void InputWidget::RenderSolid(const glm::mat4& transform, bounds2f bounds, glm::vec4 color) const
{
	VertexShape_2f vertices;
	vertices._mode = GL_TRIANGLE_STRIP;
	vertices.AddVertex(Vertex_2f(bounds.mix_00()));
	vertices.AddVertex(Vertex_2f(bounds.mix_01()));
	vertices.AddVertex(Vertex_2f(bounds.mix_10()));
	vertices.AddVertex(Vertex_2f(bounds.mix_11()));

	GraphicsContext* gc = GetGraphicsContext();
	RenderCall<PlainShader_2f>(gc)
		.SetVertices(&vertices, "position")
		.SetUniform("transform", transform)
		.SetUniform("point_size", 1)
		.SetUniform("color", color)
		.Render();
}
