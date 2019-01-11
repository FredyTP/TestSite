#include "geoVarHandler.h"
#include <resFinder.hpp>



geoVarHandler::geoVarHandler(geoVarManager * m, const std::string tag, bg::text::Font *f)
{
	_manager = m;
	_varIdx = _manager->getIdx(tag);
	_variable = _manager->getStaticGeoVars()[_varIdx];
	_isBool = _variable.isBool;
	
	_font = f;
	
}

geoVarHandler::geoVarHandler(geoVarManager * m, const int idx, bg::text::Font *f)
{
	_manager = m;
	_varIdx = idx;
	_variable = _manager->getStaticGeoVars()[_varIdx];
	_isBool = _variable.isBool;
	_font = f;
}

void geoVarHandler::init()
{
	if (node()->drawable() != nullptr)
	{
		node()->removeComponent(node()->drawable());
	}
	//CREATE DRAWABLE NODE AND CMP
	_boxNode = new bg::scene::Node(context());
	_boxDrw = getWindowFrame(context(), bg::math::Vector2(1.0f, 0.8f), bg::math::Vector3(1.2f, 1.0f, 0.1f));
	_boxDrw->material(0)->setDiffuse(bg::math::Color::Black());
	_boxDrw->material(0)->setLightEmission(0.15f);
	_boxDrw->material(1)->setTexture(bg::db::loadTexture(context(), ts::resources.texture.wood));
	_boxNode->addComponent(_boxDrw);
	node()->addChild(_boxNode);

	//CREATE TEXT NODE AND CMP
	_screenNode = new bg::scene::Node(context());
	bg::scene::Transform *trx = new bg::scene::Transform;
	trx->matrix().translate(0.0f, -0.05f, 0.001f);
	_screenNode->addComponent(trx);
	_screenCmp = new ts::var::TextBox(_font);
	node()->addChild(_screenNode);
	_screenNode->addComponent(_screenCmp);
	_screenCmp->init();
	_screenCmp->setDisconnectionText(_variable);

	//CREATE GRAPHIC NODE AND CMP
	_graphicNode = new bg::scene::Node(context());
	_graphicNode->addComponent(new bg::scene::Transform);
	_graphicNode->transform()->matrix().scale(0.3f,0.3f,1.0f).translate(0.6f/0.3f+1.0f, 0.0f, 0.05f);
	_graphicCmp = new geoVarGraph();
	_graphicNode->addComponent(_graphicCmp);
	node()->addChild(_graphicNode);

	//init values
	_lastValue = 0;
	_disconnection = false;
	
}

void geoVarHandler::frame(float delta)
{
	if (_manager->isConnected())
	{
		_lastValue = _variable.value;
		_variable = _manager->get(_varIdx);
		if (_variable.value != _lastValue)
		{
			_screenCmp->updateText(_variable, _graphicCmp); // UPDATES THE TEXT
			_graphicCmp->update(_variable); // THE GRAPHIC CMP HANDLES IF HE IS ON OR NOT//
		}
		_disconnection = true;
	}
	else
	{
		
		if (_disconnection)
		{
			_disconnection = false;
			_screenCmp->setDisconnectionText(_variable);
		}
	}

}

void geoVarHandler::updateScreen()
{
	_screenCmp->updateText(_variable, _graphicCmp);
}

geoVarHandler::~geoVarHandler()
{
}
