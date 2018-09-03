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
