#include <robot2D/Graphics/RenderTarget.hpp>

#include <robot2DGame/Api.hpp>
#include <robot2DGame/systems/RendererSystem.hpp>

#include <game/World.hpp>
#include <game/systems/DummyPlayerSystem.hpp>
#include <game/components/PlayerComponent.hpp>
#include <game/components/Collision.hpp>

#include <game/Messages.hpp>
#include "systems/DynamicTreeSystem.hpp"

namespace {
    constexpr robot2D::vec2f startPos = {615.F, 435.F};
    constexpr robot2D::vec2f playerSize = {16.F, 16.F};

    static const robot2D::FloatRect PlayerBounds(-6.f, -10.f, 12.f, 10.f); //relative to player origin
    static const robot2D::FloatRect PlayerFoot(-5.5f, 0.f, 11.f, 4.f);
    static const robot2D::FloatRect PlayerLeftHand(-7.f, -10.f, 1.f, 12.f);
    static const robot2D::FloatRect PlayerRightHand(6.f, -10.f, 1.f, 12.f);
    //bounds = 0,0,16,16
}


World::World(robot2D::MessageBus& messageBus):
    m_bus{messageBus},
    m_scene{messageBus} {

}

void World::setup(InputParser& inputParser) {
    setupEcs();
    setupPlayer(inputParser);
    setupMap();
}

void World::setupEcs() {
    m_scene.addSystem<robot2D::RenderSystem>(m_bus);
    m_scene.addSystem<DynamicTreeSystem>(m_bus);
    m_scene.addSystem<DummyPlayerSystem>(m_bus);
}

template <typename T>
robot2D::Rect<T> combine(robot2D::Rect<T> a, robot2D::Rect<T> b)
{
    robot2D::Rect<T> ret(std::min(a.lx, b.lx),
                         std::min(a.ly, b.ly),
                         std::max(a.lx + a.width, b.lx + b.width),
                         std::max(a.ly + a.height, b.ly + b.height));
    ret.width -= ret.lx;
    ret.height -= ret.ly;

    return ret;
}

void World::setupPlayer(InputParser& inputParser) {
    auto player = robot2D::createEntity(m_scene,
                                        startPos,
                                        playerSize);

    player.getComponent<robot2D::DrawableComponent>().setColor(robot2D::Color::Red);
    player.addComponent<PlayerComponent>();

    auto bounds = player.getComponent<robot2D::TransformComponent>().getLocalBounds();
    /// bounds texture = 128 x 16
    player.getComponent<robot2D::TransformComponent>().setOrigin({ playerSize.x / 2.F, playerSize.y});

    auto& playerCollider = player.addComponent<Collider2D>();
    playerCollider.shapes[0].aabb = PlayerBounds;
    playerCollider.shapes[0].type = CollisionShape::Player;
    playerCollider.shapes[0].collisionFlags = CollisionGroup::PlayerFlags;

    playerCollider.shapes[1].aabb = PlayerFoot;
    playerCollider.shapes[1].type = CollisionShape::Foot;
    playerCollider.shapes[1].collisionFlags = CollisionGroup::PlayerFlags;

    playerCollider.shapes[2].aabb = PlayerLeftHand;
    playerCollider.shapes[2].type = CollisionShape::LeftHand;
    playerCollider.shapes[2].collisionFlags = CollisionGroup::PlayerFlags;

    playerCollider.shapes[3].aabb = PlayerRightHand;
    playerCollider.shapes[3].type = CollisionShape::RightHand;
    playerCollider.shapes[3].collisionFlags = CollisionGroup::PlayerFlags;

    playerCollider.shapeCount = 4;

    auto aabb = combine(PlayerBounds, PlayerFoot);
    aabb = combine(PlayerLeftHand, aabb);
    aabb = combine(PlayerRightHand, aabb);

    player.addComponent<robot2D::BroadPhaseComponent>().setArea(aabb);
    player.getComponent<robot2D::BroadPhaseComponent>().setFilterFlags(
            CollisionShape::Player | CollisionShape::Foot |
            CollisionShape::LeftHand | CollisionShape::RightHand
    );


    inputParser.setEntity(player);
}

void World::setupMap() {
    m_map.loadFromFile("res/map/1.map");

    for(const auto& block: m_map.getBlocks()) {
        auto entity = robot2D::createEntity(m_scene,
                                            block.position,
                                            block.size);

        auto& collider2D = entity.addComponent<Collider2D>();
        collider2D.shapes[0] = block.collisionShape;
        collider2D.shapeCount = 1;

        entity.addComponent<robot2D::BroadPhaseComponent>().setArea(collider2D.shapes[0].aabb);
        entity.getComponent<robot2D::BroadPhaseComponent>().setFilterFlags(block.collisionShape.type);
    }
}

void World::handleMessages(const robot2D::Message& message) {
    if(message.id == MessageID::AddPlatform) {
        auto data = message.getData<PlatformEvent>();

        auto entity = robot2D::createEntity(m_scene,
                                            data.position,
                                            data.size);

        CollisionShape collisionShape;
        collisionShape.shape = CollisionShape::Rectangle;
        collisionShape.aabb = {0, 0, data.size.x, data.size.y};
        collisionShape.type = CollisionShape::Solid;
        collisionShape.collisionFlags = CollisionShape::Player | CollisionShape::Foot | CollisionShape::LeftHand
                                        | CollisionShape::RightHand;

        auto& collider2D = entity.addComponent<Collider2D>();
        collider2D.shapes[0] = collisionShape;
        collider2D.shapeCount = 1;

        entity.addComponent<robot2D::BroadPhaseComponent>().setArea(collider2D.shapes[0].aabb);
        entity.getComponent<robot2D::BroadPhaseComponent>().setFilterFlags(collisionShape.type);
    }
}

void World::update(float dt) {
    m_scene.update(dt);
}

void World::draw(robot2D::RenderTarget& target,
                 robot2D::RenderStates states) const {
    target.draw(m_scene, states);
}
