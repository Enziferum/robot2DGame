#pragma once
#include <robot2D/Ecs/System.hpp>
#include <robot2D/Graphics/Rect.hpp>

//node struct used by the tree
struct TreeNode final
{
    static constexpr std::int32_t Null = -1;
    bool isLeaf() const
    {
        return childA == Null;
    }

    //this is in world coordinates
    robot2D::FloatRect fatBounds;
    robot2D::ecs::Entity entity;

    union
    {
        std::int32_t parent;
        std::int32_t next;
    };

    std::int32_t childA = Null;
    std::int32_t childB = Null;

    //leaf == 0, else Null if free
    std::int32_t height = Null;
};

class DynamicTreeSystem: public robot2D::ecs::System {
public:
    DynamicTreeSystem(robot2D::MessageBus& messageBus);
    ~DynamicTreeSystem() override = default;

    void update(float dt) override;
    std::vector<robot2D::ecs::Entity> query(robot2D::FloatRect rect, uint64_t mask);

    void onEntityAdded(robot2D::ecs::Entity entity) override;
    void onEntityRemoved(robot2D::ecs::Entity entity);
private:
    std::int32_t addToTree(robot2D::ecs::Entity);
    void removeFromTree(std::int32_t);
    //moves a proxy with the specified treeID. If the entity
    //has moved outside of the node's fattened AABB then it
    //is removed from the tree and reinsterted.
    bool moveNode(std::int32_t, robot2D::FloatRect, robot2D::vec2f);
    robot2D::FloatRect getFatAABB(std::int32_t) const;
    std::int32_t getMaxBalance() const;
    float getAreaRatio() const;

    std::int32_t allocateNode();
    void freeNode(std::int32_t);

    void insertLeaf(std::int32_t);
    void removeLeaf(std::int32_t);

    std::int32_t balance(std::int32_t);

    std::int32_t computeHeight() const;
    std::int32_t computeHeight(std::int32_t) const;

    void validateStructure(std::int32_t) const;
    void validateMetrics(std::int32_t) const;

    std::int32_t m_root;

    std::size_t m_nodeCount;
    std::size_t m_nodeCapacity;
    std::vector<TreeNode> m_nodes;

    std::int32_t m_freeList; //must be signed!

    std::size_t m_path;

    std::size_t m_insertionCount;
};

//growable stack using preallocated memory
namespace Detail
{
    template <typename T, std::size_t SIZE>
    class FixedStack final
    {
    public:

        T pop()
        {
           // XY_ASSERT(m_size != 0, "Stack is empty!");
            m_size--;
            return m_data[m_size];
        }

        void push(T data)
        {
          //  XY_ASSERT(m_size < m_data.size(), "Stack is full!");

            m_data[m_size++] = data;
        }

        std::size_t size() const
        {
            return m_size;
        }

    private:
        std::array<T, SIZE> m_data;
        std::size_t m_size = 0; //current size / next free index
    };
}
