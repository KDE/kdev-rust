#include "completionmodel.h"

#include "completionworker.h"

namespace Rust
{

CompletionModel::CompletionModel(QObject *parent)
    : KDevelop::CodeCompletionModel(parent)
{
}

KDevelop::CodeCompletionWorker *CompletionModel::createCompletionWorker()
{
    return new CompletionWorker(this);
}

}
