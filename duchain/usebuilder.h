#ifndef USEBUILDER_H
#define USEBUILDER_H

#include <language/duchain/builders/abstractusebuilder.h>
#include <serialization/indexedstring.h>

#include "contextbuilder.h"
#include "rustnode.h"

namespace Rust
{

using UseBuilderBase = KDevelop::AbstractUseBuilder<RustNode, RustPath, ContextBuilder>;

class UseBuilder : public UseBuilderBase
{
public:
    UseBuilder(const KDevelop::IndexedString &document);
    ~UseBuilder() override = default;

    RSVisitResult visitNode(RustNode *node, RustNode *parent) override;

private:
    KDevelop::IndexedString document;
};

}

#endif // USEBUILDER_H
