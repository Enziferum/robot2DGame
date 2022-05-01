#include <game/Map.hpp>
#include <game/components/Collision.hpp>

namespace {
    constexpr robot2D::vec2f windowSize = {800.F, 600.F};
    constexpr robot2D::vec2f offset = {240.F, 160.F};

    constexpr float wallSize = 10.F;

    struct SampleWall {
        robot2D::vec2f position;
        robot2D::vec2f size;
    };

    std::array<SampleWall, 4> g_walls;
}

Map::Map(): m_mapblocks{} {}

void Map::sampleMap() {
    g_walls[0] = { {offset}, {windowSize.x, wallSize}};
    g_walls[1] = { {offset}, {wallSize, windowSize.y}};
    g_walls[2] = { {offset.x, windowSize.y + offset.y},
                   {windowSize.x, wallSize}};
    g_walls[3] = { {windowSize.x + offset.x, offset.y},
                   {wallSize, windowSize.y + wallSize}};
}

bool Map::loadFromFile(const std::string& path) {
    // emulate loading
    sampleMap();

    for(const auto& wall: g_walls) {
        CollisionShape collisionShape;
        collisionShape.shape = CollisionShape::Rectangle;
        collisionShape.aabb = {0, 0, wall.size.x, wall.size.y};
        collisionShape.type = CollisionShape::Solid;
        collisionShape.collisionFlags = CollisionShape::Player | CollisionShape::Foot;

        MapBlock block;
        block.collisionShape = collisionShape;
        block.position = wall.position;
        block.size = wall.size;

        m_mapblocks.emplace_back(std::move(block));
    }

    return true;
}

const std::vector<MapBlock>& Map::getBlocks() const {
    return m_mapblocks;
}


