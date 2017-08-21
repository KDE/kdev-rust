#ifndef RUSTNODE_H
#define RUSTNODE_H

#include <language/duchain/ducontext.h>

#include "astredux.h"

#include "kdevrustduchain_export.h"

namespace Rust
{

template <typename RustObjectType, void (*RustDestructor)(RustObjectType *)>
class KDEVRUSTDUCHAIN_EXPORT RustAllocatedObject
{
public:
    RustAllocatedObject(RustObjectType *object);
    ~RustAllocatedObject();

    RustObjectType *data();
    RustObjectType *operator *();

private:
    RustObjectType *object;
};

class KDEVRUSTDUCHAIN_EXPORT RustString : public RustAllocatedObject<const char, destroy_string>
{
public:
    RustString(const char *str)
        : RustAllocatedObject(str)
    {
    }

    operator const char *()
    {
        return data();
    }
};

template <typename T> void noop_destructor(T *) {}

using RustCrate = RustAllocatedObject<RSCrate, destroy_crate>;
using RustOwnedNode = RustAllocatedObject<RSNode, destroy_node>;

class KDEVRUSTDUCHAIN_EXPORT RustNode : public RustAllocatedObject<RSNode, noop_destructor>
{
public:
    RustNode(RSNode *node);
    RustNode(RustOwnedNode &node);
    ~RustNode();
};

class KDEVRUSTDUCHAIN_EXPORT RustPath
{
public:
    explicit RustPath(RustNode *node);

    QString value;
};

template class KDEVRUSTDUCHAIN_EXPORT RustAllocatedObject<const char, destroy_string>;
template class KDEVRUSTDUCHAIN_EXPORT RustAllocatedObject<RSCrate, destroy_crate>;
template class KDEVRUSTDUCHAIN_EXPORT RustAllocatedObject<RSNode, destroy_node>;
template class KDEVRUSTDUCHAIN_EXPORT RustAllocatedObject<RSNode, noop_destructor>;

}

#endif // RUSTNODE_H
