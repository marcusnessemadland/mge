/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */


#pragma once

#include "engine/math.h"

#include <bgfx/bgfx.h>
#include <bimg/bimg.h>
#include <bx/math.h>
#include <bx/readerwriter.h>

namespace vr
{
	bx::Vec3 toBgfxVec(const Vec3& _v);
	bx::Quaternion toBgfxQuat(const Quat& _q);
}

namespace bx
{
	void mtxAdjugate(float* _result, const float* _a);
}

namespace bgfx
{
	void initBgfxUtils();
	void shutdownBgfxUtils();

	bgfx::TextureHandle loadTexture(const char* _filePath, uint64_t _flags = BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE, bgfx::TextureInfo* _info = nullptr, bimg::Orientation::Enum* _orientation = nullptr);
}