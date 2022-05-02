#include <robot2D/Graphics/RenderTarget.hpp>

#include <robot2DGame/Api.hpp>
#include <robot2DGame/systems/RendererSystem.hpp>

#include <game/World.hpp>
#include <game/systems/DummyPlayerSystem.hpp>
#include <game/components/PlayerComponent.hpp>
#include <game/components/Collision.hpp>

#include "systems/DynamicTreeSystem.hpp"

namespace {
    constexpr robot2D::vec2f startPos = {615.F, 435.F};
    constexpr robot2D::vec2f playerSize = {50.F, 50.F};
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
    m_scene.addSystem<DummyPlayerSystem>(m_bus);
    m_scene.addSystem<DynamicTreeSystem>(m_bus);
}

void World::setupPlayer(InputParser& inputParser) {
    auto player = robot2D::createEntity(m_scene,
                                        startPos,
                                        playerSize);
    player.getComponent<robot2D::DrawableComponent>().setColor(robot2D::Color::Green);
    player.addComponent<PlayerComponent>();

    auto& playerCollider = player.addComponent<Collider2D>();
    playerCollider.shapes[0].aabb = {0, 0, playerSize.x / 2.F, playerSize.y};
    playerCollider.shapes[0].type = CollisionShape::Player;
    playerCollider.shapes[0].collisionFlags = CollisionGroup::PlayerFlags;

    playerCollider.shapeCount = 1;

    player.addComponent<robot2D::BroadPhaseComponent>().setArea({0, 0, playerSize.x, playerSize.y});
    player.getComponent<robot2D::BroadPhaseComponent>().setFilterFlags(CollisionShape::Player | CollisionShape::Foot);
    player.getComponent<robot2D::TransformComponent>().setOrigin({playerSize.x / 2.F,
                                                                  playerSize.y / 2.F});

    auto& playerComp = player.getComponent<PlayerComponent>();

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

}

void World::update(float dt) {
    m_scene.update(dt);
}

void World::draw(robot2D::RenderTarget& target,
                 robot2D::RenderStates states) const {
    target.draw(m_scene, states);
}
