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


#include <bg/shader/color_correction.hpp>

#include <bg/engine.hpp>
#include <bg/engine/directx11.hpp>
#include <bg/engine/opengl_core.hpp>
#include <bg/engine/opengl_es_3.hpp>

#include <bg/base/exception.hpp>

namespace bg {
namespace shader {

ColorCorrection::ColorCorrection(bg::tools::ShaderSource * src)
	:Library(src)
{
	if(bg::Engine::IsDX11()) {
		throw bg::base::EngineException("bg::shader::ColorCorrection not implemented for DirectX 11 engine");
	}
}

ColorCorrection & ColorCorrection::colorCorrection() {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;
	shaderSource().beginFunction(kFloat4, "colorCorrection", {
		Parameter(kFloat4, "fragColor"),
		Parameter(kFloat, "hue"),
		Parameter(kFloat, "saturation"),
		Parameter(kFloat, "lightness"),
		Parameter(kFloat, "brightness"),
		Parameter(kFloat, "contrast")
	});

	if(bg::Engine::IsOpenGL()) {
		shaderSource().functionBody({
			"return applyContrast(applyBrightness(applySaturation(fragColor,hue,saturation,lightness),brightness),contrast);"
		});
	}

	shaderSource().endFunction();
	return *this;
}

ColorCorrection & ColorCorrection::applyContrast() {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;
	shaderSource().beginFunction(kFloat4, "applyContrast", {
		Parameter(kFloat4, "color"),
		Parameter(kFloat, "contrast")
	});

	if(bg::Engine::IsOpenGL()) {
		shaderSource().functionBody({ "return clamp(vec4((color.rgb * max(contrast + 0.5,0.0)),1.0),0.0,1.0);" });
	}

	shaderSource().endFunction();
	return *this;
}

ColorCorrection & ColorCorrection::applyBrightness() {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;
	shaderSource().beginFunction(kFloat4, "applyBrightness", {
		Parameter(kFloat4, "color"),
		Parameter(kFloat, "brightness")
	});

	if(bg::Engine::IsOpenGL()) {
		shaderSource().functionBody({ "return clamp(vec4(color.rgb + brightness - 0.5,1.0),0.0,1.0);" });
	}

	shaderSource().endFunction();
	return *this;
}

ColorCorrection & ColorCorrection::applySaturation() {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;
	shaderSource().beginFunction(kFloat4, "applySaturation", {
		Parameter(kFloat4, "color"),
		Parameter(kFloat, "hue"),
		Parameter(kFloat, "saturation"),
		Parameter(kFloat, "lightness")
	});

	if(bg::Engine::IsOpenGL()) {
		shaderSource().functionBody({
			"vec3 fragRGB = clamp(color.rgb + vec3(0.001), 0.0, 1.0);",
			"vec3 fragHSV = rgb2hsv(fragRGB);",
			"lightness -= 0.01;",
			"float h = hue;",
			"fragHSV.x *= h;",
			"fragHSV.yz *= vec2(saturation, lightness);",
			"fragHSV.x = mod(fragHSV.x, 1.0);",
			"fragHSV.y = mod(fragHSV.y, 1.0);",
			"fragHSV.z = mod(fragHSV.z, 1.0);",
			"return clamp(vec4(hsv2rgb(fragHSV), color.w), 0.0, 1.0);"
		});
	}

	shaderSource().endFunction();
	return *this;
}

ColorCorrection & ColorCorrection::rgb2hsv() {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;
	shaderSource().beginFunction(kFloat3, "rgb2hsv", {
		Parameter(kFloat3, "c")
	});

	if(bg::Engine::IsOpenGL()) {
		shaderSource().functionBody({
			"vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);",
			"vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));",
			"vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));",

			"float d = q.x - min(q.w, q.y);",
			"float e = 1.0e-10;",
			"return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);"
		});
	}

	shaderSource().endFunction();
	return *this;
}

ColorCorrection & ColorCorrection::hsv2rgb() {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;
	shaderSource().beginFunction(kFloat3, "hsv2rgb", {
		Parameter(kFloat3, "c")
	});

	if(bg::Engine::IsOpenGL()) {
		shaderSource().functionBody({
			"vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);",
			"vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);",
			"return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);"
		});
	}

	shaderSource().endFunction();
	return *this;
}

}
}
