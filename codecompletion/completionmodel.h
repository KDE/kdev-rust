#ifndef COMPLETIONMODEL_H
#define COMPLETIONMODEL_H

#include <language/codecompletion/codecompletionmodel.h>

#include "kdevrustcompletion_export.h"

namespace Rust
{

class KDEVRUSTCOMPLETION_EXPORT CompletionModel : public KDevelop::CodeCompletionModel
{
    Q_OBJECT
public:
    CompletionModel(QObject *parent);

protected:
    KDevelop::CodeCompletionWorker *createCompletionWorker() override;
};

}

#endif // COMPLETIONMODEL_H
