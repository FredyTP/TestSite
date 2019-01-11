#include "geoNodeManager.hpp"
#include <app/App.h>
#include <resFinder.hpp>


geoNodeManager::geoNodeManager()
{
	_geoManager.init();
}


void geoNodeManager::init()
{
	using namespace bg::scene;
	
	
	bg::system::Path path = bg::system::Path::AppDir();

	//bg::db::FontLoader::RegisterPlugin(new bg::db::plugin::ReadFontTrueType());

	path.addComponent(ts::resources.font.nunito_black);

	_font = bg::db::loadFont(context(), path, 30.0f);

	bg::base::Material * mat = new bg::base::Material();
	mat->setDiffuse(bg::math::Color::White());
	mat->setLightEmission(1.0f);
	mat->setProcessAsTransparent(true);
	mat->setCullFace(false);
	_font->setMaterial(mat);
	_font->setTextScale(0.40f);

	if (ts::App::Get().isVR())
	{
		node()->addComponent(new ts::vr::Pointer);
		std::cout << "POITER VR CRETED" << std::endl;
	}
	else
	{
		node()->addComponent(new gui3d::Pointer);
	}
	
	std::vector<geoVar> sGV = _geoManager.getStaticGeoVars();
	
	for (auto boxSearcher : node()->children())
	{
		for (int i = 0; i < _geoManager.size(); i++)
		{
			if (boxSearcher->name().compare(sGV[i].name) == 0)
			{
				boxSearcher->addComponent(new geoVarHandler(&_geoManager, sGV[i].name, _font.getPtr()));
				boxSearcher->addComponent(new gui3d::Widget(1.0f, 0.7f, sGV[i].name));
				boxSearcher->component<gui3d::Widget>()->init();
				_varsHandlers.push_back(boxSearcher->component<geoVarHandler>());
			}
		}
	}
	
}

void geoNodeManager::frame(float delta)
{
	for (auto w : ts::App::Get().guiManager()->widgets())
	{
		if (w->getButtonEvent() == gui3d::mouseButtonEvent::kButtonDown)
		{
			if (_selected.valid())
			{
				_selected->boxDrawable()->material(0)->setDiffuse(bg::math::Color::Black());
			}
			_selected = w->node()->component<geoVarHandler>();
			if (_selected.valid() && _selected->boxDrawable()!=nullptr)
			{
				_selected->boxDrawable()->material(0)->setDiffuse(bg::math::Color(1.0f, 0.3f, 0.05f, 1.0f));
				SceneConsole *c = ts::App::Get().scene()->console();
				c->clear();
				c->addText("Elemento Selecionado: ");
				c->addText(_selected->variable().name);
				c->endl();
			}

		}
	}
}

void geoNodeManager::mouseDown(const bg::base::MouseEvent &evt)
{
	if (_selected.valid() && _selected->boxDrawable() != nullptr)
	{
		_selected->boxDrawable()->material(0)->setDiffuse(bg::math::Color::Black());
	}
	_selected = nullptr;
}

void geoNodeManager::keyDown(const bg::base::KeyboardEvent &evt)
{
	if (evt.keyboard().key() == evt.keyboard().kKeyH)
	{
		if (_selected.valid() && !_selected->isBool())
		{
			_selected->graphic()->hide();
			_selected->updateScreen();
		}
	}
	else if (evt.keyboard().key() == evt.keyboard().kKeyS)
	{
		if (_selected.valid() && !_selected->isBool())
		{
			_selected->graphic()->show();
			_selected->updateScreen();
		}
	}
	else if (evt.keyboard().key() == evt.keyboard().kKeyC)
	{
		if (_selected.valid() && !_selected->isBool())
		{
			_selected->graphic()->clear();
			_selected->updateScreen();
		}
	}
	else if (evt.keyboard().key() == evt.keyboard().kKeyP)
	{
		if (_selected.valid() && !_selected->isBool())
			_selected->graphic()->pause();
	}
	else if (evt.keyboard().key() == evt.keyboard().kKeyB)
	{
		if (_selected.valid() && !_selected->isBool())
			_selected->graphic()->start();
	}

	else if (evt.keyboard().key() == evt.keyboard().kKeyRight)
	{
		if (_selected.valid() && !_selected->isBool())
			_selected->graphicNode()->transform()->matrix().translate(0.1f, 0.0f, 0.0f);
	}
	else if (evt.keyboard().key() == evt.keyboard().kKeyLeft)
	{
		if (_selected.valid() && !_selected->isBool())
			_selected->graphicNode()->transform()->matrix().translate(-0.1f, 0.0f, 0.0f);
	}
	else if (evt.keyboard().key() == evt.keyboard().kKeyUp)
	{
		if (_selected.valid() && !_selected->isBool())
			_selected->graphicNode()->transform()->matrix().translate(0.0f, 0.1f, 0.0f);
	}
	else if (evt.keyboard().key() == evt.keyboard().kKeyDown)
	{
		if (_selected.valid() && !_selected->isBool())
			_selected->graphicNode()->transform()->matrix().translate(0.0f, -0.1f, 0.0f);
	}
	else if (evt.keyboard().key() == evt.keyboard().kKeyAdd)
	{
		if (_selected.valid() && !_selected->isBool())
			_selected->graphicNode()->transform()->matrix().translate(0.0f, 0.0f, 0.1f);
	}
	else if (evt.keyboard().key() == evt.keyboard().kKeySub)
	{
		if (_selected.valid() && !_selected->isBool())
			_selected->graphicNode()->transform()->matrix().translate(0.0f, 0.0f, -0.1f);
	}
}


geoNodeManager::~geoNodeManager()
{
	_geoManager.exit();
}
