#pragma once

#include <robot2DGame/components/BroadphaseComponent.hpp>

struct CollisionShape final
{
    //also used as flags for collision filtering
    enum Type
    {
        Solid = 0x1,
        Player = 0x2,
        Foot = 0x4,
        LeftHand = 0x8,
        RightHand = 0x10
    }type = Solid;

    //these are the types this shape collides with
    std::uint64_t collisionFlags = Player | Foot | LeftHand | RightHand;

    enum Shape
    {
        Rectangle, Polygon, Segment
    }shape = Rectangle;

    robot2D::FloatRect aabb;

    //used for map objects with properties such as NPC/Collectible type
    //or index into speech text file array
    std::int32_t ID = -1;
};

struct Collider2D {
    std::array<CollisionShape, 4u> shapes;
    std::size_t shapeCount = 0;
    std::uint16_t collisionFlags = 0; //types on this body which currently have collisions
};

namespace CollisionGroup
{
    static const std::uint64_t PlayerFlags =
            CollisionShape::Solid;
}
