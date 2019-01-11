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

#ifndef _bg2e_base_texture_effect_hpp_
#define _bg2e_base_texture_effect_hpp_

#include <bg/base/context_object.hpp>
#include <bg/base/texture.hpp>
#include <bg/base/render_surface.hpp>
#include <bg/base/poly_list.hpp>


namespace bg {
namespace base {

class BG2E_EXPORT TextureEffect : public ContextObject, public ReferencedPointer {
public:
	TextureEffect(Context *);
	
	virtual void activate(Texture *) {}
	virtual void activate(TextureRenderSurface * surface) { activate(surface->texture(0)); }
	virtual void activate(TextureVector & textures) { activate(textures[0].getPtr()); }

	virtual void bindPolyList(bg::base::PolyList *) = 0;
	virtual void unbind() = 0;
	
	void drawTexture(Texture *);
	void drawTexture(TextureRenderSurface *);
	void drawTexture(TextureVector &);

protected:
	virtual ~TextureEffect();
	
	ptr<PolyList> _rect;
};

}
}

#endif