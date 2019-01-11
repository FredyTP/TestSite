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


#include <bg/base/light.hpp>

namespace bg {
namespace base {

Light::Light()
	:_enabled(true)
	,_type(kTypeDirectional)
	,_ambient(0.2f, 0.2f, 0.2f, 1.0f)
	,_diffuse(0.9f, 0.9f, 0.9f, 1.0f)
	,_specular(1.0f, 1.0f, 1.0f, 1.0f)
	,_attenuation(1.0f, 0.5f, 0.1f)
	,_spotCutoff(20.0f)
	,_spotExponent(30.0f)
    ,_innerSpotCutoff(15.0f)
	,_shadowStrength(0.9f)
	,_cutoffDistance(-1.0f)
	,_castShadows(true)
	,_shadowBias(0.00005f)
	,_direction(0.0f, 0.0f, -1.0f)
{
	_projection = bg::math::Matrix4::Ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.5f, 300.0f);
}

Light::Light(Light * light)
	:_enabled(light->_enabled)
	,_type(light->_type)
	,_ambient(light->_ambient)
	,_diffuse(light->_diffuse)
	,_specular(light->_specular)
	,_attenuation(light->_attenuation)
	,_spotCutoff(light->_spotCutoff)
	,_spotExponent(light->_spotExponent)
	,_shadowStrength(light->_shadowStrength)
	,_cutoffDistance(light->_cutoffDistance)
	,_castShadows(light->_castShadows)
	,_shadowBias(light->_shadowBias)
	,_direction(light->_direction)
	,_projection(light->_projection)
{
}

Light::~Light() {

}

}
}
