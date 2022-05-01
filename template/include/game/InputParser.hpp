#pragma once

#include <array>

#include <robot2D/Core/Keyboard.hpp>
#include <robot2D/Core/Event.hpp>
#include <robot2D/Ecs/Entity.hpp>

namespace InputFlag
{
    enum
    {
        Left = 0x1,
        Right = 0x2,
        Jump = 0x4,
        Shoot = 0x8
    };
}

struct InputBinding final
{
    enum
    {
        Jump, Shoot, Left, Right, Count
    };

    std::array<robot2D::Key, Count> keys = { robot2D::Key::W,
                                             robot2D::Key::SPACE,
                                             robot2D::Key::A, robot2D::Key::D };
    std::array<uint32_t , 3u> buttons = { 0, 1 };
    uint32_t controllerID = 0;
};

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
