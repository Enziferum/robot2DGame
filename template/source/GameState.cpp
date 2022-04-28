#include <game/GameState.hpp>

GameState::GameState(robot2D::IStateMachine& machine,
                     robot2D::AppContext& context, robot2D::MessageBus& messageBus):
            robot2D::State{machine, context, messageBus},
            m_world{messageBus},
            m_gameUI{messageBus}
            {}

void GameState::setup() {
    /// Initalize Resources
    m_world.setup();
    m_gameUI.setup();
}

void GameState::destroy() {
    /// Destroy Resources
}


void GameState::handleEvents(const robot2D::Event& event) {

}

void GameState::handleMessages(const robot2D::Message& message) {
    m_world.handleMessages(message);
    m_gameUI.handleMessages(message);
}

void GameState::update(float dt) {
    m_world.update(dt);
    m_gameUI.update(dt);
}

void GameState::render() {
    m_window -> draw(m_world);
    m_window -> draw(m_gameUI);
}
