#pragma once
#include <bg\bg2e.hpp>
#include <vector>
#include <limits>
class GraphicGenerator
{
public:
	GraphicGenerator();
	GraphicGenerator(bg::base::Context* ctx);
	~GraphicGenerator();
	void addPoint(bg::math::Vector2 p);
	void build();
	void setGraphSize(float w, float h) { _graphSize.set(w, h); }
	void normalize();
	void scale(float xs, float ys);
	void setKeepProportion(bool p);
	void setMaxPoints(int n,bool scrollGraph=true);
	int maxPoints() { return _maxPoints; }
	void clearAll();
	void deleteMark();
	void clearGraph();
	void deleteBG();
	bool scroll() { return _scroll; }
	size_t nPoints() { return _Originalpoints.size(); }
	bg::math::Vector2 max() const { return bg::math::Vector2(xLimits.x(),yLimits.x()); }
	bg::math::Vector2 min() const { return bg::math::Vector2(xLimits.y(), yLimits.y()); }

	void createMark(const bg::math::Color &c = bg::math::Color(0.0f, 0.0f, 0.0f, 1.0f));
	void createBG(const bg::math::Color &c = bg::math::Color(1.0f, 1.0f, 1.0f, 1.0f));
	//void generateAxis(float x0, float y0);
	inline bg::base::Material* material() { return _material.getPtr(); }
	inline bg::scene::Drawable* drawable() { return _drw.getPtr(); }
	inline bg::scene::Drawable* getDrawable() { if (isBuild)return reinterpret_cast<bg::scene::Drawable*>(_drw->clone()); return nullptr; }
private:
	std::vector<bg::math::Vector2> _Originalpoints;
	std::vector<bg::math::Vector2> _points;
	std::vector<bg::math::Vector2> _outPoints;
	bg::math::Vector2 _graphSize;
	bg::ptr<bg::base::Context> _context;
	bg::ptr<bg::base::PolyList> _plist;
	bg::ptr<bg::base::Material> _material;
	bg::ptr<bg::scene::Drawable> _drw;
	//DATA_INFO
	bg::math::Vector2 xLimits;
	bg::math::Vector2 yLimits;
	bg::math::Vector2 offSet;
	bg::math::Vector2 ratio;
	//OUTPUT_INFO
	bg::math::Vector2 minLimits;
	bg::math::Vector2 maxLimits;
	bool isBuild;
	bool _keepP;
	bool _scroll;
	float originalP;
	//
	int _maxPoints;
};

