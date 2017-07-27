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
    return kind == Crate || kind == Module || kind == StructDecl || kind == EnumDecl
            || kind == TraitDecl || kind == ImplDecl || kind == FunctionDecl;
}

constexpr KDevelop::DUContext::ContextType contextType(RSNodeKind kind)
{
    using namespace KDevelop;
    return kind == Crate        ? DUContext::Global
        :  kind == Module       ? DUContext::Namespace
        :  kind == StructDecl   ? DUContext::Class
        :  kind == EnumDecl     ? DUContext::Enum
        :  kind == TraitDecl    ? DUContext::Class
        :  kind == ImplDecl     ? DUContext::Class
        :  kind == FunctionDecl ? DUContext::Other // FIXME: function args should be in a Function context
        : static_cast<DUContext::ContextType>(-1);
}

constexpr bool isKDevDeclaration(RSNodeKind kind)
{
    return kind == EnumDecl || kind == TypeAliasDecl || kind == EnumVariantDecl
            || kind == ParmDecl || kind == VarDecl || kind == Module;
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
