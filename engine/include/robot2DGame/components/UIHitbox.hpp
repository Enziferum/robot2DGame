#pragma once
#include <array>

#include <robot2D/Core/Vector2.hpp>
#include <robot2D/Graphics/Rect.hpp>

namespace robot2D {
    struct UIHitbox {
        enum CallbackID {
            MouseUp = 1,
            MouseDown = 2,
            Count
        };

        std::array<std::uint32_t, CallbackID::Count> callbackIDs{};

        bool contains(const robot2D::vec2f& position) {
            return m_area.contains(position);
        }

        robot2D::FloatRect m_area;
    };
}
