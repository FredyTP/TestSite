/*
*	bg2 engine license
*	Copyright (c) 2016 Fernando Serrano <ferserc1@gmail.com>
*
*	Permission is hereby granted, free of charge, to any person obtaining a copy
*	of this software and associated documentation files (the "Software"), to deal
*	in the Software without restriction, including without limitation the rights
*	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
*	of the Software, and to permit persons to whom the Software is furnished to do
*	so, subject to the following conditions:
*
*	The above copyright notice and this permission notice shall be included in all
*	copies or substantial portions of the Software.
*
*	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
*	INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
*	PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
*	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
*	OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
*	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
*/


#include <bg/db/plugin/bg2_skin_plugin.hpp>
#include <bg/db/json/parser.hpp>
#include <bg/base/exception.hpp>

// Plugin dependencies
#include <bg/db/font.hpp>
#include <bg/db/plugin/true_type_font.hpp>
#include <bg/db/texture.hpp>

#include <bg/base/material.hpp>

namespace bg {
namespace db {
namespace plugin {

ReadGuiSkinBg2::ReadGuiSkinBg2()
{

}

ReadGuiSkinBg2::~ReadGuiSkinBg2()
{

}

bool ReadGuiSkinBg2::supportFileType(const std::string & extension) {
	return extension == "bg2skin";
}

bg::gui::Skin * ReadGuiSkinBg2::load(bg::base::Context * ctx, const bg::system::Path & path) {
	using namespace bg::db::json;
	bg::ptr<bg::gui::Skin> skin = new bg::gui::Skin();
	bg::system::Path skinPath = path.pathRemovingLastComponent();

	bg::ptr<Value> document = Parser::ParseFile(path);

	//bg::math::Vector3 version = Value::Vec3((*document)["version"]);

	Value * font = (*document)["font"];
	if (font) {
		std::string fontFile = Value::String((*font)["file"]);
		float size = Value::Float((*font)["size"], 12.0f);
		float scale = Value::Float((*font)["scale"], 1.0f);
		bg::math::Color textColor = Value::Color((*font)["defaultColor"], bg::math::Color::White());
		skin->setFont(loadFont(ctx, skinPath.pathAddingComponent(fontFile), size));
		skin->font()->setTextScale(scale);

		bg::base::Material * mat = new bg::base::Material();
		mat->setDiffuse(textColor);
		mat->setCullFace(false);
		skin->font()->setMaterial(mat);
	}
	else {
		throw bg::base::MalformedFileException("Missing font definition in skin file.");
	}

	parseWidgetProperties(ctx, skin.getPtr(), (*document)["View"], bg::gui::Skin::kView, skinPath);
	parseWidgetProperties(ctx, skin.getPtr(), (*document)["Button"], bg::gui::Skin::kButton, skinPath);
	parseWidgetProperties(ctx, skin.getPtr(), (*document)["ImageButton"], bg::gui::Skin::kImageButton, skinPath);
	parseWidgetProperties(ctx, skin.getPtr(), (*document)["Label"], bg::gui::Skin::kLabel, skinPath);
	parseWidgetProperties(ctx, skin.getPtr(), (*document)["TextInput"], bg::gui::Skin::kTextInput, skinPath);
	parseWidgetProperties(ctx, skin.getPtr(), (*document)["CheckBox"], bg::gui::Skin::kCheckBox, skinPath);
	parseWidgetProperties(ctx, skin.getPtr(), (*document)["RadioButton"], bg::gui::Skin::kRadioButton, skinPath);
	parseWidgetProperties(ctx, skin.getPtr(), (*document)["SpinButtonUp"], bg::gui::Skin::kSpinButtonUp, skinPath);
	parseWidgetProperties(ctx, skin.getPtr(), (*document)["SpinButtonDown"], bg::gui::Skin::kSpinButtonDown, skinPath);
	parseWidgetProperties(ctx, skin.getPtr(), (*document)["CheckBoxField"], bg::gui::Skin::kCheckBoxField, skinPath);
	parseWidgetProperties(ctx, skin.getPtr(), (*document)["RadioButtonField"], bg::gui::Skin::kRadioButtonField, skinPath);
	parseWidgetProperties(ctx, skin.getPtr(), (*document)["SpinButtonField"], bg::gui::Skin::kSpinButtonField, skinPath);
	parseWidgetProperties(ctx, skin.getPtr(), (*document)["ScrollControl"], bg::gui::Skin::kScrollControl, skinPath);
	parseWidgetProperties(ctx, skin.getPtr(), (*document)["ScrollBar"], bg::gui::Skin::kScrollBar, skinPath);
	parseWidgetProperties(ctx, skin.getPtr(), (*document)["ToolbarButton"], bg::gui::Skin::kToolbarButton, skinPath);
	parseWidgetProperties(ctx, skin.getPtr(), (*document)["ToolbarItem"], bg::gui::Skin::kToolbarItem, skinPath);

	return skin.release();
}

void ReadGuiSkinBg2::registerDependencies() {
	bg::db::FontLoader::RegisterPlugin(new bg::db::plugin::ReadFontTrueType());
}

void ReadGuiSkinBg2::parseWidgetProperties(bg::base::Context * ctx, bg::gui::Skin * skin, bg::db::json::Value * doc, bg::gui::Skin::WidgetType widgetType, const bg::system::Path & path) {
	using namespace bg::gui;
	if (!doc) throw bg::base::MalformedFileException("Missing widget type in skin definition file");
	
	parseStatusProperties(ctx, skin, (*doc)["Normal"], widgetType, kStatusNormal, path);
	parseStatusProperties(ctx, skin, (*doc)["Hover"], widgetType, kStatusHover, path);
	parseStatusProperties(ctx, skin, (*doc)["Press"], widgetType, kStatusPress, path);
	parseStatusProperties(ctx, skin, (*doc)["Disabled"], widgetType, kStatusDisabled, path);
}

void ReadGuiSkinBg2::parseStatusProperties(bg::base::Context * ctx, bg::gui::Skin * skin, bg::db::json::Value * doc, bg::gui::Skin::WidgetType widgetType, bg::gui::Status status, const bg::system::Path & path) {
	using namespace bg::gui;
	using namespace bg::math;
	using namespace bg::db::json;
	if (!doc) throw bg::base::MalformedFileException("Missing widget status definition in skin definition file.");

	Skin::Properties prop;
	prop.backgroundColor = Value::Color((*doc)["backgroundColor"]);
	prop.borderColor = Value::Color((*doc)["borderColor"]);
	prop.borderWidth = Value::Int((*doc)["borderWidth"]);
	prop.padding = Value::Int((*doc)["textPadding"]);
	prop.textColor = Value::Color((*doc)["textColor"]);
	prop.defaultSize = Value::Vec2i((*doc)["defaultSize"]);

	std::string textureFile = Value::String((*doc)["image"]);
	if (!textureFile.empty()) {
		bg::ptr<bg::base::Texture> tex = loadTexture(ctx, path.pathAddingComponent(textureFile));
		if (tex.isNull()) throw bg::base::MalformedFileException("Error loading skin definition texture resource.");
		prop.image = tex.getPtr();
	}

	skin->setWidgetProperties(widgetType, prop, status);
}

}
}
}
