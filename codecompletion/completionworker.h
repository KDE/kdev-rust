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

#ifndef COMPLETIONWORKER_H
#define COMPLETIONWORKER_H

#include <language/codecompletion/codecompletionworker.h>

namespace Rust
{

class CompletionModel;

class CompletionWorker : public KDevelop::CodeCompletionWorker
{
    Q_OBJECT
public:
    CompletionWorker(CompletionModel *parent);

protected:
    KDevelop::CodeCompletionContext *createCompletionContext(const KDevelop::DUContextPointer &context,
                                                             const QString &contextText,
                                                             const QString &followingText,
                                                             const KDevelop::CursorInRevision &position) const override;
};

}

#endif // COMPLETIONWORKER_H
