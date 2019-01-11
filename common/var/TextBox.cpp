#include "TextBox.hpp"



namespace ts
{
namespace var
{


TextBox::TextBox(bg::text::Font * f) : bg::scene::TextComponent(f)
{

}


std::string TextBox::createText(const geoVar &variable)
{
	std::string texto;
	if (variable.isBool)
	{
		texto += "Variable: ";
		texto += variable.name;
		texto += "\nValue: ";
		if (variable.value == std::numeric_limits<float>::min())
		{
			texto += "Loading...";
		}
		else if (variable.value)
		{
			texto += "ON\n";
		}
		else
		{
			texto += "OFF\n";
		}
	}
	else
	{
		texto += "Variable: ";
		texto += variable.name;
		texto += "\nValue: ";
		if (variable.value == std::numeric_limits<float>::min())
		{
			texto += "Loading...";
		}
		else
		{
			std::ostringstream out;
			out.precision(3);
			out << std::fixed << variable.value;
			texto += out.str();
			texto += " ";
			texto += variable.unit;
		}

	}


	return texto;
}

std::string TextBox::createText(const geoVar & variable, geoVarGraph * graph)
{
	std::string texto;
	texto = this->createText(variable);
	if (graph != nullptr && !graph->isHidden() && graph->size() > 1)
	{
		std::ostringstream out;
		out.precision(4);


		texto += "\n";
		texto += "GraphicRange:\n";
		out << std::fixed << "[" << graph->min().y() << "," << graph->max().y() << "]";
		texto += out.str();

	}
	return texto;

}

void TextBox::updateText(const geoVar &variable)
{
	this->setText(this->createText(variable));
	//_textRect->setText(this->createText(variable));
}
void TextBox::updateText(const geoVar & variable, geoVarGraph * graph)
{
	this->setText(this->createText(variable, graph));
	//_textRect->setText(this->createText(variable, graph));
}
void TextBox::setDisconnectionText(const geoVar & variable)
{
	std::string texto;
	texto += "Variable: ";
	texto += variable.name;
	texto += "\n";
	texto += "Loading...";
	this->setText(texto);
}
TextBox::~TextBox()
{
}




}
}