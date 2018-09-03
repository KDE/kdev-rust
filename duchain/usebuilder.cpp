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

#include "usebuilder.h"

#include <language/duchain/ducontext.h>
#include <language/duchain/declaration.h>
#include <language/duchain/problem.h>
#include <language/editor/documentrange.h>

#include "rustdebug.h"

namespace Rust
{

using namespace KDevelop;

UseBuilder::UseBuilder(const KDevelop::IndexedString &document)
    : document(document)
{
}

RSVisitResult UseBuilder::visitNode(RustNode *node, RustNode *parent)
{
    RSNodeKind kind = node_get_kind(node->data());
    RSNodeKind parentKind = node_get_kind(parent->data());

    if (kind == Path) {
        visitPath(node, parent);
    } else if (parentKind == Path && kind == PathSegment) {
        visitPathSegment(node, parent);
    }

    return ContextBuilder::visitNode(node, parent);
}

void UseBuilder::visitPath(RustNode *node, RustNode *parent)
{
    RustPath path(node);
    fullPath = identifierForNode(&path);
    currentPath.clear();
}

void UseBuilder::visitPathSegment(RustNode *node, RustNode *parent)
{
    RustPath segment(node);
    IndexedIdentifier pathSegment = IndexedIdentifier(Identifier(segment.value));

    currentPath.push(pathSegment);

    DUContext::SearchFlags flags = DUContext::NoSearchFlags;

    if (fullPath.isQualified()) {
        flags = DUContext::NoFiltering;
    }

    RangeInRevision useRange = editorFindRange(node, node);
    DUContext *context = topContext()->findContextAt(useRange.start);
    QList<Declaration *> declarations = context->findDeclarations(currentPath,
                                                                  CursorInRevision::invalid(),
                                                                  AbstractType::Ptr(),
                                                                  nullptr,
                                                                  flags);

    if (declarations.isEmpty() || !declarations.first()) {
        ProblemPointer p = ProblemPointer(new Problem());
        p->setFinalLocation(DocumentRange(document, useRange.castToSimpleRange()));
        p->setSource(IProblem::SemanticAnalysis);
        p->setSeverity(IProblem::Hint);
        p->setDescription(i18n("Undefined %1", fullPath.toString()));

        DUChainWriteLocker lock(DUChain::lock());
        topContext()->addProblem(p);
    } else {
        for (Declaration *declaration : declarations) {
            if (fullPath.isQualified() && currentPath != fullPath) {
                // We are dealing with a container-like path, ignore functions and variables
                if (!declaration->internalContext()
                        || declaration->internalContext()->type() == DUContext::Other
                        || declaration->internalContext()->type() == DUContext::Function) {
                    continue;
                }
            }

            if (declaration->range() != useRange) {
                UseBuilderBase::newUse(node, useRange, DeclarationPointer(declaration));
                break;
            }
        }
    }
}

}
