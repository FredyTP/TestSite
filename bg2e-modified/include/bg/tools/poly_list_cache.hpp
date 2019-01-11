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


#ifndef _bg2e_tools_poly_list_cache_hpp_
#define _bg2e_tools_poly_list_cache_hpp_

#include <bg/base/poly_list.hpp>

namespace bg {
namespace tools {

typedef std::unordered_map<std::string, bg::ptr<bg::base::PolyList> > PolyListMap;

class BG2E_EXPORT PolyListCache : public bg::base::ContextObject, public bg::base::ReferencedPointer {
public:
	/*
	 *	Gets the poly list cache object, associated with the context
	 *	Throws bg::base::NullParameterException if the context is null
	 */
	static PolyListCache * Get(bg::base::Context *);
	
	/*
	 *	Register a poly list using a name, in its associated context. Throws bg::base::NullParameterException
	 *	if the poly list is null or empty (it doesn't contains any polylist).
	 */
	static void RegisterPolyList(const std::string & name, bg::base::PolyList *);
	
	/*
	 *	Returns the poly list associated with the key "text", or nullptr if there isn't a
	 *	poly list registered with this key.
	 *	Throws bg::base::NullParameterException if the context is null
	 */
	static bg::base::PolyList * GetPolyList(bg::base::Context *, const std::string & text);
	
	/*
	 *	Remove a poly list from the register. Returns fase if the poly list is not found
	 *	Throws bg::base::NullParameterException if the context is null
	 */
	static bool UnregisterPolyList(bg::base::Context *, const std::string & name);
	
	/*
	 *	Removes a poly list from the register, using the poly list pointer. Returns false if
	 *	the poly list is not found
	 *	Throws bg::base::NullParameterException if the poly list is null
	 */
	static bool UnregisterPolyList(bg::base::PolyList *);
	
	// Non-static versions of the previous functions
	void registerPolyList(const std::string & name, bg::base::PolyList *);
	bg::base::PolyList * getPolyList(const std::string & text);
	bool unregisterPolyList(const std::string & name);
	bool unregisterPolyList(bg::base::PolyList *);
	
protected:
	PolyListCache(bg::base::Context *);
	virtual ~PolyListCache();
	
	static bg::base::ContextObjectRegistry<PolyListCache> s_registry;
	static bool s_destroyCallbackRegistered;
	
	PolyListMap _plistMap;
};

}
}

#endif