#pragma once

#include "shaders/glsl/vs_deferred.sc.bin.h"
#include "shaders/essl/vs_deferred.sc.bin.h"
#include "shaders/spirv/vs_deferred.sc.bin.h"

#include "shaders/glsl/fs_deferred.sc.bin.h"
#include "shaders/essl/fs_deferred.sc.bin.h"
#include "shaders/spirv/fs_deferred.sc.bin.h"

#if defined(_WIN32)
#include "shaders/dx11/vs_deferred.sc.bin.h"
#include "shaders/dx11/fs_deferred.sc.bin.h"
#endif //  defined(_WIN32)
#if __APPLE__
#include "shaders/mtl/vs_deferred.sc.bin.h"
#include "shaders/mtl/fs_deferred.sc.bin.h"
#endif // __APPLE__