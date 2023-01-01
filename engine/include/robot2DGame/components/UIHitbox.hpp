#pragma once
#include <array>

#include <robot2D/Core/Vector2.hpp>
#include <robot2D/Graphics/Rect.hpp>

namespace robot2D {
    struct UIHitbox {
        enum CallbackID {
            MouseUp = 1,
            MouseDown = 2,
            MouseMoved = 3,
            Count
        };

        std::array<std::int32_t, CallbackID::Count> callbackIDs{-1, -1, -1, -1};

        bool onHoverOnce = true;
        bool onUnHoverOnce = true;

        bool wasHovered = false;

        bool isHovered{false};

        bool contains(const robot2D::vec2f& position) {
            return m_area.contains(position);
        }

        robot2D::FloatRect m_area;
    };
}
