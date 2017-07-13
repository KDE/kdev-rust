#include "completioncontext.h"

#include "rustdebug.h"

namespace Rust
{

using namespace KDevelop;

CompletionContext::CompletionContext(DUContextPointer context,
                                     const QString &contextText,
                                     const QString &followingText,
                                     const CursorInRevision &position,
                                     int depth)
    : KDevelop::CodeCompletionContext(context, contextText, position, depth),
      m_followingText(followingText)
{

}

QList<CompletionTreeItemPointer> CompletionContext::completionItems(bool &abort, bool fullCompletion)
{
    qCDebug(KDEV_RUST) << "Hello world, no completions yet";

    return QList<CompletionTreeItemPointer>();
}

}
