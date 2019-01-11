#pragma once

//TestSite
#include <extra\GraphicGenerator.h>
#include <var\TestSiteVariables.hpp>
#include <extra\PosAnimation.h>

//Bg2e
#include <bg\bg2e.hpp>


class geoVarGraph : public bg::scene::Component
{
public:
	geoVarGraph();
	void frame(float delta);
	void init();
	void update(const geoVar &var);
	void hide();
	void show();
	void clear();
	void pause();
	void start();
	inline const bg::math::Vector2 & max() const { return _gGen.max(); }
	inline const bg::math::Vector2 & min() const { return _gGen.min(); }
	size_t size() { return _gGen.nPoints(); }
	bool isRunning() { return _running; }
	bool isHidden() { return _isHidden; }
	bg::scene::Component* clone() { return new geoVarGraph; }
protected:
	~geoVarGraph();
	float timer;
	GraphicGenerator _gGen;
	bool _isHidden;
	bool _running;
	bg::math::Vector2 _max;
	bg::math::Vector2 _min;
};

