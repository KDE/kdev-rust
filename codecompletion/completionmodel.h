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

#ifndef COMPLETIONMODEL_H
#define COMPLETIONMODEL_H

#include <language/codecompletion/codecompletionmodel.h>

#include "kdevrustcompletion_export.h"

namespace Rust
{

class KDEVRUSTCOMPLETION_EXPORT CompletionModel : public KDevelop::CodeCompletionModel
{
    Q_OBJECT
public:
    CompletionModel(QObject *parent);

protected:
    KDevelop::CodeCompletionWorker *createCompletionWorker() override;
};

}

#endif // COMPLETIONMODEL_H
