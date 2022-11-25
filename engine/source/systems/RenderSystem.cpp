#include <robot2D/Graphics/RenderTarget.hpp>
#include <robot2D/Ecs/EntityManager.hpp>

#include <robot2DGame/systems/RendererSystem.hpp>
#include <robot2DGame/systems/TextSystem.hpp>

#include <robot2DGame/components/DrawableComponent.hpp>
#include <robot2DGame/components/TransformComponent.hpp>
#include <robot2DGame/components/TextComponent.hpp>

namespace robot2D {
    RenderSystem::RenderSystem(robot2D::MessageBus& messageBus):
            ecs::System(messageBus,typeid(RenderSystem)),
            m_needUpdateZBuffer{false} {
        addRequirement<TransformComponent>();
        addRequirement<DrawableComponent>();
    }

    void RenderSystem::update(float dt) {
        (void)dt;

        for(auto& it: m_entities) {
            auto& drawable = it.getComponent<DrawableComponent>();
            if(drawable.m_needUpdateZbuffer) {
                m_needUpdateZBuffer = true;
                drawable.m_needUpdateZbuffer = false;
            }
        }

        if(m_needUpdateZBuffer) {
            std::sort(m_entities.begin(), m_entities.end(),
                      [](const ecs::Entity &left, const ecs::Entity &right) {
                return left.getComponent<DrawableComponent>().getDepth() <
                        right.getComponent<DrawableComponent>().getDepth();
            });

            m_needUpdateZBuffer = false;
        }
    }

    void RenderSystem::draw(RenderTarget& target, RenderStates states) const {
        for(auto& it: m_entities) {
            auto& transform = it.getComponent<TransformComponent>();
            auto& drawable = it.getComponent<DrawableComponent>();

            auto t = transform.getTransform();

            robot2D::RenderStates renderStates;
            renderStates.transform *= t;
            renderStates.texture = &drawable.getTexture();
            renderStates.color = drawable.getColor();
            renderStates.layerID = drawable.getLayerIndex();

            if(getScene() -> hasSystem<TextSystem>() && it.hasComponent<TextComponent>()) {
                auto textSystem = getScene() -> getSystem<TextSystem>();
                auto vertexArray =  textSystem -> getVertexArray();
                renderStates.shader = const_cast<robot2D::ShaderHandler*>(&textSystem -> getShader());
                renderStates.shader -> use();
                auto view = target.getView(drawable.getLayerIndex()).getTransform().get_matrix();
                renderStates.shader -> setMatrix("projection",
                                                 const_cast<float *>(view));
                renderStates.shader -> unUse();
                renderStates.renderInfo.indexCount = textSystem -> getIndexCount();
                target.draw(vertexArray, renderStates);
            }
            else
                target.draw(renderStates);
        }
    }
}