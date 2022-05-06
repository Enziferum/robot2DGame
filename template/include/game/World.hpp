#pragma once

#include <robot2D/Ecs/Scene.hpp>
#include <robot2D/Core/Message.hpp>
#include <robot2D/Graphics/Drawable.hpp>

#include "InputParser.hpp"
#include "Map.hpp"

class World: public robot2D::Drawable {
public:
    World(robot2D::MessageBus& messageBus);
    ~World() = default;

    void setup(InputParser& inputParser);

    void handleMessages(const robot2D::Message& message);
    void update(float dt);
    void draw(robot2D::RenderTarget& target,
              robot2D::RenderStates states) const override;

private:
    void setupEcs();
    void setupPlayer(InputParser& inputParser);
    void setupMap();
private:
    robot2D::MessageBus& m_bus;
    robot2D::ecs::Scene m_scene;
    Map m_map;
};