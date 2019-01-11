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


#include <bg/fx/forward.hpp>

#include <bg/engine.hpp>
#include <bg/engine/directx11.hpp>
#include <bg/engine/opengl_core.hpp>
#include <bg/engine/opengl_es_3.hpp>

namespace bg {
	namespace fx {

Forward::Forward(bg::base::Context * ctx)
	:bg::base::Effect(ctx)
{
	bool mobile = bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLES3>();
	_lights.matrixState = &matrixState();
	_lights.context = ctx;
	if (bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::DirectX11>()) {
		// TODO: Direct3D support
		throw bg::base::EngineException("fx::Forward effect is not compatible with DirectX 11 engine.");
		//buildDX11();
	}
	else if (bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLCore>() || mobile) {
		buildGL(mobile);
	}
}

Forward::~Forward() {

}

void Forward::activate() {
	bool mobile = bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLES3>();
	if (bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::DirectX11>()) {
		activateDX11();
	}
	else if (bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLCore>() || mobile) {
		activateGL(mobile);
	}
}

void Forward::bindPolyList(bg::base::PolyList * plist) {
	bool mobile = bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLES3>();
	if (bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::DirectX11>()) {
		bindPolyListDX11(plist);
	}
	else if (bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLCore>() || mobile) {
		bindPolyListGL(plist, mobile);
	}
}

void Forward::unbind() {
	bool mobile = bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLES3>();
	if (bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::DirectX11>()) {
		unbindDX11();
	}
	else if (bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLCore>() || mobile) {
		unbindGL(mobile);
	}
}

}
}