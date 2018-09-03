/*
 * Copyright 2017  Emma Gospodinova <emma.gospodinova@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

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
