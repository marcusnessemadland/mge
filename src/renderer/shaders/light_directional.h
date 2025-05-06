#pragma once

#include "generated/glsl/vs_light_directional.sc.bin.h"
#include "generated/essl/vs_light_directional.sc.bin.h"
#include "generated/spirv/vs_light_directional.sc.bin.h"
#include "generated/glsl/fs_light_directional.sc.bin.h"
#include "generated/essl/fs_light_directional.sc.bin.h"
#include "generated/spirv/fs_light_directional.sc.bin.h"
#if defined(_WIN32)
#include "generated/dx11/vs_light_directional.sc.bin.h"
#include "generated/dx11/fs_light_directional.sc.bin.h"
#endif //  defined(_WIN32)
#if __APPLE__
#include "generated/mtl/vs_light_directional.sc.bin.h"
#include "generated/mtl/fs_light_directional.sc.bin.h"
#endif // __APPLE__