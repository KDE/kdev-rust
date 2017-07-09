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

    if (kind == PathUse) {
        RustPath path(node);


        RustPath name(node);
        RangeInRevision useRange = editorFindSpellingRange(node, name.value);

        qCDebug(KDEV_RUST) << "USE:" << name.value << "; spelling range: ("
                           << useRange.start.line + 1 << ":" << useRange.start.column << "-"
                           << useRange.end.line + 1 << ":" << useRange.end.column << ")";

        DUContext *context = topContext()->findContextAt(useRange.start);
        QList<Declaration *> declarations = context->findDeclarations(identifierForNode(&path));

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
