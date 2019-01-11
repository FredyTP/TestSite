#include "Clock.hpp"
#include <GeneralConstants.h>
#include <app/App.h>
#include <resFinder.hpp>

namespace ts
{



Clock::Clock()
{
	_showSeconds = false;
	_definedColors.resize(3);
	_definedColors[0].set(74.0f / 255.0f, 235.0f / 255.0f, 63.0f / 255.0f, 1.0f);
	_definedColors[1].set(190.0f / 255.0f, 19.0f / 255.0f, 19.0f / 255.0f, 1.0f);
	_definedColors[2].set(30.0f / 255.0f, 67.0f / 255.0f, 216.0f / 255.0f, 1.0f);
}

void Clock::init()
{
	
	_font = bg::db::loadFont(context(), ts::resources.font.digital, 22);

	//MATERIAL CONFIG
	bg::base::Material* mat = new bg::base::Material();
	mat->setDiffuse(_definedColors[0]);
	mat->setLightEmission(0.1f);
	mat->setCullFace(false);
	mat->setCastShadows(false);
	mat->setReceiveShadows(false);
	mat->setProcessAsTransparent(true);

	_font->setMaterial(mat);
	_stringRenderer = new bg::text::StringRenderer(_font.getPtr(), context());

	bg::text::Paragraph _p;
	_p.setAlignment(bg::text::Paragraph::kHAlignCenter);
	_p.setAlignment(bg::text::Paragraph::kVAlignCenter);

	_stringRenderer->setParagraph(_p);
}

void Clock::draw(bg::base::Pipeline * pl)
{
	_stringRenderer->draw(pl, getTimeStr());
}

std::string Clock::getTimeStr()
{
	int time;
	std::string timeStr;
	//HOURS
	time = ts::App::Get().localTime()->hour();
	if (time < 10)
	{
		timeStr += "0";
	}
	timeStr += std::to_string(time);
	timeStr += ":";
	//MINUTES
	time = ts::App::Get().localTime()->minute();
	if (time < 10)
	{
		timeStr += "0";
	}
	timeStr += std::to_string(time);
	if (_showSeconds)
	{
		timeStr += ":";
		time = ts::App::Get().localTime()->second();
		if (time < 10)
		{
			timeStr += "0";
		}
		timeStr += std::to_string(time);

	}
	return timeStr;
}

AnalogicClock::AnalogicClock()
{
}

void AnalogicClock::init()
{
	bg::scene::PrimitiveFactory factory(context());

	_drw = factory.plane(1.0f,1.0f);
	_drw->material(0)->setTexture(bg::db::loadTexture(context(), ts::resources.texture.clock));
	_drw->material(0)->setUnlit(true);
	_drw->material(0)->setCullFace(false);
	node()->addComponent(_drw.getPtr());
	bg::scene::Transform* trx = new bg::scene::Transform;
	trx->matrix().rotate(bg::math::kPiOver2, -1.0f, 0.0f, 0.0f);
	node()->addComponent(trx);

	//HOURS
	_hDrw = factory.plane(0.04f, 0.25f);
	_hDrw->transform(0).setPosition(0.0f, 0.0f, 0.12f);
	_hDrw->material(0)->setUnlit(true);
	_hDrw->material(0)->setCullFace(false);

	_hour = new bg::scene::Node(context());
	_hour->addComponent(_hDrw.getPtr());
	_hour->addComponent(new bg::scene::Transform);
	node()->addChild(_hour.getPtr());

	//MINUTES
	_mDrw = factory.plane(0.02f, 0.31f);
	_mDrw->transform(0).setPosition(0.0f, 0.0f, 0.16f);
	_mDrw->material(0)->setUnlit(true);
	_mDrw->material(0)->setCullFace(false);

	_minute = new bg::scene::Node(context());
	_minute->addComponent(_mDrw.getPtr());
	_minute->addComponent(new bg::scene::Transform);
	node()->addChild(_minute.getPtr());
}

void AnalogicClock::frame(float delta)
{
	float minutes = ts::App::Get().localTime()->hour() * 60.0f + ts::App::Get().localTime()->minute();
	float rotH = bg::math::k2Pi* ((float)minutes / 720.0f);
	float rotM = bg::math::k2Pi*((float)ts::App::Get().localTime()->minute() / 60.0f);
	_hour->transform()->matrix().resetRotation();
	_hour->transform()->matrix().rotate(rotH, 0.0f, 1.0f, 0.f);

	_minute->transform()->matrix().resetRotation();
	_minute->transform()->matrix().rotate(rotM, 0.0f, 1.0f, 0.f);
}


}
