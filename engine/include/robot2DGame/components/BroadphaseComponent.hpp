#pragma once

#include <cstdint>
#include <limits>
#include <robot2D/Graphics/Rect.hpp>

namespace robot2D {
    struct BroadPhaseComponent final {
        BroadPhaseComponent() = default;
        BroadPhaseComponent(robot2D::FloatRect area,
                            std::uint64_t flag = std::numeric_limits<std::uint64_t>::max()):
                m_bounds{area}, m_filterFlags{flag} {}

        inline void setArea(robot2D::FloatRect area);
        inline robot2D::FloatRect getArea() const;

        inline void setFilterFlags(std::uint64_t flags);
        inline std::uint64_t getFilterFlags() const;
    public:
        friend class DynamicTreeSystem;
        robot2D::FloatRect m_bounds;

        std::int32_t m_treeID = -1;
        std::uint64_t m_filterFlags = std::numeric_limits<std::uint64_t>::max();

        robot2D::vec2f m_lastWorldPosition = {};
    };

    inline void BroadPhaseComponent::setArea(robot2D::FloatRect area) { m_bounds = area;}
    inline robot2D::FloatRect BroadPhaseComponent::getArea() const {return m_bounds;}

    inline void BroadPhaseComponent::setFilterFlags(std::uint64_t flags) { m_filterFlags = flags; }
    inline std::uint64_t BroadPhaseComponent::getFilterFlags() const { return m_filterFlags; }
}