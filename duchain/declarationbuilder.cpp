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

#include "declarationbuilder.h"

#include <type_traits>

#include <language/duchain/duchainlock.h>

#include "types/declarationtypes.h"
#include "nodetraits.h"
#include "rustdebug.h"

namespace Rust
{

using namespace KDevelop;

RSVisitResult DeclarationBuilder::visitNode(RustNode *node, RustNode *parent)
{
    RSNodeKind kind = node_get_kind(node->data());

    switch (kind) {
    case Module:
        return buildDeclaration<Module>(node, parent);
    case StructDecl:
        return buildDeclaration<StructDecl>(node, parent);
    case EnumDecl:
        return buildDeclaration<EnumDecl>(node, parent);
    case FunctionDecl:
        return buildDeclaration<FunctionDecl>(node, parent);
    case TraitDecl:
        return buildDeclaration<TraitDecl>(node, parent);
    case TypeAliasDecl:
        return buildDeclaration<TypeAliasDecl>(node, parent);
    case EnumVariantDecl:
        return buildDeclaration<EnumVariantDecl>(node, parent);
    case FieldDecl:
        return buildDeclaration<FieldDecl>(node, parent);
    case VarDecl:
        return buildDeclaration<VarDecl>(node, parent);
    default:
        return ContextBuilder::visitNode(node, parent);
    }
}

template<RSNodeKind Kind>
RSVisitResult DeclarationBuilder::buildDeclaration(RustNode *node, RustNode *parent)
{
    Q_UNUSED(parent);
    constexpr bool hasContext = NodeTraits::hasContext(Kind);

    RustPath name(node);

    createDeclaration<Kind>(node, &name, hasContext);
    RSVisitResult ret = buildContext<Kind>(node, parent);
    if (hasContext) eventuallyAssignInternalContext();
    closeDeclaration();
    return ret;
}

template <RSNodeKind Kind>
Declaration *DeclarationBuilder::createDeclaration(RustNode *node, RustPath *name, bool hasContext)
{
    auto range = editorFindSpellingRange(node, name->value);

    typename DeclType<Kind>::Type *decl = openDeclaration<typename DeclType<Kind>::Type>(Identifier(name->value), range,
                                                                                         hasContext ? DeclarationIsDefinition : NoFlags);

    if (NodeTraits::isTypeDeclaration(Kind)) {
        decl->setKind(Declaration::Type);
    }

    auto type = createType<Kind>(node);
    openType(type);

    setDeclData<Kind>(decl);
    setType<Kind>(decl, type.data());
    closeType();

    return decl;
}

template <RSNodeKind Kind, EnableIf<NodeTraits::isTypeDeclaration(Kind)>>
typename IdType<Kind>::Type::Ptr DeclarationBuilder::createType(RustNode *node)
{
    Q_UNUSED(node);
    return typename IdType<Kind>::Type::Ptr(new typename IdType<Kind>::Type);
}

template <RSNodeKind Kind, EnableIf<Kind == FunctionDecl>>
FunctionType::Ptr DeclarationBuilder::createType(RustNode *node)
{
    Q_UNUSED(node);
    return FunctionType::Ptr(new FunctionType);
}

template <RSNodeKind Kind, EnableIf<!NodeTraits::isTypeDeclaration(Kind) && Kind != FunctionDecl>>
AbstractType::Ptr DeclarationBuilder::createType(RustNode *node)
{
    Q_UNUSED(node);
    return AbstractType::Ptr(new IntegralType(IntegralType::TypeMixed));
}

template <RSNodeKind Kind>
void DeclarationBuilder::setType(Declaration *decl, typename IdType<Kind>::Type *type)
{
    setType<Kind>(decl, static_cast<IdentifiedType *>(type));
    setType<Kind>(decl, static_cast<AbstractType *>(type));
}

template <RSNodeKind Kind>
void DeclarationBuilder::setType(Declaration *decl, IdentifiedType *type)
{
    type->setDeclaration(decl);
}

template <RSNodeKind Kind>
void DeclarationBuilder::setType(Declaration *decl, AbstractType *type)
{
    decl->setAbstractType(AbstractType::Ptr(type));
}

template<RSNodeKind Kind, EnableIf<NodeTraits::isTypeDeclaration(Kind)>>
void DeclarationBuilder::setDeclData(ClassDeclaration *decl)
{
    if (Kind == StructDecl || Kind == ImplDecl) {
        decl->setClassType(ClassDeclarationData::Struct);
    } else if (Kind == TraitDecl) {
        decl->setClassType(ClassDeclarationData::Trait);
    }
}

template<RSNodeKind Kind, EnableIf<Kind == Module>>
void DeclarationBuilder::setDeclData(Declaration *decl)
{
    decl->setKind(Declaration::Namespace);
}

template<RSNodeKind Kind, EnableIf<Kind == VarDecl>>
void DeclarationBuilder::setDeclData(Declaration *decl)
{
    decl->setKind(Declaration::Instance);
}

template<RSNodeKind Kind, EnableIf<Kind == TypeAliasDecl>>
void DeclarationBuilder::setDeclData(AliasDeclaration *decl)
{
    decl->setIsTypeAlias(true);
    decl->setKind(Declaration::Type);
}

template<RSNodeKind Kind, EnableIf<Kind != VarDecl && Kind != Module>>
void DeclarationBuilder::setDeclData(Declaration *decl)
{
    Q_UNUSED(decl);
}

}
