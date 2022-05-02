#pragma once

#include <robot2D/Ecs/System.hpp>
#include "robot2DGame/components/TextComponent.hpp"


namespace robot2D {
    class TextSystem: public robot2D::ecs::System {
    public:
        TextSystem(robot2D::MessageBus& messageBus);
        ~TextSystem() override = default;

        void update(float dt);
    };

}