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

#ifndef _bg2e_engine_hpp_
#define _bg2e_engine_hpp_

#include <bg/export.hpp>
#include <bg/wnd/context.hpp>

#include <bg/engine/pipeline_impl.hpp>
#include <bg/base/pipeline.hpp>
#include <bg/engine/poly_list_impl.hpp>
#include <bg/base/poly_list.hpp>
#include <bg/engine/texture_impl.hpp>
#include <bg/engine/render_surface_impl.hpp>
#include <bg/engine/effect_impl.hpp>
#include <bg/engine/command_queue_impl.hpp>

// Resolve forward declarations on implementation class headers
#include <bg/base/pipeline.hpp>
#include <bg/base/poly_list.hpp>
#include <bg/base/texture.hpp>
#include <bg/base/render_surface.hpp>

#include <string>
#include <vector>
#include <functional>

namespace bg {

enum EngineFeature {
	kMultibuffer		= 0x1 << 0,		// Can generate multiple color buffers in fragment shaders
	kTessellation		= 0x1 << 1,		// Supports tessellation shaders
	kFloatTextures		= 0x1 << 2,		// Supports floating point textures
	kDepthTextures		= 0x1 << 3,
	kFloatTexture32		= 0x1 << 4,		// 32 bits float textures
	kFloatTexture16		= 0x1 << 5		// 16 bits float textures
};

class BG2E_EXPORT Engine {
public:

	static bool IsDX11();
	static bool IsOpenGLCore();
	static bool IsOpenGLES3();
	static bool IsOpenGL();	// Equivalent to IsOpenGLCore() || IsOpenGLES3()
    static bool IsVulkan();

	static void Init(Engine * e);
	static Engine * Get() { return s_engine; }
	static void Destroy();
	
	template <class T>
	static size_t Identifier() { return typeid(T).hash_code(); }

	inline size_t identifier() const { return typeid(*this).hash_code(); }
	inline const std::string & name() const { return _name; }
	
	virtual void initialize(bg::base::Context *) = 0;

	virtual bool supportsFeature(bg::base::Context *, EngineFeature feature) = 0;
	
	virtual bg::engine::PipelineImpl * createPipeline(bg::base::Pipeline *) = 0;
	virtual bg::engine::PolyListImpl * createPolyList(bg::base::PolyList *) = 0;
	virtual bg::engine::TextureImpl * createTextureImpl(bg::base::Texture *) = 0;
	virtual bg::engine::RenderSurfaceImpl * createRenderSurfaceImpl(bg::base::RenderSurface *, engine::RenderSurfaceImpl::RenderSurfaceTarget target) = 0;
    virtual bg::engine::EffectImpl * createEffectImpl(bg::base::Effect *) { return nullptr; }
    virtual bg::engine::CommandQueueImpl * createCommandQueueImpl(bg::base::CommandQueue *) { return nullptr; }
	
	/*
	// TODO: Implementation classes
	inline TextureImpl * texture() { return _textureImpl; }
	inline const TextureImpl * texture() const { return _textureImpl; }
	
	inline ShaderSourceImpl * shaderSource() { return _shaderSourceImpl; }
	inline const ShaderSourceImpl * shaderSource() const { return _shaderSourceImpl; }
	inline ColorBufferImpl * colorBuffer() { return _colorBufferImpl; }
	inline const ColorBufferImpl * colorBuffer() const { return _colorBufferImpl; }
	inline TetureBufferImpl * textureBuffer() { return _textureBufferImpl; }
	inline const TetureBufferImpl * textureBuffer() const { return _textureBufferImpl; }
	*/
	
	static void AddDestroyCallback(std::function<void()> cb) { s_destroyCallback.push_back(cb); }
	
protected:
	virtual ~Engine() {}
	static Engine * s_engine;
	
	std::string _name;
		
	/*
	 // TODO: Implementation classes
	TextureImpl * _textureImpl;
	
	PolyListImpl * _polyListImpl;
	ShaderImpl * _shaderImpl;
	ShaderSourceImpl * _shaderSourceImpl;
	ColorBufferImpl * _colorBufferImpl;
	TextureBufferImpl * _textureBufferImpl;
	 */
	
	 static std::vector<std::function<void()>> s_destroyCallback;
};
	
}

#endif	// _bg2e_export_hpp_


