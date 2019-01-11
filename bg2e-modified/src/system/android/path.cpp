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
#include <bg/android/app.hpp>

#include <sstream>

#include <fstream>

// TODO: Implement this file

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

namespace bg {
namespace system {

static bool is_file(const char * path) {
    struct stat path_stat;
    bool exists = stat(path,&path_stat)==0;
    bool isReg = S_ISREG(path_stat.st_mode);
    bool isDir = S_ISDIR(path_stat.st_mode);
    return exists && isReg && !isDir;
}

static bool is_directory(const char * path) {
    struct stat statbuf;
    bool exists = stat(path, &statbuf)==0;
    bool isDir = S_ISDIR(statbuf.st_mode);
    return exists && isDir;
}


static void rec_mkdir(const char * dir) {
    char tmp[PATH_MAX];
    char * p = nullptr;
    size_t len;

    snprintf(tmp,sizeof(tmp),"%s",dir);
    len = strlen(tmp);
    if (tmp[len-1] == '/') {
        tmp[len - 1] = 0;
    }
    for (p = tmp + 1; *p; ++p) {
        if (*p=='/') {
            *p = 0;
            mkdir(tmp, S_IRWXU);
            *p = '/';
        }
    }
    mkdir(tmp, S_IRWXU);
}

// src: can be a file or a directory.
// dstDir: MUST be a directory
static bool rec_copy(const char * src, const char * dstDir) {
    Path dstPath(dstDir);
    Path srcPath(src);

    if (!dstPath.isDirectory()) return false;
    if (srcPath.isFile()) {
        std::string dstFileName = srcPath.lastPathComponent();
        std::ifstream srcFile(src,std::ios::binary);
        std::ofstream dstFile(dstPath.pathAddingComponent(dstFileName).text(),std::ios::binary);
        if (srcFile.is_open() && dstFile.is_open()) {
            dstFile << srcFile.rdbuf();
            srcFile.close();
            dstFile.close();
            return true;
        }
        else {
            return false;
        }
    }
    else {
        // Create new destination directory
        Path newDirPath = dstPath.pathAddingComponent(srcPath.lastPathComponent());
        mkdir(newDirPath.text().c_str(),S_IRWXU);

        // Copy directory contents
        DIR * dir;
        struct dirent * entry;
        char path[PATH_MAX];
        dir = opendir(src);
        while ((entry = readdir(dir))!=nullptr) {
            if (strcmp(entry->d_name, ".") && strcmp(entry->d_name,"..")) {
                bool status = rec_copy(srcPath.pathAddingComponent(entry->d_name).text().c_str(),
                         newDirPath.text().c_str());
                if (!status) return false;
            }
        }
        return true;
    }
}

static void rec_rmdir(const char * dirname) {
    DIR * dir;
    struct dirent * entry;
    char path[PATH_MAX];
    dir = opendir(dirname);
    if (dir==nullptr) {
        // Directory already removed or is not a directory
        return;
    }

    while ((entry = readdir(dir))!=nullptr) {
        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
            snprintf(path, static_cast<size_t>(PATH_MAX), "%s/%s", dirname, entry->d_name);
            if (entry->d_type == DT_DIR) {
                rec_rmdir(path);
            }
            else {
                unlink(path);
            }
        }
    }
    closedir(dir);
    rmdir(dirname);
}

Path Path::HomeDir() {
    return Path(bg::android::App::Get().internalStoragePath());
}

Path Path::ResourcesDir() {
    return Path(bg::android::App::Get().internalStoragePath());
}

Path Path::ExecDir() {
    return Path(bg::android::App::Get().internalStoragePath());
}

Path Path::TempDir() {
    Path path(bg::android::App::Get().internalStoragePath());
    path.addComponent("temp");
    return path;
}

bool Path::exists() const {
	return isDirectory() || isFile();
}

bool Path::create(bool recursive) const {
    if (isDirectory()) {
        return true;
    }
    else if (isFile()) {
        return false;
    }
    else if (recursive) {
        rec_mkdir(text().c_str());
    }
    else {
        mkdir(text().c_str(), S_IRWXU);
    }
	return isDirectory();
}

bool Path::remove() const {
    rec_rmdir(text().c_str());
	return !isDirectory();
}

bool Path::copy(const Path & dst) const {
    if (!exists()) {
        return false;
    }

    // Prepare destination
    if (dst.isFile()) {
        return false;
    }
    else if (!dst.exists()) {
        dst.create(true);
    }

    return rec_copy(text().c_str(), dst.text().c_str());
}

bool Path::isFile() const {
    return is_file(text().c_str());
}

bool Path::isDirectory() const {
    return is_directory(text().c_str());
}

void Path::listDirectory(const std::string & path, std::vector<std::string> & files, std::vector<std::string> & directories, bool clearVectors) const {
    Path p(path);
    if (!p.isDirectory()) return;

    if (clearVectors) {
        files.clear();
        directories.clear();
    }

    DIR * dir;
    struct dirent * entry;
    dir = opendir(path.c_str());
    while ((entry = readdir(dir))!=nullptr) {
        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name,"..")) {
            Path entryPath = p.pathAddingComponent(entry->d_name);
            if (entryPath.isDirectory()) {
                directories.push_back(entry->d_name);
            }
            else {
                files.push_back(entry->d_name);
            }
        }
    }
}

}
}