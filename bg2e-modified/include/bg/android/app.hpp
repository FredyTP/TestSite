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
#ifndef _bg2e_android_app_hpp_
#define _bg2e_android_app_hpp_

#include <bg/platform.hpp>
#include <bg/android/cplusplus_support.hpp>
#include <bg/android/asset.hpp>

#if BG2E_ANDROID==1

#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include <bg/math/vector.hpp>
#include <bg/system/path.hpp>
#include <bg/base/buffer.hpp>

namespace bg {
namespace android {



class App {
public:
	static App & Get() {
        if (s_app==nullptr) {
            s_app = new App();
        }
		return *s_app;
	}

	void init(JNIEnv * env, jobject assetManagerObj, jstring internalStoragePath, jstring externalStoragePath, int displayWidth, int displayHeight, float displayDensity);

	inline const bg::system::Path & internalStoragePath() const { return _internalStoragePath; }
	inline const bg::system::Path & externalStoragePath() const { return _externalStoragePath; }
    inline const bg::math::Size2Di & displaySize() const { return _displaySize; }
    inline float displayDensity() const { return _displayDensity; }

	bg::base::Buffer * getAssetBuffer(const std::string & asset);

    bg::android::Asset * openAsset(const std::string & assetName);

	/*
	 *  Extract an asset file and put it into dst.
	 *  If forceExtract==false, the asset will not be extracted if there is a file with the same name
	 *  at the destination path. IF forceExtract==true, and there is a file with the same name at
	 *  destination path, the existing file will be overwritten.
	 */
	void extractAsset(const std::string & assetName, const bg::system::Path & dst, bool forceExtract = false);

	/*
	 * 	Extract all assets from the specified asset subpath to the specified destination path. If assetDir is
	 * 	an empty string, all the files in the asset root will be extracted. Note that this function is not
	 * 	recursive
	 * 	If forceExtract==true, the assets will be overwritten if exists in dst
	 */
	void extractAllAssets(const bg::system::Path & dst, const std::string & subpath = "", bool forceExtract = false);

	void logDebug(const std::string & msg);
	void logError(const std::string & msg);
	void logWarning(const std::string & msg);
	void logVerbose(const std::string & msg);

	std::string toStdString(jstring str);

protected:
	App();
	virtual ~App();

	static App * s_app;

	JNIEnv * _env;
	AAssetManager * _assetManager;
	bg::system::Path _internalStoragePath;
	bg::system::Path _externalStoragePath;
    bg::math::Size2Di _displaySize;
    float _displayDensity;
};

}
}

#else


namespace bg {
namespace android {

class App {
public:
	static App & Get() {
		throw bg::base::CompatibilityException("Trying to use bg::android::App singleton outside android application.");
	}

	inline const bg::system::Path & internalStoragePath() const { return _internalStoragePath; }
	inline const bg::system::Path & externalStoragePath() const { return _externalStoragePath; }

	bg::base::Buffer * getAssetBuffer(const std::string & asset) { return nullptr; }

	void extractAsset(const std::string &, const bg::system::Path &) {}
	void extractAllAssets(const bg::system::Path &, const std::string & subpath = "", bool forceExtract = false) {}

	void logDebug(const std::string & msg) {}
	void logError(const std::string & msg) {}
	void logWarning(const std::string & msg) {}
	void logVerbose(const std::string & msg) {}

protected:
	App() {}
	virtual ~App() {}
	bg::system::Path _internalStoragePath;
	bg::system::Path _externalStoragePath;
};

}
}

#endif



#endif
