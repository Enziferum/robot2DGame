#pragma once

#include <array>
#include <unordered_map>

#include <robot2D/Graphics/Texture.hpp>
#include <robot2D/Graphics/VertexArray.hpp>

namespace robot2D {

    struct GlyphCharacter {
        robot2D::vec2f size;
        robot2D::vec2i bearing;
        robot2D::vec2f indexPosition;
        robot2D::vec2f bmpSize;

        float advance;
        float textureIndex;
    };

    struct GlyphVertex {
        robot2D::vec2f position;
        robot2D::vec2f uvs;
    };

    struct GlyphQuad {
        std::array<GlyphVertex, 4> vertices;
    };

    /// Not' thread safe because if load in thread, we create textures onto other thread
    class Font {
    public:
        Font();
        ~Font();

        bool loadFromFile(const std::string& path, int charSize = 20);

        std::unordered_map<int, GlyphQuad> getBufferCache(float scale) const;
        const std::unordered_map<int, GlyphCharacter>& getGlyphCharacters() const;
        const std::vector<std::unique_ptr<robot2D::Texture>>& getTextures() const;
    private:
        bool setup(const std::string& path, int charSize);
        void setupAtlas();
    private:
        void* m_library;
        void* m_face;

        std::vector<std::unique_ptr<robot2D::Texture>> m_textures;
        std::unordered_map<int, GlyphCharacter> m_glyphCharacters;
    };

}
