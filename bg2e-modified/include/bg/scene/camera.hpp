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


#ifndef _bg2e_scene_camera_hpp_
#define _bg2e_scene_camera_hpp_

#include <bg/scene/component.hpp>
#include <bg/math/matrix.hpp>
#include <bg/math/vector.hpp>
#include <bg/math/matrix_strategy.hpp>
#include <bg/math/utils.hpp>
#include <bg/math/vector.hpp>
#include <bg/scene/transform_visitor.hpp>
#include <bg/db/json/value.hpp>

#ifdef near
#undef near
#endif

#ifdef far
#undef far
#endif

namespace bg {
namespace scene {

class BG2E_EXPORT ProjectionStrategy : public bg::math::MatrixStrategy, public bg::base::ReferencedPointer {
public:
	
	static ProjectionStrategy * Factory(bg::db::json::Value * jsonData);

	ProjectionStrategy();
	ProjectionStrategy(bg::math::Matrix4 * mat);

	virtual ProjectionStrategy * clone() = 0;

	inline void setNear(float n) { _near = n; }
	inline float near() const { return _near; }
	inline void setFar(float f) { _far = f; }
	inline float far() const { return _far; }
	inline void setViewport(const bg::math::Viewport & vp) { _vp = vp; }
	inline const bg::math::Viewport & viewport() const { return _vp; }
	inline bg::math::Viewport & viewport() { return _vp; }

	virtual bg::db::json::Value * serialize() = 0;
	virtual void deserialize(bg::db::json::Value * value) = 0;

protected:
	virtual ~ProjectionStrategy();

	float _near;
	float _far;
	bg::math::Viewport _vp;
};

class BG2E_EXPORT PerspectiveProjectionStrategy : public ProjectionStrategy {
public:
	PerspectiveProjectionStrategy();
	PerspectiveProjectionStrategy(bg::math::Matrix4 *);

	virtual ProjectionStrategy * clone();

	inline void setFov(float f) { _fov = f; }
	inline void setFov(const bg::math::Scalar & f) { _fov = bg::math::trigonometry::radiansToDegrees(f.value()); }
	inline float fov() const { return _fov; }

	virtual void apply();

	virtual bg::db::json::Value * serialize();
	virtual void deserialize(bg::db::json::Value * data);

protected:
	virtual ~PerspectiveProjectionStrategy();

	float _fov;
};

class BG2E_EXPORT OpticalProjectionStrategy : public ProjectionStrategy {
public:
	OpticalProjectionStrategy();
	OpticalProjectionStrategy(bg::math::Matrix4 * mat);

	virtual ProjectionStrategy * clone();

	inline void setFocalLength(float fl) { _focalLength = fl; }
	inline void setFocalLength(const bg::math::Scalar & fl) { _focalLength = fl.value() * bg::math::distance::mm; }
	inline float focalLength() const { return _focalLength; }
	inline void setFrameSize(float fs) { _frameSize = fs; }
	inline void setFrameSize(const bg::math::Scalar & fs) { _frameSize = fs.value() * bg::math::distance::mm; }
	inline float frameSize() const { return _frameSize; }

	virtual void apply();

	virtual bg::db::json::Value * serialize();

	virtual void deserialize(bg::db::json::Value *);

protected:
	virtual ~OpticalProjectionStrategy();

	float _focalLength;
	float _frameSize;
};

class BG2E_EXPORT OrthographicProjectionStrategy : public ProjectionStrategy {
public:
    OrthographicProjectionStrategy();
    OrthographicProjectionStrategy(bg::math::Matrix4 * mat);
    
    virtual ProjectionStrategy * clone();
    
    inline float viewWidth() const { return _viewWidth; }
    inline void setViewWidth(float w) { _viewWidth = w; }
    
    virtual void apply();
    
    virtual bg::db::json::Value * serialize();

    virtual void deserialize(bg::db::json::Value *);

protected:
    virtual ~OrthographicProjectionStrategy();
    
    float _viewWidth;
};

class BG2E_EXPORT Camera : public Component {
public:
	Camera();

	Component * clone();

	/*
	 *	setProjection have no effect if the camera contains a projection strategy
	 */
	inline void setProjection(const bg::math::Matrix4 & p) { if (!_projectionStrategy.valid()) _projection = p; }
	inline const bg::math::Matrix4 & projection() const { return _projection; }
	inline bg::math::Matrix4 & projection() { return _projection; }

	/*
	 *	setViewport() updates the projectionStrategy, if there is one configured
	 */
	void setViewport(const bg::math::Viewport & vp);
	inline const bg::math::Viewport & viewport() const { return _viewport; }
	inline bg::math::Viewport & viewport() { return _viewport; }

	void setProjectionStrategy(ProjectionStrategy * strategy);

	template <class T>
	inline T * projectionStrategy() { return dynamic_cast<T*>(_projectionStrategy.getPtr()); }

	inline ProjectionStrategy * projectionStrategy() { return _projectionStrategy.getPtr(); }

	inline float focus() const { return _focus; }
	inline void setFocus(float f) { _focus = f; }
	inline void setFocus(const bg::math::Scalar & f) { _focus = f.value(); }

	inline unsigned int clearBuffers() const { return _clearBuffers; }
	inline void setClearBuffers(unsigned int b) { _clearBuffers = b; }

	const bg::math::Matrix4 & modelMatrix();

	const bg::math::Matrix4 & viewMatrix();
 
    const bg::math::Vector3 & worldPosition();

	virtual void frame(float);

	virtual void deserialize(bg::base::Context *, bg::db::json::Value *, const bg::system::Path &);
	virtual bg::db::json::Value * serialize(bg::base::Context *, const bg::system::Path &);

protected:
	virtual ~Camera();

	bg::math::Matrix4 _projection;
	bg::math::Viewport _viewport;

	bg::ptr<TransformVisitor> _visitor;
	bg::math::Matrix4 _viewMatrix;
    bg::math::Vector3 _position;
	bool _rebuildTransform;
	bool _rebuildViewMatrix;
    bool _rebuildPosition;
	unsigned int _clearBuffers;
	float _focus;

	bg::ptr<ProjectionStrategy> _projectionStrategy;
};

}
}

#endif
