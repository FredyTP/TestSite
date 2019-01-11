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


#ifndef _bg2e_db_bg2_bg2_file_loader_hpp_
#define _bg2e_db_bg2_bg2_file_loader_hpp_

#include <bg/base/referenced_pointer.hpp>
#include <bg/base/vwglb-utils.hpp>
#include <bg/math/matrix.hpp>
#include <bg/math/vector.hpp>
#include <bg/base/texture.hpp>
#include <bg/base/poly_list.hpp>

#include <bg/base/exception.hpp>
#include <bg/scene/drawable.hpp>

#include <map>

namespace bg {
namespace db {
namespace bg2 {
	
class Bg2FileReaderDelegate {
public:
	struct FileVersion {
		unsigned char major;
		unsigned char minor;
		unsigned char revision;
	};

	enum FileMetadata {
		kNumberOfPlist = 1
	};

	enum JointType {
		kJointTypeInput = 0,
		kJointTypeOutput
	};

	Bg2FileReaderDelegate() :_expectedPlist(0) {}

	// TODO: Define the delegate methods

	virtual void onError(const std::string & error) = 0;
	virtual void onWarning(const std::string & warning) = 0;

	virtual void fileVersion(const FileVersion & version) = 0;
	virtual void metadata(FileMetadata metadata, int value) = 0;
	virtual void metadata(FileMetadata metadata, const std::string & value) = 0;
	virtual void metadata(FileMetadata metadata, bool value) = 0;
	virtual void metadata(FileMetadata metadata, float value) = 0;

	virtual void polyList(base::PolyList * plist) = 0;

	virtual void materials(const std::string & materialsString) = 0;

	virtual void joint(JointType type, const bg::math::Vector3 & offset, float pitch, float roll, float yaw) = 0;

protected:

	int _expectedPlist;
};
	
class BG2E_EXPORT Bg2FileReader {
public:
	Bg2FileReader(bg::base::Context * ctx) :_context(ctx) {}

	bool load(const std::string & path);
	bool loadHeader(const std::string & path);

	inline void setDelegate(Bg2FileReaderDelegate * delegate) { _delegate = delegate; }
	inline Bg2FileReaderDelegate * getDelegate() { return _delegate; }

	inline const std::string & componentData() const { return _componentData; }

protected:
	Bg2FileReaderDelegate * _delegate;

	void readHeader(std::string & materialsString, int & numberOfPlist);
	void readPolyList(int numberOfPlist);
	void readSinglePolyList(bool);

	base::VwglbUtils _fileUtils;
	std::map<std::string, bg::ptr<base::PolyList> > _plistMap;
	std::string _componentData;
	
	bg::ptr<bg::base::Context> _context;
};

class BG2E_EXPORT Bg2MatReader : public bg::base::ContextObject {
public:
	Bg2MatReader(bg::base::Context * ctx, const bg::system::Path & modelPath)
		:ContextObject(ctx), _modelFilePath(modelPath) {}

	bool isPresent();

	void load(bg::scene::Drawable *);

protected:
	bg::system::Path _modelFilePath;
};
	
}
}
}

#endif
