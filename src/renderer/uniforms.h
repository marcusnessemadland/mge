/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
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
        MetalRoughness = 2,
        Normal = 3,
        Occlusion = 4,
        Emissive = 5,
        
        Lights = 6,
        
        DeferredDiffuseA = 7,
        DeferredNormal = 8,
        DeferredFresnelMetallic = 9,
        DeferredEmissiveOcclusion = 10,
        DeferredDepth = 11,
	};
};