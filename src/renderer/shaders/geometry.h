#pragma once

#include "generated/glsl/vs_geometry.sc.bin.h"
#include "generated/essl/vs_geometry.sc.bin.h"
#include "generated/spirv/vs_geometry.sc.bin.h"
#include "generated/glsl/fs_geometry.sc.bin.h"
#include "generated/essl/fs_geometry.sc.bin.h"
#include "generated/spirv/fs_geometry.sc.bin.h"
#if defined(_WIN32)
#include "generated/dx11/vs_geometry.sc.bin.h"
#include "generated/dx11/fs_geometry.sc.bin.h"
#endif //  defined(_WIN32)
#if __APPLE__
#include "generated/mtl/vs_geometry.sc.bin.h"
#include "generated/mtl/fs_geometry.sc.bin.h"
#endif // __APPLE__