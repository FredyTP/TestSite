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


#ifndef	_bg2e_wnd_file_dialog_hpp_
#define	_bg2e_wnd_file_dialog_hpp_

#include <bg/base/referenced_pointer.hpp>

#include <string>
#include <vector>

namespace bg {
namespace wnd {

class BG2E_EXPORT FileDialog : public bg::base::ReferencedPointer {
public:
	enum DialogType {
		kTypeOpenFile = 0,
		kTypeSaveFile = 1,
		kTypeSelectFolder = 2
	};
	
	static FileDialog * Get();
	static FileDialog * OpenFileDialog() {
		FileDialog * d = Get();
		if (d) {
			d->setType(kTypeOpenFile);
		}
		return d;
	}
	
	static FileDialog * SaveFileDialog() {
		FileDialog * d = Get();
		if (d) {
			d->setType(kTypeSaveFile);
		}
		return d;
	}
	
	static FileDialog * SelectFolderDialog() {
		FileDialog * d = Get();
		if (d) {
			d->setType(kTypeSelectFolder);
		}
		return d;
	}
	
	inline void setType(DialogType t) { _type = t; }
	inline DialogType getType() const { return _type; }
	
	inline const std::string & getResultPath() const { return _resultPath; }
	
	inline const std::vector<std::string> & getFilters() const { return _filters; }
	inline void addFilter(const std::string & f) { _filters.push_back(f); }
	inline void clearFilters() { _filters.clear(); }
	
	virtual bool show() = 0;
	
protected:
	FileDialog();
	virtual ~FileDialog();
	
	DialogType _type;
	std::string _resultPath;
	std::string _initPath;
	std::vector<std::string> _filters;
};

}
}

#endif
