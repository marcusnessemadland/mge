#pragma once

#include "shaders/glsl/vs_tonemap.sc.bin.h"
#include "shaders/essl/vs_tonemap.sc.bin.h"
#include "shaders/spirv/vs_tonemap.sc.bin.h"
#include "shaders/glsl/fs_tonemap.sc.bin.h"
#include "shaders/essl/fs_tonemap.sc.bin.h"
#include "shaders/spirv/fs_tonemap.sc.bin.h"
#if defined(_WIN32)
#include "shaders/dx11/vs_tonemap.sc.bin.h"
#include "shaders/dx11/fs_tonemap.sc.bin.h"
#endif //  defined(_WIN32)
#if __APPLE__
#include "shaders/mtl/vs_tonemap.sc.bin.h"
#include "shaders/mtl/fs_tonemap.sc.bin.h"
#endif // __APPLE__