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


#ifndef _bg2e_scene_light_hpp_
#define _bg2e_scene_light_hpp_

#include <bg/scene/component.hpp>
#include <bg/scene/node.hpp>
#include <bg/scene/transform_visitor.hpp>

#include <bg/base/light.hpp>
#include <bg/system/system.hpp>

#include <vector>

namespace bg {
namespace scene {

class BG2E_EXPORT Light : public Component {
public:
	static std::vector<Light*> & ActiveLights() { return s_activeLights; }

	Light();

	Component * clone();

	virtual void addedToNode(SceneObject *);
	virtual void removedFromNode(SceneObject *);

	inline const base::Light * light() const { return _light.getPtr(); }
	inline base::Light * light() { return _light.getPtr(); }

	bg::math::Matrix4 & transform();
    
    const bg::math::Vector3 & worldPosition();
    const bg::math::Vector3 & worldDirection();
    
    #if BG2E_WINDOWS==1
    __declspec(deprecated("Use worldPosition() instead")) inline const bg::math::Vector3 & absolutePosition() { return worldPosition(); }
    __declspec(deprecated("Use worldDirection() instead.")) inline const bg::math::Vector3 & absoluteDirection() { return worldDirection(); }
    #else
    inline const bg::math::Vector3 & absolutePosition() __attribute__((deprecated("Use worldPosition() instead.")))
    { return worldPosition(); }
    inline const bg::math::Vector3 & absoluteDirection()  __attribute__((deprecated("Use worldDirection() instead.")))
    { return worldDirection(); }
    #endif
    
	virtual void frame(float);

	virtual void deserialize(bg::base::Context *, bg::db::json::Value *, const bg::system::Path &);
	virtual bg::db::json::Value * serialize(bg::base::Context *, const bg::system::Path &);
 
    

protected:
	virtual ~Light();

	static std::vector<Light*> s_activeLights;
	
	ptr<base::Light> _light;
	ptr<TransformVisitor> _visitor;
	bool _rebuildTransform;
    bool _rebuildPosition;
    bool _rebuildDirection;
    
    bg::math::Vector3 _worldPosition;
    bg::math::Vector3 _worldDirection;
};

}
}

#endif
