/*
 *	bg2 engine license
 *	Copyright (c) 2016 Fernando Serrano <ferserc1@gmail.com>
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 *	of the Software, and to permit persons to whom the Software is furnished to do
 *	so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in all
 *	copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 *	INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 *	PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *	OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 *	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef _bg2e_gui_toolbar_button_hpp_
#define _bg2e_gui_toolbar_button_hpp_

#include <bg/gui/button.hpp>

#include <bg/base/texture.hpp>

namespace bg {
namespace gui {

class BG2E_EXPORT ToolbarButton : public bg::gui::Button {
public:
	ToolbarButton(bg::base::Context *, Skin *);
	
	inline void setIcon(bg::base::Texture * tex) { _icon = tex; }
	inline bg::base::Texture * icon() { return _icon.getPtr(); }
	inline const bg::base::Texture * icon() const { return _icon.getPtr(); }

protected:
	virtual ~ToolbarButton();

	virtual void updateContent(bg::base::Pipeline *);
	virtual Skin::WidgetType getSkinWidgetType() { return Skin::kToolbarButton; }
	
	bg::ptr<bg::base::Texture> _icon;
};

class BG2E_EXPORT ToolbarItem : public bg::gui::View {
public:
	ToolbarItem(bg::base::Context *, Skin *);
	
	virtual void build();
	
	inline void setIcon(bg::base::Texture * tex) { _btn->setIcon(tex); }
	inline bg::base::Texture * icon() { return _btn->icon(); }
	inline const bg::base::Texture * icon() const { return _btn->icon(); }
	
	inline void setText(const std::string & tex) { _label->setText(tex); }
	inline const std::string & text() const { return _label->text(); }
	
	inline void setAction(std::function<void()> a) { _btn->setAction(a); }
	
protected:
	virtual ~ToolbarItem();
	
	virtual void updateContent(bg::base::Pipeline *);
	virtual Skin::WidgetType getSkinWidgetType() { return Skin::kToolbarItem; }
	
	ToolbarButton * _btn = nullptr;
	Label * _label = nullptr;
};

}
}

#endif
