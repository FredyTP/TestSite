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
#include <bg/system/path.hpp>

#include <sstream>

#ifndef WIN32
#define WIN32
#endif
#include <WinSock2.h>
#include <Windows.h>
#include <Shlobj.h>

#include <fstream>

#define PATH_SIZE 2048

namespace bg {
namespace system {

bool win32_createDirectoryRecursively(const std::string &directory) {
	static const std::string separators("\\/");

	// If the specified directory name doesn't exist, do our thing
	DWORD fileAttributes = ::GetFileAttributesA(directory.c_str());
	if (fileAttributes == INVALID_FILE_ATTRIBUTES) {

		// Recursively do it all again for the parent directory, if any
		std::size_t slashIndex = directory.find_last_of(separators);
		if (slashIndex != std::wstring::npos) {
			if (!win32_createDirectoryRecursively(directory.substr(0, slashIndex))) {
				return false;
			}
		}

		// Create the last directory on the path (the recursive calls will have taken
		// care of the parent directories by now)
		BOOL result = ::CreateDirectoryA(directory.c_str(), nullptr);
		if (result == FALSE) {
			return false;
		}

	}
	else { // Specified directory name already exists as a file or directory

		bool isDirectoryOrJunction =
			((fileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) ||
			((fileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0);

		if (!isDirectoryOrJunction) {
			return false;
		}

	}
	return true;
}

bool win32_deleteDirectoryRecursively(const std::string & directory) {
#pragma warning(push)

#pragma warning(disable: 4996)
	char * dir = new char[directory.size() + 2];
	memset(dir, 0, directory.size() + 2);
	directory.copy(dir, directory.size());

	SHFILEOPSTRUCTA file_op = {
			NULL,
			FO_DELETE,
			dir,
			"",
			FOF_NOCONFIRMATION |
			FOF_NOERRORUI |
			FOF_SILENT,
			false,
			0,
			"" };
	SHFileOperationA(&file_op);
	delete[] dir;
#pragma warning(pop)
	return true;
}

void copy_file(const std::string & srce_file, const std::string & dest_file)
{
	std::ifstream srce(srce_file, std::ios::binary);
	std::ofstream dest(dest_file, std::ios::binary);
	dest << srce.rdbuf();
}


static std::string s_execPath;
static std::string s_homePath;
static std::string s_resourcesPath;
static std::string s_tempPath;

Path Path::HomeDir() {
	if (s_homePath == "") {
		WCHAR path[PATH_SIZE];
		if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, path))) {
			std::wstring ws(path);
			s_homePath = std::string(ws.begin(), ws.end());
		}
		if (s_homePath.back() != '/') s_homePath += '/';
	}
	return Path(s_homePath);
}

Path Path::ResourcesDir() {
	if(s_resourcesPath == "") {
		std::vector<char> resPath(PATH_SIZE);
		DWORD result = GetModuleFileNameA(nullptr, &resPath[0], static_cast<DWORD>(resPath.size()));
		s_resourcesPath = std::string(resPath.begin(), resPath.begin() + result);
		Path tmpPath(s_resourcesPath);
		tmpPath.removeLastComponent();
		s_resourcesPath = tmpPath.text();
	}
	Path p(s_resourcesPath);
	return p;
}

Path Path::ExecDir() {
	if (s_execPath == "") {
		std::vector<char> execPath(PATH_SIZE);
		DWORD result = GetModuleFileNameA(nullptr, &execPath[0], static_cast<DWORD>(execPath.size()));
		s_execPath = std::string(execPath.begin(), execPath.begin() + result);
		if (s_execPath.back() != '/') s_execPath += '/';
	}
	Path p(s_execPath);
	p.removeLastComponent();
	return p;
}

Path Path::TempDir() {
	if (s_tempPath == "") {
		std::vector<char> tmpPath(PATH_SIZE);
		DWORD result = GetTempPathA(static_cast<DWORD>(tmpPath.size()), &tmpPath[0]);
		s_tempPath = std::string(tmpPath.begin(), tmpPath.begin() + result);
		s_tempPath += "bg2e-tmp/";
	}
	Path p(s_tempPath);
	p.create(true);
	return p;
}

bool Path::exists() const {
	return GetFileAttributesA(_pathString.c_str()) != 0xFFFFFFFF;
}

bool Path::create(bool recursive) const {
	if (isDirectory()) return true;
	if (recursive) {
		return win32_createDirectoryRecursively(_pathString);
	}
	else {
		return ::CreateDirectoryA(_pathString.c_str(), nullptr)!=0;
	}
}

bool Path::remove() const {
	if (exists()) {
		return win32_deleteDirectoryRecursively(_pathString);
	}
	return false;
}

bool Path::copy(const Path & dst) const {
	if (exists()) {
		copy_file(_pathString, dst.text());
		return true;
	}
	return false;
}

bool Path::isFile() const {
	return exists() && !isDirectory();
}

bool Path::isDirectory() const {
	return GetFileAttributesA(_pathString.c_str())==FILE_ATTRIBUTE_DIRECTORY;
}

void Path::listDirectory(const std::string & path, std::vector<std::string> & files, std::vector<std::string> & directories, bool clearVectors) const {
	Path p(path);
	if (!p.isDirectory()) return;
	p.addComponent("*");

	HANDLE hFind;
	WIN32_FIND_DATAA FindFileData;

	if ((hFind = FindFirstFileA(p.text().c_str(), &FindFileData)) != INVALID_HANDLE_VALUE) {
		do {
			std::string file(FindFileData.cFileName);
			if (file == "." || file == "..") {
				continue;
			}
			else if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				directories.push_back(file);
			}
			else {
				files.push_back(file);
			}
		} while (FindNextFileA(hFind, &FindFileData));
		FindClose(hFind);
	}
}

}
}