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


#include <bg/db/plugin/bg2_prefab.hpp>

#include <bg/db/plugin/bg2_drawable.hpp>

#include <bg/db/bg2/bg2_file_reader.hpp>
#include <bg/base/exception.hpp>
#include <bg/base/material.hpp>
#include <bg/tools/material_tools.hpp>
#include <bg/tools/poly_list_tools.hpp>
#include <bg/tools/drawable_tools.hpp>
#include <bg/scene/camera.hpp>

#include <bg/scene/transform.hpp>
#include <bg/scene/chain.hpp>
#include <bg/log.hpp>

#include <iostream>

namespace bg {
namespace db {
namespace plugin {

ReadPrefabBg2::ReadPrefabBg2()
{
	
}

ReadPrefabBg2::~ReadPrefabBg2() {
}

bool ReadPrefabBg2::supportFileType(const std::string & extension) {
	return extension=="vwglb" || extension=="bg2";
}

bg::scene::Node * ReadPrefabBg2::load(bg::base::Context * ctx, const bg::system::Path & path) {
	bg::ptr<bg::scene::Node> result;
	{
		bg2::Bg2FileReader reader(ctx);
		Bg2ReaderDelegate del(ctx, path);
		reader.setDelegate(&del);
		reader.load(path.text());
		result = del.node();
		// Try to load the material definition file
		bg2::Bg2MatReader matReader(ctx, path);
		if (matReader.isPresent()) {
			try {
				matReader.load(del.drawable());
			}
			catch (std::runtime_error & msg) {
				bg::log(bg::log::kWarning) << "Error loading material definition file: " << msg.what() << bg::endl;
			}
		}
	}
	return result.release();
}


WritePrefabBg2::WritePrefabBg2()
{
	
}

WritePrefabBg2::~WritePrefabBg2() {
	
}

bool WritePrefabBg2::supportFileType(const std::string & extension) {
	return extension=="vwglb" || extension=="bg2";
}

void WritePrefabBg2::save(bg::base::Context *, const bg::system::Path &, bg::scene::Node *) {
	
}
		


}
}
}
