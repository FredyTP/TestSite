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


#ifndef _bg2e_scene_transform_hpp_
#define _bg2e_scene_transform_hpp_

#include <bg/scene/component.hpp>
#include <bg/math/matrix.hpp>
#include <bg/math/matrix_strategy.hpp>
#include <bg/db/json/value.hpp>

namespace bg {
namespace scene {

class BG2E_EXPORT TransformStrategy : public bg::math::MatrixStrategy, public bg::base::ReferencedPointer {
public:
	static TransformStrategy * Factory(bg::db::json::Value * jsonData);

	TransformStrategy();
	TransformStrategy(bg::math::Matrix4 * target);

	virtual TransformStrategy * clone() = 0;

	virtual void apply() = 0;

	virtual bg::db::json::Value * serialize() = 0;
	virtual void deserialize(bg::db::json::Value * value) = 0;

protected:
	virtual ~TransformStrategy();
};

class BG2E_EXPORT TRSTransformStrategy : public TransformStrategy {
public:
	enum RotationOrder {
		kOrderXYZ = 0,
		kOrderXZY,
		kOrderYXZ,
		kOrderYZX,
		kOrderZYX,
		kOrderZXY
	};

	TRSTransformStrategy();
	TRSTransformStrategy(bg::math::Matrix4 *);

	virtual TransformStrategy * clone();

	virtual void apply();

	inline void setTranslate(bg::math::Vector3 t) { _translate = t; apply(); }
	inline void setRotateX(float x) { _rotateX = x; apply(); }
	inline void setRotateY(float y) { _rotateY = y; apply(); }
	inline void setRotateZ(float z) { _rotateZ = z; apply(); }
	inline void setScale(bg::math::Vector3 s) { _scale = s; apply(); }

	inline const bg::math::Vector3 & translate() const { return _translate; }
	inline float rotateX() const { return _rotateX; }
	inline float rotateY() const { return _rotateY; }
	inline float rotateZ() const { return _rotateZ; }
	inline const bg::math::Vector3 & scale() const { return _scale; }

	inline void setRotationOrder(RotationOrder order) { _rotationOrder = order; apply(); }
	inline RotationOrder rotationOrder() const { return _rotationOrder; }

	void reset() {
		_translate.set(0.0f);
		_rotateX = 0.0f;
		_rotateY = 0.0f;
		_rotateZ = 0.0f;
		_scale.set(1.0f);
		apply();
	}

	virtual bg::db::json::Value * serialize();
	virtual void deserialize(bg::db::json::Value * value);

protected:
	virtual ~TRSTransformStrategy();

	bg::math::Vector3 _translate;
	float _rotateX;
	float _rotateY;
	float _rotateZ;
	bg::math::Vector3 _scale;

	RotationOrder _rotationOrder;
};

class BG2E_EXPORT Transform : public Component {
public:
	Transform();
	Transform(const bg::math::Matrix4 &);

	Component * clone();

	inline void setMatrix(const bg::math::Matrix4 & mat) { _matrix = mat; }
	inline const bg::math::Matrix4 & matrix() const { return _matrix; }
	inline bg::math::Matrix4 & matrix() { return _matrix; }


	const bg::math::Matrix4 & globalMatrix();

	virtual void willDraw(bg::base::Pipeline *);
	virtual void didDraw(bg::base::Pipeline *);
    virtual void willUpdate(bg::render::LowOverheadPipeline *, uint32_t currentFrame);
    virtual void didUpdate(bg::render::LowOverheadPipeline *, uint32_t currentFrame);
    
    virtual void willUpdate(bg::base::MatrixStack & viewMatrixStack);
    virtual void didUpdate(bg::base::MatrixStack & viewMatrixStack);


	virtual void deserialize(bg::base::Context *, bg::db::json::Value *, const bg::system::Path &);
	virtual bg::db::json::Value * serialize(bg::base::Context *, const bg::system::Path &);

	void setTransformStrategy(TransformStrategy *);

	template <class T>
	inline T * transformStrategy() { return dynamic_cast<T*>(_transformStrategy.getPtr()); }
	inline TransformStrategy * transformStrategy() { return _transformStrategy.getPtr(); }

protected:
	virtual ~Transform();

	bg::math::Matrix4 _matrix;
	bg::ptr<TransformStrategy> _transformStrategy;
	bg::math::Matrix4 _globalMatrix;
	bool _globalMatrixUpdated;
};

}
}

#endif
