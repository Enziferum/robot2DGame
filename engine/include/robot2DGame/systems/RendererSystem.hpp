#pragma once

#include <robot2D/Ecs/System.hpp>
#include <robot2D/Graphics/Drawable.hpp>

namespace robot2D {

    class RenderSystem: public ecs::System, public Drawable {
    public:
        RenderSystem(robot2D::MessageBus& messageBus);
        ~RenderSystem() override = default;

        void update(float dt) override;
        void draw(RenderTarget& target, RenderStates states) const override;

    private:
        bool m_needUpdateZBuffer;
    };


}