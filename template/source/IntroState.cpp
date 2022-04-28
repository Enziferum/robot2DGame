#include <robot2DGame/systems/RendererSystem.hpp>
#include <robot2DGame/Api.hpp>
#include <robot2DGame/components/UIAnimationComponent.hpp>
#include <robot2DGame/systems/UIAnimationSystem.hpp>

#include <game/IntroState.hpp>

IntroState::IntroState(robot2D::IStateMachine& machine,
                       robot2D::AppContext& context,
                       robot2D::MessageBus& messageBus):
        robot2D::State(machine, context, messageBus),
        m_scene{messageBus}
        {}

void IntroState::setup() {
    m_textures.loadFromFile(TextureID::Logo, "res/textures/logo.png");
    setupEcs();
}

void IntroState::setupEcs() {
    auto windowSz = m_window -> getSize();
    m_scene.addSystem<robot2D::RenderSystem>(m_messageBus);
    m_scene.addSystem<robot2D::UIAnimationSystem>(m_messageBus);

    robot2D::vec2f startSize = {};

    auto entity = robot2D::createEntity(m_scene,
                                        {windowSz.as<float>().x / 2.F,
                                                windowSz.as<float>().y / 2.F},
                                        startSize,
                                        m_textures.get(TextureID::Logo));

    auto& animation = entity.addComponent<robot2D::UIAnimationComponent>();
    animation.duration = robot2D::seconds(2);
    animation.from = startSize;
    animation.to = {300.F, 300.F};
}

void IntroState::destroy() {}

void IntroState::handleEvents(const robot2D::Event& event) {}

void IntroState::handleMessages(const robot2D::Message& message) {}

void IntroState::update(float dt) {
    m_scene.update(dt);
}

void IntroState::render() {
    m_window -> draw(m_scene);
}
