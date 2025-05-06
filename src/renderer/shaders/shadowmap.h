#pragma once

#include "generated/glsl/vs_shadowmap.sc.bin.h"
#include "generated/essl/vs_shadowmap.sc.bin.h"
#include "generated/spirv/vs_shadowmap.sc.bin.h"
#include "generated/glsl/fs_shadowmap.sc.bin.h"
#include "generated/essl/fs_shadowmap.sc.bin.h"
#include "generated/spirv/fs_shadowmap.sc.bin.h"
#if defined(_WIN32)
#include "generated/dx11/vs_shadowmap.sc.bin.h"
#include "generated/dx11/fs_shadowmap.sc.bin.h"
#endif //  defined(_WIN32)
#if __APPLE__
#include "generated/mtl/vs_shadowmap.sc.bin.h"
#include "generated/mtl/fs_shadowmap.sc.bin.h"
#endif // __APPLE__