#pragma once

#include <string>
#include <robot2D/Graphics/Transformable.hpp>
#include <robot2D/Graphics/Color.hpp>
#include <robot2DGame/Font.hpp>

namespace robot2D {
    class TextComponent: public robot2D::Transformable {
    public:
        TextComponent();
        ~TextComponent() override = default;

        void setText(const std::string& text);
        void setText(std::string&& text);

        std::string& getText();
        const std::string& getText() const;

        void setCharacterSize(unsigned int value);
        unsigned int getCharacterSize();

        void setFont(const Font& font);
        const Font* getFont() const;

        const robot2D::Texture& getTexture() const;
        std::unordered_map<int, GlyphQuad>& getGlyphCache();
    private:
        std::string m_text;
        unsigned int m_characterSize;
        const Font* m_font;
        bool m_needUpdate;
        std::unordered_map<int, GlyphQuad> m_bufferCache;
        friend class TextSystem;

        bool m_scaled = false;
    };
}