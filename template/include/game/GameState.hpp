#pragma once

#include <robot2DGame/State.hpp>

#include "World.hpp"
#include "GameUI.hpp"

class GameState final: public robot2D::State {
public:
    GameState(robot2D::IStateMachine&,
              robot2D::AppContext& context,
              robot2D::MessageBus& messageBus);
    ~GameState() override = default;

    void setup() override;
    void destroy() override;

    void handleEvents(const robot2D::Event&) override;
    void handleMessages(const robot2D::Message&) override;

    void render() override;
    void update(float dt) override;
private:
    World m_world;
    GameUI m_gameUI;
};