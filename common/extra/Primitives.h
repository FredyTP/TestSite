#pragma once
#include <bg/bg2e.hpp>
#include <functional>

bg::scene::Drawable* getRegularPoly(int _sides, int size, bg::base::Context *ctx);
bg::scene::Drawable* getCilinder(int _sides, float h, float r, bg::base::Context *ctx,int axis);
bg::scene::Drawable* getPrisma(int _sides, int h, int r, bg::base::Context *ctx);
bg::scene::Drawable* getTorus(int _sides, int _slides, float _R, float _r, bg::base::Context *ctx);
bg::scene::Drawable * getTorusWireframe(int _sides, int _slides, float _R, float _r, bg::base::Context * ctx);
bg::scene::Drawable * getWind(std::vector<bg::math::Vector3> points, bg::base::Context* ctx);
bg::scene::Drawable * getSurface(std::function<float(float, float)> surfEq, bg::math::Vector2 min, bg::math::Vector2 max, bg::base::Context* ctx, float h = 0.001f, bg::math::Vector2 npoints = bg::math::Vector2(-1, -1));
bg::scene::Drawable * getSurfaceWireframe(std::function<float(float, float)> surfEq, bg::math::Vector2 min, bg::math::Vector2 max, bg::base::Context* ctx, bg::math::Vector2 npoints = bg::math::Vector2(-1, -1));
float derivate(std::function<float(float, float)> f, float x, float y, bool dfdy, float h = 0.0001f);
bg::math::Vector3 getNormal(std::function<float(float, float)> f, float x, float y, float h = 0.0001f);
bg::math::Vector3 getNormalParam(std::function<bg::math::Vector3(float, float)> f, float t, float k, float h = 0.00001f);
bg::scene::Drawable *getSurface(std::function<bg::math::Vector3(float, float)>, bg::math::Vector2 min, bg::math::Vector2 max, bg::base::Context *ctx, float h = 0.001f, bool doubleFace = true);

bg::scene::Drawable *getWindowFrame(bg::base::Context *ctx,bg::math::Vector2 efectiveSize,bg::math::Vector3 size);
//PARAMETRIC FUNCTIONS
bg::math::Vector3 SeaShell(float t, float k);


