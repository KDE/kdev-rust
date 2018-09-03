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

#ifndef CONTEXTBUILDER_H
#define CONTEXTBUILDER_H

#include <language/duchain/builders/abstractcontextbuilder.h>

#include "rustnode.h"
#include "parsesession.h"

#include "kdevrustduchain_export.h"

namespace Rust
{

using ContextBuilderBase = KDevelop::AbstractContextBuilder<RustNode, RustPath>;

class KDEVRUSTDUCHAIN_EXPORT ContextBuilder : public ContextBuilderBase
{
public:
    ContextBuilder() = default;
    ~ContextBuilder() override = default;

    void setParseSession(ParseSession *session);

protected:
    KDevelop::RangeInRevision editorFindSpellingRange(RustNode *node, const QString &identifier);

    template <RSNodeKind>
    RSVisitResult buildContext(RustNode *node, RustNode *parent);

    template<RSNodeKind Kind>
    KDevelop::DUContext *createContext(RSNode *node, const KDevelop::QualifiedIdentifier& scopeId);

    virtual RSVisitResult visitNode(RustNode *node, RustNode *parent);
    void visitChildren(RustNode *node);

    void startVisiting(RustNode *node) override;
    void setContextOnNode(RustNode *node, KDevelop::DUContext *context) override;
    KDevelop::DUContext *contextFromNode(RustNode *node) override;
    KDevelop::RangeInRevision editorFindRange(RustNode *fromNode, RustNode *toNode) override;
    KDevelop::QualifiedIdentifier identifierForNode(RustPath *node) override;
    KDevelop::DUContext *newContext(const KDevelop::RangeInRevision &range) override;
    KDevelop::TopDUContext *newTopContext(const KDevelop::RangeInRevision &range, KDevelop::ParsingEnvironmentFile *file) override;

private:
    friend RSVisitResult visitCallback(RSNode *node, RSNode *parent, void *data);

    ParseSession *session;
};

}

#endif // CONTEXTBUILDER_H
