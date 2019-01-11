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

#include <bg/engine/vulkan/poly_list_impl.hpp>
#include <bg/engine/vulkan/vulkan_includes.hpp>

#include <bg/wnd/window_controller.hpp>
#include <bg/wnd/vk_context.hpp>
#include <bg/wnd/window.hpp>

namespace bg {
namespace engine {
namespace vulkan {

template <class T>
void createBuffer(bg::wnd::VKContext * ctx, const std::vector<T> & data, bg::ptr<vk::Buffer> & buffer, bg::ptr<vk::DeviceMemory> & memory) {
	BufferHelper bufferHelper(ctx->physicalDevice(), ctx->device());

	bufferHelper.createStagingBuffer(
		ctx->commandPool(), ctx->graphicsQueue(),
		vk::BUFFER_USAGE_VERTEX_BUFFER_BIT,
		data.data(), sizeof(data[0]) * data.size(),
		buffer, memory
	);
}

template <>
void createBuffer(bg::wnd::VKContext * ctx, const std::vector<uint32_t> & data, bg::ptr<vk::Buffer> & buffer, bg::ptr<vk::DeviceMemory> & memory) {
	BufferHelper bufferHelper(ctx->physicalDevice(), ctx->device());

	bufferHelper.createStagingBuffer(
		ctx->commandPool(), ctx->graphicsQueue(),
		vk::BUFFER_USAGE_INDEX_BUFFER_BIT,
		data.data(), sizeof(data[0]) * data.size(),
		buffer, memory
	);
}


PolyListImpl::PolyListImpl(bg::base::PolyList * plist)
	:bg::engine::PolyListImpl(plist)
{
}

PolyListImpl::~PolyListImpl() {
}

void PolyListImpl::build() {
	destroy();

	// Create vertex buffer
	switch (getVertexStructType()) {
	case kVertexStructType_P: {
		std::vector<VertexP> data;
		createDataVector(plist()->vertexVector(), data);
		createBuffer(vulkan::context(plist()->context()), data, _vertexBuffer, _vertexBufferMemory);
		break;
	}
	case kVertexStructType_PN: {
		std::vector<VertexPN> data;
		createDataVector(plist()->vertexVector(), plist()->normalVector(), plist()->tangentVector(), data);
		createBuffer(vulkan::context(plist()->context()), data, _vertexBuffer, _vertexBufferMemory);
		break;
	}
	case kVertexStructType_PNC: {
		std::vector<VertexPNC> data;
		createDataVector(plist()->vertexVector(), plist()->normalVector(), plist()->colorVector(), plist()->tangentVector(), data);
		createBuffer(vulkan::context(plist()->context()), data, _vertexBuffer, _vertexBufferMemory);
		break;
	}
	case kVertexStructType_PC: {
		std::vector<VertexPC> data;
		createDataVector(plist()->vertexVector(), plist()->colorVector(), data);
		createBuffer(vulkan::context(plist()->context()), data, _vertexBuffer, _vertexBufferMemory);
		break;
	}
	case kVertexStructType_PT0: {
		std::vector<VertexPT0> data;
		createDataVector(plist()->vertexVector(), plist()->texCoord0Vector(), data);
		createBuffer(vulkan::context(plist()->context()), data, _vertexBuffer, _vertexBufferMemory);
		break;
	}
	case kVertexStructType_PT0N: {
		std::vector<VertexPT0N> data;
		createDataVector(plist()->vertexVector(), plist()->texCoord0Vector(), plist()->normalVector(), plist()->tangentVector(), data);
		createBuffer(vulkan::context(plist()->context()), data, _vertexBuffer, _vertexBufferMemory);
		break;
	}
	case kVertexStructType_PT0NC: {
		std::vector<VertexPT0NC> data;
		createDataVector(plist()->vertexVector(), plist()->texCoord0Vector(), plist()->normalVector(), plist()->colorVector(), plist()->tangentVector(), data);
		createBuffer(vulkan::context(plist()->context()), data, _vertexBuffer, _vertexBufferMemory);
		break;
	}
	case kVertexStructType_PT0C: {
		std::vector<VertexPT0C> data;
		createDataVector(plist()->vertexVector(), plist()->texCoord0Vector(), plist()->colorVector(), data);
		createBuffer(vulkan::context(plist()->context()), data, _vertexBuffer, _vertexBufferMemory);
		break;
	}
	case kVertexStructType_PT0T1: {
		std::vector<VertexPT0T1> data;
		createDataVector(plist()->vertexVector(), plist()->texCoord0Vector(), plist()->texCoord0Vector(), data);
		createBuffer(vulkan::context(plist()->context()), data, _vertexBuffer, _vertexBufferMemory);
		break;
	}
	case kVertexStructType_PT0T1N: {
		std::vector<VertexPT0T1N> data;
		createDataVector(plist()->vertexVector(), plist()->texCoord0Vector(), plist()->texCoord0Vector(), plist()->normalVector(), plist()->tangentVector(), data);
		createBuffer(vulkan::context(plist()->context()), data, _vertexBuffer, _vertexBufferMemory);
		break;
	}
	case kVertexStructType_PT0T1NC: {
		std::vector<VertexPT0T1NC> data;
		createDataVector(plist()->vertexVector(), plist()->texCoord0Vector(), plist()->texCoord0Vector(), plist()->normalVector(), plist()->colorVector(), plist()->tangentVector(), data);
		createBuffer(vulkan::context(plist()->context()), data, _vertexBuffer, _vertexBufferMemory);
		break;
	}
	case kVertexStructType_PT0T1C: {
		std::vector<VertexPT0T1C> data;
		createDataVector(plist()->vertexVector(), plist()->texCoord0Vector(), plist()->texCoord0Vector(), plist()->colorVector(), data);
		createBuffer(vulkan::context(plist()->context()), data, _vertexBuffer, _vertexBufferMemory);
		break;
	}
	}

	// Create index buffer
	createBuffer(vulkan::context(plist()->context()), plist()->indexVector(), _indexBuffer, _indexBufferMemory);
}

void PolyListImpl::draw() {
	// TODO: Draw
}

void PolyListImpl::destroy() {
	_vertexBuffer = nullptr;
	_vertexBufferMemory = nullptr;
}

VertexStructType PolyListImpl::getVertexStructType() {
	bg::base::PolyList * pl = plist();
	bool P = pl->vertexCount() > 0;
	bool N = pl->normalCount() > 0 && pl->tangentCount() > 0;
	bool C = pl->colorCount() > 0;
	bool T0 = pl->texCoord0Count() > 0;
	bool T1 = pl->texCoord1Count() > 0;

	if (P && !N && !C && !T0 && !T1) { // Position
		return kVertexStructType_P;
	}
	else if (P && N && !C && !T0 && !T1) { // Position and normal
		return kVertexStructType_PN;
	}
	else if (P && !N && C && !T0 && !T1) { // Position and color
		return kVertexStructType_PC;
	}
	else if (P && N && C && !T0 && !T1) { // Position, normal and color
		return kVertexStructType_PNC;
	}
	else if (P && !N && !C && T0 && !T1) { // Position and texCoord0
		return kVertexStructType_PT0;
	}
	else if (P && !N && C && T0 && !T1) { // Position, texCoord0 and color
		return kVertexStructType_PT0C;
	}
	else if (P && N && !C && T0 && !T1) { // Position, texCoord0 and normal
		return kVertexStructType_PT0N;
	}
	else if (P && N && C && T0 && !T1) { // Position, texCoord0, normal and color 
		return kVertexStructType_PT0NC;
	}
	else if (P && !N && !C && T0 && T1) { // Position, texCoord0 and texCoord1
		return kVertexStructType_PT0T1;
	}
	else if (P && !N && C && T0 && T1) { // Position, texCoord0, texCoord1 and color
		return kVertexStructType_PT0T1C;
	}
	else if (P && N && !C && T0 && T1) { // Position, texCoord0, texCoord1 and normal
		return kVertexStructType_PT0T1N;
	}
	else if (P && N && C && T0 && T1) { // Position, texCoord0, texCoord1, normal and color 
		return kVertexStructType_PT0T1NC;
	}
	else {
		throw bg::base::VulkanEngineException("Invalid poligon vertex types.");
	}
}

void PolyListImpl::createDataVector(const std::vector<float> & p, std::vector<VertexP> & data) {
	size_t length = p.size() / 3;

	size_t ip = 0;
	for (size_t i = 0; i < length; ++i, ip += 3) {
		data.push_back({
			{ p[ip], p[ip + 1], p[ip + 2] }
			});
	}
}

void PolyListImpl::createDataVector(const std::vector<float> & p, const std::vector<float> & n, const std::vector<float> & t, std::vector<VertexPN> & data) {
	size_t length = p.size() / 3;
	if (length != n.size() / 3 || length != t.size() / 3) {
		throw new bg::base::VulkanEngineException("Invalid vertex data: inconsistent number of vertex elements");
	}

	size_t ip = 0;
	size_t in = 0;
	size_t it = 0;
	for (size_t i = 0; i < length; ++i, ip += 3, in += 3, it += 3) {
		data.push_back({
			{ p[ip], p[ip + 1], p[ip + 2] },
			{ n[in], n[in + 1], n[in + 2] },
			{ t[it], t[it + 1], t[it + 2] }
			});
	}
}

void PolyListImpl::createDataVector(const std::vector<float> & p, const std::vector<float> & n, const std::vector<float> & c, const std::vector<float> & t, std::vector<VertexPNC> & data) {
	size_t length = p.size() / 3;
	if (length != n.size() / 3 || length != c.size() / 4 || length != t.size() / 3) {
		throw new bg::base::VulkanEngineException("Invalid vertex data: inconsistent number of vertex elements");
	}

	size_t ip = 0;
	size_t in = 0;
	size_t ic = 0;
	size_t it = 0;
	for (size_t i = 0; i < length; ++i, ip += 3, in += 3, ic += 4, it += 3) {
		data.push_back({
			{ p[ip], p[ip + 1], p[ip + 2] },
			{ n[in], n[in + 1], n[in + 2] },
			{ c[ic], c[ic + 1], c[ic + 2], c[ic + 3] },
			{ t[it], t[it + 1], t[it + 2] }
			});
	}
}

void PolyListImpl::createDataVector(const std::vector<float> & p, const std::vector<float> & c, std::vector<VertexPC> & data) {
	size_t length = p.size() / 3;
	if (length != c.size() / 4) {
		throw new bg::base::VulkanEngineException("Invalid vertex data: inconsistent number of vertex elements");
	}

	size_t ip = 0;
	size_t ic = 0;
	for (size_t i = 0; i < length; ++i, ip += 3, ic += 4) {
		data.push_back({
			{ p[ip], p[ip + 1], p[ip + 2] },
			{ c[ic], c[ic + 1], c[ic + 2], c[ic + 3] }
			});
	}
}

void PolyListImpl::createDataVector(const std::vector<float> & p, const std::vector<float> & t0, std::vector<VertexPT0> & data) {
	size_t length = p.size() / 3;
	if (length != t0.size() / 2) {
		throw new bg::base::VulkanEngineException("Invalid vertex data: inconsistent number of vertex elements");
	}

	size_t ip = 0;
	size_t it0 = 0;
	for (size_t i = 0; i < length; ++i, ip += 3, it0 += 2) {
		data.push_back({
			{ p[ip], p[ip + 1], p[ip + 2] },
			{ t0[it0], t0[it0 + 1] }
			});
	}
}

void PolyListImpl::createDataVector(const std::vector<float> & p, const std::vector<float> & t0, const std::vector<float> & n, const std::vector<float> & t, std::vector<VertexPT0N> & data) {
	size_t length = p.size() / 3;
	if (length != t0.size() / 2 || length != n.size() / 3 || length != t.size() / 3) {
		throw new bg::base::VulkanEngineException("Invalid vertex data: inconsistent number of vertex elements");
	}

	size_t ip = 0;
	size_t it0 = 0;
	size_t in = 0;
	size_t it = 0;
	for (size_t i = 0; i < length; ++i, ip += 3, it0 += 2, in += 3, it += 3) {
		data.push_back({
			{ p[ip], p[ip + 1], p[ip + 2] },
			{ t0[it0], t0[it0 + 1] },
			{ n[in], n[in + 1], n[in + 2] },
			{ t[it], t[it + 1], t[it + 2] }
			});
	}
}

void PolyListImpl::createDataVector(const std::vector<float> & p, const std::vector<float> & t0, const std::vector<float> & n, const std::vector<float> & c, const std::vector<float> & t, std::vector<VertexPT0NC> & data) {
	size_t length = p.size() / 3;
	if (length != t0.size() / 2 || length != n.size() / 3 || length != c.size() / 4 || length != t.size() / 3) {
		throw new bg::base::VulkanEngineException("Invalid vertex data: inconsistent number of vertex elements");
	}

	size_t ip = 0;
	size_t it0 = 0;
	size_t in = 0;
	size_t ic = 0;
	size_t it = 0;
	for (size_t i = 0; i < length; ++i, ip += 3, it0 += 2, in += 3, ic += 4, it += 3) {
		data.push_back({
			{ p[ip], p[ip + 1], p[ip + 2] },
			{ t0[it0], t0[it0 + 1] },
			{ n[in], n[in + 1], n[in + 2] },
			{ c[ic], c[ic + 1], c[ic + 2], c[ic + 3] },
			{ t[it], t[it + 1], t[it + 2] }
			});
	}
}

void PolyListImpl::createDataVector(const std::vector<float> & p, const std::vector<float> & t0, const std::vector<float> & c, std::vector<VertexPT0C> & data) {
	size_t length = p.size() / 3;
	if (length != t0.size() / 2 || length != c.size() / 4) {
		throw new bg::base::VulkanEngineException("Invalid vertex data: inconsistent number of vertex elements");
	}

	size_t ip = 0;
	size_t it0 = 0;
	size_t ic = 0;
	for (size_t i = 0; i < length; ++i, ip += 3, it0 += 2, ic += 4) {
		data.push_back({
			{ p[ip], p[ip + 1], p[ip + 2] },
			{ t0[it0], t0[it0 + 1] },
			{ c[ic], c[ic + 1] , c[ic + 2] , c[ic + 3] }
			});
	}
}

void PolyListImpl::createDataVector(const std::vector<float> & p, const std::vector<float> & t0, const std::vector<float> & t1, std::vector<VertexPT0T1> & data) {
	size_t length = p.size() / 3;
	if (length != t0.size() / 2 || length != t1.size() / 2) {
		throw new bg::base::VulkanEngineException("Invalid vertex data: inconsistent number of vertex elements");
	}

	size_t ip = 0;
	size_t it0 = 0;
	size_t it1 = 0;
	for (size_t i = 0; i < length; ++i, ip += 3, it0 += 2, it1 += 2) {
		data.push_back({
			{ p[ip], p[ip + 1], p[ip + 2] },
			{ t0[it0], t0[it0 + 1] },
			{ t1[it1], t1[it1 + 1] }
			});
	}
}

void PolyListImpl::createDataVector(const std::vector<float> & p, const std::vector<float> & t0, const std::vector<float> & t1, const std::vector<float> & n, const std::vector<float> & t, std::vector<VertexPT0T1N> & data) {
	size_t length = p.size() / 3;
	if (length != t0.size() / 2 || length != t1.size() / 2 || length != n.size() / 3 || length != t.size() / 3) {
		throw new bg::base::VulkanEngineException("Invalid vertex data: inconsistent number of vertex elements");
	}

	size_t ip = 0;
	size_t it0 = 0;
	size_t it1 = 0;
	size_t in = 0;
	size_t it = 0;
	for (size_t i = 0; i < length; ++i, ip += 3, it0 += 2, it1 += 2, in += 3, it += 3) {
		data.push_back({
			{ p[ip], p[ip + 1], p[ip + 2] },
			{ t0[it0], t0[it0 + 1] },
			{ t1[it1], t1[it1 + 1] },
			{ n[in], n[in + 1], n[in + 2] },
			{ t[it], t[it + 1], t[it + 2] }
			});
	}
}

void PolyListImpl::createDataVector(const std::vector<float> & p, const std::vector<float> & t0, const std::vector<float> & t1, const std::vector<float> & n, const std::vector<float> & c, const std::vector<float> & t, std::vector<VertexPT0T1NC> & data) {
	size_t length = p.size() / 3;
	if (length != t0.size() / 2 || length != t1.size() / 2 || length != n.size() / 3 || length != c.size() / 4 || length != t.size() / 3) {
		throw new bg::base::VulkanEngineException("Invalid vertex data: inconsistent number of vertex elements");
	}

	size_t ip = 0;
	size_t it0 = 0;
	size_t it1 = 0;
	size_t in = 0;
	size_t ic = 0;
	size_t it = 0;
	for (size_t i = 0; i < length; ++i, ip += 3, it0 += 2, it1 += 2, in += 3, ic += 4, it += 3) {
		data.push_back({
			{ p[ip], p[ip + 1], p[ip + 2] },
			{ t0[it0], t0[it0 + 1] },
			{ t1[it1], t1[it1 + 1] },
			{ n[in], n[in + 1], n[in + 2] },
			{ c[ic], c[ic + 1], c[ic + 2] , c[ic + 3] },
			{ t[it], t[it + 1], t[it + 2] }
			});
	}
}

void PolyListImpl::createDataVector(const std::vector<float> & p, const std::vector<float> & t0, const std::vector<float> & t1, const std::vector<float> & c, std::vector<VertexPT0T1C> & data) {
	size_t length = p.size() / 3;
	if (length != t0.size() / 2 || length != t1.size() / 2 || length != c.size() / 4) {
		throw new bg::base::VulkanEngineException("Invalid vertex data: inconsistent number of vertex elements");
	}

	size_t ip = 0;
	size_t it0 = 0;
	size_t it1 = 0;
	size_t ic = 0;
	for (size_t i = 0; i < length; ++i, ip += 3, it0 += 2, it1 += 2, ic += 4) {
		data.push_back({
			{ p[ip], p[ip + 1], p[ip + 2] },
			{ t0[it0], t0[it0 + 1] },
			{ t1[it1], t1[it1 + 1] },
			{ c[ic], c[ic + 1] , c[ic + 2] , c[ic + 3] }
			});
	}
}
    
PolyListImpl::PipelineObjects * PolyListImpl::pipelineObjects(bg::base::Effect * effect) {
    auto h = hash(effect);
    if (_pipelineObjects.find(h) == _pipelineObjects.end()) {
        return &_pipelineObjects[h];
    }
    return nullptr;
}

PolyListImpl::PipelineObjects * PolyListImpl::createPipelineObjects(bg::base::Effect * effect, vk::DescriptorSet * ds, vk::Buffer * uniformBuffer, vk::DeviceMemory * uniformBufferMemory) {
    auto h = hash(effect);
    if (_pipelineObjects.find(h)==_pipelineObjects.end()) {
        _pipelineObjects[h] = {
            ds, uniformBuffer, uniformBufferMemory
        };
    }
    return &_pipelineObjects[h];
}

}
}
}
