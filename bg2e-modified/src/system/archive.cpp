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


#include <bg/system/archive.hpp>
#include <bg/system/path.hpp>

namespace bg {
namespace system {

Archive::Archive()
    :_status(kStatusClosed)
{
    
}

void Archive::open(const Path & packagePath, OpenMode mode) {
	mode == kModePack ? prepareToPack(packagePath.text()) : prepareToUnpack(packagePath.text());
	_status = static_cast<PackageStatus>(mode);
	_packagePath = packagePath;
}

void Archive::addFile(const Path & filePath, const std::string & packagePathStr) {
	if (_status == kStatusPack) {
		if (filePath.isFile()) {
			Path dstFilePath(packagePathStr);
			dstFilePath.addComponent(filePath.lastPathComponent());

			std::fstream inFile;
			inFile.open(filePath.text(), std::ios::binary | std::ios::in);
			if (!inFile.is_open()) {
				throw base::NoSuchFileException("Could not add archive. No such file or directory: " + filePath.text());
			}
			inFile.seekg(0, inFile.end);
			int fileSize = static_cast<int>(inFile.tellg());
			inFile.seekg(0, inFile.beg);

			_packageFile.writeString(dstFilePath.text());
			_packageFile.writeInteger(fileSize);
			char * buffer = new char[fileSize];
			inFile.read(buffer, fileSize);
			_packageFile.stream().write(buffer, fileSize);
			delete [] buffer;
			inFile.close();
		}
	}
	else {
		throw base::InvalidStateException("Could not add file to archive. Invalid open mode");
	}
}

void Archive::unpackTo(const Path & path, bool overwrite) {
	if (_status == kStatusUnpack) {
		Path destinationPath = path;
		destinationPath.addComponent(_packagePath.fileName());
		
		if (!overwrite && destinationPath.exists()) {
			throw base::OverwriteFileException("Could not unpack archive. File exists at destination path: " + destinationPath.text());
		}
		else if (overwrite && destinationPath.exists()) {
			destinationPath.remove();
		}
		
		// Create destination directory
		destinationPath.create();
		
		// Unpack files
		std::string filePathStr;
		int fileSize = 0;
		char * buffer = nullptr;
		while (!_packageFile.stream().eof()) {
			bool status = _packageFile.readString(filePathStr);
			status = status && _packageFile.readInteger(fileSize);
			if (status && fileSize>0) {
				buffer = new char[fileSize];
				_packageFile.stream().read(buffer, fileSize);
				Path filePath(destinationPath);
				filePath.addComponent(filePathStr);
				Path fileDirectory(filePath.filePath());
				if (!fileDirectory.exists()) {
					fileDirectory.create();
				}

				std::fstream outFile(filePath.text(), std::ios::binary | std::ios::out);
				outFile.write(buffer, fileSize);
				outFile.close();
				delete [] buffer;
			}
			else if (!_packageFile.stream().eof()) {
				throw base::ReadFileException("Could not unpack archive: Error reading from archive file");
			}
		}
	}
	else {
		throw base::InvalidStateException("Could not unpack archive. Invalid open mode.");
	}
}

bool Archive::close() {
	if (!_packageFile.stream().is_open() || _status == kStatusClosed) {
		return false;
	}
	_packageFile.close();
	return true;
}

void Archive::prepareToPack(const std::string & path) {
	if (_packageFile.stream().is_open() || _status != kStatusClosed) {
		throw base::InvalidStateException("Trying to open an archive that is already opened");
	}
	if (!_packageFile.open(path, base::VwglbUtils::kModeWrite)) {
		throw base::WriteFileException("Could not open the archive file to write");
	}
}

void Archive::prepareToUnpack(const std::string & path) {
	if (_packageFile.stream().is_open() || _status != kStatusClosed) {
		throw base::InvalidStateException("Trying to open an archive that is already opened");
	}
	if (!_packageFile.open(path, base::VwglbUtils::kModeRead)) {
		throw base::NoSuchFileException("Could not open archive file to read");
	}
}

}
}