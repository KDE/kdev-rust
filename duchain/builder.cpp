#include "builder.h"
#include "astredux.h"

#include "rustdebug.h"

RSVisitResult visit_callback(RSNode *node, RSNode *parent, void *data);

class DUChainBuilder
{
public:
    DUChainBuilder(RSCrate *crate);
    ~DUChainBuilder();

    RSVisitResult visit(RSNode *node, RSNode *parent);

private:
    RSNode *crateNode;
};

DUChainBuilder::DUChainBuilder(RSCrate *crate)
    : crateNode(node_from_crate(crate))
{
    visit_children(crateNode, visit_callback, this);
}

DUChainBuilder::~DUChainBuilder()
{
    destroy_node(crateNode);
}

RSVisitResult DUChainBuilder::visit(RSNode *node, RSNode *parent)
{
    qCDebug(KDEV_RUST) << "got node";
    return Recurse;
}

RSVisitResult visit_callback(RSNode *node, RSNode *parent, void *data)
{
    DUChainBuilder *builder = static_cast<DUChainBuilder *>(data);
    return builder->visit(node, parent);
}

void Builder::buildDUChain(RSCrate *crate)
{
    DUChainBuilder builder(crate);
}
