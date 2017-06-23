#include "rustparsejob.h"

#include "rustdebug.h"

#include <serialization/indexedstring.h>
#include <interfaces/icore.h>
#include <language/interfaces/ilanguagesupport.h>
#include <language/backgroundparser/urlparselock.h>

#include <QReadLocker>

//#include "parser/parsesession.cpp"
#include "duchain/astredux.h"
#include "duchain/builder.h"

using namespace KDevelop;

namespace Rust
{

ParseJob::ParseJob(const IndexedString &url, ILanguageSupport *languageSupport)
    : KDevelop::ParseJob(url, languageSupport)
{

}

void ParseJob::run(ThreadWeaver::JobPointer self, ThreadWeaver::Thread *thread)
{
    if (abortRequested() || ICore::self()->shuttingDown()) {
        return abortJob();
    }

    qCDebug(KDEV_RUST) << "Parse job starting for: " << document().toUrl();

    QReadLocker parseLock(languageSupport()->parseLock());
    UrlParseLock urlLock(document());

    readContents();

    qCDebug(KDEV_RUST) << "source:" << contents().contents;
    RSIndex *index = create_index();
    RSCrate *crate = parse_crate("", contents().contents, index);

    Builder::buildDUChain(crate);

    destroy_crate(crate);
    destroy_index(index);

    qCDebug(KDEV_RUST) << "Parse job finished for: " << document().toUrl();
}

}
