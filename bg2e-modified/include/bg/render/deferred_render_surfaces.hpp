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


#include <bg/base/context_object.hpp>
#include <bg/base/render_surface.hpp>
#include <bg/base/definitions.hpp>

namespace bg {
namespace render {

class BG2E_EXPORT DeferredRenderSurfaces : public bg::base::ContextObject,
										   public bg::base::ReferencedPointer {
public:
	enum SurfaceType {
		kTypeUndefined = 0,
		kTypeOpaque,
		kTypeTransparent
	};
	
	typedef bg::base::TextureRenderSurface * Surface;
	typedef bg::ptr<bg::base::TextureRenderSurface> SurfacePtr;

	
	DeferredRenderSurfaces(bg::base::Context *, bool memoryOptimized = false);
	
	void createOpaqueSurfaces();
	void createTransparentSurfaces(DeferredRenderSurfaces * deferredSurfaces);
	inline SurfaceType type() const { return _type; }
	
	void resize(const bg::math::Size2Di &, bg::base::Quality ssaoQuality = bg::base::kQualityExtreme, float ssrtScale = 1.0f);

	void invalidateBuffers();
	void invalidateTextures();

	inline Surface gbufferUByteSurface() { return _gbufferUByteSurface.getPtr(); }
	inline Surface gbufferFloatSurface() { return _gbufferFloatSurface.getPtr(); }
	inline Surface lightingSurface() { return _lightingSurface.getPtr(); }
	inline Surface shadowSurface() { return _shadowSurface.getPtr(); }
	inline Surface ssaoSurface() { return _ssaoSurface.getPtr(); }
	inline Surface ssrtSurface() { return _ssrtSurface.getPtr(); }
	inline Surface mixSurface() { return _mixSurface.getPtr(); }
	
	inline bg::base::Texture * diffuse() { return _memoryOptimized ? _gbufferUByteSurface->texture(0) : _gbufferFloatSurface->texture(0); }
	inline bg::base::Texture * specular() { return _memoryOptimized ? _gbufferUByteSurface->texture(1) : _gbufferFloatSurface->texture(1); }
	inline bg::base::Texture * normal() { return _memoryOptimized ? _gbufferUByteSurface->texture(2) : _gbufferFloatSurface->texture(2); }
	inline bg::base::Texture * material() { return _memoryOptimized ? _gbufferUByteSurface->texture(3) : _gbufferFloatSurface->texture(3); }
	inline bg::base::Texture * position() { return _memoryOptimized ? _gbufferFloatSurface->texture(0) : _gbufferFloatSurface->texture(4); }
	inline bg::base::Texture * lighting() { return _lightingSurface->texture(); }
	inline bg::base::Texture * shadow() { return _shadowSurface->texture(); }
	inline bg::base::Texture * ambientOcclusion() { return _ssaoSurface->texture(); }
	inline bg::base::Texture * reflection() { return _ssrtSurface->texture(); }
	inline bg::base::Texture * mix() { return _mixSurface->texture(); }
	inline bg::base::Texture * reflectionColor() { return _type==kTypeOpaque ? lighting() : _opaqueSurfaces->lighting(); }
	inline bg::base::Texture * reflectionDepth() { return _type==kTypeOpaque ? position() : _opaqueSurfaces->position(); }
	inline bg::base::Texture * mixDepthMap() { return _type==kTypeOpaque ? nullptr : _opaqueSurfaces->position(); }
	inline bg::base::Texture * shininess() { return _lightingSurface->texture(1); }
	
protected:
	virtual ~DeferredRenderSurfaces();
	
	bool _memoryOptimized;
	SurfaceType _type;
	
	SurfacePtr _gbufferUByteSurface;
	SurfacePtr _gbufferFloatSurface;
	SurfacePtr _lightingSurface;
	SurfacePtr _shadowSurface;
	SurfacePtr _ssaoSurface;
	SurfacePtr _mixSurface;
	
	// Used only in opaque surfaces
	SurfacePtr _ssrtSurface;
	
	// Used only in transparent surfaces
	DeferredRenderSurfaces * _opaqueSurfaces;
	
	void createCommon();
};

}
}

