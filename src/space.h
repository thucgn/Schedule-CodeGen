/*************************************************************************
	> File Name: space.h
	> Author: 
	> Mail: 
	> Created Time: Thu 16 May 2019 06:10:33 AM UTC
 ************************************************************************/

#ifndef _SPACE_H
#define _SPACE_H

#include <string>
#include <memory>
#include "refcountptr.h"
#include "node.h"
#include "iter.h"
#include "numberset.h"

namespace SC
{

enum class SpaceType : uint8_t
{
    SPLIT,
    REORDER,
    //FUSE,
    UNROLL
};

struct BaseSpace;

struct BaseSpaceNode
{
    virtual ~BaseSpaceNode() {}
    BaseSpaceNode(SpaceType type) : space_type(type) {}

    // reference counter, managed by BaseSpace
    mutable RefCount ref_count;
    //space type
    SpaceType space_type;
};

template <> inline RefCount& ref_count<BaseSpaceNode>(const BaseSpaceNode* p) {
    return p->ref_count;
}

template <> inline void destroy<BaseSpaceNode>(const BaseSpaceNode* p) {
    if(p) delete p;
}

struct BaseSpace : public RefCountPtr<const BaseSpaceNode>
{
    BaseSpace() : RefCountPtr<const BaseSpaceNode>() {}
    BaseSpace(const BaseSpaceNode* p) : RefCountPtr<const BaseSpaceNode>(p) {}

    template <typename T>
    bool is_type() const { return (ptr->space_type == T::_node_type); }

    template <typename T>
    const T* cast_to() const 
    {
        if(ptr) 
            if(is_type<T>())
                return (const T*)ptr;
    }
    
    SpaceType space_type() const { return ptr->space_type; }
};

template <typename T>
struct SubSpaceNode : BaseSpaceNode
{
    SubSpaceNode() : BaseSpaceNode(T::_node_type) {}
    virtual ~SubSpaceNode() {}
};

struct SplitSpace : SubSpaceNode<SplitSpace>
{
    Iter x;    
    std::unique_ptr<NumberSet> candidates;
    static const SpaceType _node_type = SpaceType::SPLIT;
    static BaseSpace make_space(Iter iter, int min, int max) {
        SplitSpace* n = new SplitSpace();
        n->x = std::move(iter);
        n->candidates = SC::make_unique<ContinuousSet>(min, max);
        return n;
    }
    static BaseSpace make_space(Iter iter, const std::vector<int>& candidates)
    {
        SplitSpace* n = new SplitSpace();
        n->x = std::move(iter);
        n->candidates = SC::make_unique<DiscreteSet>(std::forward<const std::vector<int>&>(candidates));
        return n;
    }
};

struct ReorderSpace : SubSpaceNode<ReorderSpace>
{
    std::vector< std::vector<Iter> > candidates;
    static const SpaceType _node_type = SpaceType::REORDER;
    static BaseSpace make_space(const std::vector<std::vector<Iter>>& candidates)
    {
        ReorderSpace* n = new ReorderSpace;
        n->candidates = candidates;
        return n;
    }
};

/*struct FuseSpace : SubSpaceNode<FuseSpace>
{
    static const SpaceType _node_type = SpaceType::FUSE;
    static 
};*/

struct UnrollSpace : SubSpaceNode<UnrollSpace>
{
    Iter x;
    std::unique_ptr<NumberSet> candidates;
    static const SpaceType _node_type = SpaceType::UNROLL;
    static BaseSpace make_space(Iter iter, const std::vector<int>& candidates)
    {
        UnrollSpace* n = new UnrollSpace;
        n->x = std::move(iter);
        n->candidates = SC::make_unique<DiscreteSet>(std::forward<const std::vector<int>&>(candidates));
        return n;
    }
};

struct SpaceNode : BaseSpaceNode
{
    std::vector<BaseSpace> splitspaces;    
    std::vector<BaseSpace> reorderspaces;
    std::vector<BaseSpace> unrollspaces;

    void define_split(Iter x, );
    void define_reorder();
    void define_fuse();
    void define_unroll();
};

} // namespace SC

#endif
