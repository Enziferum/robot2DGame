#include <robot2DGame/components/TransformComponent.hpp>

namespace robot2D {
    TransformComponent::TransformComponent():
        m_parent{nullptr} {}

    void TransformComponent::addChild(TransformComponent& child) {
        if(child.m_parent) {
            if(child.m_parent == this)
                return;
            // todo remove children
        }
        child.m_parent = this;
        m_children.push_back(&child);
    }

    void TransformComponent::removeChild(TransformComponent& child) {}

}