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
    ~UseBuilder() override = default;

    RSVisitResult visitNode(RustNode *node, RustNode *parent) override;
};

}

#endif // USEBUILDER_H
