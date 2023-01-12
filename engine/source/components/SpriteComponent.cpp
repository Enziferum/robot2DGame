#include <robot2DGame/components/SpriteComponent.hpp>

namespace robot2D {

    void SpriteComponent::setColor(const Color& color) {
        m_color = color;
        m_needUpdate = true;
    }

    const Color& SpriteComponent::getColor() const {
        return m_color;
    }

    void SpriteComponent::setTexture(const Texture& texture) {
        m_texture = &texture;
    }

    const Texture* SpriteComponent::getTexture() const {
        return m_texture;
    }

    void SpriteComponent::setTextureRect(const FloatRect& textureRect) {
        m_textureRect = textureRect;
        m_needUpdate = true;
    }

    const FloatRect& SpriteComponent::getTextureRect() {
        return m_textureRect;
    }
} // namespace robot2D