#include <random>
#include <robot2DGame/StateMachine.hpp>

#include <robot2D/Ecs/System.hpp>

#include <robot2DGame/systems/UISystem.hpp>
#include <robot2DGame/systems/RendererSystem.hpp>
#include <robot2DGame/systems/TextSystem.hpp>
#include <robot2DGame/components/UIHitbox.hpp>

#include <robot2DGame/Api.hpp>

#include <game/MenuState.hpp>
#include <game/States.hpp>

namespace {
    constexpr robot2D::vec2f buttonSize = { 200.F, 50.F };
    constexpr robot2D::vec2f cloudSize = {100.F, 40.F};

    enum class MenuMessages: int32_t {
        GenCloud = robot2D::Message::Count
    };
}

class CloudSystem: public robot2D::ecs::System {
public:
    CloudSystem(robot2D::MessageBus& messageBus, float border);
    ~CloudSystem() override  = default;

    void update(float dt) override;
private:
    float m_border;
};

struct Cloud {

};

CloudSystem::CloudSystem(robot2D::MessageBus& messageBus, float border):
    robot2D::ecs::System(messageBus, typeid(CloudSystem())),
    m_border{border}{

    addRequirement<robot2D::TransformComponent>();
    addRequirement<Cloud>();

}

void CloudSystem::update(float dt) {
    constexpr float speedX = 10.F;

    for(auto& entity: m_entities) {
        auto& transform = entity.getComponent<robot2D::TransformComponent>();
        transform.move({ speedX * dt, 0 } );
        if(transform.getPosition().x >= m_border) {
            getScene() -> removeEntity(entity);
            m_messageBus.postMessage<char>( static_cast<robot2D::Message::ID>(MenuMessages::GenCloud) );
        }
    }

}



MenuState::MenuState(robot2D::IStateMachine& machine,
                     robot2D::AppContext& context,
                     robot2D::MessageBus& messageBus):
                     robot2D::State(machine, context, messageBus),
                     m_scene{messageBus}{}

void MenuState::setup() {
    m_scene.addSystem<robot2D::RenderSystem>(m_messageBus);
    m_scene.addSystem<robot2D::UISystem>(m_messageBus);
    m_scene.addSystem<robot2D::TextSystem>(m_messageBus);
    m_scene.addSystem<CloudSystem>(m_messageBus, m_window -> getSize().x);

    m_textures.loadFromFile(ResourceID::Button, "res/textures/menu_button.png");
    m_textures.loadFromFile(ResourceID::Cloud, "res/textures/cloud.png");
    m_textures.loadFromFile(ResourceID::Background, "res/textures/background.png");


    m_fonts.loadFromFile(ResourceID::Font, "res/font/game_font.ttf");

    m_music.loadFromFile("res/audio/breakout.wav");
    m_music.setLoop(true);
    m_music.play();

    m_soundBuffer.loadFromFile("res/audio/buttonHover.wav");


    setupClouds();

    auto windowSize = m_window -> getSize();

    auto startButtonPos = robot2D::vec2f {
            windowSize.as<float>().x / 2 - buttonSize.x / 2,
            windowSize.as<float>().y / 2 - buttonSize.y / 2
    };
    sound.setSoundBuffer(&m_soundBuffer);
    sound.setVolume(100.F);

    auto hoverCallback = [this](robot2D::ecs::Entity entity) {
        entity.getComponent<robot2D::DrawableComponent>().setColor(robot2D::Color::Yellow);
        sound.play();
    };

    auto unHoverCallback = [](robot2D::ecs::Entity entity) {
        entity.getComponent<robot2D::DrawableComponent>().setColor(robot2D::Color::White);
    };

    auto startButton = robot2D::createImageButton(m_scene,
                                                  startButtonPos,
                                                  buttonSize,
                                                  m_textures.get(ResourceID::Button),
                                                  "Start",
                                                  m_fonts.get(ResourceID::Font),
                                                  BIND_BUTTON_CLASS_FN(MenuState::onStartButton));

    startButton.getComponent<robot2D::UIHitbox>().callbackIDs[robot2D::UIHitbox::CallbackID::MouseMoved] =
            m_scene.getSystem<robot2D::UISystem>() -> addMouseHoverCallback(hoverCallback);

    startButton.getComponent<robot2D::UIHitbox>().callbackIDs[robot2D::UIHitbox::CallbackID::MouseMoved] =
            m_scene.getSystem<robot2D::UISystem>() -> addMouseUnHoverCallback(unHoverCallback);

    auto startPos = startButton.getComponent<robot2D::TransformComponent>().getPosition();
    //robot2D::vec2f endBtnPosition = { startPos.x, startPos.y + buttonSize.y * 2};

    robot2D::vec2f settingsBtnPosition = { startPos.x, startPos.y + buttonSize.y * 2};

    auto settingsButton = robot2D::createImageButton(m_scene,
                                                     settingsBtnPosition,
                                                     buttonSize,
                                                     m_textures.get(ResourceID::Button),
                                                     "Settings",
                                                     m_fonts.get(ResourceID::Font),
                                                     BIND_BUTTON_CLASS_FN(MenuState::onEndButton));

    auto settingsPos = settingsButton.getComponent<robot2D::TransformComponent>().getPosition();
    robot2D::vec2f endBtnPosition = { settingsPos.x, settingsPos.y + buttonSize.y * 2};

    settingsButton.getComponent<robot2D::UIHitbox>().callbackIDs[robot2D::UIHitbox::CallbackID::MouseMoved] =
            m_scene.getSystem<robot2D::UISystem>() -> addMouseHoverCallback(hoverCallback);

    settingsButton.getComponent<robot2D::UIHitbox>().callbackIDs[robot2D::UIHitbox::CallbackID::MouseMoved] =
            m_scene.getSystem<robot2D::UISystem>() -> addMouseUnHoverCallback(unHoverCallback);

    auto endButton = robot2D::createImageButton(m_scene,
                                                endBtnPosition,
                                                buttonSize,
                                                  m_textures.get(ResourceID::Button),
                                                  "Exit",
                                                  m_fonts.get(ResourceID::Font),
                                                  BIND_BUTTON_CLASS_FN(MenuState::onEndButton));

    endButton.getComponent<robot2D::UIHitbox>().callbackIDs[robot2D::UIHitbox::CallbackID::MouseMoved] =
            m_scene.getSystem<robot2D::UISystem>() -> addMouseHoverCallback(hoverCallback);

    endButton.getComponent<robot2D::UIHitbox>().callbackIDs[robot2D::UIHitbox::CallbackID::MouseMoved] =
            m_scene.getSystem<robot2D::UISystem>() -> addMouseUnHoverCallback(unHoverCallback);


}

void MenuState::onStartButton(robot2D::ecs::Entity entity, uint64_t flags) {
    (void)entity;

    if ( flags & robot2D::UISystem::Flags::LeftMouse) {
        m_machine.pop();
        m_machine.pushState(static_cast<robot2D::StateID>(States::Game));
    }
}

void MenuState::onEndButton(robot2D::ecs::Entity entity, uint64_t flags) {
    (void)entity;
    if ( flags & robot2D::UISystem::Flags::LeftMouse)
        destroy();
}

void MenuState::destroy() {

}

void MenuState::handleEvents(const robot2D::Event& event) {
    m_scene.getSystem<robot2D::UISystem>() -> handleEvents(event);
}

void MenuState::handleMessages(const robot2D::Message& message) {
    if(message.id == static_cast<robot2D::Message::ID>(MenuMessages::GenCloud)) {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> posY(100, 200); // distribution in range [1, 6]
        std::uniform_int_distribution<std::mt19937::result_type> posX(50, 600); // distribution in range [1, 6]
        auto ent = robot2D::createEntity(m_scene, robot2D::vec2f{posX(rng), posY(rng)},
                                         cloudSize, m_textures.get(ResourceID::Cloud));
        ent.addComponent<Cloud>();
        ent.getComponent<robot2D::DrawableComponent>().setColor(robot2D::Color::fromGL(1, 1, 1, 1));
    }
}

void MenuState::update(float dt) {
    m_scene.update(dt);
}

void MenuState::render() {
    m_window -> draw(m_scene);
}

void MenuState::setupClouds() {
    std::random_device dev;
    std::mt19937 rng(dev());

    auto winSize = m_window -> getSize();
    robot2D::createEntity(m_scene, {}, winSize.as<float>(), m_textures.get(ResourceID::Background));

    constexpr int maxClouds = 10;
    robot2D::FloatRect cloudsRecs[maxClouds];

    for(int i = 0; i < 10; ++ i) {
        std::uniform_int_distribution<std::mt19937::result_type> posY(100, 200); // distribution in range [1, 6]
        std::uniform_int_distribution<std::mt19937::result_type> posX(50, 600); // distribution in range [1, 6]

        robot2D::FloatRect currRect = { static_cast<float>(posX(rng)),
                                        static_cast<float>(posY(rng)), cloudSize.x, cloudSize.y};

        cloudsRecs[i] = currRect;

        auto ent = robot2D::createEntity(m_scene, robot2D::vec2f{posX(rng), posY(rng)},
                              cloudSize, m_textures.get(ResourceID::Cloud));
        ent.addComponent<Cloud>();
        ent.getComponent<robot2D::DrawableComponent>().setColor(robot2D::Color::fromGL(1, 1, 1, 1));
    }
}
