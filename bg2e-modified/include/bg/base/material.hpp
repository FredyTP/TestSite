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

#ifndef _bg2e_base_material_hpp_
#define _bg2e_base_material_hpp_

#include <bg/base/referenced_pointer.hpp>
#include <bg/base/texture.hpp>
#include <bg/math/vector.hpp>

#include <bg/db/json/value.hpp>

#include <functional>

namespace bg {
namespace base {

class BG2E_EXPORT Material : public bg::base::ReferencedPointer {
public:    
	enum MaterialAttrib {
		kDiffuse				= 1 << 0,
		kSpecular				= 1 << 1,
		kShininess				= 1 << 2,
		kLightEmission			= 1 << 3,
		kRefractionAmount		= 1 << 4,
		kReflectionAmount		= 1 << 5,
		kTexture				= 1 << 6,
		kLightMap				= 1 << 7,
		kNormalMap				= 1 << 8,
		kTextureOffset			= 1 << 9,
		kTextureScale			= 1 << 10,
		kLightMapOffset			= 1 << 11,
		kLightMapScale			= 1 << 12,
		kNormalMapOffset		= 1 << 13,
		kNormalMapScale			= 1 << 14,
		kCastShadows			= 1 << 15,
		kReceiveShadows			= 1 << 16,
		kAlphaCutoff			= 1 << 17,
		kShininessMask			= 1 << 18,
		kLightEmissionMask		= 1 << 19,
		kDisplacementFactor		= 1 << 20,
		kDisplacementUV			= 1 << 21,
		kDisplacementMap		= 1 << 22,
		kTessellationDistances	= 1 << 23,
		kTessellationLevels		= 1 << 24,
		kReflectionMask			= 1 << 25,
		kCullFace				= 1 << 26,
		kRoughness				= 1 << 27,
		kUnlit					= 1 << 28,
		kProcessAsTransparent	= 1 << 29,
        
        kType                   = 1 << 31,
        
		kAllAttributes			= ~0
	};
	
	static Material * FromMaterialDefinition(bg::base::Context *, bg::db::json::Value *, const std::string & basePath = "");

	Material();
	Material(Material *);

	inline void setDiffuse(const bg::math::Vector4 & d) { _diffuse = d; }
	inline bg::math::Vector4 & diffuse() { return _diffuse; }
	inline const bg::math::Vector4 & diffuse() const { return _diffuse; }
	
	inline void setSpecular(const bg::math::Vector4 & d) { _specular = d; }
	inline bg::math::Vector4 & specular() { return _specular; }
	inline const bg::math::Vector4 & specular() const { return _specular; }
	
	inline void setShininess(float s) { _shininess = s; }
	inline float shininess() const { return _shininess; }
    	
	inline void setLightEmission(float e) { _lightEmission = e; }
	inline float lightEmission() const { return _lightEmission; }
	
	inline void setRefractionAmount(float r) { _refractionAmount = r; }
	inline float refractionAmount() const { return _refractionAmount; }
	
	inline void setReflectionAmount(float r) { _reflectionAmount = r; }
	inline float reflectionAmount() const { return _reflectionAmount; }    
	
	inline void setTexture(bg::base::Texture * t) { _texture = t; }
	inline bg::base::Texture * texture() { return _texture.getPtr(); }
	inline const bg::base::Texture * texture() const { return _texture.getPtr(); }
	
	inline void setLightMap(bg::base::Texture * t) { _lightMap = t; }
	inline bg::base::Texture * lightMap() { return _lightMap.getPtr(); }
	inline const bg::base::Texture * lightMap() const { return _lightMap.getPtr(); }
	
	inline void setNormalMap(bg::base::Texture * t) { _normalMap = t; }
	inline bg::base::Texture * normalMap() { return _normalMap.getPtr(); }
	inline const bg::base::Texture * normalMap() const { return _normalMap.getPtr(); }
	
	inline void setTextureOffset(const bg::math::Vector2 & o) { _textureOffset = o; }
	inline bg::math::Vector2 & textureOffset() { return _textureOffset; }
	inline const bg::math::Vector2 & textureOffset() const { return _textureOffset; }
	
	inline void setTextureScale(const bg::math::Vector2 & o) { _textureScale = o; }
	inline bg::math::Vector2 & textureScale() { return _textureScale; }
	inline const bg::math::Vector2 & textureScale() const { return _textureScale; }
	
	inline void setLightMapOffset(const bg::math::Vector2 & o) { _lightMapOffset = o; }
	inline bg::math::Vector2 & lightMapOffset() { return _lightMapOffset; }
	inline const bg::math::Vector2 & lightMapOffset() const { return _lightMapOffset; }
	
	inline void setLightMapScale(const bg::math::Vector2 & o) { _lightMapScale = o; }
	inline bg::math::Vector2 & lightMapScale() { return _lightMapScale; }
	inline const bg::math::Vector2 & lightMapScale() const { return _lightMapScale; }
	
	inline void setNormalMapScale(const bg::math::Vector2 & o) { _normalMapScale = o; }
	inline bg::math::Vector2 & normalMapScale() { return _normalMapScale; }
	inline const bg::math::Vector2 & normalMapScale() const { return _normalMapScale; }
	
	inline void setNormalMapOffset(const bg::math::Vector2 & o) { _normalMapOffset = o; }
	inline bg::math::Vector2 & normalMapOffset() { return _normalMapOffset; }
	inline const bg::math::Vector2 & normalMapOffset() const { return _normalMapOffset; }
	
	inline void setCastShadows(bool s) { _castShadows = s; }
	inline bool castShadows() const { return _castShadows; }
	
	inline void setReceiveShadows(bool s) { _receiveShadows = s; }
	inline bool receiveShadows() const { return _receiveShadows; }
	
	inline void setAlphaCutoff(float c) { _alphaCutoff = c; }
	inline float alphaCutoff() const { return _alphaCutoff; }
	
	inline void setShininessMask(bg::base::Texture * shininessMask, int channel = 0, bool invert = false) {
		_shininessMask = shininessMask;
		_shininessMaskChannel = channel;
		_shininessMaskInvert = invert;
	}
	inline bg::base::Texture * shininessMask() { return _shininessMask.getPtr(); }
	inline const bg::base::Texture * shininessMask() const { return _shininessMask.getPtr(); }
	inline void setShininessMaskChannel(int channel) { _shininessMaskChannel = channel; }
	inline int shininessMaskChannel() const { return _shininessMaskChannel; }
	inline bool invertShininessMask() const { return _shininessMaskInvert; }
	inline void setInvertShininessMask(bool invert) { _shininessMaskInvert = invert; }
    
    inline void setMetalnessMask(bg::base::Texture * shininessMask, int channel = 0, bool invert = false) {
        _shininessMask = shininessMask;
        _shininessMaskChannel = channel;
        _shininessMaskInvert = invert;
    }
	
	inline void setLightEmissionMask(bg::base::Texture * leMask, int channel = 0, bool invert = false) {
		_lightEmissionMask = leMask;
		_lightEmissionMaskChannel = channel;
		_lightEmissionMaskInvert = invert;
	}
	inline bg::base::Texture * lightEmissionMask() { return _lightEmissionMask.getPtr(); }
	inline const bg::base::Texture * lightEmissionMask() const { return _lightEmissionMask.getPtr(); }
	inline void setLightEmissionMaskChannel(int channel) { _lightEmissionMaskChannel = channel; }
	inline int lightEmissionMaskChannel() const { return _lightEmissionMaskChannel; }
	inline bool invertLightEmissionMask() const { return _lightEmissionMaskInvert; }
	inline void setInvertLightEmissionMask(bool invert) { _lightEmissionMaskInvert = invert; }
	
	inline void setDisplacementFactor(float f) { _displacementFactor = f; }
	inline void setDisplacementUV(int uv) { _displacementUV = uv; }
	inline void setDisplacementMap(bg::base::Texture * map) { _displacementMap = map; }
	inline void setTessellationDistances(const bg::math::Vector4 & d) { _tessellationDistances = d; }
	inline void setTessellationLevels(const bg::math::Vector4 & l) { _tessellationLevels = l; }
	
	inline float displacementFactor() const { return _displacementFactor; }
	inline int displacementUV() const { return _displacementUV; }
	inline bg::base::Texture * displacementMap() { return _displacementMap.getPtr(); }
	inline const bg::base::Texture * displacementMap() const { return _displacementMap.getPtr(); }
	inline const bg::math::Vector4 & tessellationDistances() const { return _tessellationDistances; }
	inline const bg::math::Vector4 & tessellationLevels() const { return _tessellationLevels; }
	
	inline void setReflectionMask(bg::base::Texture * rMask, int channel = 0, bool invert = false) {
		_reflectionMask = rMask;
		_reflectionMaskChannel = channel>=0 && channel<4 ? channel : 0;
		_reflectionMaskInvert = invert;
	}
	inline bg::base::Texture * reflectionMask() { return _reflectionMask.getPtr(); }
	inline const bg::base::Texture * reflectionMask() const { return _reflectionMask.getPtr(); }
	inline void setReflectionMaskChannel(int channel) { _reflectionMaskChannel = channel;  }
	inline int reflectionMaskChannel() const { return _reflectionMaskChannel;  }
	inline bool invertReflectionMask() const { return _reflectionMaskInvert; }
	inline void setInvertReflectionMask(bool invert) { _reflectionMaskInvert = invert; }
    
	inline bool unlit() const { return _unlit;  }
	inline void setUnlit(bool ul) { _unlit = ul; }
	inline float roughness() const { return _roughness; }
	inline void setRoughness(float r) { _roughness = r; }
	inline const bg::base::Texture * roughnessMask() const { return _roughnessMask.getPtr(); }
	inline bg::base::Texture * roughnessMask() { return _roughnessMask.getPtr(); }
	inline void setRoughnessMask(Texture * tex) { _roughnessMask = tex; }
	inline int roughnessMaskChannel() const { return _roughnessMaskChannel; }
	inline void setRoughnessMaskChannel(int ch) { _roughnessMaskChannel = ch; }
	inline bool invertRoughnessMask() const { return _roughnessMaskInvert; }
	inline void setInvertRoughnessMask(bool i) { _roughnessMaskInvert = i; }
	
	inline bool transparent() const { return _processAsTransparent || _diffuse.a()<1.0f; }
	
	static bg::math::Vector4 GetMaskVector(int channel) {
		if (channel<0 || channel>3) channel = 0;
		return bg::math::Vector4(channel==0 ? 1.0f:0.0f,
								 channel==1 ? 1.0f:0.0f,
								 channel==2 ? 1.0f:0.0f,
								 channel==3 ? 1.0f:0.0f);
	}
	
	inline bool cullFace() const { return _cullFace; }
	inline void setCullFace(bool cf) { _cullFace = cf; }

	inline const std::string & name() const { return _name; }
	inline void setName(const std::string & name) { _name = name; }

	inline const bool processAsTransparent() const { return _processAsTransparent; }
	inline void setProcessAsTransparent(bool t) { _processAsTransparent = t; }
	
protected:
	virtual ~Material();

	bg::math::Color _diffuse;
	bg::math::Color _specular;
	float _shininess;
	float _lightEmission;
	float _refractionAmount;
	float _reflectionAmount;
	ptr<Texture> _texture;
	ptr<Texture> _lightMap;
	ptr<Texture> _normalMap;
	bg::math::Vector2 _textureOffset;
	bg::math::Vector2 _textureScale;
	bg::math::Vector2 _lightMapOffset;
	bg::math::Vector2 _lightMapScale;
	bg::math::Vector2 _normalMapOffset;
	bg::math::Vector2 _normalMapScale;
	bool _castShadows;
	bool _receiveShadows;
	float _alphaCutoff;
	ptr<Texture> _shininessMask;
	int _shininessMaskChannel;
	bool _shininessMaskInvert;
	ptr<Texture> _lightEmissionMask;
	int _lightEmissionMaskChannel;
	bool _lightEmissionMaskInvert;
	float _displacementFactor;
	int _displacementUV;
	ptr<Texture> _displacementMap;
	bg::math::Vector4 _tessellationDistances;
	bg::math::Vector4 _tessellationLevels;
	ptr<Texture> _reflectionMask;
	int _reflectionMaskChannel;
	bool _reflectionMaskInvert;
	bool _cullFace;

	bool _unlit;
	float _roughness;
	ptr<Texture> _roughnessMask;
	int _roughnessMaskChannel;
	bool _roughnessMaskInvert;

	
	bool _processAsTransparent;
	
	std::string _name;
};

}
}

#endif
