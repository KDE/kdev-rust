#ifndef KDEVRUSTLANGUAGESUPPORT_H
#define KDEVRUSTLANGUAGESUPPORT_H

#include <interfaces/iplugin.h>
#include <interfaces/ilanguagecheck.h>
#include <interfaces/ilanguagecheckprovider.h>
#include <language/interfaces/ilanguagesupport.h>

#include <QVariant>

namespace Rust
{

class LanguageSupport : public KDevelop::IPlugin
        , public KDevelop::ILanguageSupport
{
    Q_OBJECT
    Q_INTERFACES( KDevelop::ILanguageSupport )

public:
    // KPluginFactory-based plugin wants constructor with this signature
    LanguageSupport( QObject* parent, const QVariantList& args = QVariantList() );

    QString name() const override;
    KDevelop::ParseJob *createParseJob(const KDevelop::IndexedString &url) override;
};

}

#endif // KDEVRUSTLANGUAGESUPPORT_H
