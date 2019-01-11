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

#ifndef _bg2e_scene_skybox_hpp_
#define _bg2e_scene_skybox_hpp_

#include <bg/scene/component.hpp>

#include <bg/base/texture.hpp>
#include <bg/base/poly_list.hpp>
#include <bg/base/material.hpp>
#include <bg/system/path.hpp>

namespace bg {
namespace scene {

class BG2E_EXPORT Skybox : public bg::scene::Component {
public:
	Skybox();

	virtual Component * clone();

	inline const std::string & getImageFile(bg::base::Texture::CubemapFace face) const { return _images[face]; }
    inline void setImageFile(bg::base::Texture::CubemapFace face, const bg::system::Path & file) { setImageFile(face,file.text()); }
	inline void setImageFile(bg::base::Texture::CubemapFace face, const std::string & file) { _images[face] = file; }

	void loadSkybox(bg::base::Context *);
    
    void draw(bg::base::Pipeline * pipeline);

	void deserialize(bg::base::Context *, bg::db::json::Value *, const bg::system::Path &);
	bg::db::json::Value * serialize(bg::base::Context *, const bg::system::Path &);

protected:
	virtual ~Skybox();

	std::vector<std::string> _images{ "","","","","","" };
    std::vector<bg::ptr<bg::base::Texture>> _textures;
    std::vector<bg::ptr<bg::base::PolyList>> _cube;
    bg::ptr<bg::base::Material> _material;
    
    void createCube(bg::base::Context *);
};

}
}
#endif
