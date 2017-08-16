#include "rusthighlighting.h"
#include <language/duchain/declaration.h>

using namespace KDevelop;

namespace Rust
{

class HighlightingInstance : public CodeHighlightingInstance
{
public:
    HighlightingInstance(const Highlighting *highlighting);
    ~HighlightingInstance() override;

    bool useRainbowColor(Declaration *dec) const override;
};

HighlightingInstance::HighlightingInstance(const Highlighting *highlighting)
    : CodeHighlightingInstance(highlighting)
{
}

HighlightingInstance::~HighlightingInstance()
{
}

bool HighlightingInstance::useRainbowColor(Declaration *dec) const
{
    return dec->context()->type() == DUContext::Function || dec->context()->type() == DUContext::Other;
}

Highlighting::Highlighting(QObject *parent)
    : CodeHighlighting(parent)
{
}

CodeHighlightingInstance *Highlighting::createInstance() const
{
    return new HighlightingInstance(this);
}

}
