#ifndef USEBUILDER_H
#define USEBUILDER_H

#include <language/duchain/builders/abstractusebuilder.h>
#include <serialization/indexedstring.h>

#include "contextbuilder.h"
#include "rustnode.h"

#include "kdevrustduchain_export.h"

namespace Rust
{

using UseBuilderBase = KDevelop::AbstractUseBuilder<RustNode, RustPath, ContextBuilder>;

class KDEVRUSTDUCHAIN_EXPORT UseBuilder : public UseBuilderBase
{
public:
    UseBuilder(const KDevelop::IndexedString &document);
    ~UseBuilder() override = default;

    RSVisitResult visitNode(RustNode *node, RustNode *parent) override;

private:
    void visitPath(RustNode *node, RustNode *parent);
    void visitPathSegment(RustNode *node, RustNode *parent);

    KDevelop::QualifiedIdentifier fullPath;
    KDevelop::QualifiedIdentifier currentPath;

    KDevelop::IndexedString document;
};

}

#endif // USEBUILDER_H
