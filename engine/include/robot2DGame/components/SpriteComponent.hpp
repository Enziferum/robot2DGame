#pragma once

#include <robot2D/Graphics/Rect.hpp>

namespace robot2D {
    class SpriteComponent {
    public:
        SpriteComponent();
        ~SpriteComponent() = default;

        void setTextureRect(const robot2D::FloatRect& textureRect);
    private:
        robot2D::FloatRect m_textureRect;
    };
}