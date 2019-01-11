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

#include <vulkan/vulkan.h>

#include <bg/engine/vulkan/vulkan_includes.hpp>

#include <bg/base/exception.hpp>

#include <bg/wnd/window_controller.hpp>

#include <bg/wnd/vk_context.hpp>

#include <memory>
#include <iostream>
#include <fstream>

#include <bg/log.hpp>

namespace bg {
namespace engine {
namespace vulkan {

PipelineLayoutManager::PipelineLayoutManager(bg::wnd::VKContext * ctx)
	:_ctx(ctx)
{

}

PipelineLayoutManager::~PipelineLayoutManager() {
	for (auto dbi : _descriptorBufferInfos) {
		delete dbi;
	}
	_descriptorBufferInfos.clear();
	for (auto dii : _descriptorImageInfos) {
		delete dii;
	}
	_descriptorImageInfos.clear();

	_descriptorSetLayouts.clear();
	_descriptorPool = nullptr;
	_pipelineLayout = nullptr;
}

void PipelineLayoutManager::newLayout() {
	_descriptorSetLayouts.push_back(new vk::DescriptorSetLayout());
}

void PipelineLayoutManager::addBinding(uint32_t binding, vk::DescriptorType type, uint32_t descriptorCount, vk::ShaderStageFlags shaderStages) {
	vk::DescriptorSetLayout * layout = currentLayout();
	if (!layout) {
		newLayout();
		layout = currentLayout();
	}

	vk::DescriptorSetLayoutBinding layoutBinding;
	layoutBinding.binding = binding;
	layoutBinding.descriptorCount = descriptorCount;
	layoutBinding.descriptorType = type;
	layoutBinding.pImmutableSamplers = nullptr;
	layoutBinding.stageFlags = shaderStages;
	layout->createInfo.bindings.push_back(layoutBinding);
}

void PipelineLayoutManager::addDescriptorPoolSize(vk::DescriptorType type, uint32_t count) {
	createPoolIfNeeded();
	_descriptorPool->createInfo.poolSizes.push_back({
		type,
		count
	});
}

void PipelineLayoutManager::setDescriptorPoolMaxSets(uint32_t maxSets) {
	createPoolIfNeeded();
	_descriptorPool->createInfo.maxSets = maxSets;
}

void PipelineLayoutManager::create() {
	_descriptorPool->create(_ctx->device());

	for (auto & lo : _descriptorSetLayouts) {
		lo->create(_ctx->device());
	}

/*	_descriptorSet = new vulkan::vk::DescriptorSet();
	_descriptorSet->allocateInfo.descriptorPool = _descriptorPool.getPtr();
	for (auto & lo : _descriptorSetLayouts) {
		_descriptorSet->allocateInfo.setLayouts.push_back(lo.getPtr());
	}
	_descriptorSet->allocateDescriptorSets(_ctx->device());
	*/

	_pipelineLayout = new vk::PipelineLayout();
	for (auto & lo : _descriptorSetLayouts) {
		_pipelineLayout->createInfo.setLayouts.push_back(lo.getPtr());
	}
	_pipelineLayout->create(_ctx->device());
}

vk::DescriptorSet * PipelineLayoutManager::createDescriptorSet() {
	bg::ptr<vk::DescriptorSet> descriptorSet = new vk::DescriptorSet();
	descriptorSet->allocateInfo.descriptorPool = _descriptorPool.getPtr();
	for (auto & lo : _descriptorSetLayouts) {
		descriptorSet->allocateInfo.setLayouts.push_back(lo.getPtr());
	}
	descriptorSet->allocateDescriptorSets(_ctx->device());

	return descriptorSet.release();
}

void PipelineLayoutManager::addWriteDescriptorSet(uint32_t binding, uint32_t dstArrayElem, vk::DescriptorType type, uint32_t descCount, vk::DescriptorBufferInfo * bufferInfos) {
	vk::WriteDescriptorSet wds;
	// This step is done in updateDescriptorSets() function.
	// wds.dstSet = _descriptorSet.getPtr();
	wds.dstBinding = binding;
	wds.dstArrayElement = dstArrayElem;
	wds.descriptorType = type;
	wds.descriptorCount = descCount;
	wds.pBufferInfo = bufferInfos;
	_writeDescriptorSets.push_back(wds);
}

void PipelineLayoutManager::addWriteDescriptorSet(uint32_t binding, uint32_t dstArrayElem, vk::DescriptorType type, uint32_t descCount, vk::DescriptorImageInfo * imgInfos) {
	vk::WriteDescriptorSet wds;
	// This step is done in updateDescriptorSets() function.
	// wds.dstSet = _descriptorSet.getPtr();
	wds.dstBinding = binding;
	wds.dstArrayElement = dstArrayElem;
	wds.descriptorType = type;
	wds.descriptorCount = descCount;
	wds.pImageInfo = imgInfos;
	_writeDescriptorSets.push_back(wds);
}

void PipelineLayoutManager::addWriteDescriptorSet(uint32_t binding, uint32_t dstArrayElem, vk::DescriptorType type, uint32_t descCount, vk::Buffer * buffer, uint32_t offset, uint32_t range) {
	vk::DescriptorBufferInfo * dbi = new vk::DescriptorBufferInfo;
	dbi->buffer = buffer;
	dbi->offset = offset;
	dbi->range = range;
	_descriptorBufferInfos.push_back(dbi);

	addWriteDescriptorSet(binding, dstArrayElem, type, descCount, dbi);
}

void PipelineLayoutManager::addWriteDescriptorSet(uint32_t binding, uint32_t dstArrayElem, vk::DescriptorType type, uint32_t descCount, vk::Sampler * sampler, vk::ImageView * imgView, vk::ImageLayout imgLayout) {
	vk::DescriptorImageInfo * dii = new vk::DescriptorImageInfo;
	dii->sampler = sampler;
	dii->imageView = imgView;
	dii->imageLayout = imgLayout;
	_descriptorImageInfos.push_back(dii);

	addWriteDescriptorSet(binding, dstArrayElem, type, descCount, dii);
}

void PipelineLayoutManager::updateDescriptorSet(vk::DescriptorSet * ds) {
	for (auto & wds : _writeDescriptorSets) {
		wds.dstSet = ds;
	}
	ds->updateDescriptorSets(_writeDescriptorSets);

	for (auto dbi : _descriptorBufferInfos) {
		delete dbi;
	}
	_descriptorBufferInfos.clear();
	for (auto dii : _descriptorImageInfos) {
		delete dii;
	}
	_descriptorImageInfos.clear();
	_writeDescriptorSets.clear();
}

}
}
}
