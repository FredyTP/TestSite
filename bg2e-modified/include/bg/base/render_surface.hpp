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

#ifndef _bg2e_base_render_surface_hpp_
#define _bg2e_base_render_surface_hpp_

#include <bg/export.hpp>
#include <bg/base/context_object.hpp>
#include <bg/base/texture.hpp>
#include <bg/math/vector.hpp>
#include <bg/engine/render_surface_impl.hpp>


namespace bg {
namespace base {

class BG2E_EXPORT RenderSurface : public ContextObject, public ReferencedPointer {
public:
	enum RenderSurfaceType {
		kTypeUndefined = 0,
		kTypeColor = 1,
		kTypeDepth
	};
	
	enum RenderSurfaceFormat {
		kFormatUndefined = 0,
		kFormatUnsignedByte = 1,
		kFormatUnsignedShort,
		kFormatFloat,
		kFormatScreen
	};
	
	struct ColorPlane {
		RenderSurfaceType type;
		RenderSurfaceFormat format;
		
		ColorPlane() :type(kTypeColor), format(kFormatUnsignedByte) {}
		ColorPlane(RenderSurfaceType t, RenderSurfaceFormat f) :type(t), format(f) {}
		
		inline bool operator==(const ColorPlane & other) const { return type==other.type && format==other.format; }
		inline bool operator!=(const ColorPlane & other) const { return type!=other.type || format!=other.format; }
		inline void operator=(const ColorPlane & other) { type=other.type; format=other.format; }
	};
	
	typedef std::vector<ColorPlane> ColorPlaneSetup;

	static void GetTypeAndFormatString(ColorPlane colorPlane, std::string & type, std::string & format);
	
	/*
	 *	Returns true if the engine supports the supplied
	 *	render surface format
	 */
	bool supportFormat(RenderSurfaceFormat fmt);
	
	/*
	 *	Returns true if the engine supports the specified
	 *	color output.
	 */
	bool supportType(RenderSurfaceType type);
	
	/*
	 *	Returns the maximum number of color outputs that
	 *	can handle the current engine. For example, this
	 *	value is equivalent to the GL_MAX_COLOR_ATTACHMENTS
	 *	in OpenGL
	 */
	int maxOutputColorPlanes();

	/*
	 *	By calling this function, we notify the underlying rendering engine
	 *	that we don't want to use the content of the sufrace anymore. You can use
	 *	this function to invalidate a texture surface at the end of frame rendering
	 *	to optimize the rendering speed using multiple GPUs (SLI, Crossfire), because
	 *	this action will mark the textures as invalid, and will not be sinchronized
	 *	between the different GPUs
	 */
	void invalidateSurface();

	// The same for internal textures.
	void invalidateTextures();
	
	void setSize(const bg::math::Size2Di &);
	const bg::math::Size2Di & size() const;
	
	inline void setResizeOnViewportChanged(bool r) { _resizeOnViewportChanged = r; }
	inline bool resizeOnViewportChanged() const { return _resizeOnViewportChanged; }
	
	void create();
	void create(const ColorPlaneSetup & setup);

	void setActive();
	
	unsigned char * readPixels(const bg::math::Viewport &);
	
	void destroy();
	
	template <class T>
	T * impl() { return dynamic_cast<T*>(_impl.getPtr()); }
	
protected:
	// Crete the render surface using the specialced classes
	RenderSurface(Context * ctx);
	
	virtual ~RenderSurface();
	
	
	ptr<bg::engine::RenderSurfaceImpl> _impl;
	bool _resizeOnViewportChanged = true;
};

class BG2E_EXPORT ScreenRenderSurface : public RenderSurface {
public:
	ScreenRenderSurface(Context * ctx);
	
protected:
	virtual ~ScreenRenderSurface();
};

class BG2E_EXPORT TextureRenderSurface : public RenderSurface {
public:
	TextureRenderSurface(Context * ctx);
	
	Texture * texture(int colorPlane = 0);
	
protected:
	virtual ~TextureRenderSurface();
};

}
}
#endif
