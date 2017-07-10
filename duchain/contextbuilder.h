#ifndef CONTEXTBUILDER_H
#define CONTEXTBUILDER_H

#include <language/duchain/builders/abstractcontextbuilder.h>

#include "rustnode.h"

namespace Rust
{

using ContextBuilderBase = KDevelop::AbstractContextBuilder<RustNode, RustPath>;

class ContextBuilder : public ContextBuilderBase
{
public:
    ContextBuilder() = default;
    ~ContextBuilder() override = default;

protected:
    KDevelop::RangeInRevision editorFindSpellingRange(RustNode *node, const QString &identifier);

    template<RSNodeKind Kind>
    KDevelop::DUContext *createContext(RSNode *node, const KDevelop::QualifiedIdentifier& scopeId);

    virtual RSVisitResult visitNode(RustNode *node, RustNode *parent) = 0;
    void visitChildren(RustNode *node);

    void startVisiting(RustNode *node) override;
    void setContextOnNode(RustNode *node, KDevelop::DUContext *context) override;
    KDevelop::DUContext *contextFromNode(RustNode *node) override;
    KDevelop::RangeInRevision editorFindRange(RustNode *fromNode, RustNode *toNode) override;
    KDevelop::QualifiedIdentifier identifierForNode(RustPath *node) override;
    KDevelop::DUContext *newContext(const KDevelop::RangeInRevision &range) override;
    KDevelop::TopDUContext *newTopContext(const KDevelop::RangeInRevision &range, KDevelop::ParsingEnvironmentFile *file) override;

private:
    friend RSVisitResult visitCallback(RSNode *node, RSNode *parent, void *data);
};

}

#endif // CONTEXTBUILDER_H
