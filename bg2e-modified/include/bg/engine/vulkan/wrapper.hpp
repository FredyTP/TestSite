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

#ifndef _bg2e_engine_vulkan_wrapper_hpp_
#define _bg2e_engine_vulkan_wrapper_hpp_

#include <bg/base/referenced_pointer.hpp>

#include <bg/wnd/window.hpp>
#include <bg/base/event_handler.hpp>

#include <string>
#include <vector>
#include <set>
#include <map>

// From vulkan_core.h
#define BG_VK_UUID_SIZE						16
#define BG_VK_MAX_MEMORY_TYPES				32
#define BG_VK_MAX_MEMORY_HEAPS              16
#define BG_VK_MAX_EXTENSION_NAME_SIZE		256


namespace bg {
namespace engine {
namespace vulkan {

namespace vk {

#define BG_VK_LOD_CLAMP_NONE                 1000.0f
#define BG_VK_REMAINING_MIP_LEVELS           (~0U)
#define BG_VK_REMAINING_ARRAY_LAYERS         (~0U)
#define BG_VK_WHOLE_SIZE                     (~0ULL)
#define BG_VK_ATTACHMENT_UNUSED              (~0U)
#define BG_VK_TRUE                           1
#define BG_VK_FALSE                          0
#define BG_VK_QUEUE_FAMILY_IGNORED           (~0U)
#define BG_VK_SUBPASS_EXTERNAL               (~0U)
#define BG_VK_MAX_PHYSICAL_DEVICE_NAME_SIZE  256
#define BG_VK_UUID_SIZE                      16
#define BG_VK_MAX_MEMORY_TYPES               32
#define BG_VK_MAX_MEMORY_HEAPS               16
#define BG_VK_MAX_EXTENSION_NAME_SIZE        256
#define BG_VK_MAX_DESCRIPTION_SIZE           256

typedef uint64_t DeviceSize;
typedef uint32_t Flags;
typedef uint32_t SampleMask;

BG2E_EXPORT extern const uint32_t SUCCESS;
BG2E_EXPORT extern const uint32_t NOT_READY;
BG2E_EXPORT extern const uint32_t TIMEOUT;
BG2E_EXPORT extern const uint32_t EVENT_SET;
BG2E_EXPORT extern const uint32_t EVENT_RESET;
BG2E_EXPORT extern const uint32_t INCOMPLETE;
BG2E_EXPORT extern const uint32_t ERROR_OUT_OF_HOST_MEMORY;
BG2E_EXPORT extern const uint32_t ERROR_OUT_OF_DEVICE_MEMORY;
BG2E_EXPORT extern const uint32_t ERROR_INITIALIZATION_FAILED;
BG2E_EXPORT extern const uint32_t ERROR_DEVICE_LOST;
BG2E_EXPORT extern const uint32_t ERROR_MEMORY_MAP_FAILED;
BG2E_EXPORT extern const uint32_t ERROR_LAYER_NOT_PRESENT;
BG2E_EXPORT extern const uint32_t ERROR_EXTENSION_NOT_PRESENT;
BG2E_EXPORT extern const uint32_t ERROR_FEATURE_NOT_PRESENT;
BG2E_EXPORT extern const uint32_t ERROR_INCOMPATIBLE_DRIVER;
BG2E_EXPORT extern const uint32_t ERROR_TOO_MANY_OBJECTS;
BG2E_EXPORT extern const uint32_t ERROR_FORMAT_NOT_SUPPORTED;
BG2E_EXPORT extern const uint32_t ERROR_FRAGMENTED_POOL;
BG2E_EXPORT extern const uint32_t ERROR_OUT_OF_POOL_MEMORY;
BG2E_EXPORT extern const uint32_t ERROR_INVALID_EXTERNAL_HANDLE;
BG2E_EXPORT extern const uint32_t ERROR_SURFACE_LOST_KHR;
BG2E_EXPORT extern const uint32_t ERROR_NATIVE_WINDOW_IN_USE_KHR;
BG2E_EXPORT extern const uint32_t SUBOPTIMAL_KHR;
BG2E_EXPORT extern const uint32_t ERROR_OUT_OF_DATE_KHR;
BG2E_EXPORT extern const uint32_t ERROR_INCOMPATIBLE_DISPLAY_KHR;
BG2E_EXPORT extern const uint32_t ERROR_VALIDATION_FAILED_EXT;
BG2E_EXPORT extern const uint32_t ERROR_INVALID_SHADER_NV;
BG2E_EXPORT extern const uint32_t ERROR_FRAGMENTATION_EXT;
BG2E_EXPORT extern const uint32_t ERROR_NOT_PERMITTED_EXT;
BG2E_EXPORT extern const uint32_t ERROR_OUT_OF_POOL_MEMORY_KHR;
BG2E_EXPORT extern const uint32_t ERROR_INVALID_EXTERNAL_HANDLE_KHR;
BG2E_EXPORT extern const uint32_t RESULT_BEGIN_RANGE;
BG2E_EXPORT extern const uint32_t RESULT_END_RANGE;
BG2E_EXPORT extern const uint32_t RESULT_RANGE_SIZE;
BG2E_EXPORT extern const uint32_t RESULT_MAX_ENUM;

typedef uint32_t Result;

BG2E_EXPORT extern uint32_t makeVersion(uint32_t major, uint32_t minor, uint32_t patch);
BG2E_EXPORT extern const uint32_t API_VERSION_1_0;
BG2E_EXPORT extern const uint32_t API_VERSION_1_1;

/* Extension names */
BG2E_EXPORT extern const char * KHR_SURFACE_EXTENSION_NAME;
BG2E_EXPORT extern const char * KHR_SWAPCHAIN_EXTENSION_NAME;
BG2E_EXPORT extern const char * KHR_WIN32_SURFACE_EXTENSION_NAME;
BG2E_EXPORT extern const char * EXT_DEBUG_REPORT_EXTENSION_NAME;
/* End extension names */

BG2E_EXPORT extern const uint32_t FORMAT_UNDEFINED;
BG2E_EXPORT extern const uint32_t FORMAT_R4G4_UNORM_PACK8;
BG2E_EXPORT extern const uint32_t FORMAT_R4G4B4A4_UNORM_PACK16;
BG2E_EXPORT extern const uint32_t FORMAT_B4G4R4A4_UNORM_PACK16;
BG2E_EXPORT extern const uint32_t FORMAT_R5G6B5_UNORM_PACK16;
BG2E_EXPORT extern const uint32_t FORMAT_B5G6R5_UNORM_PACK16;
BG2E_EXPORT extern const uint32_t FORMAT_R5G5B5A1_UNORM_PACK16;
BG2E_EXPORT extern const uint32_t FORMAT_B5G5R5A1_UNORM_PACK16;
BG2E_EXPORT extern const uint32_t FORMAT_A1R5G5B5_UNORM_PACK16;
BG2E_EXPORT extern const uint32_t FORMAT_R8_UNORM;
BG2E_EXPORT extern const uint32_t FORMAT_R8_SNORM;
BG2E_EXPORT extern const uint32_t FORMAT_R8_USCALED;
BG2E_EXPORT extern const uint32_t FORMAT_R8_SSCALED;
BG2E_EXPORT extern const uint32_t FORMAT_R8_UINT;
BG2E_EXPORT extern const uint32_t FORMAT_R8_SINT;
BG2E_EXPORT extern const uint32_t FORMAT_R8_SRGB;
BG2E_EXPORT extern const uint32_t FORMAT_R8G8_UNORM;
BG2E_EXPORT extern const uint32_t FORMAT_R8G8_SNORM;
BG2E_EXPORT extern const uint32_t FORMAT_R8G8_USCALED;
BG2E_EXPORT extern const uint32_t FORMAT_R8G8_SSCALED;
BG2E_EXPORT extern const uint32_t FORMAT_R8G8_UINT;
BG2E_EXPORT extern const uint32_t FORMAT_R8G8_SINT;
BG2E_EXPORT extern const uint32_t FORMAT_R8G8_SRGB;
BG2E_EXPORT extern const uint32_t FORMAT_R8G8B8_UNORM;
BG2E_EXPORT extern const uint32_t FORMAT_R8G8B8_SNORM;
BG2E_EXPORT extern const uint32_t FORMAT_R8G8B8_USCALED;
BG2E_EXPORT extern const uint32_t FORMAT_R8G8B8_SSCALED;
BG2E_EXPORT extern const uint32_t FORMAT_R8G8B8_UINT;
BG2E_EXPORT extern const uint32_t FORMAT_R8G8B8_SINT;
BG2E_EXPORT extern const uint32_t FORMAT_R8G8B8_SRGB;
BG2E_EXPORT extern const uint32_t FORMAT_B8G8R8_UNORM;
BG2E_EXPORT extern const uint32_t FORMAT_B8G8R8_SNORM;
BG2E_EXPORT extern const uint32_t FORMAT_B8G8R8_USCALED;
BG2E_EXPORT extern const uint32_t FORMAT_B8G8R8_SSCALED;
BG2E_EXPORT extern const uint32_t FORMAT_B8G8R8_UINT;
BG2E_EXPORT extern const uint32_t FORMAT_B8G8R8_SINT;
BG2E_EXPORT extern const uint32_t FORMAT_B8G8R8_SRGB;
BG2E_EXPORT extern const uint32_t FORMAT_R8G8B8A8_UNORM;
BG2E_EXPORT extern const uint32_t FORMAT_R8G8B8A8_SNORM;
BG2E_EXPORT extern const uint32_t FORMAT_R8G8B8A8_USCALED;
BG2E_EXPORT extern const uint32_t FORMAT_R8G8B8A8_SSCALED;
BG2E_EXPORT extern const uint32_t FORMAT_R8G8B8A8_UINT;
BG2E_EXPORT extern const uint32_t FORMAT_R8G8B8A8_SINT;
BG2E_EXPORT extern const uint32_t FORMAT_R8G8B8A8_SRGB;
BG2E_EXPORT extern const uint32_t FORMAT_B8G8R8A8_UNORM;
BG2E_EXPORT extern const uint32_t FORMAT_B8G8R8A8_SNORM;
BG2E_EXPORT extern const uint32_t FORMAT_B8G8R8A8_USCALED;
BG2E_EXPORT extern const uint32_t FORMAT_B8G8R8A8_SSCALED;
BG2E_EXPORT extern const uint32_t FORMAT_B8G8R8A8_UINT;
BG2E_EXPORT extern const uint32_t FORMAT_B8G8R8A8_SINT;
BG2E_EXPORT extern const uint32_t FORMAT_B8G8R8A8_SRGB;
BG2E_EXPORT extern const uint32_t FORMAT_A8B8G8R8_UNORM_PACK32;
BG2E_EXPORT extern const uint32_t FORMAT_A8B8G8R8_SNORM_PACK32;
BG2E_EXPORT extern const uint32_t FORMAT_A8B8G8R8_USCALED_PACK32;
BG2E_EXPORT extern const uint32_t FORMAT_A8B8G8R8_SSCALED_PACK32;
BG2E_EXPORT extern const uint32_t FORMAT_A8B8G8R8_UINT_PACK32;
BG2E_EXPORT extern const uint32_t FORMAT_A8B8G8R8_SINT_PACK32;
BG2E_EXPORT extern const uint32_t FORMAT_A8B8G8R8_SRGB_PACK32;
BG2E_EXPORT extern const uint32_t FORMAT_A2R10G10B10_UNORM_PACK32;
BG2E_EXPORT extern const uint32_t FORMAT_A2R10G10B10_SNORM_PACK32;
BG2E_EXPORT extern const uint32_t FORMAT_A2R10G10B10_USCALED_PACK32;
BG2E_EXPORT extern const uint32_t FORMAT_A2R10G10B10_SSCALED_PACK32;
BG2E_EXPORT extern const uint32_t FORMAT_A2R10G10B10_UINT_PACK32;
BG2E_EXPORT extern const uint32_t FORMAT_A2R10G10B10_SINT_PACK32;
BG2E_EXPORT extern const uint32_t FORMAT_A2B10G10R10_UNORM_PACK32;
BG2E_EXPORT extern const uint32_t FORMAT_A2B10G10R10_SNORM_PACK32;
BG2E_EXPORT extern const uint32_t FORMAT_A2B10G10R10_USCALED_PACK32;
BG2E_EXPORT extern const uint32_t FORMAT_A2B10G10R10_SSCALED_PACK32;
BG2E_EXPORT extern const uint32_t FORMAT_A2B10G10R10_UINT_PACK32;
BG2E_EXPORT extern const uint32_t FORMAT_A2B10G10R10_SINT_PACK32;
BG2E_EXPORT extern const uint32_t FORMAT_R16_UNORM;
BG2E_EXPORT extern const uint32_t FORMAT_R16_SNORM;
BG2E_EXPORT extern const uint32_t FORMAT_R16_USCALED;
BG2E_EXPORT extern const uint32_t FORMAT_R16_SSCALED;
BG2E_EXPORT extern const uint32_t FORMAT_R16_UINT;
BG2E_EXPORT extern const uint32_t FORMAT_R16_SINT;
BG2E_EXPORT extern const uint32_t FORMAT_R16_SFLOAT;
BG2E_EXPORT extern const uint32_t FORMAT_R16G16_UNORM;
BG2E_EXPORT extern const uint32_t FORMAT_R16G16_SNORM;
BG2E_EXPORT extern const uint32_t FORMAT_R16G16_USCALED;
BG2E_EXPORT extern const uint32_t FORMAT_R16G16_SSCALED;
BG2E_EXPORT extern const uint32_t FORMAT_R16G16_UINT;
BG2E_EXPORT extern const uint32_t FORMAT_R16G16_SINT;
BG2E_EXPORT extern const uint32_t FORMAT_R16G16_SFLOAT;
BG2E_EXPORT extern const uint32_t FORMAT_R16G16B16_UNORM;
BG2E_EXPORT extern const uint32_t FORMAT_R16G16B16_SNORM;
BG2E_EXPORT extern const uint32_t FORMAT_R16G16B16_USCALED;
BG2E_EXPORT extern const uint32_t FORMAT_R16G16B16_SSCALED;
BG2E_EXPORT extern const uint32_t FORMAT_R16G16B16_UINT;
BG2E_EXPORT extern const uint32_t FORMAT_R16G16B16_SINT;
BG2E_EXPORT extern const uint32_t FORMAT_R16G16B16_SFLOAT;
BG2E_EXPORT extern const uint32_t FORMAT_R16G16B16A16_UNORM;
BG2E_EXPORT extern const uint32_t FORMAT_R16G16B16A16_SNORM;
BG2E_EXPORT extern const uint32_t FORMAT_R16G16B16A16_USCALED;
BG2E_EXPORT extern const uint32_t FORMAT_R16G16B16A16_SSCALED;
BG2E_EXPORT extern const uint32_t FORMAT_R16G16B16A16_UINT;
BG2E_EXPORT extern const uint32_t FORMAT_R16G16B16A16_SINT;
BG2E_EXPORT extern const uint32_t FORMAT_R16G16B16A16_SFLOAT;
BG2E_EXPORT extern const uint32_t FORMAT_R32_UINT;
BG2E_EXPORT extern const uint32_t FORMAT_R32_SINT;
BG2E_EXPORT extern const uint32_t FORMAT_R32_SFLOAT;
BG2E_EXPORT extern const uint32_t FORMAT_R32G32_UINT;
BG2E_EXPORT extern const uint32_t FORMAT_R32G32_SINT;
BG2E_EXPORT extern const uint32_t FORMAT_R32G32_SFLOAT;
BG2E_EXPORT extern const uint32_t FORMAT_R32G32B32_UINT;
BG2E_EXPORT extern const uint32_t FORMAT_R32G32B32_SINT;
BG2E_EXPORT extern const uint32_t FORMAT_R32G32B32_SFLOAT;
BG2E_EXPORT extern const uint32_t FORMAT_R32G32B32A32_UINT;
BG2E_EXPORT extern const uint32_t FORMAT_R32G32B32A32_SINT;
BG2E_EXPORT extern const uint32_t FORMAT_R32G32B32A32_SFLOAT;
BG2E_EXPORT extern const uint32_t FORMAT_R64_UINT;
BG2E_EXPORT extern const uint32_t FORMAT_R64_SINT;
BG2E_EXPORT extern const uint32_t FORMAT_R64_SFLOAT;
BG2E_EXPORT extern const uint32_t FORMAT_R64G64_UINT;
BG2E_EXPORT extern const uint32_t FORMAT_R64G64_SINT;
BG2E_EXPORT extern const uint32_t FORMAT_R64G64_SFLOAT;
BG2E_EXPORT extern const uint32_t FORMAT_R64G64B64_UINT;
BG2E_EXPORT extern const uint32_t FORMAT_R64G64B64_SINT;
BG2E_EXPORT extern const uint32_t FORMAT_R64G64B64_SFLOAT;
BG2E_EXPORT extern const uint32_t FORMAT_R64G64B64A64_UINT;
BG2E_EXPORT extern const uint32_t FORMAT_R64G64B64A64_SINT;
BG2E_EXPORT extern const uint32_t FORMAT_R64G64B64A64_SFLOAT;
BG2E_EXPORT extern const uint32_t FORMAT_B10G11R11_UFLOAT_PACK32;
BG2E_EXPORT extern const uint32_t FORMAT_E5B9G9R9_UFLOAT_PACK32;
BG2E_EXPORT extern const uint32_t FORMAT_D16_UNORM;
BG2E_EXPORT extern const uint32_t FORMAT_X8_D24_UNORM_PACK32;
BG2E_EXPORT extern const uint32_t FORMAT_D32_SFLOAT;
BG2E_EXPORT extern const uint32_t FORMAT_S8_UINT;
BG2E_EXPORT extern const uint32_t FORMAT_D16_UNORM_S8_UINT;
BG2E_EXPORT extern const uint32_t FORMAT_D24_UNORM_S8_UINT;
BG2E_EXPORT extern const uint32_t FORMAT_D32_SFLOAT_S8_UINT;
BG2E_EXPORT extern const uint32_t FORMAT_BC1_RGB_UNORM_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_BC1_RGB_SRGB_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_BC1_RGBA_UNORM_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_BC1_RGBA_SRGB_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_BC2_UNORM_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_BC2_SRGB_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_BC3_UNORM_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_BC3_SRGB_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_BC4_UNORM_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_BC4_SNORM_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_BC5_UNORM_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_BC5_SNORM_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_BC6H_UFLOAT_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_BC6H_SFLOAT_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_BC7_UNORM_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_BC7_SRGB_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_ETC2_R8G8B8_UNORM_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_ETC2_R8G8B8_SRGB_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_EAC_R11_UNORM_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_EAC_R11_SNORM_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_EAC_R11G11_UNORM_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_EAC_R11G11_SNORM_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_ASTC_4x4_UNORM_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_ASTC_4x4_SRGB_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_ASTC_5x4_UNORM_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_ASTC_5x4_SRGB_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_ASTC_5x5_UNORM_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_ASTC_5x5_SRGB_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_ASTC_6x5_UNORM_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_ASTC_6x5_SRGB_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_ASTC_6x6_UNORM_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_ASTC_6x6_SRGB_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_ASTC_8x5_UNORM_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_ASTC_8x5_SRGB_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_ASTC_8x6_UNORM_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_ASTC_8x6_SRGB_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_ASTC_8x8_UNORM_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_ASTC_8x8_SRGB_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_ASTC_10x5_UNORM_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_ASTC_10x5_SRGB_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_ASTC_10x6_UNORM_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_ASTC_10x6_SRGB_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_ASTC_10x8_UNORM_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_ASTC_10x8_SRGB_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_ASTC_10x10_UNORM_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_ASTC_10x10_SRGB_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_ASTC_12x10_UNORM_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_ASTC_12x10_SRGB_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_ASTC_12x12_UNORM_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_ASTC_12x12_SRGB_BLOCK;
BG2E_EXPORT extern const uint32_t FORMAT_G8B8G8R8_422_UNORM;
BG2E_EXPORT extern const uint32_t FORMAT_B8G8R8G8_422_UNORM;
BG2E_EXPORT extern const uint32_t FORMAT_G8_B8_R8_3PLANE_420_UNORM;
BG2E_EXPORT extern const uint32_t FORMAT_G8_B8R8_2PLANE_420_UNORM;
BG2E_EXPORT extern const uint32_t FORMAT_G8_B8_R8_3PLANE_422_UNORM;
BG2E_EXPORT extern const uint32_t FORMAT_G8_B8R8_2PLANE_422_UNORM;
BG2E_EXPORT extern const uint32_t FORMAT_G8_B8_R8_3PLANE_444_UNORM;
BG2E_EXPORT extern const uint32_t FORMAT_R10X6_UNORM_PACK16;
BG2E_EXPORT extern const uint32_t FORMAT_R10X6G10X6_UNORM_2PACK16;
BG2E_EXPORT extern const uint32_t FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16;
BG2E_EXPORT extern const uint32_t FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16;
BG2E_EXPORT extern const uint32_t FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16;
BG2E_EXPORT extern const uint32_t FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16;
BG2E_EXPORT extern const uint32_t FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16;
BG2E_EXPORT extern const uint32_t FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16;
BG2E_EXPORT extern const uint32_t FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16;
BG2E_EXPORT extern const uint32_t FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16;
BG2E_EXPORT extern const uint32_t FORMAT_R12X4_UNORM_PACK16;
BG2E_EXPORT extern const uint32_t FORMAT_R12X4G12X4_UNORM_2PACK16;
BG2E_EXPORT extern const uint32_t FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16;
BG2E_EXPORT extern const uint32_t FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16;
BG2E_EXPORT extern const uint32_t FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16;
BG2E_EXPORT extern const uint32_t FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16;
BG2E_EXPORT extern const uint32_t FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16;
BG2E_EXPORT extern const uint32_t FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16;
BG2E_EXPORT extern const uint32_t FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16;
BG2E_EXPORT extern const uint32_t FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16;
BG2E_EXPORT extern const uint32_t FORMAT_G16B16G16R16_422_UNORM;
BG2E_EXPORT extern const uint32_t FORMAT_B16G16R16G16_422_UNORM;
BG2E_EXPORT extern const uint32_t FORMAT_G16_B16_R16_3PLANE_420_UNORM;
BG2E_EXPORT extern const uint32_t FORMAT_G16_B16R16_2PLANE_420_UNORM;
BG2E_EXPORT extern const uint32_t FORMAT_G16_B16_R16_3PLANE_422_UNORM;
BG2E_EXPORT extern const uint32_t FORMAT_G16_B16R16_2PLANE_422_UNORM;
BG2E_EXPORT extern const uint32_t FORMAT_G16_B16_R16_3PLANE_444_UNORM;
BG2E_EXPORT extern const uint32_t FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG;
BG2E_EXPORT extern const uint32_t FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG;
BG2E_EXPORT extern const uint32_t FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG;
BG2E_EXPORT extern const uint32_t FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG;
BG2E_EXPORT extern const uint32_t FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG;
BG2E_EXPORT extern const uint32_t FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG;
BG2E_EXPORT extern const uint32_t FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG;
BG2E_EXPORT extern const uint32_t FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG;
BG2E_EXPORT extern const uint32_t FORMAT_G8B8G8R8_422_UNORM_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_B8G8R8G8_422_UNORM_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_G8_B8_R8_3PLANE_420_UNORM_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_G8_B8R8_2PLANE_420_UNORM_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_G8_B8_R8_3PLANE_422_UNORM_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_G8_B8R8_2PLANE_422_UNORM_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_G8_B8_R8_3PLANE_444_UNORM_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_R10X6_UNORM_PACK16_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_R10X6G10X6_UNORM_2PACK16_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_R12X4_UNORM_PACK16_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_R12X4G12X4_UNORM_2PACK16_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_G16B16G16R16_422_UNORM_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_B16G16R16G16_422_UNORM_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_G16_B16_R16_3PLANE_420_UNORM_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_G16_B16R16_2PLANE_420_UNORM_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_G16_B16_R16_3PLANE_422_UNORM_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_G16_B16R16_2PLANE_422_UNORM_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_G16_B16_R16_3PLANE_444_UNORM_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_BEGIN_RANGE;
BG2E_EXPORT extern const uint32_t FORMAT_END_RANGE;
BG2E_EXPORT extern const uint32_t FORMAT_RANGE_SIZE;
BG2E_EXPORT extern const uint32_t FORMAT_MAX_ENUM;
typedef uint32_t Format;

BG2E_EXPORT extern const uint32_t FORMAT_FEATURE_SAMPLED_IMAGE_BIT;
BG2E_EXPORT extern const uint32_t FORMAT_FEATURE_STORAGE_IMAGE_BIT;
BG2E_EXPORT extern const uint32_t FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT;
BG2E_EXPORT extern const uint32_t FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT;
BG2E_EXPORT extern const uint32_t FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT;
BG2E_EXPORT extern const uint32_t FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT;
BG2E_EXPORT extern const uint32_t FORMAT_FEATURE_VERTEX_BUFFER_BIT;
BG2E_EXPORT extern const uint32_t FORMAT_FEATURE_COLOR_ATTACHMENT_BIT;
BG2E_EXPORT extern const uint32_t FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT;
BG2E_EXPORT extern const uint32_t FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;
BG2E_EXPORT extern const uint32_t FORMAT_FEATURE_BLIT_SRC_BIT;
BG2E_EXPORT extern const uint32_t FORMAT_FEATURE_BLIT_DST_BIT;
BG2E_EXPORT extern const uint32_t FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT;
BG2E_EXPORT extern const uint32_t FORMAT_FEATURE_TRANSFER_SRC_BIT;
BG2E_EXPORT extern const uint32_t FORMAT_FEATURE_TRANSFER_DST_BIT;
BG2E_EXPORT extern const uint32_t FORMAT_FEATURE_MIDPOINT_CHROMA_SAMPLES_BIT;
BG2E_EXPORT extern const uint32_t FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_LINEAR_FILTER_BIT;
BG2E_EXPORT extern const uint32_t FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_SEPARATE_RECONSTRUCTION_FILTER_BIT;
BG2E_EXPORT extern const uint32_t FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_BIT;
BG2E_EXPORT extern const uint32_t FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_FORCEABLE_BIT;
BG2E_EXPORT extern const uint32_t FORMAT_FEATURE_DISJOINT_BIT;
BG2E_EXPORT extern const uint32_t FORMAT_FEATURE_COSITED_CHROMA_SAMPLES_BIT;
BG2E_EXPORT extern const uint32_t FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_CUBIC_BIT_IMG;
BG2E_EXPORT extern const uint32_t FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_MINMAX_BIT_EXT;
BG2E_EXPORT extern const uint32_t FORMAT_FEATURE_TRANSFER_SRC_BIT_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_FEATURE_TRANSFER_DST_BIT_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_FEATURE_MIDPOINT_CHROMA_SAMPLES_BIT_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_LINEAR_FILTER_BIT_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_SEPARATE_RECONSTRUCTION_FILTER_BIT_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_BIT_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_FORCEABLE_BIT_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_FEATURE_DISJOINT_BIT_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_FEATURE_COSITED_CHROMA_SAMPLES_BIT_KHR;
BG2E_EXPORT extern const uint32_t FORMAT_FEATURE_FLAG_BITS_MAX_ENUM;
typedef uint32_t FormatFeatureFlagBits;
typedef Flags FormatFeatureFlags;

BG2E_EXPORT extern const uint32_t COLOR_SPACE_SRGB_NONLINEAR_KHR;
BG2E_EXPORT extern const uint32_t COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT;
BG2E_EXPORT extern const uint32_t COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT;
BG2E_EXPORT extern const uint32_t COLOR_SPACE_DCI_P3_LINEAR_EXT;
BG2E_EXPORT extern const uint32_t COLOR_SPACE_DCI_P3_NONLINEAR_EXT;
BG2E_EXPORT extern const uint32_t COLOR_SPACE_BT709_LINEAR_EXT;
BG2E_EXPORT extern const uint32_t COLOR_SPACE_BT709_NONLINEAR_EXT;
BG2E_EXPORT extern const uint32_t COLOR_SPACE_BT2020_LINEAR_EXT;
BG2E_EXPORT extern const uint32_t COLOR_SPACE_HDR10_ST2084_EXT;
BG2E_EXPORT extern const uint32_t COLOR_SPACE_DOLBYVISION_EXT;
BG2E_EXPORT extern const uint32_t COLOR_SPACE_HDR10_HLG_EXT;
BG2E_EXPORT extern const uint32_t COLOR_SPACE_ADOBERGB_LINEAR_EXT;
BG2E_EXPORT extern const uint32_t COLOR_SPACE_ADOBERGB_NONLINEAR_EXT;
BG2E_EXPORT extern const uint32_t COLOR_SPACE_PASS_THROUGH_EXT;
BG2E_EXPORT extern const uint32_t COLOR_SPACE_EXTENDED_SRGB_NONLINEAR_EXT;
BG2E_EXPORT extern const uint32_t COLOR_SPACE_BEGIN_RANGE_KHR;
BG2E_EXPORT extern const uint32_t COLOR_SPACE_END_RANGE_KHR;
BG2E_EXPORT extern const uint32_t COLOR_SPACE_RANGE_SIZE_KHR;
BG2E_EXPORT extern const uint32_t COLOR_SPACE_MAX_ENUM_KHR;
typedef uint32_t ColorSpaceKHR;

BG2E_EXPORT extern const uint32_t ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT;
BG2E_EXPORT extern const uint32_t ATTACHMENT_DESCRIPTION_FLAG_BITS_MAX_ENUM;

typedef Flags VkAttachmentDescriptionFlagBits;
typedef uint32_t AttachmentDescriptionFlags;


BG2E_EXPORT extern const uint32_t ATTACHMENT_LOAD_OP_LOAD;
BG2E_EXPORT extern const uint32_t ATTACHMENT_LOAD_OP_CLEAR;
BG2E_EXPORT extern const uint32_t ATTACHMENT_LOAD_OP_DONT_CARE;
BG2E_EXPORT extern const uint32_t ATTACHMENT_LOAD_OP_BEGIN_RANGE;
BG2E_EXPORT extern const uint32_t ATTACHMENT_LOAD_OP_END_RANGE;
BG2E_EXPORT extern const uint32_t ATTACHMENT_LOAD_OP_RANGE_SIZE;
BG2E_EXPORT extern const uint32_t ATTACHMENT_LOAD_OP_MAX_ENUM;
typedef Flags AttachmentLoadOp;

BG2E_EXPORT extern const uint32_t ATTACHMENT_STORE_OP_STORE;
BG2E_EXPORT extern const uint32_t ATTACHMENT_STORE_OP_DONT_CARE;
BG2E_EXPORT extern const uint32_t ATTACHMENT_STORE_OP_BEGIN_RANGE;
BG2E_EXPORT extern const uint32_t ATTACHMENT_STORE_OP_END_RANGE;
BG2E_EXPORT extern const uint32_t ATTACHMENT_STORE_OP_RANGE_SIZE;
BG2E_EXPORT extern const uint32_t ATTACHMENT_STORE_OP_MAX_ENUM;
typedef Flags AttachmentStoreOp;

BG2E_EXPORT extern const uint32_t IMAGE_LAYOUT_UNDEFINED;
BG2E_EXPORT extern const uint32_t IMAGE_LAYOUT_GENERAL;
BG2E_EXPORT extern const uint32_t IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
BG2E_EXPORT extern const uint32_t IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
BG2E_EXPORT extern const uint32_t IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
BG2E_EXPORT extern const uint32_t IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
BG2E_EXPORT extern const uint32_t IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
BG2E_EXPORT extern const uint32_t IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
BG2E_EXPORT extern const uint32_t IMAGE_LAYOUT_PREINITIALIZED;
BG2E_EXPORT extern const uint32_t IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL;
BG2E_EXPORT extern const uint32_t IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL;
BG2E_EXPORT extern const uint32_t IMAGE_LAYOUT_PRESENT_SRC_KHR;
BG2E_EXPORT extern const uint32_t IMAGE_LAYOUT_SHARED_PRESENT_KHR;
BG2E_EXPORT extern const uint32_t IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL_KHR;
BG2E_EXPORT extern const uint32_t IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL_KHR;
BG2E_EXPORT extern const uint32_t IMAGE_LAYOUT_BEGIN_RANGE;
BG2E_EXPORT extern const uint32_t IMAGE_LAYOUT_END_RANGE;
BG2E_EXPORT extern const uint32_t IMAGE_LAYOUT_RANGE_SIZE;
BG2E_EXPORT extern const uint32_t IMAGE_LAYOUT_MAX_ENUM;
typedef Flags ImageLayout;

BG2E_EXPORT extern const uint32_t SUBPASS_DESCRIPTION_PER_VIEW_ATTRIBUTES_BIT_NVX;
BG2E_EXPORT extern const uint32_t SUBPASS_DESCRIPTION_PER_VIEW_POSITION_X_ONLY_BIT_NVX;
BG2E_EXPORT extern const uint32_t SUBPASS_DESCRIPTION_FLAG_BITS_MAX_ENUM;
typedef Flags SubpassDescriptionFlagBits;
typedef uint32_t SubpassDescriptionFlags;

BG2E_EXPORT extern const uint32_t PIPELINE_BIND_POINT_GRAPHICS;
BG2E_EXPORT extern const uint32_t PIPELINE_BIND_POINT_COMPUTE;
BG2E_EXPORT extern const uint32_t PIPELINE_BIND_POINT_BEGIN_RANGE;
BG2E_EXPORT extern const uint32_t PIPELINE_BIND_POINT_END_RANGE;
BG2E_EXPORT extern const uint32_t PIPELINE_BIND_POINT_RANGE_SIZE;
BG2E_EXPORT extern const uint32_t PIPELINE_BIND_POINT_MAX_ENUM;
typedef Flags PipelineBindPoint;

BG2E_EXPORT extern const uint32_t PIPELINE_STAGE_TOP_OF_PIPE_BIT;
BG2E_EXPORT extern const uint32_t PIPELINE_STAGE_DRAW_INDIRECT_BIT;
BG2E_EXPORT extern const uint32_t PIPELINE_STAGE_VERTEX_INPUT_BIT;
BG2E_EXPORT extern const uint32_t PIPELINE_STAGE_VERTEX_SHADER_BIT;
BG2E_EXPORT extern const uint32_t PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT;
BG2E_EXPORT extern const uint32_t PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT;
BG2E_EXPORT extern const uint32_t PIPELINE_STAGE_GEOMETRY_SHADER_BIT;
BG2E_EXPORT extern const uint32_t PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
BG2E_EXPORT extern const uint32_t PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
BG2E_EXPORT extern const uint32_t PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
BG2E_EXPORT extern const uint32_t PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
BG2E_EXPORT extern const uint32_t PIPELINE_STAGE_COMPUTE_SHADER_BIT;
BG2E_EXPORT extern const uint32_t PIPELINE_STAGE_TRANSFER_BIT;
BG2E_EXPORT extern const uint32_t PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
BG2E_EXPORT extern const uint32_t PIPELINE_STAGE_HOST_BIT;
BG2E_EXPORT extern const uint32_t PIPELINE_STAGE_ALL_GRAPHICS_BIT;
BG2E_EXPORT extern const uint32_t PIPELINE_STAGE_ALL_COMMANDS_BIT;
BG2E_EXPORT extern const uint32_t PIPELINE_STAGE_COMMAND_PROCESS_BIT_NVX;
BG2E_EXPORT extern const uint32_t PIPELINE_STAGE_FLAG_BITS_MAX_ENUM;
typedef Flags PipelineStageFlagBits;
typedef uint32_t PipelineStageFlags;
typedef uint32_t MemoryMapFlags;

BG2E_EXPORT extern const uint32_t ACCESS_INDIRECT_COMMAND_READ_BIT;
BG2E_EXPORT extern const uint32_t ACCESS_INDEX_READ_BIT;
BG2E_EXPORT extern const uint32_t ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
BG2E_EXPORT extern const uint32_t ACCESS_UNIFORM_READ_BIT;
BG2E_EXPORT extern const uint32_t ACCESS_INPUT_ATTACHMENT_READ_BIT;
BG2E_EXPORT extern const uint32_t ACCESS_SHADER_READ_BIT;
BG2E_EXPORT extern const uint32_t ACCESS_SHADER_WRITE_BIT;
BG2E_EXPORT extern const uint32_t ACCESS_COLOR_ATTACHMENT_READ_BIT;
BG2E_EXPORT extern const uint32_t ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
BG2E_EXPORT extern const uint32_t ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
BG2E_EXPORT extern const uint32_t ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
BG2E_EXPORT extern const uint32_t ACCESS_TRANSFER_READ_BIT;
BG2E_EXPORT extern const uint32_t ACCESS_TRANSFER_WRITE_BIT;
BG2E_EXPORT extern const uint32_t ACCESS_HOST_READ_BIT;
BG2E_EXPORT extern const uint32_t ACCESS_HOST_WRITE_BIT;
BG2E_EXPORT extern const uint32_t ACCESS_MEMORY_READ_BIT;
BG2E_EXPORT extern const uint32_t ACCESS_MEMORY_WRITE_BIT;
BG2E_EXPORT extern const uint32_t ACCESS_COMMAND_PROCESS_READ_BIT_NVX;
BG2E_EXPORT extern const uint32_t ACCESS_COMMAND_PROCESS_WRITE_BIT_NVX;
BG2E_EXPORT extern const uint32_t ACCESS_COLOR_ATTACHMENT_READ_NONCOHERENT_BIT_EXT;
BG2E_EXPORT extern const uint32_t ACCESS_FLAG_BITS_MAX_ENUM;
typedef Flags AccessFlagBits;
typedef uint32_t AccessFlags;

BG2E_EXPORT extern const uint32_t DEPENDENCY_BY_REGION_BIT;
BG2E_EXPORT extern const uint32_t DEPENDENCY_DEVICE_GROUP_BIT;
BG2E_EXPORT extern const uint32_t DEPENDENCY_VIEW_LOCAL_BIT;
BG2E_EXPORT extern const uint32_t DEPENDENCY_VIEW_LOCAL_BIT_KHR;
BG2E_EXPORT extern const uint32_t DEPENDENCY_DEVICE_GROUP_BIT_KHR;
BG2E_EXPORT extern const uint32_t DEPENDENCY_FLAG_BITS_MAX_ENUM;
typedef Flags DependencyFlagBits;
typedef uint32_t DependencyFlags;

typedef struct MemoryRequirements {
    DeviceSize    size;
    DeviceSize    alignment;
    uint32_t        memoryTypeBits;
    
    inline void operator =(const MemoryRequirements & other) {
        size = other.size;
        alignment = other.alignment;
        memoryTypeBits = other.memoryTypeBits;
    }
} MemoryRequirements;

// TODO: Allocator
typedef void * Allocator;

class BG2E_EXPORT Instance : public bg::base::ReferencedPointer {
public:
	struct ApplicationInfo {
		std::string applicationName = "No name";
		uint32_t applicationVersion = makeVersion(1, 0, 0);
		std::string engineName = "bg2 engine";
		uint32_t engineVersion = makeVersion(3, 1, 0);
		uint32_t apiVersion = API_VERSION_1_0;
	};

	ApplicationInfo applicationInfo;

	std::vector<std::string> enabledExtensions;
	std::vector<std::string> enabledLayers;

	Instance(Allocator * alloc = nullptr);

	Result create();

	void * getProcAddr(const std::string & fnName);

	bg::plain_ptr vkInstance() { return _vk_instance; }

protected:
	virtual ~Instance();

	bg::plain_ptr _vk_instance = nullptr;
};


typedef struct PhysicalDeviceFeatures {
	bool    robustBufferAccess = false;
	bool    fullDrawIndexUint32 = false;
	bool    imageCubeArray = false;
	bool    independentBlend = false;
	bool    geometryShader = false;
	bool    tessellationShader = false;
	bool    sampleRateShading = false;
	bool    dualSrcBlend = false;
	bool    logicOp = false;
	bool    multiDrawIndirect = false;
	bool    drawIndirectFirstInstance = false;
	bool    depthClamp = false;
	bool    depthBiasClamp = false;
	bool    fillModeNonSolid = false;
	bool    depthBounds = false;
	bool    wideLines = false;
	bool    largePoints = false;
	bool    alphaToOne = false;
	bool    multiViewport = false;
	bool    samplerAnisotropy = false;
	bool    textureCompressionETC2 = false;
	bool    textureCompressionASTC_LDR = false;
	bool    textureCompressionBC = false;
	bool    occlusionQueryPrecise = false;
	bool    pipelineStatisticsQuery = false;
	bool    vertexPipelineStoresAndAtomics = false;
	bool    fragmentStoresAndAtomics = false;
	bool    shaderTessellationAndGeometryPointSize = false;
	bool    shaderImageGatherExtended = false;
	bool    shaderStorageImageExtendedFormats = false;
	bool    shaderStorageImageMultisample = false;
	bool    shaderStorageImageReadWithoutFormat = false;
	bool    shaderStorageImageWriteWithoutFormat = false;
	bool    shaderUniformBufferArrayDynamicIndexing = false;
	bool    shaderSampledImageArrayDynamicIndexing = false;
	bool    shaderStorageBufferArrayDynamicIndexing = false;
	bool    shaderStorageImageArrayDynamicIndexing = false;
	bool    shaderClipDistance = false;
	bool    shaderCullDistance = false;
	bool    shaderFloat64 = false;
	bool    shaderInt64 = false;
	bool    shaderInt16 = false;
	bool    shaderResourceResidency = false;
	bool    shaderResourceMinLod = false;
	bool    sparseBinding = false;
	bool    sparseResidencyBuffer = false;
	bool    sparseResidencyImage2D = false;
	bool    sparseResidencyImage3D = false;
	bool    sparseResidency2Samples = false;
	bool    sparseResidency4Samples = false;
	bool    sparseResidency8Samples = false;
	bool    sparseResidency16Samples = false;
	bool    sparseResidencyAliased = false;
	bool    variableMultisampleRate = false;
	bool    inheritedQueries = false;
} PhysicalDeviceFeatures;

class PhysicalDevice;
class BG2E_EXPORT ExtensionProperties {
public:
	typedef std::vector<ExtensionProperties> Vector;

	std::string extensionName;
	uint32_t specVersion;

	static void Enumerate(Vector & result);
	static void Enumerate(PhysicalDevice *, Vector & result);
	static bool CheckSupport(const std::vector<std::string> & required);
	static bool CheckSupport(const std::string & name);
	static bool CheckSupport(Vector & available, const std::vector<std::string> & required);

	ExtensionProperties() {}
	ExtensionProperties(const char * name, uint32_t specVersion);


	inline bool operator == (const ExtensionProperties & other) const { return other.extensionName == extensionName && other.specVersion == specVersion; }
	inline void operator = (const ExtensionProperties & other) { extensionName = other.extensionName; specVersion = other.specVersion; }
};

typedef struct PhysicalDeviceLimits {
	uint32_t            maxImageDimension1D;
	uint32_t            maxImageDimension2D;
	uint32_t            maxImageDimension3D;
	uint32_t            maxImageDimensionCube;
	uint32_t            maxImageArrayLayers;
	uint32_t            maxTexelBufferElements;
	uint32_t            maxUniformBufferRange;
	uint32_t            maxStorageBufferRange;
	uint32_t            maxPushConstantsSize;
	uint32_t            maxMemoryAllocationCount;
	uint32_t            maxSamplerAllocationCount;
	DeviceSize          bufferImageGranularity;
	DeviceSize          sparseAddressSpaceSize;
	uint32_t            maxBoundDescriptorSets;
	uint32_t            maxPerStageDescriptorSamplers;
	uint32_t            maxPerStageDescriptorUniformBuffers;
	uint32_t            maxPerStageDescriptorStorageBuffers;
	uint32_t            maxPerStageDescriptorSampledImages;
	uint32_t            maxPerStageDescriptorStorageImages;
	uint32_t            maxPerStageDescriptorInputAttachments;
	uint32_t            maxPerStageResources;
	uint32_t            maxDescriptorSetSamplers;
	uint32_t            maxDescriptorSetUniformBuffers;
	uint32_t            maxDescriptorSetUniformBuffersDynamic;
	uint32_t            maxDescriptorSetStorageBuffers;
	uint32_t            maxDescriptorSetStorageBuffersDynamic;
	uint32_t            maxDescriptorSetSampledImages;
	uint32_t            maxDescriptorSetStorageImages;
	uint32_t            maxDescriptorSetInputAttachments;
	uint32_t            maxVertexInputAttributes;
	uint32_t            maxVertexInputBindings;
	uint32_t            maxVertexInputAttributeOffset;
	uint32_t            maxVertexInputBindingStride;
	uint32_t            maxVertexOutputComponents;
	uint32_t            maxTessellationGenerationLevel;
	uint32_t            maxTessellationPatchSize;
	uint32_t            maxTessellationControlPerVertexInputComponents;
	uint32_t            maxTessellationControlPerVertexOutputComponents;
	uint32_t            maxTessellationControlPerPatchOutputComponents;
	uint32_t            maxTessellationControlTotalOutputComponents;
	uint32_t            maxTessellationEvaluationInputComponents;
	uint32_t            maxTessellationEvaluationOutputComponents;
	uint32_t            maxGeometryShaderInvocations;
	uint32_t            maxGeometryInputComponents;
	uint32_t            maxGeometryOutputComponents;
	uint32_t            maxGeometryOutputVertices;
	uint32_t            maxGeometryTotalOutputComponents;
	uint32_t            maxFragmentInputComponents;
	uint32_t            maxFragmentOutputAttachments;
	uint32_t            maxFragmentDualSrcAttachments;
	uint32_t            maxFragmentCombinedOutputResources;
	uint32_t            maxComputeSharedMemorySize;
	uint32_t            maxComputeWorkGroupCount[3];
	uint32_t            maxComputeWorkGroupInvocations;
	uint32_t            maxComputeWorkGroupSize[3];
	uint32_t            subPixelPrecisionBits;
	uint32_t            subTexelPrecisionBits;
	uint32_t            mipmapPrecisionBits;
	uint32_t            maxDrawIndexedIndexValue;
	uint32_t            maxDrawIndirectCount;
	float               maxSamplerLodBias;
	float               maxSamplerAnisotropy;
	uint32_t            maxViewports;
	uint32_t            maxViewportDimensions[2];
	float               viewportBoundsRange[2];
	uint32_t            viewportSubPixelBits;
	size_t              minMemoryMapAlignment;
	DeviceSize          minTexelBufferOffsetAlignment;
	DeviceSize          minUniformBufferOffsetAlignment;
	DeviceSize          minStorageBufferOffsetAlignment;
	int32_t             minTexelOffset;
	uint32_t            maxTexelOffset;
	int32_t             minTexelGatherOffset;
	uint32_t            maxTexelGatherOffset;
	float               minInterpolationOffset;
	float               maxInterpolationOffset;
	uint32_t            subPixelInterpolationOffsetBits;
	uint32_t            maxFramebufferWidth;
	uint32_t            maxFramebufferHeight;
	uint32_t            maxFramebufferLayers;
	Flags				framebufferColorSampleCounts;
	Flags				framebufferDepthSampleCounts;
	Flags				framebufferStencilSampleCounts;
	Flags				framebufferNoAttachmentsSampleCounts;
	uint32_t            maxColorAttachments;
	Flags				sampledImageColorSampleCounts;
	Flags				sampledImageIntegerSampleCounts;
	Flags				sampledImageDepthSampleCounts;
	Flags				sampledImageStencilSampleCounts;
	Flags				storageImageSampleCounts;
	uint32_t            maxSampleMaskWords;
	bool				timestampComputeAndGraphics;
	float               timestampPeriod;
	uint32_t            maxClipDistances;
	uint32_t            maxCullDistances;
	uint32_t            maxCombinedClipAndCullDistances;
	uint32_t            discreteQueuePriorities;
	float               pointSizeRange[2];
	float               lineWidthRange[2];
	float               pointSizeGranularity;
	float               lineWidthGranularity;
	bool				strictLines;
	bool				standardSampleLocations;
	DeviceSize          optimalBufferCopyOffsetAlignment;
	DeviceSize          optimalBufferCopyRowPitchAlignment;
	DeviceSize          nonCoherentAtomSize;
} PhysicalDeviceLimits;

typedef struct PhysicalDeviceSparseProperties {
	bool    residencyStandard2DBlockShape;
	bool    residencyStandard2DMultisampleBlockShape;
	bool    residencyStandard3DBlockShape;
	bool    residencyAlignedMipSize;
	bool    residencyNonResidentStrict;
} PhysicalDeviceSparseProperties;

class BG2E_EXPORT LayerProperties {
public:
	typedef std::vector<LayerProperties> Vector;

	std::string layerName;
	uint32_t specVersion;
	uint32_t implementationVersion;
	std::string description;

	static void Enumerate(Vector & result);
	static bool CheckSupport(const std::vector<std::string> & required);
	static bool CheckSupport(const std::string & name);
	
	LayerProperties() {}
	LayerProperties(const char * name, uint32_t specVersion, uint32_t implementationVersion, const char * description);


	inline bool operator == (const LayerProperties & other) const { return other.layerName == layerName && other.specVersion == specVersion && other.implementationVersion == implementationVersion; }
	inline void operator = (const LayerProperties & other) { layerName = other.layerName; specVersion = other.specVersion; implementationVersion = other.implementationVersion; description = other.description; }
};

typedef struct Extent2D {
	uint32_t width;
	uint32_t height;

	inline void operator = (const Extent2D & other) {
		width = other.width;
		height = other.height;
	}
} Extent2D;

typedef struct Extent3D {
	uint32_t width;
	uint32_t height;
	uint32_t depth;

	inline void operator = (const Extent3D & other) {
		width = other.width;
		height = other.height;
		depth = other.depth;
	}
} Extent3D;

typedef struct Viewport {
    float    x = 0.0f;
    float    y = 0.0f;
    float    width = 0.0f;
    float    height =  0.0f;
    float    minDepth = 0.0f;
    float    maxDepth = 1.0f;

	inline void operator = (const Viewport & other) {
		x = other.x;
		y = other.y;
		width = other.width;
		height = other.height;
		minDepth = other.minDepth;
		maxDepth = other.maxDepth;
	}
} Viewport;

typedef struct Offset2D {
    int32_t    x;
    int32_t    y;

	inline void operator =(const Offset2D & other) {
		x = other.x;
		y = other.y;
	}
} Offset2D;

typedef struct Offset3D {
    int32_t    x;
    int32_t    y;
    int32_t    z;
    
    inline void operator = (const Offset3D & other) {
        x = other.x;
        y = other.y;
        z = other.z;
    }
} Offset3D;

typedef struct Rect2D {
    Offset2D    offset;
    Extent2D    extent;

	inline void operator =(const Rect2D & other) {
		offset = other.offset;
		extent = other.extent;
	}
} Rect2D;

BG2E_EXPORT extern const int32_t PHYSICAL_DEVICE_TYPE_OTHER;
BG2E_EXPORT extern const int32_t PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU;
BG2E_EXPORT extern const int32_t PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
BG2E_EXPORT extern const int32_t PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU;
BG2E_EXPORT extern const int32_t PHYSICAL_DEVICE_TYPE_CPU;
BG2E_EXPORT extern const int32_t PHYSICAL_DEVICE_TYPE_BEGIN_RANGE;
BG2E_EXPORT extern const int32_t PHYSICAL_DEVICE_TYPE_END_RANGE;
BG2E_EXPORT extern const int32_t PHYSICAL_DEVICE_TYPE_RANGE_SIZE;
BG2E_EXPORT extern const int32_t PHYSICAL_DEVICE_TYPE_MAX_ENUM;
typedef uint32_t PhysicalDeviceType;

BG2E_EXPORT extern const int32_t MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
BG2E_EXPORT extern const int32_t MEMORY_PROPERTY_HOST_VISIBLE_BIT;
BG2E_EXPORT extern const int32_t MEMORY_PROPERTY_HOST_COHERENT_BIT;
BG2E_EXPORT extern const int32_t MEMORY_PROPERTY_HOST_CACHED_BIT;
BG2E_EXPORT extern const int32_t MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT;
BG2E_EXPORT extern const int32_t MEMORY_PROPERTY_PROTECTED_BIT;
BG2E_EXPORT extern const int32_t MEMORY_PROPERTY_FLAG_BITS_MAX_ENUM;
typedef Flags MemoryPropertyFlags;

BG2E_EXPORT extern const uint32_t MEMORY_HEAP_DEVICE_LOCAL_BIT;
BG2E_EXPORT extern const uint32_t MEMORY_HEAP_MULTI_INSTANCE_BIT;
BG2E_EXPORT extern const uint32_t MEMORY_HEAP_MULTI_INSTANCE_BIT_KHR;
BG2E_EXPORT extern const uint32_t MEMORY_HEAP_FLAG_BITS_MAX_ENUM;
typedef Flags MemoryHeapFlags;

BG2E_EXPORT extern const uint32_t QUEUE_GRAPHICS_BIT;
BG2E_EXPORT extern const uint32_t QUEUE_COMPUTE_BIT;
BG2E_EXPORT extern const uint32_t QUEUE_TRANSFER_BIT;
BG2E_EXPORT extern const uint32_t QUEUE_SPARSE_BINDING_BIT;
BG2E_EXPORT extern const uint32_t QUEUE_PROTECTED_BIT;
BG2E_EXPORT extern const uint32_t QUEUE_FLAG_BITS_MAX_ENUM;
typedef Flags QueueFlags;

typedef struct MemoryType {
	MemoryPropertyFlags	propertyFlags;
	uint32_t heapIndex;
} MemoryType;

typedef struct MemoryHeap {
	DeviceSize size;
	MemoryHeapFlags flags;
} MemoryHeap;

typedef struct PhysicalDeviceMemoryProperties {
	uint32_t      memoryTypeCount;
	MemoryType    memoryTypes[BG_VK_MAX_MEMORY_TYPES];
	uint32_t      memoryHeapCount;
	MemoryHeap    memoryHeaps[BG_VK_MAX_MEMORY_HEAPS];
} PhysicalDeviceMemoryProperties;

typedef struct QueueFamilyProperties {
	QueueFlags queueFlags;
	uint32_t queueCount;
	uint32_t timestampValidBits;
	Extent3D minImageTransferGranularity;

	QueueFamilyProperties(QueueFlags f, uint32_t c, uint32_t t, Extent3D g)
		:queueFlags(f), queueCount(c), timestampValidBits(t), minImageTransferGranularity(g)
	{}
} QueueFamilyProperties;

class SurfaceKHR;
class BG2E_EXPORT PhysicalDevice : public bg::base::ReferencedPointer {
public:
	typedef std::vector<bg::ptr<PhysicalDevice>> Vector;

	uint32_t apiVersion;
	uint32_t driverVersion;
	uint32_t vendorID;
	uint32_t deviceID;
	PhysicalDeviceType deviceType;
	std::string deviceName;
	uint8_t pipelineCacheUUID[BG_VK_UUID_SIZE];
	PhysicalDeviceLimits limits;
	PhysicalDeviceSparseProperties sparseProperties;
	PhysicalDeviceMemoryProperties memoryProperties;
	ExtensionProperties::Vector extensions;
	std::vector<QueueFamilyProperties> queueFamilyProperties;

	// Extra properties
	std::string vendorName;
	uint64_t vramSize;
	std::string vramString;

	static void Enumerate(Instance *, Vector &);

	inline bg::plain_ptr vkDevice() { return _vk_device; }
    

protected:
	PhysicalDevice();
	virtual ~PhysicalDevice();

	bg::plain_ptr _vk_device = nullptr;
	Instance * _instance;
};

BG2E_EXPORT extern const uint32_t SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
BG2E_EXPORT extern const uint32_t SURFACE_TRANSFORM_ROTATE_90_BIT_KHR;
BG2E_EXPORT extern const uint32_t SURFACE_TRANSFORM_ROTATE_180_BIT_KHR;
BG2E_EXPORT extern const uint32_t SURFACE_TRANSFORM_ROTATE_270_BIT_KHR;
BG2E_EXPORT extern const uint32_t SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR;
BG2E_EXPORT extern const uint32_t SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR;
BG2E_EXPORT extern const uint32_t SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR;
BG2E_EXPORT extern const uint32_t SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR;
BG2E_EXPORT extern const uint32_t SURFACE_TRANSFORM_INHERIT_BIT_KHR;
BG2E_EXPORT extern const uint32_t SURFACE_TRANSFORM_FLAG_BITS_MAX_ENUM_KHR;
typedef Flags SurfaceGransformFlagsKHR;
typedef uint32_t SurfaceTransformFlagBitsKHR;

BG2E_EXPORT extern const uint32_t COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
BG2E_EXPORT extern const uint32_t COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR;
BG2E_EXPORT extern const uint32_t COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR;
BG2E_EXPORT extern const uint32_t COMPOSITE_ALPHA_INHERIT_BIT_KHR;
BG2E_EXPORT extern const uint32_t COMPOSITE_ALPHA_FLAG_BITS_MAX_ENUM_KHR;
typedef Flags CompositeAlphaFlagsKHR;
typedef uint32_t CompositeAlphaFlagBitsKHR;

BG2E_EXPORT extern const uint32_t IMAGE_USAGE_TRANSFER_SRC_BIT;
BG2E_EXPORT extern const uint32_t IMAGE_USAGE_TRANSFER_DST_BIT;
BG2E_EXPORT extern const uint32_t IMAGE_USAGE_SAMPLED_BIT;
BG2E_EXPORT extern const uint32_t IMAGE_USAGE_STORAGE_BIT;
BG2E_EXPORT extern const uint32_t IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
BG2E_EXPORT extern const uint32_t IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
BG2E_EXPORT extern const uint32_t IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT;
BG2E_EXPORT extern const uint32_t IMAGE_USAGE_INPUT_ATTACHMENT_BIT;
BG2E_EXPORT extern const uint32_t IMAGE_USAGE_FLAG_BITS_MAX_ENUM;
typedef Flags ImageUsageFlags;

BG2E_EXPORT extern const uint32_t PRESENT_MODE_IMMEDIATE_KHR;
BG2E_EXPORT extern const uint32_t PRESENT_MODE_MAILBOX_KHR;
BG2E_EXPORT extern const uint32_t PRESENT_MODE_FIFO_KHR;
BG2E_EXPORT extern const uint32_t PRESENT_MODE_FIFO_RELAXED_KHR;
BG2E_EXPORT extern const uint32_t PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR;
BG2E_EXPORT extern const uint32_t PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR;
BG2E_EXPORT extern const uint32_t PRESENT_MODE_BEGIN_RANGE_KHR;
BG2E_EXPORT extern const uint32_t PRESENT_MODE_END_RANGE_KHR;
BG2E_EXPORT extern const uint32_t PRESENT_MODE_RANGE_SIZE_KHR;
BG2E_EXPORT extern const uint32_t PRESENT_MODE_MAX_ENUM_KHR;
typedef Flags PresentModeKHR;

BG2E_EXPORT extern const uint32_t SHARING_MODE_EXCLUSIVE;
BG2E_EXPORT extern const uint32_t SHARING_MODE_CONCURRENT;
BG2E_EXPORT extern const uint32_t SHARING_MODE_BEGIN_RANGE;
BG2E_EXPORT extern const uint32_t SHARING_MODE_END_RANGE;
BG2E_EXPORT extern const uint32_t SHARING_MODE_RANGE_SIZE;
BG2E_EXPORT extern const uint32_t SHARING_MODE_MAX_ENUM;
typedef Flags SharingMode;

BG2E_EXPORT extern const uint32_t IMAGE_VIEW_TYPE_1D;
BG2E_EXPORT extern const uint32_t IMAGE_VIEW_TYPE_2D;
BG2E_EXPORT extern const uint32_t IMAGE_VIEW_TYPE_3D;
BG2E_EXPORT extern const uint32_t IMAGE_VIEW_TYPE_CUBE;
BG2E_EXPORT extern const uint32_t IMAGE_VIEW_TYPE_1D_ARRAY;
BG2E_EXPORT extern const uint32_t IMAGE_VIEW_TYPE_2D_ARRAY;
BG2E_EXPORT extern const uint32_t IMAGE_VIEW_TYPE_CUBE_ARRAY;
BG2E_EXPORT extern const uint32_t IMAGE_VIEW_TYPE_BEGIN_RANGE;
BG2E_EXPORT extern const uint32_t IMAGE_VIEW_TYPE_END_RANGE;
BG2E_EXPORT extern const uint32_t IMAGE_VIEW_TYPE_RANGE_SIZE;
BG2E_EXPORT extern const uint32_t IMAGE_VIEW_TYPE_MAX_ENUM;
typedef Flags ImageViewType;

BG2E_EXPORT extern const uint32_t COMPONENT_SWIZZLE_IDENTITY;
BG2E_EXPORT extern const uint32_t COMPONENT_SWIZZLE_ZERO;
BG2E_EXPORT extern const uint32_t COMPONENT_SWIZZLE_ONE;
BG2E_EXPORT extern const uint32_t COMPONENT_SWIZZLE_R;
BG2E_EXPORT extern const uint32_t COMPONENT_SWIZZLE_G;
BG2E_EXPORT extern const uint32_t COMPONENT_SWIZZLE_B;
BG2E_EXPORT extern const uint32_t COMPONENT_SWIZZLE_A;
BG2E_EXPORT extern const uint32_t COMPONENT_SWIZZLE_BEGIN_RANGE;
BG2E_EXPORT extern const uint32_t COMPONENT_SWIZZLE_END_RANGE;
BG2E_EXPORT extern const uint32_t COMPONENT_SWIZZLE_RANGE_SIZE;
BG2E_EXPORT extern const uint32_t COMPONENT_SWIZZLE_MAX_ENUM;
typedef Flags ComponentSwizzle;

BG2E_EXPORT extern const uint32_t IMAGE_ASPECT_COLOR_BIT;
BG2E_EXPORT extern const uint32_t IMAGE_ASPECT_DEPTH_BIT;
BG2E_EXPORT extern const uint32_t IMAGE_ASPECT_STENCIL_BIT;
BG2E_EXPORT extern const uint32_t IMAGE_ASPECT_METADATA_BIT;
BG2E_EXPORT extern const uint32_t IMAGE_ASPECT_PLANE_0_BIT;
BG2E_EXPORT extern const uint32_t IMAGE_ASPECT_PLANE_1_BIT;
BG2E_EXPORT extern const uint32_t IMAGE_ASPECT_PLANE_2_BIT;
BG2E_EXPORT extern const uint32_t IMAGE_ASPECT_PLANE_0_BIT_KHR;
BG2E_EXPORT extern const uint32_t IMAGE_ASPECT_PLANE_1_BIT_KHR;
BG2E_EXPORT extern const uint32_t IMAGE_ASPECT_PLANE_2_BIT_KHR;
BG2E_EXPORT extern const uint32_t IMAGE_ASPECT_FLAG_BITS_MAX_ENUM;
typedef Flags ImageAspectFlags;

typedef struct ComponentMapping {
    ComponentSwizzle    r;
    ComponentSwizzle    g;
    ComponentSwizzle    b;
    ComponentSwizzle    a;
} ComponentMapping;

typedef struct ImageSubresourceRange {
    ImageAspectFlags      aspectMask = 0;
    uint32_t              baseMipLevel = 0;
    uint32_t              levelCount = 0;
    uint32_t              baseArrayLayer = 0;
    uint32_t              layerCount = 0;
    
    ImageSubresourceRange() {}
    ImageSubresourceRange(ImageAspectFlags am, uint32_t bml, uint32_t lc, uint32_t bal, uint32_t layer)
        :aspectMask(am), baseMipLevel(bml), levelCount(lc), baseArrayLayer(bal), layerCount(layer) {}
    
    inline void operator =(const ImageSubresourceRange & other) {
        aspectMask = other.aspectMask;
        baseMipLevel = other.baseMipLevel;
        levelCount = other.levelCount;
        baseArrayLayer = other.baseArrayLayer;
        layerCount = other.layerCount;
    }
} ImageSubresourceRange;

typedef struct SurfaceCapabilitiesKHR {
	uint32_t                       minImageCount;
	uint32_t                       maxImageCount;
	Extent2D                       currentExtent;
	Extent2D                       minImageExtent;
	Extent2D                       maxImageExtent;
	uint32_t                       maxImageArrayLayers;
	SurfaceGransformFlagsKHR       supportedTransforms;
	SurfaceTransformFlagBitsKHR    currentTransform;
	CompositeAlphaFlagsKHR         supportedCompositeAlpha;
	ImageUsageFlags                supportedUsageFlags;
} SurfaceCapabilitiesKHR;

typedef struct SurfaceFormatKHR {
	Format format;
	ColorSpaceKHR colorSpace;
} SurfaceFormatKHR;

class BG2E_EXPORT SurfaceKHR : public bg::base::ReferencedPointer {
public:

	SurfaceKHR();

	void create(Instance *, bg::wnd::Window * window);

	void getCapabilities(PhysicalDevice * dev, SurfaceCapabilitiesKHR & capabilities);
	void getFormats(PhysicalDevice * dev, std::vector<SurfaceFormatKHR> & formats);
    void getPresentModes(PhysicalDevice * dev, std::vector<PresentModeKHR> & presentModes);
	
	inline bg::plain_ptr vkSurface() { return _vk_surface; }

protected:
	virtual ~SurfaceKHR();

	bg::plain_ptr _vk_instance = nullptr;
	bg::plain_ptr _vk_surface = nullptr;
};

class Queue;
class Semaphore;
class Fence;
class BG2E_EXPORT Device : public bg::base::ReferencedPointer {
public:
	void setEnabledExtensions(const std::vector<std::string> & ext);
	void setEnabledLayers(const std::vector<std::string> & layers);
	
	PhysicalDeviceFeatures enabledFeatures;

	void create(PhysicalDevice * physicalDev, const std::set<int32_t> queueFamilies);

	inline bg::plain_ptr vkDevice() { return _vk_device; }

	Queue * getDeviceQueue(uint32_t family);

	void waitIdle();
	void waitForFences(const std::vector<Fence*> & fences, bool waitAll, uint64_t timeout);
	void resetFences(const std::vector<Fence*> & fences);

protected:
	virtual ~Device();

	bg::plain_ptr _vk_device = nullptr;

	std::vector<std::string> _extensions;
	std::vector<std::string> _layers;

	std::vector<bg::ptr<Queue>> _queues;
};

class CommandBuffer;
class SwapchainKHR;
class BG2E_EXPORT Queue : public bg::base::ReferencedPointer {
	friend class Device;
public:
	// A queue must be invalid if the associated device is destroyed
	inline bool valid() const { return _vk_queue != nullptr; }

	inline bg::plain_ptr vkQueue() { return _vk_queue; }

	inline Device * device() { return _device; }

	void submit(const std::map<PipelineStageFlags,Semaphore*> & waitSem, const std::vector<CommandBuffer*> & cmdBuff, const std::vector<Semaphore*> & signalSem, Fence * fence = nullptr);
	void present(const std::vector<Semaphore*> & waitSem, const std::vector<SwapchainKHR*> & swapchains, const std::vector<uint32_t> & imgIndices);
    void submitWaitIdle(const std::vector<CommandBuffer*> & cmdBuffer);
	void waitIdle();

protected:
	Queue(Device * dev, uint32_t familyIndex);
	virtual ~Queue();

	bg::plain_ptr _vk_queue = nullptr;
	Device * _device = nullptr;
};

BG2E_EXPORT extern const uint32_t IMAGE_CREATE_SPARSE_BINDING_BIT;
BG2E_EXPORT extern const uint32_t IMAGE_CREATE_SPARSE_RESIDENCY_BIT;
BG2E_EXPORT extern const uint32_t IMAGE_CREATE_SPARSE_ALIASED_BIT;
BG2E_EXPORT extern const uint32_t IMAGE_CREATE_MUTABLE_FORMAT_BIT;
BG2E_EXPORT extern const uint32_t IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
BG2E_EXPORT extern const uint32_t IMAGE_CREATE_ALIAS_BIT;
BG2E_EXPORT extern const uint32_t IMAGE_CREATE_SPLIT_INSTANCE_BIND_REGIONS_BIT;
BG2E_EXPORT extern const uint32_t IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT;
BG2E_EXPORT extern const uint32_t IMAGE_CREATE_BLOCK_TEXEL_VIEW_COMPATIBLE_BIT;
BG2E_EXPORT extern const uint32_t IMAGE_CREATE_EXTENDED_USAGE_BIT;
BG2E_EXPORT extern const uint32_t IMAGE_CREATE_PROTECTED_BIT;
BG2E_EXPORT extern const uint32_t IMAGE_CREATE_DISJOINT_BIT;
BG2E_EXPORT extern const uint32_t IMAGE_CREATE_SAMPLE_LOCATIONS_COMPATIBLE_DEPTH_BIT_EXT;
BG2E_EXPORT extern const uint32_t IMAGE_CREATE_SPLIT_INSTANCE_BIND_REGIONS_BIT_KHR;
BG2E_EXPORT extern const uint32_t IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT_KHR;
BG2E_EXPORT extern const uint32_t IMAGE_CREATE_BLOCK_TEXEL_VIEW_COMPATIBLE_BIT_KHR;
BG2E_EXPORT extern const uint32_t IMAGE_CREATE_EXTENDED_USAGE_BIT_KHR;
BG2E_EXPORT extern const uint32_t IMAGE_CREATE_DISJOINT_BIT_KHR;
BG2E_EXPORT extern const uint32_t IMAGE_CREATE_ALIAS_BIT_KHR;
BG2E_EXPORT extern const uint32_t IMAGE_CREATE_FLAG_BITS_MAX_ENUM;
typedef uint32_t ImageCreateFlagBits;
typedef Flags ImageCreateFlags;

BG2E_EXPORT extern const uint32_t IMAGE_TYPE_1D;
BG2E_EXPORT extern const uint32_t IMAGE_TYPE_2D;
BG2E_EXPORT extern const uint32_t IMAGE_TYPE_3D;
BG2E_EXPORT extern const uint32_t IMAGE_TYPE_BEGIN_RANGE;
BG2E_EXPORT extern const uint32_t IMAGE_TYPE_END_RANGE;
BG2E_EXPORT extern const uint32_t IMAGE_TYPE_RANGE_SIZE;
BG2E_EXPORT extern const uint32_t IMAGE_TYPE_MAX_ENUM;
typedef Flags ImageType;

BG2E_EXPORT extern const uint32_t IMAGE_TILING_OPTIMAL;
BG2E_EXPORT extern const uint32_t IMAGE_TILING_LINEAR;
BG2E_EXPORT extern const uint32_t IMAGE_TILING_BEGIN_RANGE;
BG2E_EXPORT extern const uint32_t IMAGE_TILING_END_RANGE;
BG2E_EXPORT extern const uint32_t IMAGE_TILING_RANGE_SIZE;
BG2E_EXPORT extern const uint32_t IMAGE_TILING_MAX_ENUM;
typedef Flags ImageTiling;

BG2E_EXPORT extern const uint32_t SAMPLE_COUNT_1_BIT;
BG2E_EXPORT extern const uint32_t SAMPLE_COUNT_2_BIT;
BG2E_EXPORT extern const uint32_t SAMPLE_COUNT_4_BIT;
BG2E_EXPORT extern const uint32_t SAMPLE_COUNT_8_BIT;
BG2E_EXPORT extern const uint32_t SAMPLE_COUNT_16_BIT;
BG2E_EXPORT extern const uint32_t SAMPLE_COUNT_32_BIT;
BG2E_EXPORT extern const uint32_t SAMPLE_COUNT_64_BIT;
BG2E_EXPORT extern const uint32_t SAMPLE_COUNT_FLAG_BITS_MAX_ENUM;
typedef uint32_t SampleCountFlagBits;
typedef Flags SampleCountFlags;

BG2E_EXPORT extern const uint32_t FILTER_NEAREST;
BG2E_EXPORT extern const uint32_t FILTER_LINEAR;
BG2E_EXPORT extern const uint32_t FILTER_CUBIC_IMG;
BG2E_EXPORT extern const uint32_t FILTER_BEGIN_RANGE;
BG2E_EXPORT extern const uint32_t FILTER_END_RANGE;
BG2E_EXPORT extern const uint32_t FILTER_RANGE_SIZE;
BG2E_EXPORT extern const uint32_t FILTER_MAX_ENUM;
typedef uint32_t Filter;

BG2E_EXPORT extern const uint32_t SAMPLER_MIPMAP_MODE_NEAREST;
BG2E_EXPORT extern const uint32_t SAMPLER_MIPMAP_MODE_LINEAR;
BG2E_EXPORT extern const uint32_t SAMPLER_MIPMAP_MODE_BEGIN_RANGE;
BG2E_EXPORT extern const uint32_t SAMPLER_MIPMAP_MODE_END_RANGE;
BG2E_EXPORT extern const uint32_t SAMPLER_MIPMAP_MODE_RANGE_SIZE;
BG2E_EXPORT extern const uint32_t SAMPLER_MIPMAP_MODE_MAX_ENUM;
typedef uint32_t SamplerMipmapMode;

BG2E_EXPORT extern const uint32_t SAMPLER_ADDRESS_MODE_REPEAT;
BG2E_EXPORT extern const uint32_t SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
BG2E_EXPORT extern const uint32_t SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
BG2E_EXPORT extern const uint32_t SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
BG2E_EXPORT extern const uint32_t SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
BG2E_EXPORT extern const uint32_t SAMPLER_ADDRESS_MODE_BEGIN_RANGE;
BG2E_EXPORT extern const uint32_t SAMPLER_ADDRESS_MODE_END_RANGE;
BG2E_EXPORT extern const uint32_t SAMPLER_ADDRESS_MODE_RANGE_SIZE;
BG2E_EXPORT extern const uint32_t SAMPLER_ADDRESS_MODE_MAX_ENUM;
typedef uint32_t SamplerAddressMode;

BG2E_EXPORT extern const uint32_t BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
BG2E_EXPORT extern const uint32_t BORDER_COLOR_INT_TRANSPARENT_BLACK;
BG2E_EXPORT extern const uint32_t BORDER_COLOR_FLOAT_OPAQUE_BLACK;
BG2E_EXPORT extern const uint32_t BORDER_COLOR_INT_OPAQUE_BLACK;
BG2E_EXPORT extern const uint32_t BORDER_COLOR_FLOAT_OPAQUE_WHITE;
BG2E_EXPORT extern const uint32_t BORDER_COLOR_INT_OPAQUE_WHITE;
BG2E_EXPORT extern const uint32_t BORDER_COLOR_BEGIN_RANGE;
BG2E_EXPORT extern const uint32_t BORDER_COLOR_END_RANGE;
BG2E_EXPORT extern const uint32_t BORDER_COLOR_RANGE_SIZE;
BG2E_EXPORT extern const uint32_t BORDER_COLOR_MAX_ENUM;
typedef uint32_t BorderColor;

class Sampler;
class BG2E_EXPORT Image : public bg::base::ReferencedPointer {
public:

    struct ImageCreateInfo {
        ImageCreateFlags       flags = 0;
        ImageType              imageType;
        Format                 format;
        Extent3D               extent;
        uint32_t               mipLevels;
        uint32_t               arrayLayers;
        SampleCountFlagBits    samples;
        ImageTiling            tiling;
        ImageUsageFlags        usage;
        SharingMode            sharingMode;
        std::vector<uint32_t>  queueFamilyIndices;
        ImageLayout            initialLayout;
    } createInfo;

    Image();
    Image(bg::plain_ptr vkImage);

    void create(Device *);
    
    inline MemoryRequirements getImageMemoryRequirements() { MemoryRequirements m; geImageMemoryRequirements(m); return m; }
    void geImageMemoryRequirements(MemoryRequirements & mem);

    inline bg::plain_ptr vkImage() { return _vk_image; }
    inline const bg::plain_ptr vkImage() const { return _vk_image; }
    
    template <class T>
    static void ToVkArray(const std::vector<bg::ptr<Image>> & src, std::vector<T> & dst) {
        for (auto & img : src) {
            dst.push_back(bg::native_cast<T>(img->vkImage()));
        }
    }
    
protected:
    virtual ~Image();
    
    bg::plain_ptr _vk_image = nullptr;
    bg::plain_ptr _vk_device = nullptr;
};

class BG2E_EXPORT ImageView : public bg::base::ReferencedPointer {
public:

	struct ImageViewCreateInfo {
		Image *                   image;
		ImageViewType            viewType;
		Format                   format;
		ComponentMapping         components;
		ImageSubresourceRange    subresourceRange;
	} createInfo;

    void create(Device * dev, Image * img, ImageViewType type, Format format, const ComponentMapping & mapping, const ImageSubresourceRange & subresourceRange);
	void create(Device *);
    
    inline bg::plain_ptr vkImageView() { return _vk_imageView; }
    
    template <class T>
    static void ToVkArray(const std::vector<bg::ptr<ImageView>> & src, std::vector<T> & dst) {
        for (auto & img : src) {
            dst.push_back(bg::native_cast<T>(img->vkImageView()));
        }
    }
    
protected:
    virtual ~ImageView();
    
    bg::plain_ptr _vk_imageView = nullptr;
    bg::plain_ptr _vk_device = nullptr;
};

class Semaphore;
class Fence;
class BG2E_EXPORT SwapchainKHR : public bg::base::ReferencedPointer {
public:

    void create(
        Device * dev,
        SurfaceKHR * surface,
        uint32_t minImageCount,
        Format imageFormat,
        ColorSpaceKHR colorSpace,
        Extent2D ext,
        uint32_t arrayLayers,
        ImageUsageFlags usageFlags,
        SharingMode sharingMode,
        std::vector<uint32_t> & queueFamilies,
        SurfaceTransformFlagBitsKHR preTransform,
        CompositeAlphaFlagBitsKHR compositeAlpha,
        PresentModeKHR presentMode,
        SwapchainKHR * oldSwapChain);
    
    inline bg::plain_ptr vkSwapchain() { return _vk_swapchain; }
    
    inline Format format() const { return _format; }
    inline const Extent2D & extent() const { return _extent; }

	void acquireNextImage(uint64_t timeout, Semaphore * sem, Fence *, uint32_t & imageIndex);
    
protected:
    virtual ~SwapchainKHR();
    
    bg::plain_ptr _vk_swapchain = nullptr;
    bg::plain_ptr _vk_device = nullptr;
    
    Format _format;
    Extent2D _extent;
};

class BG2E_EXPORT ShaderModule : public bg::base::ReferencedPointer {
public:
    typedef std::vector<char> Buffer;
    
    void create(Device * dev, const Buffer & buffer);

    inline bg::plain_ptr vkShaderModule() { return _vk_shaderModule; }
    
protected:
    virtual ~ShaderModule();
    
    bg::plain_ptr _vk_shaderModule = nullptr;
    bg::plain_ptr _vk_device = nullptr;
};

typedef struct SpecializationMapEntry {
    uint32_t    constantID;
    uint32_t    offset;
    size_t      size;
} SpecializationMapEntry;

typedef struct SpecializationInfo {
    std::vector<SpecializationMapEntry>    mapEntries;
    size_t                             dataSize;
    const void*                        pData;
} SpecializationInfo;

BG2E_EXPORT extern const uint32_t SHADER_STAGE_VERTEX_BIT;
BG2E_EXPORT extern const uint32_t SHADER_STAGE_TESSELLATION_CONTROL_BIT;
BG2E_EXPORT extern const uint32_t SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
BG2E_EXPORT extern const uint32_t SHADER_STAGE_GEOMETRY_BIT;
BG2E_EXPORT extern const uint32_t SHADER_STAGE_FRAGMENT_BIT;
BG2E_EXPORT extern const uint32_t SHADER_STAGE_COMPUTE_BIT;
BG2E_EXPORT extern const uint32_t SHADER_STAGE_ALL_GRAPHICS;
BG2E_EXPORT extern const uint32_t SHADER_STAGE_ALL;
BG2E_EXPORT extern const uint32_t SHADER_STAGE_FLAG_BITS_MAX_ENUM;
typedef Flags ShaderStageFlagBits;

typedef Flags PipelineVertexInputStateCreateFlags;
typedef Flags PipelineInputAssemblyStateCreateFlags;
typedef Flags PipelineTessellationStateCreateFlags;
typedef Flags PipelineViewportStateCreateFlags;
typedef Flags PipelineRasterizationStateCreateFlags;

BG2E_EXPORT extern const uint32_t VERTEX_INPUT_RATE_VERTEX;
BG2E_EXPORT extern const uint32_t VERTEX_INPUT_RATE_INSTANCE;
BG2E_EXPORT extern const uint32_t VERTEX_INPUT_RATE_BEGIN_RANGE;
BG2E_EXPORT extern const uint32_t VERTEX_INPUT_RATE_END_RANGE;
BG2E_EXPORT extern const uint32_t VERTEX_INPUT_RATE_RANGE_SIZE;
BG2E_EXPORT extern const uint32_t VERTEX_INPUT_RATE_MAX_ENUM;
typedef Flags VertexInputRate;

typedef struct VertexInputBindingDescription {
    uint32_t             binding;
    uint32_t             stride;
    VertexInputRate      inputRate;
    
    inline void operator = (const VertexInputBindingDescription & other) {
        binding = other.binding;
        stride = other.stride;
        inputRate = other.inputRate;
    }
} VertexInputBindingDescription;

typedef struct VertexInputAttributeDescription {
    uint32_t    location;
    uint32_t    binding;
    Format      format;
    uint32_t    offset;
    
    inline void operator = (const VertexInputAttributeDescription & other) {
        location = other.location;
        binding = other.binding;
        format = other.format;
        offset = other.offset;
    }
} VertexInputAttributeDescription;

BG2E_EXPORT extern const uint32_t PRIMITIVE_TOPOLOGY_POINT_LIST;
BG2E_EXPORT extern const uint32_t PRIMITIVE_TOPOLOGY_LINE_LIST;
BG2E_EXPORT extern const uint32_t PRIMITIVE_TOPOLOGY_LINE_STRIP;
BG2E_EXPORT extern const uint32_t PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
BG2E_EXPORT extern const uint32_t PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
BG2E_EXPORT extern const uint32_t PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
BG2E_EXPORT extern const uint32_t PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY;
BG2E_EXPORT extern const uint32_t PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY;
BG2E_EXPORT extern const uint32_t PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY;
BG2E_EXPORT extern const uint32_t PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY;
BG2E_EXPORT extern const uint32_t PRIMITIVE_TOPOLOGY_PATCH_LIST;
BG2E_EXPORT extern const uint32_t PRIMITIVE_TOPOLOGY_BEGIN_RANGE;
BG2E_EXPORT extern const uint32_t PRIMITIVE_TOPOLOGY_END_RANGE;
BG2E_EXPORT extern const uint32_t PRIMITIVE_TOPOLOGY_RANGE_SIZE;
BG2E_EXPORT extern const uint32_t PRIMITIVE_TOPOLOGY_MAX_ENUM;
typedef Flags PrimitiveTopology;

BG2E_EXPORT extern const uint32_t POLYGON_MODE_FILL;
BG2E_EXPORT extern const uint32_t POLYGON_MODE_LINE;
BG2E_EXPORT extern const uint32_t POLYGON_MODE_POINT;
BG2E_EXPORT extern const uint32_t POLYGON_MODE_FILL_RECTANGLE_NV;
BG2E_EXPORT extern const uint32_t POLYGON_MODE_BEGIN_RANGE;
BG2E_EXPORT extern const uint32_t POLYGON_MODE_END_RANGE;
BG2E_EXPORT extern const uint32_t POLYGON_MODE_RANGE_SIZE;
BG2E_EXPORT extern const uint32_t POLYGON_MODE_MAX_ENUM;
typedef Flags PolygonMode;

BG2E_EXPORT extern const uint32_t FRONT_FACE_COUNTER_CLOCKWISE;
BG2E_EXPORT extern const uint32_t FRONT_FACE_CLOCKWISE;
BG2E_EXPORT extern const uint32_t FRONT_FACE_BEGIN_RANGE;
BG2E_EXPORT extern const uint32_t FRONT_FACE_END_RANGE;
BG2E_EXPORT extern const uint32_t FRONT_FACE_RANGE_SIZE;
BG2E_EXPORT extern const uint32_t FRONT_FACE_MAX_ENUM;
typedef Flags FrontFace;

BG2E_EXPORT extern const uint32_t COMPARE_OP_NEVER;
BG2E_EXPORT extern const uint32_t COMPARE_OP_LESS;
BG2E_EXPORT extern const uint32_t COMPARE_OP_EQUAL;
BG2E_EXPORT extern const uint32_t COMPARE_OP_LESS_OR_EQUAL;
BG2E_EXPORT extern const uint32_t COMPARE_OP_GREATER;
BG2E_EXPORT extern const uint32_t COMPARE_OP_NOT_EQUAL;
BG2E_EXPORT extern const uint32_t COMPARE_OP_GREATER_OR_EQUAL;
BG2E_EXPORT extern const uint32_t COMPARE_OP_ALWAYS;
BG2E_EXPORT extern const uint32_t COMPARE_OP_BEGIN_RANGE;
BG2E_EXPORT extern const uint32_t COMPARE_OP_END_RANGE;
BG2E_EXPORT extern const uint32_t COMPARE_OP_RANGE_SIZE;
BG2E_EXPORT extern const uint32_t COMPARE_OP_MAX_ENUM;
typedef Flags CompareOp;

BG2E_EXPORT extern const uint32_t STENCIL_OP_KEEP;
BG2E_EXPORT extern const uint32_t STENCIL_OP_ZERO;
BG2E_EXPORT extern const uint32_t STENCIL_OP_REPLACE;
BG2E_EXPORT extern const uint32_t STENCIL_OP_INCREMENT_AND_CLAMP;
BG2E_EXPORT extern const uint32_t STENCIL_OP_DECREMENT_AND_CLAMP;
BG2E_EXPORT extern const uint32_t STENCIL_OP_INVERT;
BG2E_EXPORT extern const uint32_t STENCIL_OP_INCREMENT_AND_WRAP;
BG2E_EXPORT extern const uint32_t STENCIL_OP_DECREMENT_AND_WRAP;
BG2E_EXPORT extern const uint32_t STENCIL_OP_BEGIN_RANGE;
BG2E_EXPORT extern const uint32_t STENCIL_OP_END_RANGE;
BG2E_EXPORT extern const uint32_t STENCIL_OP_RANGE_SIZE;
BG2E_EXPORT extern const uint32_t STENCIL_OP_MAX_ENUM;
typedef Flags StencilOp;

BG2E_EXPORT extern const uint32_t LOGIC_OP_CLEAR;
BG2E_EXPORT extern const uint32_t LOGIC_OP_AND;
BG2E_EXPORT extern const uint32_t LOGIC_OP_AND_REVERSE;
BG2E_EXPORT extern const uint32_t LOGIC_OP_COPY;
BG2E_EXPORT extern const uint32_t LOGIC_OP_AND_INVERTED;
BG2E_EXPORT extern const uint32_t LOGIC_OP_NO_OP;
BG2E_EXPORT extern const uint32_t LOGIC_OP_XOR;
BG2E_EXPORT extern const uint32_t LOGIC_OP_OR;
BG2E_EXPORT extern const uint32_t LOGIC_OP_NOR;
BG2E_EXPORT extern const uint32_t LOGIC_OP_EQUIVALENT;
BG2E_EXPORT extern const uint32_t LOGIC_OP_INVERT;
BG2E_EXPORT extern const uint32_t LOGIC_OP_OR_REVERSE;
BG2E_EXPORT extern const uint32_t LOGIC_OP_COPY_INVERTED;
BG2E_EXPORT extern const uint32_t LOGIC_OP_OR_INVERTED;
BG2E_EXPORT extern const uint32_t LOGIC_OP_NAND;
BG2E_EXPORT extern const uint32_t LOGIC_OP_SET;
BG2E_EXPORT extern const uint32_t LOGIC_OP_BEGIN_RANGE;
BG2E_EXPORT extern const uint32_t LOGIC_OP_END_RANGE;
BG2E_EXPORT extern const uint32_t LOGIC_OP_RANGE_SIZE;
BG2E_EXPORT extern const uint32_t LOGIC_OP_MAX_ENUM;
typedef Flags LogicOp;

BG2E_EXPORT extern const uint32_t BLEND_FACTOR_ZERO;
BG2E_EXPORT extern const uint32_t BLEND_FACTOR_ONE;
BG2E_EXPORT extern const uint32_t BLEND_FACTOR_SRC_COLOR;
BG2E_EXPORT extern const uint32_t BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
BG2E_EXPORT extern const uint32_t BLEND_FACTOR_DST_COLOR;
BG2E_EXPORT extern const uint32_t BLEND_FACTOR_ONE_MINUS_DST_COLOR;
BG2E_EXPORT extern const uint32_t BLEND_FACTOR_SRC_ALPHA;
BG2E_EXPORT extern const uint32_t BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
BG2E_EXPORT extern const uint32_t BLEND_FACTOR_DST_ALPHA;
BG2E_EXPORT extern const uint32_t BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
BG2E_EXPORT extern const uint32_t BLEND_FACTOR_CONSTANT_COLOR;
BG2E_EXPORT extern const uint32_t BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
BG2E_EXPORT extern const uint32_t BLEND_FACTOR_CONSTANT_ALPHA;
BG2E_EXPORT extern const uint32_t BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
BG2E_EXPORT extern const uint32_t BLEND_FACTOR_SRC_ALPHA_SATURATE;
BG2E_EXPORT extern const uint32_t BLEND_FACTOR_SRC1_COLOR;
BG2E_EXPORT extern const uint32_t BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;
BG2E_EXPORT extern const uint32_t BLEND_FACTOR_SRC1_ALPHA;
BG2E_EXPORT extern const uint32_t BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;
BG2E_EXPORT extern const uint32_t BLEND_FACTOR_BEGIN_RANGE;
BG2E_EXPORT extern const uint32_t BLEND_FACTOR_END_RANGE;
BG2E_EXPORT extern const uint32_t BLEND_FACTOR_RANGE_SIZE;
BG2E_EXPORT extern const uint32_t BLEND_FACTOR_MAX_ENUM;
typedef Flags BlendFactor;

BG2E_EXPORT extern const uint32_t BLEND_OP_ADD;
BG2E_EXPORT extern const uint32_t BLEND_OP_SUBTRACT;
BG2E_EXPORT extern const uint32_t BLEND_OP_REVERSE_SUBTRACT;
BG2E_EXPORT extern const uint32_t BLEND_OP_MIN;
BG2E_EXPORT extern const uint32_t BLEND_OP_MAX;
BG2E_EXPORT extern const uint32_t BLEND_OP_ZERO_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_SRC_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_DST_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_SRC_OVER_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_DST_OVER_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_SRC_IN_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_DST_IN_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_SRC_OUT_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_DST_OUT_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_SRC_ATOP_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_DST_ATOP_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_XOR_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_MULTIPLY_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_SCREEN_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_OVERLAY_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_DARKEN_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_LIGHTEN_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_COLORDODGE_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_COLORBURN_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_HARDLIGHT_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_SOFTLIGHT_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_DIFFERENCE_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_EXCLUSION_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_INVERT_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_INVERT_RGB_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_LINEARDODGE_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_LINEARBURN_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_VIVIDLIGHT_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_LINEARLIGHT_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_PINLIGHT_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_HARDMIX_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_HSL_HUE_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_HSL_SATURATION_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_HSL_COLOR_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_HSL_LUMINOSITY_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_PLUS_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_PLUS_CLAMPED_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_PLUS_CLAMPED_ALPHA_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_PLUS_DARKER_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_MINUS_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_MINUS_CLAMPED_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_CONTRAST_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_INVERT_OVG_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_RED_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_GREEN_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_BLUE_EXT;
BG2E_EXPORT extern const uint32_t BLEND_OP_BEGIN_RANGE;
BG2E_EXPORT extern const uint32_t BLEND_OP_END_RANGE;
BG2E_EXPORT extern const uint32_t BLEND_OP_RANGE_SIZE;
BG2E_EXPORT extern const uint32_t BLEND_OP_MAX_ENUM;
typedef Flags BlendOp;

BG2E_EXPORT extern const uint32_t DYNAMIC_STATE_VIEWPORT;
BG2E_EXPORT extern const uint32_t DYNAMIC_STATE_SCISSOR;
BG2E_EXPORT extern const uint32_t DYNAMIC_STATE_LINE_WIDTH;
BG2E_EXPORT extern const uint32_t DYNAMIC_STATE_DEPTH_BIAS;
BG2E_EXPORT extern const uint32_t DYNAMIC_STATE_BLEND_CONSTANTS;
BG2E_EXPORT extern const uint32_t DYNAMIC_STATE_DEPTH_BOUNDS;
BG2E_EXPORT extern const uint32_t DYNAMIC_STATE_STENCIL_COMPARE_MASK;
BG2E_EXPORT extern const uint32_t DYNAMIC_STATE_STENCIL_WRITE_MASK;
BG2E_EXPORT extern const uint32_t DYNAMIC_STATE_STENCIL_REFERENCE;
BG2E_EXPORT extern const uint32_t DYNAMIC_STATE_VIEWPORT_W_SCALING_NV;
BG2E_EXPORT extern const uint32_t DYNAMIC_STATE_DISCARD_RECTANGLE_EXT;
BG2E_EXPORT extern const uint32_t DYNAMIC_STATE_SAMPLE_LOCATIONS_EXT;
BG2E_EXPORT extern const uint32_t DYNAMIC_STATE_BEGIN_RANGE;
BG2E_EXPORT extern const uint32_t DYNAMIC_STATE_END_RANGE;
BG2E_EXPORT extern const uint32_t DYNAMIC_STATE_RANGE_SIZE;
BG2E_EXPORT extern const uint32_t DYNAMIC_STATE_MAX_ENUM;
typedef Flags DynamicState;

BG2E_EXPORT extern const uint32_t CULL_MODE_NONE;
BG2E_EXPORT extern const uint32_t CULL_MODE_FRONT_BIT;
BG2E_EXPORT extern const uint32_t CULL_MODE_BACK_BIT;
BG2E_EXPORT extern const uint32_t CULL_MODE_FRONT_AND_BACK;
BG2E_EXPORT extern const uint32_t CULL_MODE_FLAG_BITS_MAX_ENUM;
typedef uint32_t CullModeFlags;
typedef Flags VkCullModeFlagBits;
typedef Flags PipelineMultisampleStateCreateFlags;
typedef Flags PipelineDepthStencilStateCreateFlags;
typedef Flags PipelineColorBlendStateCreateFlags;

BG2E_EXPORT extern const uint32_t COLOR_COMPONENT_R_BIT;
BG2E_EXPORT extern const uint32_t COLOR_COMPONENT_G_BIT;
BG2E_EXPORT extern const uint32_t COLOR_COMPONENT_B_BIT;
BG2E_EXPORT extern const uint32_t COLOR_COMPONENT_A_BIT;
BG2E_EXPORT extern const uint32_t COLOR_COMPONENT_FLAG_BITS_MAX_ENUM;
typedef Flags ColorComponentFlags;
typedef Flags PipelineDynamicStateCreateFlags;
typedef Flags PipelineLayoutCreateFlags;
typedef Flags ShaderStageFlags;
typedef Flags SamplerCreateFlags;

typedef struct PipelineColorBlendAttachmentState {
    bool                   blendEnable = false;
    BlendFactor            srcColorBlendFactor = BLEND_FACTOR_ZERO;
    BlendFactor            dstColorBlendFactor = BLEND_FACTOR_ZERO;
    BlendOp                colorBlendOp = BLEND_OP_ADD;
    BlendFactor            srcAlphaBlendFactor = BLEND_FACTOR_ZERO;
    BlendFactor            dstAlphaBlendFactor = BLEND_FACTOR_ZERO;
    BlendOp                alphaBlendOp = BLEND_OP_ADD;
    ColorComponentFlags    colorWriteMask = COLOR_COMPONENT_R_BIT | COLOR_COMPONENT_G_BIT | COLOR_COMPONENT_B_BIT | COLOR_COMPONENT_A_BIT;
    
    inline void operator = (const PipelineColorBlendAttachmentState & other) {
        blendEnable = other.blendEnable;
        srcColorBlendFactor = other.srcColorBlendFactor;
        dstColorBlendFactor = other.dstColorBlendFactor;
        colorBlendOp = other.colorBlendOp;
        srcAlphaBlendFactor = other.srcAlphaBlendFactor;
        dstAlphaBlendFactor = other.dstAlphaBlendFactor;
        alphaBlendOp = other.alphaBlendOp;
        colorWriteMask = other.colorWriteMask;
    }
} PipelineColorBlendAttachmentState;

BG2E_EXPORT extern const uint32_t COLOR_COMPONENT_R_BIT;
BG2E_EXPORT extern const uint32_t COLOR_COMPONENT_G_BIT;
BG2E_EXPORT extern const uint32_t COLOR_COMPONENT_B_BIT;
BG2E_EXPORT extern const uint32_t COLOR_COMPONENT_A_BIT;
BG2E_EXPORT extern const uint32_t COLOR_COMPONENT_FLAG_BITS_MAX_ENUM;
typedef Flags ColorComponentFlags;
typedef Flags PipelineDynamicStateCreateFlags;
typedef Flags PipelineLayoutCreateFlags;
typedef Flags ShaderStageFlags;
typedef Flags SamplerCreateFlags;


BG2E_EXPORT extern const uint32_t DESCRIPTOR_TYPE_SAMPLER;
BG2E_EXPORT extern const uint32_t DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
BG2E_EXPORT extern const uint32_t DESCRIPTOR_TYPE_SAMPLED_IMAGE;
BG2E_EXPORT extern const uint32_t DESCRIPTOR_TYPE_STORAGE_IMAGE;
BG2E_EXPORT extern const uint32_t DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
BG2E_EXPORT extern const uint32_t DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
BG2E_EXPORT extern const uint32_t DESCRIPTOR_TYPE_UNIFORM_BUFFER;
BG2E_EXPORT extern const uint32_t DESCRIPTOR_TYPE_STORAGE_BUFFER;
BG2E_EXPORT extern const uint32_t DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
BG2E_EXPORT extern const uint32_t DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
BG2E_EXPORT extern const uint32_t DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
BG2E_EXPORT extern const uint32_t DESCRIPTOR_TYPE_BEGIN_RANGE;
BG2E_EXPORT extern const uint32_t DESCRIPTOR_TYPE_END_RANGE;
BG2E_EXPORT extern const uint32_t DESCRIPTOR_TYPE_RANGE_SIZE;
BG2E_EXPORT extern const uint32_t DESCRIPTOR_TYPE_MAX_ENUM;
typedef Flags DescriptorType;

typedef struct PushConstantRange {
    ShaderStageFlags    stageFlags;
    uint32_t              offset;
    uint32_t              size;
    
    inline void operator = (const PushConstantRange & other) {
        stageFlags = other.stageFlags;
        offset = other.offset;
        size = other.size;
    }
} PushConstantRange;

class BG2E_EXPORT Sampler : public bg::base::ReferencedPointer {
public:

	struct SamplerCreateInfo {
		Filter                magFilter;
		Filter                minFilter;
		SamplerMipmapMode     mipmapMode;
		SamplerAddressMode    addressModeU;
		SamplerAddressMode    addressModeV;
		SamplerAddressMode    addressModeW;
		float                 mipLodBias;
		bool                  anisotropyEnable;
		float                 maxAnisotropy;
		bool                  compareEnable;
		CompareOp             compareOp;
		float                 minLod;
		float                 maxLod;
		BorderColor           borderColor;
		bool                  unnormalizedCoordinates;
  
        inline void operator = (const SamplerCreateInfo & other) {
            magFilter = other.magFilter;
            minFilter = other.minFilter;
            mipmapMode = other.mipmapMode;
            addressModeU = other.addressModeU;
            addressModeV = other.addressModeV;
            addressModeW = other.addressModeW;
            mipLodBias = other.mipLodBias;
            anisotropyEnable = other.anisotropyEnable;
            maxAnisotropy = other.maxAnisotropy;
            compareEnable = other.compareEnable;
            compareOp = other.compareOp;
            minLod = other.minLod;
            maxLod = other.maxLod;
            borderColor = other.borderColor;
            unnormalizedCoordinates = other.unnormalizedCoordinates;
        }
	} createInfo;

    void create(Device *);
    
    inline bg::plain_ptr vkSampler() { return _vk_sampler; }
    
protected:
    virtual ~Sampler();
    
    bg::plain_ptr _vk_sampler;
    bg::plain_ptr _vk_device;
};

typedef struct DescriptorPoolSize {
	DescriptorType    type;
	uint32_t          descriptorCount;

	DescriptorPoolSize(DescriptorType t, uint32_t c) :type(t), descriptorCount(c) {}

	inline void operator = (const DescriptorPoolSize & s) {
		type = s.type;
		descriptorCount = s.descriptorCount;
	}
} DescriptorPoolSize;

class BG2E_EXPORT DescriptorPool : public bg::base::ReferencedPointer {
public:
	struct DescriptorPoolCreateInfo {
		uint32_t                       maxSets;
		std::vector<DescriptorPoolSize> poolSizes;
	} createInfo;

	void create(Device *);

	bg::plain_ptr vkDescriptorPool() { return _vk_descriptorPool; }

protected:
	virtual ~DescriptorPool();

	bg::plain_ptr _vk_descriptorPool = nullptr;
	bg::plain_ptr _vk_device = nullptr;
};

typedef struct DescriptorSetLayoutBinding {
    uint32_t              binding;
    DescriptorType      descriptorType;
    uint32_t              descriptorCount;
    ShaderStageFlags    stageFlags;
    const Sampler *      pImmutableSamplers = nullptr;
    
    inline void operator = (const DescriptorSetLayoutBinding & other) {
        binding = other.binding;
        descriptorType = other.descriptorType;
        descriptorCount = other.descriptorCount;
        stageFlags = other.stageFlags;
    }
} DescriptorSetLayoutBinding;

class BG2E_EXPORT DescriptorSetLayout : public bg::base::ReferencedPointer {
public:
    struct DescriptorSetLayoutCreateInfo {
        std::vector<DescriptorSetLayoutBinding> bindings;
    } createInfo;
    
    void create(Device *);
    
    inline bg::plain_ptr vkDescriptorSetLayout() { return _vk_descriptorSetLayout; }
    
protected:
    virtual ~DescriptorSetLayout();
    
    bg::plain_ptr _vk_descriptorSetLayout = nullptr;
    bg::plain_ptr _vk_device = nullptr;
};

class BG2E_EXPORT BufferView : public bg::base::ReferencedPointer {
public:

	inline bg::plain_ptr vkBufferView() { return _vk_bufferView; }

protected:
	virtual ~BufferView();

	bg::plain_ptr _vk_bufferView = nullptr;
};

class Buffer;
class DescriptorSet;
typedef struct DescriptorBufferInfo {
	bg::ptr<Buffer> buffer;
	uint32_t offset = 0;
	uint32_t range = 0;
} DescriptorBufferInfo;

typedef struct DescriptorImageInfo {
	bg::ptr<Sampler>        sampler;
	bg::ptr<ImageView>      imageView;
	ImageLayout				imageLayout = 0;
} DescriptorImageInfo;

typedef struct WriteDescriptorSet {
	bg::ptr<DescriptorSet> dstSet;
	uint32_t dstBinding = 0;
	uint32_t dstArrayElement = 0;
	DescriptorType descriptorType = 0;
	uint32_t descriptorCount = 0;
	const DescriptorBufferInfo * pBufferInfo = nullptr;
	const DescriptorImageInfo * pImageInfo = nullptr;
	bg::ptr<BufferView> pTexelBufferView;
} WriteDescriptorSet;

class BG2E_EXPORT DescriptorSet : public bg::base::ReferencedPointer {
public:

	struct DescriptorSetAllocateInfo {
		bg::ptr<DescriptorPool>         descriptorPool;
		std::vector<bg::ptr<DescriptorSetLayout>> setLayouts;
	} allocateInfo;

	void allocateDescriptorSets(Device *);

	void updateDescriptorSets(const std::vector<WriteDescriptorSet> & wds);

	inline bg::plain_ptr vkDescriptorSet() { return _vk_descriptorSet; }

protected:
	virtual ~DescriptorSet();

	bg::plain_ptr _vk_descriptorSet = nullptr;
	bg::plain_ptr _vk_device = nullptr;
};

typedef struct AttachmentDescription {
    AttachmentDescriptionFlags    flags = 0;
    Format                        format;
    SampleCountFlagBits           samples;
    AttachmentLoadOp              loadOp;
    AttachmentStoreOp             storeOp;
    AttachmentLoadOp              stencilLoadOp;
    AttachmentStoreOp             stencilStoreOp;
    ImageLayout                   initialLayout;
    ImageLayout                   finalLayout;
    
    inline void operator = (const AttachmentDescription & other) {
        flags = other.flags;
        format = other.format;
        samples = other.samples;
        loadOp = other.loadOp;
        storeOp = other.storeOp;
        stencilLoadOp = other.stencilLoadOp;
        stencilStoreOp = other.stencilStoreOp;
        initialLayout = other.initialLayout;
        finalLayout = other.finalLayout;
    }
} AttachmentDescription;

typedef struct AttachmentReference {
    uint32_t         attachment;
    ImageLayout      layout;
    
    inline void operator = (const AttachmentReference & other) {
        attachment = other.attachment;
        layout = other.layout;
    }
} AttachmentReference;

typedef struct SubpassDescription {
    SubpassDescriptionFlags       flags = 0;
    PipelineBindPoint             pipelineBindPoint;
    std::vector<AttachmentReference> inputAttachments;
    std::vector<AttachmentReference> colorAttachments;
    std::vector<AttachmentReference> resolveAttachments;
    AttachmentReference depthStencilAttachment;
	bool useDepthStencilAttachment = false;
    std::vector<uint32_t> reserveAttachments;
    
    inline void operator =(const SubpassDescription & other) {
        flags = other.flags;
        pipelineBindPoint = other.pipelineBindPoint;
        inputAttachments = other.inputAttachments;
        colorAttachments = other.colorAttachments;
        resolveAttachments = other.resolveAttachments;
        depthStencilAttachment = other.depthStencilAttachment;
        reserveAttachments = other.reserveAttachments;
    }
} SubpassDescription;

typedef struct SubpassDependency {
    uint32_t                srcSubpass = 0;
    uint32_t                dstSubpass = 0;
    PipelineStageFlags    srcStageMask = 0;
    PipelineStageFlags    dstStageMask = 0;
    AccessFlags           srcAccessMask = 0;
    AccessFlags           dstAccessMask = 0;
    DependencyFlags       dependencyFlags = 0;
    
    inline void operator =(const SubpassDependency & other) {
        srcSubpass = other.srcSubpass;
        dstSubpass = other.dstSubpass;
        srcStageMask = other.srcStageMask;
        dstStageMask = other.dstStageMask;
        srcAccessMask = other.srcAccessMask;
        dstAccessMask = other.dstAccessMask;
        dependencyFlags = other.dependencyFlags;
    }
    
    SubpassDependency() {}
    SubpassDependency(
        uint32_t              ss,
        uint32_t              ds,
        PipelineStageFlags    ssm,
        PipelineStageFlags    dsm,
        AccessFlags           sam,
        AccessFlags           dam,
        DependencyFlags       df
    )
        :srcSubpass(ss)
        ,dstSubpass(ds)
        ,srcStageMask(ssm)
        ,dstStageMask(dsm)
        ,srcAccessMask(sam)
        ,dstAccessMask(dam)
        ,dependencyFlags(df)
    {
    }
} SubpassDependency;

class BG2E_EXPORT PipelineLayout : public bg::base::ReferencedPointer {
public:

    struct PipelineLayoutCreateInfo {
        std::vector<bg::ptr<DescriptorSetLayout>> setLayouts;
        std::vector<PushConstantRange> pushConstantRanges;
    } createInfo;
    
    void create(Device *);

    inline bg::plain_ptr vkPipelineLayout() { return _vk_pipelineLayout; }

protected:
    virtual ~PipelineLayout();
    
    bg::plain_ptr _vk_pipelineLayout;
    bg::plain_ptr _vk_device;
};

class BG2E_EXPORT RenderPass : public bg::base::ReferencedPointer {
public:
    struct RenderPassCreateInfo {
        std::vector<AttachmentDescription> attachments;
        std::vector<SubpassDescription> subpasses;
        std::vector<SubpassDependency> dependencies;
    } createInfo;
    
    void create(Device * dev);
    
    inline bg::plain_ptr vkRenderPass() { return _vk_renderPass; }
    
protected:
    virtual ~RenderPass();
    
    bg::plain_ptr _vk_renderPass;
    bg::plain_ptr _vk_device;
};

typedef struct StencilOpState {
    StencilOp    failOp;
    StencilOp    passOp;
    StencilOp    depthFailOp;
    CompareOp    compareOp;
    uint32_t     compareMask;
    uint32_t     writeMask;
    uint32_t     reference;
    
    inline void operator = (const StencilOpState & other) {
        failOp = other.failOp;
        passOp = other.passOp;
        depthFailOp = other.depthFailOp;
        compareOp = other.compareOp;
        compareMask = other.compareMask;
        writeMask = other.writeMask;
        reference = other.reference;
    }
} StencilOpState;

class BG2E_EXPORT Pipeline : public bg::base::ReferencedPointer {
public:

    struct ShaderStageCreateInfo {
        bg::ptr<ShaderModule> module;
        std::string name = "main";
        ShaderStageFlagBits stage;
        const SpecializationInfo * pSpecializationInfo = nullptr;
        
        ShaderStageCreateInfo(ShaderModule * m, const std::string & n, ShaderStageFlagBits s) :module(m), name(n), stage(s) {}
        ShaderStageCreateInfo(ShaderModule * m, const std::string & n, ShaderStageFlagBits s, const SpecializationInfo * pS) :module(m), name(n), stage(s), pSpecializationInfo(pS) {}
    };
    std::vector<ShaderStageCreateInfo> shaderStages;
    
    struct VertexInputStateCreateInfo {
        std::vector<VertexInputBindingDescription>    vertexBindingDescriptions;
        std::vector<VertexInputAttributeDescription>  vertexAttributeDescriptions;
    } vertexInputState;
    
    struct InputAssemblyStateCreateInfo {
        PrimitiveTopology       topology;
        bool                    primitiveRestartEnable;
    };
    InputAssemblyStateCreateInfo inputAssemblyState;
    
    struct ViewportStateCreateInfo {
        std::vector<Viewport> viewports;
        std::vector<Rect2D> scissors;
    } viewportState;
    
    struct RasterizationStateCreateInfo {
        bool            depthClampEnable = false;
        bool            rasterizerDiscardEnable = false;
        PolygonMode     polygonMode = POLYGON_MODE_FILL;
        CullModeFlags   cullMode = CULL_MODE_BACK_BIT;
        FrontFace       frontFace = FRONT_FACE_CLOCKWISE;
        bool            depthBiasEnable = false;
        float           depthBiasConstantFactor = 0.0f;
        float           depthBiasClamp = 0.0f;
        float           depthBiasSlopeFactor = 0.0f;
        float           lineWidth = 1.0f;
    } rasterizationState;
    
    struct PipelineMultisampleStateCreateInfo {
        SampleCountFlagBits   rasterizationSamples = SAMPLE_COUNT_1_BIT;
        bool                  sampleShadingEnable = false;
        float                 minSampleShading = 0.0f;
        const SampleMask*     pSampleMask = nullptr;
        bool                  alphaToCoverageEnable = false;
        bool                  alphaToOneEnable = false;
    } multisampleState;

    struct PipelineDepthStencilStateCreateInfo {
        bool                depthTestEnable = false;
        bool                depthWriteEnable = false;
        CompareOp           depthCompareOp = COMPARE_OP_LESS;
        bool                depthBoundsTestEnable = false;
        bool                stencilTestEnable = false;
        StencilOpState      front;
        StencilOpState      back;
        float               minDepthBounds = 0.0f;
        float               maxDepthBounds = 1.0f;
    } depthStencilState;

    struct PipelineColorBlendStateCreateInfo {
        bool                                            logicOpEnable = false;
        LogicOp                                         logicOp = LOGIC_OP_COPY;
        std::vector<PipelineColorBlendAttachmentState>  attachments;
        float                                           blendConstants[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    } colorBlendState;

    struct PipelineDynamicStateCreateInfo {
        std::vector<DynamicState> dynamicStates;
    } dynamicState;

    void create(Device *);

    inline void setPipelineLayout(PipelineLayout * pipelineLayout) { _pipelineLayout = pipelineLayout; }
    inline PipelineLayout * pipelineLayout() { return _pipelineLayout.getPtr(); }
    
    inline void setRenderPass(RenderPass * rp, uint32_t subpass = 0) { _renderPass = rp; _subpass = subpass; }
    inline RenderPass * renderPass() { return _renderPass.getPtr(); }
    inline uint32_t renderPassSubpass() const { return _subpass; }
    
    inline bg::plain_ptr vkPipeline() { return _vk_pipeline; }
    
protected:
    virtual ~Pipeline();
    
    bg::ptr<PipelineLayout> _pipelineLayout;
    bg::ptr<RenderPass> _renderPass;
    uint32_t _subpass;
    
    bg::plain_ptr _vk_pipeline = nullptr;
    bg::plain_ptr _vk_device = nullptr;
};

BG2E_EXPORT extern const uint32_t BUFFER_USAGE_TRANSFER_SRC_BIT;
BG2E_EXPORT extern const uint32_t BUFFER_USAGE_TRANSFER_DST_BIT;
BG2E_EXPORT extern const uint32_t BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT;
BG2E_EXPORT extern const uint32_t BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT;
BG2E_EXPORT extern const uint32_t BUFFER_USAGE_UNIFORM_BUFFER_BIT;
BG2E_EXPORT extern const uint32_t BUFFER_USAGE_STORAGE_BUFFER_BIT;
BG2E_EXPORT extern const uint32_t BUFFER_USAGE_INDEX_BUFFER_BIT;
BG2E_EXPORT extern const uint32_t BUFFER_USAGE_VERTEX_BUFFER_BIT;
BG2E_EXPORT extern const uint32_t BUFFER_USAGE_INDIRECT_BUFFER_BIT;
BG2E_EXPORT extern const uint32_t BUFFER_USAGE_FLAG_BITS_MAX_ENUM;
typedef uint32_t BufferUsageFlagBits;
typedef Flags BufferUsageFlags;
typedef Flags BufferViewCreateFlags;
typedef Flags ImageViewCreateFlags;
typedef Flags ShaderModuleCreateFlags;
typedef Flags PipelineCacheCreateFlags;

class BG2E_EXPORT Buffer : public bg::base::ReferencedPointer {
public:
    struct BufferCreateInfo {
        DeviceSize           size;
        BufferUsageFlags     usage;
        SharingMode          sharingMode;
        std::vector<uint32_t> queueFamilyIndices;
    } createInfo;
    
    void create(Device *);
    
    inline bg::plain_ptr vkBuffer() { return _vk_buffer; }
    
    inline MemoryRequirements getBufferMemoryRequirements() { MemoryRequirements m; getBufferMemoryRequirements(m); return m; }
    void getBufferMemoryRequirements(MemoryRequirements &);
    
protected:
    virtual ~Buffer();
    
    bg::plain_ptr _vk_buffer = nullptr;
    bg::plain_ptr _vk_device = nullptr;
};

class BG2E_EXPORT DeviceMemory : public bg::base::ReferencedPointer {
public:

    DeviceMemory();
    
    struct MemoryAllocateInfo {
        DeviceSize allocationSize;
        uint32_t memoryTypeIndex;
    } allocateInfo;

    void allocateMemory(Device *);

    inline bg::plain_ptr vkDeviceMemory() { return _vk_deviceMemory; }
    
    void bindBufferMemory(Buffer *, DeviceSize offset);
    void bindImageMemory(Image *, DeviceSize offset);
    void mapMemory(DeviceSize offset, DeviceSize size, MemoryMapFlags flags, void ** pData);
    void unmapMemory();
    
protected:
    virtual ~DeviceMemory();
    
    bg::plain_ptr _vk_deviceMemory = nullptr;
    bg::plain_ptr _vk_device = nullptr;
};

class BG2E_EXPORT Framebuffer : public bg::base::ReferencedPointer {
public:

    struct FramebufferCreateInfo {
        bg::ptr<RenderPass>              renderPass;
        std::vector<bg::ptr<ImageView>>  attachments;
        uint32_t                         width;
        uint32_t                         height;
        uint32_t                         layers;
    } createInfo;
    
    void create(Device * dev);
    
    inline bg::plain_ptr vkFramebuffer() { return _vk_framebuffer; }
    
protected:
    virtual ~Framebuffer();
    
    bg::plain_ptr _vk_framebuffer = nullptr;
    bg::plain_ptr _vk_device = nullptr;
};

BG2E_EXPORT extern const uint32_t COMMAND_POOL_CREATE_TRANSIENT_BIT;
BG2E_EXPORT extern const uint32_t COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
BG2E_EXPORT extern const uint32_t COMMAND_POOL_CREATE_PROTECTED_BIT;
BG2E_EXPORT extern const uint32_t COMMAND_POOL_CREATE_FLAG_BITS_MAX_ENUM;
typedef uint32_t CommandPoolCreateFlagBits;
typedef Flags CommandPoolCreateFlags;

BG2E_EXPORT extern const uint32_t COMMAND_BUFFER_LEVEL_PRIMARY;
BG2E_EXPORT extern const uint32_t COMMAND_BUFFER_LEVEL_SECONDARY;
BG2E_EXPORT extern const uint32_t COMMAND_BUFFER_LEVEL_BEGIN_RANGE;
BG2E_EXPORT extern const uint32_t COMMAND_BUFFER_LEVEL_END_RANGE;
BG2E_EXPORT extern const uint32_t COMMAND_BUFFER_LEVEL_RANGE_SIZE;
BG2E_EXPORT extern const uint32_t COMMAND_BUFFER_LEVEL_MAX_ENUM;
typedef Flags CommandBufferLevel;


class BG2E_EXPORT CommandPool : public bg::base::ReferencedPointer {
public:
    struct CommandPoolCreateInfo {
        CommandPoolCreateFlags    flags;
        uint32_t                    queueFamilyIndex;
    } createInfo;
    
    void create(Device *);
    
    inline bg::plain_ptr vkCommandPool() { return _vk_commandPool; }

protected:
    virtual ~CommandPool();
    
    bg::plain_ptr _vk_commandPool = nullptr;
    bg::plain_ptr _vk_device = nullptr;
};

BG2E_EXPORT extern const uint32_t COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
BG2E_EXPORT extern const uint32_t COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
BG2E_EXPORT extern const uint32_t COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
BG2E_EXPORT extern const uint32_t COMMAND_BUFFER_USAGE_FLAG_BITS_MAX_ENUM;
typedef uint32_t CommandBufferUsageFlagBits;
typedef Flags CommandBufferUsageFlags;

BG2E_EXPORT extern const uint32_t QUERY_CONTROL_PRECISE_BIT;
BG2E_EXPORT extern const uint32_t QUERY_CONTROL_FLAG_BITS_MAX_ENUM;
typedef uint32_t QueryControlFlagBits;
typedef Flags QueryControlFlags;

BG2E_EXPORT extern const uint32_t QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_VERTICES_BIT;
BG2E_EXPORT extern const uint32_t QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_PRIMITIVES_BIT;
BG2E_EXPORT extern const uint32_t QUERY_PIPELINE_STATISTIC_VERTEX_SHADER_INVOCATIONS_BIT;
BG2E_EXPORT extern const uint32_t QUERY_PIPELINE_STATISTIC_GEOMETRY_SHADER_INVOCATIONS_BIT;
BG2E_EXPORT extern const uint32_t QUERY_PIPELINE_STATISTIC_GEOMETRY_SHADER_PRIMITIVES_BIT;
BG2E_EXPORT extern const uint32_t QUERY_PIPELINE_STATISTIC_CLIPPING_INVOCATIONS_BIT;
BG2E_EXPORT extern const uint32_t QUERY_PIPELINE_STATISTIC_CLIPPING_PRIMITIVES_BIT;
BG2E_EXPORT extern const uint32_t QUERY_PIPELINE_STATISTIC_FRAGMENT_SHADER_INVOCATIONS_BIT;
BG2E_EXPORT extern const uint32_t QUERY_PIPELINE_STATISTIC_TESSELLATION_CONTROL_SHADER_PATCHES_BIT;
BG2E_EXPORT extern const uint32_t QUERY_PIPELINE_STATISTIC_TESSELLATION_EVALUATION_SHADER_INVOCATIONS_BIT;
BG2E_EXPORT extern const uint32_t QUERY_PIPELINE_STATISTIC_COMPUTE_SHADER_INVOCATIONS_BIT;
BG2E_EXPORT extern const uint32_t QUERY_PIPELINE_STATISTIC_FLAG_BITS_MAX_ENUM;
typedef uint32_t QueryPipelineStatisticFlagBits;
typedef Flags QueryPipelineStatisticFlags;

BG2E_EXPORT extern const uint32_t SUBPASS_CONTENTS_INLINE;
BG2E_EXPORT extern const uint32_t SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS;
BG2E_EXPORT extern const uint32_t SUBPASS_CONTENTS_BEGIN_RANGE;
BG2E_EXPORT extern const uint32_t SUBPASS_CONTENTS_END_RANGE;
BG2E_EXPORT extern const uint32_t SUBPASS_CONTENTS_RANGE_SIZE;
BG2E_EXPORT extern const uint32_t SUBPASS_CONTENTS_MAX_ENUM;
typedef Flags SubpassContents;


BG2E_EXPORT extern const uint32_t COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT;
BG2E_EXPORT extern const uint32_t COMMAND_BUFFER_RESET_FLAG_BITS_MAX_ENUM;
typedef uint32_t CommandBufferResetFlagBits;
typedef Flags CommandBufferResetFlags;


BG2E_EXPORT extern const uint32_t INDEX_TYPE_UINT16;
BG2E_EXPORT extern const uint32_t INDEX_TYPE_UINT32;
BG2E_EXPORT extern const uint32_t INDEX_TYPE_BEGIN_RANGE;
BG2E_EXPORT extern const uint32_t INDEX_TYPE_END_RANGE;
BG2E_EXPORT extern const uint32_t INDEX_TYPE_RANGE_SIZE;
BG2E_EXPORT extern const uint32_t INDEX_TYPE_MAX_ENUM;
typedef Flags IndexType;

typedef struct CommandBufferInheritanceInfo {
    RenderPass *                     renderPass = nullptr;
    uint32_t                         subpass = 0;
    Framebuffer *                    framebuffer = nullptr;
    bool                             occlusionQueryEnable = false;
    QueryControlFlags                queryFlags = 0;
    QueryPipelineStatisticFlags      pipelineStatistics = 0;
} CommandBufferInheritanceInfo;

typedef struct MemoryBarrier {
    AccessFlags      srcAccessMask = 0;
    AccessFlags      dstAccessMask = 0;
} MemoryBarrier;

typedef struct BufferMemoryBarrier {
    AccessFlags      srcAccessMask = 0;
    AccessFlags      dstAccessMask = 0;
    uint32_t         srcQueueFamilyIndex = 0;
    uint32_t         dstQueueFamilyIndex = 0;
    Buffer *         buffer = nullptr;
    DeviceSize       offset = 0;
    DeviceSize       size = 0;
} BufferMemoryBarrier;

typedef struct ImageMemoryBarrier {
    AccessFlags              srcAccessMask = 0;
    AccessFlags              dstAccessMask = 0;
    ImageLayout              oldLayout = 0;
    ImageLayout              newLayout = 0;
    uint32_t                 srcQueueFamilyIndex = 0;
    uint32_t                 dstQueueFamilyIndex = 0;
    Image *                  image = nullptr;
    ImageSubresourceRange    subresourceRange;
} ImageMemoryBarrier;

typedef struct ImageSubresourceLayers {
    ImageAspectFlags      aspectMask;
    uint32_t              mipLevel;
    uint32_t              baseArrayLayer;
    uint32_t              layerCount;
} ImageSubresourceLayers;

typedef struct BufferImageCopy {
    DeviceSize                bufferOffset;
    uint32_t                    bufferRowLength;
    uint32_t                    bufferImageHeight;
    ImageSubresourceLayers    imageSubresource;
    Offset3D                  imageOffset;
    Extent3D                  imageExtent;
} BufferImageCopy;

typedef union ClearColorValue {
    float       float32[4];
    int32_t     int32[4];
    uint32_t    uint32[4];
} ClearColorValue;

typedef struct ClearDepthStencilValue {
    float       depth;
    uint32_t    stencil;
} ClearDepthStencilValue;

typedef union ClearValue {
    ClearColorValue           color;
    ClearDepthStencilValue    depthStencil;
} ClearValue;

class BG2E_EXPORT CommandBuffer : public bg::base::ReferencedPointer {
public:
    
    static void Allocate(
        Device *,
        CommandPool *,
        CommandBufferLevel level,
        uint32_t commandBufferCount,
        std::vector<bg::ptr<CommandBuffer>> & result);
    
    static void Allocate(
        Device *,
        CommandPool *,
        CommandBufferLevel level,
        bg::ptr<CommandBuffer> & result);
    
    inline bg::plain_ptr vkCommandBuffer() { return _vk_commandBuffer; }
    
    void beginCommandBuffer(CommandBufferUsageFlags flags, CommandBufferInheritanceInfo * pInheritanceInfo = nullptr);
	bool endCommandBuffer();
    void resetCommandBuffer(CommandBufferResetFlags flags);

	void cmdBeginRenderPass(RenderPass * rp, Framebuffer * fb, const Rect2D & area, const std::vector<vk::ClearValue> & clearValues, SubpassContents contents = SUBPASS_CONTENTS_INLINE);
	void cmdBindPipeline(PipelineBindPoint bindPoint, Pipeline * pipeline);
	void cmdDraw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);
    void cmdDrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, uint32_t vertexOffset, uint32_t firstInstance);
	void cmdEndRenderPass();
	void cmdBindVertexBuffers(uint32_t firstBinding, uint32_t bindingCount, const std::vector<Buffer*> & buffers, const std::vector<DeviceSize> & offsets);
    void cmdCopyBuffer(vk::DeviceSize size, vk::Buffer * src, vk::Buffer * dst);
    // TODO: Implement copyBuffer with regions
    void cmdBindIndexBuffer(vk::Buffer * buffer, vk::DeviceSize offset, vk::IndexType indexType);
	void cmdBindDescriptorSets(PipelineBindPoint pipelineBindPoint, PipelineLayout * pipelineLayout, uint32_t firstSet, const std::vector<DescriptorSet*> & descSet, std::vector<uint32_t> dynamicOffsets);
    void cmdPipelineBarrier(PipelineStageFlags srcStageMask, PipelineStageFlags dstStageMask, DependencyFlags depFlags, const std::vector<MemoryBarrier> & memoryBarriers, const std::vector<BufferMemoryBarrier> & bufferMemoryBarriers, const std::vector<ImageMemoryBarrier> & imgMemoryBarriers);
    void cmdCopyBufferToImage(vk::Buffer * buffer, vk::Image * image, ImageLayout dstImgLayout, const std::vector<BufferImageCopy> & regions);

protected:
    CommandBuffer();
    virtual ~CommandBuffer();
    
    bg::plain_ptr _vk_commandBuffer = nullptr;
};

class BG2E_EXPORT Semaphore : public bg::base::ReferencedPointer {
public:
	Semaphore() {}
	Semaphore(Device * dev) { create(dev); }

	void create(Device *);

	inline bg::plain_ptr vkSemaphore() { return _vk_semaphore; }

protected:
	virtual ~Semaphore();

	bg::plain_ptr _vk_semaphore = nullptr;
	bg::plain_ptr _vk_device = nullptr;
};

class BG2E_EXPORT Fence : public bg::base::ReferencedPointer {
public:
	Fence();
	Fence(Device * dev);

	void create(Device * dev);

	bg::plain_ptr vkFence() { return _vk_fence; }

protected:
	virtual ~Fence();

	bg::plain_ptr _vk_fence = nullptr;
	bg::plain_ptr _vk_device = nullptr;
};



}
}
}
}

#endif
