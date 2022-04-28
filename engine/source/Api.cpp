#include <robot2DGame/Api.hpp>
#include <robot2DGame/components/UIHitbox.hpp>
#include <robot2DGame/systems/UISystem.hpp>

namespace robot2D {
    robot2D::ecs::Entity
    createEntity(robot2D::ecs::Scene& scene,
                 vec2f position,
                 vec2f size,
                 Texture& texture) {
        auto entity= scene.createEntity();
        auto& transform = entity.addComponent<robot2D::TransformComponent>();
        transform.setPosition(position);
        transform.setSize(size);

        auto& drawable = entity.addComponent<robot2D::DrawableComponent>();
        drawable.setTexture(texture);

        return entity;
    }

    robot2D::ecs::Entity
    createImageButton(ecs::Scene& scene,
                      robot2D::vec2f position,
                      robot2D::vec2f size,
                      Texture& texture,
                      std::function<void(robot2D::ecs::Entity entity,
                                         std::uint64_t flags)>&& callback) {
        auto entity= scene.createEntity();
        auto& transform = entity.addComponent<robot2D::TransformComponent>();
        transform.setPosition(position);
        transform.setSize(size);

        auto& drawable = entity.addComponent<robot2D::DrawableComponent>();
        drawable.setTexture(texture);

        entity.addComponent<robot2D::UIHitbox>().callbackIDs[robot2D::UIHitbox::CallbackID::MouseDown] =
                scene.getSystem<robot2D::UISystem>()
                        -> addMousePressedCallback(std::move(callback));

        auto Bounds = entity.getComponent<robot2D::TransformComponent>().getGlobalBounds();
        Bounds.lx = Bounds.lx - Bounds.width / 2;
        Bounds.ly = Bounds.ly - Bounds.height / 2;
        entity.getComponent<robot2D::UIHitbox>().m_area = Bounds;

        return entity;
    }
}