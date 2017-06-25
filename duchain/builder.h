#ifndef BUILDER_H
#define BUILDER_H

#include <language/duchain/topducontext.h>

#include "astredux.h"
#include "parsesession.h"

namespace Rust
{

namespace Builder
{
    KDevelop::ReferencedTopDUContext buildDUChain(const ParseSession &parseSession, RSIndex *index);
}

}

#endif // BUILDER_H
