#pragma once

#include <robot2D/Ecs/Entity.hpp>
#include <robot2D/Core/Message.hpp>

enum MessageID {
    PlayerMessage = robot2D::Message::Count
};

struct PlayerEvent {
    enum MessageType{
        Shot
    }type;
    robot2D::ecs::Entity entity;
};