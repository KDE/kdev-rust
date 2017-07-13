#include "contextbuilder.h"

#include <language/duchain/parsingenvironment.h>
#include <interfaces/icore.h>
#include <interfaces/idocumentcontroller.h>

#include "rustducontext.h"
#include "nodetraits.h"

namespace Rust
{

RSVisitResult visitCallback(RSNode *node, RSNode *parent, void *data);

RangeInRevision ContextBuilder::editorFindSpellingRange(RustNode *node, const QString &identifier)
{
    RSRange range = node_get_spelling_range(node->data());
    KTextEditor::Range incorrectRange = KTextEditor::Range(range.start.line - 1, range.start.column, INT_MAX, INT_MAX);
    IDocument *document = ICore::self()->documentController()
            ->documentForUrl(topContext()->url().toUrl());

    QVector<KTextEditor::Range> ranges;
    if (document) {
        ranges = document->textDocument()->searchText(incorrectRange, identifier);
    } else {
        ranges = { KTextEditor::Range::invalid() };
    }

    return RangeInRevision::castFromSimpleRange(ranges.first());
}

void ContextBuilder::visitChildren(RustNode *node)
{
    visit_children(node->data(), visitCallback, this);
}

void ContextBuilder::startVisiting(RustNode *node)
{
    visitChildren(node);
}

void ContextBuilder::setContextOnNode(RustNode *node, KDevelop::DUContext *context)
{
    node->setContext(context);
}

KDevelop::DUContext *ContextBuilder::contextFromNode(RustNode *node)
{
    return node->getContext();
}

KDevelop::RangeInRevision ContextBuilder::editorFindRange(RustNode *fromNode, RustNode *toNode)
{
    RSRange fromRange = node_get_extent(fromNode->data());
    RSRange toRange = node_get_extent(toNode->data());

    return RangeInRevision(fromRange.start.line - 1, fromRange.start.column, toRange.end.line - 1, toRange.end.column);
}

KDevelop::QualifiedIdentifier ContextBuilder::identifierForNode(RustPath *node)
{
    return QualifiedIdentifier(node->value);
}

KDevelop::DUContext *ContextBuilder::newContext(const KDevelop::RangeInRevision &range)
{
    return new RustNormalDUContext(range, currentContext());
}

KDevelop::TopDUContext *ContextBuilder::newTopContext(const KDevelop::RangeInRevision &range, KDevelop::ParsingEnvironmentFile *file)
{
    if (!file) {
        file = new ParsingEnvironmentFile(document());
        file->setLanguage(IndexedString("Rust"));
    }

    return new RustTopDUContext(document(), range, file);
}

RSVisitResult visitCallback(RSNode *node, RSNode *parent, void *data)
{
    ContextBuilder *builder = static_cast<ContextBuilder *>(data);
    RustNode currentNode(node);
    RustNode parentNode(parent);
    return builder->visitNode(&currentNode, &parentNode);
}

}
