#pragma once

#include <robot2D/Graphics/Texture.hpp>
#include <robot2D/Graphics/Color.hpp>
#include <robot2D/Graphics/Rect.hpp>

namespace robot2D {
    class DrawableComponent final {
    public:
        DrawableComponent();
        ~DrawableComponent() = default;

        void setTexture(const robot2D::Texture& texture);
        robot2D::Texture& getTexture();
        const robot2D::Texture& getTexture() const;

        FloatRect getBounds() const;

        void setColor(const robot2D::Color& color);
        const robot2D::Color& getColor() const;
    private:
        const robot2D::Texture* m_texture;
        robot2D::Color m_color;
    };
}