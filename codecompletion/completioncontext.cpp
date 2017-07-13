#include "completioncontext.h"

#include <language/duchain/ducontext.h>
#include <language/duchain/declaration.h>
#include <language/duchain/duchainlock.h>
#include <language/codecompletion/normaldeclarationcompletionitem.h>

#include "rustdebug.h"

namespace Rust
{

using namespace KDevelop;

CompletionContext::CompletionContext(DUContextPointer context,
                                     const QString &contextText,
                                     const QString &followingText,
                                     const CursorInRevision &position,
                                     int depth)
    : KDevelop::CodeCompletionContext(context, contextText, position, depth),
      m_followingText(followingText)
{

}

QList<CompletionTreeItemPointer> CompletionContext::completionItems(bool &abort, bool fullCompletion)
{
    QList<CompletionTreeItemPointer> items;

    DUChainReadLocker lock;
    auto declarations = m_duContext->allDeclarations(CursorInRevision::invalid(), m_duContext->topContext());
    for(const QPair<Declaration *, int> &decl : declarations)
    {
        if(decl.first->topContext() != m_duContext->topContext())
            continue;
        if(decl.first->identifier() == globalImportIdentifier() || decl.first->identifier() == globalAliasIdentifier()
            || decl.first->identifier() == Identifier())
            continue;

        items << CompletionTreeItemPointer(new NormalDeclarationCompletionItem(DeclarationPointer(decl.first)));
    }
    return items;
}

}
