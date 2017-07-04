#ifndef RUSTNODE_H
#define RUSTNODE_H

#include <language/duchain/ducontext.h>

#include "astredux.h"

namespace Rust
{

template <typename RustObjectType, void (*RustDestructor)(RustObjectType *)>
class RustAllocatedObject
{
public:
    RustAllocatedObject(RustObjectType *object);
    ~RustAllocatedObject();

    RustObjectType *data();
    RustObjectType *operator *();

private:
    RustObjectType *object;
};

class RustString : public RustAllocatedObject<const char, destroy_string>
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

class RustNode : public RustAllocatedObject<RSNode, noop_destructor>
{
public:
    RustNode(RSNode *node);
    RustNode(RustOwnedNode &node);

    KDevelop::DUContext *getContext();
    void setContext(KDevelop::DUContext *context);

private:
    KDevelop::DUContext *context;
};

class RustPath
{
public:
    explicit RustPath(RustNode *node);

    QString value;
};

}

#endif // RUSTNODE_H
