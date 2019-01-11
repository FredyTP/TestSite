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



#ifndef _bg2e_shader_utils_hpp_
#define _bg2e_shader_utils_hpp_

#include <bg/shader/library.hpp>

namespace bg {
namespace shader {

class BG2E_EXPORT Utils : public Library {
public:
	Utils(bg::tools::ShaderSource *);

	/*
	 *	Get a sampler color, applying an offset and scale
	 *	Return type: float4
	 *	Parameters: sampler, uv, offset, scale
	 */
	Utils & samplerColor();

	/*
	 *	Get a sampler normal, applying an offset and scale. This function
	 *	automatically converts the color value into a normalized vector
	 *	Return type: float3
	 *	Parameters: sampler, uv, offset, scale
	 *	Required dependencies:
	 *		samplerColor()
	 */
	Utils & samplerNormal();

	/*
	 *	Combine a normal vector with a normal extracted from a normal map
	 *	Return type: float3
	 *	Parameters: normal, tangent, bitangent, normalMapValue
	 */
	Utils & combineNormalWithMap();

	/*
	 *	Apply a texture map to a floating point value
	 *	Return type: float
	 *	Parameters: value, textureMaskSampler, uv, offset, scale, channelMask, invert
	 *	Required dependencies:
	 *		samplerColor();
	 */
	Utils & applyTextureMask();

	/*
	 *	Compute specular color from a specular value and the shininess map
	 *	Return type: float
	 *	Parameters: specularColor, shininessMaskSampler, uv, offset, scale, channelMask, invert
	 *	Required dependencies:
	 *		applyTextureMask()
	 */
	Utils & specularColor();


	/*
	 *	Pack a floating point value into a RGB pixel
	 *	Return type: float4
	 *	Parameters: floatValue
	 */
	Utils & pack();

	/*
	 *	Unpack a floating point value packed with pack() function
	 *	Return type: float
	 *	Parameters: packedVector 
	 */
	Utils & unpack();

	/*
	 *	Generate a pseudo random number
	 *	Return type: float
	 *	Parameters: seed
	 */
	Utils & random();

	/*
	 *	Apply blur
	 *	Return type: float4
	 *	Parameters: sampler2D, texCoord, radius, viewSize
	 */
	Utils & blur();

	/*
	 *	Apply bloom
	 *	Return type: float4
	 *	Parameters: sampler2D, texCoord, radius, viewSize
	 */
	Utils & bloom();

	/*
	 *	Sample a texture at lower resolution
	 *	Return type: float4
	 *	Parameters: sampler, uv, textureSize, reduction
	 */
	Utils & textureDownsample();

	/*
	 *	Apply blur with a downsample, to increase the performance
	 *	Return type: float4
	 *	Parameters: sampler2D, texCoord, radius, viewSize, downsample
	 */
	Utils & blurDownsample();
    
    /*
     *  Apply blur to a cubemap
     */
    Utils & blurCube();
};

}
}

#endif
