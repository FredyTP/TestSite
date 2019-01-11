#include "GraphicGenerator.h"




GraphicGenerator::GraphicGenerator()
{
	isBuild = false;
	_keepP = false;
	originalP = 1.0f;
}

GraphicGenerator::GraphicGenerator(bg::base::Context * ctx)
{
	_context = ctx;
	_drw = new bg::scene::Drawable();
	_plist = new bg::base::PolyList(_context.getPtr());
	_plist->setDrawMode(bg::base::PolyList::kLines);
	_material = new bg::base::Material();
	_material->setDiffuse(bg::math::Color::Red());
	_material->setLightEmission(1.0f);
	isBuild = false;
	_keepP = false;
	originalP = 1.0f;
	_maxPoints = 100000;
}

GraphicGenerator::~GraphicGenerator()
{
}

void GraphicGenerator::addPoint(bg::math::Vector2  p)
{
	_Originalpoints.push_back(p);
	if (_Originalpoints.size() > _maxPoints)
	{
		if (_scroll)
		{
			_Originalpoints.erase(_Originalpoints.begin());
		}
		else
		{
			_Originalpoints.erase(_Originalpoints.begin()+1+rand()%(_maxPoints-3));
		}
	}
}

void GraphicGenerator::build()
{
	this->normalize();
	if (_keepP)
	{
		if (originalP < 1)
		{
			this->scale(1, originalP);
		}
		else
		{
			this->scale(1 / originalP, 1);
		}
	}
	else
	{
		;
	}
	if (isBuild)
	{
		_drw->removePolyList(_plist.getPtr());
		_plist->destroy();
	}
	for (int i = 0; i < _outPoints.size(); i++)
	{
		_plist->addVertex(_outPoints[i]);
		_plist->addNormal(bg::math::Vector3(0.0f, 1.0f, 0.0f));
		_plist->addTexCoord0(bg::math::Vector2(0.0f, 0.0f));
		if (i>1)
			_plist->addIndex(i - 1);
		_plist->addIndex(i);
	}
	_plist->build();
	_drw->addPolyList(_plist.getPtr(), _material.getPtr());
	isBuild = true;

}

void GraphicGenerator::normalize()
{
	xLimits.x(std::numeric_limits<float>::min());
	xLimits.y(std::numeric_limits<float>::max());
	yLimits.x(xLimits[0]);
	yLimits.y(xLimits[1]);
	_points = _Originalpoints;
	for (int i = 0; i < _points.size(); i++)
	{
		//MAXX
		if (_points[i].x() > xLimits[0])
		{
			xLimits.x(_points[i].x());
		}
		//MINX
		if (_points[i].x() < xLimits[1])
		{
			xLimits.y(_points[i].x());
		}
		//MAXY
		if (_points[i].y() > yLimits[0])
		{
			yLimits.x(_points[i].y());
		}
		//MINY
		if (_points[i].y() < yLimits[1])
		{
			yLimits.y(_points[i].y());
		}
	}
	offSet.set((xLimits[0] + xLimits[1]) / 2, (yLimits[0] + yLimits[1]) / 2);
	ratio.set(2.0f / (xLimits[0] - xLimits[1]), 2.0f / (yLimits[0] - yLimits[1]));
	originalP = ratio.x() / ratio.y();

	for (int i = 0; i < _points.size(); i++)
	{
		_points[i].sub(offSet);
		_points[i] = _points[i] * ratio;
	}
	_outPoints = _points;
}

void GraphicGenerator::scale(float xs, float ys)
{
	bg::math::Vector2 vecS(xs, ys);
	for (int i = 0; i < _outPoints.size(); i++)
	{
		_outPoints[i] = _outPoints[i] * vecS;
	}
}

void GraphicGenerator::setKeepProportion(bool p)
{
	_keepP = p;
}

void GraphicGenerator::setMaxPoints(int n, bool scroll)
{
	_maxPoints = n;
	_scroll = scroll;
}

void GraphicGenerator::clearAll()
{
	this->deleteMark();
	this->deleteBG();
	this->clearGraph();
}

void GraphicGenerator::deleteMark()
{
	_drw->removePolyList(_drw->find("Mark")->polyList.getPtr());
}

void GraphicGenerator::clearGraph()
{
	_Originalpoints.clear();
	_points.clear();
	_outPoints.clear();
	_drw->removePolyList(_plist.getPtr());
	_plist->destroy();
}

void GraphicGenerator::deleteBG()
{
	_drw->removePolyList(_drw->find("BG")->polyList.getPtr());
}


void GraphicGenerator::createMark(const bg::math::Color & c)
{
	bg::ptr<bg::base::PolyList> _mL = new bg::base::PolyList(_context.getPtr());
	_mL->setName("Mark");
	bg::ptr<bg::base::Material> _mM = new bg::base::Material();
	bg::math::Matrix4 trx = bg::math::Matrix4::Identity();
	trx.translate(0.0f, 0.0f, 0.001f);
	_mM->setDiffuse(c);
	_mM->setLightEmission(1.0f);

	_mL->setDrawMode(bg::base::PolyList::kLines);

	_mL->addVertex(bg::math::Vector2(1,1));
	_mL->addNormal(bg::math::Vector3(0.0f, 1.0f, 0.0f));
	_mL->addTexCoord0(bg::math::Vector2(0.0f, 0.0f));

	_mL->addVertex(bg::math::Vector2(1, -1));
	_mL->addNormal(bg::math::Vector3(0.0f, 1.0f, 0.0f));
	_mL->addTexCoord0(bg::math::Vector2(0.0f, 0.0f));

	_mL->addVertex(bg::math::Vector2(-1, -1));
	_mL->addNormal(bg::math::Vector3(0.0f, 1.0f, 0.0f));
	_mL->addTexCoord0(bg::math::Vector2(0.0f, 0.0f));

	_mL->addVertex(bg::math::Vector2(-1, 1));
	_mL->addNormal(bg::math::Vector3(0.0f, 1.0f, 0.0f));
	_mL->addTexCoord0(bg::math::Vector2(0.0f, 0.0f));

	_mL->addIndex(0);
	_mL->addIndex(1);

	_mL->addIndex(1);
	_mL->addIndex(2);

	_mL->addIndex(2);
	_mL->addIndex(3);

	_mL->addIndex(3);
	_mL->addIndex(0);

	_mL->build();

	_drw->addPolyList(_mL.getPtr(), _mM.getPtr(),trx);


}

void GraphicGenerator::createBG(const bg::math::Color & c)
{
	using namespace bg::math;
	bg::ptr<bg::base::PolyList> bgn = new bg::base::PolyList(_context.getPtr());
	bgn->setName("BG");

	bg::ptr<bg::base::Material> mat = new bg::base::Material();
	mat->setDiffuse(c);
	mat->setLightEmission(1.0f);
	mat->setCullFace(false);

	bg::math::Matrix4 trx = bg::math::Matrix4::Identity();
	trx.translate(0.0f, 0.0f, -0.001f);

	bgn->setDrawMode(bg::base::PolyList::kTriangles);

	bgn->addVertex(Vector3(1, 1, 0));
	bgn->addVertex(Vector3(-1, 1, 0));
	bgn->addVertex(Vector3(-1, -1, 0));
	bgn->addVertex(Vector3(1, -1, 0));

	bgn->addNormal(Vector3(0.0f, 0.0f, 1.0f));
	bgn->addNormal(Vector3(0.0f, 0.0f, 1.0f));
	bgn->addNormal(Vector3(0.0f, 0.0f, 1.0f));
	bgn->addNormal(Vector3(0.0f, 0.0f, 1.0f));

	bgn->addTexCoord0(Vector2(1.0f, 1.0f));
	bgn->addTexCoord0(Vector2(0.0f, 1.0f));
	bgn->addTexCoord0(Vector2(0.0f, 0.0f));
	bgn->addTexCoord0(Vector2(1.0f, 0.0f));

	bgn->addTriangle(0, 1, 3);
	bgn->addTriangle(1, 2, 3);

	bgn->build();

	_drw->addPolyList(bgn.getPtr(), mat.getPtr(), trx);
}
