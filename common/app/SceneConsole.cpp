#include "SceneConsole.h"


SceneConsole::SceneConsole(bg::text::Font * f)
{
	_font = f;
}

void SceneConsole::init()
{
	_drwNode = new bg::scene::Node(context());
	_drwNode->addComponent(new bg::scene::Transform);
	_drwNode->transform()->matrix().translate(0.0f, 0.0f, -0.001f);
	_drw = getWindowFrame(context(), bg::math::Vector2(2.0f, 2.0f), bg::math::Vector3(2.2f, 2.2f, 0.1f));
	_drwNode->addComponent(_drw.getPtr());
	node()->addChild(_drwNode.getPtr());
	bg::text::Paragraph p;
	p.setAlignment(bg::text::Paragraph::kHAlignCenter);
	p.setAlignment(bg::text::Paragraph::kVAlignTop);
	_renderer = new bg::text::StringRenderer(_font.getPtr(), context());
	_renderer->setScale(1.0f);
	
}


void SceneConsole::draw(bg::base::Pipeline * pl)
{
	_renderer->draw(pl, _text);
}

void SceneConsole::clear()
{
	_text.clear();
}

SceneConsole::~SceneConsole()
{

}

