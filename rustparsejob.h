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

#ifndef RUSTPARSEJOB_H
#define RUSTPARSEJOB_H

#include <serialization/indexedstring.h>
#include <language/backgroundparser/parsejob.h>
#include <language/duchain/duchainpointer.h>
#include <language/duchain/topducontext.h>
#include <language/duchain/problem.h>

#include "duchain/parsesession.h"

namespace Rust
{

class LanguageSupport;

class ParseJob : public KDevelop::ParseJob
{
    Q_OBJECT
public:
    enum {
        Rescheduled = (KDevelop::TopDUContext::LastFeature << 1),
    };

    ParseJob(const KDevelop::IndexedString &url, KDevelop::ILanguageSupport *languageSupport);

protected:
    void run(ThreadWeaver::JobPointer self, ThreadWeaver::Thread *thread) override;

private:
    LanguageSupport *rust() const;
    ParseSessionData::Ptr findParseSessionData(const KDevelop::IndexedString &url);
};

}

#endif // RUSTPARSEJOB_H
