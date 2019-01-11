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


#include <bg/render/renderer.hpp>


#include <bg/render/forward_renderer.hpp>
#include <bg/render/deferred_renderer.hpp>
#include <bg/render/low_overhead_forward_renderer.hpp>

namespace bg {
namespace render {

Renderer * Renderer::Create(bg::base::Context * ctx, RenderPath path) {
	bg::ptr<Renderer> result;

    if (bg::Engine::IsVulkan()) {
        if (path==kRenderPathForward) {
            result = new LowOverheadForwardRenderer(ctx);
        }
        else {
            bg::log(bg::log::kWarning) << "Deferred render is implemented using Vulkan rendering engine" << bg::endl;
            result = new LowOverheadForwardRenderer(ctx);
        }
        
        if (result->isSupported()) {
            result->create();
        }
        else {
            throw bg::base::CompatibilityException("No suitable renderer found. Try to use another rendering engine.");
        }
    }
    else {
        if (path==kRenderPathDeferred) {
            result = new DeferredRenderer(ctx);
        }
        
        if (path==kRenderPathForward ||
            (result.valid() && !result->isSupported()))
        {
            result = new ForwardRenderer(ctx);
        }
        
        if (result->isSupported()) {
            result->create();
        }
        else {
            throw bg::base::CompatibilityException("No suitable renderer found. Try to use another rendering engine.");
        }
    }
	
	return result.release();
}

Renderer::Renderer(bg::base::Context * ctx)
	:bg::base::ContextObject(ctx)
	,_frame(new bg::scene::FrameVisitor())
	,_autoInvalidateTextures(true)
{
}

Renderer::~Renderer() {
	
}

void Renderer::frame(bg::scene::Node * node, float delta) {
	_frame->setDelta(delta);
	node->accept(_frame.getPtr());
	bg::scene::Node::FlushSceneChanges();
}

void Renderer::setRenderSetting(const std::string & category, const std::string & key, const std::string & value) {
	for (auto setting : _settings) {
		if (setting.second->settingsCategory()==category) {
			setting.second->setSettingValue(key, value);
		}
	}
}

void Renderer::setRenderSetting(const std::string & category, const std::string & key, bool value) {
	for(auto setting : _settings) {
		if(setting.second->settingsCategory() == category) {
			setting.second->setSettingValue(key, value);
		}
	}
}

void Renderer::setRenderSetting(const std::string & category, const std::string & key, int value) {
	for(auto setting : _settings) {
		if(setting.second->settingsCategory() == category) {
			setting.second->setSettingValue(key, value);
		}
	}
}

void Renderer::setRenderSetting(const std::string & category, const std::string & key, float value) {
	for(auto setting : _settings) {
		if(setting.second->settingsCategory() == category) {
			setting.second->setSettingValue(key, value);
		}
	}
}
    
void Renderer::setRenderSetting(const std::string & category, const std::string & key, const bg::math::Scalar & value) {
    for(auto setting : _settings) {
        if(setting.second->settingsCategory() == category) {
            setting.second->setSettingValue(key, value);
        }
    }
}

void Renderer::setRenderSetting(const std::string & category, const std::string & key, const bg::math::Vector2 & value) {
	for(auto setting : _settings) {
		if(setting.second->settingsCategory() == category) {
			setting.second->setSettingValue(key, value);
		}
	}
}

void Renderer::setRenderSetting(const std::string & category, const std::string & key, const bg::math::Vector3 & value) {
	for(auto setting : _settings) {
		if(setting.second->settingsCategory() == category) {
			setting.second->setSettingValue(key, value);
		}
	}
}

void Renderer::setRenderSetting(const std::string & category, const std::string & key, const bg::math::Vector4 & value) {
	for(auto setting : _settings) {
		if(setting.second->settingsCategory() == category) {
			setting.second->setSettingValue(key, value);
		}
	}
}

void Renderer::setRenderSetting(const std::string & category, const std::string & key, const bg::math::Size2Di & value) {
	for(auto setting : _settings) {
		if(setting.second->settingsCategory() == category) {
			setting.second->setSettingValue(key, value);
		}
	}
}

const std::string & Renderer::stringSettingValue(const std::string & category, const std::string & key) const {
	for(auto setting : _settings) {
		if(setting.second->settingsCategory() == category) {
			return setting.second->stringSettingValue(key);
		}
	}
	return _emptyString;
}

bool Renderer::boolSettingValue(const std::string & category, const std::string & key) const {
	for(auto setting : _settings) {
		if(setting.second->settingsCategory() == category) {
			return setting.second->boolSettingValue(key);
		}
	}
	return false;
}

int Renderer::intSettingValue(const std::string & category, const std::string & key) const {
	for(auto setting : _settings) {
		if(setting.second->settingsCategory() == category) {
			return setting.second->intSettingValue(key);
		}
	}
	return -1;
}

float Renderer::floatSettingValue(const std::string & category, const std::string & key) const {
	for(auto setting : _settings) {
		if(setting.second->settingsCategory() == category) {
			return setting.second->floatSettingValue(key);
		}
	}
	return -1.0f;
}

const bg::math::Vector2 & Renderer::vec2SettingValue(const std::string & category, const std::string & key) const {
	for(auto setting : _settings) {
		if(setting.second->settingsCategory() == category) {
			return setting.second->vec2SettingValue(key);
		}
	}
	return _emptyVec2;
}

const bg::math::Vector3 & Renderer::vec3SettingValue(const std::string & category, const std::string & key) const {
	for(auto setting : _settings) {
		if(setting.second->settingsCategory() == category) {
			return setting.second->vec3SettingValue(key);
		}
	}
	return _emptyVec3;
}

const bg::math::Vector4 & Renderer::vec4SettingValue(const std::string & category, const std::string & key) const {
	for(auto setting : _settings) {
		if(setting.second->settingsCategory() == category) {
			return setting.second->vec4SettingValue(key);
		}
	}
	return _emptyVec4;
}

const bg::math::Size2Di Renderer::sizeSettingValue(const std::string & category, const std::string & key) const {
	for(auto setting : _settings) {
		if(setting.second->settingsCategory() == category) {
			return setting.second->sizeSettingValue(key);
		}
	}
	return _emptySize;
}

ClassicRenderer::ClassicRenderer(bg::base::Context * ctx)
    :Renderer(ctx)
{
    _pipeline = new bg::base::Pipeline(context());
    _mixEffect = new bg::fx::Renderer(context());
    _pipeline->setTextureEffect(_mixEffect.getPtr());
    _pipeline->setBuffersToClear(bg::base::kColorDepth);
}

ClassicRenderer::~ClassicRenderer() {

}

}
}
