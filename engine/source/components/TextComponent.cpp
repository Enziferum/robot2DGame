#include <robot2DGame/components/TextComponent.hpp>

namespace robot2D {
    robot2D::TextComponent::TextComponent():
    m_text{""},
    m_characterSize{20},
    m_font{nullptr},
    m_needUpdate{false}{}

    void robot2D::TextComponent::setText(const std::string& text) {
        m_text = text;
        m_needUpdate = true;
    }

    void robot2D::TextComponent::setText(std::string&& text) {
        m_text = std::move(text);
        m_needUpdate = true;
    }

    std::string& robot2D::TextComponent::getText() {
        return m_text;
    }

    const std::string& robot2D::TextComponent::getText() const {
        return m_text;
    }

    void robot2D::TextComponent::setCharacterSize(unsigned int value) {
        m_characterSize = value;
        m_needUpdate = true;
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

    const robot2D::Texture& TextComponent::getTexture() const {
        return *(std::move(m_font -> getTextures()[0]));
    }

    std::unordered_map<int, GlyphQuad>& TextComponent::getGlyphCache() {
        if (m_bufferCache.empty() || m_scaled) {
            m_bufferCache = m_font -> getBufferCache(1.F);
            m_scaled = false;
        }
        return m_bufferCache;
    }
}