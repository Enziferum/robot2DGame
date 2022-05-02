#pragma once
#include <functional>

#include <robot2D/Ecs/Scene.hpp>
#include <robot2D/Ecs/EntityManager.hpp>
#include <robot2DGame/components/TransformComponent.hpp>
#include <robot2DGame/components/DrawableComponent.hpp>

namespace robot2D {
#define BIND_BUTTON_CLASS_FN(fn) [this](auto&&... args) -> decltype(auto) \
{                                                                         \
    return this -> fn(std::forward<decltype(args)>(args)...);              \
}

    robot2D::ecs::Entity createEntity(robot2D::ecs::Scene& scene,
                                      robot2D::vec2f position,
                                      robot2D::vec2f size);

    robot2D::ecs::Entity createEntity(robot2D::ecs::Scene& scene,
                                      robot2D::vec2f position,
                                      robot2D::vec2f size,
                                      robot2D::Texture& texture);

    robot2D::ecs::Entity createImageButton(robot2D::ecs::Scene& scene,
                                           robot2D::vec2f position,
                                           robot2D::vec2f size,
                                           robot2D::Texture& texture,
                                           std::string text,
                                           std::function<void(robot2D::ecs::Entity entity,
                                                              std::uint64_t flags)>&& callback);
}