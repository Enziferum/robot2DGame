#pragma once

#include <functional>
#include <robot2D/Core/Vector2.hpp>
#include <robot2D/Core/Time.hpp>

namespace robot2D {
    using AnimationCallback = std::function<void()>;

    struct UIAnimationComponent {
    public:
        enum class AnimationType {
            Scale,
            Rotate
        };

        AnimationType type = AnimationType::Scale;
        robot2D::Time duration;
        robot2D::vec2f from;
        robot2D::vec2f to;

        UIAnimationComponent() = default;
        ~UIAnimationComponent() = default;

        AnimationCallback animationCallback;

        bool used = false;

        void onCallback() {
            if(!used && animationCallback) {
                used = true;
                animationCallback();
            }
        }

        robot2D::vec2f getValue(float dt) {
            if(once) {
                // one
                fullDistX = to.x - from.x;
                fullDistY = to.y - from.y;
                once = false;
                m_duration = duration.asSeconds();
            }

            processed_time += dt;
            float part_of_dist = processed_time / m_duration;

            // moved distance in pixels
            float dtDistX = fullDistX * (part_of_dist - old_part_of_dist);
            float dtDistY = fullDistY * (part_of_dist - old_part_of_dist);

            old_part_of_dist = part_of_dist;
            from.x += dtDistX;
            from.y += dtDistY;

            return from;
        }
    private:
        float fullDistX = 0.F;
        float fullDistY = 0.F;
        float processed_time = 0.F;
        float m_duration = 0.F;
        float old_part_of_dist = 0.F;
        bool once = true;
    };
}