#ifndef NODETRAITS_H
#define NODETRAITS_H

#include <language/duchain/ducontext.h>

#include "astredux.h"

namespace Rust
{

namespace NodeTraits
{

constexpr bool hasContext(RSNodeKind kind)
{
    return kind == Crate || kind == StructDecl || kind == EnumDecl
            || kind == TraitDecl || kind == ImplDecl || kind == FunctionDecl;
}

constexpr KDevelop::DUContext::ContextType contextType(RSNodeKind kind)
{
    using namespace KDevelop;
    return kind == Crate        ? DUContext::Global
        :  kind == StructDecl   ? DUContext::Class
        :  kind == EnumDecl     ? DUContext::Enum
        :  kind == TraitDecl    ? DUContext::Helper
        :  kind == ImplDecl     ? DUContext::Helper
        :  kind == FunctionDecl ? DUContext::Function
        : static_cast<DUContext::ContextType>(-1);
}

constexpr bool isKDevDeclaration(RSNodeKind kind)
{
    return kind == EnumDecl || kind == TypeAliasDecl || kind == EnumVariantDecl
            || kind == ParmDecl || kind == VarDecl;
}

constexpr bool isTypeDeclaration(RSNodeKind kind)
{
    return  kind == EnumDecl || kind == EnumVariantDecl
            || kind == TypeAliasDecl || kind == StructDecl
            || kind == ImplDecl || kind == TraitDecl;
}

}

}

#endif // NODETRAITS_H
