#include <robot2DGame/Api.hpp>
#include <robot2DGame/systems/UISystem.hpp>

#include <robot2DGame/components/UIHitbox.hpp>
#include <robot2DGame/components/TextComponent.hpp>

namespace robot2D {
    robot2D::ecs::Entity
    createEntity(robot2D::ecs::Scene& scene,
                 vec2f position,
                 vec2f size) {
        auto entity= scene.createEntity();
        auto& transform = entity.addComponent<robot2D::TransformComponent>();
        transform.setPosition(position);
        transform.setSize(size);

        auto& drawable = entity.addComponent<robot2D::DrawableComponent>();

        return entity;
    }

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

    robot2D::ecs::Entity createLabel(robot2D::ecs::Scene& scene,
                                     robot2D::vec2f position,
                                     std::string text,
                                     const robot2D::Font& font) {

        auto textCopy = text;
        auto textSize = font.calculateSize(std::move(textCopy));

        auto entity = scene.createEntity();
        auto& transform= entity.addComponent<robot2D::TransformComponent>();
        position -= textSize / 2.F;
        transform.setPosition(position);

        entity.addComponent<robot2D::DrawableComponent>();
        auto& textComp = entity.addComponent<robot2D::TextComponent>();
        textComp.setFont(font);
        textComp.setText(text);

        return entity;
    }

    robot2D::ecs::Entity createButton(robot2D::ecs::Scene& scene,
                                      robot2D::vec2f position,
                                      robot2D::vec2f size,
                                      std::string text,
                                      const robot2D::Font& font,
                                      std::function<void(robot2D::ecs::Entity entity,
                                                         std::uint64_t flags)>&& callback) {
        auto entity= scene.createEntity();
        auto& transform = entity.addComponent<robot2D::TransformComponent>();
        transform.setPosition(position);
        transform.setSizeScale(size);

        auto& drawable = entity.addComponent<robot2D::DrawableComponent>();

        entity.addComponent<robot2D::UIHitbox>().callbackIDs[robot2D::UIHitbox::CallbackID::MouseDown] =
                scene.getSystem<robot2D::UISystem>()
                        -> addMousePressedCallback(std::move(callback));

        auto Bounds = entity.getComponent<robot2D::TransformComponent>().getGlobalBounds();
        Bounds.lx = Bounds.lx;
        Bounds.ly = Bounds.ly;
        entity.getComponent<robot2D::UIHitbox>().m_area = Bounds;

        transform.setSize(size);

        auto textCopy = text;

        auto textPos = robot2D::vec2f {
            position.x + size.x / 2.F,
            position.y + size.y / 2.F
        };

        auto childLabel = createLabel(scene, textPos, std::move(text), font);

        return entity;
    }

    robot2D::ecs::Entity
    createImageButton(ecs::Scene& scene,
                      robot2D::vec2f position,
                      robot2D::vec2f size,
                      Texture& texture,
                      std::string text,
                      const robot2D::Font& font,
                      std::function<void(robot2D::ecs::Entity entity,
                                         std::uint64_t flags)>&& callback) {
        auto entity = createButton(scene, position, size, std::move(text), font, std::move(callback));
        entity.getComponent<robot2D::DrawableComponent>().setTexture(texture);
        return entity;
    }
}