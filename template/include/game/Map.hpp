#pragma once

#include <vector>
#include <string>

#include <game/components/Collision.hpp>

struct MapBlock {
    robot2D::vec2f position;
    robot2D::vec2f size;
    CollisionShape collisionShape;
};

class Map {
public:
    Map();
    ~Map() = default;

    bool loadFromFile(const std::string& path);
    const std::vector<MapBlock>& getBlocks() const;

private:
    void sampleMap();
private:
    std::vector<MapBlock> m_mapblocks;
};