#include "completionworker.h"
#include "completionmodel.h"
#include "completioncontext.h"

namespace Rust
{

CompletionWorker::CompletionWorker(CompletionModel *parent)
    : KDevelop::CodeCompletionWorker(parent)
{
}

KDevelop::CodeCompletionContext *CompletionWorker::createCompletionContext(KDevelop::DUContextPointer context,
                                                                           const QString &contextText,
                                                                           const QString &followingText,
                                                                           const KDevelop::CursorInRevision &position) const
{
    if (!context) {
        return nullptr;
    }

    return new CompletionContext(context, contextText, followingText, position, 0);
}

}
