#ifndef COMPLETIONWORKER_H
#define COMPLETIONWORKER_H

#include <language/codecompletion/codecompletionworker.h>

namespace Rust
{

class CompletionModel;

class CompletionWorker : public KDevelop::CodeCompletionWorker
{
    Q_OBJECT
public:
    CompletionWorker(CompletionModel *parent);

protected:
    KDevelop::CodeCompletionContext *createCompletionContext(KDevelop::DUContextPointer context,
                                                             const QString &contextText,
                                                             const QString &followingText,
                                                             const KDevelop::CursorInRevision &position) const override;
};

}

#endif // COMPLETIONWORKER_H
