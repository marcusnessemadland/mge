/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#pragma once

/// These should be the same as in samplers.sh for shaders
/// 
struct Samplers
{
	enum Enum
	{
        None = 0,
        
        BaseColor = 1,
        Metal = 2,
        Roughness = 3,
        Normal = 4,
        Occlusion = 5,
        Emissive = 6,
        
        DeferredDiffuseA = 7,
        DeferredNormal = 8,
        DeferredFresnelMetallic = 9,
        DeferredEmissiveOcclusion = 10,
        DeferredDepth = 11,

        SkyboxCubemap = 12,

	};
};