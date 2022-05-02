#pragma once

#include <array>

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