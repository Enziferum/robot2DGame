#pragma once

#include <robot2D/Graphics/Transformable.hpp>

namespace robot2D {
    class TransformComponent final: public robot2D::Transformable {
    public:
        TransformComponent() = default;
        ~TransformComponent() override = default;

        const vec2f& getSize() const {
            return m_size;
        }

        const vec2f& getPosition() const {
            return m_pos;
        }

        FloatRect getLocalBounds() {
            float w = abs(m_size.x);
            float h = abs(m_size.y);
            return {0, 0, w, h};
        }

        FloatRect getGlobalBounds() {
            return getTransform().transformRect(getLocalBounds());
        }

        FloatRect getGlobalBounds(const FloatRect& otherBounds) {
            return getTransform().transformRect(otherBounds);
        }
    };
}