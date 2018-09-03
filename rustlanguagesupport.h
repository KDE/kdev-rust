/*
 * Copyright 2017  Emma Gospodinova <emma.gospodinova@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

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
    LanguageSupport(QObject *parent, const QVariantList &args = QVariantList());
    ~LanguageSupport() override;

    QString name() const override;
    KDevelop::ParseJob *createParseJob(const KDevelop::IndexedString &url) override;

    KDevelop::ICodeHighlighting *codeHighlighting() const override;

    KDevelop::SourceFormatterItemList sourceFormatterItems() const override;

private:
    Highlighting *m_highlighting;
};

}

#endif // KDEVRUSTLANGUAGESUPPORT_H
