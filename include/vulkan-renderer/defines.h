/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#pragma once

/**
 * Material states.
 *
 */
#define VR_MATERIAL_NONE			UINT32_C(0x00000000) //!< No material flags.
#define VR_MATERIAL_BLEND			UINT32_C(0x00010000) //!< Render transparency.
#define VR_MATERIAL_DOUBLE_SIDED	UINT32_C(0x00020000) //!< Render both sides.