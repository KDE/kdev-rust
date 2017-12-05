#ifndef DECLARATIONBUILDER_H
#define DECLARATIONBUILDER_H

#include <type_traits>

#include <language/duchain/builders/abstracttypebuilder.h>
#include <language/duchain/builders/abstractdeclarationbuilder.h>
#include <language/duchain/types/alltypes.h>

#include "types/declarationtypes.h"
#include "contextbuilder.h"
#include "nodetraits.h"
#include "rustnode.h"
#include "astredux.h"

#include "kdevrustduchain_export.h"

namespace Rust
{

using TypeBuilderBase = KDevelop::AbstractTypeBuilder<RustNode, RustPath, ContextBuilder>;
using DeclarationBuilderBase = KDevelop::AbstractDeclarationBuilder<RustNode, RustPath, TypeBuilderBase>;

namespace detail { enum class enabler {}; }
constexpr detail::enabler dummy = {};
template <bool Condition>
using EnableIf = typename std::enable_if<Condition, detail::enabler>::type;

class KDEVRUSTDUCHAIN_EXPORT DeclarationBuilder : public DeclarationBuilderBase
{
public:
    DeclarationBuilder() = default;
    ~DeclarationBuilder() override = default;

protected:
    RSVisitResult visitNode(RustNode *node, RustNode *parent) override;

private:
    template <RSNodeKind Kind>
    RSVisitResult buildDeclaration(RustNode *node, RustNode *parent);

    template <RSNodeKind Kind>
    KDevelop::Declaration *createDeclaration(RustNode *node, RustPath *name, bool hasContext);

    template <RSNodeKind Kind, EnableIf<NodeTraits::isTypeDeclaration(Kind)> = dummy>
    typename IdType<Kind>::Type::Ptr createType(RustNode *node);

    template <RSNodeKind Kind, EnableIf<Kind == FunctionDecl> = dummy>
    KDevelop::FunctionType::Ptr createType(RustNode *node);

    template <RSNodeKind Kind, EnableIf<!NodeTraits::isTypeDeclaration(Kind) && Kind != FunctionDecl> = dummy>
    KDevelop::AbstractType::Ptr createType(RustNode *node);

    template <RSNodeKind Kind, EnableIf<Kind != VarDecl && Kind != Module> = dummy>
    void setDeclData(KDevelop::Declaration *decl);

    template<RSNodeKind Kind, EnableIf<Kind == VarDecl> = dummy>
    void setDeclData(KDevelop::Declaration *decl);

    template<RSNodeKind Kind, EnableIf<Kind == Module> = dummy>
    void setDeclData(KDevelop::Declaration *decl);

    template<RSNodeKind Kind, EnableIf<Kind == TypeAliasDecl> = dummy>
    void setDeclData(KDevelop::AliasDeclaration *decl);

    template <RSNodeKind Kind, EnableIf<NodeTraits::isTypeDeclaration(Kind)> = dummy>
    void setDeclData(KDevelop::ClassDeclaration *decl);

    template <RSNodeKind Kind>
    void setType(KDevelop::Declaration *decl, typename IdType<Kind>::Type *type);

    template <RSNodeKind Kind>
    void setType(KDevelop::Declaration *decl, KDevelop::IdentifiedType *type);

    template <RSNodeKind Kind>
    void setType(KDevelop::Declaration *decl, KDevelop::AbstractType *type);
};

}

#endif // DECLARATIONBUILDER_H
