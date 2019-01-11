#include "geoVarGraph.h"



geoVarGraph::geoVarGraph()
{
	timer = 0.0f;
	_isHidden = true;
	_running = false;
}

void geoVarGraph::frame(float delta)
{
	if(_running)
		timer += delta;
}

void geoVarGraph::init()
{
	_gGen = GraphicGenerator(context());
	_gGen.createMark();
	_gGen.createBG();
	_gGen.setKeepProportion(false);
	_gGen.setMaxPoints(80);
}
void geoVarGraph::hide()
{
	_isHidden = true;
	if (node()->drawable())
	{
		node()->removeComponent(_gGen.drawable());
	}
}
void geoVarGraph::show()
{
	_isHidden = false;
	node()->addComponent(_gGen.drawable());
}
void geoVarGraph::clear()
{
	_gGen.clearGraph();
	timer = 0.0f;
}
void geoVarGraph::pause()
{
	_running = false;
}
void geoVarGraph::start()
{
	_running = true;
}
void geoVarGraph::update(const geoVar & var)
{
	if (_running)
	{
		_gGen.addPoint(bg::math::Vector2(timer, var.value));
		_gGen.build();
	}

}


geoVarGraph::~geoVarGraph()
{
}
