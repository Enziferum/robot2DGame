#include <game/InputParser.hpp>
#include <game/components/PlayerComponent.hpp>
#include <robot2D/Ecs/EntityManager.hpp>

InputParser::InputParser():
m_inputBinding{} {}

void InputParser::handleEvent(const robot2D::Event& evt) {
    if(!enabled)
        return;
    if (evt.type == robot2D::Event::KeyPressed)
    {
        if (evt.key.code == m_inputBinding.keys[InputBinding::Jump])
        {
            m_currentInput |= InputFlag::Jump;
        }
        else if (evt.key.code == m_inputBinding.keys[InputBinding::Left])
        {
            m_currentInput |= InputFlag::Left;
        }
        else if (evt.key.code == m_inputBinding.keys[InputBinding::Right])
        {
            m_currentInput |= InputFlag::Right;
        }
        else if (evt.key.code == m_inputBinding.keys[InputBinding::Shoot])
        {
            m_currentInput |= InputFlag::Shoot;
        }
    }
    else if (evt.type == robot2D::Event::KeyReleased)
    {
        if (evt.key.code == m_inputBinding.keys[InputBinding::Jump])
        {
            m_currentInput &= ~InputFlag::Jump;
        }
        else if (evt.key.code == m_inputBinding.keys[InputBinding::Left])
        {
            m_currentInput &= ~InputFlag::Left;
        }
        else if (evt.key.code == m_inputBinding.keys[InputBinding::Right])
        {
            m_currentInput &= ~InputFlag::Right;
        }
        else if (evt.key.code == m_inputBinding.keys[InputBinding::Shoot])
        {
            m_currentInput &= ~InputFlag::Shoot;
        }
    }
}

void InputParser::update() {
    if(!enabled)
        return;
    auto& player = m_playerEntity.getComponent<PlayerComponent>();

    //update player input history
    player.input = m_currentInput;
    player.accelerationMultiplier = m_runningMultiplier;

    //reset analogue multiplier
    //this has to be set at one for keyboard input
    if ((m_currentInput & (InputFlag::Left | InputFlag::Right)) == 0)
    {
        m_runningMultiplier = 1.f;
    }
}

void InputParser::setEntity(robot2D::ecs::Entity entity) {
    if(!entity.hasComponent<PlayerComponent>())
        return;
    m_playerEntity = entity;
}
