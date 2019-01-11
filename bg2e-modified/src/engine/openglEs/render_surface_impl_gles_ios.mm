
#include <bg/engine/openglEs/render_surface_impl.hpp>
#include <bg/engine/opengl_es_3.hpp>
#include <bg/ios/gles_context.hpp>

#import <GLKit/GLKit.h>

namespace bg {
namespace engine {
namespace openglEs {


#if BG2E_IOS==1
void RenderSurfaceImpl::create() {
	openglCore::RenderSurfaceImpl::create();
	if (_target==kTargetTexture) {
		bg::ios::GLESContext * ctx = dynamic_cast<bg::ios::GLESContext*>(_renderSurface->context());
		if (ctx) {
			GLKView * view = (__bridge GLKView*) ctx->view();
			if (view) {
				[view bindDrawable];
			}
		}
	}
}

void RenderSurfaceImpl::setActive() {
	openglCore::RenderSurfaceImpl::setActive();
	if (_target==kTargetScreen) {
		bg::ios::GLESContext * ctx = dynamic_cast<bg::ios::GLESContext*>(_renderSurface->context());
		if (ctx) {
			GLKView * view = (__bridge GLKView*) ctx->view();
			if (view) {
				[view bindDrawable];
			}
		}
	}
}

void RenderSurfaceImpl::destroy() {
	if (_fbo) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		bg::ios::GLESContext * ctx = dynamic_cast<bg::ios::GLESContext*>(_renderSurface->context());
		if (ctx) {
			GLKView * view = (__bridge GLKView*) ctx->view();
			if (view) {
				[view bindDrawable];
			}
		}
		glDeleteBuffers(1, &_fbo);
		_fbo = 0;
		for (auto att : _attachments) {
			if (att.texture.valid()) {
				att.texture->destroy();
			}
			else if (att.renderbuffer) {
				glDeleteRenderbuffers(1, &att.renderbuffer);
			}
		}
		_attachments.clear();
	}
}
#endif

}
}
}