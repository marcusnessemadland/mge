/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#pragma once

#include "defines.h"
#include "engine/math.h"

#include <bgfx/bgfx.h>

#include <memory>

namespace vr
{
    class Texture;

    /// 
    /// https://github.com/KhronosGroup/glTF/tree/master/specification/2.0#metallic-roughness-material
    class Material
    {
        friend class Renderer;
        friend class GBuffer;
        friend class Scene;

    public:
        Material(uint32_t _flags);
        ~Material();

        /// 
        friend std::shared_ptr<Material> createMaterial(uint32_t _flags = VR_MATERIAL_NONE);

        /// 
        void setColor(std::shared_ptr<Texture> _baseColor);

        /// 
        void setColor(const Vec3& _baseColor);

        /// 
        void setMetallic(float _metallic);

        /// 
        void setMetallic(std::shared_ptr<Texture> _metallic);

        /// 
        void setRoughness(float _roughness);

        /// 
        void setRoughness(std::shared_ptr<Texture> _roughness);

        /// 
        void setNormal(std::shared_ptr<Texture> _normal);

        /// 
        void setNormal(float _normal);

        /// 
        void setOcclusion(std::shared_ptr<Texture> _occlusion);

        /// 
        void setOcclusion(float _occlusion);

        /// 
        void setEmissive(std::shared_ptr<Texture> _emissive);

        /// 
        void setEmissive(const Vec3& _emissive);

    private:
        bool blend;
        bool doubleSided;

        std::shared_ptr<Texture> baseColorTexture;
        std::shared_ptr<Texture> metallicTexture; 
        std::shared_ptr<Texture> roughnessTexture;
        std::shared_ptr<Texture> normalTexture;
        std::shared_ptr<Texture> occlusionTexture;
        std::shared_ptr<Texture> emissiveTexture;

        Vec3  baseColorFactor;
        float metallicFactor;
        float roughnessFactor;
        float normalScale;
        float occlusionStrength;
        Vec3  emissiveFactor;
    };
}

