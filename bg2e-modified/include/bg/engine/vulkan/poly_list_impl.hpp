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

#ifndef _bg_engine_vulkan_poly_list_impl_hpp_
#define _bg_engine_vulkan_poly_list_impl_hpp_

#include <bg/engine/poly_list_impl.hpp>

#include <bg/math/vector.hpp>

#include <bg/engine/vulkan/vulkan_includes.hpp>

namespace bg {
namespace engine {
namespace vulkan {

enum VertexStructType {
	kVertexStructType_P = 1,		// Only vertex
	kVertexStructType_PN,			// Position and normal
	kVertexStructType_PNC,			// Position, normal and color
	kVertexStructType_PC,			// Position and color
	kVertexStructType_PT0,			// Position and Tex0
	kVertexStructType_PT0N,			// Position, Tex0 and normal
	kVertexStructType_PT0NC,		// Position, Tex0, normal and color
	kVertexStructType_PT0C,			// Position, Tex0 and color
	kVertexStructType_PT0T1,		// Position, Tex0 and Tex1
	kVertexStructType_PT0T1N,		// Position, Tex0, Tex1 and normal
	kVertexStructType_PT0T1NC,		// Position, Tex0, Tex1, normal and color
	kVertexStructType_PT0T1C,		// Position, Tex0, Tex1 and color
};

template <VertexStructType structType>
struct VertexType;

// Only vertex
template <>
struct VertexType<kVertexStructType_P> {
	bg::math::Vector3 position;
};
typedef VertexType<kVertexStructType_P> VertexP;

// Vertex and normal
template <>
struct VertexType<kVertexStructType_PN> {
	bg::math::Vector3 position;
	bg::math::Vector3 normal;
	bg::math::Vector3 tangent;
};
typedef VertexType<kVertexStructType_PN> VertexPN;

// Vertex, normal and color
template <>
struct VertexType<kVertexStructType_PNC> {
	bg::math::Vector3 position;
	bg::math::Vector3 normal;
	bg::math::Color color;
	bg::math::Vector3 tangent;
};
typedef VertexType<kVertexStructType_PNC> VertexPNC;

// Vertex and color
template <>
struct VertexType<kVertexStructType_PC> {
	bg::math::Vector3 position;
	bg::math::Color color;
};
typedef VertexType<kVertexStructType_PC> VertexPC;

// Vertex and Tex0
template <>
struct VertexType<kVertexStructType_PT0> {
	bg::math::Vector3 position;
	bg::math::Vector2 texCoord0;
};
typedef VertexType<kVertexStructType_PT0> VertexPT0;

// Vertex, Tex0 and normal
template <>
struct VertexType<kVertexStructType_PT0N> {
	bg::math::Vector3 position;
	bg::math::Vector2 texCoord0;
	bg::math::Vector3 normal;
	bg::math::Vector3 tangent;
};
typedef VertexType<kVertexStructType_PT0N> VertexPT0N;

// Vertex, Tex0, normal and color
template <>
struct VertexType<kVertexStructType_PT0NC> {
	bg::math::Vector3 position;
	bg::math::Vector2 texCoord0;
	bg::math::Vector3 normal;
	bg::math::Color color;
	bg::math::Vector3 tangent;
};
typedef VertexType<kVertexStructType_PT0NC> VertexPT0NC;

// Vertex, Tex0 and color
template <>
struct VertexType<kVertexStructType_PT0C> {
	bg::math::Vector3 position;
	bg::math::Vector2 texCoord0;
	bg::math::Color color;
	bg::math::Vector3 tangent;
};
typedef VertexType<kVertexStructType_PT0C> VertexPT0C;

// Vertex, Tex0 and Tex1
template <>
struct VertexType<kVertexStructType_PT0T1> {
	bg::math::Vector3 position;
	bg::math::Vector2 texCoord0;
	bg::math::Vector2 texCoord1;
};
typedef VertexType<kVertexStructType_PT0T1> VertexPT0T1;

// Vertex, Tex0, Tex1 and normal
template <>
struct VertexType<kVertexStructType_PT0T1N> {
	bg::math::Vector3 position;
	bg::math::Vector2 texCoord0;
	bg::math::Vector2 texCoord1;
	bg::math::Vector3 normal;
	bg::math::Vector3 tangent;
};
typedef VertexType<kVertexStructType_PT0T1N> VertexPT0T1N;

// Vertex, Tex0, Tex1, normal and color
template <>
struct VertexType<kVertexStructType_PT0T1NC> {
	bg::math::Vector3 position;
	bg::math::Vector2 texCoord0;
	bg::math::Vector2 texCoord1;
	bg::math::Vector3 normal;
	bg::math::Color color;
	bg::math::Vector3 tangent;
};
typedef VertexType<kVertexStructType_PT0T1NC> VertexPT0T1NC;

// Vertex, Tex0, Tex1 and color
template <>
struct VertexType<kVertexStructType_PT0T1C> {
	bg::math::Vector3 position;
	bg::math::Vector2 texCoord0;
	bg::math::Vector2 texCoord1;
	bg::math::Color color;
};
typedef VertexType<kVertexStructType_PT0T1C> VertexPT0T1C;

template <class T>
inline vk::VertexInputBindingDescription getVertexInputBindingDescription(uint32_t binding) {
	return {
		binding,
		sizeof(T),
		vk::VERTEX_INPUT_RATE_VERTEX
	};
}

template <class T>
inline std::vector<vk::VertexInputAttributeDescription> getAttributeDescriptions(uint32_t binding) { return {}; }

template <>
inline std::vector<vk::VertexInputAttributeDescription> getAttributeDescriptions<VertexP>(uint32_t binding) {
    return {
        {
            0,
            binding,
            vk::FORMAT_R32G32B32_SFLOAT,
            offsetof(VertexP,position)
        }
    };
}

template <>
inline std::vector<vk::VertexInputAttributeDescription> getAttributeDescriptions<VertexPN>(uint32_t binding) {
    return {
        {
            0,
            binding,
            vk::FORMAT_R32G32B32_SFLOAT,
            offsetof(VertexPN,position)
        },
        {
            1,
            binding,
            vk::FORMAT_R32G32B32_SFLOAT,
            offsetof(VertexPN,normal)
        },
		{
			2,
			binding,
			vk::FORMAT_R32G32B32_SFLOAT,
			offsetof(VertexPN,tangent)
		}
    };
}

template <>
inline std::vector<vk::VertexInputAttributeDescription> getAttributeDescriptions<VertexPNC>(uint32_t binding) {
    return {
        {
            0,
            binding,
            vk::FORMAT_R32G32B32_SFLOAT,
            offsetof(VertexPNC,position)
        },
        {
            1,
            binding,
            vk::FORMAT_R32G32B32_SFLOAT,
            offsetof(VertexPNC,normal)
        },
        {
            2,
            binding,
            vk::FORMAT_R32G32B32A32_SFLOAT,
            offsetof(VertexPNC,color)
        },
		{
			3,
			binding,
			vk::FORMAT_R32G32B32_SFLOAT,
			offsetof(VertexPNC,tangent)
		}
    };
}

template <>
inline std::vector<vk::VertexInputAttributeDescription> getAttributeDescriptions<VertexPT0>(uint32_t binding) {
    return {
        {
            0,
            binding,
            vk::FORMAT_R32G32B32_SFLOAT,
            offsetof(VertexPT0, position)
        },
        {
            1,
            binding,
            vk::FORMAT_R32G32_SFLOAT,
            offsetof(VertexPT0, texCoord0)
        }
    };
}

template <>
inline std::vector<vk::VertexInputAttributeDescription> getAttributeDescriptions<VertexPT0C>(uint32_t binding) {
    return {
        {
            0,
            binding,
            vk::FORMAT_R32G32B32_SFLOAT,
            offsetof(VertexPT0C, position)
        },
        {
            1,
            binding,
            vk::FORMAT_R32G32_SFLOAT,
            offsetof(VertexPT0C, texCoord0)
        },
        {
            2,
            binding,
            vk::FORMAT_R32G32B32A32_SFLOAT,
            offsetof(VertexPT0C, color)
        }
    };
}

template <>
inline std::vector<vk::VertexInputAttributeDescription> getAttributeDescriptions<VertexPT0N>(uint32_t binding) {
    return {
        {
            0,
            binding,
            vk::FORMAT_R32G32B32_SFLOAT,
            offsetof(VertexPT0N, position)
        },
        {
            1,
            binding,
            vk::FORMAT_R32G32_SFLOAT,
            offsetof(VertexPT0N, texCoord0)
        },
        {
            2,
            binding,
            vk::FORMAT_R32G32B32_SFLOAT,
            offsetof(VertexPT0N, normal)
        },
		{
			3,
			binding,
			vk::FORMAT_R32G32B32_SFLOAT,
			offsetof(VertexPT0N,tangent)
		}
    };
}

template <>
inline std::vector<vk::VertexInputAttributeDescription> getAttributeDescriptions<VertexPT0NC>(uint32_t binding) {
    return {
        {
            0,
            binding,
            vk::FORMAT_R32G32B32_SFLOAT,
            offsetof(VertexPT0NC, position)
        },
        {
            1,
            binding,
            vk::FORMAT_R32G32_SFLOAT,
            offsetof(VertexPT0NC, texCoord0)
        },
        {
            2,
            binding,
            vk::FORMAT_R32G32B32_SFLOAT,
            offsetof(VertexPT0NC, normal)
        },
        {
            3,
            binding,
            vk::FORMAT_R32G32B32A32_SFLOAT,
            offsetof(VertexPT0NC, color)
        },
		{
			4,
			binding,
			vk::FORMAT_R32G32B32_SFLOAT,
			offsetof(VertexPNC,tangent)
		}
    };
}

template <>
inline std::vector<vk::VertexInputAttributeDescription> getAttributeDescriptions<VertexPT0T1>(uint32_t binding) {
    return {
        {
            0,
            binding,
            vk::FORMAT_R32G32B32_SFLOAT,
            offsetof(VertexPT0T1, position)
        },
        {
            1,
            binding,
            vk::FORMAT_R32G32_SFLOAT,
            offsetof(VertexPT0T1, texCoord0)
        },
        {
            2,
            binding,
            vk::FORMAT_R32G32_SFLOAT,
            offsetof(VertexPT0T1, texCoord1)
        }
    };
}

template <>
inline std::vector<vk::VertexInputAttributeDescription> getAttributeDescriptions<VertexPT0T1N>(uint32_t binding) {
    return {
        {
            0,
            binding,
            vk::FORMAT_R32G32B32_SFLOAT,
            offsetof(VertexPT0T1N, position)
        },
        {
            1,
            binding,
            vk::FORMAT_R32G32_SFLOAT,
            offsetof(VertexPT0T1N, texCoord0)
        },
        {
            2,
            binding,
            vk::FORMAT_R32G32_SFLOAT,
            offsetof(VertexPT0T1N, texCoord1)
        },
        {
            3,
            binding,
            vk::FORMAT_R32G32B32_SFLOAT,
            offsetof(VertexPT0T1N,normal)
        },
		{
			4,
			binding,
			vk::FORMAT_R32G32B32_SFLOAT,
			offsetof(VertexPT0T1N,tangent)
		}
    };
}

template <>
inline std::vector<vk::VertexInputAttributeDescription> getAttributeDescriptions<VertexPT0T1NC>(uint32_t binding) {
    return {
        {
            0,
            binding,
            vk::FORMAT_R32G32B32_SFLOAT,
            offsetof(VertexPT0T1NC, position)
        },
        {
            1,
            binding,
            vk::FORMAT_R32G32_SFLOAT,
            offsetof(VertexPT0T1NC, texCoord0)
        },
        {
            2,
            binding,
            vk::FORMAT_R32G32_SFLOAT,
            offsetof(VertexPT0T1NC, texCoord1)
        },
        {
            3,
            binding,
            vk::FORMAT_R32G32B32_SFLOAT,
            offsetof(VertexPT0T1NC,normal)
        },
        {
            4,
            binding,
            vk::FORMAT_R32G32B32A32_SFLOAT,
            offsetof(VertexPT0T1NC,color)
        },
		{
			5,
			binding,
			vk::FORMAT_R32G32B32_SFLOAT,
			offsetof(VertexPT0T1NC,tangent)
		}
    };
}

template <>
inline std::vector<vk::VertexInputAttributeDescription> getAttributeDescriptions<VertexPT0T1C>(uint32_t binding) {
    return {
        {
            0,
            binding,
            vk::FORMAT_R32G32B32_SFLOAT,
            offsetof(VertexPT0T1C, position)
        },
        {
            1,
            binding,
            vk::FORMAT_R32G32_SFLOAT,
            offsetof(VertexPT0T1C, texCoord0)
        },
        {
            2,
            binding,
            vk::FORMAT_R32G32_SFLOAT,
            offsetof(VertexPT0T1C, texCoord1)
        },
        {
            3,
            binding,
            vk::FORMAT_R32G32B32A32_SFLOAT,
            offsetof(VertexPT0T1C,color)
        }
    };
}

class BG2E_EXPORT PolyListImpl : public bg::engine::PolyListImpl {
public:

	PolyListImpl(bg::base::PolyList *);

	virtual void build();
	virtual void draw();
	virtual void destroy();

	VertexStructType getVertexStructType();

	inline vk::Buffer * vertexBuffer() { return _vertexBuffer.getPtr(); }
	inline vk::DeviceMemory * vertexBufferMemory() { return _vertexBufferMemory.getPtr(); }
	inline vk::Buffer * indexBuffer() { return _indexBuffer.getPtr(); }
	inline vk::DeviceMemory * indexBufferMemory() { return _indexBufferMemory.getPtr(); }
    //vk::DescriptorSet * descriptorSet(bg::base::Effect * effect);
    //vk::DescriptorSet * createDescriptorSet(bg::base::Effect * effect, PipelineLayoutManager * layoutManager);

    struct PipelineObjects {
        bg::ptr<vk::DescriptorSet> descriptorSet;
        bg::ptr<vk::Buffer> uniformBuffer;
        bg::ptr<vk::DeviceMemory> uniformBufferMemory;
    };
    
    PipelineObjects * pipelineObjects(bg::base::Effect * effect);
    PipelineObjects * createPipelineObjects(bg::base::Effect * effect, vk::DescriptorSet * ds, vk::Buffer * uniformBuffer, vk::DeviceMemory * uniformBufferMemory);
    
protected:
	virtual ~PolyListImpl();

	bg::ptr<vk::Buffer> _vertexBuffer;
	bg::ptr<vk::DeviceMemory> _vertexBufferMemory;
	bg::ptr<vk::Buffer> _indexBuffer;
	bg::ptr<vk::DeviceMemory> _indexBufferMemory;
    
    // Assign a descriptor set to each Effect class
    
    //std::unordered_map<size_t, bg::ptr<vk::DescriptorSet>> _descriptorSets;
    std::unordered_map<size_t, PipelineObjects> _pipelineObjects;
    
    inline size_t hash(bg::base::Effect * effect) const { return typeid(*effect).hash_code(); }

	void createDataVector(const std::vector<float> & p, std::vector<VertexP> & data);
	void createDataVector(const std::vector<float> & p, const std::vector<float> & n, const std::vector<float> & t, std::vector<VertexPN> & data);
	void createDataVector(const std::vector<float> & p, const std::vector<float> & n, const std::vector<float> & c, const std::vector<float> & t, std::vector<VertexPNC> & data);
	void createDataVector(const std::vector<float> & p, const std::vector<float> & c, std::vector<VertexPC> & data);
	void createDataVector(const std::vector<float> & p, const std::vector<float> & t0, std::vector<VertexPT0> & data);
	void createDataVector(const std::vector<float> & p, const std::vector<float> & t0, const std::vector<float> & n, const std::vector<float> & t, std::vector<VertexPT0N> & data);
	void createDataVector(const std::vector<float> & p, const std::vector<float> & t0, const std::vector<float> & n, const std::vector<float> & c, const std::vector<float> & t, std::vector<VertexPT0NC> & data);
	void createDataVector(const std::vector<float> & p, const std::vector<float> & t0, const std::vector<float> & c, std::vector<VertexPT0C> & data);
	void createDataVector(const std::vector<float> & p, const std::vector<float> & t0, const std::vector<float> & t1, std::vector<VertexPT0T1> & data);
	void createDataVector(const std::vector<float> & p, const std::vector<float> & t0, const std::vector<float> & t1, const std::vector<float> & n, const std::vector<float> & t, std::vector<VertexPT0T1N> & data);
	void createDataVector(const std::vector<float> & p, const std::vector<float> & t0, const std::vector<float> & t1, const std::vector<float> & n, const std::vector<float> & c, const std::vector<float> & t, std::vector<VertexPT0T1NC> & data);
	void createDataVector(const std::vector<float> & p, const std::vector<float> & t0, const std::vector<float> & t1, const std::vector<float> & c, std::vector<VertexPT0T1C> & data);
};

}
}
}

#endif
