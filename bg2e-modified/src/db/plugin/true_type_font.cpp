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

#include <bg/db/plugin/true_type_font.hpp>
#include <bg/tools/texture_cache.hpp>
#include <fstream>
#include <iostream>
#include <vector>

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

namespace bg {
namespace db {
namespace plugin {

static const float kScale = 10.0f;


ReadFontTrueType::ReadFontTrueType()
{
}

ReadFontTrueType::~ReadFontTrueType() {
}

bool ReadFontTrueType::supportFileType(const std::string & extension) {
	return extension=="ttf";
}

bg::text::Font * ReadFontTrueType::load(bg::base::Context * ctx, const bg::system::Path & path, float size) {
	std::fstream fontFile;
	size *= kScale;
	fontFile.open(path.text(), std::ios::binary | std::ios::in);
	if(!fontFile.is_open()) {
		throw bg::base::ReadFileException("No such font file");
	}
	std::vector<unsigned char> fileContents((std::istreambuf_iterator<char>(fontFile)), std::istreambuf_iterator<char>());
	stbtt_fontinfo fontInfo;
	unsigned char * ttf_buffer = &fileContents[0];
	stbtt_InitFont(&fontInfo, ttf_buffer, stbtt_GetFontOffsetForIndex(ttf_buffer, 0));

	bg::ptr<bg::text::Font> font = new bg::text::Font(path.text(), size);
	std::string glyphString = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~¡¢£¤¥¦§¨©ª«¬®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷øùúûüýþÿ	";;
	float scale = stbtt_ScaleForPixelHeight(&fontInfo, size);
	float maxAdvance = 0.0f;
	float maxHeight = 0.0f;
	bg::system::utf::iterateString(glyphString, [&](std::string & mb, bg::system::UTFCode cp, bg::system::UTFCode hex) {
		bg::text::BitmapGlyph * glyph = getGlyph(ctx, &fontInfo, scale, mb, cp);
		if (glyph) {
			font->addGlyph(cp, glyph);
			maxAdvance = maxAdvance<glyph->advance() ? glyph->advance() : maxAdvance;
			maxHeight = maxHeight<glyph->scaledHeight() ? glyph->scaledHeight() : maxHeight;
		}
	});
	// Add white space
	bg::text::BitmapGlyph * space = new bg::text::BitmapGlyph();
	space->setBitmap(nullptr);
	space->setMaterial(new bg::base::Material());
	space->material()->setTexture(bg::tools::TextureCache::TransparentTexture(ctx));
	space->setCharacter(" ");
	space->setBounds(bg::math::Bounds2(0.0f,maxAdvance,0.0f,0.0f));
	space->setBearing(bg::math::Vector2(maxAdvance,0.0f));
	space->setAdvance(maxAdvance * 0.33f);
	font->addGlyph(bg::system::utf::kCodeSpace, space);
	
	font->setLineHeight(maxHeight);
	
	return font.release();
}


bg::text::BitmapGlyph * ReadFontTrueType::getGlyph(bg::base::Context * context, void * ap_font, float scale, const std::string & mbChar, bg::system::UTFCode codepoint) {
	stbtt_fontinfo * font = reinterpret_cast<stbtt_fontinfo*>(ap_font);
	bg::ptr<bg::text::BitmapGlyph> glyph = new bg::text::BitmapGlyph();
	int advance, bearing;
	int x0, y0, x1, y1;

	stbtt_GetCodepointHMetrics(font, static_cast<int>(codepoint), &advance, &bearing);
	stbtt_GetCodepointBitmapBoxSubpixel(font, static_cast<int>(codepoint), scale, scale, 0, 0, &x0, &y0, &x1, &y1);
	int glyph_x = x1 - x0;
	int glyph_y = y1 - y0;
	int width = 4;

	uint8_t *scr_buffer = new uint8_t[glyph_x*glyph_y];
	uint8_t *four_buffer = new uint8_t[glyph_x*glyph_y*width];

	stbtt_MakeCodepointBitmapSubpixel(font, scr_buffer, glyph_x, glyph_y, glyph_x, scale, scale, 0, 0, static_cast<int>(codepoint));

	stbtt_GetCodepointBox(font, static_cast<int>(codepoint), &x0, &y0, &x1, &y1);
	float s_advance = static_cast<float>(advance) * scale;
	float s_x0 = static_cast<float>(x0) * scale;
	float s_y0 = static_cast<float>(y0) * scale;
	float s_x1 = static_cast<float>(x1) * scale;
	float s_y1 = static_cast<float>(y1) * scale;

	int i;
	for(i = 0; i < glyph_x*glyph_y; i++)
		*(uint32_t*)(four_buffer + i*width) = (scr_buffer[i] << 24) | 0x00FFFFFF;
	
	if (glyph_x==0 || glyph_y==0) return nullptr;

	bg::ptr<bg::base::Image> img = new bg::base::Image();
	img->setData(reinterpret_cast<unsigned char*>(four_buffer), bg::math::Size2Di(glyph_x, glyph_y), 4, bg::base::Image::kFormatRGBA);
	img->flipVertically();

	bg::ptr<bg::base::Texture> texture = new bg::base::Texture(context);
	texture->setResizeFilter(bg::base::Texture::kFilterNearest);
	texture->setWrapModeU(bg::base::Texture::kWrapModeClamp);
	texture->setWrapModeV(bg::base::Texture::kWrapModeClamp);
	texture->createWithImage(img.getPtr());
	glyph->setBitmap(img.getPtr());
	glyph->setMaterial(new bg::base::Material());
	glyph->material()->setProcessAsTransparent(true);
	glyph->material()->setTexture(texture.getPtr());
	glyph->material()->setCullFace(false);
	glyph->material()->setAlphaCutoff(0.002f);
	glyph->setCharacter(mbChar);
	glyph->setBounds(bg::math::Bounds2(s_x0, s_x1, s_y0, s_y1));
	glyph->setBearing(bg::math::Vector2(s_x0, s_y1));
	glyph->setAdvance(s_advance);
	return glyph.release();
}

}
}
}
