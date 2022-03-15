#pragma once

#include <robot2D/Ecs/Scene.hpp>
#include <robot2D/Core/Message.hpp>
#include <robot2D/Graphics/Drawable.hpp>

class World: public robot2D::Drawable {
public:
    World(robot2D::MessageBus& messageBus);
    ~World() = default;

    void setup();

    void handleMessages(const robot2D::Message& message);
    void update(float dt);
    void draw(robot2D::RenderTarget& target, robot2D::RenderStates states) const override;
private:
    robot2D::ecs::Scene m_scene;
};