#pragma once

#include "generated/glsl/vs_skybox.sc.bin.h"
#include "generated/essl/vs_skybox.sc.bin.h"
#include "generated/spirv/vs_skybox.sc.bin.h"
#include "generated/glsl/fs_skybox.sc.bin.h"
#include "generated/essl/fs_skybox.sc.bin.h"
#include "generated/spirv/fs_skybox.sc.bin.h"
#if defined(_WIN32)
#include "generated/dx11/vs_skybox.sc.bin.h"
#include "generated/dx11/fs_skybox.sc.bin.h"
#endif //  defined(_WIN32)
#if __APPLE__
#include "generated/mtl/vs_skybox.sc.bin.h"
#include "generated/mtl/fs_skybox.sc.bin.h"
#endif // __APPLE__