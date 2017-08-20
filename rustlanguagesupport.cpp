#include "rustlanguagesupport.h"

#include <rustdebug.h>

#include <interfaces/icore.h>
#include <interfaces/idocument.h>
#include <interfaces/idocumentcontroller.h>
#include <interfaces/ilanguagecontroller.h>
#include <language/duchain/duchainlock.h>
#include <language/codecompletion/codecompletion.h>

#include <KPluginFactory>
#include <KPluginLoader>

#include <QReadWriteLock>
#include <QStandardPaths>

#include "rustparsejob.h"
#include "codecompletion/completionmodel.h"

K_PLUGIN_FACTORY_WITH_JSON(KDevRustSupportFactory, "kdevrustsupport.json", registerPlugin<Rust::LanguageSupport>(); )

using namespace KDevelop;

namespace Rust
{

LanguageSupport::LanguageSupport(QObject *parent, const QVariantList &args)
    : KDevelop::IPlugin(QStringLiteral("kdevrustsupport"), parent),
      KDevelop::ILanguageSupport(),
      m_highlighting(new Highlighting(this))
{
    Q_UNUSED(args);

    new CodeCompletion(this, new CompletionModel(this), name());
}

LanguageSupport::~LanguageSupport()
{
    parseLock()->lockForWrite();
    parseLock()->unlock();

    delete m_highlighting;
}

QString LanguageSupport::name() const
{
    return "Rust";
}

KDevelop::ParseJob *LanguageSupport::createParseJob(const KDevelop::IndexedString &url)
{
    return new ParseJob(url, this);
}

ICodeHighlighting *LanguageSupport::codeHighlighting() const
{
    return m_highlighting;
}

SourceFormatterItemList LanguageSupport::sourceFormatterItems() const
{
    SourceFormatterStyle rustFormatter("rustfmt");
    rustFormatter.setCaption("rustfmt");
    rustFormatter.setDescription(i18n("Format source with rustfmt."));
    rustFormatter.setMimeTypes(SourceFormatterStyle::MimeList {
        SourceFormatterStyle::MimeHighlightPair { "text/rust", "Rust" },
        SourceFormatterStyle::MimeHighlightPair { "text/x-rust", "Rust" }
    });

    QString rustfmtPath = QStandardPaths::findExecutable("rustfmt");
    if (rustfmtPath.isEmpty()) {
        qCDebug(KDEV_RUST) << "Could not find the rustfmt executable";
        rustfmtPath = "/usr/bin/rustfmt";
    }
    rustFormatter.setContent(rustfmtPath + " --skip-children $TMPFILE");

    return SourceFormatterItemList { SourceFormatterStyleItem { "customscript", rustFormatter } };
}

}

#include "rustlanguagesupport.moc"
