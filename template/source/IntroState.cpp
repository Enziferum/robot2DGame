#include <robot2DGame/systems/RendererSystem.hpp>
#include <robot2DGame/components/DrawableComponent.hpp>
#include <robot2DGame/components/TransformComponent.hpp>

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
    auto entity = m_scene.createEntity();

    auto& transform = entity.addComponent<robot2D::TransformComponent>();
    transform.setPosition({windowSz.as<float>().x / 2.F, windowSz.as<float>().y / 2.F});
    transform.setSize({200.F, 200.F});

    entity.addComponent<robot2D::DrawableComponent>().setTexture(m_textures.get(TextureID::Logo));
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
