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

#include <bg/android/app.hpp>
#include <bg/base/exception.hpp>

#include <fstream>
#include <bg/log.hpp>

namespace bg {
namespace android {

#define LOG_TAG "bg2engine"

App * App::s_app = nullptr;

App::App()
	:_env(nullptr)
	,_assetManager(nullptr)
{

}

App::~App() {

}

void App::init(JNIEnv * env, jobject assetManagerObj, jstring internalStoragePath, jstring externalStoragePath, int displayWidth, int displayHeight, float displayDensity) {
	_env = env;
	_assetManager = AAssetManager_fromJava(env,assetManagerObj);
    std::string inPath = toStdString(internalStoragePath);
    std::string exPath = toStdString(externalStoragePath);
	_internalStoragePath = bg::system::Path(inPath);
	_externalStoragePath = bg::system::Path(exPath);
    _displaySize.set(displayWidth,displayHeight);
    _displayDensity = displayDensity;
}

bg::base::Buffer * App::getAssetBuffer(const std::string & asset) {
	AAsset * file = AAssetManager_open(_assetManager, asset.c_str(), AASSET_MODE_BUFFER);
	if (!file) {
		throw bg::base::NoSuchFileException("Asset not found");
	}
	size_t length = AAsset_getLength(file);
	char * contents = new char[length];
	AAsset_read(file,contents,length);
	bg::ptr<bg::base::Buffer> buffer = new bg::base::Buffer(contents, length);
	return buffer.release();
}

bg::android::Asset * App::openAsset(const std::string & assetName) {
    bg::ptr<Asset> result;
    AAsset * file = AAssetManager_open(_assetManager, assetName.c_str(), AASSET_MODE_UNKNOWN);
    if (file==nullptr) {
        throw bg::base::NoSuchFileException("Asset not found");
    }
    off_t start;
    off_t length;
    int32_t desc = AAsset_openFileDescriptor(file,&start,&length);
    AAsset_close(file);
    result = new Asset(desc,start,length);
    return result.release();
}

void App::extractAsset(const std::string & assetName, const bg::system::Path & dst, bool forceExtract) {
	bg::system::Path finalDestPath = dst;
	bg::system::Path dstFilePath = dst.pathAddingComponent(assetName);
	bg::system::Path assetSubpath(assetName);
	assetSubpath.removeLastComponent();
	if (!assetSubpath.text().empty()) {
		finalDestPath.addComponent(assetSubpath.text());
	}

	if (!finalDestPath.exists()) {
		finalDestPath.create(true);
	}

	if (dstFilePath.isDirectory()) {
		throw bg::base::InvalidStateException("Could not extract asset. There is a directory at the testintation path with the same name.");
	}
	if (!dstFilePath.exists() || forceExtract) {
		bg::ptr<bg::base::Buffer> buffer = getAssetBuffer(assetName);
		std::ofstream dstFile(dstFilePath.text(), std::ios::binary);
		if (!dstFile.good()) {
			throw bg::base::WriteFileException("Could not write asset to the specified destination path.");
		}
		dstFile.write(buffer->buffer(), buffer->size());
		if (dstFile.bad()) {
			throw bg::base::WriteFileException("Could not write asset data. Maybe the destination drive is full?");
		}
		dstFile.close();
	}
}

void App::extractAllAssets(const bg::system::Path & dst, const std::string & subpath, bool forceExtract) {
	AAssetDir * assetDir = AAssetManager_openDir(_assetManager, subpath.c_str());
	const char * fileName;
	while ((fileName = AAssetDir_getNextFileName(assetDir)) != nullptr) {
		bg::system::Path filePath = dst;
		bg::system::Path srcPath(subpath);
		srcPath.addComponent(fileName);
		extractAsset(srcPath.text(),filePath,forceExtract);
	}
	AAssetDir_close(assetDir);
}

void App::logDebug(const std::string & msg) {
	__android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,"%s",msg.c_str());
}

void App::logError(const std::string & msg) {
	__android_log_print(ANDROID_LOG_ERROR,LOG_TAG,"%s",msg.c_str());
}

void App::logWarning(const std::string & msg) {
	__android_log_print(ANDROID_LOG_WARN,LOG_TAG,"%s",msg.c_str());
}

void App::logVerbose(const std::string & msg) {
	__android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG,"%s",msg.c_str());
}

std::string App::toStdString(jstring str) {
	const jsize len = _env->GetStringUTFLength(str);
	const char * strChars = _env->GetStringUTFChars(str,(jboolean*)0);
	std::string result(strChars,len);
	_env->ReleaseStringUTFChars(str,strChars);
	return result;
}


}
}