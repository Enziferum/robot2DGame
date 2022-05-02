#pragma once

#include <robot2D/Core/Keyboard.hpp>
#include <robot2D/Core/Event.hpp>
#include <robot2D/Ecs/Entity.hpp>

#include "Input.hpp"

class InputParser final {
public:
    InputParser();
    ~InputParser() = default;

    void setEntity(robot2D::ecs::Entity entity);

    void handleEvent(const robot2D::Event& event);
    void update();
private:
    InputBinding m_inputBinding;
    robot2D::ecs::Entity m_playerEntity;

    float m_runningMultiplier;
    std::uint16_t m_currentInput;
};
