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


#include <bg/platform.hpp>
#include <bg/system/system.hpp>
#include <bg/system/path.hpp>

#include <sstream>
#include <algorithm>
#include <regex>

namespace bg {
namespace system {


void Path::standarize_path(const std::string & inputPath, std::string & outputPath) const {
	outputPath = inputPath;
	std::replace(outputPath.begin(),outputPath.end(), '\\','/');
}

std::string Path::add_path_component(const std::string & base, const std::string & append) const {
	if (base == "") return append;
	std::string stdPath;
	std::string lastComponent = append;
	standarize_path(base, stdPath);
	
	if (lastComponent.find_first_of("/")==0) {
		lastComponent = lastComponent.erase(0,1);
	}
	if (stdPath.find_last_of("/")!=(stdPath.size() - 1)) {
		stdPath = stdPath + "/" + lastComponent;
	}
	else {
		stdPath += lastComponent;
	}
	return stdPath;
}

Path Path::AppDir() {
	Path execPath = ExecDir();
	if (bg::system::currentPlatform()==bg::system::kMac) {
		execPath.addComponent("../../..");
	}
	return execPath;
}

Path::Path()
	:_beginSlash(false)
{
    
}

Path::Path(const std::string & path)
	:_beginSlash(false)
{
	setText(path);
}

Path::Path(const Path & p)
{
	setText(p.text());
}

Path & Path::setText(const std::string & path){
	_path.clear();
	standarize_path(path, _pathString);
	
	_beginSlash = _pathString.size()>0 && _pathString.front()=='/';
	bool skipLetter = false;
#ifdef BG2E_WINDOWS
	std::regex absoluteRE("^([a-zA-Z]):[\\/\\\\]");
	std::smatch match;
	if (std::regex_search(_pathString, match, absoluteRE)) {
		_beginSlash = true;
		_letter = match[1];
		skipLetter = true;
	}
#endif

	if (_pathString.size()>1 && _pathString.back()=='/') {
		_pathString.pop_back();
	}
	
	std::stringstream sstr(_pathString);
	std::string token;
    size_t numElems = 0;
	while (std::getline(sstr,token,'/')) {
		if (!token.empty() && !skipLetter) {
            ++numElems;
            if (_path.capacity()<=numElems) {
                _path.reserve(_path.capacity() + 10);
            }
            _path.push_back(token);
        }
		skipLetter = false;
	}

	return *this;
}

Path & Path::addComponent(const std::string & c) {
	std::string newComp;
	standarize_path(c, newComp);
	
	std::stringstream sstr(newComp);
	std::string token;
    size_t numElems = 0;
	while (std::getline(sstr,token,'/')) {
        if (!token.empty()) {
            ++numElems;
            if (_path.capacity()<=numElems) {
                _path.reserve(_path.capacity() + 10);
            }
            _path.push_back(token);
        }
	}
	buildString();
	return *this;
}

Path & Path::removeLastComponent() {
	if (_path.size()>0) {
		_path.pop_back();
		buildString();
	}
	return *this;
}

Path & Path::removeExtension() {
	if (_path.size()>0) {
		std::string last = _path.back();
		_path.pop_back();
		if (last.find_last_of(".")!=std::string::npos) {
			last.erase(last.find_last_of("."));
		}
		_path.push_back(last);
		buildString();
	}
	return *this;
}

Path & Path::addExtension(const std::string & ext) {
	if (_path.size()>0) {
		std::string last = _path.back();
		_path.pop_back();
		if (ext.front()!='.') {
			last += '.';
		}
		last += ext;
		_path.push_back(last);
		buildString();
	}
	return *this;
}
	

void Path::list(std::function<void(const Path & file)> closure) const {
	std::vector<std::string> files;
	std::vector<std::string> directories;
	listDirectory(_pathString, files, directories);
	for (auto f : files) {
		Path p(_pathString);
		p.addComponent(f);
		closure(p);
	}
	for (auto f : directories) {
		Path p(_pathString);
		p.addComponent(f);
		closure(p);
	}
}

void Path::listRecursive(std::function<void(const Path & file)> closure) const {
	std::vector<std::string> files;
	listFilesRecursive(_pathString, files);
	for (auto f : files) {
		Path p(_pathString);
		p.addComponent(f);
		closure(p);
	}
}
	
bool Path::isAbsolute() const {
	if (BG2E_WINDOWS==1) {
		if (_pathString.size()<2) return false;
		return _pathString.at(1)==':' || _pathString.at(0)=='/';
	}
	else {
		if (_pathString.size()==0) return false;
		return _pathString.front()=='/';
	}
}

std::string Path::lastPathComponent() const {
	return _path.back();
}

std::string Path::fileName() const {
    std::string result = "";
	if (_path.size()>0) {
		result = _path.back();
		if (result.find_last_of(".")!=std::string::npos) {
			result.erase(result.find_last_of("."));
		}
	}
	return result;
}

std::string Path::extension() const {
    std::string result = "";
	if (_path.size()>0) {
		result = _path.back();
		if (result.find_last_of(".")!=std::string::npos) {
			result.erase(0,result.find_last_of(".") + 1);
			std::transform(result.begin(),
						   result.end(),
						   result.begin(),
						   ::tolower);
		}
		else {
			result = "";
		}
	}
	return result;
}

std::string Path::filePath() const {
    std::string result = "";
	if (_path.size()>0) {
		std::vector<std::string>::const_iterator it;
		std::string separator = _beginSlash ? "/":"";
		if (!_letter.empty()) separator = _letter + ":/";
		for (it=_path.begin(); it!=_path.end() - 1; ++it) {
			result += separator + *it;
			separator = "/";
		}
	}
	return result;
}
	
void Path::buildString() {
	_pathString = "";
	std::string separator = "";
	if (_beginSlash) separator = "/";
	if (!_letter.empty()) separator = _letter + ":/";
	for (auto item : _path) {
		_pathString += separator + item;
		separator = "/";
	}
	if (_pathString.empty()) _pathString = separator;
}

void Path::listFilesRecursive(const std::string & path, std::vector<std::string> & allFiles, bool clearVector, const std::string & basePath) const {
	if (clearVector) {
		allFiles.clear();
	}
	std::vector<std::string> files;
	std::vector<std::string> directories;
	listDirectory(path, files, directories);
	std::vector<std::string>::iterator it;
	for (it=files.begin(); it!=files.end(); ++it) {
		if ((*it).front()=='.') continue;
		allFiles.push_back(add_path_component(basePath, *it));
	}
	for (it=directories.begin(); it!=directories.end(); ++it) {
		std::string newBasePath = add_path_component(basePath, *it);
		listFilesRecursive(add_path_component(path, *it), allFiles, false, newBasePath);
	}
}

}
}


