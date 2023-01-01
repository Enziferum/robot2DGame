#include <robot2DGame/components/DrawableComponent.hpp>

#include <cmath>

namespace robot2D {
    DrawableComponent::DrawableComponent():
    m_texture{nullptr},
    m_color(robot2D::Color::White),
    m_depth{0},
    m_layerIndex{1},
    m_needUpdateZbuffer{false}{}

    void DrawableComponent::setTexture(const robot2D::Texture& texture) {
        m_texture = &texture;
    }

    robot2D::Texture& DrawableComponent::getTexture() {
        return const_cast<robot2D::Texture &>(*m_texture);
    }

    const robot2D::Texture& DrawableComponent::getTexture() const {
        return *m_texture;
    }

    void DrawableComponent::setColor(const robot2D::Color& color) {
        m_color = color;
    }

    const robot2D::Color& DrawableComponent::getColor() const {
        return m_color;
    }

    FloatRect DrawableComponent::getBounds() const {
        auto txsize = m_texture -> getSize();

        float w = std::abs(static_cast<float>(txsize.x));
        float h = std::abs(static_cast<float>(txsize.y));
        return {0, 0, w, h};
    }

}