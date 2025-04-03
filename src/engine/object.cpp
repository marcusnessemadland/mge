/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#include "engine/object.h"
#include "engine/world.h"
#include "engine/component.h"

namespace mge
{
    void Object::preUpdate(double _dt)
    {
        for (auto& component : m_components)
        {
            component.second->preUpdate(_dt);
        }
    }

    void Object::postUpdate(double _dt)
    {
        for (auto& component : m_components)
        {
            component.second->postUpdate(_dt);
        }
    }

    Object::Object()
        : m_position(Vec3()) 
        , m_rotation(Quat()) // Identity
        , m_scale(Vec3(1.0f, 1.0f, 1.0f))
    {
    }

    Object::~Object()
    {
    }

    void Object::setPosition(const Vec3& _position)
    {
        m_position = _position;
    }

    Vec3 Object::getPosition() const
    {
        return m_position;
    }

    void Object::setRotation(const Vec3& _euler)
    {
        m_rotation = quat_from_euler(_euler);
    }

    void Object::setRotation(const Quat& _rotation)
    {
        m_rotation = _rotation;
    }

    Quat Object::getRotation() const
    {
        return m_rotation;
    }

    void Object::setScale(const Vec3& _scale)
    {
        m_scale = _scale;
    }

    Vec3 Object::getScale() const
    {
        return m_scale;
    }

} // namespace mge