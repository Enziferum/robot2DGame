#include <robot2D/Graphics/RenderTarget.hpp>
#include <robot2DGame/Api.hpp>
#include <robot2DGame/systems/UISystem.hpp>
#include <robot2DGame/systems/RendererSystem.hpp>

#include <game/GameUI.hpp>

GameUI::GameUI(robot2D::MessageBus& messageBus):
    m_bus{messageBus},
    m_scene(messageBus) {}

void GameUI::setup() {
    setupEcs();
}

void GameUI::setupEcs() {
    m_scene.addSystem<robot2D::RenderSystem>(m_bus);
    m_scene.addSystem<robot2D::UISystem>(m_bus);
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
