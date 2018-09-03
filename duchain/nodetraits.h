/*
 * Copyright 2017  Emma Gospodinova <emma.gospodinova@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

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
            || kind == TraitDecl || kind == ImplDecl || kind == FunctionDecl
            || kind == Block || kind == Arm;
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
        :  kind == FunctionDecl ? DUContext::Function
        :  kind == Block        ? DUContext::Other
        :  kind == Arm          ? DUContext::Other
        : static_cast<DUContext::ContextType>(-1);
}

constexpr bool isKDevDeclaration(RSNodeKind kind)
{
    return kind == EnumDecl || kind == EnumVariantDecl
            || kind == ParmDecl || kind == VarDecl || kind == Module;
}

constexpr bool isTypeDeclaration(RSNodeKind kind)
{
    return  kind == EnumDecl || kind == EnumVariantDecl
            || kind == StructDecl || kind == ImplDecl || kind == TraitDecl;
}

}

}

#endif // NODETRAITS_H
