#pragma once

//TestSite

//Bg2e
#include <bg/bg2e.hpp>
namespace ts
{


class AnalogicClock : public bg::scene::Component
{
public:
	AnalogicClock();
	bg::scene::Component* clone() { return new AnalogicClock; }

	void init();
	void frame(float delta);
protected:
	bg::ptr<bg::scene::Drawable> _drw;

	bg::ptr<bg::scene::Node> _hour;
	bg::ptr<bg::scene::Drawable> _hDrw;
	bg::ptr<bg::scene::Node> _minute;
	bg::ptr<bg::scene::Drawable> _mDrw;
};


class Clock : public bg::scene::Component
{
public:
	Clock();
	bg::scene::Component* clone() { return new Clock; }
	void init();
	void draw(bg::base::Pipeline *pl);
	std::string getTimeStr();

	void setShowSeconds(bool s) { _showSeconds = s; }
	bool showSeconds() { return _showSeconds; }

protected:
	bg::ptr<bg::scene::Drawable> _drw;
	bg::ptr<bg::text::StringRenderer> _stringRenderer;
	bg::ptr<bg::text::Font> _font;
	std::vector<bg::math::Color> _definedColors;
	bool _showSeconds;

};

}