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

#ifndef _bg2e_fx_ssrt_hpp_
#define _bg2e_fx_ssrt_hpp_

#include <bg/base/texture_effect.hpp>
#include <bg/base/definitions.hpp>

#include <bg/engine/openglCore/shader.hpp>
#include <bg/tools/shader_source.hpp>


namespace bg {
namespace fx {

#define kMaxSamples 100

class BG2E_EXPORT Ssrt : public bg::base::TextureEffect {
public:
	Ssrt(bg::base::Context *);

	// TextureVector:
	//	0: position map
	//	1: normal map
	//	2: lighting map
	//	3: material map
	virtual void activate(bg::base::TextureVector &);
	virtual void bindPolyList(bg::base::PolyList *);
	virtual void unbind();

	inline void setProjectionMatrix(const bg::math::Matrix4 & proj) { _projectionMatrix = proj; }
	inline const bg::math::Matrix4 & projectionMatrix() const { return _projectionMatrix; }
	inline void setCameraPosition(const bg::math::Vector3 & pos) { _cameraPos = pos; }
	inline const bg::math::Vector3 & cameraPosition() const { return _cameraPos; }
	inline void setQuality(bg::base::Quality q) { _qualityChanged = _quality!=q; _quality = q; }
	inline bg::base::Quality quality() const { return _quality; }

	inline bool qualityChanged() const { return _qualityChanged; }

protected:
	virtual ~Ssrt();

	void buildGL(bool);
	void activateGL(bool, bg::base::TextureVector &);
	void bindPolyListGL(bg::base::PolyList *, bool);
	void unbindGL(bool);

	bg::ptr<bg::engine::openglCore::Shader> _glShader;

	void getVertexShader(bg::tools::ShaderSource &);
	void getFragmentShader(bg::tools::ShaderSource &);

	// Effect parameters
	bg::math::Matrix4 _projectionMatrix;
	bg::math::Vector3 _cameraPos;
	bg::base::Quality _quality;
	bool _qualityChanged = false;
	int _frameIndex = 0;
};

}
}

#endif