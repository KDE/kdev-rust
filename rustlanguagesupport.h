#ifndef KDEVRUSTLANGUAGESUPPORT_H
#define KDEVRUSTLANGUAGESUPPORT_H

#include <interfaces/iplugin.h>
#include <interfaces/idocument.h>
#include <interfaces/ilanguagecheck.h>
#include <interfaces/ilanguagecheckprovider.h>
#include <language/interfaces/ilanguagesupport.h>

#include <QVariant>

#include "rusthighlighting.h"
#include "duchain/astredux.h"

namespace Rust
{

class LanguageSupport
        : public KDevelop::IPlugin
        , public KDevelop::ILanguageSupport
{
    Q_OBJECT
    Q_INTERFACES( KDevelop::ILanguageSupport )

public:
    LanguageSupport(QObject* parent, const QVariantList& args = QVariantList());
    ~LanguageSupport() override;

    QString name() const override;
    KDevelop::ParseJob* createParseJob(const KDevelop::IndexedString &url) override;

    KDevelop::ICodeHighlighting *codeHighlighting() const override;

private:
    Highlighting* m_highlighting;
};

}

#endif // KDEVRUSTLANGUAGESUPPORT_H
