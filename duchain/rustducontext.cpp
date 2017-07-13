#include "rustducontext.h"

#include <language/duchain/topducontextdata.h>

namespace Rust
{

REGISTER_DUCHAIN_ITEM_WITH_DATA(RustTopDUContext, TopDUContextData);
REGISTER_DUCHAIN_ITEM_WITH_DATA(RustNormalDUContext, DUContextData);

template <>
QWidget *RustNormalDUContext::createNavigationWidget(KDevelop::Declaration *decl,
                                                     KDevelop::TopDUContext *topContext,
                                                     const QString &htmlPrefix,
                                                     const QString &htmlSuffix,
                                                     KDevelop::AbstractNavigationWidget::DisplayHints hints) const
{
    return nullptr;
}

template <>
QWidget *RustTopDUContext::createNavigationWidget(KDevelop::Declaration *decl,
                                                  KDevelop::TopDUContext *topContext,
                                                  const QString &htmlPrefix,
                                                  const QString &htmlSuffix,
                                                  KDevelop::AbstractNavigationWidget::DisplayHints hints) const
{
    return nullptr;
}

}
