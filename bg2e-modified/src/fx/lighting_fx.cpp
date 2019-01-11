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


#include <bg/fx/lighting.hpp>

#include <bg/engine/opengl_core.hpp>
#include <bg/engine/opengl_es_3.hpp>
#include <bg/engine/openglCore/shader.hpp>
#include <bg/engine/openglEs/shader.hpp>

#include <bg/tools/texture_cache.hpp>
#include <bg/tools/shader_source.hpp>

#include <bg/shader/utils.hpp>
#include <bg/shader/lighting.hpp>

#include <bg/math/matrix.hpp>

namespace bg {
namespace fx {

Lighting::Lighting(bg::base::Context * ctx)
	:TextureEffect(ctx)
{
	bool mobile = bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLES3>();
	if(bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLCore>() || mobile) {
		buildGL(mobile);
	}
	else {
		throw bg::base::EngineException("fx::Lighting effect is not compatible with the current rendering API.");
	}
}

Lighting::~Lighting() {
}

void Lighting::activate(bg::base::TextureRenderSurface * surface) {
	bool mobile = bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLES3>();
	if (bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLCore>() || mobile) {
		activateGL(mobile, surface);
	}
}

void Lighting::bindPolyList(bg::base::PolyList * plist) {
	bool mobile = bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLES3>();
	if(bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLCore>() || mobile) {
		bindPolyListGL(plist, mobile);
	}
}

void Lighting::unbind() {
	bool mobile = bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLES3>();
	if(bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLCore>() || mobile) {
		unbindGL(mobile);
	}
}

void Lighting::buildGL(bool mobile) {
	using namespace bg::engine;
	using namespace bg::tools;
	ShaderSource vshader(mobile ? ShaderSource::kLangOpenGLES3 : ShaderSource::kLangOpenGLCore);
	ShaderSource pointVShader(mobile ? ShaderSource::kLangOpenGLES3 : ShaderSource::kLangOpenGLCore);
	ShaderSource spotVShader(mobile ? ShaderSource::kLangOpenGLES3 : ShaderSource::kLangOpenGLCore);
	ShaderSource pointFShader(mobile ? ShaderSource::kLangOpenGLES3 : ShaderSource::kLangOpenGLCore);
	ShaderSource directionalFShader(mobile ? ShaderSource::kLangOpenGLES3 : ShaderSource::kLangOpenGLCore);
	ShaderSource spotFShader(mobile ? ShaderSource::kLangOpenGLES3 : ShaderSource::kLangOpenGLCore);
	getVertexShader(vshader);
	getVertexShader(pointVShader);
	getVertexShader(spotVShader);
	getPointLightFShader(pointFShader);
	getDirectionalLightFShader(directionalFShader);
	getSpotLightFShader(spotFShader);

	if (mobile) {
		_directionalShader = new openglEs::Shader(context());
		_pointShader = new openglEs::Shader(context());
		_spotShader = new openglEs::Shader(context());
	}
	else {
		_directionalShader = new openglCore::Shader(context());
		_pointShader = new openglCore::Shader(context());
		_spotShader = new openglCore::Shader(context());
	}
	_directionalShader->attachShader(openglCore::ShaderType::kVertexShader, vshader.text());
	_directionalShader->attachShader(openglCore::ShaderType::kFragmentShader, directionalFShader.text());
	_directionalShader->link();
	_directionalShader->setOutputParameterName(openglCore::ShaderParamType::kShaderOutFragmentLocation, "fs_outColor");
	
	vshader.initVertexBufferInputs(_directionalShader.getPtr());
	std::vector<std::string> uniformNames;
	vshader.uniformNames(uniformNames);
	directionalFShader.uniformNames(uniformNames);
	_directionalShader->initUniforms(uniformNames);
	

	uniformNames.clear();
	_pointShader->attachShader(openglCore::ShaderType::kVertexShader, pointVShader.text());
	_pointShader->attachShader(openglCore::ShaderType::kFragmentShader, pointFShader.text());
	_pointShader->link();
	_pointShader->setOutputParameterName(openglCore::ShaderParamType::kShaderOutFragmentLocation, "fs_outColor");
	
	pointVShader.initVertexBufferInputs(_pointShader.getPtr());
	pointVShader.uniformNames(uniformNames);
	pointFShader.uniformNames(uniformNames);
	_pointShader->initUniforms(uniformNames);

	uniformNames.clear();
	_spotShader->attachShader(openglCore::ShaderType::kVertexShader, spotVShader.text());
	_spotShader->attachShader(openglCore::ShaderType::kFragmentShader, spotFShader.text());
	_spotShader->link();
	_spotShader->setOutputParameterName(openglCore::ShaderParamType::kShaderOutFragmentLocation, "fs_outColor");

	spotVShader.initVertexBufferInputs(_spotShader.getPtr());
	spotVShader.uniformNames(uniformNames);
	spotFShader.uniformNames(uniformNames);
	_spotShader->initUniforms(uniformNames);
}

void Lighting::activateGL(bool, bg::base::TextureRenderSurface * surface) {
	bg::base::Texture * diffuse = surface->texture(0);
	bg::base::Texture * specular = surface->texture(1);
	bg::base::Texture * normal = surface->texture(2);
	bg::base::Texture * material = surface->texture(3);
	bg::base::Texture * positionTexture = _positionTexture.valid() ? _positionTexture.getPtr():
	surface->texture(4);
	float lightEmissionFactor = _firstLight ? 10.0f : 0.0f;
	if (_light.valid() && _light->type()==bg::base::Light::kTypeDirectional) {
		_directionalShader->setActive();

		_directionalShader->setUniform("inDiffuse", diffuse, 0);
		_directionalShader->setUniform("inSpecular", specular, 1);
		_directionalShader->setUniform("inNormal", normal, 2);
		_directionalShader->setUniform("inMaterial", material, 3);
		_directionalShader->setUniform("inPosition", positionTexture, 4);
		
		_directionalShader->setUniform("inShadowTexture", _shadowTexture.getPtr(), 5);
		
		bg::math::Matrix4 transform = _viewMatrix;
		transform.mult(_lightTransform);
		bg::math::Vector3 direction = transform
			.rotation()
			.multVector(_light->direction())
			.xyz();

		_directionalShader->setUniform("inLightAmbient",_light->ambient());
		_directionalShader->setUniform("inLightDiffuse",_light->diffuse());
		_directionalShader->setUniform("inLightSpecular",_light->specular());
		_directionalShader->setUniform("inLightDirection",direction);

		_directionalShader->setUniform("inClearColor", _clearColor);

		_directionalShader->setUniform("inLightEmissionFactor", lightEmissionFactor);
	}
	else if (_light.valid() && _light->type()==bg::base::Light::kTypePoint) {
		_pointShader->setActive();
		
		_pointShader->setUniform("inDiffuse", diffuse, 0);
		_pointShader->setUniform("inSpecular", specular, 1);
		_pointShader->setUniform("inNormal", normal, 2);
		_pointShader->setUniform("inMaterial", material, 3);
		_pointShader->setUniform("inPosition", positionTexture, 4);
		
		bg::math::Matrix4 transform = _viewMatrix;
		transform.mult(_lightTransform);
		
		_pointShader->setUniform("inLightAmbient",_light->ambient());
		_pointShader->setUniform("inLightDiffuse",_light->diffuse());
		_pointShader->setUniform("inLightSpecular",_light->specular());
		_pointShader->setUniform("inLightAttenuation",_light->attenuationVector());
		_pointShader->setUniform("inLightPosition",transform.position());

		_pointShader->setUniform("inClearColor",_clearColor);

		_pointShader->setUniform("inLightEmissionFactor", lightEmissionFactor);
	}
	else if(_light.valid() && _light->type() == bg::base::Light::kTypeSpot) {
		_spotShader->setActive();

		_spotShader->setUniform("inDiffuse", diffuse, 0);
		_spotShader->setUniform("inSpecular", specular, 1);
		_spotShader->setUniform("inNormal", normal, 2);
		_spotShader->setUniform("inMaterial", material, 3);
		_spotShader->setUniform("inPosition", positionTexture, 4);
		_spotShader->setUniform("inShadowTexture", _shadowTexture.getPtr(), 5);

		bg::math::Matrix4 transform = _viewMatrix;
		transform.mult(_lightTransform);
		bg::math::Vector3 position = transform.position();
		bg::math::Vector3 direction = transform
			.rotation()
			.multVector(_light->direction())
			.xyz()
			.normalize();
		

		_spotShader->setUniform("inLightAmbient", _light->ambient());
		_spotShader->setUniform("inLightDiffuse", _light->diffuse());
		_spotShader->setUniform("inLightSpecular", _light->specular());
		_spotShader->setUniform("inLightAttenuation", _light->attenuationVector());
		_spotShader->setUniform("inLightPosition", position);
		_spotShader->setUniform("inLightDirection",direction);
		_spotShader->setUniform("inSpotCutoff",_light->spotCutoff());
		_spotShader->setUniform("inSpotExponent",_light->spotExponent());

		_spotShader->setUniform("inClearColor", _clearColor);

		_spotShader->setUniform("inLightEmissionFactor", lightEmissionFactor);
	}
	_firstLight = false;
}

void Lighting::bindPolyListGL(bg::base::PolyList * plist, bool) {
	if (_light.valid() && _light->type()==bg::base::Light::kTypeDirectional) {
		_directionalShader->bindPolyList(plist);
	}
	else if (_light.valid() && _light->type()==bg::base::Light::kTypePoint) {
		_pointShader->bindPolyList(plist);
	}
	else if(_light.valid() && _light->type() == bg::base::Light::kTypeSpot) {
		_spotShader->bindPolyList(plist);
	}
}

void Lighting::unbindGL(bool) {
	if (_light.valid() && _light->type()==bg::base::Light::kTypeDirectional) {
		_directionalShader->unbindPolyList();
	}
	else if(_light.valid() && _light->type() == bg::base::Light::kTypePoint) {
		_pointShader->unbindPolyList();
	}
	else if(_light.valid() && _light->type() == bg::base::Light::kTypeSpot) {
		_spotShader->unbindPolyList();
	}
}

void Lighting::getVertexShader(bg::tools::ShaderSource & src) {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;

	src.precision(kHighp, kFloat);
	src.addShaderInput(Parameter(kFloat3, "inPosition", kRoleVertex));
	src.addShaderInput(Parameter(kFloat2, "inTex0", kRoleTex0));

	src.addShaderOutput(Parameter(kFloat2, "fsTex0Coord", kRoleOutput));

	src.beginFunction(kVoid, "main", {});
	src.functionBody({
		"gl_Position = vec4(inPosition, 1.0);",
		"fsTex0Coord = inTex0;"
	});
	src.endFunction();
}

void Lighting::getDirectionalLightFShader(bg::tools::ShaderSource & src) {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;

	src.precision(kHighp, kFloat);
	src.addShaderOutput(Parameter(kFloat4, "fs_outColor", 0));
	src.addShaderOutput(Parameter(kFloat4, "fs_outShininess", 1));

	src.addUniform({
		Parameter(kSampler2D, "inDiffuse"),
		Parameter(kSampler2D, "inSpecular"),
		Parameter(kSampler2D, "inNormal"),
		Parameter(kSampler2D, "inMaterial"),
		Parameter(kSampler2D, "inPosition"),
		Parameter(kSampler2D, "inShadowTexture"),
		Parameter(kFloat4, "inLightAmbient"),
		Parameter(kFloat4, "inLightDiffuse"),
		Parameter(kFloat4, "inLightSpecular"),
		Parameter(kFloat3, "inLightDirection"),
		Parameter(kFloat4, "inClearColor"),
		Parameter(kSampler2D, "inShininess"),
		// this is used to process the light emission and the unlit component only by the first light
		Parameter(kFloat, "inLightEmissionFactor")
	});
	src.addShaderInput(Parameter(kFloat2, "fsTex0Coord"));
	
	bg::shader::Utils(&src)
		.samplerColor()
		.samplerNormal()
		.combineNormalWithMap()
		.applyTextureMask()
		.specularColor()
		.pack()
		.unpack()
		.random();
	
	bg::shader::Lighting(&src)
		.getDirectionalLight()
		.getPointLight()
		.getShadowColor();

	src.beginFunction(kVoid, "main",{});
	src.functionBody({
		"vec4 position = texture(inPosition,fsTex0Coord);",
		"vec4 diffuse = texture(inDiffuse,fsTex0Coord);",
		"vec4 normalTex = texture(inNormal,fsTex0Coord);"
		"vec3 normal = normalTex.rgb * 2.0 - 1.0;",
		"bool unlit = normalTex.a == 0.0;",
		"vec4 shininessColor;",
		"if (position==vec4(0.0,0.0,0.0,1.0)) {",
		"	fs_outColor = inClearColor;",
		"	fs_outShininess = vec4(0.0,0.0,0.0,1.0);",
		"}",
		"else if (unlit) {",
		"	fs_outColor = vec4(diffuse.rgb * min(inLightEmissionFactor,1.0),1.0);",
		"	fs_outShininess = vec4(0.0,0.0,0.0,1.0);",
		"}",
		"else {",
		"	vec4 specular = texture(inSpecular,fsTex0Coord);",
		"	vec4 material = texture(inMaterial,fsTex0Coord);",
		
		"	vec4 shadowColor = max(inLightAmbient,texture(inShadowTexture,fsTex0Coord));",
		"	float lightEmission = material.r;",
		"	float shininess = material.g * 255.0;",
		"	vec4 outColor = getDirectionalLight(inLightAmbient,inLightDiffuse,inLightSpecular,shininess,"
										 "-inLightDirection,position.rgb,normal,vec4(diffuse.rgb,1.0),specular,shadowColor,shininessColor);",
		"	fs_outColor = vec4(outColor.rgb + (diffuse.rgb * lightEmission * inLightEmissionFactor),diffuse.a);",
		"	fs_outShininess = shininessColor * shadowColor;",
		"}"
	});
	src.endFunction();
}

void Lighting::getPointLightFShader(bg::tools::ShaderSource & src) {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;
	
	src.precision(kHighp, kFloat);
	src.addShaderOutput(Parameter(kFloat4, "fs_outColor", 0));
	src.addShaderOutput(Parameter(kFloat4, "fs_outShininess", 1));
	
	src.addUniform({
		Parameter(kSampler2D, "inDiffuse"),
		Parameter(kSampler2D, "inSpecular"),
		Parameter(kSampler2D, "inNormal"),
		Parameter(kSampler2D, "inMaterial"),
		Parameter(kSampler2D, "inPosition"),
		Parameter(kFloat4, "inLightAmbient"),
		Parameter(kFloat4, "inLightDiffuse"),
		Parameter(kFloat4, "inLightSpecular"),
		Parameter(kFloat3, "inLightPosition"),
		Parameter(kFloat3, "inLightAttenuation"),
		Parameter(kFloat4, "inClearColor"),
		Parameter(kSampler2D, "inShininess"),
		// this is used to process the light emission and the unlit component only by the first light
		Parameter(kFloat, "inLightEmissionFactor")
	});
	
	src.addShaderInput(Parameter(kFloat2, "fsTex0Coord"));

	bg::shader::Lighting(&src)
		.getPointLight();
	
	src.beginFunction(kVoid, "main",{});
	src.functionBody({
		"vec4 position = texture(inPosition,fsTex0Coord);",
		"vec4 diffuse = texture(inDiffuse,fsTex0Coord);",
		"vec4 normalTex = texture(inNormal,fsTex0Coord);"
		"vec3 normal = normalTex.rgb * 2.0 - 1.0;",
		"bool unlit = normalTex.a == 0.0;",
		"vec4 shininessColor;",
		"if (position==vec4(0.0,0.0,0.0,1.0)) {",
		"	fs_outColor = inClearColor;",
		"	fs_outShininess = vec4(0.0,0.0,0.0,1.0);",
		"}",
		"else if (unlit) {",
		"	fs_outColor = vec4(diffuse.rgb * min(inLightEmissionFactor,1.0),1.0);",
		"	fs_outShininess = vec4(0.0,0.0,0.0,1.0);",
		"}",
		"else {",
		"	vec4 specular = texture(inSpecular,fsTex0Coord);",
		"	vec4 material = texture(inMaterial,fsTex0Coord);",
		
		"	float shininess = material.g * 255.0;",
		"	float lightEmission = material.r;",
		"	vec4 outColor = getPointLight(inLightAmbient,inLightDiffuse,inLightSpecular,shininess,inLightPosition,"
										"inLightAttenuation.x, inLightAttenuation.y, inLightAttenuation.z,"
										"position.xyz,normal,vec4(diffuse.rgb,1.0),specular,shininessColor);",
		"	fs_outColor = vec4(outColor.rgb + (diffuse.rgb * lightEmission * inLightEmissionFactor),diffuse.a);",
		"	fs_outShininess = shininessColor;",
		"}"
	});
	src.endFunction();
}

void Lighting::getSpotLightFShader(bg::tools::ShaderSource & src) {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;

	src.precision(kHighp, kFloat);
	src.addShaderOutput(Parameter(kFloat4, "fs_outColor", 0));
	src.addShaderOutput(Parameter(kFloat4, "fs_outShininess", 1));

	src.addUniform({
		Parameter(kSampler2D, "inDiffuse"),
		Parameter(kSampler2D, "inSpecular"),
		Parameter(kSampler2D, "inNormal"),
		Parameter(kSampler2D, "inMaterial"),
		Parameter(kSampler2D, "inPosition"),
		Parameter(kFloat4, "inLightAmbient"),
		Parameter(kFloat4, "inLightDiffuse"),
		Parameter(kFloat4, "inLightSpecular"),
		Parameter(kFloat3, "inLightPosition"),
		Parameter(kFloat3, "inLightDirection"),
		Parameter(kFloat3, "inLightAttenuation"),
		Parameter(kFloat4, "inClearColor"),
		Parameter(kFloat, "inSpotCutoff"),
		Parameter(kFloat, "inSpotExponent"),
		Parameter(kSampler2D, "inShadowTexture"),
		Parameter(kSampler2D, "inShininess"),
		// this is used to process the light emission and the unlit component only by the first light
		Parameter(kFloat, "inLightEmissionFactor")
	});

	src.addShaderInput(Parameter(kFloat2, "fsTex0Coord"));

	bg::shader::Utils(&src)
		.combineNormalWithMap()
		.unpack()
		.random();
	
	bg::shader::Lighting(&src)
		.getSpotLight()
		.getShadowColor();

	src.beginFunction(kVoid, "main", {});
	src.functionBody({
		"vec4 position = texture(inPosition,fsTex0Coord);",
		"vec4 diffuse = texture(inDiffuse,fsTex0Coord);",
		"vec4 normalTex = texture(inNormal,fsTex0Coord);"
		"vec3 normal = normalTex.rgb * 2.0 - 1.0;",
		"bool unlit = normalTex.a == 0.0;",
		"vec4 shininessColor;",
		"if (position==vec4(0.0,0.0,0.0,1.0)) {",
		"	fs_outColor = inClearColor;",
		"	fs_outShininess = vec4(0.0,0.0,0.0,1.0);",
		"}",
		"else if (unlit) {",
		"	fs_outColor = vec4(diffuse.rgb * min(inLightEmissionFactor,1.0),1.0);",
		"	fs_outShininess = vec4(0.0,0.0,0.0,1.0);",
		"}",
		"else {",
		"	vec4 specular = texture(inSpecular,fsTex0Coord);",
		"	vec4 material = texture(inMaterial,fsTex0Coord);",

		"	vec4 shadowColor = texture(inShadowTexture,fsTex0Coord);",
		"	float shininess = material.g * 255.0;",
		"	float lightEmission = material.r;",
		"	vec4 outColor = getSpotLight(inLightAmbient,inLightDiffuse,inLightSpecular,shininess,inLightPosition,inLightDirection,"
										"inLightAttenuation.x, inLightAttenuation.y, inLightAttenuation.z,"
										"inSpotCutoff,inSpotExponent,"
										"position.xyz,normal,vec4(diffuse.rgb,1.0),specular,shadowColor,shininessColor);",
		"	fs_outColor = vec4(outColor.rgb + (diffuse.rgb * lightEmission * inLightEmissionFactor),diffuse.a);",
		"	fs_outShininess = shininessColor * shadowColor;",
		"}"
	});
	src.endFunction();
}


}
}
