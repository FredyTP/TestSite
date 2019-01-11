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


#include <bg/tools/poly_list_cache.hpp>
#include <bg/engine.hpp>

namespace bg {
namespace tools {

bg::base::ContextObjectRegistry<PolyListCache> PolyListCache::s_registry;
bool PolyListCache::s_destroyCallbackRegistered = false;

PolyListCache::PolyListCache(bg::base::Context * ctx)
	:bg::base::ContextObject(ctx)
{
}

PolyListCache::~PolyListCache() {
}

PolyListCache * PolyListCache::Get(bg::base::Context * ctx) {
	if (ctx==nullptr) throw bg::base::NullParameterException("Invalid parameter: trying to get TextureCache of null context.");
	if (!s_registry.find(ctx)) {
		s_registry.set(new PolyListCache(ctx));
	}
	if (!s_destroyCallbackRegistered) {
		s_destroyCallbackRegistered = true;
		bg::Engine::AddDestroyCallback([&]() {
			s_registry.clear();
		});
	}
	return s_registry.get(ctx);
}

void PolyListCache::RegisterPolyList(const std::string & name, bg::base::PolyList * plist) {
	Get(bg::base::ContextObject::GetContext(plist))->registerPolyList(name, plist);
}
	
bg::base::PolyList * PolyListCache::GetPolyList(bg::base::Context * ctx, const std::string & text) {
	return Get(ctx)->getPolyList(text);
}

bool PolyListCache::UnregisterPolyList(bg::base::Context * ctx, const std::string & name) {
	return Get(ctx)->unregisterPolyList(name);
}

bool PolyListCache::UnregisterPolyList(bg::base::PolyList * plist) {
	if (!plist) throw bg::base::NullParameterException("Invalid parameter: trying to unregister a null poly list.");
	return Get(plist->context())->unregisterPolyList(plist);
}

// Non-static versions of the previous functions
void PolyListCache::registerPolyList(const std::string & name, bg::base::PolyList * plist) {
	if (name.empty()) {
		throw bg::base::InvalidParameterException("Invalid parameter: trying to register a poly list using an empty key.");
	}
	else if (!plist) {
		throw bg::base::InvalidParameterException("Invalid parameter: trying to register a null poly list.");
	}
	
	_plistMap[name] = plist;
}

bg::base::PolyList * PolyListCache::getPolyList(const std::string & text) {
	return _plistMap[text].getPtr();
}

bool PolyListCache::unregisterPolyList(const std::string & name) {
	if (name.empty()) throw bg::base::NullParameterException("Invalid parameter: trying to unregister a poly list with an empty key");
	PolyListMap::iterator it = _plistMap.find(name);
	if (it!=_plistMap.end()) {
		_plistMap.erase(it);
		return true;
	}
	return false;
}

bool PolyListCache::unregisterPolyList(bg::base::PolyList * plist) {
	if (!plist) throw bg::base::NullParameterException("Invalid parameter: trying to unregister a null poly list.");
	auto it = _plistMap.begin();
	for (auto plistMapItem : _plistMap) {
		if (plistMapItem.second.getPtr()==plist) {
			break;
		}
		++it;
	}
	if (it!=_plistMap.end()) {
		_plistMap.erase(it);
		return true;
	}
	return false;
}
	

}
}