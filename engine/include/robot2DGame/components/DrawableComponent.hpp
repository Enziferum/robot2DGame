#pragma once

#include <array>

#include <robot2D/Graphics/Texture.hpp>
#include <robot2D/Graphics/Color.hpp>
#include <robot2D/Graphics/Rect.hpp>
#include <robot2D/Graphics/Vertex.hpp>

namespace robot2D {
    using quadVertexArray = std::array<robot2D::Vertex, 4>;

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

        inline void setDepth(int value);
        inline int getDepth() const;

        inline void setLayerIndex(unsigned int value);
        inline unsigned int getLayerIndex() const;

        void setQuadVertexArray(const quadVertexArray& array);
        const quadVertexArray& getVertices() const;
    private:
        const robot2D::Texture* m_texture;
        robot2D::Color m_color;
        quadVertexArray m_vertices;

        int m_depth;
        unsigned int m_layerIndex;
        /// emulate layers and make possible set when render
        bool m_needUpdateZbuffer;

        friend class RenderSystem;
    };

    inline void DrawableComponent::setDepth(int value) {
        m_depth = value;
        m_needUpdateZbuffer = true;
    }

    inline int DrawableComponent::getDepth() const {
        return m_depth;
    }

    inline void DrawableComponent::setLayerIndex(unsigned int value) {
        m_layerIndex = value;
    }

    inline unsigned int DrawableComponent::getLayerIndex() const {
        return m_layerIndex;
    }

}