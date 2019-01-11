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


#include <bg/engine.hpp>

#include <bg/engine/directx11.hpp>
#include <bg/engine/opengl_core.hpp>
#include <bg/engine/opengl_es_3.hpp>
#include <bg/engine/vulkan.hpp>

#include <bg/base/exception.hpp>

#include <bg/tools/texture_cache.hpp>

// Resolve forward declarations
#include <bg/wnd/window_controller.hpp>

namespace bg {


Engine * Engine::s_engine = nullptr;
std::vector<std::function<void()>> Engine::s_destroyCallback;

bool Engine::IsDX11() {
	return bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::DirectX11>();
}

bool Engine::IsOpenGLCore() {
	return bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLCore>();
}

bool Engine::IsOpenGLES3() {
	return bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLES3>();
}

bool Engine::IsOpenGL() {
	return	bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLCore>() ||
			bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLES3>();
}
    
bool Engine::IsVulkan() {
    return bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::Vulkan>();
}

void Engine::Init(Engine * e) {
	if (s_engine) {
		throw bg::base::InvalidStateException("Could not initialize engine. The engine is already initialized: " + s_engine->name());
	}
	s_engine = e;
}

void Engine::Destroy() {
	if (s_engine) {
		for (auto cb : s_destroyCallback) {
			cb();
		}
		s_destroyCallback.clear();
		delete s_engine;
		s_engine = nullptr;
	}
}

}



