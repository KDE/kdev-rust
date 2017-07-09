#ifndef USEBUILDER_H
#define USEBUILDER_H

#include <language/duchain/builders/abstractusebuilder.h>

#include "contextbuilder.h"
#include "rustnode.h"

namespace Rust
{

using UseBuilderBase = KDevelop::AbstractUseBuilder<RustNode, RustPath, ContextBuilder>;

class UseBuilder : public UseBuilderBase
{
public:
    UseBuilder() = default;
    virtual ~UseBuilder() = default;

    virtual RSVisitResult visitNode(RustNode *node, RustNode *parent);
};

}

#endif // USEBUILDER_H
