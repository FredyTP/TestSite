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


#include <bg/fx/texture.hpp>
#include <bg/math/matrix.hpp>
#include <bg/math/vector.hpp>
#include <bg/base/texture.hpp>

namespace bg {
namespace fx {

namespace textureDx11 {

static std::string vertexShader = "\
struct VertexInputType {\n\
	float4 position : POSITION;\n\
	float2 tex0 : TEXCOORD0;\n\
};\n\
\n\
struct PixelInputType {\n\
	float4 position: SV_POSITION;\n\
	float2 tex0: TEXCOORD0;\n\
};\n\
\n\
PixelInputType TextureVertexShader(VertexInputType input) {\n\
	PixelInputType output;\n\
	input.position.w = 1.0f;\n\
	output.position = input.position;\n\
	\n\
	output.tex0 = input.tex0;\n\
	return output;\n\
}";

static std::string pixelShader = "\
Texture2D shaderTexture;\n\
SamplerState sampleType;\n\
\n\
struct PixelInputType {\n\
	float4 position : SV_POSITION;\n\
	float2 tex0 : TEXCOORD0;\n\
};\n\
\n\
float4 TexturePixelShader(PixelInputType input) : SV_TARGET {\n\
	return shaderTexture.Sample(sampleType, input.tex0);\n\
}";

}

void Texture::buildDX11() {
	using namespace bg::engine::directx11;
	using namespace bg::fx::textureDx11;

	_dxVertexShader = new VertexShader(context());
	_dxVertexShader->addInputLayout(BufferRole::kRoleVertex);
	_dxVertexShader->addInputLayout(BufferRole::kRoleTex0);
	_dxVertexShader->create(vertexShader, "TextureVertexShader");

	_dxPixelShader = new PixelShader(context());
	_dxPixelShader->create(pixelShader, "TexturePixelShader");
}

void Texture::activateDX11(bg::base::Texture * tex) {
	using namespace bg::engine::directx11;

	_dxPixelShader->setTexture(0, 1, tex);

	_dxVertexShader->activate();
	_dxPixelShader->activate();
}

}
}
