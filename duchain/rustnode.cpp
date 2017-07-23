#include "rustnode.h"

namespace Rust
{

RustPath::RustPath(RustNode *node)
{
    RustString name = RustString(node_get_spelling_name(node->data()));

    if (*name == nullptr) {
        value = "unknown";
    } else {
        value = QString::fromUtf8(*name);
    }
}

RustNode::RustNode(RSNode *node)
    : RustAllocatedObject(node)
{
}

RustNode::RustNode(RustOwnedNode &node)
    : RustAllocatedObject(node.data())
{
}

template<typename RustObjectType, void (*RustDestructor)(RustObjectType *)>
RustAllocatedObject<RustObjectType, RustDestructor>::RustAllocatedObject(RustObjectType *object)
    : object(object)
{
}

template<typename RustObjectType, void (*RustDestructor)(RustObjectType *)>
RustAllocatedObject<RustObjectType, RustDestructor>::~RustAllocatedObject()
{
    RustDestructor(object);
    object = nullptr;
}

template<typename RustObjectType, void (*RustDestructor)(RustObjectType *)>
RustObjectType *RustAllocatedObject<RustObjectType, RustDestructor>::data()
{
    return object;
}

template<typename RustObjectType, void (*RustDestructor)(RustObjectType *)>
RustObjectType *RustAllocatedObject<RustObjectType, RustDestructor>::operator *()
{
    return object;
}

template class RustAllocatedObject<const char, destroy_string>;
template class RustAllocatedObject<RSCrate, destroy_crate>;
template class RustAllocatedObject<RSNode, destroy_node>;
template class RustAllocatedObject<RSNode, noop_destructor>;

}
