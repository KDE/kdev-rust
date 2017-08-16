#include "rustducontext.h"

#include <language/duchain/topducontextdata.h>

namespace Rust
{

REGISTER_DUCHAIN_ITEM_WITH_DATA(RustTopDUContext, TopDUContextData);
REGISTER_DUCHAIN_ITEM_WITH_DATA(RustNormalDUContext, DUContextData);

}
