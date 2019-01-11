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

#ifndef _bg2e_base_effect_hpp_
#define _bg2e_base_effect_hpp_

#include <bg/base/context_object.hpp>
#include <bg/base/matrix_state.hpp>
#include <bg/base/poly_list.hpp>
#include <bg/base/material.hpp>

#include <string>

#include <bg/android/cplusplus_support.hpp>

#include <bg/engine/effect_impl.hpp>

namespace bg {
namespace base {

class BG2E_EXPORT Effect : public ContextObject, public ReferencedPointer {
public:
	Effect(bg::base::Context *);
	
	inline void setMatrixState(bg::base::MatrixState * s) { if (s) _matrixState = s; }
	inline bg::base::MatrixState & matrixState() { return *(_matrixState.getPtr()); }
	inline const bg::base::MatrixState & matrixState() const { return *(_matrixState.getPtr()); }
	
	inline void setMaterial(bg::base::Material * mat) { if (mat) _material = mat; }
	inline bg::base::Material & material() { return *(_material.getPtr()); }
	inline const bg::base::Material & material() const { return *(_material.getPtr()); }
	inline bg::base::Material * materialPtr() { return _material.getPtr(); }
	inline const bg::base::Material * materialPtr() const { return _material.getPtr(); }

    /*
     *  Update memory structures in graphic card, before begin draw
     *  This function is used only in new generation APIs (Vulkan, DX12, etc)
     */
    virtual void frame(float,bg::base::PolyList *) {}

	/*
	 *	Called when the effect about to be used
	 */
	virtual void activate() = 0;
	
	/*
	 *	Called when a poly list is attached to the effect
	 */
	virtual void bindPolyList(bg::base::PolyList *) = 0;
	
	/*
	 *	Called when a poly list is detached from the effect
	 */
	virtual void unbind() = 0;

    template <class T>
    inline T * impl() { return dynamic_cast<T*>(_impl.getPtr()); }
    
protected:
	virtual ~Effect();

	bg::ptr<bg::base::MatrixState> _matrixState;
	bg::ptr<bg::base::Material> _material;
    
    bg::ptr<bg::engine::EffectImpl> _impl;
};

}
}

#endif
