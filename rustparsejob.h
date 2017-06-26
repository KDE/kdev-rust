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
    ParseJob(const KDevelop::IndexedString &url, KDevelop::ILanguageSupport *languageSupport);

protected:
    void run(ThreadWeaver::JobPointer self, ThreadWeaver::Thread *thread) override;

private:
    LanguageSupport *rust() const;
    ParseSessionData::Ptr findParseSessionData(const KDevelop::IndexedString &url);
};

}

#endif // RUSTPARSEJOB_H