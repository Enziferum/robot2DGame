#include <robot2D/Graphics/RenderTarget.hpp>
#include <game/GameUI.hpp>

GameUI::GameUI(robot2D::MessageBus& messageBus):
    m_scene(messageBus) {}

void GameUI::setup() {

}

void GameUI::handleMessages(const robot2D::Message& message) {

}

void GameUI::update(float dt) {
    m_scene.update(dt);
}

void GameUI::draw(robot2D::RenderTarget& target,
                  robot2D::RenderStates states) const {
    target.draw(m_scene);
}
