#pragma once
#include <robot2D/Core/MessageBus.hpp>
#include <robot2D/Graphics/Drawable.hpp>
#include <robot2D/Ecs/Scene.hpp>

class GameUI final: public robot2D::Drawable {
public:
    GameUI(robot2D::MessageBus& messageBus);
    ~GameUI() override = default;

    void setup();

    void handleMessages(const robot2D::Message& message);
    void update(float dt);
    void draw(robot2D::RenderTarget& target,
              robot2D::RenderStates states) const override;
private:
    robot2D::ecs::Scene m_scene;
};