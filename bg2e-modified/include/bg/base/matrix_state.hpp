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

#ifndef _bg2e_base_matrix_state_hpp_
#define _bg2e_base_matrix_state_hpp_

#include <bg/base/referenced_pointer.hpp>
#include <bg/base/matrix_stack.hpp>

#include <iostream>

namespace bg {
namespace base {

class MatrixState : public ReferencedPointer {
public:
	static MatrixState * Current() {
		if (!s_matrixState.valid()) {
			s_matrixState = new MatrixState();
		}
		return s_matrixState.getPtr();
	}

	static MatrixState * SetCurrent(MatrixState * s) {
		s_matrixState = s;
		return s_matrixState.getPtr();
	}
	
	MatrixState() :_viewMatrixInvertChanged(true) {}

	inline MatrixStack & modelMatrixStack() { return _modelMatrixStack; }
	inline const MatrixStack & modelMatrixStack() const { return _modelMatrixStack; }
	inline MatrixStack & viewMatrixStack() { return _viewMatrixStack; }
	inline const MatrixStack & viewMatrixStack() const { return _viewMatrixStack; }
	inline MatrixStack & projectionMatrixStack() { return _projectionMatrixStack; }
	inline const MatrixStack & projectionMatrixStack() const { return _projectionMatrixStack; }
	inline const bg::math::Matrix4 & modelViewMatrix() {
		if (_modelMatrixStack.changed() || _viewMatrixStack.changed()) {
			_modelViewMatrix = _viewMatrixStack.matrix();
			_modelViewMatrix.mult(_modelMatrixStack.matrix());
			_modelMatrixStack.setChanged(false);
			_viewMatrixStack.setChanged(false);
		}
		return _modelViewMatrix;
	}
 
    inline const bg::math::Matrix4 & normalModelMatrix() {
        calculateNormalMatrix();
        return _normalModelMatrix;
    }
    
    inline const bg::math::Matrix4 & normalModelViewMatrix() {
        calculateNormalMatrix();
        return _normalModelViewMatrix;
    }


    #if BG2E_WINDOWS==1
	__declspec(deprecated("Use normalModelViewMatrix() instead.")) inline const bg::math::Matrix4 & normalMatrix() { return normalModelViewMatrix(); }
    #else
    inline const bg::math::Matrix4 & normalMatrix() __attribute__((deprecated("Use normalModelViewMatrix() instead."))) { return normalModelViewMatrix(); }
    #endif

	inline const bg::math::Matrix4 & viewMatrixInvert() {
		if (_viewMatrixStack.changed() || _viewMatrixInvertChanged) {
			_viewMatrixInvert = viewMatrixStack().matrix();
			_viewMatrixInvert.invert();
			_viewMatrixInvertChanged = false;
		}
		return _viewMatrixInvert;
	}
	
protected:
	static bg::ptr<bg::base::MatrixState> s_matrixState;
	virtual ~MatrixState() {}
	
	MatrixStack _modelMatrixStack;
	MatrixStack _viewMatrixStack;
	MatrixStack _projectionMatrixStack;
	math::Matrix4 _modelViewMatrix;
    
	math::Matrix4 _normalModelViewMatrix;
    math::Matrix4 _normalModelMatrix;
    
	
	bool _viewMatrixInvertChanged;
	math::Matrix4 _viewMatrixInvert;
 
    inline void calculateNormalMatrix() {
        if (_modelMatrixStack.changed() || _viewMatrixStack.changed()) {
            _normalModelViewMatrix = modelViewMatrix();
            _normalModelViewMatrix.invert();
            _normalModelViewMatrix.transpose();
            
            _normalModelMatrix = modelMatrixStack().matrix();
            _normalModelMatrix.invert();
            _normalModelMatrix.transpose();
            
            _modelMatrixStack.setChanged(false);
            _viewMatrixStack.setChanged(false);
        }
    }
};
	
}
}
#endif
