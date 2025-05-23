/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#include "engine/material.h"

namespace mge
{
	Material::Material(uint32_t _flags)
        : baseColorFactor(Vec3(1.0f, 1.0f, 1.0f))
        , metallicFactor(1.0f)
        , roughnessFactor(1.0f)
        , normalScale(1.0f)
        , occlusionStrength(1.0f)
    {
        blend = (_flags & MGE_MATERIAL_BLEND) != 0;
        doubleSided = (_flags & MGE_MATERIAL_DOUBLE_SIDED) != 0;
    }

    Material::~Material()
    {
    }

    std::shared_ptr<Material> createMaterial(uint32_t _flags)
    {
        return std::make_shared<Material>(_flags);
    }

    void Material::setColor(std::shared_ptr<Texture> _baseColor)
    {
        baseColorTexture = _baseColor;
    }

    void Material::setColor(const Vec3& _baseColor)
    {
        baseColorFactor = _baseColor;
    }

    void Material::setMetallic(float _metallic)
    {
        metallicFactor = _metallic;
    }

    void Material::setMetallic(std::shared_ptr<Texture> _metallic)
    {
        metallicTexture = _metallic;
    }

    void Material::setRoughness(float _roughness)
    {
        roughnessFactor = _roughness;
    }

    void Material::setRoughness(std::shared_ptr<Texture> _roughness)
    {
        roughnessTexture = _roughness;
    }

    void Material::setNormal(std::shared_ptr<Texture> _normal)
    {
        normalTexture = _normal;
    }

    void Material::setNormal(float _normal)
    {
        normalScale = _normal;
    }

    void Material::setOcclusion(std::shared_ptr<Texture> _occlusion)
    {
        occlusionTexture = _occlusion;
    }

    void Material::setOcclusion(float _occlusion)
    {
        occlusionStrength = _occlusion;
    }

    void Material::setEmissive(std::shared_ptr<Texture> _emissive)
    {
        emissiveTexture = _emissive;
    }

    void Material::setEmissive(const Vec3& _emissive)
    {
        emissiveFactor = _emissive;
    }

} // namespace mge