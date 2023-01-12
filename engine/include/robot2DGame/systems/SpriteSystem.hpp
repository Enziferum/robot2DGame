#pragma once

#include <robot2D/Ecs/System.hpp>

namespace robot2D {
    class SpriteSystem final: public robot2D::ecs::System {
    public:
        SpriteSystem(robot2D::MessageBus& messageBus);
        ~SpriteSystem() override = default;

        void update(float dt) override;
    };
}