#include "Primitives.h"


bg::scene::Drawable* getRegularPoly(int _sides, int size, bg::base::Context *ctx)
{
	int sides = _sides;
	if (sides < 3) sides = 3;
	using namespace bg::math;
	bg::ptr<bg::base::PolyList> _plist;
	_plist = new bg::base::PolyList(ctx);
	_plist->setDrawMode(bg::base::PolyList::kTriangles);

	//add center//
	_plist->addVertex(Vector3(0.0f, 0.0f, 0.0f));
	_plist->addNormal(Vector3(0.0f, 0.0f, 1.0f));
	_plist->addTexCoord0(Vector2(0.5f, 0.5f));
	float x;
	float y;
	for (int i = 1; i <= sides + 1; i++)
	{
		x = (float)(i) / sides * bg::math::k2Pi;
		y = trigonometry::sin(x);
		x = trigonometry::cos(x);
		_plist->addVertex(Vector3(x, y, 0.0f));
		_plist->addNormal(Vector3(0.0f, 0.0f, 1.0f));
		_plist->addTexCoord0(Vector2(x / 2 + 0.5f, y / 2 + 0.5f));
		if (i > 1)
		{
			_plist->addTriangle(0, i - 1, i);
		}

	}
	_plist->build();
	bg::ptr<bg::scene::Drawable> drw = new bg::scene::Drawable();
	bg::ptr<bg::base::Material> mat = new bg::base::Material();
	mat->setDiffuse(bg::math::Color::White());
	mat->setUnlit(true);
	drw->addPolyList(_plist.getPtr(), mat.getPtr());
	return drw.release();

}
bg::scene::Drawable * getCilinder(int _sides, float h, float r, bg::base::Context * ctx, int axis)
{

	//INVERTIR CARAS//
	int sides = _sides;
	if (sides < 3) sides = 3;
	using namespace bg::math;
	bg::ptr<bg::base::PolyList> _plist;
	_plist = new bg::base::PolyList(ctx);
	_plist->setDrawMode(bg::base::PolyList::kTriangles);
	float x, y;

	if (axis = 2)
	{
		for (int i = 0; i < sides + 1; i++)
		{
			x = (float)(i) / sides * bg::math::k2Pi;
			y = r * trigonometry::sin(x);
			x = r * trigonometry::cos(x);

			_plist->addVertex(Vector3(x, y, -h));
			_plist->addNormal(Vector3(x, y, 0));
			_plist->addTexCoord0(Vector2((float)i / (sides), 0));

			_plist->addVertex(Vector3(x, y, h));
			_plist->addNormal(Vector3(x, y, 0));
			_plist->addTexCoord0(Vector2((float)i / sides, 1));

			if (i > 0)
			{
				_plist->addTriangle(2 * i, 2 * (i - 1), 2 * i - 1);
				_plist->addTriangle(2 * i + 1, 2 * i, 2 * i - 1);
			}
		}
		_plist->addTriangle(0, (sides) * 2, (sides) * 2 + 1);
		_plist->addTriangle(1, 0, (sides) * 2 + 1);


		//add centers// 2l
		_plist->addVertex(Vector3(0.0f, 0, -h));
		_plist->addNormal(Vector3(0.0f, 0.0f, -1.0f));
		_plist->addTexCoord0(Vector2(0.5f, 0.5f));

		int os = 2 * (sides + 1) + 1;
		for (int i = 0; i < sides; i++)
		{
			x = (float)(i) / sides * bg::math::k2Pi;
			y = r * trigonometry::sin(x);
			x = r * trigonometry::cos(x);
			_plist->addVertex(Vector3(x, y, -h));
			_plist->addNormal(Vector3(0.0f, 0.0f, -1.0f));
			_plist->addTexCoord0(Vector2(x / 2 + 0.5f, y / 2 + 0.5f));

			if (i > 0)
			{
				_plist->addTriangle(os - 1, os + i - 1, os + i);
			}
		}
		_plist->addTriangle(os - 1, os + sides - 1, os);

		os = os + sides + 1;

		_plist->addVertex(Vector3(0.0f, 0.0f, h));
		_plist->addNormal(Vector3(0.0f, 0.0f, 1.0f));
		_plist->addTexCoord0(Vector2(0.5f, 0.5f));
		for (int i = 0; i < sides; i++)
		{
			x = (float)(i) / sides * bg::math::k2Pi;
			y = r * trigonometry::sin(x);
			x = r * trigonometry::cos(x);
			_plist->addVertex(Vector3(x, y, h));
			_plist->addNormal(Vector3(0.0f, 0.0f, 1.0f));
			_plist->addTexCoord0(Vector2(x / 2 + 0.5f, y / 2 + 0.5f));

			if (i > 0)
			{
				_plist->addTriangle(os + i - 1, os - 1, os + i);
			}
		}
		_plist->addTriangle(os + sides - 1, os - 1, os);
	}
	if (axis = 1)
	{
		for (int i = 0; i < sides + 1; i++)
		{
			x = (float)(i) / sides * bg::math::k2Pi;
			y = r * trigonometry::sin(x);
			x = r * trigonometry::cos(x);

			_plist->addVertex(Vector3(x, -h, y));
			_plist->addNormal(Vector3(x, 0, y));
			_plist->addTexCoord0(Vector2((float)i / (sides), 0));

			_plist->addVertex(Vector3(x, h, y));
			_plist->addNormal(Vector3(x, 0, y));
			_plist->addTexCoord0(Vector2((float)i / sides, 1));

			if (i > 0)
			{
				_plist->addTriangle(2 * i, 2 * (i - 1), 2 * i - 1);
				_plist->addTriangle(2 * i + 1, 2 * i, 2 * i - 1);
			}
		}
		_plist->addTriangle(0, (sides) * 2, (sides) * 2 + 1);
		_plist->addTriangle(1, 0, (sides) * 2 + 1);


		//add centers// 2l
		_plist->addVertex(Vector3(0.0f, -h, 0));
		_plist->addNormal(Vector3(0.0f, -1.0f, 0.0f));
		_plist->addTexCoord0(Vector2(0.5f, 0.5f));

		int os = 2 * (sides + 1) + 1;
		for (int i = 0; i < sides; i++)
		{
			x = (float)(i) / sides * bg::math::k2Pi;
			y = r * trigonometry::sin(x);
			x = r * trigonometry::cos(x);
			_plist->addVertex(Vector3(x, -h, y));
			_plist->addNormal(Vector3(0.0f, -1.0f, 0.0f));
			_plist->addTexCoord0(Vector2(x / 2 + 0.5f, y / 2 + 0.5f));

			if (i > 0)
			{
				_plist->addTriangle(os - 1, os + i - 1, os + i);
			}
		}
		_plist->addTriangle(os - 1, os + sides - 1, os);

		os = os + sides + 1;

		_plist->addVertex(Vector3(0.0f, h, 0.0f));
		_plist->addNormal(Vector3(0.0f, 1.0f, 0.0f));
		_plist->addTexCoord0(Vector2(0.5f, 0.5f));
		for (int i = 0; i < sides; i++)
		{
			x = (float)(i) / sides * bg::math::k2Pi;
			y = r * trigonometry::sin(x);
			x = r * trigonometry::cos(x);
			_plist->addVertex(Vector3(x, h, y));
			_plist->addNormal(Vector3(0.0f, 1.0f, 0.0f));
			_plist->addTexCoord0(Vector2(x / 2 + 0.5f, y / 2 + 0.5f));

			if (i > 0)
			{
				_plist->addTriangle(os + i - 1, os - 1, os + i);
			}
		}
		_plist->addTriangle(os + sides - 1, os - 1, os);
	}
	

	


	_plist->build();
	bg::ptr<bg::scene::Drawable> drw = new bg::scene::Drawable();
	bg::ptr<bg::base::Material> mat = new bg::base::Material();
	mat->setDiffuse(bg::math::Color::White());
	//mat->setTexture(bg::db::loadTexture(ctx, ts::kColorCircleTexture1));
	//mat->setUnlit(true);
	drw->addPolyList(_plist.getPtr(), mat.getPtr());
	return drw.release();
}
bg::scene::Drawable * getPrisma(int _sides, int h, int r, bg::base::Context * ctx)
{
	int sides = _sides;
	if (sides < 3) sides = 3;
	using namespace bg::math;
	bg::ptr<bg::base::PolyList> _plist;
	_plist = new bg::base::PolyList(ctx);
	_plist->setDrawMode(bg::base::PolyList::kTriangles);


	float x, y;
	bg::math::Vector3 p1, p2, p3;
	bg::math::Vector3 normal, v1, v2;
	for (int i = 0; i < sides; i++)
	{
		x = (float)(i) / sides * bg::math::k2Pi;
		y = trigonometry::sin(x);
		x = trigonometry::cos(x);
		p1.set(x, -h, y);
		p2.set(x, h, y);

		_plist->addVertex(Vector3(x, -h, y));
		_plist->addTexCoord0(Vector2((float)i / (sides), 0));

		_plist->addVertex(Vector3(x, h, y));
		_plist->addTexCoord0(Vector2((float)i / sides, 1));

		x = (float)(i + 1) / sides * bg::math::k2Pi;
		y = trigonometry::sin(x);
		x = trigonometry::cos(x);

		p3.set(x, -h, y);
		v1 = p2 - p1;
		v2 = p3 - p1;
		normal = v1.cross(v2);


		_plist->addVertex(Vector3(x, -h, y));
		_plist->addTexCoord0(Vector2((float)(i + 1) / (sides), 0));

		_plist->addVertex(Vector3(x, h, y));
		_plist->addTexCoord0(Vector2((float)(i + 1) / (sides), 1));

		_plist->addNormal(normal);
		_plist->addNormal(normal);
		_plist->addNormal(normal);
		_plist->addNormal(normal);

		_plist->addTriangle(4 * i + 2, 4 * i, 4 * i + 1);
		_plist->addTriangle(4 * i + 3, 4 * i + 2, 4 * i + 1);
	}


	//add centers// 2l
	_plist->addVertex(Vector3(0.0f, -h, 0));
	_plist->addNormal(Vector3(0.0f, -1.0f, 0.0f));
	_plist->addTexCoord0(Vector2(0.5f, 0.5f));

	int os = 4 * (sides)+1;
	for (int i = 0; i < sides; i++)
	{
		x = (float)(i) / sides * bg::math::k2Pi;
		y = trigonometry::sin(x);
		x = trigonometry::cos(x);
		_plist->addVertex(Vector3(x, -h, y));
		_plist->addNormal(Vector3(0.0f, -1.0f, 0.0f));
		_plist->addTexCoord0(Vector2(x / 2 + 0.5f, y / 2 + 0.5f));

		if (i > 0)
		{
			_plist->addTriangle(os - 1, os + i - 1, os + i);
		}
	}
	_plist->addTriangle(os - 1, os + sides - 1, os);

	os = os + sides + 1;

	_plist->addVertex(Vector3(0.0f, h, 0.0f));
	_plist->addNormal(Vector3(0.0f, 1.0f, 0.0f));
	_plist->addTexCoord0(Vector2(0.5f, 0.5f));
	for (int i = 0; i < sides; i++)
	{
		x = (float)(i) / sides * bg::math::k2Pi;
		y = trigonometry::sin(x);
		x = trigonometry::cos(x);
		_plist->addVertex(Vector3(x, h, y));
		_plist->addNormal(Vector3(0.0f, 1.0f, 0.0f));
		_plist->addTexCoord0(Vector2(x / 2 + 0.5f, y / 2 + 0.5f));

		if (i > 0)
		{
			_plist->addTriangle(os + i - 1, os - 1, os + i);
		}
	}
	_plist->addTriangle(os + sides - 1, os - 1, os);



	_plist->build();
	bg::ptr<bg::scene::Drawable> drw = new bg::scene::Drawable();
	bg::ptr<bg::base::Material> mat = new bg::base::Material();
	mat->setDiffuse(bg::math::Color::White());

	drw->addPolyList(_plist.getPtr(), mat.getPtr());
	return drw.release();
}
bg::scene::Drawable * getTorus(int _sides, int _slides, float _R, float _r, bg::base::Context * ctx)
{
	using namespace bg::math;
	int sides = _sides + 1;
	int slides = _slides + 1;
	float R = _R;
	float r = _r;
	if (sides < 3) sides = 3;
	if (slides < 3) slides = 3;

	bg::ptr<bg::base::PolyList> _plist;
	_plist = new bg::base::PolyList(ctx);
	_plist->setDrawMode(bg::base::PolyList::kTriangles);

	float x;
	float y;
	float z;
	float a;
	float b;

	Vector3 normal;
	for (int i = 0; i < slides; i++)
	{
		a = (float)i / (slides - 1) * bg::math::k2Pi;
		for (int j = 0; j < sides; j++)
		{
			b = (float)j / (sides - 1) * bg::math::k2Pi;
			x = trigonometry::cos(a)*(R + r * trigonometry::cos(b));
			y = r * trigonometry::sin(b);
			z = trigonometry::sin(a)*(R + r * trigonometry::cos(b));
			normal.set(trigonometry::cos(a)*trigonometry::cos(b), trigonometry::sin(b), trigonometry::sin(a)*trigonometry::cos(b));



			_plist->addVertex(Vector3(x, y, z));
			_plist->addNormal(normal); // OKEY
			_plist->addTexCoord0(Vector2((float)j / sides, (float)i / slides)); //THINK OKE


			if (j > 0)
			{
				if (i > 0)
				{
					_plist->addTriangle(i*sides + j, i*sides + j - 1, (i - 1)*sides + j);
					_plist->addTriangle((i - 1)*sides + j, i*sides + j - 1, (i - 1)*sides + j - 1);
				}

			}

		}
		if (i > 0)
		{
			_plist->addTriangle(i*sides, i*sides + sides - 1, (i - 1)*sides);
			_plist->addTriangle((i - 1)*sides, i*sides + sides - 1, (i - 1)*sides + sides - 1);
		}

	}
	for (int n = 1; n < sides; n++)
	{
		_plist->addTriangle(n - 1, (slides - 1)*sides + n, n);
		_plist->addTriangle((slides - 1)*sides + n, n - 1, (slides - 1)*sides + n - 1);
	}
	_plist->addTriangle(0, sides - 1, (slides - 1)*sides);
	_plist->addTriangle((slides - 1)*sides, sides - 1, slides*sides - 1);

	_plist->build();
	bg::ptr<bg::scene::Drawable> drw = new bg::scene::Drawable();
	bg::ptr<bg::base::Material> mat = new bg::base::Material();
	mat->setDiffuse(bg::math::Color::White());
	drw->addPolyList(_plist.getPtr(), mat.getPtr());
	return drw.release();
}
bg::scene::Drawable * getTorusWireframe(int _sides, int _slides, float _R, float _r, bg::base::Context * ctx)
{
	using namespace bg::math;
	int sides = _sides;
	int slides = _slides;
	float R = _R;
	float r = _r;
	bg::ptr<bg::base::PolyList> _plist;
	_plist = new bg::base::PolyList(ctx);
	_plist->setDrawMode(bg::base::PolyList::kLines);

	float x;
	float y;
	float z;
	float a;
	float b;
	for (int i = 0; i < slides; i++)
	{
		a = (float)i / slides * bg::math::k2Pi;
		for (int j = 0; j < sides; j++)
		{
			b = (float)j / sides * bg::math::k2Pi;
			x = trigonometry::cos(a)*(R + r * trigonometry::cos(b));
			y = r * trigonometry::sin(b);
			z = trigonometry::sin(a)*(R + r * trigonometry::cos(b));


			_plist->addVertex(Vector3(x, y, z));
			_plist->addNormal(Vector3(x, y, z));//ITS FALSE
			_plist->addTexCoord0(Vector2(0, 0));//ALSO FALSE
			if (j > 0)
			{
				_plist->addIndex(i*sides + j - 1);
				_plist->addIndex(i*sides + j);
				if (i > 0)
				{
					_plist->addIndex(i*sides + j);
					_plist->addIndex((i - 1)*sides + j);
				}

			}
			_plist->addIndex(i*sides + sides - 1);
			_plist->addIndex(i*sides);
		}
	}
	for (int n = 0; n < sides; n++)
	{
		_plist->addIndex((slides - 1)*(sides)+n);
		_plist->addIndex(n);
	}

	_plist->build();
	bg::ptr<bg::scene::Drawable> drw = new bg::scene::Drawable();
	bg::ptr<bg::base::Material> mat = new bg::base::Material();
	mat->setDiffuse(bg::math::Color::White());
	mat->setUnlit(true);

	drw->addPolyList(_plist.getPtr(), mat.getPtr());
	return drw.release();


}
bg::scene::Drawable * getWind(std::vector<bg::math::Vector3> points, bg::base::Context * ctx)
{

	using namespace bg::math;
	bg::ptr<bg::base::PolyList> _plist;
	_plist = new bg::base::PolyList(ctx);
	_plist->setDrawMode(bg::base::PolyList::kLines);
	Vector3 randVector;
	for (int i = 0; i < points.size(); i++)
	{
		randVector.set(bg::math::random(), bg::math::random(), bg::math::random());
		randVector.scale(0.001f);
		_plist->addVertex(points[i]);
		_plist->addNormal(bg::math::Vector3(1.0f, 0.0f, 0.0f));
		_plist->addTexCoord0(bg::math::Vector2(0.0f, 0.0f));

		_plist->addVertex(points[i] + randVector);
		_plist->addNormal(bg::math::Vector3(1.0f, 0.0f, 0.0f));
		_plist->addTexCoord0(bg::math::Vector2(1.0f, 1.0f));

		_plist->addIndex(2 * i);
		_plist->addIndex(2 * i + 1);
	}

	_plist->build();
	bg::ptr<bg::scene::Drawable> drw = new bg::scene::Drawable();
	bg::ptr<bg::base::Material> mat = new bg::base::Material();
	mat->setDiffuse(bg::math::Color::White());
	mat->setUnlit(true);
	mat->setCastShadows(false);
	mat->setReceiveShadows(false);

	drw->addPolyList(_plist.getPtr(), mat.getPtr());
	return drw.release();
}
//FALTA AÑADIR SI ES SUPERFICIE INTERIOR O EXTERIOR//
bg::scene::Drawable * getSurface(std::function<float(float, float)> surfEq, bg::math::Vector2 min, bg::math::Vector2 max, bg::base::Context * ctx, float h, bg::math::Vector2 npoints)
{
	using namespace bg::math;
	bg::ptr<bg::base::PolyList> _plist;
	_plist = new bg::base::PolyList(ctx);
	_plist->setDrawMode(bg::base::PolyList::kTriangles);
	float x, y, z;
	int xsteps, ysteps;
	bg::math::Vector3 normal;
	bg::math::Vector3 aux;
	if (npoints == bg::math::Vector2(-1, -1))
	{
		xsteps = (int)(max.x() - min.x()) * 5;
		ysteps = (int)(max.y() - min.y()) * 5;
	}
	else
	{
		xsteps = npoints.x();
		ysteps = npoints.y();
	}

	float dx = (max.x() - min.x()) / (xsteps - 1);
	float dy = (max.y() - min.y()) / (ysteps - 1);

	for (int i = 0; i < xsteps; i++)
	{
		for (int j = 0; j < ysteps; j++)
		{
			x = (float)i*dx + min.x();
			y = (float)j*dy + min.y();
			z = surfEq(x, y);
			_plist->addVertex(bg::math::Vector3(x, z, y));
			_plist->addTexCoord0(bg::math::Vector2((float)i / xsteps, (float)j / ysteps));
			aux = getNormal(surfEq, x, y, h);
			normal.set(aux.x(), aux.z(), aux.y());
			_plist->addNormal(normal.scale(-1));

			if (j > 0)
			{
				if (i > 0)
				{
					_plist->addTriangle(i*ysteps + j, i*ysteps + j - 1, (i - 1)*ysteps + j - 1);
					_plist->addTriangle(i*ysteps + j, (i - 1)*ysteps + j - 1, (i - 1)*ysteps + j);
				}
			}

		}
	}
	size_t offset = xsteps * ysteps;
	for (int i = 0; i < xsteps; i++)
	{
		for (int j = 0; j < ysteps; j++)
		{
			x = (float)i*dx + min.x();
			y = (float)j*dy + min.y();
			z = surfEq(x, y);
			_plist->addVertex(bg::math::Vector3(x, z, y));
			_plist->addTexCoord0(bg::math::Vector2((float)i / xsteps, (float)j / ysteps));
			aux = getNormal(surfEq, x, y, h);
			normal.set(aux.x(), aux.z(), aux.y());
			_plist->addNormal(normal);

			if (j > 0)
			{
				if (i > 0)
				{
					_plist->addTriangle(offset + i * ysteps + j - 1, offset + i * ysteps + j, offset + (i - 1)*ysteps + j - 1);
					_plist->addTriangle(offset + (i - 1)*ysteps + j - 1, offset + i * ysteps + j, offset + (i - 1)*ysteps + j);
				}
			}

		}
	}
	_plist->build();
	bg::ptr<bg::scene::Drawable> drw = new bg::scene::Drawable();
	bg::ptr<bg::base::Material> mat = new bg::base::Material();
	mat->setDiffuse(bg::math::Color::White());
	mat->setCullFace(true);
	drw->addPolyList(_plist.getPtr(), mat.getPtr());
	return drw.release();
}
bg::scene::Drawable * getSurfaceWireframe(std::function<float(float, float)> surfEq, bg::math::Vector2 min, bg::math::Vector2 max, bg::base::Context * ctx, bg::math::Vector2 npoints)
{
	using namespace bg::math;
	bg::ptr<bg::base::PolyList> _plist;
	_plist = new bg::base::PolyList(ctx);
	_plist->setDrawMode(bg::base::PolyList::kLines);
	float x, y, z;
	int xsteps, ysteps;
	if (npoints == bg::math::Vector2(-1, -1))
	{
		xsteps = (max.x() - min.x()) * 5;
		ysteps = (max.y() - min.y()) * 5;
	}
	else
	{
		xsteps = npoints.x();
		ysteps = npoints.y();
	}

	float dx = (max.x() - min.x()) / (xsteps - 1);
	float dy = (max.y() - min.y()) / (ysteps - 1);

	for (int i = 0; i < xsteps; i++)
	{
		for (int j = 0; j < ysteps; j++)
		{
			x = (float)i*dx + min.x();
			y = (float)j*dy + min.y();
			z = surfEq(x, y);
			_plist->addVertex(bg::math::Vector3(x, z, y));
			_plist->addTexCoord0(bg::math::Vector2(0.0f, 1.0f));
			_plist->addNormal(bg::math::Vector3(0.0f, 1.0f, 0.0f));
			if (j > 0)
			{
				_plist->addIndex(i*ysteps + j - 1);
				_plist->addIndex(i*ysteps + j);

			}
			if (i > 0)
			{
				_plist->addIndex(i*ysteps + j);
				_plist->addIndex((i - 1)*ysteps + j);
			}
		}
	}

	_plist->build();
	bg::ptr<bg::scene::Drawable> drw = new bg::scene::Drawable();
	bg::ptr<bg::base::Material> mat = new bg::base::Material();
	mat->setDiffuse(bg::math::Color::White());
	mat->setUnlit(true);
	drw->addPolyList(_plist.getPtr(), mat.getPtr());
	return drw.release();
}
float derivate(std::function<float(float, float)> f, float x, float y, bool dfdy, float h)
{
	float d;
	if (!dfdy)
	{
		d = (f(x + h, y) - f(x, y)) / h;
	}
	else
	{
		d = (f(x, y + h) - f(x, y)) / h;
	}
	return d;
}
bg::math::Vector3 getNormal(std::function<float(float, float)> f, float x, float y, float h)
{
	return bg::math::Vector3(derivate(f, x, y, 0, h), derivate(f, x, y, 1, h), -1);
}

bg::math::Vector3 getNormalParam(std::function<bg::math::Vector3(float, float)> f, float t, float k, float h)
{
	bg::math::Vector3 O(f(t, k));
	bg::math::Vector3 v1(f(t + h, k));
	bg::math::Vector3 v2(f(t, k + h));
	v1.sub(O);
	v2.sub(O);
	return v1.cross(v2);
}

bg::scene::Drawable * getSurface(std::function<bg::math::Vector3(float, float)> surfParam, bg::math::Vector2 min, bg::math::Vector2 max, bg::base::Context * ctx, float h, bool doubleFace)
{
	using namespace bg::math;
	bg::ptr<bg::base::PolyList> _plist;
	_plist = new bg::base::PolyList(ctx);
	_plist->setDrawMode(bg::base::PolyList::kTriangles);
	float t, k;
	int tsteps, ksteps;
	bg::math::Vector3 normal;
	bg::math::Vector3 aux;
	/*if (npoints == bg::math::Vector2(-1, -1))
	{*/
		tsteps = (max.x() - min.x()) * 5;
		ksteps = (max.y() - min.y()) * 5;
	//}
	/*else
	{
		xsteps = npoints.x();
		ysteps = npoints.y();
	}*/

	float dt = (max.x() - min.x()) / (tsteps - 1);
	float dk = (max.y() - min.y()) / (ksteps - 1);

	for (int i = 0; i < tsteps; i++)
	{
		for (int j = 0; j < ksteps; j++)
		{
			t = (float)i*dt + min.x();
			k = (float)j*dk + min.y();
			_plist->addVertex(surfParam(t,k));
			_plist->addTexCoord0(bg::math::Vector2((float)i / tsteps, (float)j / ksteps));
			aux = getNormalParam(surfParam, t, k, h);
			normal.set(aux.x(), aux.z(), aux.y());
			_plist->addNormal(normal.scale(-1));

			if (j > 0)
			{
				if (i > 0)
				{
					_plist->addTriangle(i*ksteps + j, i*ksteps + j - 1, (i - 1)*ksteps + j - 1);
					_plist->addTriangle(i*ksteps + j, (i - 1)*ksteps + j - 1, (i - 1)*ksteps + j);
				}
			}

		}
	}
	if (doubleFace)
	{
		size_t offset = tsteps * ksteps;
		for (int i = 0; i < tsteps; i++)
		{
			for (int j = 0; j < ksteps; j++)
			{
				t = (float)i*dt + min.x();
				k = (float)j*dk + min.y();

				_plist->addVertex(surfParam(t, k));
				_plist->addTexCoord0(bg::math::Vector2((float)i / tsteps, (float)j / ksteps));
				aux = getNormalParam(surfParam, t, k, h);
				normal.set(aux.x(), aux.y(), aux.z());
				_plist->addNormal(normal);

				if (j > 0)
				{
					if (i > 0)
					{
						_plist->addTriangle(offset + i * ksteps + j - 1, offset + i * ksteps + j, offset + (i - 1)*ksteps + j - 1);
						_plist->addTriangle(offset + (i - 1)*ksteps + j - 1, offset + i * ksteps + j, offset + (i - 1)*ksteps + j);
					}
				}

			}
		}
	}

	_plist->build();
	bg::ptr<bg::scene::Drawable> drw = new bg::scene::Drawable();
	bg::ptr<bg::base::Material> mat = new bg::base::Material();
	mat->setDiffuse(bg::math::Color::White());
	mat->setCullFace(true);
	drw->addPolyList(_plist.getPtr(), mat.getPtr());
	return drw.release();
}

bg::scene::Drawable * getWindowFrame(bg::base::Context * ctx, bg::math::Vector2 efectiveSize, bg::math::Vector3 size)
{
	bg::ptr<bg::base::PolyList> _efectSurface = new bg::base::PolyList(ctx);

	bg::math::Vector2 _efectiveSize = efectiveSize;
	_efectiveSize.scale(0.5f);

	//FACE 1
	_efectSurface->addVertex(bg::math::Vector2(_efectiveSize.x(), _efectiveSize.y()));
	_efectSurface->addVertex(bg::math::Vector2(_efectiveSize.x(), -_efectiveSize.y()));
	_efectSurface->addVertex(bg::math::Vector2(-_efectiveSize.x(), -_efectiveSize.y()));
	_efectSurface->addVertex(bg::math::Vector2(-_efectiveSize.x(), _efectiveSize.y()));

	_efectSurface->addTexCoord0(bg::math::Vector2(1.0f, 1.0f));
	_efectSurface->addTexCoord0(bg::math::Vector2(1.0f, 0.0f));
	_efectSurface->addTexCoord0(bg::math::Vector2(0.0f, 0.0f));
	_efectSurface->addTexCoord0(bg::math::Vector2(0.0f, 1.0f));

	_efectSurface->addNormal(bg::math::Vector3(0.0f, 0.0f, 1.0f));
	_efectSurface->addNormal(bg::math::Vector3(0.0f, 0.0f, 1.0f));
	_efectSurface->addNormal(bg::math::Vector3(0.0f, 0.0f, 1.0f));
	_efectSurface->addNormal(bg::math::Vector3(0.0f, 0.0f, 1.0f));

	_efectSurface->addTriangle(2, 1, 0);
	_efectSurface->addTriangle(0, 3, 2);

	_efectSurface->build();


	bg::ptr<bg::base::PolyList> _frame = new bg::base::PolyList(ctx);

	bg::math::Vector3 _size = size;
	_size.scale(0.5f);
	bg::math::Vector3 _frameDim((size.x() - efectiveSize.x())/2, (size.y() - efectiveSize.y())/2, size.z());

	_frame->addVertex(bg::math::Vector3(_size.x(), _size.y(), 0.0f));
	_frame->addVertex(bg::math::Vector3(_size.x(), -_size.y(), 0.0f));
	_frame->addVertex(bg::math::Vector3(-_size.x(), -_size.y(), 0.0f));
	_frame->addVertex(bg::math::Vector3(-_size.x(), _size.y(), 0.0f));

	_frame->addNormal(bg::math::Vector3(0.0f, 0.0f, -1.0f));
	_frame->addNormal(bg::math::Vector3(0.0f, 0.0f, -1.0f));
	_frame->addNormal(bg::math::Vector3(0.0f, 0.0f, -1.0f));
	_frame->addNormal(bg::math::Vector3(0.0f, 0.0f, -1.0f));

	_frame->addTexCoord0(bg::math::Vector2(1.0f, 1.0f));
	_frame->addTexCoord0(bg::math::Vector2(1.0f, 0.0f));
	_frame->addTexCoord0(bg::math::Vector2(0.0f, 0.0f));
	_frame->addTexCoord0(bg::math::Vector2(0.0f, 1.0f));

	_frame->addTriangle(0, 1, 2);
	_frame->addTriangle(2, 3, 0);

	//SECOND 4-0

	_frame->addVertex(bg::math::Vector3(-_size.x(), _size.y(), 0.0f));
	_frame->addVertex(bg::math::Vector3(_size.x(), _size.y(), 0.0f));
	_frame->addVertex(bg::math::Vector3(-_size.x(), _size.y(), _frameDim.z()));
	_frame->addVertex(bg::math::Vector3(_size.x(), _size.y(), _frameDim.z()));


	_frame->addNormal(bg::math::Vector3(0.0f, 1.0f, 0.0f));
	_frame->addNormal(bg::math::Vector3(0.0f, 1.0f, 0.0f));
	_frame->addNormal(bg::math::Vector3(0.0f, 1.0f, 0.0f));
	_frame->addNormal(bg::math::Vector3(0.0f, 1.0f, 0.0f));

	_frame->addTexCoord0(bg::math::Vector2(0.0f, 1.0f));
	_frame->addTexCoord0(bg::math::Vector2(1.0f, 1.0f));
	_frame->addTexCoord0(bg::math::Vector2(0.0f, 0.0f));
	_frame->addTexCoord0(bg::math::Vector2(1.0f, 0.0f));

	_frame->addTriangle(4, 6, 5);
	_frame->addTriangle(6, 7, 5);


	//BOT FACE// 8-0


	_frame->addVertex(bg::math::Vector3(-_size.x(), -_size.y(), 0.0f));
	_frame->addVertex(bg::math::Vector3(_size.x(), -_size.y(), 0.0f));
	_frame->addVertex(bg::math::Vector3(-_size.x(), -_size.y(), _frameDim.z()));
	_frame->addVertex(bg::math::Vector3(_size.x(), -_size.y(), _frameDim.z()));


	_frame->addNormal(bg::math::Vector3(0.0f, -1.0f, 0.0f));
	_frame->addNormal(bg::math::Vector3(0.0f, -1.0f, 0.0f));
	_frame->addNormal(bg::math::Vector3(0.0f, -1.0f, 0.0f));
	_frame->addNormal(bg::math::Vector3(0.0f, -1.0f, 0.0f));

	_frame->addTexCoord0(bg::math::Vector2(0.0f, 1.0f));
	_frame->addTexCoord0(bg::math::Vector2(1.0f, 1.0f));
	_frame->addTexCoord0(bg::math::Vector2(0.0f, 0.0f));
	_frame->addTexCoord0(bg::math::Vector2(1.0f, 0.0f));

	_frame->addTriangle(8, 9, 10);
	_frame->addTriangle(9, 11, 10);


	//RIGHT FACE// 12-0
	_frame->addVertex(bg::math::Vector3(_size.x(), _size.y(), 0.0f));
	_frame->addVertex(bg::math::Vector3(_size.x(), _size.y(), _frameDim.z()));
	_frame->addVertex(bg::math::Vector3(_size.x(), -_size.y(), 0.0f));
	_frame->addVertex(bg::math::Vector3(_size.x(), -_size.y(), _frameDim.z()));


	_frame->addNormal(bg::math::Vector3(1.0f, 0.0f, 0.0f));
	_frame->addNormal(bg::math::Vector3(1.0f, 0.0f, 0.0f));
	_frame->addNormal(bg::math::Vector3(1.0f, 0.0f, 0.0f));
	_frame->addNormal(bg::math::Vector3(1.0f, 0.0f, 0.0f));

	_frame->addTexCoord0(bg::math::Vector2(0.0f, 1.0f));
	_frame->addTexCoord0(bg::math::Vector2(1.0f, 1.0f));
	_frame->addTexCoord0(bg::math::Vector2(0.0f, 0.0f));
	_frame->addTexCoord0(bg::math::Vector2(1.0f, 0.0f));

	_frame->addTriangle(12, 13, 14);
	_frame->addTriangle(15, 14, 13);

	//LEFT FACE// 16-0
	_frame->addVertex(bg::math::Vector3(-_size.x(), _size.y(), 0.0f));
	_frame->addVertex(bg::math::Vector3(-_size.x(), _size.y(), _frameDim.z()));
	_frame->addVertex(bg::math::Vector3(-_size.x(), -_size.y(), 0.0f));
	_frame->addVertex(bg::math::Vector3(-_size.x(), -_size.y(), _frameDim.z()));


	_frame->addNormal(bg::math::Vector3(-1.0f, 0.0f, 0.0f));
	_frame->addNormal(bg::math::Vector3(-1.0f, 0.0f, 0.0f));
	_frame->addNormal(bg::math::Vector3(-1.0f, 0.0f, 0.0f));
	_frame->addNormal(bg::math::Vector3(-1.0f, 0.0f, 0.0f));

	_frame->addTexCoord0(bg::math::Vector2(0.0f, 1.0f));
	_frame->addTexCoord0(bg::math::Vector2(1.0f, 1.0f));
	_frame->addTexCoord0(bg::math::Vector2(0.0f, 0.0f));
	_frame->addTexCoord0(bg::math::Vector2(1.0f, 0.0f));

	_frame->addTriangle(18, 17, 16);
	_frame->addTriangle(17, 18, 19);

	//FRONT RIGHT FACE// 20-0

	_frame->addVertex(bg::math::Vector3(_size.x(), _size.y(), _frameDim.z()));
	_frame->addVertex(bg::math::Vector3(_size.x(), -_size.y(), _frameDim.z()));
	_frame->addVertex(bg::math::Vector3(_efectiveSize.x(), _efectiveSize.y(), _frameDim.z()));
	_frame->addVertex(bg::math::Vector3(_efectiveSize.x(), -_efectiveSize.y(), _frameDim.z()));


	_frame->addNormal(bg::math::Vector3(0.0f, 0.0f, 1.0f));
	_frame->addNormal(bg::math::Vector3(0.0f, 0.0f, 1.0f));
	_frame->addNormal(bg::math::Vector3(0.0f, 0.0f, 1.0f));
	_frame->addNormal(bg::math::Vector3(0.0f, 0.0f, 1.0f));

	//FIX THIS TEXTCOORD
	_frame->addTexCoord0(bg::math::Vector2(1.0f, 1.0f));
	_frame->addTexCoord0(bg::math::Vector2(1.0f, 0.0f));
	_frame->addTexCoord0(bg::math::Vector2((_size.x()+_efectiveSize.x())/(2*_size.x()), (_size.y() + _efectiveSize.y()) / (2 * _size.y())));
	_frame->addTexCoord0(bg::math::Vector2((_size.x() + _efectiveSize.x()) / (2 * _size.x()), (_size.y() - _efectiveSize.y()) / (2 * _size.y())));

	_frame->addTriangle(20, 22, 21);
	_frame->addTriangle(22, 23, 21);


	//FRONT LEft FACE// 24-0

	_frame->addVertex(bg::math::Vector3(-_size.x(), _size.y(), _frameDim.z()));
	_frame->addVertex(bg::math::Vector3(-_size.x(), -_size.y(), _frameDim.z()));
	_frame->addVertex(bg::math::Vector3(-_efectiveSize.x(), _efectiveSize.y(), _frameDim.z()));
	_frame->addVertex(bg::math::Vector3(-_efectiveSize.x(), -_efectiveSize.y(), _frameDim.z()));


	_frame->addNormal(bg::math::Vector3(0.0f, 0.0f, 1.0f));
	_frame->addNormal(bg::math::Vector3(0.0f, 0.0f, 1.0f));
	_frame->addNormal(bg::math::Vector3(0.0f, 0.0f, 1.0f));
	_frame->addNormal(bg::math::Vector3(0.0f, 0.0f, 1.0f));

	_frame->addTexCoord0(bg::math::Vector2(0.0f, 1.0f));
	_frame->addTexCoord0(bg::math::Vector2(0.0f, 0.0f));
	_frame->addTexCoord0(bg::math::Vector2((_size.x() - _efectiveSize.x()) / (2 * _size.x()), (_size.y() + _efectiveSize.y()) / (2 * _size.y())));
	_frame->addTexCoord0(bg::math::Vector2((_size.x() - _efectiveSize.x()) / (2 * _size.x()), (_size.y() - _efectiveSize.y()) / (2 * _size.y())));

	_frame->addTriangle(25, 26, 24);
	_frame->addTriangle(25, 27, 26);

	_frame->addTriangle(24, 26, 20);
	_frame->addTriangle(20, 26, 22);

	_frame->addTriangle(27, 21, 23);
	_frame->addTriangle(27, 25, 21);

	//28-0
	_frame->addVertex(bg::math::Vector3(_efectiveSize.x(), _efectiveSize.y(), _frameDim.z()));
	_frame->addVertex(bg::math::Vector3(_efectiveSize.x(), -_efectiveSize.y(), _frameDim.z()));
	_frame->addVertex(bg::math::Vector3(_efectiveSize.x(), _efectiveSize.y(), 0.0f));
	_frame->addVertex(bg::math::Vector3(_efectiveSize.x(), -_efectiveSize.y(), 0.0f));


	_frame->addNormal(bg::math::Vector3(-1.0f, 0.0f, 0.0f));
	_frame->addNormal(bg::math::Vector3(-1.0f, 0.0f, 0.0f));
	_frame->addNormal(bg::math::Vector3(-1.0f, 0.0f, 0.0f));
	_frame->addNormal(bg::math::Vector3(-1.0f, 0.0f, 0.0f));

	_frame->addTexCoord0(bg::math::Vector2(0.0f, 1.0f));
	_frame->addTexCoord0(bg::math::Vector2(1.0f, 1.0f));
	_frame->addTexCoord0(bg::math::Vector2(0.0f, 0.0f));
	_frame->addTexCoord0(bg::math::Vector2(1.0f, 0.0f));

	_frame->addTriangle(28, 30, 29);
	_frame->addTriangle(30, 31, 29);


	//32-0
	_frame->addVertex(bg::math::Vector3(-_efectiveSize.x(), _efectiveSize.y(), _frameDim.z()));
	_frame->addVertex(bg::math::Vector3(-_efectiveSize.x(), -_efectiveSize.y(), _frameDim.z()));
	_frame->addVertex(bg::math::Vector3(-_efectiveSize.x(), _efectiveSize.y(), 0.0f));
	_frame->addVertex(bg::math::Vector3(-_efectiveSize.x(), -_efectiveSize.y(), 0.0f));


	_frame->addNormal(bg::math::Vector3(-1.0f, 0.0f, 0.0f));
	_frame->addNormal(bg::math::Vector3(-1.0f, 0.0f, 0.0f));
	_frame->addNormal(bg::math::Vector3(-1.0f, 0.0f, 0.0f));
	_frame->addNormal(bg::math::Vector3(-1.0f, 0.0f, 0.0f));

	_frame->addTexCoord0(bg::math::Vector2(0.0f, 1.0f));
	_frame->addTexCoord0(bg::math::Vector2(1.0f, 1.0f));
	_frame->addTexCoord0(bg::math::Vector2(0.0f, 0.0f));
	_frame->addTexCoord0(bg::math::Vector2(1.0f, 0.0f));

	_frame->addTriangle(33, 34, 32);
	_frame->addTriangle(33, 35, 34);


	//36-0
	_frame->addVertex(bg::math::Vector3(_efectiveSize.x(), _efectiveSize.y(), _frameDim.z()));
	_frame->addVertex(bg::math::Vector3(-_efectiveSize.x(), _efectiveSize.y(), _frameDim.z()));
	_frame->addVertex(bg::math::Vector3(_efectiveSize.x(), _efectiveSize.y(), 0.0f));
	_frame->addVertex(bg::math::Vector3(-_efectiveSize.x(), _efectiveSize.y(), 0.0f));


	_frame->addNormal(bg::math::Vector3(-1.0f, 0.0f, 0.0f));
	_frame->addNormal(bg::math::Vector3(-1.0f, 0.0f, 0.0f));
	_frame->addNormal(bg::math::Vector3(-1.0f, 0.0f, 0.0f));
	_frame->addNormal(bg::math::Vector3(-1.0f, 0.0f, 0.0f));

	_frame->addTexCoord0(bg::math::Vector2(0.0f, 1.0f));
	_frame->addTexCoord0(bg::math::Vector2(1.0f, 1.0f));
	_frame->addTexCoord0(bg::math::Vector2(0.0f, 0.0f));
	_frame->addTexCoord0(bg::math::Vector2(1.0f, 0.0f));

	_frame->addTriangle(37, 38, 36);
	_frame->addTriangle(37, 39, 38);

	//40-0

	_frame->addVertex(bg::math::Vector3(_efectiveSize.x(), -_efectiveSize.y(), _frameDim.z()));
	_frame->addVertex(bg::math::Vector3(-_efectiveSize.x(), -_efectiveSize.y(), _frameDim.z()));
	_frame->addVertex(bg::math::Vector3(_efectiveSize.x(), -_efectiveSize.y(), 0.0f));
	_frame->addVertex(bg::math::Vector3(-_efectiveSize.x(), -_efectiveSize.y(), 0.0f));


	_frame->addNormal(bg::math::Vector3(-1.0f, 0.0f, 0.0f));
	_frame->addNormal(bg::math::Vector3(-1.0f, 0.0f, 0.0f));
	_frame->addNormal(bg::math::Vector3(-1.0f, 0.0f, 0.0f));
	_frame->addNormal(bg::math::Vector3(-1.0f, 0.0f, 0.0f));

	_frame->addTexCoord0(bg::math::Vector2(0.0f, 1.0f));
	_frame->addTexCoord0(bg::math::Vector2(1.0f, 1.0f));
	_frame->addTexCoord0(bg::math::Vector2(0.0f, 0.0f));
	_frame->addTexCoord0(bg::math::Vector2(1.0f, 0.0f));

	_frame->addTriangle(40, 42, 41);
	_frame->addTriangle(42, 43, 41);






	_frame->build();
	bg::base::Material *mat1 = new bg::base::Material;
	bg::base::Material *mat2 = new bg::base::Material;
	mat1->setDiffuse(bg::math::Color::Red());
	//mat2->setDiffuse(bg::math::Color::Green());
	bg::ptr<bg::scene::Drawable> drw = new bg::scene::Drawable;
	drw->addPolyList(_efectSurface.getPtr(), mat1);
	drw->addPolyList(_frame.getPtr(), mat2);

	return drw.release();
}

bg::math::Vector3 SeaShell(float t, float k)
{
	float x, y, z;
	float k1 = 1.2f;
	float a = 1.5f;
	x = pow(k1, t)*(1 + bg::math::trigonometry::cos(k))*bg::math::trigonometry::cos(t);
	y = pow(k1, t)*(1 + bg::math::trigonometry::cos(k))*bg::math::trigonometry::sin(t);
	z = pow(k1, t)*bg::math::trigonometry::sin(k) - a * pow(k1, t);
	return bg::math::Vector3(x, y, z);
}
