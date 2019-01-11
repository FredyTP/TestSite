#pragma once

//TestSite
#include <extra\Primitives.h>

//Bg2e
#include <bg/bg2e.hpp>

//std
#include <string>

class SceneConsole : public bg::scene::Component
{
public:

	SceneConsole(bg::text::Font *f);
	bg::scene::Component * clone()
	{
		return new SceneConsole(_font.getPtr());
	}
	void init();
	template<class T>
	inline void addText(T add)
	{
		std::stringstream ss;
		ss << add;
		std::string to_Add = ss.str();
		_text += to_Add;
	}
	
	void draw(bg::base::Pipeline *pl);
	void clear();
	void endl() { _text += "\n"; }

	bg::scene::Drawable * drawable() { return _drw.getPtr(); }


protected:
	~SceneConsole();
	bg::ptr<bg::text::Font> _font;
	bg::ptr<bg::text::StringRenderer> _renderer;
	bg::ptr<bg::scene::Drawable> _drw;
	bg::ptr<bg::scene::Node> _drwNode;
	std::string _text;
};

/*template<class T>
bg::ptr<SceneConsole> &operator << (bg::ptr<SceneConsole> c, T in)
{
	std::stringstream ss;
	ss << in;
	std::string to_Add = ss.str();
	c->addText(to_Add);
	return c;
}*/



