#pragma once

#include <vector>
#include <robot2D/Graphics/Transformable.hpp>

namespace robot2D {
    class TransformComponent final: public robot2D::Transformable {
    public:
        TransformComponent();
        ~TransformComponent() override = default;

        void addChild(TransformComponent& );
        void removeChild(TransformComponent& );

        void setOrigin(const robot2D::vec2f& origin) override {
            if(origin.x > 1.F && origin.y > 1.F) {
                auto rect = getLocalBounds();
                m_origin.x = origin.x / rect.width;
                m_origin.y = origin.y / rect.width;
                m_transformedOrigin = origin;
            } else
                m_origin = origin;
            m_update_transform = true;
        }

        const vec2f& getOrigin() const {
            return m_transformedOrigin;
        }

        void setSize(const robot2D::vec2f& size) override {
            if(m_size == robot2D::vec2f{}) {
                scale(size);
            } else {
                float scaleX = 0.F;
                float scaleY = 0.F;
                float maxX = std::max(m_size.x, size.x);
                float minX = std::min(m_size.x, size.x);
                float maxY = std::max(m_size.y, size.y);
                float minY = std::min(m_size.y, size.y);

                scaleX = maxX / minX;
                scaleY = maxY / minY;
                scale({scaleX, scaleY});
            }
            m_size = size;
        }
        void setSize(const float& x, const float& y) override {
            setSize({x, y});
        }
        const vec2f& getSize() const {
            return m_size;
        }

        const vec2f& getPosition() const {
            if(m_parent)
                return m_parent -> m_pos;
            return m_pos;
        }

        Transform getWorldTransform() const {
            if(m_parent)
                return m_parent -> getWorldTransform();
            return getTransform();
        }
        vec2f getWorldPosition() const {
            return getWorldTransform().transformPoint({});
        }

        const robot2D::vec2f& getScale() const {
            return m_scale_factor;
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
    private:
        robot2D::vec2f m_transformedOrigin;
        TransformComponent* m_parent;
        std::vector<TransformComponent*> m_children;
    };
}