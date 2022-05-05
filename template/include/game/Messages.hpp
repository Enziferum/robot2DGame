#pragma once

#include <robot2D/Ecs/Entity.hpp>
#include <robot2D/Core/Message.hpp>

enum MessageID {
    PlayerMessage = robot2D::Message::Count,
    AddPlatform
};

struct PlayerEvent {
    enum MessageType{
        Shot
    }type;
    robot2D::ecs::Entity entity;
};

struct PlatformEvent {
    robot2D::vec2f size;
    robot2D::vec2f position;
};