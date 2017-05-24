#include "rustlanguagesupport.h"

#include <rustdebug.h>

#include <KPluginFactory>

K_PLUGIN_FACTORY_WITH_JSON(KDevRustSupportFactory, "kdevrustsupport.json", registerPlugin<Rust::LanguageSupport>(); )

namespace Rust
{
    LanguageSupport::LanguageSupport(QObject *parent, const QVariantList& args)
        : KDevelop::IPlugin(QStringLiteral("kdevrustsupport"), parent),
          KDevelop::ILanguageSupport()
    {
        Q_UNUSED(args);

        qCDebug(KDEV_RUST) << "Hello world, Rust plugin initialized is loaded!";
    }

    QString LanguageSupport::name() const
    {
        return "Rust Language Support";
    }

    KDevelop::ParseJob *LanguageSupport::createParseJob(const KDevelop::IndexedString &url)
    {
        return nullptr;
    }

}

#include "rustlanguagesupport.moc"
