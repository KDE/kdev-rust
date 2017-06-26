#include "rustlanguagesupport.h"

#include <rustdebug.h>

#include <interfaces/icore.h>
#include <interfaces/idocument.h>
#include <interfaces/idocumentcontroller.h>
#include <interfaces/ilanguagecontroller.h>
#include <language/duchain/duchainlock.h>

#include <KPluginFactory>
#include <KPluginLoader>

#include <QReadWriteLock>

#include "rustparsejob.h"

K_PLUGIN_FACTORY_WITH_JSON(KDevRustSupportFactory, "kdevrustsupport.json", registerPlugin<Rust::LanguageSupport>(); )

using namespace KDevelop;

namespace Rust
{

LanguageSupport::LanguageSupport(QObject *parent, const QVariantList& args)
    : KDevelop::IPlugin(QStringLiteral("kdevrustsupport"), parent),
      KDevelop::ILanguageSupport(),
      m_highlighting(new Highlighting(this))
{
    Q_UNUSED(args);
    m_index = create_index();

    qCDebug(KDEV_RUST) << "Hello world, Rust plugin initialized is loaded!";
}

LanguageSupport::~LanguageSupport()
{
    parseLock()->lockForWrite();
    parseLock()->unlock();

    destroy_index(m_index);

    delete m_highlighting;
}

QString LanguageSupport::name() const
{
    return "Rust Language Support";
}

KDevelop::ParseJob* LanguageSupport::createParseJob(const KDevelop::IndexedString &url)
{
    return new ParseJob(url, this);
}

ICodeHighlighting *LanguageSupport::codeHighlighting() const
{
    return m_highlighting;
}

RSIndex *LanguageSupport::index()
{
    return m_index;
}

}

#include "rustlanguagesupport.moc"