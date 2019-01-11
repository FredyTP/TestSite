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

#ifndef _bg2e_archive_hpp_
#define _bg2e_archive_hpp_

#include <bg/export.hpp>

#include <bg/base/exception.hpp>
#include <bg/base/vwglb-utils.hpp>
#include <bg/system/path.hpp>

#include <string>
#include <vector>
#include <functional>

namespace bg {
namespace system {

class BG2E_EXPORT Archive {
public:
	enum PackageStatus {
		kStatusClosed = 0,
		kStatusPack = 1,
		kStatusUnpack = 2
	};

	enum OpenMode {
		kModePack = PackageStatus::kStatusPack,
		kModeUnpack = PackageStatus::kStatusUnpack
	};

	Archive();

	/*
	*	Open a package file and prepare it for unpack or unpack files
	*	Returns false if packagePath does not exist.
	*	Throws: - bg::base::NoSuchFileException if the packagePath does not exist
	*			- bg::base::InvalidStateException if the archive is already open
	*/
	void open(const Path & packagePath, OpenMode mode);


	/*
	*	Add the file filePath to the package.
	*	packagePath is the internal path of this file inside the package (only the path, without the file name)
	*	This function returns false if the file does not exists, if the file could not be opened or if the package
	*	is not prepared to pack files
	*	Throws: - bg::base::NoSuchFileException if the file to add does not exists
	*			- bg::base::InvalidStateException if the archive is not open, or the open mode is kModeUnpack
	*/
	void addFile(const Path & filePath, const std::string & packagePath = "");

	/*
	*	Unpack the package to path
	*	Returns false if the package format is not correct
	*	Throws:	- bg::base::OverwriteException if the destination path exists and overwrite parameter is false
	*			- bg::base::InvalidStateException if the archive is closed or the open mode is kModePack
	*/
	void unpackTo(const Path & path, bool overwrite = false);

	/*
	*	Close the package.
	*	Returns false if the package is not prepared for pack or is not opened
	*/
	bool close();

	/*
	*	Rerturns the status of the package, that could be:
	*		kStatusClosed
	*		kStatusPack: the package is ready to pack files
	*		kStatusUnpack: the package is ready to unpack files
	*/
	PackageStatus getStatus() const { return _status; }

protected:
	PackageStatus _status;
	Path _packagePath;
//	std::string _packagePath;
//	std::string _packageFileName;

	base::VwglbUtils _packageFile;

	void prepareToPack(const std::string & path);
	void prepareToUnpack(const std::string & path);
};

}
}

#endif	// _bg2e_archive_hpp_


