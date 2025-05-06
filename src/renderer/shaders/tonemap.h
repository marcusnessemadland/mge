#pragma once

#include "generated/glsl/vs_tonemap.sc.bin.h"
#include "generated/essl/vs_tonemap.sc.bin.h"
#include "generated/spirv/vs_tonemap.sc.bin.h"
#include "generated/glsl/fs_tonemap.sc.bin.h"
#include "generated/essl/fs_tonemap.sc.bin.h"
#include "generated/spirv/fs_tonemap.sc.bin.h"
#if defined(_WIN32)
#include "generated/dx11/vs_tonemap.sc.bin.h"
#include "generated/dx11/fs_tonemap.sc.bin.h"
#endif //  defined(_WIN32)
#if __APPLE__
#include "generated/mtl/vs_tonemap.sc.bin.h"
#include "generated/mtl/fs_tonemap.sc.bin.h"
#endif // __APPLE__