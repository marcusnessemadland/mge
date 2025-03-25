/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#include "engine/entity.h"
#include "engine/world.h"
#include "engine/component.h"

namespace vr
{
    void Entity::preUpdate(double _dt)
    {
        for (auto& component : m_components)
        {
            component->preUpdate(_dt);
        }
    }

    void Entity::postUpdate(double _dt)
    {
        for (auto& component : m_components)
        {
            component->postUpdate(_dt);
        }
    }

    Entity::Entity()
        : m_position(Vec3()) 
        , m_rotation(Quat()) // Identity
        , m_scale(Vec3(1.0f, 1.0f, 1.0f))
    {
    }

    Entity::~Entity()
    {
    }

    void Entity::setPosition(const Vec3& _position)
    {
        m_position = _position;
    }

    Vec3 Entity::getPosition() const
    {
        return m_position;
    }

    void Entity::setRotation(const Vec3& _euler)
    {
        m_rotation = quat_from_euler(_euler);
    }

    void Entity::setRotation(const Quat& _rotation)
    {
        m_rotation = _rotation;
    }

    Quat Entity::getRotation() const
    {
        return m_rotation;
    }

    void Entity::setScale(const Vec3& _scale)
    {
        m_scale = _scale;
    }

    Vec3 Entity::getScale() const
    {
        return m_scale;
    }

} // namespace vr