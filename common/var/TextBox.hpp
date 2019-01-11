#pragma once

//TestSite
#include <var\TestSiteVariables.hpp>
#include <var/TextComponent.hpp>
#include <var\geoVarGraph.h>

//Bg2e
#include <bg\bg2e.hpp>

//std
#include <iomanip>

//NEEDS TO BE CLEANED JUST A BIT


namespace ts
{
namespace var
{


class TextBox : public bg::scene::TextComponent
{
public:
	TextBox(bg::text::Font * f);
	TextBox * clone() { return new TextBox(_font.getPtr()); }

	void updateText(const geoVar &variable);
	void updateText(const geoVar &variable, geoVarGraph* graph);

	void setDisconnectionText(const geoVar &variable);


protected:
	~TextBox();

	std::string createText(const geoVar &variable);
	std::string createText(const geoVar &variable, geoVarGraph* graph);
};



}
}


