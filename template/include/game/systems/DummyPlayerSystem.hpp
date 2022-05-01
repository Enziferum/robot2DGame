#pragma once

#include <robot2D/Ecs/System.hpp>

class DummyPlayerSystem: public robot2D::ecs::System {
public:
    DummyPlayerSystem(robot2D::MessageBus& messageBus);
    ~DummyPlayerSystem() override = default;

    void update(float dt) override;

private:
    void processRunning(robot2D::ecs::Entity, float dt);
    void processFalling(robot2D::ecs::Entity, float dt);

    void doCollision(robot2D::ecs::Entity, bool debug = false);
    void resolveCollision(robot2D::ecs::Entity entity,
                          robot2D::ecs::Entity other,
                          robot2D::FloatRect overlap);
    void applyVelocity(robot2D::ecs::Entity entity, float dt);
};