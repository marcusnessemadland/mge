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
#include <optick.h>

namespace vr
{
	struct BgfxCallback : bgfx::CallbackI
	{
		BgfxCallback()
		{
		}

		virtual ~BgfxCallback()
		{
		}

		virtual void fatal(const char* _filePath, uint16_t _line, bgfx::Fatal::Enum _code, const char* _str) override
		{
			BX_UNUSED(_filePath, _line);

			// Something unexpected happened, inform user and bail out.
			bx::debugPrintf("Fatal error: 0x%08x: %s", _code, _str);

			// Must terminate, continuing will cause crash anyway.
			abort();
		}

		virtual void traceVargs(const char* _filePath, uint16_t _line, const char* _format, va_list _argList) override
		{
			bx::debugPrintf("%s (%d): ", _filePath, _line);
			bx::debugPrintfVargs(_format, _argList);
		}

		virtual void profilerBegin(const char* _name, uint32_t _abgr, const char* _filePath, uint16_t _line) override
		{
			BX_UNUSED(_abgr, _filePath, _line);
			OPTICK_PUSH(_name);
		}

		virtual void profilerBeginLiteral(const char* _name, uint32_t _abgr, const char* _filePath, uint16_t _line) override
		{
			BX_UNUSED(_abgr, _filePath, _line);
			OPTICK_PUSH(_name);
		}

		virtual void profilerEnd() override
		{
			OPTICK_POP();
		}

		virtual uint32_t cacheReadSize(uint64_t _id) override
		{
			return 0;
		}

		virtual bool cacheRead(uint64_t _id, void* _data, uint32_t _size) override
		{
			return false;
		}

		virtual void cacheWrite(uint64_t _id, const void* _data, uint32_t _size) override
		{
		}

		virtual void screenShot(const char* _filePath, uint32_t _width, uint32_t _height, uint32_t _pitch, const void* _data, uint32_t /*_size*/, bool _yflip) override
		{
		}

		virtual void captureBegin(uint32_t _width, uint32_t _height, uint32_t /*_pitch*/, bgfx::TextureFormat::Enum /*_format*/, bool _yflip) override
		{
		}

		virtual void captureEnd() override
		{
		}

		virtual void captureFrame(const void* _data, uint32_t /*_size*/) override
		{
		}
	};

	bx::Vec3 toBgfxVec(const Vec3& _v);
	bx::Quaternion toBgfxQuat(const Quat& _q);
}

namespace bx
{
	void mtxAdjugate(float* _result, const float* _a);
	void mtxSRT(float* _result, const vr::Vec3& _pos, const vr::Quat& _rotation, const vr::Vec3& _scale);
}

namespace bgfx
{
	void initBgfxUtils();
	void shutdownBgfxUtils();

	bgfx::TextureHandle loadTexture(const char* _filePath, uint64_t _flags = BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE, bgfx::TextureInfo* _info = nullptr, bimg::Orientation::Enum* _orientation = nullptr);
}