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

#ifndef DECLARATIONTYPES_H
#define DECLARATIONTYPES_H

#include <type_traits>

#include <language/duchain/types/abstracttype.h>
#include <language/duchain/types/alltypes.h>
#include <language/duchain/classdeclaration.h>
#include <language/duchain/functiondeclaration.h>
#include <language/duchain/classmemberdeclaration.h>
#include <language/duchain/aliasdeclaration.h>

#include "astredux.h"
#include "nodetraits.h"

namespace Rust
{

template <RSNodeKind Kind, class Enable = void>
struct IdType;

template <RSNodeKind Kind>
struct IdType<Kind, typename std::enable_if<Kind == StructDecl || Kind == TraitDecl || Kind == ImplDecl>::type>
{
    typedef KDevelop::StructureType Type;
};

template <RSNodeKind Kind>
struct IdType<Kind, typename std::enable_if<Kind == TypeAliasDecl>::type>
{
    typedef KDevelop::TypeAliasType Type;
};

template <RSNodeKind Kind>
struct IdType<Kind, typename std::enable_if<Kind == EnumDecl>::type>
{
    typedef KDevelop::EnumerationType Type;
};

template <RSNodeKind Kind>
struct IdType<Kind, typename std::enable_if<Kind == EnumVariantDecl>::type>
{
    typedef KDevelop::EnumeratorType Type;
};

template <RSNodeKind Kind, class Enable = void>
struct DeclType;

template <RSNodeKind Kind>
struct DeclType<Kind, typename std::enable_if<NodeTraits::isKDevDeclaration(Kind)>::type>
{
    typedef KDevelop::Declaration Type;
};

template <RSNodeKind Kind>
struct DeclType<Kind, typename std::enable_if<Kind == StructDecl || Kind == TraitDecl || Kind == ImplDecl>::type>
{
    typedef KDevelop::ClassDeclaration Type;
};

template <RSNodeKind Kind>
struct DeclType<Kind, typename std::enable_if<Kind == TypeAliasDecl>::type>
{
    typedef KDevelop::AliasDeclaration Type;
};

// FIXME: specialize
//template <RSNodeKind Kind>
//struct DeclType<Kind, typename std::enable_if<Kind == FunctionDecl>::type>
//{
//    typedef KDevelop::ClassFunctionDeclaration Type;
//};

template <RSNodeKind Kind>
struct DeclType<Kind, typename std::enable_if<Kind == FunctionDecl>::type>
{
    typedef KDevelop::FunctionDeclaration Type;
};

template <RSNodeKind Kind>
struct DeclType<Kind, typename std::enable_if<Kind == FieldDecl>::type>
{
    typedef KDevelop::ClassMemberDeclaration Type;
};

}

#endif // DECLARATIONTYPES_H
