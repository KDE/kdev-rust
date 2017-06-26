#include "builder.h"

#include <QScopedValueRollback>

#include <language/duchain/duchainlock.h>
#include <language/duchain/topducontext.h>
#include <language/duchain/classdeclaration.h>
#include <language/duchain/functiondeclaration.h>
#include <language/duchain/classfunctiondeclaration.h>
#include <language/duchain/types/abstracttype.h>
#include <language/duchain/types/identifiedtype.h>
#include <language/duchain/types/structuretype.h>
#include <language/duchain/types/typealiastype.h>
#include <language/duchain/types/enumerationtype.h>
#include <language/duchain/types/enumeratortype.h>
#include <language/duchain/types/functiontype.h>
#include <language/duchain/types/identifiedtype.h>
#include <language/duchain/duchain.h>
#include <language/duchain/problem.h>
#include <language/duchain/parsingenvironment.h>
#include <language/backgroundparser/urlparselock.h>
#include <interfaces/idocumentcontroller.h>
#include <interfaces/icore.h>
#include <interfaces/idocument.h>
#include <KTextEditor/Document>

#include "rustdebug.h"
#include "astredux.h"
#include "duchain/parsesession.h"
#include "duchain/rustducontext.h"
#include "duchain/nodetraits.h"

namespace Rust
{

RSVisitResult visitCallback(RSNode *node, RSNode *parent, void *data);

namespace detail { enum class enabler {}; }
constexpr detail::enabler dummy = {};
template <bool Condition>
using EnableIf = typename std::enable_if<Condition, detail::enabler>::type;

template<RSNodeKind Kind, class Enable = void>
struct IdType;

template<RSNodeKind Kind>
struct IdType<Kind, typename std::enable_if<Kind == StructDecl || Kind == TraitDecl || Kind == ImplDecl>::type>
{
    typedef StructureType Type;
};

template<RSNodeKind Kind>
struct IdType<Kind, typename std::enable_if<Kind == TypeAliasDecl>::type>
{
    typedef TypeAliasType Type;
};

template<RSNodeKind Kind>
struct IdType<Kind, typename std::enable_if<Kind == EnumDecl>::type>
{
    typedef EnumerationType Type;
};

template<RSNodeKind Kind>
struct IdType<Kind, typename std::enable_if<Kind == EnumVariantDecl>::type>
{
    typedef EnumeratorType Type;
};

template<RSNodeKind Kind, class Enable = void>
struct DeclType;

template<RSNodeKind Kind>
struct DeclType<Kind, typename std::enable_if<NodeTraits::isKDevDeclaration(Kind)>::type>
{
    typedef Declaration Type;
};

template<RSNodeKind Kind>
struct DeclType<Kind, typename std::enable_if<Kind == StructDecl || Kind == TraitDecl || Kind == ImplDecl>::type>
{
    typedef ClassDeclaration Type;
};

// FIXME: specialize
//template<RSNodeKind Kind>
//struct DeclType<Kind, typename std::enable_if<Kind == FunctionDecl>::type>
//{
//    typedef ClassFunctionDeclaration Type;
//};

template<RSNodeKind Kind>
struct DeclType<Kind, typename std::enable_if<Kind == FunctionDecl>::type>
{
    typedef FunctionDeclaration Type;
};

template<RSNodeKind Kind>
struct DeclType<Kind, typename std::enable_if<Kind == FieldDecl>::type>
{
    typedef ClassMemberDeclaration Type;
};

class DUChainBuilder
{
public:
    DUChainBuilder(DUContext *topContext, RSIndex *index, RSCrate *crate, bool update);
    ~DUChainBuilder();

    template <RSNodeKind Kind>
    RSVisitResult buildDeclaration(RSNode *node, RSNode *parent);

private:
    template <RSNodeKind Kind>
    DUContext *createContext(RSNode *node, const QualifiedIdentifier& scopeId = {});

    template <RSNodeKind Kind>
    Declaration *createDeclaration(RSNode *node, const Identifier& id, DUContext *context);

    RangeInRevision toRangeInRevision(RSRange range);
    RangeInRevision correctedSpellingRange(RSRange range, const Identifier& identifier);

    template <RSNodeKind Kind, EnableIf<NodeTraits::isTypeDeclaration(Kind)> = dummy>
    typename IdType<Kind>::Type *createType(RSNode *node);

    template <RSNodeKind Kind, EnableIf<Kind == FunctionDecl> = dummy>
    FunctionType *createType(RSNode *node);

    template <RSNodeKind Kind, EnableIf<!NodeTraits::isTypeDeclaration(Kind) && Kind != FunctionDecl> = dummy>
    AbstractType *createType(RSNode *node);

    template <RSNodeKind Kind>
    void setDeclData(Declaration *decl);

    template <RSNodeKind Kind, EnableIf<NodeTraits::isTypeDeclaration(Kind)> = dummy>
    void setDeclData(ClassDeclaration *decl);

    template <RSNodeKind Kind>
    void setType(Declaration *decl, typename IdType<Kind>::Type *type);

    template <RSNodeKind Kind>
    void setType(Declaration *decl, IdentifiedType *type);

    template <RSNodeKind Kind>
    void setType(Declaration *decl, AbstractType *type);

private:
    DUContext *m_parentContext;

    RSIndex *index;
    RSCrate *crate;
    RSNode *crateNode;
    bool m_update;
};

DUChainBuilder::DUChainBuilder(DUContext *topContext, RSIndex *index, RSCrate *crate, bool update)
    : m_parentContext(topContext),
      index(index),
      crate(crate),
      crateNode(node_from_crate(crate)),
      m_update(update)
{
    visit_children(crateNode, visitCallback, this);
}

DUChainBuilder::~DUChainBuilder()
{
    destroy_node(crateNode);
}

RangeInRevision DUChainBuilder::toRangeInRevision(RSRange range)
{
    return RangeInRevision(range.start.line - 1, range.start.column, range.end.line - 1, range.end.column);
}

RangeInRevision DUChainBuilder::correctedSpellingRange(RSRange range, const Identifier& identifier)
{
    KTextEditor::Range incorrectRange = KTextEditor::Range(range.start.line - 1, range.start.column, INT_MAX, INT_MAX);
    IDocument *document = ICore::self()->documentController()
            ->documentForUrl(m_parentContext->topContext()->url().toUrl());

    QVector<KTextEditor::Range> ranges;
    if (document) {
        ranges = document->textDocument()->searchText(incorrectRange, identifier.toString());
    } else {
        ranges = { KTextEditor::Range::invalid() };
    }

    return RangeInRevision::castFromSimpleRange(ranges.first());
}

template<RSNodeKind Kind>
DUContext* DUChainBuilder::createContext(RSNode *node, const QualifiedIdentifier& scopeId)
{
    constexpr DUContext::ContextType contextType = NodeTraits::contextType(Kind);

    RSRange range = node_get_extent(node, index);
    DUChainWriteLocker lock;

    auto context = new RustNormalDUContext(toRangeInRevision(range), m_parentContext);
    context->setType(contextType);
    context->setLocalScopeIdentifier(scopeId);

    return context;
}

template<RSNodeKind Kind>
Declaration* DUChainBuilder::createDeclaration(RSNode *node, const Identifier& id, DUContext *context)
{
    auto range = correctedSpellingRange(node_get_spelling_range(node, index), id);

    auto decl = new typename DeclType<Kind>::Type(range, nullptr);
    auto type = createType<Kind>(node);
    decl->setIdentifier(id);

    if (Kind == TypeAliasDecl) {
        decl->setIsTypeAlias(true);
    }

    if (NodeTraits::isTypeDeclaration(Kind)) {
        decl->setKind(Declaration::Type);
    }

    DUChainWriteLocker lock;
    decl->setContext(m_parentContext);
    if (context) {
        decl->setInternalContext(context);
    }

    setDeclData<Kind>(decl);
    setType<Kind>(decl, type);

    return decl;
}

template<RSNodeKind Kind>
RSVisitResult DUChainBuilder::buildDeclaration(RSNode *node, RSNode *parent)
{
    constexpr bool hasContext = NodeTraits::hasContext(Kind);

    RustAllocatedString name(node_get_spelling_name(node));
    RSRange range = node_get_spelling_range(node, index);
    RSRange extent = node_get_extent(node, index);

    qCDebug(KDEV_RUST) << "DECLARATION:" << name << "; spelling range: ("
                       << range.start.line << ":" << range.start.column << "-"
                       << range.end.line << ":" << range.end.column << "); extent: ("
                       << extent.start.line << ":" << extent.start.column << "-"
                       << extent.end.line << ":" << extent.end.column << ")";

    if (hasContext) {
        auto context = createContext<Kind>(node, QualifiedIdentifier(name.data()));
        createDeclaration<Kind>(node, Identifier(name.data()), context);
        QScopedValueRollback<DUContext *> pushCurrent(m_parentContext, context);
        visit_children(node, &visitCallback, this);
        return Continue;
    }
    createDeclaration<Kind>(node, Identifier(name.data()), nullptr);
    return Recurse;
}

RSVisitResult visitCallback(RSNode *node, RSNode *parent, void *data)
{
    DUChainBuilder *builder = static_cast<DUChainBuilder *>(data);
    RSNodeKind kind = node_get_kind(node);

    switch (kind) {
    case StructDecl:
        return builder->buildDeclaration<StructDecl>(node, parent);
    case EnumDecl:
        return builder->buildDeclaration<EnumDecl>(node, parent);
    case FunctionDecl:
        return builder->buildDeclaration<FunctionDecl>(node, parent);
    case ImplDecl:
        return builder->buildDeclaration<ImplDecl>(node, parent);
    case TraitDecl:
        return builder->buildDeclaration<TraitDecl>(node, parent);
    case TypeAliasDecl:
        return builder->buildDeclaration<TypeAliasDecl>(node, parent);
    case EnumVariantDecl:
        return builder->buildDeclaration<EnumVariantDecl>(node, parent);
    case VarDecl:
//        return builder->buildDeclaration<VarDecl>(node, parent);
    default:
        return Recurse;
    }
}

void dumpDuContext(DUContext *context, int indent) {
    QString indentStr = QString(indent, ' ');
    QString typeName;
    switch (context->type()) {
    case DUContext::Global:    typeName = "Global";    break;
    case DUContext::Namespace: typeName = "Namespace"; break;
    case DUContext::Class:     typeName = "Class";     break;
    case DUContext::Function:  typeName = "Function";  break;
    case DUContext::Template:  typeName = "Template";  break;
    case DUContext::Enum:      typeName = "Enum";      break;
    case DUContext::Helper:    typeName = "Helper";    break;
    case DUContext::Other:     typeName = "Other";     break;
    }

    qCDebug(KDEV_RUST) << indentStr.toStdString().c_str() << "DU CONTEXT:" << context->localScopeIdentifier() << "TYPE:" << typeName;
    qCDebug(KDEV_RUST) << indentStr.toStdString().c_str() << "DECLARATIONS:";
    for (auto decl : context->localDeclarations()) {
        qCDebug(KDEV_RUST) << indentStr.toStdString().c_str() << "-" << decl->identifier();
    }

    qCDebug(KDEV_RUST) << indentStr.toStdString().c_str() << "CHILDREN:";
    for (auto child : context->childContexts()) {
        dumpDuContext(child, indent + 2);
    }
}

ReferencedTopDUContext Builder::buildDUChain(const ParseSession &parseSession, RSIndex *index)
{
    using namespace KDevelop;

    bool update = false;
    UrlParseLock urlLock(parseSession.document());
    ReferencedTopDUContext context;
    {
        DUChainWriteLocker lock;
        context = DUChain::self()->chainForDocument(parseSession.document());
        if (!context) {
            ParsingEnvironmentFile *file = new ParsingEnvironmentFile(parseSession.document());
            file->setLanguage(IndexedString("rust"));

            context = new Rust::RustTopDUContext(parseSession.document(), RangeInRevision(0, 0, INT_MAX, INT_MAX), file);

            DUChain::self()->addDocumentChain(context);
        } else {
            context->deleteChildContextsRecursively();
            context->deleteLocalDeclarations();
            update = true;
        }
    }

    DUChainBuilder builder(context, index, parseSession.crate(), update);

//    dumpDuContext(context, 0);

    DUChain::self()->emitUpdateReady(parseSession.document(), context);

    return context;
}

template <RSNodeKind Kind, EnableIf<NodeTraits::isTypeDeclaration(Kind)>>
typename IdType<Kind>::Type *DUChainBuilder::createType(RSNode *node)
{
    return new typename IdType<Kind>::Type;
}


template <RSNodeKind Kind, EnableIf<Kind == FunctionDecl>>
FunctionType *DUChainBuilder::createType(RSNode *node)
{
    return new FunctionType;
}

template <RSNodeKind Kind, EnableIf<!NodeTraits::isTypeDeclaration(Kind) && Kind != FunctionDecl>>
AbstractType *DUChainBuilder::createType(RSNode *node)
{
    return nullptr;
}

template <RSNodeKind Kind>
void DUChainBuilder::setType(Declaration *decl, typename IdType<Kind>::Type *type)
{
    setType<Kind>(decl, static_cast<IdentifiedType *>(type));
    setType<Kind>(decl, static_cast<AbstractType *>(type));
}

template <RSNodeKind Kind>
void DUChainBuilder::setType(Declaration *decl, IdentifiedType *type)
{
    type->setDeclaration(decl);
}

template <RSNodeKind Kind>
void DUChainBuilder::setType(Declaration *decl, AbstractType *type)
{
    decl->setAbstractType(AbstractType::Ptr(type));
}

template<RSNodeKind Kind, EnableIf<NodeTraits::isTypeDeclaration(Kind)>>
void DUChainBuilder::setDeclData(ClassDeclaration *decl)
{
    if (Kind == StructDecl || Kind == ImplDecl) {
        decl->setClassType(ClassDeclarationData::Struct);
    } else if (Kind == TraitDecl) {
        decl->setClassType(ClassDeclarationData::Trait);
    }
}

template<RSNodeKind Kind>
void DUChainBuilder::setDeclData(Declaration *decl)
{
}


}
