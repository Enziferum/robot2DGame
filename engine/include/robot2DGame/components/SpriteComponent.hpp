#pragma once

#include <robot2D/Graphics/Rect.hpp>
#include <robot2D/Graphics/Color.hpp>
#include <robot2D/Graphics/Texture.hpp>

namespace robot2D {
    class SpriteComponent {
    public:
        SpriteComponent() = default;
        ~SpriteComponent() = default;

        void setColor(const Color& color);
        const Color& getColor() const;
        void setTexture(const Texture& texture);
        const Texture* getTexture() const;

        void setTextureRect(const FloatRect& textureRect);
        const FloatRect& getTextureRect();

        struct Animation final {
            std::vector<robot2D::IntRect> frames;
            std::vector<robot2D::IntRect> flip_frames;
            float speed;
            bool looped = false;
        };

        void addAnimations(std::vector<Animation>& animations) { m_animations = animations; }
        const std::vector<Animation>& getAnimation() const { return m_animations; }
    private:
        friend class SpriteSystem;
        const Texture* m_texture{nullptr};
        bool m_needUpdate {false};
        FloatRect m_textureRect{};
        robot2D::Color m_color{robot2D::Color::White};

        std::vector<Animation> m_animations;
    };
}