#include "usebuilder.h"

#include <language/duchain/ducontext.h>
#include <language/duchain/declaration.h>
#include <language/duchain/problem.h>
#include <language/editor/documentrange.h>

#include "rustdebug.h"

namespace Rust
{

RSVisitResult UseBuilder::visitNode(RustNode *node, RustNode *parent)
{
    using namespace KDevelop;
    RSNodeKind kind = node_get_kind(node->data());

    if (kind == PathSegment) {
        RustPath segment(node);
        RustPath path(parent);
        QualifiedIdentifier qualifiedPath = identifierForNode(&path);
        IndexedIdentifier pathSegment = IndexedIdentifier(Identifier(segment.value));

        // eh, this feels way too much like a hack
        while (qualifiedPath.last() != pathSegment) {
            qualifiedPath.pop();
        }

        RangeInRevision useRange = editorFindRange(node, node);

        qCDebug(KDEV_RUST) << "USE:" << segment.value << "; spelling range: ("
                           << useRange.start.line + 1 << ":" << useRange.start.column << "-"
                           << useRange.end.line + 1 << ":" << useRange.end.column << ")";

        DUContext *context = topContext()->findContextAt(useRange.start);
        QList<Declaration *> declarations = context->findDeclarations(qualifiedPath);

        if (declarations.isEmpty()) {
            Problem *p = new Problem();
            p->setFinalLocation(DocumentRange(document(), useRange.castToSimpleRange()));
            p->setSource(IProblem::SemanticAnalysis);
            p->setSeverity(IProblem::Hint);
            p->setDescription(i18n("Undefined %1", path.value));
            {
                DUChainWriteLocker wlock(DUChain::lock());
                ProblemPointer ptr(p);
                topContext()->addProblem(ptr);
            }
        } else if (declarations.first() && declarations.first()->range() != useRange) {
            UseBuilderBase::newUse(node, useRange, DeclarationPointer(declarations.first()));
        }
    }

    return Recurse;
}

}
