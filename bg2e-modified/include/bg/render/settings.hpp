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


#ifndef _bg2e_render_settings_hpp_
#define _bg2e_render_settings_hpp_

#include <string>

#include <bg/math/vector.hpp>

namespace bg {
namespace render {

typedef std::string SettingsKey;

class RenderSettings {
public:
	// The derivated settings class must specify as name the same name as the class with cammel case notation
	RenderSettings(const SettingsKey & name) :_settingsCategory(name) {}

	inline const SettingsKey & settingsCategory() const { return _settingsCategory; }

	// The settings keys must match the getter function
	virtual void setSettingValue(const SettingsKey & key, const std::string & value) {}
	virtual void setSettingValue(const SettingsKey & key, bool value) {}
	virtual void setSettingValue(const SettingsKey & key, int value) {}
	virtual void setSettingValue(const SettingsKey & key, float value) {}
    virtual void setSettingValue(const SettingsKey & key, const bg::math::Scalar & s) { setSettingValue(key, s.value()); }
	virtual void setSettingValue(const SettingsKey & key , const bg::math::Vector2 & value) {}
	virtual void setSettingValue(const SettingsKey & key, const bg::math::Vector3 & value) {}
	virtual void setSettingValue(const SettingsKey & key, const bg::math::Vector4 & value) {}
	virtual void setSettingValue(const SettingsKey & key, const bg::math::Size2Di & value) {}

	virtual const std::string & stringSettingValue(const SettingsKey & key) const { return _emptyString; }
	virtual bool boolSettingValue(const SettingsKey & key) const { return false; }
	virtual int intSettingValue(const SettingsKey & key) const { return -1; }
	virtual float floatSettingValue(const SettingsKey & key) const { return -1.0f; }
	virtual const bg::math::Vector2 & vec2SettingValue(const SettingsKey & key) const { return _emptyVec2; }
	virtual const bg::math::Vector3 & vec3SettingValue(const SettingsKey & key) const { return _emptyVec3; }
	virtual const bg::math::Vector4 & vec4SettingValue(const SettingsKey & key) const { return _emptyVec4; }
	virtual const bg::math::Size2Di sizeSettingValue(const SettingsKey & key) const { return _emptySize; }

protected:
	std::string _settingsCategory;
	std::string _emptyString;
	bg::math::Vector2 _emptyVec2;
	bg::math::Vector3 _emptyVec3;
	bg::math::Vector4 _emptyVec4;
	bg::math::Size2Di _emptySize;
};

}
}

#endif
