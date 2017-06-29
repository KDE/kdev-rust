#ifndef RUSTNODE_H
#define RUSTNODE_H

#include <language/duchain/ducontext.h>

#include "astredux.h"

namespace Rust
{

class RustNode
{
public:
    RustNode(RSNode *node);

    RSNode *node;
    KDevelop::DUContext *context;
};

class RustPath
{
public:
    explicit RustPath(RustNode *node);

    QString value;
};

}

#endif // RUSTNODE_H
