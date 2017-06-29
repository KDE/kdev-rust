#include "rustnode.h"

namespace Rust
{

RustNode::RustNode(RSNode *node)
    : node(node),
      context(nullptr)
{
}

RustPath::RustPath(RustNode *node)
{
    RustAllocatedString name = RustAllocatedString(node_get_spelling_name(node->node));

    if (name == nullptr) {
        value = "unknown";
    } else {
        value = QString::fromUtf8(name);
    }
}

}
