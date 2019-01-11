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

#ifndef _bg2e_fx_ssao_hpp_
#define _bg2e_fx_ssao_hpp_

#include <bg/base/texture_effect.hpp>

#include <bg/engine/openglCore/shader.hpp>
#include <bg/tools/shader_source.hpp>


namespace bg {
namespace fx {

#define kMaxKernelOffsets 64

class BG2E_EXPORT Ssao : public bg::base::TextureEffect {
public:
	Ssao(bg::base::Context *);

	virtual void activate(bg::base::TextureVector &);
	virtual void bindPolyList(bg::base::PolyList *);
	virtual void unbind();

	inline void setProjectionMatrix(const bg::math::Matrix4 & proj) { _projectionMatrix = proj; }

	inline void setSampleRadius(float sr) { _sampleRadius = sr; }
	inline void setKernelSize(int ks) { if (ks<=kMaxKernelOffsets) _kernelSize = ks; }
	inline void setColor(const bg::math::Color & c) { _ssaoColor = c; }
	inline void setEnabled(bool e) { _enabled = e; }
	inline void setMaxDistance(float d) { _maxDistance = d; }

	inline const bg::math::Matrix4 & projectionMatrix() const { return _projectionMatrix; }
	inline float sampleRadius() const { return _sampleRadius; }
	inline int kernelSize() const { return _kernelSize; }
	inline const bg::math::Color & color() const { return _ssaoColor; }
	inline bool isEnabled() const { return _enabled; }
	inline float maxDistance() const { return _maxDistance; }

protected:
	virtual ~Ssao();

	void buildGL(bool);
	void activateGL(bool, bg::base::TextureVector &);
	void bindPolyListGL(bg::base::PolyList *, bool);
	void unbindGL(bool);

	bg::ptr<bg::engine::openglCore::Shader> _glShader;

	void getVertexShader(bg::tools::ShaderSource &);
	void getFragmentShader(bg::tools::ShaderSource &);

	// Effect parameters
	bg::math::Matrix4 _projectionMatrix;
	float _sampleRadius = 0.5f;
	int _kernelSize = 16;
	bg::math::Color _ssaoColor = bg::math::Color::Black();
	bool _enabled = true;
	float _maxDistance = 10000.0f;

	float _kernelOffsets[kMaxKernelOffsets * 3];
	int _currentKernelSize = 0;
};

}
}

#endif