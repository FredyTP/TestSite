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
#include <bg/math/matrix.hpp>
#include <bg/math/vector.hpp>
#include <bg/base/texture.hpp>

namespace bg {
namespace fx {

	namespace forwardDx11 {

		static std::string vertexShader = "\
cbuffer MatrixBuffer {\n\
	matrix worldMatrix;\n\
	matrix viewMatrix;\n\
	matrix projectionMatrix;\n\
};\n\
\n\
struct VertexInputType {\n\
	float4 position : POSITION;\n\
	float3 normal : NORMAL;\n\
	float2 tex0 : TEXCOORD0;\n\
	float2 tex1 : TEXCOORD1;\n\
	float3 tangent : TANGENT;\n\
};\n\
\n\
struct PixelInputType {\n\
	float4 position: SV_POSITION;\n\
	float2 tex0: TEXCOORD0;\n\
	float2 tex1: TEXCOORD1;\n\
};\n\
\n\
PixelInputType BasicVertexShader(VertexInputType input) {\n\
	PixelInputType output;\n\
	input.position.w = 1.0f;\n\
	output.position = mul(mul(mul(projectionMatrix, viewMatrix), worldMatrix), input.position);\n\
	\n\
	output.tex0 = input.tex0;\n\
	output.tex1 = input.tex1;\n\
	return output;\n\
}";

		static std::string pixelShader = "\
Texture2D shaderTexture;\n\
SamplerState sampleType;\n\
\n\
cbuffer PixelBuffer {\n\
	float4 diffuse;\n\
	float4 specular;\n\
	//float shininess;\n\
	float2 texOffset;\n\
	float2 texScale;\n\
	//float alphaCutoff;\n\
	float4 l_ambient;\n\
	float4 l_diffuse;\n\
	//float4 l_specular;\n\
	//float3 l_attenuation;\n\
	//float3 l_direction;\n\
};\n\
\n\
struct PixelInputType {\n\
	float4 position : SV_POSITION;\n\
	float2 tex0 : TEXCOORD0;\n\
	float2 tex1: TEXCOORD1;\n\
};\n\
\n\
float4 GetLighting(float4 ambient, float4 matDiffuse) {\n\
	return ambient * matDiffuse;\n\
}\n\
\n\
float4 BasicPixelShader(PixelInputType input) : SV_TARGET {\n\
	float4 diffuseColor = shaderTexture.Sample(sampleType, input.tex0 * texScale + texOffset);\n\
	if (diffuseColor.w>=0.5/*alphaCutoff*/) {\n\
		float4 lighting = GetLighting(l_ambient, diffuse);\n\
		return diffuseColor * lighting;\n\
	}\n\
	else {\n\
		discard;\n\
		return float4(0.0,0.0,0.0,0.0);\n\
	}\n\
}";

struct MatrixBuffer {
	bg::math::Matrix4 modelMatrix;
	bg::math::Matrix4 viewMatrix;
	bg::math::Matrix4 projectionMatrix;
};

struct PixelBuffer {
	// Material
	bg::math::Color diffuse;
	bg::math::Color specular;
	//float shininess;
	bg::math::Vector2 texOffset;
	bg::math::Vector2 texScale;
	//float alphaCutoff;

	// Lighting
	bg::math::Color l_ambient;
	bg::math::Color l_diffuse;
	//bg::math::Color l_specular;
	//bg::math::Vector3 l_attenuation;
	//bg::math::Vector3 l_direction;
};


}

void Forward::buildDX11() {
	using namespace bg::engine::directx11;
	using namespace forwardDx11;

	_dxVertexShader = new VertexShader(context());
	_dxVertexShader->addInputLayout(BufferRole::kRoleVertex);
	_dxVertexShader->addInputLayout(BufferRole::kRoleNormal);
	_dxVertexShader->addInputLayout(BufferRole::kRoleTex0);
	_dxVertexShader->addInputLayout(BufferRole::kRoleTex1);
	_dxVertexShader->addInputLayout(BufferRole::kRoleTangent);
	_dxVertexShader->addConstantBuffer(new ConstantBuffer(context(), sizeof(MatrixBuffer), BufferUsage::kUsageDynamic));
	_dxVertexShader->create(vertexShader, "BasicVertexShader");

	_dxPixelShader = new PixelShader(context());
	_dxPixelShader->addConstantBuffer(new ConstantBuffer(context(), sizeof(PixelBuffer), BufferUsage::kUsageDynamic));
	_dxPixelShader->create(pixelShader, "BasicPixelShader");
}

void Forward::activateDX11() {
	using namespace bg::engine::directx11;
	using namespace forwardDx11;

	ConstantBuffer * matrixBuffer = _dxVertexShader->constantBuffers().front().getPtr();

	MatrixBuffer * dataPtr = matrixBuffer->beginMap<MatrixBuffer>();
	dataPtr->modelMatrix = matrixState().modelMatrixStack().matrix();
	dataPtr->viewMatrix = matrixState().viewMatrixStack().matrix();
	dataPtr->projectionMatrix = matrixState().projectionMatrixStack().matrix();
	matrixBuffer->endMap(0);

	ConstantBuffer * pixelBuffer = _dxPixelShader->constantBuffers().front().getPtr();
	PixelBuffer * pixelPtr = pixelBuffer->beginMap<PixelBuffer>();
	pixelPtr->diffuse = material().diffuse();
	pixelPtr->specular = material().specular();
	//pixelPtr->shininess = material().shininess();
	pixelPtr->texOffset = material().textureOffset();
	pixelPtr->texScale = material().textureScale();
	//pixelPtr->alphaCutoff = material().alphaCutoff();

	//pixelPtr->l_ambient = light()->ambient();
	//pixelPtr->l_diffuse = light()->diffuse();
	//pixelPtr->l_specular = light()->specular();
	//pixelPtr->l_attenuation = light()->attenuationVector();
	//pixelPtr->l_direction = light()->direction();

	pixelBuffer->endMap(0);

	_dxPixelShader->setTexture(0, 1, material().texture());

	_dxVertexShader->activate();
	_dxPixelShader->activate();
}

void Forward::bindPolyListDX11(bg::base::PolyList * plist) {
	_dxVertexShader->bindPolyList(plist);
}

void Forward::unbindDX11() {}

}
}
