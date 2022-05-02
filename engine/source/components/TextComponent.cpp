#include <robot2DGame/components/TextComponent.hpp>

namespace robot2D {
    robot2D::TextComponent::TextComponent():
    m_text{""},
    m_characterSize{20},
    m_font{nullptr},
    m_color{robot2D::Color::White} {}

    void robot2D::TextComponent::setText(const std::string& text) {
        m_text = text;
    }

    void robot2D::TextComponent::setText(std::string&& text) {
        m_text = std::move(text);
    }

    std::string& robot2D::TextComponent::getText() {
        return m_text;
    }

    const std::string& robot2D::TextComponent::getText() const {
        return m_text;
    }

    void robot2D::TextComponent::setCharacterSize(unsigned int value) {
        m_characterSize = value;
    }

    unsigned int robot2D::TextComponent::getCharacterSize() {
        return m_characterSize;
    }

    void robot2D::TextComponent::setFont(const Font& font) {
        m_font = &font;
    }

    const Font* robot2D::TextComponent::getFont() const {
        return m_font;
    }

    void robot2D::TextComponent::setColor(const Color &color) {
        m_color = color;
    }

    Color& robot2D::TextComponent::getColor() {
        return m_color;
    }

    const Color& robot2D::TextComponent::getColor() const {
        return m_color;
    }
}