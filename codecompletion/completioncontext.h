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

#ifndef COMPLETIONCONTEXT_H
#define COMPLETIONCONTEXT_H

#include <QString>

#include <language/codecompletion/codecompletioncontext.h>

#include <kdevrustcompletion_export.h>

namespace Rust
{

class KDEVRUSTCOMPLETION_EXPORT CompletionContext : public KDevelop::CodeCompletionContext
{
public:
    CompletionContext(KDevelop::DUContextPointer context,
                      const QString &contextText,
                      const QString &followingText,
                      const KDevelop::CursorInRevision &position,
                      int depth);

    QList<KDevelop::CompletionTreeItemPointer> completionItems(bool &abort, bool fullCompletion) override;

private:
    QString m_followingText;
};

}

#endif // COMPLETIONCONTEXT_H
