/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#pragma once

#include "defines.h"
#include "engine/math.h"

#include <memory>

namespace mge
{
    class Texture;

    /// Material data.
    /// 
    class Material
    {
        friend class Scene;
        friend class GBuffer;

    public:
        Material(uint32_t _flags);
        ~Material();

        /// Create a material.
        /// 
        /// @param[in] _flags Material rendering flags (see `MGE_MATERIAL_*`.)
        /// 
        /// @returns Shared Material.
        /// 
        friend std::shared_ptr<Material> createMaterial(uint32_t _flags = MGE_MATERIAL_NONE);

        /// Set material base color using a texture.
        /// 
        /// @param[in] _baseColor Shared base color texture.
        /// 
        void setColor(std::shared_ptr<Texture> _baseColor);

        /// Set material base color using a Vec3 color value.
        /// 
        /// @param[in] _baseColor Color in RGB format.
        /// 
        void setColor(const Vec3& _baseColor);

        /// Set material metallic value.
        /// 
        /// @param[in] _metallic The metallic value (range [0, 1]).
        /// 
        void setMetallic(float _metallic);

        /// Set material metallic value using a texture.
        /// 
        /// @param[in] _metallic Shared metallic texture.
        /// 
        void setMetallic(std::shared_ptr<Texture> _metallic);

        /// Set material roughness value.
        /// 
        /// @param[in] _roughness The roughness value (range [0, 1]).
        /// 
        void setRoughness(float _roughness);

        /// Set material roughness value using a texture.
        /// 
        /// @param[in] _roughness Shared roughness texture.
        /// 
        void setRoughness(std::shared_ptr<Texture> _roughness);

        /// Set material normal texture.
        /// 
        /// @param[in] _normal Shared normal map texture.
        /// 
        void setNormal(std::shared_ptr<Texture> _normal);

        /// Set material normal value.
        /// 
        /// @param[in] _normal The normal value (a scalar value).
        /// 
        void setNormal(float _normal);

        /// Set material occlusion texture.
        /// 
        /// @param[in] _occlusion Shared occlusion texture.
        /// 
        void setOcclusion(std::shared_ptr<Texture> _occlusion);

        /// Set material occlusion value.
        /// 
        /// @param[in] _occlusion The occlusion value (a scalar value).
        /// 
        void setOcclusion(float _occlusion);

        /// Set material emissive texture.
        /// 
        /// @param[in] _emissive Shared emissive texture.
        /// 
        void setEmissive(std::shared_ptr<Texture> _emissive);

        /// Set material emissive color.
        /// 
        /// @param[in] _emissive Emissive color in RGB format.
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

} // namespace mge

