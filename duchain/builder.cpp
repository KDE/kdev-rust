#include "builder.h"

#include <language/duchain/duchainlock.h>
#include <language/duchain/topducontext.h>
#include <language/duchain/duchain.h>
#include <language/duchain/problem.h>
#include <language/backgroundparser/urlparselock.h>

#include "rustdebug.h"
#include "astredux.h"
#include "duchain/parsesession.h"
#include "duchain/rustducontext.h"

namespace Rust
{

RSVisitResult visitCallback(RSNode *node, RSNode *parent, void *data);

class DUChainBuilder
{
public:
    DUChainBuilder(RSIndex *index, RSCrate *crate, bool update);
    ~DUChainBuilder();

    template <RSNodeKind Kind>
    RSVisitResult buildDeclaration(RSNode *node, RSNode *parent);

private:
    RSIndex *index;
    RSCrate *crate;
    RSNode *crateNode;
};

DUChainBuilder::DUChainBuilder(RSIndex *index, RSCrate *crate, bool update)
    : index(index),
      crate(crate),
      crateNode(node_from_crate(crate))
{
    visit_children(crateNode, visitCallback, this);
}

DUChainBuilder::~DUChainBuilder()
{
    destroy_node(crateNode);
}


template<RSNodeKind Kind>
RSVisitResult DUChainBuilder::buildDeclaration(RSNode *node, RSNode *parent)
{
    RustAllocatedString name(node_get_spelling_name(node));
    RSRange range = node_get_spelling_range(node, index);
    RSRange extent = node_get_extent(node, index);

    qCDebug(KDEV_RUST) << "DECLARATION:" << name << "; spelling range: ("
                       << range.start.line << ":" << range.start.column << "-"
                       << range.end.line << ":" << range.end.column << "); extent: ("
                       << extent.start.line << ":" << extent.start.column << "-"
                       << extent.end.line << ":" << extent.end.column << ")";

    return Recurse;
}


RSVisitResult visitCallback(RSNode *node, RSNode *parent, void *data)
{
    DUChainBuilder *builder = static_cast<DUChainBuilder *>(data);
    RSNodeKind kind = node_get_kind(node);

    switch (kind) {
    case StructDecl:
        return builder->buildDeclaration<StructDecl>(node, parent);
    case FunctionDecl:
        return builder->buildDeclaration<FunctionDecl>(node, parent);
    default:
        return Recurse;
    }
}

ReferencedTopDUContext Builder::buildDUChain(const ParseSession &parseSession, RSIndex *index)
{
    using namespace KDevelop;

    bool update = false;
    UrlParseLock urlLock(parseSession.document());
    ReferencedTopDUContext context;
    {
        DUChainWriteLocker lock;
        context = DUChain::self()->chainForDocument(parseSession.document());
        if (!context) {
            ReferencedTopDUContext context = new Rust::RustTopDUContext(parseSession.document(), RangeInRevision(0, 0, INT_MAX, INT_MAX));
            DUChain::self()->addDocumentChain(context);
        } else {
            update = true;
        }
    }

    DUChainBuilder builder(index, parseSession.crate(), update);

    DUChain::self()->emitUpdateReady(parseSession.document(), context);

    return context;
}

}
