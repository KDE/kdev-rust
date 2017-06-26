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
    ~LanguageSupport();

    QString name() const override;
    KDevelop::ParseJob* createParseJob(const KDevelop::IndexedString &url) override;

    virtual KDevelop::ICodeHighlighting *codeHighlighting() const;

    RSIndex *index();

private:
    Highlighting* m_highlighting;

    RSIndex *m_index;
};

}

#endif // KDEVRUSTLANGUAGESUPPORT_H
