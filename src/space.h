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
#include <array>
#include <map>
#include "refcountptr.h"
#include "node.h"
#include "iter.h"
#include "numberset.h"
#include "util.h"
#include "axis.h"

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

struct SplitSpaceNode : SubSpaceNode<SplitSpaceNode>
{
    Iter x;    
    std::unique_ptr<NumberSet> candidates;
    static const SpaceType _node_type = SpaceType::SPLIT;
    static BaseSpace make(Iter iter, int min, int max) {
        SplitSpaceNode* n = new SplitSpaceNode();
        n->x = std::move(iter);
        n->candidates = SC::make_unique<ContinuousSet>(min, max);
        return n;
    }
    static BaseSpace make(Iter iter, const std::vector<int>& candidates)
    {
        SplitSpaceNode* n = new SplitSpaceNode();
        n->x = std::move(iter);
        n->candidates = SC::make_unique<DiscreteSet>(std::forward<const std::vector<int>&>(candidates));
        return n;
    }
};

struct ReorderSpaceNode : SubSpaceNode<ReorderSpaceNode>
{
    std::vector< std::vector<Iter> > candidates;
    static const SpaceType _node_type = SpaceType::REORDER;
    static BaseSpace make(const std::vector<std::vector<Iter>>& candidates)
    {
        ReorderSpaceNode* n = new ReorderSpaceNode;
        n->candidates = candidates;
        return n;
    }
};

/*struct FuseSpace : SubSpaceNode<FuseSpace>
{
    static const SpaceType _node_type = SpaceType::FUSE;
    static 
};*/

struct UnrollSpaceNode : SubSpaceNode<UnrollSpaceNode>
{
    Iter x;
    std::unique_ptr<NumberSet> candidates;
    static const SpaceType _node_type = SpaceType::UNROLL;
    static BaseSpace make(Iter iter, const std::vector<int>& candidates)
    {
        UnrollSpaceNode* n = new UnrollSpaceNode;
        n->x = std::move(iter);
        n->candidates = SC::make_unique<DiscreteSet>(std::forward<const std::vector<int>&>(candidates));
        return n;
    }
};

struct SpaceNode : BaseSpaceNode
{
    /*
    std::vector<BaseSpace> splitspaces;    
    std::vector<BaseSpace> reorderspaces;
    std::vector<BaseSpace> unrollspaces;*/
    
    std::map<std::string, BaseSpace> spaces;
    
};

struct Space : public BaseSpace
{
    Space() : BaseSpace() {}
    Space(const SpaceNode* p) : BaseSpace(p) {}

    SpaceNode* get() const  { return (SpaceNode*)ptr; }
    SpaceNode* operator->() const  { return get(); }
    SpaceNode& operator*() const { return *get(); }

    /*void define_split(Iter x, int min, int max);
    void define_split(Iter x, const std::vector<int>& candidates);
    void define_reorder(const std::vector<std::vector<Iter>>& candidates);
    //void define_fuse();
    void define_unroll(Iter x, const std::vector<int>& candidates);*/
    /*void define_split(const std::string& tag, int min, int max); 
    void define_split(const std::string& tag, const std::vector<int>& candidates);
    void define_reorder(const std::string& tag, const std::vector<std::vector<Iter>>& candidates); 
    void define_unroll(const std::string& tag, const std::vector<int>& candidates);*/
    AxisPair define_split(const std::string& tag, Axis axis,  int min, int max); 
    AxisPair define_split(const std::string& tag, Axis axis, const std::vector<int>& candidates);
    void define_reorder(const std::string& tag, const std::vector< std::vector<Axis> >& candidates);
    void define_unroll(const std::string& tag, Axis axis, const std::vector<int>& candidates);
};

} // namespace SC

#endif 
