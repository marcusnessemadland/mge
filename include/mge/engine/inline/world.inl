/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#include <utility>

namespace mge
{
    template<typename T, typename... Args>
    std::shared_ptr<T> World::makeObject(Args&&... _args)
    {
        std::shared_ptr<T> object = std::make_shared<T>(std::forward<Args>(_args)...);
        m_objects.push_back(object);
        return object;
    }

} // namespace mge
