#pragma once
#include <robot2D/Core/Vector2.hpp>

struct PlayerComponent final {
    std::uint16_t input = 0;
    std::uint16_t prevInput = 0;
    float accelerationMultiplier = 0.f;

    PlayerComponent() = default;
    ~PlayerComponent() = default;

    PlayerComponent(const PlayerComponent& other):
    input{other.input}, prevInput{other.prevInput},
    state{other.state}, velocity(other.velocity) {
        accelerationMultiplier = 0.f;
    }
    PlayerComponent& operator=(const PlayerComponent& other) = default;

    enum
    {
        Running, Falling,
        Dying, Dead
    }state = Falling;

    static constexpr float Acceleration = 120.f;
    static constexpr float Drag = 0.81f;

    robot2D::vec2f velocity;
};