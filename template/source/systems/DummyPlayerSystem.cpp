#include <robot2DGame/components/TransformComponent.hpp>
#include <robot2D/Ecs/Scene.hpp>

#include <game/systems/DummyPlayerSystem.hpp>
#include <game/components/PlayerComponent.hpp>
#include <game/components/Collision.hpp>

#include <game/InputParser.hpp>
#include <game/Messages.hpp>

#include "DynamicTreeSystem.hpp"

namespace
{
    const float Gravity = 5999.9f / 3.F;
    const float JumpImpulse = 1480.f / 3.F;
    const float TerminalVelocity = 800.f;
}

struct Manifold final
{
    robot2D::vec2f normal;
    float penetration = 0.f;
};

robot2D::FloatRect boundsToWorldSpace(robot2D::FloatRect bounds,
                                      const robot2D::TransformComponent& tx) {
    auto scale = robot2D::vec2f {1.F, 1.F};
    bounds.lx *= scale.x;
    bounds.lx += tx.getOrigin().x * scale.x;
    bounds.width *= scale.x;
    bounds.ly *= scale.y;
    bounds.ly += tx.getOrigin().y * scale.y;
    bounds.height *= scale.y;
    bounds.lx += tx.getWorldPosition().x;
    bounds.ly += tx.getWorldPosition().y;
    return bounds;
}

bool intersectsAABB(robot2D::FloatRect a, robot2D::FloatRect b, Manifold& manifold)
{
    // a = 267.72 580.64 12 10
    // b = 100 580 640 10
    robot2D::FloatRect overlap;
    if(!a.intersects(b, overlap))
        return false;

    // 272.72, 585.64
    robot2D::vec2f centreA(a.lx + (a.width / 2.f), a.ly + (a.height / 2.f));
    // 420, 585
    robot2D::vec2f centreB(b.lx + (b.width / 2.f), b.ly + (b.height / 2.f));
    // 147,28 -0.64
    robot2D::vec2f collisionNormal = centreB - centreA;

    if (overlap.width < overlap.height)
    {
        manifold.normal.x = (collisionNormal.x < 0) ? 1.f : -1.f;
        manifold.penetration = overlap.width;
    }
    else
    {
        manifold.normal.y = (collisionNormal.y < 0 && (centreB.y > centreA.y)) ? 1.f : -1.f;
        manifold.penetration = overlap.height;
    }

    return true;
}

static inline float dot(const robot2D::vec2f& lv, const robot2D::vec2f& rv)
{
    return lv.x * rv.x + lv.y * rv.y;
}

////////////////////////////////////////////////////////////
template <typename T>
constexpr robot2D::Vector2<T> operator *(T left, const robot2D::Vector2<T>& right)
{
    return robot2D::Vector2<T>(right.x * left, right.y * left);
}

robot2D::vec2f reflect(robot2D::vec2f velocity, robot2D::vec2f normal) {
    return -2.f * dot(velocity, normal) * normal + velocity;
}


DummyPlayerSystem::DummyPlayerSystem(robot2D::MessageBus& messageBus):
robot2D::ecs::System(messageBus, typeid(DummyPlayerSystem)) {
    addRequirement<robot2D::TransformComponent>();
    addRequirement<PlayerComponent>();
}

void DummyPlayerSystem::update(float dt)  {

    for(auto& it: m_entities) {
        auto player = it.getComponent<PlayerComponent>();
        auto& tx = it.getComponent<robot2D::TransformComponent>();

        switch(player.state) {
            default:
                break;
            case PlayerComponent::Falling:
                processFalling(it, dt);
                break;
            case PlayerComponent::Running:
                processRunning(it, dt);
                break;
        }
    }

}

void DummyPlayerSystem::processFalling(robot2D::ecs::Entity entity, float dt) {
    auto& player = entity.getComponent<PlayerComponent>();
    auto& tx = entity.getComponent<robot2D::TransformComponent>();
    auto scale = tx.getScale();
    const auto& collision = entity.getComponent<Collider2D>();

    if ((player.input & InputFlag::Left)
        && (collision.collisionFlags & CollisionShape::LeftHand) == 0)
    {
        player.velocity.x -= PlayerComponent::Acceleration * player.accelerationMultiplier;
     //   scale.x = -scale.y;
    }

    if ((player.input & InputFlag::Right)
        && (collision.collisionFlags & CollisionShape::RightHand) == 0)
    {
        player.velocity.x += PlayerComponent::Acceleration * player.accelerationMultiplier;

        if ((player.input & InputFlag::Left) == 0)
        {
          //  scale.x = scale.y;
        }
    }
    tx.setScale(scale);

    if((player.prevInput & InputFlag::Jump)
       && (player.input & InputFlag::Jump) == 0)
    {
        player.velocity.y *= 0.15F;
    }

    if ((player.input & InputFlag::Shoot)
        && (player.prevInput & InputFlag::Shoot) == 0)
    {
        /// Send Message to create bullet or smth else
        auto* msg = m_messageBus.postMessage<PlayerEvent>(MessageID::PlayerMessage);
        msg -> type = PlayerEvent::Shot;
        msg -> entity = entity;
    }

    player.prevInput = player.input;

    //apply gravity
    float multiplier = (player.velocity.y < 0) ? 0.7f : 1.2f;
    player.velocity.y = std::min(player.velocity.y + Gravity * multiplier * dt, TerminalVelocity);

    applyVelocity(entity, dt);
    doCollision(entity);

    auto flags = entity.getComponent<Collider2D>().collisionFlags;
    if(flags & CollisionShape::Player)
        player.state = PlayerComponent::Running;
}

void DummyPlayerSystem::processRunning(robot2D::ecs::Entity entity, float dt) {
    auto& player = entity.getComponent<PlayerComponent>();
    auto& tx = entity.getComponent<robot2D::TransformComponent>();
    auto scale = tx.getScale();

    if ((player.input & InputFlag::Left))
    {
        player.velocity.x -= PlayerComponent::Acceleration * player.accelerationMultiplier;
       // scale.x = -scale.y;
    }

    if ((player.input & InputFlag::Right))
    {
        player.velocity.x += PlayerComponent::Acceleration * player.accelerationMultiplier;

        if ((player.input & InputFlag::Left) == 0)
        {
          //   scale.x = scale.y;
        }
    }

    tx.setScale(scale);

    if ((player.input & InputFlag::Jump)
        && (player.prevInput & InputFlag::Jump) == 0)
    {
        player.velocity.y -= JumpImpulse;
        player.state = PlayerComponent::Falling;
    }

    if ((player.input & InputFlag::Shoot)
        && (player.prevInput & InputFlag::Shoot) == 0)
    {
    }

    player.prevInput = player.input;

    applyVelocity(entity, dt);
    doCollision(entity, true);

    auto flags = entity.getComponent<Collider2D>().collisionFlags;
    if((flags & CollisionShape::Foot) == 0) {
        player.state = PlayerComponent::Falling;
    }

}

void DummyPlayerSystem::applyVelocity(robot2D::ecs::Entity entity, float dt)
{
    auto& tx = entity.getComponent<robot2D::TransformComponent>();
    auto& player = entity.getComponent<PlayerComponent>();

    tx.move(player.velocity * dt);
    player.velocity.x *= PlayerComponent::Drag;
}

void DummyPlayerSystem::doCollision(robot2D::ecs::Entity entity, bool debug) {
    auto& transform = entity.getComponent<robot2D::TransformComponent>();
    entity.getComponent<Collider2D>().collisionFlags = 0;

    // broad phase
    auto area = entity.getComponent<robot2D::BroadPhaseComponent>().getArea();
    auto queryArea = boundsToWorldSpace(area, transform);

    auto nearby = getScene() -> getSystem<DynamicTreeSystem>() -> query(queryArea,
                                                                        CollisionGroup::PlayerFlags);

    for(auto other: nearby) {
        if(other == entity)
            continue;
        auto otherBounds = boundsToWorldSpace(other.getComponent<robot2D::BroadPhaseComponent>().getArea(),
                                              other.getComponent<robot2D::TransformComponent>());
        if(otherBounds.intersects(queryArea))
            resolveCollision(entity, other, otherBounds);
    }
}

void DummyPlayerSystem::resolveCollision(robot2D::ecs::Entity entity,
                                         robot2D::ecs::Entity other,
                                         robot2D::FloatRect otherBounds) {
    auto& tx = entity.getComponent<robot2D::TransformComponent>();
    auto& collider2D = entity.getComponent<Collider2D>();
    auto& player = entity.getComponent<PlayerComponent>();

    for(auto it = 0u; it < collider2D.shapeCount; ++it) {
        auto bounds = collider2D.shapes[it].aabb;
        bounds = boundsToWorldSpace(bounds, tx);

        Manifold manifold;
        if(!intersectsAABB(bounds, otherBounds, manifold))
            continue;

        auto& otherCollider2D = other.getComponent<Collider2D>();

        if(otherCollider2D.shapes[0].type & CollisionShape::Solid)
            collider2D.collisionFlags |= collider2D.shapes[it].type;

        if (collider2D.shapes[it].type == CollisionShape::Player) {

            switch (otherCollider2D.shapes[0].type) {
                default:
                    break;
                case CollisionShape::Solid:
                    tx.move(manifold.normal * manifold.penetration);

                  if (manifold.normal.y != 0) {
                        player.velocity = {};
                    } else {
                        player.velocity = reflect(player.velocity, manifold.normal);
                    }
                    break;
            }
        }
    }
}
