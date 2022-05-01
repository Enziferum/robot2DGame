#include "DynamicTreeSystem.hpp"
#include "robot2DGame/components/TransformComponent.hpp"
#include <game/components/Collision.hpp>
#include <robot2D/Ecs/EntityManager.hpp>

namespace
{
    const float FattenAmount = 10.f; //this assumes approximately 1px / cm in world scale
    const float DisplacementMultiplier = 2.f;
}

namespace xy
{
    namespace Util
    {
        /*!
        \brief Functions for manipulating rectangle types
        */
        namespace Rectangle
        {
            /*!
            \brief Returns true if the second given Rect is fully contained within the first
            */
            template <typename T>
            bool contains(const robot2D::Rect<T>& first, const robot2D::Rect<T>& second)
            {
                if (second.lx < first.lx) return false;
                if (second.ly < first.ly) return false;
                if (second.lx + second.width > first.lx + first.width) return false;
                if (second.ly + second.height > first.ly + first.height) return false;

                return true;
            }

            /*!
            \brief Returns the centre point of a given Rect
            */
            template <typename T>
            robot2D::Vector2<T> centre(const robot2D::Rect<T>& rect)
            {
                return robot2D::Vector2<T>(rect.left + (rect.width / 2.f), rect.top + (rect.height / 2.f));
            }

            /*!
            \brief Returns a new Rect from the given upper and lower bounds.
            */
            template <typename T>
            robot2D::Rect<T> fromBounds(const robot2D::Vector2<T>& lower, const robot2D::Vector2<T>& upper)
            {
                return robot2D::Rect<T>(lower.x, lower.y, upper.x - lower.x, upper.y - lower.y);
            }

            /*!
            \brief Returns the total perimeter of the given rectangle
            */
            template <typename T>
            T getPerimeter(robot2D::Rect<T> rect)
            {
                return 2 * (rect.width + rect.height);
            }

            /*!
            \brief Merges 2 given rectangles and returns the result
            */
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
        }
    }
}


DynamicTreeSystem::DynamicTreeSystem(robot2D::MessageBus& messageBus):
        robot2D::ecs::System(messageBus, typeid(DynamicTreeSystem)),
        m_root          (TreeNode::Null),
        m_nodeCount     (0),
        m_nodeCapacity  (64),
        m_nodes         (m_nodeCapacity),
        m_freeList      (0),
        m_path          (0),
        m_insertionCount(0)
        {
            addRequirement<robot2D::BroadPhaseComponent>();
            addRequirement<robot2D::TransformComponent>();

            for (auto i = 0u; i < m_nodeCapacity - 1; ++i)
            {
                m_nodes[i].next = static_cast<std::int32_t>(i + 1);
                m_nodes[i].height = -1;
            }
            m_nodes.back().next = TreeNode::Null;
            m_nodes.back().height = -1;
}

void DynamicTreeSystem::update(float dt) {
    for (auto entity : m_entities)
    {

        auto& bpc = entity.getComponent<robot2D::BroadPhaseComponent>();
        const auto& tx = entity.getComponent<robot2D::TransformComponent>();
        auto worldPosition = tx.getWorldPosition();
        auto worldBounds = bpc.m_bounds;

        worldBounds.lx += tx.getOrigin().x;
        worldBounds.ly += tx.getOrigin().y;

        worldBounds = tx.getWorldTransform().transformRect(worldBounds);

        moveNode(bpc.m_treeID, worldBounds, worldPosition - bpc.m_lastWorldPosition);

        bpc.m_lastWorldPosition = worldPosition;

    }
}

std::vector<robot2D::ecs::Entity> DynamicTreeSystem::query(robot2D::FloatRect area, uint64_t filter) {
    Detail::FixedStack<std::int32_t, 256> stack;
    stack.push(m_root);

    std::vector<robot2D::ecs::Entity> retVal;
    retVal.reserve(256);

    while (stack.size() > 0)
    {
        auto treeID = stack.pop();
        if (treeID == TreeNode::Null)
        {
            continue;
        }

        const auto& node = m_nodes[treeID];
        if (area.intersects(node.fatBounds))
        {
            //&& node.entity.
            //TODO it would be nice to precache the filter fetch, but it would miss changes at the component level
            if (node.isLeaf()
                && (node.entity.getComponent<robot2D::BroadPhaseComponent>().m_filterFlags & filter))
            {
                //we have a candidate, stash
                retVal.push_back(node.entity);
            }
            else
            {
                stack.push(node.childA);
                stack.push(node.childB);
            }
        }
    }
    return retVal;
}

void DynamicTreeSystem::onEntityAdded(robot2D::ecs::Entity entity)
{
    entity.getComponent<robot2D::BroadPhaseComponent>().m_treeID = addToTree(entity);
}

void DynamicTreeSystem::onEntityRemoved(robot2D::ecs::Entity entity)
{
    removeFromTree(entity.getComponent<robot2D::BroadPhaseComponent>().m_treeID);
}

//private
std::int32_t DynamicTreeSystem::addToTree(robot2D::ecs::Entity entity)
{
    auto treeID = allocateNode();

    const auto& tx = entity.getComponent<robot2D::TransformComponent>();
    auto bounds = entity.getComponent<robot2D::BroadPhaseComponent>().m_bounds;

    bounds.lx += tx.getOrigin().x;
    bounds.ly += tx.getOrigin().y;
    bounds = tx.getWorldTransform().transformRect(bounds);


    //fatten AABB
    bounds.lx -= FattenAmount;
    bounds.ly -= FattenAmount;
    bounds.width += (FattenAmount * 2.f);
    bounds.height += (FattenAmount * 2.f);

    m_nodes[treeID].fatBounds = bounds;
    m_nodes[treeID].entity = entity;
    m_nodes[treeID].height = 0;

    insertLeaf(treeID);

    return treeID;
}

void DynamicTreeSystem::removeFromTree(std::int32_t treeID)
{
  //  XY_ASSERT(treeID > -1 && treeID < m_nodeCapacity, "Invalid tree id");
  //  XY_ASSERT(m_nodes[treeID].isLeaf(), "Not a leaf node!");

    removeLeaf(treeID);
    freeNode(treeID);
}

bool DynamicTreeSystem::moveNode(std::int32_t treeID, robot2D::FloatRect worldArea, robot2D::vec2f displacement)
{
   // XY_ASSERT(treeID > -1 && treeID < m_nodeCapacity, "Invalid tree id");
  //  XY_ASSERT(m_nodes[treeID].isLeaf(), "Not a leaf node!");

    if (xy::Util::Rectangle::contains(m_nodes[treeID].fatBounds, worldArea))
    {
        return false;
    }

    removeLeaf(treeID);

    //expand the new aabb and reinsert in tree
    worldArea.lx -= FattenAmount;
    worldArea.ly -= FattenAmount;
    worldArea.width += (FattenAmount * 2.f);
    worldArea.height += (FattenAmount * 2.f);

    //displacment prediction
    displacement.x *= DisplacementMultiplier;
    displacement.y *= DisplacementMultiplier;

    if (displacement.x < 0) //not really understanding the original here, so quite possibly creating a bug!
    {
        worldArea.lx += displacement.x;
    }
    else
    {
        worldArea.width += displacement.x;
    }

    if (displacement.y < 0)
    {
        worldArea.ly += displacement.y;
    }
    else
    {
        worldArea.height += displacement.y;
    }

    //reinsert
    m_nodes[treeID].fatBounds = worldArea;
    insertLeaf(treeID);

    return true;
}

robot2D::FloatRect DynamicTreeSystem::getFatAABB(std::int32_t treeID) const
{
   // XY_ASSERT(treeID > -1 && treeID < m_nodeCapacity, "Invalid tree id");
    return m_nodes[treeID].fatBounds;
}

std::int32_t DynamicTreeSystem::getMaxBalance() const
{
    std::int32_t maxBalance = 0;

    for (auto i = 0u; i < m_nodeCapacity; ++i)
    {
        const auto& node = m_nodes[i];
        if (node.height <= 1)
        {
            continue;
        }

      //  XY_ASSERT(!node.isLeaf(), "We shouldn't be at the end!");

        auto balance = std::abs(m_nodes[node.childB].height - m_nodes[node.childA].height);
        maxBalance = std::max(balance, maxBalance);
    }
    return maxBalance;
}

float DynamicTreeSystem::getAreaRatio() const
{
    if (m_root == TreeNode::Null)
    {
        return 0.f;
    }

    const auto& rootNode = m_nodes[m_root];
    auto rootArea = xy::Util::Rectangle::getPerimeter(rootNode.fatBounds);

    float totalArea = 0.f;
    for (auto i = 0u; i < m_nodeCapacity; ++i)
    {
        const auto& node = m_nodes[i];
        if (node.height > -1) //not a free node
        {
            totalArea += xy::Util::Rectangle::getPerimeter(node.fatBounds);
        }
    }

    return totalArea / rootArea;
}

std::int32_t DynamicTreeSystem::allocateNode()
{
    //grow the node list if full
    if (m_freeList == TreeNode::Null)
    {
        //XY_ASSERT(m_nodeCount == m_nodeCapacity, "List not actually full?");

        m_nodeCapacity *= 2;
        m_nodes.resize(m_nodeCapacity);

       // LOG("Resized tree capacity to " + std::to_string(m_nodeCapacity), xy::Logger::Type::Info);

        //update the linked list for the new capacity
        for (auto i = m_nodeCount; i < m_nodeCapacity - 1; ++i)
        {
            m_nodes[i].next = static_cast<std::int32_t>(i + 1);
            m_nodes[i].height = -1;
        }
        m_nodes.back().next = TreeNode::Null;
        m_nodes.back().height = -1;
        m_freeList = static_cast<std::int32_t>(m_nodeCount); //we doubled therefore this is how many are free
    }

    //set next node as free - remember we're recycling these so can't rely on default values
    auto treeID = m_freeList;
    m_freeList = m_nodes[treeID].next;

    m_nodes[treeID].parent = TreeNode::Null;
    m_nodes[treeID].childA = TreeNode::Null;
    m_nodes[treeID].childB = TreeNode::Null;
    m_nodes[treeID].height = 0;
    m_nodes[treeID].entity = {};
    m_nodeCount++;

    return treeID;
}

void DynamicTreeSystem::freeNode(std::int32_t treeID)
{
    //XY_ASSERT(treeID > TreeNode::Null && treeID < m_nodeCapacity, "Invalid tree id");
    //XY_ASSERT(m_nodeCount > 0, "No nodes exist to free");

    m_nodes[treeID].next = m_freeList;
    m_nodes[treeID].height = -1;
    m_freeList = treeID;
    m_nodeCount--;
}

void DynamicTreeSystem::insertLeaf(std::int32_t treeID)
{
    m_insertionCount++;

    if (m_root == TreeNode::Null)
    {
        m_root = treeID;
        m_nodes[m_root].parent = TreeNode::Null;
        return;
    }

    //walk the tree for a suitable leaf position
    auto leafBounds = m_nodes[treeID].fatBounds;
    auto index = m_root;

    while (!m_nodes[index].isLeaf())
    {
        auto childA = m_nodes[index].childA;
        auto childB = m_nodes[index].childB;

        float perimeter = xy::Util::Rectangle::getPerimeter(m_nodes[index].fatBounds);

        auto combinedAABB = xy::Util::Rectangle::combine(m_nodes[index].fatBounds, leafBounds);
        auto combinedPerimeter = xy::Util::Rectangle::getPerimeter(combinedAABB);

        //cost of creating a new node / parent for the leaf
        float cost = 2.f * combinedPerimeter;

        //minimum cost for pushing the leaf down the tree
        float inheritedCost = 2.f * (combinedPerimeter - perimeter);

        //cost of descending to childA
        float costA = 0.f;
        if (m_nodes[childA].isLeaf())
        {
            auto bounds = xy::Util::Rectangle::combine(leafBounds, m_nodes[childA].fatBounds);
            costA = xy::Util::Rectangle::getPerimeter(bounds) + inheritedCost;
        }
        else
        {
            auto bounds = xy::Util::Rectangle::combine(leafBounds, m_nodes[childA].fatBounds);
            auto oldPerimeter = xy::Util::Rectangle::getPerimeter(m_nodes[childA].fatBounds);
            auto newPerimenter = xy::Util::Rectangle::getPerimeter(bounds);
            costA = (newPerimenter - oldPerimeter) + inheritedCost;
        }


        //cost of descending to childB
        float costB = 0.f;
        if (m_nodes[childB].isLeaf())
        {
            auto bounds = xy::Util::Rectangle::combine(leafBounds, m_nodes[childB].fatBounds);
            costB = xy::Util::Rectangle::getPerimeter(bounds) + inheritedCost;
        }
        else
        {
            auto bounds = xy::Util::Rectangle::combine(leafBounds, m_nodes[childB].fatBounds);
            auto oldPerimeter = xy::Util::Rectangle::getPerimeter(m_nodes[childB].fatBounds);
            auto newPerimenter = xy::Util::Rectangle::getPerimeter(bounds);
            costB = (newPerimenter - oldPerimeter) + inheritedCost;
        }

        //and descend according to least cost
        if (cost < costA && cost < costB)
        {
            break; //we arrived!
        }

        if (costA < costB)
        {
            index = childA;
        }
        else
        {
            index = childB;
        }
    }

    auto sibling = index;

    //create new parent
    auto oldParent = m_nodes[sibling].parent;
    auto newParent = allocateNode();
    m_nodes[newParent].parent = oldParent;
    m_nodes[newParent].entity = {};
    m_nodes[newParent].fatBounds = xy::Util::Rectangle::combine(leafBounds, m_nodes[sibling].fatBounds);
    m_nodes[newParent].height = m_nodes[sibling].height + 1;

    if (oldParent != TreeNode::Null)
    {
        //we're not attaching to the root
        if (m_nodes[oldParent].childA == sibling)
        {
            m_nodes[oldParent].childA = newParent;
        }
        else
        {
            m_nodes[oldParent].childB = newParent;
        }

        m_nodes[newParent].childA = sibling;
        m_nodes[newParent].childB = treeID;
        m_nodes[sibling].parent = newParent;
        m_nodes[treeID].parent = newParent;
    }
    else
    {
        m_nodes[newParent].childA = sibling;
        m_nodes[newParent].childB = treeID;
        m_nodes[sibling].parent = newParent;
        m_nodes[treeID].parent = newParent;
        m_root = newParent;
    }

    //walk back up the tree updating heights and bounds
    index = m_nodes[treeID].parent;
    while (index != TreeNode::Null)
    {
        index = balance(index);

        auto childA = m_nodes[index].childA;
        auto childB = m_nodes[index].childB;

       // XY_ASSERT(childA != TreeNode::Null, "Can't be null");
       // XY_ASSERT(childB != TreeNode::Null, "Can't be null");

        m_nodes[index].height = std::max(m_nodes[childA].height, m_nodes[childB].height);
        m_nodes[index].fatBounds = xy::Util::Rectangle::combine(m_nodes[childA].fatBounds, m_nodes[childB].fatBounds);

        index = m_nodes[index].parent;
    }
}

void DynamicTreeSystem::removeLeaf(std::int32_t treeID)
{
    if (treeID == m_root)
    {
        m_root = TreeNode::Null;
        return;
    }

    auto parent = m_nodes[treeID].parent;
    auto grandparent = m_nodes[parent].parent;
    auto sibling = TreeNode::Null;

    if (m_nodes[parent].childA == treeID)
    {
        sibling = m_nodes[parent].childB;
    }
    else
    {
        sibling = m_nodes[parent].childA;
    }

    if (grandparent != TreeNode::Null)
    {
        if (m_nodes[grandparent].childA == parent)
        {
            m_nodes[grandparent].childA = sibling;
        }
        else
        {
            m_nodes[grandparent].childB = sibling;
        }
        m_nodes[sibling].parent = grandparent;
        freeNode(parent);

        //update bounds
        auto index = grandparent;
        while (index != TreeNode::Null)
        {
            index = balance(index);

            auto childA = m_nodes[index].childA;
            auto childB = m_nodes[index].childB;

            m_nodes[index].fatBounds = xy::Util::Rectangle::combine(m_nodes[childA].fatBounds, m_nodes[childB].fatBounds);
            m_nodes[index].height = std::max(m_nodes[childA].height, m_nodes[childB].height) + 1;

            index = m_nodes[index].parent;
        }
    }
    else
    {
        m_root = sibling;
        m_nodes[sibling].parent = TreeNode::Null;
        freeNode(parent);
    }
}

std::int32_t DynamicTreeSystem::balance(std::int32_t iA)
{
    //performs left or right rotation if a is imbalanced
    //returns the new root

   // XY_ASSERT(iA != TreeNode::Null, "Invalid node");

    auto& A = m_nodes[iA];
    if (A.isLeaf() || A.height < 2)
    {
        return iA;
    }

    auto iB = A.childA;
    auto iC = A.childB;

   // XY_ASSERT(iB > -1 && iB < m_nodeCapacity, "Invalid node");
   // XY_ASSERT(iC > -1 && iC < m_nodeCapacity, "Invalid node");

    auto& B = m_nodes[iB];
    auto& C = m_nodes[iC];

    auto balance = C.height - B.height;

    //rotate C up
    if (balance > 1)
    {
        auto iF = C.childA;
        auto iG = C.childB;
        auto& F = m_nodes[iF];
        auto& G = m_nodes[iG];

      //  XY_ASSERT(iF > -1 && iF < m_nodeCapacity, "Invalid node");
     //  XY_ASSERT(iG > -1 && iG < m_nodeCapacity, "Invalid node");

        //swap A and C
        C.childA = iA;
        C.parent = A.parent;
        A.parent = iC;

        //A's old parent should point to C
        if (C.parent != TreeNode::Null)
        {
            if (m_nodes[C.parent].childA == iA)
            {
                m_nodes[C.parent].childA = iC;
            }
            else
            {
           //     XY_ASSERT(m_nodes[C.parent].childB == iA, "");
                m_nodes[C.parent].childB = iC;
            }
        }
        else
        {
            m_root = iC;
        }

        //rotate
        if (F.height > G.height)
        {
            C.childB = iF;
            A.childB = iG;
            G.parent = iA;
            A.fatBounds = xy::Util::Rectangle::combine(B.fatBounds, G.fatBounds);
            C.fatBounds = xy::Util::Rectangle::combine(A.fatBounds, F.fatBounds);

            A.height = std::max(B.height, G.height) + 1;
            C.height = std::max(A.height, F.height) + 1;
        }
        else
        {
            C.childB = iG;
            A.childB = iF;
            F.parent = iA;
            A.fatBounds = xy::Util::Rectangle::combine(B.fatBounds, F.fatBounds);
            C.fatBounds = xy::Util::Rectangle::combine(A.fatBounds, G.fatBounds);

            A.height = std::max(B.height, F.height) + 1;
            C.height = std::max(A.height, G.height) + 1;
        }

        return iC;
    }

    //rotate B up
    if (balance < -1)
    {
        auto iD = B.childA;
        auto iE = B.childB;
        auto& D = m_nodes[iD];
        auto& E = m_nodes[iE];
       // XY_ASSERT(iD > -1 && iD < m_nodeCapacity, "Invalid node");
      //  XY_ASSERT(iE > -1 && iE < m_nodeCapacity, "Invalid node");

        //swap A and B
        B.childA = iA;
        B.parent = A.parent;
        A.parent = iB;

        //A's old parent should point to B
        if (B.parent != TreeNode::Null)
        {
            if (m_nodes[B.parent].childA == iA)
            {
                m_nodes[B.parent].childA = iB;
            }
            else
            {
          //      XY_ASSERT(m_nodes[B.parent].childB == iA, "");
                m_nodes[B.parent].childB = iB;
            }
        }
        else
        {
            m_root = iB;
        }

        //rotate
        if (D.height > E.height)
        {
            B.childB = iD;
            A.childA = iE;
            E.parent = iA;
            A.fatBounds = xy::Util::Rectangle::combine(C.fatBounds, E.fatBounds);
            B.fatBounds = xy::Util::Rectangle::combine(A.fatBounds, D.fatBounds);

            A.height = std::max(C.height, E.height) + 1;
            B.height = std::max(A.height, D.height) + 1;
        }
        else
        {
            B.childB = iE;
            A.childA = iD;
            D.parent = iA;
            A.fatBounds = xy::Util::Rectangle::combine(C.fatBounds, D.fatBounds);
            B.fatBounds = xy::Util::Rectangle::combine(A.fatBounds, E.fatBounds);

            A.height = std::max(C.height, D.height) + 1;
            B.height = std::max(A.height, E.height) + 1;
        }

        return iB;
    }
    return iA;
}

std::int32_t DynamicTreeSystem::computeHeight() const
{
    return computeHeight(m_root);
}

std::int32_t DynamicTreeSystem::computeHeight(std::int32_t treeID) const
{
   // XY_ASSERT(treeID > TreeNode::Null && treeID < m_nodeCapacity, "Invalid tree id");

    if (m_nodes[treeID].isLeaf())
    {
        return 0;
    }

    auto heightA = computeHeight(m_nodes[treeID].childA);
    auto heightB = computeHeight(m_nodes[treeID].childB);

    return std::max(heightA, heightB) + 1;
}

void DynamicTreeSystem::validateStructure(std::int32_t) const
{

}

void DynamicTreeSystem::validateMetrics(std::int32_t) const
{

}