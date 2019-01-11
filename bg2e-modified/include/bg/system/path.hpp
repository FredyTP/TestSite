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

#ifndef _bg2e_path_hpp_
#define _bg2e_path_hpp_

#include <bg/export.hpp>

#include <string>
#include <vector>
#include <functional>

namespace bg {
namespace system {

class BG2E_EXPORT Path {
public:
    /*
     *  Returns the current user's home directory in macOS and Windows, or the
     *  internal storage folder in Android.
     */
    static Path HomeDir();
    
    /*
     *  Returns the executable path. In OS X, the executable path
     *  is $(APP_BUNDLE)/Contents/MacOS
     */
    static Path ExecDir();
	
	/*
	 *	Returns the application resource directory. In macOS is the
	 *	Resources folder inside application bundle.
	 *	In iOS and Windows is the same as ExecDir
	 */
	static Path ResourcesDir();
	
	/*
	 *	Returns the executable bundle path. If the current platform does not
	 *	use application bundles, this will return the same value as ExecDir()
	 */
	static Path AppDir();
    
    /*
     *  Returns the system temporal directory
     */
    static Path TempDir();


    Path();
    Path(const std::string & path);
	Path(const Path & p);

    /*
     *  Set the path from a string
     */
    Path & setText(const std::string & path);
    
    /*
     *  Returns the full path as a string. In Windows, the first
     *  element may be the disk letter
     */
    const std::string & text() const { return _pathString; }
    
    /*
     *  Adds a path component at the end of the current path
     */
    Path & addComponent(const std::string & c);
    
    /*
     *  Removes the last path component
     */
    Path & removeLastComponent();
    
    /*
     *  Removes the last path component extension, if proceed
     */
    Path & removeExtension();
	
	/*
	 *	Adds a extension to the last path component
	 */
	Path & addExtension(const std::string & ext);
	
	/*
	 *  Returns a new path adding a path component
	 */
	Path pathAddingComponent(const std::string & c) const {
		Path newPath(*this);
		newPath.addComponent(c);
		return newPath;
	}
	
	/*
	 *  Returns a new path removing the last path component
	 */
	Path pathRemovingLastComponent() const {
		Path newPath(*this);
		newPath.removeLastComponent();
		return newPath;
	}
	
	/*
	 *  Returns a new path removing the path component extension, if proceed
	 */
	Path pathRemovingExtension() const {
		Path newPath(*this);
		newPath.removeExtension();
		return newPath;
	}
	
	/*
	 *	Returns a new path adding a extension to the last path component
	 */
	Path pathAddingExtension(const std::string & ext) const {
		Path newPath(*this);
		newPath.addExtension(ext);
		return newPath;
	}
	
	
    /*
     *  Returns true if the path exists
     */
    bool exists() const;
    
    /*
     *  Create the directory if not exist. It returns false if the
     *  directory cannot be created
     */
    bool create(bool recursive = true) const;
    
    /*
     *  Remove the directory or file pointed by this path. It returns false
     *  if the directory or file cannot be removed
     */
    bool remove() const;
	
	/*
	 *	Copy the file pointed by this path to 'dst' path
	 */
	bool copy(const Path & dst) const;
    
    /*
     *  Returns true if the path is an absolute path. In Windows, the path must
     *  start with a drive letter to be absolute
     */
    bool isAbsolute() const;
    
    /*
     *  Returns true if the path is pointing to a file
     */
    bool isFile() const;
    
    /*
     *  Returns true if the path is pointing to a directory
     */
    bool isDirectory() const;
    
    /*
     *  Iterate the directory (if the path is a directory) using a lambda closure
     */
    void list(std::function<void(const Path & file)> closure) const;
    
    /*
     *  Iterate the directory recursively (if the path is a directory) using a lambda closure
     */
    void listRecursive(std::function<void(const Path & file)> closure) const;

    /*
     *  Returns the last path component
     */
    std::string lastPathComponent() const;
    
    /*
     *  Returns the last path component without extension
     */
    std::string fileName() const;
    
    /*
     *  Returns the last path component extension in lower case, or ""
     */
    std::string extension() const;
    
    /*
     *  Returns the full path, excluding the las component
     */
    std::string filePath() const;

	/*
	 *	Returns the drive letter, in Windows
	 */
	const std::string & driveLetter() const { return _letter; }
    
    /*
     *  Assign operator
     */
    void operator =(const Path & other) {
        _path.clear();
        _pathString = other._pathString;
		_beginSlash = other._beginSlash;
		_letter = other._letter;
        for (auto item : other._path) {
            _path.push_back(item);
        }
    }
    
    /*
     *  Comparation operator
     */
    inline bool operator==(const Path & p) const {
        return p._pathString==_pathString;
    }
    
	inline bool operator!=(const Path & p) const {
		return p._pathString!=_pathString;
	}

protected:
    std::vector<std::string> _path;
    std::string _pathString;
	bool _beginSlash;
	std::string _letter;	// Used only on Windows platform
	
	void buildString();
	void listDirectory(const std::string & path, std::vector<std::string> & files, std::vector<std::string> & directories, bool clearVectors = true) const;
	void listFilesRecursive(const std::string & path, std::vector<std::string> & allFiles, bool clearVector = true, const std::string & basePath = "") const;

private:

    void standarize_path(const std::string & inputPath, std::string & outputPath) const;
    std::string add_path_component(const std::string & base, const std::string & append) const;
};

}
}

#endif	// _bg2e_export_hpp_


