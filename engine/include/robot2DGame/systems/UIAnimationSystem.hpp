#pragma once

#include <robot2D/Ecs/System.hpp>
#include <robot2D/Core/Time.hpp>

namespace robot2D {

    inline Time seconds(float amount)
    {
        return Time(static_cast<int64_t>(amount * 1000000));
    }

    inline Time microseconds(int64_t amount) {
        return {amount};
    }

    inline Time operator-(Time right) {
        return microseconds(-right.asMicroSeconds());
    }

    inline Time operator-(Time left, Time right) {
        return microseconds(left.asMicroSeconds() - right.asMicroSeconds());
    }

    inline Time& operator -=(Time& left, Time right) {
        return left = left - right;
    }

    class UIAnimationSystem: public ecs::System {
    public:
        UIAnimationSystem(MessageBus&);
        ~UIAnimationSystem() override = default;

        void update(float dt);
    };
}