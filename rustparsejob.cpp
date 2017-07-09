#include "rustparsejob.h"

#include <serialization/indexedstring.h>
#include <interfaces/icore.h>
#include <language/interfaces/ilanguagesupport.h>
#include <language/backgroundparser/urlparselock.h>
#include <language/duchain/duchainlock.h>
#include <language/duchain/duchainutils.h>

#include <QReadLocker>

#include "duchain/parsesession.h"
#include "duchain/astredux.h"
#include "duchain/declarationbuilder.h"
#include "duchain/usebuilder.h"

#include "rustlanguagesupport.h"
#include "rustdebug.h"

using namespace KDevelop;

namespace Rust
{

ParseJob::ParseJob(const IndexedString &url, ILanguageSupport *languageSupport)
    : KDevelop::ParseJob(url, languageSupport)
{

}

LanguageSupport *ParseJob::rust() const
{
    return static_cast<LanguageSupport *>(languageSupport());
}

ParseSessionData::Ptr ParseJob::findParseSessionData(const IndexedString &url)
{
    DUChainReadLocker lock;
    auto context = KDevelop::DUChainUtils::standardContextForUrl(url.toUrl());

    if (context) {
        return ParseSessionData::Ptr(dynamic_cast<ParseSessionData *>(context->ast().data()));
    }

    return {};
}

void ParseJob::run(ThreadWeaver::JobPointer self, ThreadWeaver::Thread *thread)
{
    if (abortRequested() || ICore::self()->shuttingDown()) {
        return abortJob();
    }

    qCDebug(KDEV_RUST) << "Parse job starting for: " << document().toUrl();

    QReadLocker parseLock(languageSupport()->parseLock());

    {
        UrlParseLock urlLock(document());
        readContents();
    }

    if (abortRequested()) {
        return;
    }

    ParseSession session(findParseSessionData(document()));

    if (!session.data()) {
        session.setData(ParseSessionData::Ptr(new ParseSessionData(document(), contents().contents)));
    }

    if (abortRequested()) {
        return;
    }

    ReferencedTopDUContext toUpdate = nullptr;
    {
        DUChainReadLocker lock;
        toUpdate = DUChainUtils::standardContextForUrl(document().toUrl());
    }
    if (toUpdate) {
        translateDUChainToRevision(toUpdate);
        toUpdate->setRange(RangeInRevision(0, 0, INT_MAX, INT_MAX));
    }

    session.parse();

    RustOwnedNode crateNode = RustOwnedNode(node_from_crate(session.crate()));
    RustNode node(crateNode);

    DeclarationBuilder builder;
    auto context = builder.build(document(), &node, toUpdate);

    setDuChain(context);

    UseBuilder uses;
    uses.buildUses(&node);

    if (abortRequested()) {
        return;
    }

    if (context) {
        if (minimumFeatures() & TopDUContext::AST) {
            DUChainWriteLocker lock;
            context->setAst(IAstContainer::Ptr(session.data()));
        }

        highlightDUChain();
    }
    qCDebug(KDEV_RUST) << "Parse job finished for: " << document().toUrl();
}

}
