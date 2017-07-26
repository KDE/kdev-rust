#include "usebuilder.h"

#include <language/duchain/ducontext.h>
#include <language/duchain/declaration.h>
#include <language/duchain/problem.h>
#include <language/editor/documentrange.h>

#include "rustdebug.h"

namespace Rust
{

UseBuilder::UseBuilder(const KDevelop::IndexedString &document)
    : document(document)
{
}

RSVisitResult UseBuilder::visitNode(RustNode *node, RustNode *parent)
{
    using namespace KDevelop;
    RSNodeKind kind = node_get_kind(node->data());
    RSNodeKind parentKind = node_get_kind(parent->data());

    if (parentKind == Path && kind == PathSegment) {
        RustPath segment(node);
        RustPath path(parent);
        QualifiedIdentifier qualifiedPath = identifierForNode(&path);
        IndexedIdentifier pathSegment = IndexedIdentifier(Identifier(segment.value));

        // eh, this feels way too much like a hack
        while (qualifiedPath.last() != pathSegment) {
            qualifiedPath.pop();
        }

        RangeInRevision useRange = editorFindRange(node, node);
        DUContext *context = topContext()->findContextAt(useRange.start);
        QList<Declaration *> declarations = context->findDeclarations(qualifiedPath);

//        qCDebug(KDEV_RUST) << "USE:" << segment.value << "; spelling range: ("
//                           << useRange.start.line + 1 << ":" << useRange.start.column << "-"
//                           << useRange.end.line + 1 << ":" << useRange.end.column << "); context:"
//                           << context->localScopeIdentifier()
//                           << context->range();


        if (declarations.isEmpty() || !declarations.first()) {
            ProblemPointer p = ProblemPointer(new Problem());
            p->setFinalLocation(DocumentRange(document, useRange.castToSimpleRange()));
            p->setSource(IProblem::SemanticAnalysis);
            p->setSeverity(IProblem::Hint);
            p->setDescription(i18n("Undefined %1", path.value));

            DUChainWriteLocker lock(DUChain::lock());
            topContext()->addProblem(p);
        } else if (declarations.first()->range() != useRange) {
            UseBuilderBase::newUse(node, useRange, DeclarationPointer(declarations.first()));
        }
    }

    return ContextBuilder::visitNode(node, parent);
}

}
