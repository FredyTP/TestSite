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


#ifndef _bg2e_shader_lighting_hpp_
#define _bg2e_shader_lighting_hpp_

#include <bg/shader/library.hpp>

namespace bg {
namespace shader {

class BG2E_EXPORT Lighting : public Library {
public:
	Lighting(bg::tools::ShaderSource *);
    
    Lighting & beckmannDistribution();
    Lighting & beckmannSpecular();
    
    /*
     *  Compute the light, using the rest of the rest of the functions
     */
    Lighting & getLight();
	
	/*	Compute the directional light component
	 *	return type: float4
	 *	parameters: ambient, diffuse, specular, shininess, lightDirection,
	 *				targetPosition, targetNormal,
	 *				materialDiffuse, materialSpecular,
	 *				shadowColor
	 */
	Lighting & getDirectionalLight();

	/*
	 *	Compute the point light component
	 *	return type: float4
	 *	parameters: ambient, diffuse, specular, shininess, lightPosition,
	 *				constAttenuation, linearAttenuation, expAttenuation,
	 *				targetPosition, targetNormal,
	 *				materialDiffuse, materialSpecular
	 */
	Lighting & getPointLight();

	/*
	 *	Compute the spot light component
	 *	return type: float4
	 *	parameters: ambient, diffuse, specular, shininess, lightPosition, lightDirection
	 *				constAttenuation, linearAttenuation, expAttenuation,
	 *				spotCutoff, spotExponent,
	 *				vertexPos, normal, matDiffuse, matSpecular
	 *				shadowColor
	 */
	Lighting & getSpotLight();

	/*
	 *	Compute the shadow color from a shadow map
	 *	return type: float4
	 *	parameters:	vertexPositionFromLight, shadowMap, shadowType, 
	 *				shadowStrength, shadowBias, shadowColor
	 *	Required dependencies:
	 *		Utils::unpack()
	 *		Utils::random()
	 */
	Lighting & getShadowColor();


};

}
}

#endif
