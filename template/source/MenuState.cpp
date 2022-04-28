#include <game/MenuState.hpp>

MenuState::MenuState(robot2D::IStateMachine& machine,
                     robot2D::AppContext& context,
                     robot2D::MessageBus& messageBus):
                     robot2D::State(machine, context, messageBus),
                     m_scene{messageBus}{}

void MenuState::setup() {

}

void MenuState::destroy() {

}

void MenuState::handleEvents(const robot2D::Event& event) {

}

void MenuState::handleMessages(const robot2D::Message& message) {

}

void MenuState::update(float dt) {
    m_scene.update(dt);
}

void MenuState::render() {
    m_window -> draw(m_scene);
}
