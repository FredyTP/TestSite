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


#include <bg/shader/utils.hpp>

#include <bg/engine.hpp>

#include <bg/base/exception.hpp>

namespace bg {
namespace shader {

static const int sMaxBlurIterations = 40;

Utils::Utils(bg::tools::ShaderSource * src)
	:Library(src)
{
	if (bg::Engine::IsDX11()) {
		throw bg::base::EngineException("bg::shader::Utils not implemented for DirectX 11 engine");
	}
}

Utils & Utils::samplerColor() {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;
	shaderSource().beginFunction(kFloat4, "samplerColor",{
		Parameter(kSampler2D, "sampler"),
		Parameter(kFloat2, "uv"),
		Parameter(kFloat2, "offset"),
		Parameter(kFloat2, "scale")
	});

	if (bg::Engine::IsOpenGL()) {
		shaderSource().functionBody({ "return texture(sampler, uv * scale + offset);" });
	}

	shaderSource().endFunction();

	return *this;
}

Utils & Utils::samplerNormal() {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;
	shaderSource().beginFunction(kFloat3, "samplerNormal", {
		Parameter(kSampler2D, "sampler"),
		Parameter(kFloat2, "uv"),
		Parameter(kFloat2, "offset"),
		Parameter(kFloat2, "scale")
	});

	if(bg::Engine::IsOpenGL()) {
		shaderSource().functionBody({ "return normalize(samplerColor(sampler,uv,offset,scale).xyz * 2.0 - 1.0);" });
	}

	shaderSource().endFunction();
	return *this;
}

Utils & Utils::combineNormalWithMap() {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;
	shaderSource().beginFunction(kFloat3, "combineNormalWithMap",{
		Parameter(kFloat3, "normalCoord"),
		Parameter(kFloat3, "tangent"),
		Parameter(kFloat3, "bitangent"),
		Parameter(kFloat3, "normalMapValue")
	});

	if(bg::Engine::IsOpenGL()) {
		shaderSource().functionBody({
			"mat3 tbnMat = mat3( tangent.x, bitangent.x, normalCoord.x,",
			"					tangent.y, bitangent.y, normalCoord.y,",
			"					tangent.z, bitangent.z, normalCoord.z);",
			"return normalize(normalMapValue * tbnMat);"
		});
	}

	shaderSource().endFunction();
	return *this;
}

Utils & Utils::applyTextureMask() {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;
	shaderSource().beginFunction(kFloat, "applyTextureMask",{
		Parameter(kFloat, "value"),
		Parameter(kSampler2D, "textureMask"),
		Parameter(kFloat2, "uv"),
		Parameter(kFloat2, "offset"),
		Parameter(kFloat2, "scale"),
		Parameter(kFloat4, "channelMask"),
		Parameter(kBool, "invert")
	});

	if(bg::Engine::IsOpenGL()) {
		shaderSource().functionBody({
			"float mask;",
			"vec4 color = samplerColor(textureMask,uv,offset,scale);",
			"mask = color.r * channelMask.r +"
			"	   color.g * channelMask.g +"
			"	   color.b * channelMask.b +"
			"	   color.a * channelMask.a;",
			"if (invert) {",
			"	mask = 1.0 - mask;",
			"}",
			"return value * mask;"
		});
	}

	shaderSource().endFunction();
	return *this;
}

Utils & Utils::specularColor() {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;
	shaderSource().beginFunction(kFloat4, "specularColor",{
		Parameter(kFloat4, "specular"),
		Parameter(kSampler2D, "shininessMask"),
		Parameter(kFloat2, "uv"),
		Parameter(kFloat2, "offset"),
		Parameter(kFloat2, "scale"),
		Parameter(kFloat4, "channelMask"),
		Parameter(kBool, "invert")
	});

	if(bg::Engine::IsOpenGL()) {
		shaderSource().functionBody({
			"float maskValue = applyTextureMask(1.0, shininessMask,uv,offset,scale,channelMask,invert);",
			"",
			"return vec4(specular.rgb * maskValue, 1.0);"
		});
	}

	shaderSource().endFunction();
	return *this;
}

Utils & Utils::pack() {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;
	shaderSource().beginFunction(kFloat4, "pack", {
		Parameter(kFloat, "value")
	});

	if(bg::Engine::IsOpenGL()) {
		shaderSource().functionBody({
			"	const vec4 bitSh = vec4(256.0 * 256.0 * 256.0, 256.0 * 256.0, 256.0, 1.0);",
			"	const vec4 bitMsk = vec4(0.0, 1.0 / 256.0, 1.0 / 256.0, 1.0 / 256.0);",
			"	vec4 comp = fract(value * bitSh);",
			"	comp -= comp.xxyz * bitMsk;",
			"	return comp;"
		});
	}

	shaderSource().endFunction();
	return *this;
}

Utils & Utils::unpack() {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;
	shaderSource().beginFunction(kFloat, "unpack", {
		Parameter(kFloat4, "color")
	});

	if(bg::Engine::IsOpenGL()) {
		shaderSource().functionBody({
			"const vec4 bitShifts = vec4(1.0 / (256.0 * 256.0 * 256.0), 1.0 / (256.0 * 256.0), 1.0 / 256.0, 1.0);",
			"",
			"return dot(color, bitShifts);",
		});
	}

	shaderSource().endFunction();
	return *this;
}

Utils & Utils::random() {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;
	shaderSource().beginFunction(kFloat, "random", {
		Parameter(kFloat, "seed")
	});

	if(bg::Engine::IsOpenGL()) {
		shaderSource().functionBody({
			"vec4 seed4 = vec4(seed);",
			"float dot_product = dot(seed4, vec4(12.9898, 78.233, 45.164, 94.673));",
			"return fract(sin(dot_product) * 43758.5453);",
		});
	}

	shaderSource().endFunction();
	return *this;
}

Utils & Utils::blur() {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;
	shaderSource().beginFunction(kFloat4, "blur", {
		Parameter(kSampler2D, "tex"),
		Parameter(kFloat2, "texCoord"),
		Parameter(kInt, "radius"),
		Parameter(kFloat2, "viewSize")
	});

	if(bg::Engine::IsOpenGL()) {
		shaderSource().functionBody({
			"vec2 texelSize = 1.0 / viewSize;",
			"vec3 result = vec3(0.0);",
			"vec2 hlim = vec2(float(-radius) * 0.5 + 0.5);",
			"for (int x=0; x<radius; ++x) {",
			"	for (int y=0; y<radius; ++y) {",
			"		vec2 offset = (hlim + vec2(float(x), float(y))) * texelSize;",
			"		result += texture(tex, texCoord + offset).rgb;",
			"	}",
			"}",
			"return vec4(result / float(radius * radius), 1.0);"
		});
	}

	shaderSource().endFunction();
	return *this;
}

Utils & Utils::bloom() {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;
	shaderSource().beginFunction(kFloat4, "bloom", {
		Parameter(kSampler2D, "tex"),
		Parameter(kFloat2, "texCoord"),
		Parameter(kInt, "radius"),
		Parameter(kFloat2, "viewSize")
	});

	if(bg::Engine::IsOpenGL()) {
		shaderSource().functionBody({
			"vec2 texelSize = 1.0 / viewSize;",
			"vec3 result = vec3(0.0);",
			"vec2 hlim = vec2(float(-radius) * 0.5 + 0.5, 0.0);",
			"for (int x=0; x<radius; ++x) {",
			"	vec2 offset = (hlim + vec2(float(x), 0.0)) * texelSize;",
			"	result += texture(tex, texCoord + offset).rgb;",
			"}",
			"return vec4(result / float(radius), 1.0);"
		});
	}

	shaderSource().endFunction();
	return *this;
}

Utils & Utils::textureDownsample() {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;
	shaderSource().beginFunction(kFloat4, "textureDownsample", {
		Parameter(kSampler2D, "textureInput"),
		Parameter(kFloat2, "texCoord"),
		Parameter(kFloat2, "size"),
		Parameter(kFloat2, "reduction")
	});

	if (bg::Engine::IsOpenGL()) {
		shaderSource().functionBody({
			"float dx = reduction.x / size.x;",
			"float dy = reduction.y / size.y;",
			"vec2 coord = vec2(dx * texCoord.x / dx, dy * texCoord.y / dy);",
			"return texture(textureInput,coord);"
		});
	}

	shaderSource().endFunction();
	return *this;
}

Utils & Utils::blurDownsample() {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;
	shaderSource().beginFunction(kFloat4, "blurDownsample", {
		Parameter(kSampler2D, "textureInput"),
		Parameter(kFloat2, "texCoord"),
		Parameter(kInt, "size"),
		Parameter(kFloat2, "samplerSize"),
		Parameter(kInt, "downsample")
	});

	if (bg::Engine::IsOpenGL()) {
		shaderSource().functionBody({
			"vec2 texelSize = 1.0 / samplerSize;",
			"vec3 result = vec3(0.0);",
			"size = int(max(float(size / downsample),1.0));",
			"vec2 hlim = vec2(float(-size) * 0.5 + 0.5);",
			"vec2 sign = vec2(1.0);",
			"float blurFactor = 10.0 - 0.2 * float(size) * log(float(size));",
			"for (int x = 0; x<" + std::to_string(sMaxBlurIterations) + "; ++x) {",
			"	if (x == size) break;",
			"	for (int y = 0; y<" + std::to_string(sMaxBlurIterations) + "; ++y) {",
			"		if (y == size) break;",
			"		vec2 offset = (hlim + vec2(float(x), float(y))) * texelSize * float(downsample) / blurFactor;",
			"		result += textureDownsample(textureInput, texCoord + offset,samplerSize,vec2(downsample)).rgb;",
			"	}",
			"}",
			"return vec4(result / float(size * size), 1.0);"
		});
	}

	shaderSource().endFunction();
	return *this;
}

Utils & Utils::blurCube() {
    using namespace bg::tools;
    using namespace bg::tools::shaderTypes;
    shaderSource().beginFunction(kFloat4, "blurCube", {
        Parameter(kSamplerCubemap, "textureInput"),
        Parameter(kFloat3, "texCoord"),
        Parameter(kInt, "size"),
        Parameter(kFloat2, "samplerSize"),
        Parameter(kFloat, "dist")
    });
    
    if (bg::Engine::IsOpenGL()) {
        shaderSource().functionBody({
            "int downsample = int(max(1.0,dist));",
            "vec2 texelSize = 1.0 / samplerSize;",
            "vec3 result = vec3(0.0);",
            "size = int(max(float(size / downsample),1.0));",
            "vec2 hlim = vec2(float(-size) * 0.5 + 0.5);",
            "vec2 sign = vec2(1.0);",
            "for (int x=0; x<40; ++x) {",
            "    if (x==size) break;",
            "    for (int y=0; y<40; ++y) {",
            "        if (y==size) break;",
            "        vec3 offset = vec3((hlim + vec2(float(x*downsample), float(y*downsample))) * texelSize,0.0);",
            "        result += texture(textureInput, texCoord + offset,2.0).rgb;",
            "    }",
            "}",
            "return vec4(result / float(size * size), 1.0);"
            /*
            "vec2 texelSize = 1.0 / samplerSize;",
            "vec3 result = vec3(0.0);",
            "size = int(max(float(size / downsample),1.0));",
            "vec2 hlim = vec2(float(-size) * 0.5 + 0.5);",
            "vec2 sign = vec2(1.0);",
            "float blurFactor = 10.0 - 0.2 * float(size) * log(float(size));",
            "for (int x = 0; x<" + std::to_string(sMaxBlurIterations) + "; ++x) {",
            "    if (x == size) break;",
            "    for (int y = 0; y<" + std::to_string(sMaxBlurIterations) + "; ++y) {",
            "        if (y == size) break;",
            "        vec2 offset = (hlim + vec2(float(x), float(y))) * texelSize * float(downsample) / blurFactor;",
            "        result += textureDownsample(textureInput, texCoord + offset,samplerSize,vec2(downsample)).rgb;",
            "    }",
            "}",
            "return vec4(result / float(size * size), 1.0);"*/
        });
    }
    
    shaderSource().endFunction();
    return *this;
}
    
}
}
