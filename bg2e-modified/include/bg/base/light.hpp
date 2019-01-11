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

#ifndef _bg2e_base_light_hpp_
#define _bg2e_base_light_hpp_

#include <bg/base/referenced_pointer.hpp>
#include <bg/math/vector.hpp>
#include <bg/math/matrix.hpp>
#include <bg/math/utils.hpp>

namespace bg {
namespace base {

class BG2E_EXPORT Light : public ReferencedPointer {
public:
	enum Type {
		kTypeDirectional = 4,
		kTypeSpot = 1,
		kTypePoint = 5,
		kTypeDisabled = 10	// deprecated
	};

	Light();
	Light(Light *);

	inline void setEnabled(bool e) { _enabled = e; }
	inline bool enabled() const { return _enabled; }

	inline void setType(Type t) {
		_type = t;
		if (_type==kTypeSpot) {
			_shadowBias = 0.0005f;
		}
		else if (_type==kTypeDirectional) {
			_shadowBias = 0.0005f;
		}
	}
	inline Type type() const { return _type; }

	inline void setDirection(const bg::math::Vector3 & d) { _direction = d; }
	inline const bg::math::Vector3 & direction() const { return _direction; }
	inline bg::math::Vector3 & direction() { return _direction; }

	inline void setAmbient(const bg::math::Color & c) { _ambient = c; }
	inline const bg::math::Color & ambient() const { return _ambient; }
	inline bg::math::Color & ambient() { return _ambient; }

	inline void setDiffuse(const bg::math::Color & c) { _diffuse = c; }
	inline const bg::math::Color & diffuse() const { return _diffuse; }
	inline bg::math::Color & diffuse() { return _diffuse; }

	inline void setSpecular(const bg::math::Color & c) { _specular = c; }
	inline const bg::math::Color & specular() const { return _specular; }
	inline bg::math::Color & specular() { return _specular; }

	inline void setAttenuationVector(const bg::math::Vector3 & att) { _attenuation = att; }
	inline const bg::math::Vector3 & attenuationVector() const { return _attenuation; }
	inline bg::math::Vector3 & attenuationVector() { return _attenuation; }

	inline void setConstantAttenuation(float a) { _attenuation.x(a); }
	inline float constantAttenuation() const { return _attenuation.x(); }
	inline void setLinearAttenuation(float a) { _attenuation.y(a); }
	inline float linearAttenuation() const { return _attenuation.y(); }
	inline void setQuadraticAttenuation(float a) { _attenuation.z(a); }
	inline float quadraticAttenuation() const { return _attenuation.z(); }

	inline void setCutoffDistance(float c) { _cutoffDistance = c; }
    inline void setCutoffDistance(const bg::math::Scalar & s) { _cutoffDistance = s.value(); }
	inline float cutoffDistance() const { return _cutoffDistance; }
	
	inline void setSpotCutoff(float c) { _spotCutoff = c; }
    inline void setSpotCutoff(const bg::math::Scalar & s) { _spotCutoff = bg::math::trigonometry::radiansToDegrees(s.value()); }
	inline float spotCutoff() const { return _spotCutoff; }

	inline void setSpotExponent(float e) { _spotExponent = e; }
	inline float spotExponent() const { return _spotExponent; }
 
    // The spot inner cutoff is used in the new lighting model instead of spot exponent
    inline void setInnerSpotCutoff(float c) { _innerSpotCutoff = c; }
    inline void setInnerSpotCutoff(const bg::math::Scalar & s) { _innerSpotCutoff = bg::math::trigonometry::radiansToDegrees(s.value()); }
    inline float innerSpotCutoff() const { return _innerSpotCutoff; }

	inline void setShadowStrength(float s) { _shadowStrength = s; }
	inline float shadowStrength() const { return _shadowStrength; }

	inline void setCastShadows(bool cs) { _castShadows = cs; }
	inline bool castShadows() const { return _castShadows; }
	
	inline void setProjection(const bg::math::Matrix4 & proj) { _projection = proj; }
	inline const bg::math::Matrix4 & projection() const { return _projection; }
	inline bg::math::Matrix4 & projection() { return _projection; }

	inline void setShadowBias(float bias) { _shadowBias = bias; }
	inline float shadowBias() const { return _shadowBias; }
	
protected:
	virtual ~Light();

	bool _enabled;
	Type _type;
	bg::math::Color _ambient;
	bg::math::Color _diffuse;
	bg::math::Color _specular;
	bg::math::Vector3 _attenuation;
	float _spotCutoff;
	float _spotExponent;
	float _shadowStrength;
	float _cutoffDistance;
	bool _castShadows;
	float _shadowBias;
    float _innerSpotCutoff;

	bg::math::Vector3 _direction;
	
	bg::math::Matrix4 _projection;
};

}
}

#endif
