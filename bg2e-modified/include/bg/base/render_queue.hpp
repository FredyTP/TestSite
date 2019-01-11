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

#ifndef _bg2e_base_render_queue_hpp_
#define _bg2e_base_render_queue_hpp_

#include <bg/base/poly_list.hpp>
#include <bg/base/material.hpp>
#include <bg/math/matrix.hpp>

#include <vector>
#include <algorithm>

namespace bg {
namespace base {

class RenderQueue {
public:
    struct RenderQueueObject {
        PolyList * plist;
        Material * material;
        bg::math::Matrix4 modelMatrix;
        float cameraDistance;
    };
    
    typedef std::vector<RenderQueueObject> Queue;

    inline void beginFrame(const bg::math::Vector3 & worldCameraPosition) { _opaqueQueue.clear(); _transparentQueue.clear(); _worldCameraPosition = worldCameraPosition; }
    
    inline void renderOpaque(bg::base::PolyList * pl, bg::base::Material * m, const bg::math::Matrix4 & t) { _opaqueQueue.push_back({ pl, m, t }); }
    inline void renderTransparent(bg::base::PolyList * pl, bg::base::Material * m, const bg::math::Matrix4 & t) {
        _transparentQueue.push_back({ pl, m, t, (t.position() - _worldCameraPosition).magnitude() });
    }
    
    inline void sortTransparentObjects() {
        std::sort(_transparentQueue.begin(), _transparentQueue.end(), [](const RenderQueueObject & a, const RenderQueueObject & b) {
            return a.cameraDistance > b.cameraDistance;
        });
    }
    
    inline const Queue & opaqueQueue() const { return _opaqueQueue; }
    inline const Queue & transparentQueue() const { return _transparentQueue; }

protected:
    Queue _opaqueQueue;
    Queue _transparentQueue;
    bg::math::Vector3 _worldCameraPosition;
};

}
}
#endif
