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

template <typename T> void noop_destructor(T *) {}

using RustCrate = RustAllocatedObject<RSCrate, destroy_crate>;
using RustOwnedNode = RustAllocatedObject<RSNode, destroy_node>;
using RustString = RustAllocatedObject<const char, destroy_string>;
using RustDiagnostic = RustAllocatedObject<RSDiagnostic, destroy_diagnostic>;
using RustDiagnosticIterator = RustAllocatedObject<RSDiagnosticIterator, destroy_diagnostic_iterator>;

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
template class KDEVRUSTDUCHAIN_EXPORT RustAllocatedObject<RSDiagnostic, destroy_diagnostic>;
template class KDEVRUSTDUCHAIN_EXPORT RustAllocatedObject<RSDiagnosticIterator, destroy_diagnostic_iterator>;
template class KDEVRUSTDUCHAIN_EXPORT RustAllocatedObject<RSNode, noop_destructor>;

}

#endif // RUSTNODE_H
