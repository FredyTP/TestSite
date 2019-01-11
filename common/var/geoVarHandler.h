#pragma once

//TestSite
#include <var\geoVarManager.h>
#include <var\TestSiteVariables.hpp>
#include <var\TextBox.hpp>
#include <var\geoVarGraph.h>
#include <extra\Primitives.h>
#include <GeneralConstants.h>

//Bg2e
#include <bg\bg2e.hpp>


//BARELY CLEANED


class geoVarHandler : public bg::scene::Component
{
public:
	geoVarHandler(geoVarManager *m, const std::string tag, bg::text::Font *f);
	geoVarHandler(geoVarManager *m, const int idx, bg::text::Font *f);
	bg::scene::Component* clone() { return new geoVarHandler(_manager,_varIdx,_font); }
	void init();
	void frame(float delta);
	bool isBool() const { return _isBool; }
	void updateScreen();

	ts::var::TextBox* screen() { return _screenCmp; }
	geoVarGraph* graphic() { return _graphicCmp; }
	bg::scene::Drawable* boxDrawable() { return _boxDrw; }
	bg::scene::Node* screenNode() { return _screenNode; }
	bg::scene::Node* graphicNode() { return _graphicNode; }
	bg::scene::Node* boxNode() { return _boxNode; }
	const geoVar& variable() { return _variable; }
protected:
	~geoVarHandler();
	geoVarManager * _manager; //-> GEOVARMANAGER WE PULL THE DATA FROM HERE//^^
	geoVar _variable; //-> Where actual data remains
	float _lastValue; //-> Stores de last value of the variable;
	int _varIdx; //->INDEX OF THE VALIABLE TO BE FILLED
	
	bool _isBool; 
	bool _disconnection;

	bg::text::Font* _font;

	//NODES
	bg::scene::Node* _screenNode;
	bg::scene::Node* _graphicNode;
	bg::scene::Node* _boxNode;
	ts::var::TextBox* _screenCmp;
	geoVarGraph* _graphicCmp;

	bg::scene::Drawable* _boxDrw;
};

