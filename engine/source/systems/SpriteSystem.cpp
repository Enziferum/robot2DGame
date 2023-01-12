#include <robot2DGame/systems/SpriteSystem.hpp>
#include <robot2D/Ecs/EntityManager.hpp>

#include <robot2DGame/components/DrawableComponent.hpp>
#include <robot2DGame/components/SpriteComponent.hpp>

namespace robot2D {

    SpriteSystem::SpriteSystem(MessageBus& messageBus): robot2D::ecs::System(messageBus, typeid(SpriteSystem)) {
        addRequirement<DrawableComponent>();
        addRequirement<SpriteComponent>();
    }

    void SpriteSystem::update(float dt) {
        (void)dt;
        auto convertToGL = [](float value, float textureSize) {
            return static_cast<float>(value / textureSize);
        };

        for(auto& entity: m_entities) {
            auto& sprite = entity.getComponent<SpriteComponent>();
            if(sprite.m_needUpdate) {
                auto& drawable = entity.getComponent<DrawableComponent>();
                const auto* texture = sprite.getTexture();
                vec2u tx_s{};
                if(texture) {
                    drawable.setTexture(*texture);
                    tx_s = sprite.getTexture() -> getSize();
                }
                else
                    tx_s = drawable.getTexture().getSize();

                drawable.setColor(sprite.getColor());
                auto& vertices = drawable.getVertices();
                auto textureRect = sprite.getTextureRect();


                if(textureRect.lx + textureRect.width > tx_s.x) {

                }

                if(textureRect.ly + textureRect.height > tx_s.y) {

                }

                float left  = textureRect.lx;
                float right = left + textureRect.width;

                float bottom = textureRect.ly;
                float top = textureRect.ly + textureRect.height;

                vertices[0].texCoords = {convertToGL(left, static_cast<float>(tx_s.x)),
                                         convertToGL(bottom, static_cast<float>(tx_s.y))};
                vertices[1].texCoords = {convertToGL(right, static_cast<float>(tx_s.x)),
                                         convertToGL(bottom, static_cast<float>(tx_s.y))};
                vertices[2].texCoords = {convertToGL(right, static_cast<float>(tx_s.x)),
                                         convertToGL(top, static_cast<float>(tx_s.y))};
                vertices[3].texCoords = {convertToGL(left, static_cast<float>(tx_s.x)),
                                         convertToGL(top, static_cast<float>(tx_s.y))};

                sprite.m_needUpdate = false;
            }

        }

    }
} // namespace robot2D