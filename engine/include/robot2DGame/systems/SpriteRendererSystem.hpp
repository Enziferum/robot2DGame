#pragma once

#include <robot2D/Ecs/System.hpp>
#include <robot2D/Graphics/Drawable.hpp>
namespace robot2D {

    class SpriteRenderSystem: public ecs::System, public Drawable {
    public:
        SpriteRenderSystem();
        ~SpriteRenderSystem() override = default;

        void update(float dt) override;

    };
}