/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#include <utility>
#include <type_traits>
#include <typeinfo>

namespace mge
{
    template<typename T, typename... Args>
    void Object::addComponent(Args&&... _args)
    {
        static_assert(std::is_base_of<Component, T>::value,
            "T must be a subclass of Component");

        const char* name = typeid(T).name();

        auto it = m_components.find(name);
        if (it != m_components.end())
        {
            return;
        }

        m_components[name] = std::make_shared<T>(std::forward<Args>(_args)...);
        m_components[name]->m_owner = shared_from_this();
    }
   
    template<typename T>
    std::shared_ptr<T> Object::getComponent() const
    {
        static_assert(std::is_base_of<Component, T>::value,
            "T must be a subclass of Component");

        const char* name = typeid(T).name();

        auto it = m_components.find(name);
        if (it != m_components.end())
        {
            return std::static_pointer_cast<T>(it->second);
        }

        return nullptr;
    }
    
    template<typename T, typename... Args>
    std::shared_ptr<T> createObject(std::shared_ptr<World> _world, Args&&... _args)
    {
        return _world->makeObject<T>(std::forward<Args>(_args)...);
    }

} // namespace mge
