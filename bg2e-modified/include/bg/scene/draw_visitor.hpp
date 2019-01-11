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

#ifndef _bg2e_scene_draw_visitor_hpp_
#define _bg2e_scene_draw_visitor_hpp_

#include <bg/export.hpp>
#include <bg/scene/node_visitor.hpp>
#include <bg/base/pipeline.hpp>
#include <bg/base/render_queue.hpp>

namespace bg {
namespace scene {

class BG2E_EXPORT DrawVisitor : public NodeVisitor {
public:

	DrawVisitor();
	DrawVisitor(bg::base::Pipeline *);

    
    // TODO: Use render queues. Probably the pipeline will not be
    // necesary any more after implement this using render queues
	inline void setPipeline(bg::base::Pipeline * pl) { _pipeline = pl; }
	inline bg::base::Pipeline * pipeline() { return _pipeline.getPtr(); }
	inline const bg::base::Pipeline * pipeline() const { return _pipeline.getPtr(); }
 
    inline bg::base::RenderQueue & renderQueue() { return _renderQueue; }
    inline const bg::base::RenderQueue & renderQueue() const { return _renderQueue; }

	virtual void visit(Node*);
	virtual void didVisit(Node *);

protected:
	virtual ~DrawVisitor();

    // TODO: Render queues
	bg::ptr<bg::base::Pipeline> _pipeline;
 
     bg::base::RenderQueue _renderQueue;
};

}
}

#endif
