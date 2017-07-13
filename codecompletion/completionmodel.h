#ifndef COMPLETIONMODEL_H
#define COMPLETIONMODEL_H

#include <language/codecompletion/codecompletionmodel.h>

namespace Rust
{

class CompletionModel : public KDevelop::CodeCompletionModel
{
    Q_OBJECT
public:
    CompletionModel(QObject *parent);

protected:
    KDevelop::CodeCompletionWorker *createCompletionWorker() override;
};

}

#endif // COMPLETIONMODEL_H
