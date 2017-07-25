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
    case StructDecl:
        return buildDeclaration<StructDecl>(node, parent);
    case EnumDecl:
        return buildDeclaration<EnumDecl>(node, parent);
    case FunctionDecl:
        return buildDeclaration<FunctionDecl>(node, parent);
    case ImplDecl:
        return buildDeclaration<ImplDecl>(node, parent);
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
        return Recurse;
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

    Declaration *decl = openDeclaration<typename DeclType<Kind>::Type>(Identifier(name->value), range,
                                                                       hasContext ? DeclarationIsDefinition : NoFlags);
    auto type = createType<Kind>(node);
    openType(type);

    if (Kind == TypeAliasDecl) {
        decl->setIsTypeAlias(true);
    }

    if (NodeTraits::isTypeDeclaration(Kind)) {
        decl->setKind(Declaration::Type);
    }

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
    return AbstractType::Ptr(nullptr);
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

template<RSNodeKind Kind>
void DeclarationBuilder::setDeclData(Declaration *decl)
{
}

}
